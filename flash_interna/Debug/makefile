################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
flash_interna.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
flash_interna.list \


# All Target
all: flash_interna.elf secondary-outputs

# Tool invocations
flash_interna.elf: $(OBJS) $(USER_OBJS) /home/fabricio/STM32/ENC28J60/Projetos/flash_interna/STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "flash_interna.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"/home/fabricio/STM32/ENC28J60/Projetos/flash_interna/STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="flash_interna.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

flash_interna.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "flash_interna.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
