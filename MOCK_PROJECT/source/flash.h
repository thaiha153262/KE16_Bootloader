#include "MKE16Z4.h"
#include <stdint.h>
#ifndef FLASH_H
#define FLASH_H

typedef enum
{
	ProgramLongWord = 0x06U,
	EraseSector = 0x09U,
} FTFA_Command_t;

/**
 * @brief Load address to excute command
 * 
 * @param addr 
 */
void FTFA_ProgramFlashAddr(uint16_t *addr);

/**
 * @brief Load data to excute command
 * 
 * @param data 
 */
void FTFA_ProgramLongWord(uint32_t *data);

/**
 * @brief Command excute EraseSector 
 * @return None
 */
void FTFA_EraseSector();

/**
 * @brief Erase n sector from start address
 * 
 * @param start_addr 
 * @param num_sector 
 * @return None
 */
void erase_sector(uint16_t start_addr, uint16_t num_sector);

/**
 * @brief Init Flash module
 * 
 */
void FTFA_Init();

/**
 * @brief DeInit Flash module
 * 
 */
void FTFA_DeInit();

/**
 * This function writes a long word to the flash memory.
 * It calculates the address to write to
 *
 * @param address The base address to write to.
 * @param data_u32 The long word to write.
 * @param count_write The number of long words already written.
 */
void writeToFlash(uint16_t address, uint32_t data_u32, uint32_t count_write);

#endif
