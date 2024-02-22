################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Lpit.c \
../source/bootloader.c \
../source/flash.c \
../source/lpuart_interrupt.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/uart.c 

C_DEPS += \
./source/Lpit.d \
./source/bootloader.d \
./source/flash.d \
./source/lpuart_interrupt.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/uart.d 

OBJS += \
./source/Lpit.o \
./source/bootloader.o \
./source/flash.o \
./source/lpuart_interrupt.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/uart.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DFRDM_KE16Z -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\source" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\utilities" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\drivers" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\device" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\component\uart" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\component\lists" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\CMSIS" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\board" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\frdmke16z\driver_examples\lpuart\interrupt" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Lpit.d ./source/Lpit.o ./source/bootloader.d ./source/bootloader.o ./source/flash.d ./source/flash.o ./source/lpuart_interrupt.d ./source/lpuart_interrupt.o ./source/main.d ./source/main.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/uart.d ./source/uart.o

.PHONY: clean-source

