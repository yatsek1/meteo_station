/*
 * my_app.h
 *
 *  Created on: Apr 28, 2025
 *      Author: sapal
 */

#ifndef INC_MY_APP_H_
#define INC_MY_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "usart.h" // Twój UART handle
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

static inline float round2(float val)
{
    return (float)((int)(val * 100 + (val >= 0 ? 0.5f : -0.5f))) / 100.0f;
}

extern SemaphoreHandle_t uartMutex;
extern QueueHandle_t logQueue;

typedef enum {
    ESP_MSG_THINGSPEAK,
    ESP_MSG_DIAGNOSTICS,
    ESP_MSG_LOG
} ESPMessageType;

typedef struct {
    ESPMessageType type;
    char payload[512];  // może być JSON do TCP, log, itp.
} ESPMessage;

void uartPrint(const char* format, ...);
void logMessage(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* INC_MY_APP_H_ */
