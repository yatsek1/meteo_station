# Project name
PROJECT_NAME = tft_SPI2_v9_wd.elf

# Toolchain and tools
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -std=gnu11 -Wall -g -O2 -ffunction-sections -fdata-sections
CFLAGS += -DUSE_HAL_DRIVER -DSTM32F446xx
CFLAGS += -MMD -MP

LDFLAGS = -TSTM32F446RETX_FLASH.ld -Wl,--gc-sections -Wl,-Map=$(PROJECT_NAME).map

# Source and include directories
SRC_DIR = Core/Src
INCLUDE_DIRS = \
  -ICore/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
  -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
  -IDrivers/CMSIS/Include \
  -IFreeRTOS/Source/include \
  -IMiddlewares/Third_Party/FreeRTOS/Source/include \
  -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
  -IInc \
  -IFonts

# Collect source files
C_SOURCES = \
  $(wildcard $(SRC_DIR)/**/*.c) \
  $(wildcard Drivers/**/*.c) \
  $(wildcard FreeRTOS/**/*.c) \
  $(wildcard Middlewares/**/*.c)

# Object files
OBJ = $(C_SOURCES:.c=.o)

# Default target
all: $(PROJECT_NAME)

# Linking
$(PROJECT_NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $^ -o $@ $(LDFLAGS)
	$(OBJCOPY) -O ihex $@ $(PROJECT_NAME:.elf=.hex)
	$(OBJCOPY) -O binary $@ $(PROJECT_NAME:.elf=.bin)
	$(SIZE) $@

# Compilation
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Clean
clean:
	rm -f $(PROJECT_NAME) $(PROJECT_NAME:.elf=.bin) $(PROJECT_NAME:.elf=.hex) $(OBJ) $(OBJ:.o=.d) $(PROJECT_NAME:.elf=.map)

# Dependencies
-include $(OBJ:.o=.d)
