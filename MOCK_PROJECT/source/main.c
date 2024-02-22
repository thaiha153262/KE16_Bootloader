/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "pin_mux.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "flash.h"
#include "booloader.h"
#include <math.h>
#include "uart.h"
#include "common.h"
#include "fsl_common.h"
#include "Lpit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPUART LPUART0
#define DEMO_LPUART_CLKSRC kCLOCK_ScgSysOscClk
#define DEMO_LPUART_CLK_FREQ CLOCK_GetFreq(kCLOCK_ScgSysOscClk)
#define DEMO_LPUART_IRQn LPUART0_IRQn
#define DEMO_LPUART_IRQHandler LPUART0_IRQHandler
#define START_SIGNAL_LENGTH 5
#define END_SIGNAL_LENGTH 3
#define VALUE_ERROR *(uint32_t *)0x3000U
#define IS_ERROR 0x1000000U
#define VALUE_DEFAULT_FLASH 0xFFFFFFFF

typedef void (*funtionPointer)(void);
#define MAIN_APP_ADDRESS 0x8800U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
static void BootJump(uint32_t Address);

/*******************************************************************************
 * Code
 ******************************************************************************/
SignalError SignalErrors;
SignalDelete SignalDeletee;
BootloaderState bootState;
SRec currentRecord;
uint32_t dataIndex = 0;
uint8_t databack;
uint8_t complete;
uint8_t complete2;
volatile uint32_t new_firmware_error = 0;
volatile uint16_t txIndex;
volatile uint16_t rxIndex = 0;
uint8_t receivingData = 'f';
uint8_t receivedByte1;
uint8_t receivedByte;
uint8_t count_message;
uint8_t index_message_Error;
uint32_t total_line;
volatile uint32_t address = 0;
volatile uint8_t size;
uint32_t address_app = 0x6000U;
uint32_t address_appbackup = 0x4400U;
uint32_t timer_count = 0;
uint8_t exit_loop = 0;
uint8_t flag_interrupt = 0;
uint8_t flag_button = 1;
volatile uint32_t address_flash = 0;
volatile uint8_t flag_lpit_interrupt;
void PORTD_DeInit();
void initButton();
void Peripheral_DeInit();

int main(void)
{
    /* Initialize FTFA */
    FTFA_Init();

    /* Initialize UART */
    UART_Init();

    /* Initialize the button */
    initButton();

    /* Initialize the Lpit */
    LPIT_Init();

    flag_button = (GPIOD->PDIR) & (1 << 2U);
    /* Modify if you want to press  */
    if (flag_button==0)
    {
        bootloader();
    }
    /* Check if there is an error and if the backup address is not empty */
    if (VALUE_ERROR == IS_ERROR && *(uint32_t *)address_appbackup != 0xFFFFFFFF)
    {
        BootJump(address_appbackup);
    }
    else
    {
        /* If the app address is default and the backup address is not default, jump to the backup */
        if ((*(uint32_t *)address_app == VALUE_DEFAULT_FLASH) && (*(uint32_t *)address_appbackup != VALUE_DEFAULT_FLASH))
        {
            BootJump(address_appbackup);
        }

        /* If the app address is not default, jump to the app */
        else if (*(uint32_t *)address_app != VALUE_DEFAULT_FLASH)
        {
            BootJump(address_app);
        }

        /* If there is no app and no backup, return to the bootloader */
        else
        {
            SEND_RESPONE("NO APP , NO BACKUP, PLEASE BACK TO BOOTLOADER");
        }
    }
}
static void BootJump(uint32_t Address)
{
    /* 1. Make sure, the CPU is in privileged mode */
    if (CONTROL_nPRIV_Msk & __get_CONTROL())
    {
        /* not in privileged mode */
        __set_CONTROL(__get_CONTROL() & ~CONTROL_nPRIV_Msk);
    }

    /* 2. Disable all enabled interrupts in NVIC. */
    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICER[1] = 0xFFFFFFFF;
    NVIC->ICER[2] = 0xFFFFFFFF;
    NVIC->ICER[3] = 0xFFFFFFFF;
    NVIC->ICER[4] = 0xFFFFFFFF;
    NVIC->ICER[5] = 0xFFFFFFFF;
    NVIC->ICER[6] = 0xFFFFFFFF;
    NVIC->ICER[7] = 0xFFFFFFFF;

    /* 3.Disable all enabled peripherals which might generate interrupt requests, and clear all pending interrupt flags in those peripherals. Because this is device-specific, refer to the device datasheet for the proper way to clear these peripheral interrupts. */
    Peripheral_DeInit();

    /* 4. Clear all pending interrupt requests in NVIC.*/
    NVIC->ICPR[0] = 0xFFFFFFFF;
    NVIC->ICPR[1] = 0xFFFFFFFF;
    NVIC->ICPR[2] = 0xFFFFFFFF;
    NVIC->ICPR[3] = 0xFFFFFFFF;
    NVIC->ICPR[4] = 0xFFFFFFFF;
    NVIC->ICPR[5] = 0xFFFFFFFF;
    NVIC->ICPR[6] = 0xFFFFFFFF;
    NVIC->ICPR[7] = 0xFFFFFFFF;

    /* 5. Disable SysTick and clear its exception pending bit, if it is used in the bootloader, e. g. by the RTX. */
    SysTick->CTRL = 0;
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

    /* 6. Disable individual fault handlers if the bootloader used them. */
    SCB->CCR &= ~(SCB_CCR_UNALIGN_TRP_Msk |
                  SCB_CCR_STKALIGN_Msk);

    /* 7. Activate the MSP, if the core is found to currently run with the PSP. As the compiler might still use the stack, the PSP needs to be copied to the MSP before this. */
    if (CONTROL_SPSEL_Msk & __get_CONTROL())
    {
        __set_MSP(__get_PSP());
        __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);
    }

    /* 8. Load the vector table address of the user application into SCB->VTOR register. Make sure the address meets the alignment requirements.*/
    SCB->VTOR = (uint32_t)Address;

    /* 9. The final part is to set the MSP to the value found in the user application vector table and then load the PC with the reset vector value of the user application. This can't be done in C, as it is always possible, that the compiler uses the current SP.*/
    __set_MSP(*(uint32_t *)Address);

    funtionPointer JUMP_ADDRESS;
    /* reset address */
    JUMP_ADDRESS = (funtionPointer)(*(uint32_t *)(Address + 4));
    JUMP_ADDRESS();
}
void initButton()
{
    PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC_MASK;
    PORTD->PCR[2] &= ~(PORT_PCR_MUX_MASK);
    PORTD->PCR[2] |= PORT_PCR_MUX(1) | (1 << 1U) | 1;
    GPIOD->PDDR &= ~(1U << 2U);
}
void Peripheral_DeInit()
{
    /* PORT DeInit */
    PCC->CLKCFG[PCC_PORTD_INDEX] &= ~PCC_CLKCFG_CGC_MASK;

    /* UART DeInit */
    UART_Deinit();

    /* FTFA_DeInit */
    FTFA_DeInit();

    /* ADC DeInit */
    PCC->CLKCFG[PCC_ADC0_INDEX] &= ~PCC_CLKCFG_CGC_MASK;
}
/***************************************************************************************************
 *End of file
 **************************************************************************************************/
