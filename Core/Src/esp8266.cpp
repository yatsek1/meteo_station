/*
 * esp8266.cpp
 *
 *  Created on: May 2, 2025
 *      Author: sapal
 */


// esp8266.cpp
#include "esp8266.h"
#include <cstring>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


ESP8266::ESP8266(UART_HandleTypeDef* uart, UART_HandleTypeDef* debug_uart)
    : _uart(uart), _debug(debug_uart), _debug_enabled(debug_uart != nullptr) {}

void ESP8266::setDebug(bool enable) {
    _debug_enabled = enable;
}

void ESP8266::debugPrint(const char* str) {
    if (_debug && _debug_enabled) {
        HAL_UART_Transmit(_debug, (uint8_t*)str, strlen(str), 100);
    }
}

bool ESP8266::isBusy() const { return busy; }
void ESP8266::setBusy(bool b) { busy = b; }

bool ESP8266::sendCommand(char* cmd, char* response, size_t response_size, uint32_t timeout) {
    memset(response, 0, response_size);

    HAL_UART_Transmit(_uart, (uint8_t*)cmd, strlen(cmd), 100);
    debugPrint("[ESP TX] ");
    debugPrint(cmd);
    debugPrint("\r\n");

    uint8_t ch;
    unsigned int index = 0;
    uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < timeout) {
        if (HAL_UART_Receive(_uart, &ch, 1, 10) == HAL_OK) {
            if (index < response_size - 1) {
                response[index++] = ch;
                response[index] = '\0';

                if (strstr(response, "OK")) {
                    debugPrint("[ESP RX] ");
                    debugPrint(response);
                    debugPrint("\r\n");
                    return true;
                }
                if (strstr(response, "ERROR") || strstr(response, "FAIL")) {
                    debugPrint("[ESP RX] ");
                    debugPrint(response);
                    debugPrint("\r\n");
                    return false;
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

    debugPrint("[ESP RX TIMEOUT] ");
    debugPrint(response);
    debugPrint("\r\n");

    return false;
}


bool ESP8266::connectToWiFi(const char* ssid, const char* password, char* response, size_t response_size) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);

    if (!sendCommand(cmd, response, response_size, 15000)) {  // Więcej czasu na połączenie
        return false;
    }
    if (strstr(response, "+CWJAP:1")) debugPrint("Wrong password.\r\n");
    else if (strstr(response, "+CWJAP:2")) debugPrint("AP not found.\r\n");
    else if (strstr(response, "+CWJAP:3")) debugPrint("Connection timeout.\r\n");
    // Szukamy znanych oznak sukcesu
    if (strstr(response, "WIFI CONNECTED") && strstr(response, "WIFI GOT IP") && strstr(response, "OK")) {
        return true;
    }

    // Możliwe przyczyny niepowodzenia
    if (strstr(response, "FAIL") || strstr(response, "WRONG PASSWORD") || strstr(response, "NO AP FOUND")) {
        return false;
    }

    // Domyślnie: nieudane połączenie
    return false;
}

bool ESP8266::isConnected(const char* ssid_expected) {
    char response[256];
    if (!sendCommand((char*)"AT+CWJAP?\r\n", response, sizeof(response), 1000)) {
        return false;
    }
    return strstr(response, ssid_expected);

}

bool ESP8266::sendToThingSpeak(const char* apiKey, float temp, float hum, int32_t pres) {
    while (busy) vTaskDelay(pdMS_TO_TICKS(10));
    busy = true;
    char cmd[64];
    char http[512];
    char resp[512];

    // 1. Start TCP connection
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    if (!sendCommand(cmd, resp, sizeof(resp), 3000)) return false;

    // 2. Prepare HTTP request
    snprintf(http, sizeof(http),
        "POST /update?api_key=%s&field1=%.2f&field2=%.2f&field3=%ld HTTP/1.1\r\n"
        "Host: api.thingspeak.com\r\n"
        "Connection: close\r\n"
        "Content-Length: 0\r\n"
        "\r\n",
        apiKey, temp, hum, pres);

    // 3. Request CIPSEND
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d\r\n", (int)strlen(http));
    HAL_UART_Transmit(_uart, (uint8_t*)cmd, strlen(cmd), 100);

    // 3.1 Wait for '>' prompt
    uint8_t ch;
    unsigned int index = 0;
    uint32_t start = HAL_GetTick();
    memset(resp, 0, sizeof(resp));

    while ((HAL_GetTick() - start) < 3000) {
        if (HAL_UART_Receive(_uart, &ch, 1, 10) == HAL_OK) {
            if (index < sizeof(resp) - 1) {
                resp[index++] = ch;
                resp[index] = '\0';

                if (ch == '>') {
                    debugPrint("[ESP RX] > prompt received\r\n");
                    break;
                }

                if (strstr(resp, "ERROR")) {
                    debugPrint("[ESP RX] ERROR after CIPSEND\r\n");
                    return false;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    // 4. Send actual HTTP payload
    HAL_UART_Transmit(_uart, (uint8_t*)http, strlen(http), 200);
    debugPrint("[ESP TX] ");
    debugPrint(http);

    // 5. Wait for "SEND OK"
    index = 0;
    start = HAL_GetTick();
    memset(resp, 0, sizeof(resp));
    bool success = false;

    while ((HAL_GetTick() - start) < 5000) {
        if (HAL_UART_Receive(_uart, &ch, 1, 10) == HAL_OK) {
            if (index < sizeof(resp) - 1) {
                resp[index++] = ch;
                resp[index] = '\0';

                if (strstr(resp, "SEND OK")) {
                    debugPrint("[ESP RX] SEND OK received\r\n");
                    success = true;
                    break;
                }
                if (strstr(resp, "ERROR") || strstr(resp, "FAIL")) {
                    debugPrint("[ESP RX] ERROR or FAIL received\r\n");
                    break;
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

    // 6. Close connection
    snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE\r\n");
    HAL_UART_Transmit(_uart, (uint8_t*)cmd, strlen(cmd), 100);
    debugPrint("[ESP TX] AT+CIPCLOSE\r\n");

    // Optionally read response (but not required)
    HAL_UART_Receive(_uart, &ch, 1, 100); // absorb any 'CLOSED'
    busy = false;
    return success;
}

bool ESP8266::sendRawTcp(const char* ip, uint16_t port, const char* data, size_t len) {
    while (busy) vTaskDelay(pdMS_TO_TICKS(10));
    busy = true;

    char cmd[128];
    char resp[512];
    uint8_t ch;
    unsigned int index;
    uint32_t start;
    bool promptReceived = false;
    bool success = false;

    // 1. Start TCP connection
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%u\r\n", ip, port);
    if (!sendCommand(cmd, resp, sizeof(resp), 3000)) {
        debugPrint("[ESP] CIPSTART failed:\r\n");
        debugPrint(resp);
        goto fail;
    }

    // 2. Request CIPSEND
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%u\r\n", (unsigned int)len);
    HAL_UART_Transmit(_uart, (uint8_t*)cmd, strlen(cmd), 100);

    // 3. Wait for '>' prompt
    index = 0;
    start = HAL_GetTick();
    memset(resp, 0, sizeof(resp));

    while ((HAL_GetTick() - start) < 3000) {
        if (HAL_UART_Receive(_uart, &ch, 1, 10) == HAL_OK) {
            if (index < sizeof(resp) - 1) {
                resp[index++] = ch;
                resp[index] = '\0';

                if (ch == '>') {
                    debugPrint("[ESP RX] > prompt received\r\n");
                    promptReceived = true;
                    break;
                }

                if (strstr(resp, "ERROR")) {
                    debugPrint("[ESP RX] ERROR after CIPSEND\r\n");
                    goto fail;
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

    if (!promptReceived) {
        debugPrint("[ESP] CIPSEND prompt '>' not received (timeout)\r\n");
        goto fail;
    }

    // 4. Send actual data
    HAL_UART_Transmit(_uart, (uint8_t*)data, len, 1000);
    debugPrint("[ESP TX] ");
    debugPrint(data);

    // 5. Wait for "SEND OK"
    index = 0;
    start = HAL_GetTick();
    memset(resp, 0, sizeof(resp));


    while ((HAL_GetTick() - start) < 5000) {
        if (HAL_UART_Receive(_uart, &ch, 1, 10) == HAL_OK) {
            if (index < sizeof(resp) - 1) {
                resp[index++] = ch;
                resp[index] = '\0';

                if (strstr(resp, "SEND OK")) {
                    debugPrint("[ESP RX] SEND OK received\r\n");
                    success = true;
                    break;
                }
                if (strstr(resp, "ERROR") || strstr(resp, "FAIL")) {
                    debugPrint("[ESP RX] ERROR or FAIL received after data\r\n");
                    break;
                }
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }

    // 6. Close connection (always!)
    snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE\r\n");
    HAL_UART_Transmit(_uart, (uint8_t*)cmd, strlen(cmd), 100);
    debugPrint("[ESP TX] AT+CIPCLOSE\r\n");
    HAL_UART_Receive(_uart, &ch, 1, 100); // absorb any 'CLOSED'

    busy = false;
    return success;

fail:
    // Awaryjne zamknięcie po błędzie
    snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE\r\n");
    HAL_UART_Transmit(_uart, (uint8_t*)cmd, strlen(cmd), 100);
    debugPrint("[ESP] Sent CIPCLOSE after failure\r\n");
    HAL_UART_Receive(_uart, &ch, 1, 100);

    busy = false;
    return false;
}




