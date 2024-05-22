################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/freertos/croutine.c \
../system/freertos/event_groups.c \
../system/freertos/list.c \
../system/freertos/queue.c \
../system/freertos/stream_buffer.c \
../system/freertos/tasks.c \
../system/freertos/timers.c 

OBJS += \
./system/freertos/croutine.o \
./system/freertos/event_groups.o \
./system/freertos/list.o \
./system/freertos/queue.o \
./system/freertos/stream_buffer.o \
./system/freertos/tasks.o \
./system/freertos/timers.o 

C_DEPS += \
./system/freertos/croutine.d \
./system/freertos/event_groups.d \
./system/freertos/list.d \
./system/freertos/queue.d \
./system/freertos/stream_buffer.d \
./system/freertos/tasks.d \
./system/freertos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
system/freertos/%.o system/freertos/%.su system/freertos/%.cyclo: ../system/freertos/%.c system/freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -DDEBUG -DEXTERNAL_MODE -c -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/app/474re" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/pil" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/inc_impl" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/cmsis/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/driver/inc" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/include" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/portable/GCC/ARM_CM4F" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/system/freertos/CMSIS_RTOS" -I"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg" -include"C:/Users/raphael.baumeler/STM32CubeIDE/workspace_1.15.1/103_Plecs_Test/cg/plx_defines.h" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-system-2f-freertos

clean-system-2f-freertos:
	-$(RM) ./system/freertos/croutine.cyclo ./system/freertos/croutine.d ./system/freertos/croutine.o ./system/freertos/croutine.su ./system/freertos/event_groups.cyclo ./system/freertos/event_groups.d ./system/freertos/event_groups.o ./system/freertos/event_groups.su ./system/freertos/list.cyclo ./system/freertos/list.d ./system/freertos/list.o ./system/freertos/list.su ./system/freertos/queue.cyclo ./system/freertos/queue.d ./system/freertos/queue.o ./system/freertos/queue.su ./system/freertos/stream_buffer.cyclo ./system/freertos/stream_buffer.d ./system/freertos/stream_buffer.o ./system/freertos/stream_buffer.su ./system/freertos/tasks.cyclo ./system/freertos/tasks.d ./system/freertos/tasks.o ./system/freertos/tasks.su ./system/freertos/timers.cyclo ./system/freertos/timers.d ./system/freertos/timers.o ./system/freertos/timers.su

.PHONY: clean-system-2f-freertos

