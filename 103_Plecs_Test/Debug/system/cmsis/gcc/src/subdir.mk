################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../system/cmsis/gcc/src/startup_stm32g474retx.S 

OBJS += \
./system/cmsis/gcc/src/startup_stm32g474retx.o 

S_UPPER_DEPS += \
./system/cmsis/gcc/src/startup_stm32g474retx.d 


# Each subdirectory must supply rules for building sources it contributes
system/cmsis/gcc/src/%.o: ../system/cmsis/gcc/src/%.S system/cmsis/gcc/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DSTM32G474xx -DUSE_HAL_DRIVER -DUSE_NUCLEO_64 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@" "$<"

clean: clean-system-2f-cmsis-2f-gcc-2f-src

clean-system-2f-cmsis-2f-gcc-2f-src:
	-$(RM) ./system/cmsis/gcc/src/startup_stm32g474retx.d ./system/cmsis/gcc/src/startup_stm32g474retx.o

.PHONY: clean-system-2f-cmsis-2f-gcc-2f-src

