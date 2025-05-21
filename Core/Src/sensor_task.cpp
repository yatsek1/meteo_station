/*
 * sensor_task.cpp
 *
 *  Created on: May 8, 2025
 *      Author: sapal
 */


#include "sensor_task.h"
#include <cstring>
#include <stdio.h>
#include "i2c.h"
#include "my_app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "spi.h"
#include "usart.h"
#include "filters.h"
#include "queue.h"
#include "BMPXX80.h"
#include "watchdog.h"


weatherData wData;

QueueHandle_t weatherQueue = nullptr;



void Sensor_Task(void *argument){
	uartPrint("Sensor TASK Started!\n");
	WatchdogManager::instance().registerTask("SensorTask", xTaskGetCurrentTaskHandle(), 5);
	uartPrint("Stack remaining in %s task: %u\n", "sensor_task", uxTaskGetStackHighWaterMark(nullptr));
	Med_Filter<float, 5> med_temp_filter;
	Med_Filter<float, 5> med_hum_filter;
	Med_Filter<int32_t, 5> med_pres_filter;
	weatherQueue = xQueueCreate(1, sizeof(weatherData));
	configASSERT(weatherQueue != nullptr);
	for(;;){
        BME280_ReadTemperatureAndPressureAndHuminidity(&wData.temperature, &wData.pres, &wData.humidity);
        wData.pres = med_pres_filter.filter(BME280_GetSeaLevelPressure(74.0)/100);
        wData.temperature = round2(med_temp_filter.filter(wData.temperature));
        wData.humidity = round2(med_hum_filter.filter(wData.humidity));

        char data[70];
        sprintf(data, "temp: %.2f, pressure: %ld, humidity: %.2f\r\n", wData.temperature, wData.pres, wData.humidity);
        //uartPrint(data);

        xQueueOverwrite(weatherQueue, &wData);
        WatchdogManager::instance().notifyAlive("SensorTask");
        vTaskDelay(pdMS_TO_TICKS(2000));

    }

}
