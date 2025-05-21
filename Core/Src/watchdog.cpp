#include "watchdog.h"
#include "stm32f4xx_hal.h"
#include <cstdio>
#include "my_app.h"


extern IWDG_HandleTypeDef hiwdg;

extern uint32_t idleCounter;

WatchdogManager& WatchdogManager::instance() {
    static WatchdogManager instance;
    return instance;
}

void WatchdogManager::registerTask(const char* name, TaskHandle_t handle, uint32_t maxSilentTicks) {
    if (taskCount < MAX_WATCHDOG_TASKS) {
        tasks[taskCount] = {
            .name = name,
            .handle = handle,
            .aliveCounter = 0,
            .maxSilentTicks = maxSilentTicks,
            .missedTicks = 0,
            .lastSeenCounter = 0
        };
        taskCount++;
    }
}

void WatchdogManager::notifyAlive(const char* name) {
    for (uint32_t i = 0; i < taskCount; i++) {
        if (strcmp(tasks[i].name, name) == 0) {
            tasks[i].aliveCounter++;
            //uartPrint("Notify from task:%s\r\n", name);
            break;
        }
    }
}

void WatchdogManager::watchdogTask() {
    const TickType_t delay = pdMS_TO_TICKS(1000);
    uartPrint("watchdogTask: this = %p\n", this);
    uint8_t counter = 0;
    while (true) {
        vTaskDelay(delay);
        uint32_t lastIdleCount = idleCounter;
        idleCounter = 0;
        if(counter >= 10){
        	counter = 0;
            float idlePercent = (float)lastIdleCount / MAX_IDLE_COUNT * 100.0f;
            float cpuLoad = 100.0f - idlePercent;

            uartPrint("CPU Load: %.1f%%\r\n", cpuLoad);
        }
        ++counter;


        for (uint32_t i = 0; i < taskCount; i++) {
            if (tasks[i].aliveCounter == tasks[i].lastSeenCounter) {
                tasks[i].missedTicks++;
                if (tasks[i].missedTicks >= tasks[i].maxSilentTicks) {
                    printf("Watchdog timeout: %s not responding\r\n", tasks[i].name);
                    vTaskDelay(1000);
                    NVIC_SystemReset();  // Reset MCU
                }
            } else {
                tasks[i].missedTicks = 0;
            }
            tasks[i].lastSeenCounter = tasks[i].aliveCounter;
        }
        HAL_IWDG_Refresh(&hiwdg);
    }
}

void StartWatchdogTask(void *argument) {
    WatchdogManager::instance().watchdogTask();
}
