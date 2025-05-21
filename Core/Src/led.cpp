/*
 * led.c
 *
 *  Created on: Apr 29, 2025
 *      Author: sapal
 */

#include "led.h"
#include "my_app.h"


Led::Led(GPIO_TypeDef *gpio_port, uint16_t gpio_num, uint16_t freq):
task_blink(NULL), gpio_port(gpio_port), gpio_num(gpio_num), freq(freq), status(GPIO_PIN_RESET){
    xTaskCreate(
        &Led::blink_task_entry,  // funkcja taska
        "LedTask",               // nazwa taska
        128,                     // stos w słowach (nie bajtach!)
        this,                    // argument = this
		tskIDLE_PRIORITY + 1,    // priorytet
        &task_blink              // uchwyt na task
    );
};

void Led::blink_task_entry(void *argument)
{
    Led *instance = static_cast<Led*>(argument);
    instance->blink_task();
}

void Led::blink_task()
{
	//uartPrint("Stack remaining in %s task: %u\n", "LED_Task", uxTaskGetStackHighWaterMark(nullptr));

    for(;;)
    {
        uint16_t local_freq;
        GPIO_PinState local_status;

        // Sekcja krytyczna — kopiujemy zmienne
        taskENTER_CRITICAL();
        local_freq = this->freq;
        local_status = this->status;
        taskEXIT_CRITICAL();

        if (local_freq != 0 && local_status == GPIO_PIN_SET) {
            HAL_GPIO_TogglePin(gpio_port, gpio_num);
            vTaskDelay(pdMS_TO_TICKS(local_freq));
        } else {
            HAL_GPIO_WritePin(gpio_port, gpio_num, local_status);
            vTaskDelay(pdMS_TO_TICKS(100));  // zapobiega 100% CPU
        }
    }
}

void Led::set_freq(uint16_t freq){
    taskENTER_CRITICAL();
    this->freq = freq;
    taskEXIT_CRITICAL();
}

uint16_t Led::get_freq(){
    uint16_t local;
    taskENTER_CRITICAL();
    local = this->freq;
    taskEXIT_CRITICAL();
    return local;
}

void Led::start(){
    taskENTER_CRITICAL();
    status = GPIO_PIN_SET;
    taskEXIT_CRITICAL();
}

void Led::stop(){
    taskENTER_CRITICAL();
    status = GPIO_PIN_RESET;
    taskEXIT_CRITICAL();
}
