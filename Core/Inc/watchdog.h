#pragma once

#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include <cstdint>
#include <array>
#include <cstring>

#define MAX_WATCHDOG_TASKS 8
#define MAX_IDLE_COUNT 134235201

class WatchdogManager {
public:
    struct TaskInfo {
        const char* name;
        TaskHandle_t handle;
        volatile uint32_t aliveCounter;
        uint32_t maxSilentTicks;
        uint32_t missedTicks;
        uint32_t lastSeenCounter;
    };

    static WatchdogManager& instance();

    void registerTask(const char* name, TaskHandle_t handle, uint32_t maxSilentTicks);
    void notifyAlive(const char* name);
    void watchdogTask();
    std::array<TaskInfo, MAX_WATCHDOG_TASKS> tasks;
    uint32_t taskCount = 0;

private:
    WatchdogManager() = default;
};

void StartWatchdogTask(void *argument);
