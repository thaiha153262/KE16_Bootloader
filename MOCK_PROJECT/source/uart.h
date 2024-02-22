#include <stdio.h>
#include <stdint.h>
#include <common.h>
#include <math.h>
#include "MKE16Z4.h"
#include "flash.h"
#include <string.h>
#ifndef _UART_H
#define _UART_H

/**
 * This function sends a response message.
 *
 * @param message Pointer to the message to be sent.
 */
void SEND_RESPONE(uint8_t *message);

/**
 * This function sends a string via UART.
 *
 * @param str Pointer to the string to be sent.
 * @param len Length of the string to be sent.
 */
void UART_SendStr(uint8_t *str, uint8_t len);

/**
 * This function initializes the UART.
 */
void UART_Init();

/**
 * This is the interrupt handler for LPUART0.
 */
void LPUART0_IRQHandler(void);

/**
 * This function deinitializes the UART.
 */
void UART_Deinit();
#endif
