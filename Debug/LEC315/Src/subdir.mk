################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LEC315/Src/LEC315.c 

OBJS += \
./LEC315/Src/LEC315.o 

C_DEPS += \
./LEC315/Src/LEC315.d 


# Each subdirectory must supply rules for building sources it contributes
LEC315/Src/%.o: ../LEC315/Src/%.c LEC315/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/pc/STM32CubeIDE/workspace_1.8.0/STM32_HAL_LEC315/LEC315/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-LEC315-2f-Src

clean-LEC315-2f-Src:
	-$(RM) ./LEC315/Src/LEC315.d ./LEC315/Src/LEC315.o

.PHONY: clean-LEC315-2f-Src

