################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/driver/src/stm32g4xx_hal.c \
../system/driver/src/stm32g4xx_hal_cortex.c \
../system/driver/src/stm32g4xx_hal_dac.c \
../system/driver/src/stm32g4xx_hal_exti.c \
../system/driver/src/stm32g4xx_hal_fdcan.c \
../system/driver/src/stm32g4xx_hal_gpio.c \
../system/driver/src/stm32g4xx_hal_pwr.c \
../system/driver/src/stm32g4xx_hal_pwr_ex.c \
../system/driver/src/stm32g4xx_hal_rcc.c \
../system/driver/src/stm32g4xx_hal_rcc_ex.c \
../system/driver/src/stm32g4xx_hal_tim.c \
../system/driver/src/stm32g4xx_hal_tim_ex.c \
../system/driver/src/stm32g4xx_hal_uart.c \
../system/driver/src/stm32g4xx_hal_uart_ex.c \
../system/driver/src/stm32g4xx_ll_adc.c \
../system/driver/src/stm32g4xx_ll_comp.c \
../system/driver/src/stm32g4xx_ll_dac.c \
../system/driver/src/stm32g4xx_ll_exti.c \
../system/driver/src/stm32g4xx_ll_gpio.c \
../system/driver/src/stm32g4xx_ll_opamp.c \
../system/driver/src/stm32g4xx_ll_spi.c \
../system/driver/src/stm32g4xx_ll_tim.c 

OBJS += \
./system/driver/src/stm32g4xx_hal.o \
./system/driver/src/stm32g4xx_hal_cortex.o \
./system/driver/src/stm32g4xx_hal_dac.o \
./system/driver/src/stm32g4xx_hal_exti.o \
./system/driver/src/stm32g4xx_hal_fdcan.o \
./system/driver/src/stm32g4xx_hal_gpio.o \
./system/driver/src/stm32g4xx_hal_pwr.o \
./system/driver/src/stm32g4xx_hal_pwr_ex.o \
./system/driver/src/stm32g4xx_hal_rcc.o \
./system/driver/src/stm32g4xx_hal_rcc_ex.o \
./system/driver/src/stm32g4xx_hal_tim.o \
./system/driver/src/stm32g4xx_hal_tim_ex.o \
./system/driver/src/stm32g4xx_hal_uart.o \
./system/driver/src/stm32g4xx_hal_uart_ex.o \
./system/driver/src/stm32g4xx_ll_adc.o \
./system/driver/src/stm32g4xx_ll_comp.o \
./system/driver/src/stm32g4xx_ll_dac.o \
./system/driver/src/stm32g4xx_ll_exti.o \
./system/driver/src/stm32g4xx_ll_gpio.o \
./system/driver/src/stm32g4xx_ll_opamp.o \
./system/driver/src/stm32g4xx_ll_spi.o \
./system/driver/src/stm32g4xx_ll_tim.o 

C_DEPS += \
./system/driver/src/stm32g4xx_hal.d \
./system/driver/src/stm32g4xx_hal_cortex.d \
./system/driver/src/stm32g4xx_hal_dac.d \
./system/driver/src/stm32g4xx_hal_exti.d \
./system/driver/src/stm32g4xx_hal_fdcan.d \
./system/driver/src/stm32g4xx_hal_gpio.d \
./system/driver/src/stm32g4xx_hal_pwr.d \
./system/driver/src/stm32g4xx_hal_pwr_ex.d \
./system/driver/src/stm32g4xx_hal_rcc.d \
./system/driver/src/stm32g4xx_hal_rcc_ex.d \
./system/driver/src/stm32g4xx_hal_tim.d \
./system/driver/src/stm32g4xx_hal_tim_ex.d \
./system/driver/src/stm32g4xx_hal_uart.d \
./system/driver/src/stm32g4xx_hal_uart_ex.d \
./system/driver/src/stm32g4xx_ll_adc.d \
./system/driver/src/stm32g4xx_ll_comp.d \
./system/driver/src/stm32g4xx_ll_dac.d \
./system/driver/src/stm32g4xx_ll_exti.d \
./system/driver/src/stm32g4xx_ll_gpio.d \
./system/driver/src/stm32g4xx_ll_opamp.d \
./system/driver/src/stm32g4xx_ll_spi.d \
./system/driver/src/stm32g4xx_ll_tim.d 


# Each subdirectory must supply rules for building sources it contributes
system/driver/src/%.o system/driver/src/%.su system/driver/src/%.cyclo: ../system/driver/src/%.c system/driver/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DEXTERNAL_MODE -c -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app/474re" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/pil" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc_impl" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/cmsis/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/driver/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/portable/GCC/ARM_CM4F" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/CMSIS_RTOS" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg" -include"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg/plx_defines.h" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-system-2f-driver-2f-src

clean-system-2f-driver-2f-src:
	-$(RM) ./system/driver/src/stm32g4xx_hal.cyclo ./system/driver/src/stm32g4xx_hal.d ./system/driver/src/stm32g4xx_hal.o ./system/driver/src/stm32g4xx_hal.su ./system/driver/src/stm32g4xx_hal_cortex.cyclo ./system/driver/src/stm32g4xx_hal_cortex.d ./system/driver/src/stm32g4xx_hal_cortex.o ./system/driver/src/stm32g4xx_hal_cortex.su ./system/driver/src/stm32g4xx_hal_dac.cyclo ./system/driver/src/stm32g4xx_hal_dac.d ./system/driver/src/stm32g4xx_hal_dac.o ./system/driver/src/stm32g4xx_hal_dac.su ./system/driver/src/stm32g4xx_hal_exti.cyclo ./system/driver/src/stm32g4xx_hal_exti.d ./system/driver/src/stm32g4xx_hal_exti.o ./system/driver/src/stm32g4xx_hal_exti.su ./system/driver/src/stm32g4xx_hal_fdcan.cyclo ./system/driver/src/stm32g4xx_hal_fdcan.d ./system/driver/src/stm32g4xx_hal_fdcan.o ./system/driver/src/stm32g4xx_hal_fdcan.su ./system/driver/src/stm32g4xx_hal_gpio.cyclo ./system/driver/src/stm32g4xx_hal_gpio.d ./system/driver/src/stm32g4xx_hal_gpio.o ./system/driver/src/stm32g4xx_hal_gpio.su ./system/driver/src/stm32g4xx_hal_pwr.cyclo ./system/driver/src/stm32g4xx_hal_pwr.d ./system/driver/src/stm32g4xx_hal_pwr.o ./system/driver/src/stm32g4xx_hal_pwr.su ./system/driver/src/stm32g4xx_hal_pwr_ex.cyclo ./system/driver/src/stm32g4xx_hal_pwr_ex.d ./system/driver/src/stm32g4xx_hal_pwr_ex.o ./system/driver/src/stm32g4xx_hal_pwr_ex.su ./system/driver/src/stm32g4xx_hal_rcc.cyclo ./system/driver/src/stm32g4xx_hal_rcc.d ./system/driver/src/stm32g4xx_hal_rcc.o ./system/driver/src/stm32g4xx_hal_rcc.su ./system/driver/src/stm32g4xx_hal_rcc_ex.cyclo ./system/driver/src/stm32g4xx_hal_rcc_ex.d ./system/driver/src/stm32g4xx_hal_rcc_ex.o ./system/driver/src/stm32g4xx_hal_rcc_ex.su ./system/driver/src/stm32g4xx_hal_tim.cyclo ./system/driver/src/stm32g4xx_hal_tim.d ./system/driver/src/stm32g4xx_hal_tim.o ./system/driver/src/stm32g4xx_hal_tim.su ./system/driver/src/stm32g4xx_hal_tim_ex.cyclo ./system/driver/src/stm32g4xx_hal_tim_ex.d ./system/driver/src/stm32g4xx_hal_tim_ex.o ./system/driver/src/stm32g4xx_hal_tim_ex.su ./system/driver/src/stm32g4xx_hal_uart.cyclo ./system/driver/src/stm32g4xx_hal_uart.d ./system/driver/src/stm32g4xx_hal_uart.o ./system/driver/src/stm32g4xx_hal_uart.su ./system/driver/src/stm32g4xx_hal_uart_ex.cyclo ./system/driver/src/stm32g4xx_hal_uart_ex.d ./system/driver/src/stm32g4xx_hal_uart_ex.o ./system/driver/src/stm32g4xx_hal_uart_ex.su ./system/driver/src/stm32g4xx_ll_adc.cyclo ./system/driver/src/stm32g4xx_ll_adc.d ./system/driver/src/stm32g4xx_ll_adc.o ./system/driver/src/stm32g4xx_ll_adc.su ./system/driver/src/stm32g4xx_ll_comp.cyclo ./system/driver/src/stm32g4xx_ll_comp.d ./system/driver/src/stm32g4xx_ll_comp.o ./system/driver/src/stm32g4xx_ll_comp.su ./system/driver/src/stm32g4xx_ll_dac.cyclo ./system/driver/src/stm32g4xx_ll_dac.d ./system/driver/src/stm32g4xx_ll_dac.o ./system/driver/src/stm32g4xx_ll_dac.su ./system/driver/src/stm32g4xx_ll_exti.cyclo ./system/driver/src/stm32g4xx_ll_exti.d ./system/driver/src/stm32g4xx_ll_exti.o ./system/driver/src/stm32g4xx_ll_exti.su ./system/driver/src/stm32g4xx_ll_gpio.cyclo ./system/driver/src/stm32g4xx_ll_gpio.d ./system/driver/src/stm32g4xx_ll_gpio.o ./system/driver/src/stm32g4xx_ll_gpio.su ./system/driver/src/stm32g4xx_ll_opamp.cyclo ./system/driver/src/stm32g4xx_ll_opamp.d ./system/driver/src/stm32g4xx_ll_opamp.o ./system/driver/src/stm32g4xx_ll_opamp.su ./system/driver/src/stm32g4xx_ll_spi.cyclo ./system/driver/src/stm32g4xx_ll_spi.d ./system/driver/src/stm32g4xx_ll_spi.o ./system/driver/src/stm32g4xx_ll_spi.su ./system/driver/src/stm32g4xx_ll_tim.cyclo ./system/driver/src/stm32g4xx_ll_tim.d ./system/driver/src/stm32g4xx_ll_tim.o ./system/driver/src/stm32g4xx_ll_tim.su

.PHONY: clean-system-2f-driver-2f-src

