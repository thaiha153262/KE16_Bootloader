################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DFRDM_KE16Z -DFREEDOM -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\source" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\utilities" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\drivers" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\device" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\component\uart" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\component\lists" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\CMSIS" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\board" -I"C:\Users\84396\Documents\MCUXpressoIDE_11.8.1_1197\workspace\frdmke16z34_lpuart_interrupt\frdmke16z\driver_examples\lpuart\interrupt" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

