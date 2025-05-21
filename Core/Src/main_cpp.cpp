/*
 * main_cpp.cpp
 *
 *  Created on: Apr 29, 2025
 *      Author: sapal
 */



#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <stdio.h>
#include "my_app.h"
#include "main_cpp.h"
#include "led.h"
#include "bme_display_task.h"
#include "esp_wifi.h"
#include "gc9a01a.h"
#include "sensor_task.h"
#include "BMPXX80.h"
#include "eeprom.h"
#include "ButtonHandler.h"
#include "watchdog.h"




/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

Led led_wifi(GPIOA, GPIO_PIN_4,100);
TaskHandle_t bmeDisplayTaskHandle = nullptr;
TaskHandle_t ESPwifiTaskHandle = nullptr;
TaskHandle_t LCDInterruptTaskHandle = nullptr;
ButtonHandler* g_button = nullptr;

extern GC9A01A tft;


//Interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_11 && g_button)
    {
    	g_button->onInterrupt();
    }
}



extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (hspi == &hspi1 && bmeDisplayTaskHandle != nullptr) {
        vTaskNotifyGiveFromISR(bmeDisplayTaskHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // Możesz np. migać diodą, zatrzymać system, wysłać info po UART, itp.
    uartPrint("Stack overflow in task: %s\n", pcTaskName);
    while (1);  // Zatrzymaj system
}

extern "C" void vApplicationMallocFailedHook(void) {
    uartPrint("Malloc failed!\n");
    while (1);  // Zatrzymaj system lub podejmij inne działanie
}

volatile uint32_t idleCounter = 0;

extern "C" void vApplicationIdleHook(void) {
    idleCounter++;
}



int main_cpp(void)
{
    uartMutex = xSemaphoreCreateMutex();
    if (uartMutex == nullptr) {
        // obsłuż błąd
    }
    logQueue = xQueueCreate(10, sizeof(ESPMessage));
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim2);
    HAL_UART_Transmit(&huart2, (uint8_t*)"Start main_cpp\n", 15, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)"Before BME280_Init\n", 20, HAL_MAX_DELAY);
    BME280_Init(&hi2c1, BME280_TEMPERATURE_16BIT, BME280_PRESSURE_ULTRALOWPOWER, BME280_HUMINIDITY_STANDARD, BME280_NORMALMODE);
    HAL_UART_Transmit(&huart2, (uint8_t*)"After BME280_Init\n", 19, HAL_MAX_DELAY);
    BME280_SetConfig(BME280_STANDBY_MS_10, BME280_FILTER_OFF);
    HAL_FLASH_Unlock();
    EE_Init();  // wewnętrznie robi erase i zapis
    HAL_FLASH_Lock();

    led_wifi.start();
    led_wifi.set_freq(100);
    //TASKS
    xTaskCreate(
        [](void *arg) {
            uartPrint("Watchdog task running inline\n");
            WatchdogManager::instance().watchdogTask();
        },
        "Watchdog",
        512,
        nullptr,
        tskIDLE_PRIORITY + 1,
        nullptr
    );
    xTaskCreate(
    	BME_Display_Task,
        "BME_DISPLAY",
        512,
        nullptr,
        tskIDLE_PRIORITY + 1,
		&bmeDisplayTaskHandle
    );
    xTaskCreate(
    	ESP_wifi_task,
        "ESP_wifi",
        1280,
		&led_wifi,
        tskIDLE_PRIORITY + 1,
        nullptr
    );
    xTaskCreate(
    	Sensor_Task,
        "Sensor_Read",
        512,
		nullptr,
        tskIDLE_PRIORITY + 1,
        nullptr
    );
    g_button = new ButtonHandler(GPIOC, GPIO_PIN_11, bmeDisplayTaskHandle);
    g_button->start();


    return 0;

}

