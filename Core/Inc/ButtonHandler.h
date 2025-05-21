/*
 * ButtonHandler.hpp
 *
 *  Created on: May 16, 2025
 *      Author: sapal
 */

#pragma once

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "my_app.h"

enum class ButtonEvent {
    SHORT_PRESS,
    LONG_PRESS
};

class ButtonHandler {
public:
    ButtonHandler(GPIO_TypeDef* port, uint16_t pin, TaskHandle_t listener);

    void start();               // Tworzy task do obsługi
    void onInterrupt();         // Wywoływane z ISR (EXTI)

private:
    static void taskEntry(void* param);
    void taskLoop();

    GPIO_TypeDef* _port;
    uint16_t _pin;
    TaskHandle_t _listener;
    TaskHandle_t _taskHandle;
    TickType_t _lastHandled;
};
