################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_smc.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_smc.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_smc.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DFRDM_KE16Z -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\source" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\utilities" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\drivers" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\device" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\component\uart" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\component\lists" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\CMSIS" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\board" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\frdmke16z\driver_examples\lpuart\interrupt" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_smc.d ./drivers/fsl_smc.o

.PHONY: clean-drivers

