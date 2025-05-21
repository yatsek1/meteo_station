#include "gc9a01a.h"
#include "gc9a01a_initcmd.h"
#include "delays.h"
#include "usart.h"
#include <cstring>
#include "my_app.h"

volatile bool dmaTransferComplete = true;
static uint16_t lineBuf[512];

GC9A01A::GC9A01A(SPI_HandleTypeDef *hspi,
                 GPIO_TypeDef *cs_gpio, uint32_t cs_pin,
                 GPIO_TypeDef *dc_gpio, uint32_t dc_pin,
                 GPIO_TypeDef *bl_gpio, uint32_t bl_pin,
                 GPIO_TypeDef *rst_gpio, uint32_t rst_pin)
    : hspi_(hspi),
      cs_gpio_(cs_gpio), cs_pin_(cs_pin),
      dc_gpio_(dc_gpio), dc_pin_(dc_pin),
      bl_gpio_(bl_gpio), bl_pin_(bl_pin),
      rst_gpio_(rst_gpio), rst_pin_(rst_pin) {}

void GC9A01A::init() {
    HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_SET);
    HAL_GPIO_WritePin(dc_gpio_, dc_pin_, GPIO_PIN_SET);

    HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(rst_gpio_, rst_pin_, GPIO_PIN_SET);
    HAL_Delay(100);

    const uint8_t *addr = INIT_CMD;
    while (*addr > 0) {
        uint8_t cmd = *addr++;
        uint8_t x = *addr++;
        uint8_t numArgs = x & 0x7F;
        configureParam(cmd, (uint8_t *)addr, numArgs);
        addr += numArgs;
        if (x & 0x80) HAL_Delay(150);
    }

    HAL_GPIO_WritePin(bl_gpio_, bl_pin_, GPIO_PIN_SET);
}

void GC9A01A::configureParam(uint8_t command, uint8_t *data, uint8_t n_data) {
    startSpiTransaction();
    setSpiDataSize(SPI_DATASIZE_8BIT);
    transmitCommand(command);
    HAL_SPI_Transmit(hspi_, data, n_data, 100);
    endSpiTransaction();
}

void GC9A01A::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
    uint16_t x2 = x1 + w - 1;
    uint16_t y2 = y1 + h - 1;

    setSpiDataSize(SPI_DATASIZE_8BIT);
    transmitCommand(0x2A); // CASET
    setSpiDataSize(SPI_DATASIZE_16BIT);
    transmitData(1, &x1);
    transmitData(1, &x2);

    setSpiDataSize(SPI_DATASIZE_8BIT);
    transmitCommand(0x2B); // RASET
    setSpiDataSize(SPI_DATASIZE_16BIT);
    transmitData(1, &y1);
    transmitData(1, &y2);

    setSpiDataSize(SPI_DATASIZE_8BIT);
    transmitCommand(0x2C); // RAMWR
}

void GC9A01A::drawPixel(int16_t x, int16_t y, uint16_t color) {
    startSpiTransaction();
    setAddrWindow(x, y, 1, 1);
    setSpiDataSize(SPI_DATASIZE_16BIT);
    transmitData(1, &color);
    endSpiTransaction();
}

void GC9A01A::drawPixels(uint16_t x, uint16_t y, uint16_t *color, uint16_t width, uint16_t height) {
    startSpiTransaction();
    setAddrWindow(x, y, width, height);
    setSpiDataSize(SPI_DATASIZE_16BIT);
    for (size_t i = 0; i < height; ++i) {
        transmitData(width, color + i * width);
    }
    endSpiTransaction();
}

void GC9A01A::drawScreen(uint16_t *img) {
    drawPixels(0, 0, img, width_, height_);
}

//void GC9A01A::fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
//    startSpiTransaction();
//    setAddrWindow(x, y, width, height);
//    setSpiDataSize(SPI_DATASIZE_16BIT);
//    for (uint32_t i = 0; i < width * height; ++i) {
//        transmitColor(color);
//    }
//    endSpiTransaction();
//}

void GC9A01A::fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
    startSpiTransaction();
    setAddrWindow(x, y, width, height);
    setSpiDataSize(SPI_DATASIZE_16BIT);

    const uint32_t total_pixels = width * height;

    // Bufor tymczasowy – np. 240 pixeli = 480 bajtów
    static const uint16_t block_size = 240;
    uint8_t buf[block_size * 2];

    for (uint16_t i = 0; i < block_size; ++i) {
        buf[2 * i] = color >> 8;
        buf[2 * i + 1] = color & 0xFF;
    }

    uint32_t remaining = total_pixels;
    while (remaining > 0) {
        uint16_t to_send = (remaining > block_size) ? block_size : remaining;
        HAL_SPI_Transmit(hspi_, buf, to_send * 2, HAL_MAX_DELAY);
        remaining -= to_send;
    }

    endSpiTransaction();
}


void GC9A01A::drawCharGen(uint16_t x, uint16_t y, char c, uint16_t color, const fontStruct* font, uint8_t scale) {
    if (c < font->start || c > font->end) return;
    uint16_t index = (c - font->start) * font->width;
    for (int col = 0; col < font->width; col++) {
        uint32_t line = (font->height > 8)
            ? ((const uint16_t *)font->bitmap)[index + col]
            : ((const uint8_t *)font->bitmap)[index + col];
        for (int row = 0; row < font->height; row++) {
            if (line & (1 << row)) {
                for (int dx = 0; dx < scale; dx++) {
                    for (int dy = 0; dy < scale; dy++) {
                        drawPixel(x + col * scale + dx, y + row * scale + dy, color);
                    }
                }
            }
        }
    }
}

void GC9A01A::drawTextGen(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t background, const fontStruct* font, uint8_t scale, uint8_t pos) {
    size_t length = strlen(str);
    uint16_t text_length = length * font->width * scale + (length - 1) * 2;
    uint16_t text_height = font->height * scale;
    if (pos == 1) {
        x = (width_ - text_length) / 2;
    }
    fillRectDMA(x - 1, y - 1, text_length + 1, text_height + 1, background);
    while (*str) {
        drawCharGen(x, y, *str++, color, font, scale);
        x += font->width * scale + 2;
    }
}

void GC9A01A::drawCharToBuffer(uint16_t* buffer, uint16_t width, uint16_t height,
                               char c, uint16_t fgColor, uint16_t bgColor,
                               const fontStruct* font, uint8_t scale) {
    if (c < font->start || c > font->end) return;

    uint16_t index = (c - font->start) * font->width;
    for (int col = 0; col < font->width; col++) {
        uint32_t line = (font->height > 8)
            ? ((const uint16_t*)font->bitmap)[index + col]
            : ((const uint8_t*)font->bitmap)[index + col];

        for (int row = 0; row < font->height; row++) {
            uint16_t pixelColor = (line & (1 << row)) ? fgColor : bgColor;

            for (int dx = 0; dx < scale; dx++) {
                for (int dy = 0; dy < scale; dy++) {
                    int px = (col * scale + dx);
                    int py = (row * scale + dy);
                    if (px < width && py < height) {
                        buffer[py * width + px] = pixelColor;
                    }
                }
            }
        }
    }
}

void GC9A01A::drawTextGenFast(uint16_t x, uint16_t y, const char* str,
                              uint16_t color, uint16_t bgColor,
                              const fontStruct* font, uint8_t scale, uint8_t pos) {
    size_t len = strlen(str);
    uint16_t charW = font->width * scale + 2;
    uint16_t charH = font->height * scale;
    uint16_t totalW = len * charW;

    if (pos == 1) {
        x = (width_ - totalW) / 2;
    }

    uint16_t* buffer = new uint16_t[totalW * charH];
    if (!buffer) return;

    // Wypełnij tłem
    for (size_t i = 0; i < totalW * charH; i++) buffer[i] = bgColor;

    // Wypełnij tekstem
    for (size_t i = 0; i < len; i++) {
        drawCharToBuffer(buffer + i * charW, totalW, charH, str[i], color, bgColor, font, scale);
    }

    // Wyślij jednorazowo
    drawPixels(x, y, buffer, totalW, charH);  // możesz też użyć wersji z DMA

    delete[] buffer;
}

uint16_t GC9A01A::color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

void GC9A01A::swapBytes(size_t len, uint16_t *dest, uint16_t *src) {
    for (size_t i = 0; i < len; i++) {
        dest[i] = __builtin_bswap16(src[i]);
    }
}

void GC9A01A::setSpiDataSize(uint32_t) {
    // Nie wspierane w STM32F4 — ignorowane.
}

void GC9A01A::startSpiTransaction() {
    HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_RESET);
}

void GC9A01A::endSpiTransaction() {
    HAL_GPIO_WritePin(cs_gpio_, cs_pin_, GPIO_PIN_SET);
}

void GC9A01A::transmitCommand(uint8_t command) {
    HAL_GPIO_WritePin(dc_gpio_, dc_pin_, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hspi_, &command, 1, 100);
    HAL_GPIO_WritePin(dc_gpio_, dc_pin_, GPIO_PIN_SET);
}

void GC9A01A::transmitData(size_t n, uint16_t *data) {
    for (size_t i = 0; i < n; ++i) {
        uint8_t buf[2] = {
            static_cast<uint8_t>(data[i] >> 8),
            static_cast<uint8_t>(data[i] & 0xFF)
        };
        HAL_SPI_Transmit(hspi_, buf, 2, 100);
    }
}

void GC9A01A::transmitColor(uint16_t color) {
    uint8_t buf[2] = {
        static_cast<uint8_t>(color >> 8),
        static_cast<uint8_t>(color & 0xFF)
    };
    HAL_SPI_Transmit(hspi_, buf, 2, 100);
}

size_t GC9A01A::getWidth() const { return width_; }
size_t GC9A01A::getHeight() const { return height_; }

void GC9A01A::fillRectDMA(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {


    startSpiTransaction();
    setAddrWindow(x, y, width, height);
    setSpiDataSize(SPI_DATASIZE_16BIT);
    _drawTaskHandle = xTaskGetCurrentTaskHandle();

    const uint32_t totalPixels = width * height;
    static const uint16_t blockSize = 512;  // max liczba pixelów na raz
    static uint8_t buf[blockSize * 2];

    for (uint16_t i = 0; i < blockSize; ++i) {
        buf[2 * i] = color >> 8;
        buf[2 * i + 1] = color & 0xFF;
    }

    uint32_t remaining = totalPixels;
    while (remaining > 0) {
        uint16_t toSend = (remaining > blockSize) ? blockSize : remaining;

        HAL_SPI_Transmit_DMA(hspi_, buf, toSend * 2);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // czekaj na zakończenie DMA
        remaining -= toSend;
    }

    endSpiTransaction();
    _drawTaskHandle = nullptr;
}

void GC9A01A::drawBitmapDMA(const uint16_t* img, uint16_t width, uint16_t height) {
    // Czekaj na poprzedni transfer


    startSpiTransaction();
    setAddrWindow(0, 0, width, height);
    setSpiDataSize(SPI_DATASIZE_16BIT);
    _drawTaskHandle = xTaskGetCurrentTaskHandle();

    const uint32_t totalPixels = width * height;
    const uint16_t blockSize = 512; // liczba pikseli na blok
    //uint16_t lineBuf[blockSize];    // Bufor w RAM — lokalny (na stosie)

    uint32_t remaining = totalPixels;
    const uint16_t* src = img;

    while (remaining > 0) {
        uint16_t toCopy = (remaining > blockSize) ? blockSize : remaining;

        uartPrint("Free heap before DMA: %lu\n", xPortGetFreeHeapSize());
        // Skopiuj dane z Flash (const) do RAM
        memcpy(lineBuf, src, toCopy * sizeof(uint16_t));

        // Uruchom DMA
        HAL_SPI_Transmit_DMA(hspi_, (uint8_t*)lineBuf, toCopy * sizeof(uint16_t));
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // czekaj na zakończenie DMA

        // Przesuń wskaźnik i aktualizuj liczbę pozostałych pikseli
        src += toCopy;
        remaining -= toCopy;
    }


    endSpiTransaction();
    _drawTaskHandle = nullptr;
}



void GC9A01A::drawBitmapDMA(const uint16_t* img) {
    drawBitmapDMA(img, width_, height_);
}

//void GC9A01A::drawBitmapFromFlashNoDMA(const uint16_t* img, uint16_t width, uint16_t height) {
//    startSpiTransaction();
//    setAddrWindow(0, 0, width, height);
//    setSpiDataSize(SPI_DATASIZE_16BIT);
//
//    const uint32_t totalPixels = width * height;
//    const uint16_t blockSize = 512;  // pixels
//    uint16_t lineBuf[blockSize];
//
//    uint32_t remaining = totalPixels;
//    const uint16_t* src = img;
//
//    while (remaining > 0) {
//        uint16_t toCopy = (remaining > blockSize) ? blockSize : remaining;
//
//        memcpy(lineBuf, src, toCopy * sizeof(uint16_t));
//        HAL_SPI_Transmit(hspi_, (uint8_t*)lineBuf, toCopy * 2, HAL_MAX_DELAY);
//
//        src += toCopy;
//        remaining -= toCopy;
//    }
//
//    endSpiTransaction();
//}

void GC9A01A::drawBitmap1bit(const uint8_t* img, uint16_t w, uint16_t h, uint16_t color0, uint16_t color1) {
    const uint32_t totalPixels = w * h;
    const uint16_t blockSize = 512;  // maks. liczba pikseli na raz

    const uint8_t* src = img;
    uint32_t bitIndex = 0;
    uint32_t remaining = totalPixels;

    _drawTaskHandle = xTaskGetCurrentTaskHandle();

    startSpiTransaction();
    setAddrWindow(0, 0, w, h);
    setSpiDataSize(SPI_DATASIZE_16BIT);

    while (remaining > 0) {
        uint16_t count = (remaining > blockSize) ? blockSize : remaining;

        for (uint16_t i = 0; i < count; ++i) {
            uint8_t byte = src[bitIndex / 8];
            bool bit = (byte >> (7 - (bitIndex % 8))) & 0x01;
            lineBuf[i] = bit ? color1 : color0;
            ++bitIndex;
        }

        HAL_SPI_Transmit_DMA(hspi_, (uint8_t*)lineBuf, count * 2);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // czekaj na zakończenie DMA
        remaining -= count;
    }

    endSpiTransaction();
    _drawTaskHandle = nullptr;
}



