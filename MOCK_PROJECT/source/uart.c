#include "uart.h"

#define SIGNAL_END_OF_FILE 'Z'
#define SIGNAL_START_MESSAGE 'B'
#define SIGNAL_END_MESSAGE   'E'
#define START_LINE_SREC     'S'
#define LENGTH_OF_MESSAGE_ERROR 4
char startSignal[5] = {0};
volatile uint8_t startSignalIndex = 0;
char endSignal[3] = {0};
volatile uint8_t endSignalIndex = 0;
uint8_t data_message;
uint8_t messageError[4] = {'R', 'R', 'O', 'R'};
uint8_t messageStart[4] = {'E', 'G', 'I', 'N'};
uint8_t messageEnd[4] = {'N', 'D'};
uint8_t messageDelete[4]={'E','L','T','E'};

uint8_t tempData[8] = {0};
uint8_t tempData2[5] = {0};
volatile uint32_t data_u32;
volatile uint32_t index_data = 0;
uint8_t message[10];
uint8_t count_byte = 0;
volatile uint32_t addressValue = 0;
volatile uint8_t addressIndex = 0;
uint32_t dataring;
volatile uint32_t count_write;
volatile uint8_t index_message_delete;
volatile uint8_t calculate_checksum;

uint8_t charToHex(uint8_t ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return ch - '0';
    }
    else if (ch >= 'A' && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    else if (ch >= 'a' && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    else
    {
        return 0xFF;
    }
}
void SEND_RESPONE(uint8_t *message)
{
    UART_SendStr((uint8_t *)message, strlen(message));
}

void UART_SendStr(uint8_t *str, uint8_t len)
{
    /* Enable UART transmitter */
    LPUART0->CTRL |= LPUART_CTRL_TE(1U);

    uint8_t i;
    for (i = 0; i < len; i++)
    {
        /* Wait until the buffer is empty */
        while (0U == (LPUART0->STAT & LPUART_STAT_TDRE_MASK))
            ;

        /* Write to data buffer */
        LPUART0->DATA = str[i];
    }
    /* Disable UART transmitter */
    LPUART0->CTRL &= ~LPUART_CTRL_TE_MASK;
}
void extractAddressAndSize1()
{
    for (int i = 0; i < currentRecord.datalength - 1; i++)
    {
        address += tempData[i] << ((currentRecord.datalength - 1 - i - 1) * 4);
    }
    size = tempData2[data_message - 1];
}

void extractData()
{
    for (int i = 0; i < index_data; i++)
    {
        data_u32 += currentRecord.data[i] << ((index_data - i - 1) * 4);
    }
}

void extractAddressAndSize()
{
    for (int i = 0; i < data_message - 1; i++)
    {
        address += tempData2[i] << ((data_message - 1 - i - 1) * 4);
    }
    size = tempData2[data_message - 1];
}
void LPUART0_IRQHandler(void)
{
	uint8_t data;
	/* Check if there is data to be read */
	if (LPUART0->STAT & LPUART_STAT_RDRF_MASK)
	{
	    /* Read the received byte and convert it to hexadecimal */
	    receivedByte1 = (uint8_t)(LPUART0->DATA);
	    receivedByte = charToHex(receivedByte1);
	    databack = receivedByte1;
	    rxIndex++;

	    /* Calculate the checksum for odd index received bytes */
	    if (rxIndex > 2 && rxIndex % 2 != 0 && receivedByte != 255 && calculate_checksum =='T')
	    {
	        currentRecord.checksum += receivedByte << 4;
	    }
	    /* Calculate the checksum for even index received bytes */
	    else if (rxIndex > 2 && rxIndex % 2 == 0 && receivedByte != 255 && calculate_checksum =='T')
	    {
	        currentRecord.checksum += receivedByte;
	    }

	    /* Check if the end of file signal has been received */
	    if (receivedByte1 == SIGNAL_END_OF_FILE)
	    {
	        flag_interrupt = 1;
	        currentRecord.state = SIGNAL;
	        bootState = Finish_Recive_Write_data;
	    }

	    /* Handle the delete signal */
	    switch(SignalDeletee){
	        case DEFAULT2:
	            if(receivedByte1 == 'D'){
	                SignalDeletee =CHECK_SIGNAL2;
	            }
	            break;
	        case CHECK_SIGNAL2:
	            if (receivedByte1 == messageDelete[index_message_delete])
	            {
	                index_message_delete++;
	                if (index_message_delete == 4)
	                    SignalDeletee = DELETE_APP;
	            }else{
	                SignalDeletee = DEFAULT2;
	            }
	            break;
	        default:
	            break;
	    }

	    /* Handle the error signal */
	    switch (SignalErrors)
	    {
	    case NOT_ERROR:
	    	/* If receive 'E' detect signal */
	        if (receivedByte1 == 'E')
	        {
	            SignalErrors = DETECT_Signal;
	        }
	        break;
	    case DETECT_Signal:
	    	/* If the receivedByte1 equal messsage Error */
	        if (receivedByte1 == messageError[index_message_Error])
	        {
	            index_message_Error++;
	            if (index_message_Error == 4)
	                SignalErrors = New_FIRm_ERROR;
	        }
	        else
	        {
	            SignalErrors = NOT_ERROR;
	        }
	        break;
	    default:
	       break;
	    }
	}


 /* Handle record state and Signal start */
    switch (currentRecord.state)
    {
    case SIGNAL:
    	/* If detect character 'B' convert state to check Signal start or not */
        if (receivedByte1 == 'B')
        {
            currentRecord.state = MESSAGE;
        }
        break;
    case DEFAULT:
    	/* If the received byte is 'S', set the checksum to calculate and change the state to TYPE */
        if (receivedByte1 == 'S')
        {
        	calculate_checksum= 'T';
            currentRecord.state = TYPE;
            rxIndex = 1;
        }
        break;

    case MESSAGE:
    	/* If the received byte matches the start of the message, increment the startSignalIndex */
        if (receivedByte1 == messageStart[startSignalIndex])
        {
            startSignalIndex++;
            /* If the startSignalIndex reaches 4, change the state to GET_DATA_FROM_SIGNAL */
            if (startSignalIndex == 4)
            {
                currentRecord.state = GET_DATA_FROM_SIGNAL;
            }
        }
        else
        {
        	/* If the received byte does not match, reset the state to DEFAULT */
            currentRecord.state = DEFAULT;
        }
        break;
    case GET_DATA_FROM_SIGNAL:
    	/* If the received byte is 'E', copy the message to tempData2 and extract the address and size. */
        if (receivedByte1 == 'E')
        {
            memcpy(tempData2, message, sizeof(tempData2));
            extractAddressAndSize();
            currentRecord.state = SIGNAL;
            bootState = Check_signal;
            currentRecord.checksum = 0;
            rxIndex = 0;
        }
        else
        {
        	/* If the received byte is not 'E', add the received byte to the message and increment data_message */
            message[data_message] = receivedByte;
            data_message++;
        }
        break;
    case TYPE:
    	/* Set the currentRecord type to the received byte */
        currentRecord.type = receivedByte;

        /* If the received byte is 1, 2, or 3, set the number of addresses based on the type */
        if (receivedByte == 1 || receivedByte == 2 || receivedByte == 3)
        {
            currentRecord.numberOfAddress = (currentRecord.type + 1) * 2;
        }
        else
        {
        	/* otherwise set number of address to 4 */
            currentRecord.numberOfAddress = 4;
        }
        currentRecord.state = BYTE_COUNT;
        break;

    case BYTE_COUNT:
    	/* Calculate two character bytecount */
        if (count_byte == 0)
        {
            currentRecord.byteCount = (receivedByte) * 16;
            count_byte++;
        }
        else
        {
            currentRecord.byteCount += (receivedByte);
            /* change status upon completion */
            currentRecord.state = ADDRESS;
            count_byte = 0;
        }
        break;

    case ADDRESS:
    	/* Calculate addressValue */
        addressValue += receivedByte * pow(16, (currentRecord.numberOfAddress - 1 - addressIndex));
        addressIndex++;
        /* Reset the state upon calculate completion */
        if (addressIndex >= currentRecord.numberOfAddress)
        {
            currentRecord.address = addressValue;
            addressValue = 0;
            addressIndex = 0;
            currentRecord.state = DATA;
        }

        break;
    case DATA:
    	/* If the received byte is not 255 and receivedByte1 is not '\r', increment index_data */
        if (receivedByte != 255 && receivedByte1 != '\r')
        {
            index_data++;

            /* If index_data is greater than or equal to 8 */
            if (index_data >= 8)
            {
            	/* set the last data byte to the received byte */
                currentRecord.data[index_data - 1] = (receivedByte);

                /* copy the data to tempData */
                memcpy(tempData, currentRecord.data, sizeof(currentRecord.data));

                /* extract the data */
                extractData();

                /* Write to flash */
                writeToFlash(currentRecord.address, data_u32, count_write);

                /* Increment count write to flash and reset the data */
                count_write++;
                memset(currentRecord.data, 0, sizeof(currentRecord.data));

                /* reset data another variable */
                data_u32 = 0;
                index_data = 0;
                currentRecord.datalength = 0;
            }
            else
            {
            	/*  If index_data is less than 8, set the current data byte to the received byte and set datalength to index_data */
                currentRecord.data[index_data - 1] = (receivedByte);
                currentRecord.datalength = index_data;
            }
        }
        else
        {
        	/* If the received byte is 255 or receivedByte1 is '\r', copy the data to tempData */
            memcpy(tempData, currentRecord.data, sizeof(currentRecord.data));

            /* Extract the data */
            extractData();

            /* Write the remaining data to flash if there are more than 2 bytes */
            if (index_data > 2)
            {
                index_data -= 2;
                writeToFlash(currentRecord.address, data_u32, count_write);
                count_write++;
            }
            else
            {
            	/* Reset index_data */
                index_data = 0;
            }
            /* If the checksum is not 0xFF */
            if (currentRecord.checksum != 0xFF)
            {
            	/*  reset the state to SIGNAL, set the bootState to ERROR_DATA, send an error response */
                currentRecord.state = SIGNAL;
                bootState = ERROR_DATA;
                SEND_RESPONE("ERROR CHECKSUM , PLEASE SEND FILE AGAIN");

                /* Erase the sector*/
                erase_sector(address, size);
            }

            /* Reset the struct currentRecord and another variable */
            memset(currentRecord.data, 0, sizeof(currentRecord.data));
            count_write = 0;
            index_data = 0;
            data_u32 = 0;
            currentRecord.datalength -= 2;
            currentRecord.state = DEFAULT;
            currentRecord.address = 0;
            currentRecord.checksum = 0;
            currentRecord.byteCount = 0;
            currentRecord.numberOfAddress = 0;
            rxIndex = 0;
        }
        break;
    default:
        break;
    }
}
void UART_Deinit() {
    /* Wait for transmit to complete */
    while (!(LPUART0->STAT & LPUART_STAT_TC_MASK));

    /* Disable TX and RX */
    LPUART0->CTRL &= ~LPUART_CTRL_TE_MASK;
    LPUART0->CTRL &= ~LPUART_CTRL_RE_MASK;

    /* Clear all status flags */
    LPUART0->STAT |= (LPUART_STAT_RXEDGIF_MASK | LPUART_STAT_IDLE_MASK | LPUART_STAT_OR_MASK |
                      LPUART_STAT_NF_MASK | LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK);

    /* Disable the module */
    LPUART0->CTRL = 0U;

    /* Disable UART interrupts */
    NVIC_DisableIRQ(LPUART0_IRQn);

    /* Disable LPUART0 clock */
    PCC->CLKCFG[PCC_LPUART0_INDEX] &= ~PCC_CLKCFG_CGC_MASK;
}

void UART_Init()
{
    /* Configure the port */
    PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1U);

    /* Configure the PCR[1] of PORTB */
    PORTB->PCR[1] = (PORTB->PCR[1] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2U);
    PORTB->PCR[1] |= PORT_PCR_PE(1U);
    PORTB->PCR[1] |= PORT_PCR_PS(1U);

    /* Configure the PCR[0] of PORTB */
    PORTB->PCR[0] = (PORTB->PCR[0] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(2U);
    PORTB->PCR[0] |= PORT_PCR_PE(1U);
    PORTB->PCR[0] |= PORT_PCR_PS(1U);

    /* Configure the clock */
    SCG->SIRCCFG |= SCG_SIRCCFG_RANGE(1U);
    SCG->SIRCDIV = (SCG->SIRCDIV & ~SCG_SIRCDIV_SIRCDIV2_MASK) | SCG_SIRCDIV_SIRCDIV2(3U);

    /* Configure the LPUART0 clock */
    PCC->CLKCFG[PCC_LPUART0_INDEX] = (PCC->CLKCFG[PCC_LPUART0_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(2U);
    PCC->CLKCFG[PCC_LPUART0_INDEX] |= PCC_CLKCFG_CGC(1U);

    /* Disable UART */
    LPUART0->CTRL &= ~LPUART_CTRL_TE_MASK;
    LPUART0->CTRL &= ~LPUART_CTRL_RE_MASK;

    /* Configure the baud rate */
    LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_SBR_MASK) | LPUART_BAUD_SBR(52U);
    LPUART0->BAUD |= LPUART_BAUD_BOTHEDGE(1U);
    LPUART0->BAUD = (LPUART0->BAUD & ~LPUART_BAUD_OSR_MASK) | LPUART_BAUD_OSR(3U);

    /* Configure the frame */
    LPUART0->BAUD &= ~LPUART_BAUD_M10_MASK;
    LPUART0->CTRL &= ~LPUART_CTRL_M7_MASK;
    LPUART0->CTRL &= ~LPUART_CTRL_M_MASK;
    LPUART0->CTRL &= ~LPUART_CTRL_PE_MASK;
    LPUART0->BAUD &= ~LPUART_BAUD_SBNS_MASK;
    LPUART0->STAT &= ~LPUART_STAT_MSBF_MASK;

    /* Enable receive interrupt */
    LPUART0->CTRL |= LPUART_CTRL_RIE_MASK;

    /* Enable transmitter and receiver */
    LPUART0->CTRL |= LPUART_CTRL_TE_MASK;
    LPUART0->CTRL |= LPUART_CTRL_RE_MASK;

    /* Enable receive interrupt */
    LPUART0->CTRL |= LPUART_CTRL_RIE_MASK;

    /* Enable transmitter and receiver */
    LPUART0->CTRL |= LPUART_CTRL_TE_MASK;
    LPUART0->CTRL |= LPUART_CTRL_RE_MASK;

    /* Enable LPUART0 interrupt */
    NVIC_EnableIRQ(LPUART0_IRQn);
}

