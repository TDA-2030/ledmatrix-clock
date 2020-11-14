/*

An Arduino library for the Sensirion SHTC3 humidity and temerature sensor

*/

#ifndef SF_SHTC3
#define SF_SHTC3

#include "esp_log.h"
#include "i2c_bus.h"


#define SHTC3_ADDR_7BIT 0b1110000

#define SHTC3_MAX_CLOCK_FREQ 1000000

typedef enum
{
    SHTC3_CMD_WAKE = 0x3517,
    SHTC3_CMD_SLEEP = 0xB098,
    SHTC3_CMD_SFT_RST = 0x805D,
    SHTC3_CMD_READ_ID = 0xEFC8,
} SHTC3_Commands_TypeDef;

typedef enum
{
    SHTC3_CMD_CSE_RHF_NPM = 0x5C24, // Clock stretching, RH first, Normal power mode
    SHTC3_CMD_CSE_RHF_LPM = 0x44DE, // Clock stretching, RH first, Low power mode
    SHTC3_CMD_CSE_TF_NPM = 0x7CA2,	// Clock stretching, T first, Normal power mode
    SHTC3_CMD_CSE_TF_LPM = 0x6458,	// Clock stretching, T first, Low power mode

    SHTC3_CMD_CSD_RHF_NPM = 0x58E0, // Polling, RH first, Normal power mode
    SHTC3_CMD_CSD_RHF_LPM = 0x401A, // Polling, RH first, Low power mode
    SHTC3_CMD_CSD_TF_NPM = 0x7866,	// Polling, T first, Normal power mode
    SHTC3_CMD_CSD_TF_LPM = 0x609C	// Polling, T first, Low power mode
} SHTC3_MeasurementModes_TypeDef;

typedef enum
{
    SHTC3_Status_Nominal = 0, // The one and only "all is good" return value
    SHTC3_Status_Error,		  // The most general of error values - can mean anything depending on the context
    SHTC3_Status_CRC_Fail,	  // This return value means the computed checksum did not match the provided value
    SHTC3_Status_ID_Fail	  // This status means that the ID of the device did not match the format for SHTC3
} SHTC3_Status_TypeDef;



SHTC3_Status_TypeDef sht3cx_init(void);
SHTC3_Status_TypeDef sht3cx_checkID(void);
SHTC3_Status_TypeDef sht3cx_get_data(float *humi, float *temp);


#endif /* SF_SHTC3 */