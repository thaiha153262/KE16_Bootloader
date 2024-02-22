#include <booloader.h>
static void check_signal();


static void check_signal()
{
    /* Check if the address is less than 0x1000 */
    if (address < 0x1000)
    {
        SEND_RESPONE("address must greater than 0x1000.\n");
        bootState = Wait_signal;
    }
    /* Check if the address plus the size is greater than 0x10000 */
    else if (address + size * 0x400 > 0x10000)
    {
        SEND_RESPONE("Size of must be less than 0x10000.\n");
        bootState = Wait_signal;
    }
    /* If the address and size are OK, send data */
    else
    {
        SEND_RESPONE("Address and data are OK, you can send data ");
        bootState = Get_srec;
    }
}
BootloaderState bootloader()
{
    /* Set the initial state to wait for a signal */
    bootState = Wait_signal;
    SEND_RESPONE("\nBOOTLOADER");

    SEND_RESPONE("\nWAITING SIGNAL");
    exit_loop = 0;

	LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0_MASK;
    /* Loop until a signal is received or an error occurs or a signal delete app */
    while (exit_loop != 1 && bootState != Check_signal && SignalErrors != New_FIRm_ERROR && SignalDeletee != DELETE_APP)
    {
        /* If the timer count reaches 10000000, exit the loop */
        if (flag_lpit_interrupt == 1)
        {
            bootState = Wait_signal;
            exit_loop = 1;
            flag_lpit_interrupt == 0;
        }
    }
    /* If the signal is to delete the app, erase the sector and reset the signal */
    if(SignalDeletee == DELETE_APP){
    	erase_sector(address_app, 3);
    	SignalDeletee =DEFAULT2;
    	SEND_RESPONE("\nDELETED APP");
    }
    /* If the state is to check the signal, check the signal */
    if (bootState == Check_signal)
    {
        SEND_RESPONE("\nCHECK SIGNAL don't send data");
        check_signal();
    }
    /* If the state is to get the SREC, erase the sector and wait for data */
    if (bootState == Get_srec)
    {
        if (address != 0x4400U)
        {
            address_app = address;

        }
        erase_sector(address, size);
        currentRecord.state = DEFAULT;
        SEND_RESPONE("YOU can send data ");

        SEND_RESPONE("\nWAITING DATA");
        exit_loop = 0;
        /* Loop until data is received or an error occurs */
        while (exit_loop != 1 && bootState != Finish_Recive_Write_data && bootState != ERROR_DATA)
        {
            /* If the timer count reaches 100000000, exit the loop */
            if (timer_count >= 100000000)
            {
                bootState = Wait_signal;
                currentRecord.state = SIGNAL;
                exit_loop = 1;
            }
            else
            {
                timer_count++;
            }
        }
        /* If there is an error with the data, reset and ask for the data again */
        if (bootState == ERROR_DATA)
        {
            bootState = Wait_signal;
            currentRecord.state = SIGNAL;
            SEND_RESPONE("ERROR DATA, PLEASE RESET AND PASS AGAIN ");
        }
        /* If the data is received and written, program the flash address and erase the sector */
        if (bootState == Finish_Recive_Write_data)
        {
            SignalErrors = NOT_ERROR;
            uint16_t sector_addr = 0x3000;
            FTFA_ProgramFlashAddr(&sector_addr);
            FTFA_EraseSector();
        }
    }
    /* If there is an error with the new firmware, reset and run the old firmware */
    if (SignalErrors == New_FIRm_ERROR)
    {
        new_firmware_error = 1;
        uint32_t error = 1;
        SEND_RESPONE("PLEASE RESET IT WILL RUN OLD FIRMWARE");
        uint16_t sector_addr = 0x3000;
        FTFA_ProgramFlashAddr(&sector_addr);
        uint32_t *ptr = &error;
        FTFA_ProgramLongWord(ptr);
    }
    /* Return the state of the bootloader */
    SEND_RESPONE("\nexit");
    return bootState;


}
