#include "flash.h"
void FTFA_Init()
{
	/* Enable clock for FTFA */
	PCC->CLKCFG[PCC_FLASH_INDEX] |= PCC_CLKCFG_PCS(1U);
}
void FTFA_DeInit()
{
	/* Disable clock for FTFA */
	PCC->CLKCFG[PCC_FLASH_INDEX] &= ~PCC_CLKCFG_PCS_MASK;
}
void FTFA_ProgramFlashAddr(uint16_t *addr)
{
	/* Set lower byte of flash address */
	FTFA->FCCOB3 &= ~FTFA_FCCOB3_CCOBn_MASK;
	FTFA->FCCOB3 |= FTFA_FCCOB3_CCOBn(*addr);

	/* Set upper byte of flash address */
	FTFA->FCCOB2 &= ~FTFA_FCCOB2_CCOBn_MASK;
	FTFA->FCCOB2 |= FTFA_FCCOB2_CCOBn(((*addr) & 0xFF00) >> 8);
}
void FTFA_ProgramLongWord(uint32_t *data)
{
	/* Set command */
	FTFA->FCCOB0 &= ~FTFA_FCCOB0_CCOBn_MASK;
	FTFA->FCCOB0 |= FTFA_FCCOB0_CCOBn(ProgramLongWord);

	/* Set byte 0 of program value */
	FTFA->FCCOB4 &= ~FTFA_FCCOB4_CCOBn_MASK;
	FTFA->FCCOB4 |= FTFA_FCCOB4_CCOBn(*data);

	/* Set byte 1 of program value */
	FTFA->FCCOB5 &= ~FTFA_FCCOB5_CCOBn_MASK;
	FTFA->FCCOB5 |= FTFA_FCCOB5_CCOBn(((*data) & 0xFF00) >> 8);

	/* Set byte 2 of program value */
	FTFA->FCCOB6 &= ~FTFA_FCCOB6_CCOBn_MASK;
	FTFA->FCCOB6 |= FTFA_FCCOB6_CCOBn(((*data) & 0xFF0000) >> 16);

	/* Set byte 3 of program value */
	FTFA->FCCOB7 &= ~FTFA_FCCOB7_CCOBn_MASK;
	FTFA->FCCOB7 |= FTFA_FCCOB7_CCOBn(((*data) & 0xFF000000) >> 24);

	/* Start command and wait for it to complete */
	__disable_irq();
	FTFA->FSTAT |= FTFA_FSTAT_CCIF(1U);
	while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
	__enable_irq();
}
void erase_sector(uint16_t start_addr, uint16_t num_sector)
{
    for (int i = 0; i < num_sector; i++)
    {
        /* Calculate address of sector */
        uint16_t sector_addr = start_addr + (i * 1024);

        /* Program flash address */
        FTFA_ProgramFlashAddr(&sector_addr);

        /* Erase sector */
        FTFA_EraseSector();
    }
}
void writeToFlash(uint16_t address, uint32_t data_u32, uint32_t count_write)
{
    /* Calculate address to write to */
    address += (count_write * 4);

    /* Pointer to long word */
    uint32_t *ptr = (uint32_t *)&data_u32;

    /* Program flash address */
    FTFA_ProgramFlashAddr(&address);

    /* Program long word */
    FTFA_ProgramLongWord(ptr);
}
void FTFA_EraseSector()
{
	/* Set command */
	FTFA->FCCOB0 &= ~FTFA_FCCOB0_CCOBn_MASK;
	FTFA->FCCOB0 |= FTFA_FCCOB0_CCOBn(EraseSector);

	/* Start command and wait for it to complete */
	__disable_irq();
	FTFA->FSTAT |= FTFA_FSTAT_CCIF(1U);
	while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
	__enable_irq();
}
