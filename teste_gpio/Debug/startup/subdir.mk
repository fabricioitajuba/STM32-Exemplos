################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/teste_gpio/StdPeriph_Driver/inc" -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/teste_gpio/inc" -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/teste_gpio/CMSIS/device" -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/teste_gpio/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


