
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

extern uint8_t receivedByte1;
extern uint8_t receivedByte;
extern uint8_t databack;
extern volatile uint16_t rxIndex;
extern uint8_t flag_interrupt;

extern volatile uint8_t flag_lpit_interrupt;

extern uint8_t exit_loop;
extern uint32_t timer_count;
extern volatile uint32_t new_firmware_error;
extern volatile uint32_t address;
extern volatile uint8_t size;
extern uint32_t address_app;
extern uint8_t index_message_Error;
//volatile uint8_t delete_app;



typedef enum
{
    SIGNAL,
    DEFAULT,
    TYPE,
    BYTE_COUNT,
    ADDRESS,
    DATA,
    MESSAGE,
    GET_DATA_FROM_SIGNAL,
    END_MESSAGE,

} RecordState;

typedef struct
{

    uint8_t type;
    uint8_t byteCount;
    uint32_t address;
    uint8_t data[8];
    uint8_t checksum;
    uint8_t numberOfAddress;
    uint8_t datalength;
    RecordState state;
} SRec;

typedef enum
{
    Wait_signal,
    Check_signal,
    Get_srec,
    Finish_Recive_Write_data,
	ERROR_DATA
} BootloaderState;

typedef enum
{
    NOT_ERROR,
    DETECT_Signal,
    New_FIRm_ERROR,
} SignalError;

typedef enum
{
    DEFAULT2,
	CHECK_SIGNAL2,
	DELETE_APP,
} SignalDelete;




extern SignalError SignalErrors;
extern SRec currentRecord;
extern BootloaderState bootState;
extern SignalDelete SignalDeletee;


#endif // COMMON_H
