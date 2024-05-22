################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc_stm32g4xx.c \
../src/canbus_stm32g4xx.c \
../src/comp_stm32g4xx.c \
../src/ctr_stm32g4xx.c \
../src/dac_stm32g4xx.c \
../src/dio_stm32g4xx.c \
../src/dispatcher_stm32g4xx.c \
../src/gpio_stm32g4xx.c \
../src/hrtim_pcc_stm32g4xx.c \
../src/hrtim_stm32g4xx.c \
../src/opamp_stm32g4xx.c \
../src/power.c \
../src/spi_stm32g4xx.c \
../src/tim_stm32g4xx.c \
../src/trig_stm32g4xx.c \
../src/uart_stm32g4xx.c 

OBJS += \
./src/adc_stm32g4xx.o \
./src/canbus_stm32g4xx.o \
./src/comp_stm32g4xx.o \
./src/ctr_stm32g4xx.o \
./src/dac_stm32g4xx.o \
./src/dio_stm32g4xx.o \
./src/dispatcher_stm32g4xx.o \
./src/gpio_stm32g4xx.o \
./src/hrtim_pcc_stm32g4xx.o \
./src/hrtim_stm32g4xx.o \
./src/opamp_stm32g4xx.o \
./src/power.o \
./src/spi_stm32g4xx.o \
./src/tim_stm32g4xx.o \
./src/trig_stm32g4xx.o \
./src/uart_stm32g4xx.o 

C_DEPS += \
./src/adc_stm32g4xx.d \
./src/canbus_stm32g4xx.d \
./src/comp_stm32g4xx.d \
./src/ctr_stm32g4xx.d \
./src/dac_stm32g4xx.d \
./src/dio_stm32g4xx.d \
./src/dispatcher_stm32g4xx.d \
./src/gpio_stm32g4xx.d \
./src/hrtim_pcc_stm32g4xx.d \
./src/hrtim_stm32g4xx.d \
./src/opamp_stm32g4xx.d \
./src/power.d \
./src/spi_stm32g4xx.d \
./src/tim_stm32g4xx.d \
./src/trig_stm32g4xx.d \
./src/uart_stm32g4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DEXTERNAL_MODE -c -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app/474re" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/pil" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc_impl" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/cmsis/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/driver/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/portable/GCC/ARM_CM4F" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/CMSIS_RTOS" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg" -include"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg/plx_defines.h" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/adc_stm32g4xx.cyclo ./src/adc_stm32g4xx.d ./src/adc_stm32g4xx.o ./src/adc_stm32g4xx.su ./src/canbus_stm32g4xx.cyclo ./src/canbus_stm32g4xx.d ./src/canbus_stm32g4xx.o ./src/canbus_stm32g4xx.su ./src/comp_stm32g4xx.cyclo ./src/comp_stm32g4xx.d ./src/comp_stm32g4xx.o ./src/comp_stm32g4xx.su ./src/ctr_stm32g4xx.cyclo ./src/ctr_stm32g4xx.d ./src/ctr_stm32g4xx.o ./src/ctr_stm32g4xx.su ./src/dac_stm32g4xx.cyclo ./src/dac_stm32g4xx.d ./src/dac_stm32g4xx.o ./src/dac_stm32g4xx.su ./src/dio_stm32g4xx.cyclo ./src/dio_stm32g4xx.d ./src/dio_stm32g4xx.o ./src/dio_stm32g4xx.su ./src/dispatcher_stm32g4xx.cyclo ./src/dispatcher_stm32g4xx.d ./src/dispatcher_stm32g4xx.o ./src/dispatcher_stm32g4xx.su ./src/gpio_stm32g4xx.cyclo ./src/gpio_stm32g4xx.d ./src/gpio_stm32g4xx.o ./src/gpio_stm32g4xx.su ./src/hrtim_pcc_stm32g4xx.cyclo ./src/hrtim_pcc_stm32g4xx.d ./src/hrtim_pcc_stm32g4xx.o ./src/hrtim_pcc_stm32g4xx.su ./src/hrtim_stm32g4xx.cyclo ./src/hrtim_stm32g4xx.d ./src/hrtim_stm32g4xx.o ./src/hrtim_stm32g4xx.su ./src/opamp_stm32g4xx.cyclo ./src/opamp_stm32g4xx.d ./src/opamp_stm32g4xx.o ./src/opamp_stm32g4xx.su ./src/power.cyclo ./src/power.d ./src/power.o ./src/power.su ./src/spi_stm32g4xx.cyclo ./src/spi_stm32g4xx.d ./src/spi_stm32g4xx.o ./src/spi_stm32g4xx.su ./src/tim_stm32g4xx.cyclo ./src/tim_stm32g4xx.d ./src/tim_stm32g4xx.o ./src/tim_stm32g4xx.su ./src/trig_stm32g4xx.cyclo ./src/trig_stm32g4xx.d ./src/trig_stm32g4xx.o ./src/trig_stm32g4xx.su ./src/uart_stm32g4xx.cyclo ./src/uart_stm32g4xx.d ./src/uart_stm32g4xx.o ./src/uart_stm32g4xx.su

.PHONY: clean-src

