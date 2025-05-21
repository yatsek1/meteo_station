// esp8266.h
#pragma once

#include "stm32f4xx_hal.h"
#include <cstddef>

class ESP8266 {
public:
    ESP8266(UART_HandleTypeDef* uart, UART_HandleTypeDef* debug_uart = nullptr);

    bool sendCommand(char* cmd, char* response, size_t response_size, uint32_t timeout = 3000);
    void setDebug(bool enable);
    bool connectToWiFi(const char* ssid, const char* password, char* response, size_t response_size);
    bool isConnected(const char* ssid_expected);
    bool sendToThingSpeak(const char* apiKey, float temp, float hum, int32_t pres);
    bool sendRawTcp(const char* ip, uint16_t port, const char* data, size_t len);
    bool isBusy() const;
    void setBusy(bool b);


private:
    UART_HandleTypeDef* _uart;
    UART_HandleTypeDef* _debug;
    bool _debug_enabled;
    volatile bool busy = false;


    void debugPrint(const char* str);
};
