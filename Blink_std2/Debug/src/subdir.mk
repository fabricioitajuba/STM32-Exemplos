################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/delays.c \
../src/ds18b20.c \
../src/lcd.c \
../src/main.c \
../src/one_wire.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

OBJS += \
./src/delays.o \
./src/ds18b20.o \
./src/lcd.o \
./src/main.o \
./src/one_wire.o \
./src/syscalls.o \
./src/system_stm32f10x.o 

C_DEPS += \
./src/delays.d \
./src/ds18b20.d \
./src/lcd.d \
./src/main.d \
./src/one_wire.d \
./src/syscalls.d \
./src/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/DS18B20_std2/StdPeriph_Driver/inc" -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/DS18B20_std2/inc" -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/DS18B20_std2/CMSIS/device" -I"D:/Informações/Microcontroladores/ARM/Projetos/System workbench/DS18B20_std2/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


