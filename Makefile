
######################################
# target
######################################
TARGET = bender

FLASH_SIZE = $$((256 * 1024)) # 256 kB (Sector 6 and 7 used for config data)
RAM_SIZE = $$((128 * 1024)) # 128 kB

######################################
# building variables
######################################
# debug build?
DEBUG = 1

SERIAL_DEBUG ?= 0

# optimization
OPT = -Og

# get firmware version from git
FIRMWARE_VERSION = nope

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources

C_SOURCES += \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_exti.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
ok-STM32F4/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
ok-STM32F4/middleware/FreeRTOS/Source/croutine.c \
ok-STM32F4/middleware/FreeRTOS/Source/event_groups.c \
ok-STM32F4/middleware/FreeRTOS/Source/list.c \
ok-STM32F4/middleware/FreeRTOS/Source/queue.c \
ok-STM32F4/middleware/FreeRTOS/Source/stream_buffer.c \
ok-STM32F4/middleware/FreeRTOS/Source/tasks.c \
ok-STM32F4/middleware/FreeRTOS/Source/timers.c \
ok-STM32F4/middleware/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
ok-STM32F4/middleware/FreeRTOS/Source/portable/MemMang/heap_4.c \
ok-STM32F4/middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
ok-STM32F4/system/Src/freertos.c \
ok-STM32F4/system/Src/stm32f4xx_hal_timebase_tim.c \
ok-STM32F4/system/Src/stm32f4xx_hal_msp.c \
ok-STM32F4/system/Src/stm32f4xx_it.c \
ok-STM32F4/system/Src/system_stm32f4xx.c \
ok-STM32F4/system/Src/system_clock_config.c

CPP_SOURCES += \
ok-drivers/drivers/DAC8554/DAC8554.cpp \
ok-drivers/drivers/SX1509/SX1509.cpp \
ok-drivers/drivers/MPR121/MPR121.cpp \
ok-drivers/drivers/MCP23017/MCP23017.cpp \
ok-drivers/drivers/IS31FL3246/IS31FL3246.cpp \
ok-drivers/utils/Algorithms/Algorithms.cpp \
ok-drivers/utils/ArrayMethods/ArrayMethods.cpp \
ok-drivers/utils/BitwiseMethods/BitwiseMethods.cpp \

CPP_SOURCES += $(shell find Bender -name '*.cpp')
CPP_SOURCES += $(shell find ok-STM32F4/HAL -name '*.cpp')

# C includes
C_INCLUDES += \
-Iok-STM32F4/HAL/Inc \
-Iok-STM32F4/HAL/rtos/Inc \
-Iok-STM32F4/cxxsupport \
-Iok-STM32F4/STM32F4xx_HAL_Driver/Inc \
-Iok-STM32F4/STM32F4xx_HAL_Driver/Inc/Legacy \
-Iok-STM32F4/CMSIS/Device/ST/STM32F4xx/Include \
-Iok-STM32F4/CMSIS/Include \
-Iok-STM32F4/middleware/FreeRTOS/Source/include \
-Iok-STM32F4/middleware/FreeRTOS/Source/CMSIS_RTOS_V2 \
-Iok-STM32F4/middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-Iok-STM32F4/system/Inc \
-Iok-drivers/drivers/DAC8554 \
-Iok-drivers/drivers/SX1509 \
-Iok-drivers/drivers/MPR121 \
-Iok-drivers/drivers/MCP23017 \
-Iok-drivers/drivers/IS31FL3246 \
-Iok-drivers/utils/Algorithms \
-Iok-drivers/utils/ArrayMethods \
-Iok-drivers/utils/BitwiseMethods \
-Iok-drivers/utils/OK_I2C \
-IBender/Inc \
-IBender/Tasks/Inc \

# C++ includes
CPP_INCLUDES = \

# AS includes
AS_INCLUDES = 

# ASM sources ("Assembly Language") - defines main() function
ASM_SOURCES =  \
ok-STM32F4/startup_stm32f446xx.s

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = ok-STM32F4/STM32F446RETx_FLASH.ld


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
AR = $(GCC_PATH)/$(PREFIX)ar
GDB = $(GCC_PATH)/$(PREFIX)gdb
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
GDB = $(PREFIX)gdb
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################

# Specify the name of the target CPU.
CPU = -mcpu=cortex-m4

# Specify the name of the target floating point hardware/format.
FPU = -mfpu=fpv4-sp-d16

# Specify if floating point hardware should be used.
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F446xx



###########

# -Og                   
# -Wall	Recommended compiler warnings
# -fdata-sections
# -ffunction-sections
# -g    Generate debugging information
# -gdwarf-2
# -MMD
# -MP
# -c                       Compile and assemble, but do not link.
###########

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

ifeq ($(SERIAL_DEBUG), 1)
CFLAGS += -DSERIAL_DEBUG=1
endif

# pass the firmware version into program
CFLAGS += -DFIRMWARE_VERSION=\"$(FIRMWARE_VERSION)\"

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# C++ Flags
CPPFLAGS = $(CFLAGS) $(CPP_INCLUDES)
CPPFLAGS += \
-fno-exceptions \
-fno-rtti 

C_STANDARD = -std=gnu11
CPP_STANDARD += -std=gnu++14

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# helpers
#######################################
usedFlash = $$( $(SZ) $@ | sed -n 2p | awk '{print $$1}' )
usedFlashPercent = $$(( 100 * $(usedFlash) / $(FLASH_SIZE) ))
flashMessage = Flash Used: $(usedFlash)/$(FLASH_SIZE) ( $(usedFlashPercent) % )
usedRam = $$( $(SZ) $@ | sed -n 2p | awk '{ram=$$2+$$3} {print ram}' )
usedRamPercent = $$(( 100 * $(usedRam) / $(RAM_SIZE) ))
ramMessage = Ram Used: $(usedRam)/$(RAM_SIZE) ( $(usedRamPercent) % ) - (static only)

#######################################
# build the application
#######################################
# list of .c objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of .cpp objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(CPP_SOURCES:.cpp=.o))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@
	@echo ""
	@echo "$(flashMessage)"
	@echo "$(ramMessage)"
	@echo ""

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
# searches for all .d files in given directory and inserts them into the .c file
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


######################################
# OpenOCD stuff
# TODO: add config.mk file for settings like programmer, etc.
######################################
CHIPSET ?= stm32f4x
FLASH_ADDRESS ?= 0x08000000

OCD=openocd
OCD_DIR ?= /usr/local/share/openocd/scripts # this value works, but for some reason this folder only exists at path -> /opt/homebrew/Cellar/open-ocd/0.11.0/share/openocd/scripts
PGM_DEVICE ?= interface/stlink.cfg
OCDFLAGS = -f $(PGM_DEVICE) -f target/$(CHIPSET).cfg

program:
	$(OCD) -s $(OCD_DIR) $(OCDFLAGS) \
		-c "program ./$(BUILD_DIR)/$(TARGET).elf verify reset exit"


DFU_INTERFACE_NUMBER = 0
DFU_ALT_SETTING = 0
DFU_FUSE_ADDRESS = $(FLASH_ADDRESS)

usb-upload:
	dfu-util -a $(DFU_ALT_SETTING) -s $(DFU_FUSE_ADDRESS):leave -D $(BUILD_DIR)/$(TARGET).bin
