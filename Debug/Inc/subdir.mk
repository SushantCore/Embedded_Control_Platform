################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/uart_driver.c 

OBJS += \
./Inc/uart_driver.o 

C_DEPS += \
./Inc/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/%.o Inc/%.su Inc/%.cyclo: ../Inc/%.c Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4S5xx -DSTM32 -DSTM32L4PLUS -DSTM32L4S5VITx -DB_L4S5I_IOT01A -c -IC:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"C:/Users/Sushant Sharma/STM32CubeIDE/workspace_2.2.0/Embedded_Control_Platform/Inc" -I"C:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/CMSIS/Include" -I"C:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/STM32L4xx_HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Inc

clean-Inc:
	-$(RM) ./Inc/uart_driver.cyclo ./Inc/uart_driver.d ./Inc/uart_driver.o ./Inc/uart_driver.su

.PHONY: clean-Inc

