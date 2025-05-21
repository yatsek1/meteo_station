################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/my_fonts/logo_thaleo.cpp 

C_SRCS += \
../Core/Src/my_fonts/fontData6x8.c \
../Core/Src/my_fonts/fontData9x16.c \
../Core/Src/my_fonts/my_fonts.c 

C_DEPS += \
./Core/Src/my_fonts/fontData6x8.d \
./Core/Src/my_fonts/fontData9x16.d \
./Core/Src/my_fonts/my_fonts.d 

OBJS += \
./Core/Src/my_fonts/fontData6x8.o \
./Core/Src/my_fonts/fontData9x16.o \
./Core/Src/my_fonts/logo_thaleo.o \
./Core/Src/my_fonts/my_fonts.o 

CPP_DEPS += \
./Core/Src/my_fonts/logo_thaleo.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/my_fonts/%.o Core/Src/my_fonts/%.su Core/Src/my_fonts/%.cyclo: ../Core/Src/my_fonts/%.c Core/Src/my_fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/my_fonts/%.o Core/Src/my_fonts/%.su Core/Src/my_fonts/%.cyclo: ../Core/Src/my_fonts/%.cpp Core/Src/my_fonts/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-my_fonts

clean-Core-2f-Src-2f-my_fonts:
	-$(RM) ./Core/Src/my_fonts/fontData6x8.cyclo ./Core/Src/my_fonts/fontData6x8.d ./Core/Src/my_fonts/fontData6x8.o ./Core/Src/my_fonts/fontData6x8.su ./Core/Src/my_fonts/fontData9x16.cyclo ./Core/Src/my_fonts/fontData9x16.d ./Core/Src/my_fonts/fontData9x16.o ./Core/Src/my_fonts/fontData9x16.su ./Core/Src/my_fonts/logo_thaleo.cyclo ./Core/Src/my_fonts/logo_thaleo.d ./Core/Src/my_fonts/logo_thaleo.o ./Core/Src/my_fonts/logo_thaleo.su ./Core/Src/my_fonts/my_fonts.cyclo ./Core/Src/my_fonts/my_fonts.d ./Core/Src/my_fonts/my_fonts.o ./Core/Src/my_fonts/my_fonts.su

.PHONY: clean-Core-2f-Src-2f-my_fonts

