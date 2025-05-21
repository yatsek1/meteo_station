/*
 * ButtonHandler.cpp
 *
 *  Created on: May 16, 2025
 *      Author: sapal
 */


#include "ButtonHandler.h"
#include <cstring>

ButtonHandler::ButtonHandler(GPIO_TypeDef* port, uint16_t pin, TaskHandle_t listener)
    : _port(port), _pin(pin), _listener(listener), _taskHandle(nullptr), _lastHandled(0){}

void ButtonHandler::start() {
    xTaskCreate(taskEntry, "ButtonTask", 256, this, 2, &_taskHandle);
}

void ButtonHandler::taskEntry(void* param) {
    static_cast<ButtonHandler*>(param)->taskLoop();
}

void ButtonHandler::onInterrupt() {
    if (_taskHandle) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(_taskHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void ButtonHandler::taskLoop() {
    for (;;) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (xTaskGetTickCount() - _lastHandled < pdMS_TO_TICKS(150)) {
            continue;
        }


        uint8_t releasedCount = 0;

        TickType_t pressStart = xTaskGetTickCount();
        TickType_t duration = 0;
        while (releasedCount < 5 && duration < pdMS_TO_TICKS(1000)) {
            if (HAL_GPIO_ReadPin(_port, _pin) == GPIO_PIN_SET) {
                ++releasedCount;
            } else {
                releasedCount = 0; // reset jeśli znów wciśnięty
            }

            vTaskDelay(pdMS_TO_TICKS(5));
            duration = xTaskGetTickCount() - pressStart;
        }

        uartPrint("Duration: %d, count : %d", duration, releasedCount);

       ButtonEvent event;
       if(duration >= pdMS_TO_TICKS(1000)){
    	   event = ButtonEvent::LONG_PRESS;
           xTaskNotify(_listener, static_cast<uint32_t>(event), eSetValueWithOverwrite);
       }else if(duration >= pdMS_TO_TICKS(50)){
    	   event = ButtonEvent::SHORT_PRESS;
           xTaskNotify(_listener, static_cast<uint32_t>(event), eSetValueWithOverwrite);
       }

       _lastHandled = xTaskGetTickCount();

        //vTaskDelay(pdMS_TO_TICKS(200)); // antydrgania po puszczeniu
    }
}
