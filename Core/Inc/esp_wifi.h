/*
 * esp_wifi.h
 *
 *  Created on: Apr 30, 2025
 *      Author: sapal
 */

#ifndef INC_FONTS_ESP_WIFI_H_
#define INC_FONTS_ESP_WIFI_H_

#include "esp8266.h"


void ESP_wifi_task(void *argument);
void sendDiagnosticsOverTCP(ESP8266& esp);

//extern float temperature;
//extern float humidity;
//extern int32_t pres;


#endif /* INC_FONTS_ESP_WIFI_H_ */
