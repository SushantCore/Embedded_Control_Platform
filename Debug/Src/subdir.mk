################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/app.c \
../Src/config.c \
../Src/error_handler.c \
../Src/gpio_driver.c \
../Src/interrupt_driver.c \
../Src/logging.c \
../Src/main.c \
../Src/pwm_driver.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32l4xx.c \
../Src/timer_driver.c \
../Src/uart_driver.c 

OBJS += \
./Src/app.o \
./Src/config.o \
./Src/error_handler.o \
./Src/gpio_driver.o \
./Src/interrupt_driver.o \
./Src/logging.o \
./Src/main.o \
./Src/pwm_driver.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32l4xx.o \
./Src/timer_driver.o \
./Src/uart_driver.o 

C_DEPS += \
./Src/app.d \
./Src/config.d \
./Src/error_handler.d \
./Src/gpio_driver.d \
./Src/interrupt_driver.d \
./Src/logging.d \
./Src/main.d \
./Src/pwm_driver.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32l4xx.d \
./Src/timer_driver.d \
./Src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4S5xx -DSTM32 -DSTM32L4PLUS -DSTM32L4S5VITx -DB_L4S5I_IOT01A -c -IC:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include -I"C:/Users/Sushant Sharma/STM32CubeIDE/workspace_2.2.0/Embedded_Control_Platform/Inc" -I"C:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/CMSIS/Include" -I"C:/STM32/STM32CubeL4/STM32Cube_FW_L4_V1.18.0/Drivers/STM32L4xx_HAL_Driver/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/app.cyclo ./Src/app.d ./Src/app.o ./Src/app.su ./Src/config.cyclo ./Src/config.d ./Src/config.o ./Src/config.su ./Src/error_handler.cyclo ./Src/error_handler.d ./Src/error_handler.o ./Src/error_handler.su ./Src/gpio_driver.cyclo ./Src/gpio_driver.d ./Src/gpio_driver.o ./Src/gpio_driver.su ./Src/interrupt_driver.cyclo ./Src/interrupt_driver.d ./Src/interrupt_driver.o ./Src/interrupt_driver.su ./Src/logging.cyclo ./Src/logging.d ./Src/logging.o ./Src/logging.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/pwm_driver.cyclo ./Src/pwm_driver.d ./Src/pwm_driver.o ./Src/pwm_driver.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32l4xx.cyclo ./Src/system_stm32l4xx.d ./Src/system_stm32l4xx.o ./Src/system_stm32l4xx.su ./Src/timer_driver.cyclo ./Src/timer_driver.d ./Src/timer_driver.o ./Src/timer_driver.su ./Src/uart_driver.cyclo ./Src/uart_driver.d ./Src/uart_driver.o ./Src/uart_driver.su

.PHONY: clean-Src

