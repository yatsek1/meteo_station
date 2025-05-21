/*
 * led.h
 *
 *  Created on: Apr 29, 2025
 *      Author: sapal
 */

#ifndef SRC_LED_H_
#define SRC_LED_H_

#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"

class Led {
	public:
		Led(GPIO_TypeDef *gpio_port, uint16_t gpio_num, uint16_t freq = 0);
		void start();
		void stop();
		void set_freq(uint16_t freq);
		uint16_t get_freq();
	private:

	    static void blink_task_entry(void *argument);
	    void blink_task();

		TaskHandle_t task_blink;
		GPIO_TypeDef* gpio_port;
		uint16_t gpio_num;
		volatile  uint16_t freq;
		volatile  GPIO_PinState status;
};


#endif /* SRC_LED_H_ */
