# target
TARGET = program


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
core/src/main.c \
core/src/freertos.c \
core/src/stm32f4xx_it.c \
core/src/stm32f4xx_hal_msp.c \
drivers/hal/src/stm32f4xx_hal_rcc.c \
drivers/hal/src/stm32f4xx_hal_rcc_ex.c \
drivers/hal/src/stm32f4xx_hal_flash.c \
drivers/hal/src/stm32f4xx_hal_flash_ex.c \
drivers/hal/src/stm32f4xx_hal_flash_ramfunc.c \
drivers/hal/src/stm32f4xx_hal_gpio.c \
drivers/hal/src/stm32f4xx_hal_dma_ex.c \
drivers/hal/src/stm32f4xx_hal_dma.c \
drivers/hal/src/stm32f4xx_hal_pwr.c \
drivers/hal/src/stm32f4xx_hal_pwr_ex.c \
drivers/hal/src/stm32f4xx_hal_cortex.c \
drivers/hal/src/stm32f4xx_hal.c \
drivers/hal/src/stm32f4xx_hal_exti.c \
drivers/hal/src/stm32f4xx_hal_tim.c \
drivers/hal/src/stm32f4xx_hal_tim_ex.c \
drivers/hal/src/stm32f4xx_hal_uart.c \
core/src/system_stm32f4xx.c \
core/src/FreeRTOS-openocd.c \
ThirdParty/FreeRTOS/Source/croutine.c \
ThirdParty/FreeRTOS/Source/event_groups.c \
ThirdParty/FreeRTOS/Source/list.c \
ThirdParty/FreeRTOS/Source/queue.c \
ThirdParty/FreeRTOS/Source/stream_buffer.c \
ThirdParty/FreeRTOS/Source/tasks.c \
ThirdParty/FreeRTOS/Source/timers.c \
ThirdParty/FreeRTOS/Source/portable/MemMang/heap_4.c \
ThirdParty/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c

# ASM sources
ASM_SOURCES =  \
startup_stm32f446xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
CPU = -mcpu=cortex-m4                    # cpu
FPU = -mfpu=fpv4-sp-d16                  # fpu
FLOAT-ABI = -mfloat-abi=hard             # float-abi
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI) # mcu

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F446xx

# AS includes
AS_INCLUDES =  \
-Icore/inc

# C includes
C_INCLUDES =  \
-Icore/inc \
-Idrivers/hal/inc \
-Idrivers/hal/inc/Legacy \
-IThirdParty/FreeRTOS/Source/include \
-IThirdParty/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-Idrivers/core

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F446RETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections,``--undefined=uxTopUsedPriority''

#######################################
# targets
#######################################

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# flash
#######################################
flash:
	openocd -f board/st_nucleo_f4.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# display serial comm with usb cable
#######################################
connect:
	sudo cu -s 115200 -l /dev/tty.usbmodem141303

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#######################################
# open comm with openocd and debug with gdb (in two different terminals)
#######################################
openocd:
	openocd -d2 -f board/st_nucleo_f4.cfg -c init -c targets -c "\$$_TARGETNAME configure -rtos auto"

gdb: $(BUILD_DIR)/$(TARGET).elf
	$(PREFIX)gdb -ex "target extended-remote localhost:3333" -ex "monitor reset halt" $^


