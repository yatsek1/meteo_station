################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/ButtonHandler.cpp \
../Core/Src/bme_display_task.cpp \
../Core/Src/esp8266.cpp \
../Core/Src/esp_wifi.cpp \
../Core/Src/gc9a01a.cpp \
../Core/Src/led.cpp \
../Core/Src/main_cpp.cpp \
../Core/Src/my_app.cpp \
../Core/Src/sensor_task.cpp \
../Core/Src/watchdog.cpp 

C_SRCS += \
../Core/Src/BMXX80.c \
../Core/Src/delays.c \
../Core/Src/dma.c \
../Core/Src/eeprom.c \
../Core/Src/freertos.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/iwdg.c \
../Core/Src/main.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

C_DEPS += \
./Core/Src/BMXX80.d \
./Core/Src/delays.d \
./Core/Src/dma.d \
./Core/Src/eeprom.d \
./Core/Src/freertos.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/iwdg.d \
./Core/Src/main.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

OBJS += \
./Core/Src/BMXX80.o \
./Core/Src/ButtonHandler.o \
./Core/Src/bme_display_task.o \
./Core/Src/delays.o \
./Core/Src/dma.o \
./Core/Src/eeprom.o \
./Core/Src/esp8266.o \
./Core/Src/esp_wifi.o \
./Core/Src/freertos.o \
./Core/Src/gc9a01a.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/iwdg.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/main_cpp.o \
./Core/Src/my_app.o \
./Core/Src/sensor_task.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/watchdog.o 

CPP_DEPS += \
./Core/Src/ButtonHandler.d \
./Core/Src/bme_display_task.d \
./Core/Src/esp8266.d \
./Core/Src/esp_wifi.d \
./Core/Src/gc9a01a.d \
./Core/Src/led.d \
./Core/Src/main_cpp.d \
./Core/Src/my_app.d \
./Core/Src/sensor_task.d \
./Core/Src/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/BMXX80.cyclo ./Core/Src/BMXX80.d ./Core/Src/BMXX80.o ./Core/Src/BMXX80.su ./Core/Src/ButtonHandler.cyclo ./Core/Src/ButtonHandler.d ./Core/Src/ButtonHandler.o ./Core/Src/ButtonHandler.su ./Core/Src/bme_display_task.cyclo ./Core/Src/bme_display_task.d ./Core/Src/bme_display_task.o ./Core/Src/bme_display_task.su ./Core/Src/delays.cyclo ./Core/Src/delays.d ./Core/Src/delays.o ./Core/Src/delays.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/eeprom.cyclo ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/eeprom.su ./Core/Src/esp8266.cyclo ./Core/Src/esp8266.d ./Core/Src/esp8266.o ./Core/Src/esp8266.su ./Core/Src/esp_wifi.cyclo ./Core/Src/esp_wifi.d ./Core/Src/esp_wifi.o ./Core/Src/esp_wifi.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/gc9a01a.cyclo ./Core/Src/gc9a01a.d ./Core/Src/gc9a01a.o ./Core/Src/gc9a01a.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/iwdg.cyclo ./Core/Src/iwdg.d ./Core/Src/iwdg.o ./Core/Src/iwdg.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/main_cpp.cyclo ./Core/Src/main_cpp.d ./Core/Src/main_cpp.o ./Core/Src/main_cpp.su ./Core/Src/my_app.cyclo ./Core/Src/my_app.d ./Core/Src/my_app.o ./Core/Src/my_app.su ./Core/Src/sensor_task.cyclo ./Core/Src/sensor_task.d ./Core/Src/sensor_task.o ./Core/Src/sensor_task.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_hal_timebase_tim.cyclo ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_hal_timebase_tim.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/watchdog.cyclo ./Core/Src/watchdog.d ./Core/Src/watchdog.o ./Core/Src/watchdog.su

.PHONY: clean-Core-2f-Src

