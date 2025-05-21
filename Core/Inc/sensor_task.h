/*
 * sensor_task.h
 *
 *  Created on: May 8, 2025
 *      Author: sapal
 */

#ifndef INC_SENSOR_TASK_H_
#define INC_SENSOR_TASK_H_

#include <cstdint>

struct weatherData{
	float temperature {-999.0};
	float humidity {-999.0};
	int32_t pres {-999};
};

void Sensor_Task(void *argument);

#endif /* INC_SENSOR_TASK_H_ */
