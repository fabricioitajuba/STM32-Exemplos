################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Inc" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Drivers/STM32F1xx_HAL_Driver/Inc" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Middlewares/Third_Party/FreeRTOS/Source/include" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"D:/Informa��es/Microcontroladores/ARM/Projetos/System workbench/FreeRTOS/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

