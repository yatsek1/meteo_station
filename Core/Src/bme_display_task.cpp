/*
 * bme_display_task.cpp
 *
 *  Created on: Apr 30, 2025
 *      Author: sapal
 */

#include <cstring>
#include <cmath>
#include "gc9a01a.h"
#include "gc9a01a_initcmd.h"
#include <stdio.h>
#include "BMPXX80.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "my_app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "filters.h"
#include "fonts/logo_thaleo.h"
#include "sensor_task.h"
#include "queue.h"
#include "eeprom.h"
#include "ButtonHandler.h"
#include "watchdog.h"


const uint16_t colors[12] {GC9A01A_NAVY, GC9A01A_DARKCYAN, GC9A01A_MAROON, GC9A01A_PURPLE, GC9A01A_OLIVE, GC9A01A_OLIVE, GC9A01A_CYAN, GC9A01A_MAGENTA, GC9A01A_YELLOW, GC9A01A_WHITE, GC9A01A_ORANGE, GC9A01A_PINK};
extern TaskHandle_t bmeDisplayTaskHandle;
extern volatile bool tim2_fired;

// Obiekt wyświetlacza globalnie
GC9A01A tft(&hspi1,
                   GPIOC, GPIO_PIN_7,  // CS
                   GPIOA, GPIO_PIN_8,  // DC
                   nullptr, 0,         // brak BL
                   GPIOA, GPIO_PIN_9); // RST


weatherData prevData;
weatherData currData;
extern QueueHandle_t weatherQueue;

static char temp_disp[32];
static char press_disp[32];
static char hum_disp[32];

static const fontStruct* curr_font = &font9x16;
static const uint8_t scale = 2;
static const uint16_t start = 50;                 // początkowa Y
static const uint16_t space_before_next = (tft.getHeight() - 2 * start - scale * curr_font->height * 3) / 2 + scale * curr_font->height;

static uint8_t colorIdx {0};
static uint8_t lastSavedColorIdx = 0;
static uint32_t lastWriteTick {};

extern uint16_t VirtAddVarTab[NB_OF_VAR];

static uint8_t mode = 0;
static uint16_t waitTime = 5000;


void SaveColorIdx(uint8_t idx)
{
    HAL_FLASH_Unlock();
    uint16_t status = EE_WriteVariable(VirtAddVarTab[0], idx);

    if (status == 1) {
        // Powtórz zapis raz jeszcze
        HAL_UART_Transmit(&huart2, (uint8_t*)"Retrying EE_WriteVariable...\n", 30, HAL_MAX_DELAY);
        status = EE_WriteVariable(VirtAddVarTab[0], idx);
    }

    char msg[64];
    sprintf(msg, "EE_WriteVariable status: %u (data: %u)\n", status, idx);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

    HAL_FLASH_Lock();
}


uint8_t ReadColorIdx()
{
    uint16_t val;
    if (EE_ReadVariable(VirtAddVarTab[0], &val) == HAL_OK)
        return (uint8_t)val;
    return 0;
}

void lcd_init(const uint8_t* logo){
    uartPrint("Start GC9A01A_TestTask\n");
    WatchdogManager::instance().registerTask("DisplayTask", xTaskGetCurrentTaskHandle(), 15);
    tft.init();
    //tft.drawBitmapDMA(logo);
    tft.drawBitmap1bit(logo, tft.getHeight(), tft.getWidth(), LOGO_COLOR0, LOGO_COLOR1);
    vTaskDelay(pdMS_TO_TICKS(5000));
    colorIdx = ReadColorIdx();
    lastSavedColorIdx = colorIdx;
    char msg[64];
    sprintf(msg, "Reading from VA=0x%04X\n", VirtAddVarTab[0]);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
void updateDisplayIfNeeded(bool force) {
    uint8_t result = xQueuePeek(weatherQueue, &currData, 0);

    if ((fabsf(currData.temperature - prevData.temperature) > 0.1f && result) || force) {
        sprintf(temp_disp, "T:%.1f~C", currData.temperature);
        tft.drawTextGenFast(5, start, temp_disp, GC9A01A_BLACK, colors[colorIdx], curr_font, scale, 1);
    }
    if ((fabsf(currData.pres - prevData.pres) > 0.1f && result) || force) {
        sprintf(press_disp, "P:%ldhPa", currData.pres);
        tft.drawTextGenFast(5, start + space_before_next, press_disp, GC9A01A_BLACK, colors[colorIdx], curr_font, scale, 1);
    }
    if ((fabsf(currData.humidity - prevData.humidity) > 0.1f && result) || force) {
        sprintf(hum_disp, "H:%.1f%%", currData.humidity);
        tft.drawTextGenFast(5, start + space_before_next * 2, hum_disp, GC9A01A_BLACK, colors[colorIdx], curr_font, scale, 1);
    }

    if (result) {
        prevData = currData;
    }
}

//#define SLIDE_COUNT 2
//const uint16_t* getSlideByIndex(uint8_t idx) {
//    switch(idx){
//        case 0: return slide1;
//        case 1: return logo_thaleo;
//        default: return slide1;
//    }
//}


void show_weather_data(){

    tft.fillRect(0, 0, tft.getHeight(), tft.getWidth(), colors[colorIdx]);

    for(;;)
    {
    	bool colorChanged = false;
    	uartPrint("Stack remaining in %s task: %u\n", "bme_display_task", uxTaskGetStackHighWaterMark(bmeDisplayTaskHandle));
        uint32_t rawValue;
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &rawValue, pdMS_TO_TICKS(waitTime)) == pdTRUE) {
            ButtonEvent event = static_cast<ButtonEvent>(rawValue);

            switch (event) {
            case ButtonEvent::SHORT_PRESS:
            	if(mode == 0){
                    HAL_UART_Transmit(&huart2, (uint8_t*)"SHORT PRESS\n", 12, HAL_MAX_DELAY);
                    colorIdx = (colorIdx + 1) % (sizeof(colors) / sizeof(colors[0]));
                    tft.fillRect(0, 0, tft.getHeight(), tft.getWidth(), colors[colorIdx]);
                    colorChanged = true;
                    updateDisplayIfNeeded(colorChanged);
                    lastWriteTick = xTaskGetTickCount();
            	}
            	break;
            case ButtonEvent::LONG_PRESS:
                HAL_UART_Transmit(&huart2, (uint8_t*)"LONG PRESS\n", 11, HAL_MAX_DELAY);
                mode = !mode;
            	if(mode == 1){
            		waitTime = pdMS_TO_TICKS(10000);
            		uartPrint("Free heap: %u\n", xPortGetFreeHeapSize());
            		uartPrint("Stack remaining in %s task: %u\n", "bme_display_task", uxTaskGetStackHighWaterMark(nullptr));
            		//tft.drawBitmapDMA(getSlideByIndex(slideIdx));
            	}else if(mode == 0){
            		waitTime = pdMS_TO_TICKS(5000);
            		tft.fillRect(0, 0, tft.getHeight(), tft.getWidth(), colors[colorIdx]);
            		updateDisplayIfNeeded(true);
            	}
                break;
            }
        }else{
        	if(mode == 0){
        		updateDisplayIfNeeded(false);
        	}
        	else if(mode == 1){
        		//slideIdx = (slideIdx + 1) % SLIDE_COUNT;
        		//tft.drawBitmapDMA(getSlideByIndex(slideIdx));
        	}
        }

		if(xTaskGetTickCount() - lastWriteTick > pdMS_TO_TICKS(1 * 60 * 1000)){
			if(colorIdx != lastSavedColorIdx){
				uartPrint("Tick: %lu, ColorIdx: %u, LastSaved: %u\n", xTaskGetTickCount(), colorIdx, lastSavedColorIdx);
                logMessage("Color %u saved to EEPROM", colors[colorIdx]);
				SaveColorIdx(colorIdx);
				lastSavedColorIdx = colorIdx;
				lastWriteTick = xTaskGetTickCount();
			}

		}
		if (tim2_fired) {
		    tim2_fired = false;
		    //tft.drawBitmapFromFlashNoDMA(slide1,240,240);
		    uartPrint("TIM2 interrupt fired!\n");
		}
		WatchdogManager::instance().notifyAlive("DisplayTask");
		//uartPrint("Free heap on startup: %lu bytes\n", xPortGetFreeHeapSize());


    }
}


void BME_Display_Task(void *argument)
{        // Czeka na powiadomienie z ISR (przycisk)
	lcd_init(logo_thaleo_1bit);
	show_weather_data();
}
