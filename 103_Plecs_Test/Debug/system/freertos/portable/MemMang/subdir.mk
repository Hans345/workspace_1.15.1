################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/freertos/portable/MemMang/heap_4.c 

OBJS += \
./system/freertos/portable/MemMang/heap_4.o 

C_DEPS += \
./system/freertos/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
system/freertos/portable/MemMang/%.o system/freertos/portable/MemMang/%.su system/freertos/portable/MemMang/%.cyclo: ../system/freertos/portable/MemMang/%.c system/freertos/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DEXTERNAL_MODE -c -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app/474re" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/pil" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc_impl" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/cmsis/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/driver/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/portable/GCC/ARM_CM4F" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/CMSIS_RTOS" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg" -include"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg/plx_defines.h" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-system-2f-freertos-2f-portable-2f-MemMang

clean-system-2f-freertos-2f-portable-2f-MemMang:
	-$(RM) ./system/freertos/portable/MemMang/heap_4.cyclo ./system/freertos/portable/MemMang/heap_4.d ./system/freertos/portable/MemMang/heap_4.o ./system/freertos/portable/MemMang/heap_4.su

.PHONY: clean-system-2f-freertos-2f-portable-2f-MemMang

