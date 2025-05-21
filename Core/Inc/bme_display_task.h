/*
 * bme_display_task.h
 *
 *  Created on: Apr 30, 2025
 *      Author: sapal
 */

#ifndef INC_BME_DISPLAY_TASK_H_
#define INC_BME_DISPLAY_TASK_H_



void BME_Display_Task(void *argument);
void lcd_init(const uint16_t* logo);
void show_weather_data();


#endif /* INC_BME_DISPLAY_TASK_H_ */
