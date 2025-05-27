/*
 * my_app.cpp
 *
 *  Created on: May 16, 2025
 *      Author: sapal
 */


#include "my_app.h"

SemaphoreHandle_t uartMutex = nullptr;

QueueHandle_t logQueue = nullptr;

void uartPrint(const char* format, ...) {
    if (xSemaphoreTake(uartMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        char buffer[128];

        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        HAL_UART_Transmit(&huart2, reinterpret_cast<uint8_t*>(buffer), strlen(buffer), HAL_MAX_DELAY);

        xSemaphoreGive(uartMutex);
    } else {
        // opcjonalnie: timeout, nie udało się zdobyć mutexa
    }

}

void logMessage(const char* fmt, ...) {
    char msgbuf[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgbuf, sizeof(msgbuf), fmt, args);
    va_end(args);

    ESPMessage msg = { .type = ESP_MSG_LOG };
    snprintf(msg.payload, sizeof(msg.payload),
             "{\"device\":\"esp01\",\"type\":\"log\",\"message\":\"%s\"}", msgbuf);

    xQueueSend(logQueue, &msg, 0);
}
