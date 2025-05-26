/*
 * esp_wifi.cpp
 *
 *  Created on: Apr 30, 2025
 *      Author: sapal
 */
extern "C"{
#include "FreeRTOS.h"
#include "task.h"
}
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "esp_wifi.h"
#include "esp8266.h"
#include "led.h"
#include "queue.h"
#include "sensor_task.h"
#include "my_app.h"
#include "watchdog.h"


extern QueueHandle_t weatherQueue;
weatherData current;

void sendDiagnosticsOverTCP(ESP8266& esp) {
    const char* serverIp = "13.218.166.58";
    const uint16_t port = 5678;
    char json[512];
    int len = 0;

    len += snprintf(json + len, sizeof(json) - len,
        "{"
        "\"device\":\"esp01\","
        "\"tasks\":{");

    for (uint32_t i = 0; i < WatchdogManager::instance().taskCount; ++i) {
        const auto& task = WatchdogManager::instance().tasks[i];
        len += snprintf(json + len, sizeof(json) - len,
            "\"%s\":%lu%s",
            task.name,
            uxTaskGetStackHighWaterMark(task.handle),
            (i < WatchdogManager::instance().taskCount - 1) ? "," : ""
        );
    }

    len += snprintf(json + len, sizeof(json) - len,
        "},"
        "\"heap\":{"
            "\"current\":%lu,"
            "\"min\":%lu"
        "}"
        "}",
        xPortGetFreeHeapSize(),
        xPortGetMinimumEverFreeHeapSize()
    );

    if (esp.sendRawTcp(serverIp, port, json, len)) {
        uartPrint("Diagnostics sent OK\n");
    } else {
        uartPrint("Diagnostics send FAILED\n");
    }
}



void ESP_wifi_task(void *argument)
{
	Led* led = (Led*)argument;
	ESP8266 esp(&huart3, &huart2);
	uartPrint("=== ESP WiFi Task started ===\r\n");
	WatchdogManager::instance().registerTask("WifiTask", xTaskGetCurrentTaskHandle(), 60);

    bool connected = false;
    //char cmd[64];
    static char resp[1000];
    char WapiKey[] = "7WWRU5F5YMN25XGB";
    //char RapiKey[] = "HN2XX9NLQRQPK4J2";
    TickType_t previousCycleStart = xTaskGetTickCount();

    for (;;) {
		uartPrint("Stack remaining in %s task: %u\n", "esp_wifi_task", uxTaskGetStackHighWaterMark(nullptr));
        while (!connected)
        {
            // Próba połączenia
            connected = esp.connectToWiFi("INEA-3981", "c6uAX4eG", resp, sizeof(resp));
            uartPrint(resp);

            if (connected) {
                uartPrint("CONNECTED TO WIFI!\r\n");
            } else {
                uartPrint("CONNECTION TO WIFI FAILED!\r\n");
            }
            WatchdogManager::instance().notifyAlive("WifiTask");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }

        // Sprawdzenie statusu co 10 sekund
        if (!esp.isConnected("INEA-3981")) {
            uartPrint("WIFI LOST! Reconnecting...\r\n");
            connected = false;
            led->set_freq(100);

        }else{
        	uartPrint("WIFI Connected - all is good\r\n");
        	led->set_freq(2000);
        	if (xQueuePeek(weatherQueue, &current, 0) == pdPASS) {
        	    if (esp.sendToThingSpeak(WapiKey, current.temperature, current.humidity, current.pres)) {
        	        uartPrint("ThingSpeak OK\r\n");
        	    } else {
        	        uartPrint("ThingSpeak FAIL\r\n");
        	    }
        	}else{
        		uartPrint("weatherQueue FAILED\r\n");
        	}
        	vTaskDelay(pdMS_TO_TICKS(1000));
        	sendDiagnosticsOverTCP(esp);
        	ESPMessage msg;
        	while (xQueueReceive(logQueue, &msg, 0) == pdPASS) {
        	    esp.sendRawTcp("13.218.166.58", 5678, msg.payload, strlen(msg.payload));
        	}

        }
        WatchdogManager::instance().notifyAlive("WifiTask");

        vTaskDelayUntil(&previousCycleStart, pdMS_TO_TICKS(30000));


    }

}


