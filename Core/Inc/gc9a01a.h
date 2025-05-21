#pragma once

#include "stm32f4xx_hal.h"
#include "fonts/fontData6x8.h"
#include "fonts/fontData9x16.h"
#include <cstdint>
#include "FreeRTOS.h"
#include "task.h"

#define GC9A01A_SWRESET 0x01   ///< Software Reset (maybe, not documented)
#define GC9A01A_RDDID 0x04     ///< Read display identification information
#define GC9A01A_RDDST 0x09     ///< Read Display Status
#define GC9A01A_SLPIN 0x10     ///< Enter Sleep Mode
#define GC9A01A_SLPOUT 0x11    ///< Sleep Out
#define GC9A01A_PTLON 0x12     ///< Partial Mode ON
#define GC9A01A_NORON 0x13     ///< Normal Display Mode ON
#define GC9A01A_INVOFF 0x20    ///< Display Inversion OFF
#define GC9A01A_INVON 0x21     ///< Display Inversion ON
#define GC9A01A_DISPOFF 0x28   ///< Display OFF
#define GC9A01A_DISPON 0x29    ///< Display ON
#define GC9A01A_CASET 0x2A     ///< Column Address Set
#define GC9A01A_RASET 0x2B     ///< Row Address Set
#define GC9A01A_RAMWR 0x2C     ///< Memory Write
#define GC9A01A_PTLAR 0x30     ///< Partial Area
#define GC9A01A_VSCRDEF 0x33   ///< Vertical Scrolling Definition
#define GC9A01A_TEOFF 0x34     ///< Tearing Effect Line OFF
#define GC9A01A_TEON 0x35      ///< Tearing Effect Line ON
#define GC9A01A_MADCTL 0x36    ///< Memory Access Control
#define GC9A01A_VSCRSADD 0x37  ///< Vertical Scrolling Start Address
#define GC9A01A_IDLEOFF 0x38   ///< Idle mode OFF
#define GC9A01A_IDLEON 0x39    ///< Idle mode ON
#define GC9A01A_COLMOD 0x3A    ///< Pixel Format Set
#define GC9A01A_CONTINUE 0x3C  ///< Write Memory Continue
#define GC9A01A_TEARSET 0x44   ///< Set Tear Scanline
#define GC9A01A_GETLINE 0x45   ///< Get Scanline
#define GC9A01A_SETBRIGHT 0x51 ///< Write Display Brightness
#define GC9A01A_SETCTRL 0x53   ///< Write CTRL Display
#define GC9A01A1_POWER7 0xA7   ///< Power Control 7
#define GC9A01A_TEWC 0xBA      ///< Tearing effect width control
#define GC9A01A1_POWER1 0xC1   ///< Power Control 1
#define GC9A01A1_POWER2 0xC3   ///< Power Control 2
#define GC9A01A1_POWER3 0xC4   ///< Power Control 3
#define GC9A01A1_POWER4 0xC9   ///< Power Control 4
#define GC9A01A_RDID1 0xDA     ///< Read ID 1
#define GC9A01A_RDID2 0xDB     ///< Read ID 2
#define GC9A01A_RDID3 0xDC     ///< Read ID 3
#define GC9A01A_FRAMERATE 0xE8 ///< Frame rate control
#define GC9A01A_SPI2DATA 0xE9  ///< SPI 2DATA control
#define GC9A01A_INREGEN2 0xEF  ///< Inter register enable 2
#define GC9A01A_GAMMA1 0xF0    ///< Set gamma 1
#define GC9A01A_GAMMA2 0xF1    ///< Set gamma 2
#define GC9A01A_GAMMA3 0xF2    ///< Set gamma 3
#define GC9A01A_GAMMA4 0xF3    ///< Set gamma 4
#define GC9A01A_IFACE 0xF6     ///< Interface control
#define GC9A01A_INREGEN1 0xFE  ///< Inter register enable 1

// Color definitions
#define GC9A01A_BLACK 0x0000       ///<   0,   0,   0
#define GC9A01A_NAVY 0x000F        ///<   0,   0, 123
#define GC9A01A_DARKGREEN 0x03E0   ///<   0, 125,   0
#define GC9A01A_DARKCYAN 0x03EF    ///<   0, 125, 123
#define GC9A01A_MAROON 0x7800      ///< 123,   0,   0
#define GC9A01A_PURPLE 0x780F      ///< 123,   0, 123
#define GC9A01A_OLIVE 0x7BE0       ///< 123, 125,   0
#define GC9A01A_LIGHTGREY 0xC618   ///< 198, 195, 198
#define GC9A01A_DARKGREY 0x7BEF    ///< 123, 125, 123
#define GC9A01A_BLUE 0x001F        ///<   0,   0, 255
#define GC9A01A_GREEN 0x07E0       ///<   0, 255,   0
#define GC9A01A_CYAN 0x07FF        ///<   0, 255, 255
#define GC9A01A_RED 0xF800         ///< 255,   0,   0
#define GC9A01A_MAGENTA 0xF81F     ///< 255,   0, 255
#define GC9A01A_YELLOW 0xFFE0      ///< 255, 255,   0
#define GC9A01A_WHITE 0xFFFF       ///< 255, 255, 255
#define GC9A01A_ORANGE 0xFD20      ///< 255, 165,   0
#define GC9A01A_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define GC9A01A_PINK 0xFC18        ///< 255, 130, 198



#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left

extern volatile bool dmaTransferComplete;

class GC9A01A {
public:
    GC9A01A(SPI_HandleTypeDef *hspi,
            GPIO_TypeDef *cs_gpio, uint32_t cs_pin,
            GPIO_TypeDef *dc_gpio, uint32_t dc_pin,
            GPIO_TypeDef *bl_gpio, uint32_t bl_pin,
            GPIO_TypeDef *rst_gpio, uint32_t rst_pin);

    void init();
    void configureParam(uint8_t command, uint8_t *data, uint8_t n_data);
    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawPixels(uint16_t x, uint16_t y, uint16_t *color, uint16_t width, uint16_t height);
    void drawScreen(uint16_t *img);
    void fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void drawCharGen(uint16_t x, uint16_t y, char c, uint16_t color, const fontStruct* font, uint8_t scale);
    void drawTextGen(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t background, const fontStruct* font, uint8_t scale, uint8_t pos);
    size_t getWidth() const;
    size_t getHeight() const;

    static uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);
    static void swapBytes(size_t len, uint16_t *dest, uint16_t *src);
    void fillRectDMA(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void drawCharToBuffer(uint16_t* buffer, uint16_t width, uint16_t height,
            char c, uint16_t fgColor, uint16_t bgColor,
            const fontStruct* font, uint8_t scale);

    void drawTextGenFast(uint16_t x, uint16_t y, const char* str,
                                  uint16_t color, uint16_t bgColor,
                                  const fontStruct* font, uint8_t scale, uint8_t pos);

    void drawBitmapDMA(const uint16_t* img, uint16_t width, uint16_t height);
    void drawBitmapDMA(const uint16_t* img);
    //void drawBitmapFromFlashNoDMA(const uint16_t* img, uint16_t width, uint16_t height);
    void drawBitmap1bit(const uint8_t* img, uint16_t w, uint16_t h, uint16_t color0, uint16_t color1) ;

private:
    void setSpiDataSize(uint32_t data_size);
    void startSpiTransaction();
    void endSpiTransaction();
    void transmitCommand(uint8_t command);
    void transmitData(size_t n, uint16_t *data);
    void transmitColor(uint16_t color);

    SPI_HandleTypeDef *hspi_;
    GPIO_TypeDef *cs_gpio_;
    uint32_t cs_pin_;
    GPIO_TypeDef *dc_gpio_;
    uint32_t dc_pin_;
    GPIO_TypeDef *bl_gpio_;
    uint32_t bl_pin_;
    GPIO_TypeDef *rst_gpio_;
    uint32_t rst_pin_;
    TaskHandle_t _drawTaskHandle;

    static constexpr size_t width_ = 240;
    static constexpr size_t height_ = 240;

};



