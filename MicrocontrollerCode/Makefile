
# Project name and build direcotory name

OBJDIR           := BUILD
PROJECT          := HomeAutomation
COM_PORT         := COM7

# STM32 Cube Programmer

CUBE_PROGRAMMER  := 'STM32_Programmer_CLI.exe'
EXT_FLASH_LOADER := 'N25Q128A_STM32F746G-DISCO.stldr'

# Use GCC

AS               := 'arm-none-eabi-gcc'
CC               := 'arm-none-eabi-gcc'
LD               := 'arm-none-eabi-gcc'
ELF2BIN          := 'arm-none-eabi-objcopy'

# Defines

CONFIG           += -DSTM32F746xx
CONFIG           += -DUSE_STM32746G_DISCOVERY
CONFIG           += -DUSE_HAL_DRIVER

# Static libraries

EXTERNAL_LIBS    += SourceFiles/Middlewares/STemWin/Lib/lib_STemWin532_CM7_GCC.a

# Linker script

LINKER_SCRIPT    := arm-gcc-link.ld

# Compiler flags

C_FLAGS          := -std=gnu99 -c -O3 -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard $(CONFIG)

# Linker flags

LD_FLAGS         :=-Wl,--gc-sections -Wl, -n -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard
LD_SYS_LIBS      :=-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group $(EXTERNAL_LIBS)

# Source files

SOURCE_FILES += SourceFiles/Components/ft5336/ft5336.c
SOURCE_FILES += SourceFiles/BSP/STM32746G-Discovery/Src/stm32746g_discovery.c
SOURCE_FILES += SourceFiles/BSP/STM32746G-Discovery/Src/stm32746g_discovery_lcd.c
SOURCE_FILES += SourceFiles/BSP/STM32746G-Discovery/Src/stm32746g_discovery_qspi.c
SOURCE_FILES += SourceFiles/BSP/STM32746G-Discovery/Src/stm32746g_discovery_sd.c
SOURCE_FILES += SourceFiles/BSP/STM32746G-Discovery/Src/stm32746g_discovery_sdram.c
SOURCE_FILES += SourceFiles/BSP/STM32746G-Discovery/Src/stm32746g_discovery_ts.c
SOURCE_FILES += SourceFiles/Drivers/CMSIS/Device/ST/STM32F7xx/Source/Templates/system_stm32f7xx.c

SOURCE_FILES += startup_stm32f746xx.c

SOURCE_FILES += $(wildcard SourceFiles/Middlewares/FatFs/src/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Middlewares/FatFs/src/drivers/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Middlewares/STemWin/Config/Src/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Middlewares/STemWin/OS/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Drivers/STM32F7xx_HAL_Driver/Src/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Src/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Icons/*.c)
SOURCE_FILES += $(wildcard SourceFiles/Utilities/Fonts/*.c)

OBJECT_FILES := $(patsubst %.o, $(OBJDIR)/%.o, $(SOURCE_FILES:.c=.o))

# Include directories

INCLUDE_DIRS += -I./SourceFiles/BSP/STM32746G-Discovery/Inc
INCLUDE_DIRS += -I./SourceFiles/Components
INCLUDE_DIRS += -I./SourceFiles/Components/Common
INCLUDE_DIRS += -I./SourceFiles/Middlewares/STemWin/Config/inc
INCLUDE_DIRS += -I./SourceFiles/Middlewares/STemWin/inc
INCLUDE_DIRS += -I./SourceFiles/Middlewares/FatFs/src
INCLUDE_DIRS += -I./SourceFiles/Middlewares/FatFs/src/drivers
INCLUDE_DIRS += -I./SourceFiles/Drivers/STM32F7xx_HAL_Driver
INCLUDE_DIRS += -I./SourceFiles/Drivers/STM32F7xx_HAL_Driver/Inc
INCLUDE_DIRS += -I./SourceFiles/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy
INCLUDE_DIRS += -I./SourceFiles/Drivers/STM32F7xx_HAL_Driver/Src
INCLUDE_DIRS += -I./SourceFiles/Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCLUDE_DIRS += -I./SourceFiles/Drivers/CMSIS/Include
INCLUDE_DIRS += -I./SourceFiles/Utilities/Fonts
INCLUDE_DIRS += -I./SourceFiles/Inc

.PHONY: all lst

clean:
	@rm -rf $(OBJDIR)

flash:
	$(CUBE_PROGRAMMER) -c port=SWD reset=HWrst -w $(PROJECT).hex -rst

flash-ext:
	$(CUBE_PROGRAMMER) -c port=SWD reset=HWrst -el $(EXT_FLASH_LOADER) -w $(PROJECT).hex -rst

log:
	$(CUBE_PROGRAMMER) -c port=COM7 br=57600 P=NONE sb=1 console

all: $(PROJECT).bin $(PROJECT).hex

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	+@echo 'Compile $<'
	+@mkdir -p $(@D)
	@$(CC) $(C_FLAGS) $(INCLUDE_DIRS) -o $@ $<

$(OBJDIR)/%.o: %.s | $(OBJDIR)
	+@echo 'Assemble $<'
	+@mkdir -p $(@D)
	@$(AS) -c $(ASM_FLAGS) -o $@ $<

$(OBJDIR):
	+@mkdir $@

$(PROJECT).elf: $(OBJECT_FILES)
	+@echo "link: $(notdir $@)"
	@$(LD) $(LD_FLAGS) -T $(LINKER_SCRIPT) $(LIBRARY_PATHS) --output $@ $(OBJECT_FILES) $(LIBRARIES) $(LD_SYS_LIBS)

$(PROJECT).bin: $(PROJECT).elf
	$(ELF2BIN) -O binary $< $@

$(PROJECT).hex: $(PROJECT).elf
	$(ELF2BIN) -O ihex $< $@

DEPS = $(OBJECT_FILES:.o=.d)
-include $(DEPS)
