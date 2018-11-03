################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ccsbcs.c \
../src/diskio.c \
../src/ff.c \
../src/ffsystem.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

OBJS += \
./src/ccsbcs.o \
./src/diskio.o \
./src/ff.o \
./src/ffsystem.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f10x.o 

C_DEPS += \
./src/ccsbcs.d \
./src/diskio.d \
./src/ff.d \
./src/ffsystem.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"D:/Microcontroladores/ARM/Projetos/System Workbench/sd_std/StdPeriph_Driver/inc" -I"D:/Microcontroladores/ARM/Projetos/System Workbench/sd_std/inc" -I"D:/Microcontroladores/ARM/Projetos/System Workbench/sd_std/CMSIS/device" -I"D:/Microcontroladores/ARM/Projetos/System Workbench/sd_std/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


