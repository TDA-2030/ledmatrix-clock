/*

An Arduino library for the Sensirion SHTC3 humidity and temerature sensor

*/
#include "esp_system.h"
#include "esp_log.h"
#include "shtcx.h"
#include "i2c_bus.h"

static const char *TAG = "SHTCx";

#define ACK_CHECK_EN 1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0                       /*!< I2C master will not check ack from slave */

static i2c_bus_t *g_i2c_bus = NULL;

typedef enum {
    READ_ID            = 0xEFC8, // command: read ID register
    SOFT_RESET         = 0x805D, // soft reset
    SLEEP              = 0xB098, // sleep
    WAKEUP             = 0x3517, // wakeup
    MEAS_T_RH_POLLING  = 0x7866, // meas. read T first, clock stretching disabled
    MEAS_T_RH_CLOCKSTR = 0x7CA2, // meas. read T first, clock stretching enabled
    MEAS_RH_T_POLLING  = 0x58E0, // meas. read RH first, clock stretching disabled
    MEAS_RH_T_CLOCKSTR = 0x5C24  // meas. read RH first, clock stretching enabled
} etCommands;

typedef enum {
    NO_ERROR       = 0x00, // no error
    ACK_ERROR      = 0x01, // no acknowledgment error
    CHECKSUM_ERROR = 0x02 // checksum mismatch error
} etError;


static uint8_t _Address;

void inline delay_us(uint32_t us)
{
    ets_delay_us(us);
}

//------------------------------------------------------------------------------
void SHTC3_Init(void)
{
    _Address = SHTC3_ADDR_7BIT;
    i2c_config_t i2c_conf = DEFAULT_I2C_BUS_MASTER(5, 4);
    g_i2c_bus = i2c_bus_create(I2C_NUM_0, &i2c_conf);
}

//------------------------------------------------------------------------------
static int SHTC3_CheckCrc(uint8_t data[], uint8_t nbrOfBytes,
                          uint8_t checksum)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++) {
        crc ^= (data[byteCtr]);
        for (bit = 8; bit > 0; --bit) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc = (crc << 1);
            }
        }
    }

    // verify checksum
    if (crc != checksum) {
        return CHECKSUM_ERROR;
    } else {
        return NO_ERROR;
    }
}

//------------------------------------------------------------------------------
static float SHTC3_CalcTemperature(uint16_t rawValue)
{
    // calculate temperature [°C]
    // T = -45 + 175 * rawValue / 2^16
    return 175 * (float)rawValue / 65536.0f - 45.0f;
}

//------------------------------------------------------------------------------
static float SHTC3_CalcHumidity(uint16_t rawValue)
{
    // calculate relative humidity [%RH]
    // RH = rawValue / 2^16 * 100
    return 100 * (float)rawValue / 65536.0f;
}

//------------------------------------------------------------------------------
int SHTC3_GetTempAndHumi(float *temp, float *humi)
{
    int  error = 0;      // error code
    uint16_t rawValueTemp; // temperature raw value from sensor
    uint16_t rawValueHumi; // humidity raw value from sensor

    {
        uint16_t data_wr = MEAS_T_RH_CLOCKSTR;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_Address << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, data_wr >> 8, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, data_wr, ACK_CHECK_EN);
        esp_err_t ret = i2c_master_cmd_begin(g_i2c_bus->i2c_port, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        if (ESP_OK != ret) {
            ESP_LOGE(TAG, "11i2c master encounter error[%s]", esp_err_to_name(ret));
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
        uint8_t bytes[6] = {0}; // read data array
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_Address << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
        i2c_master_read(cmd, bytes, 6, ACK_CHECK_DIS);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(g_i2c_bus->i2c_port, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        if (ESP_OK != ret) {
            ESP_LOGE(TAG, "22i2c master encounter error[%s]", esp_err_to_name(ret));
        }

        if (NO_ERROR != SHTC3_CheckCrc(&bytes[0], 2, bytes[2])) { // verify checksum
            ESP_LOGE(TAG, "crc check failed");
        }
        if (NO_ERROR != SHTC3_CheckCrc(&bytes[3], 2, bytes[5])) { // verify checksum
            ESP_LOGE(TAG, "crc check failed");
        }

        // combine the two bytes to a 16-bit value
        rawValueTemp = (bytes[0] << 8) | bytes[1];
        rawValueHumi = (bytes[3] << 8) | bytes[4];

        *temp = SHTC3_CalcTemperature(rawValueTemp);
        *humi = SHTC3_CalcHumidity(rawValueHumi);
    }
//   error = SHTC3_StartWriteAccess();

//   // measure, read temperature first, clock streching enabled
//   error |= SHTC3_WriteCommand(MEAS_T_RH_CLOCKSTR);

//   // if no error, read temperature and humidity raw values
//   if(error == NO_ERROR) {
//     error |= SHTC3_StartReadAccess();
//     error |= SHTC3_Read2BytesAndCrc(&rawValueTemp);
//     error |= SHTC3_Read2BytesAndCrc(&rawValueHumi);
//   }

//   SHTC3_StopAccess();

//   // if no error, calculate temperature in °C and humidity in %RH
//   if(error == NO_ERROR) {
//     *temp = SHTC3_CalcTemperature(rawValueTemp);
//     *humi = SHTC3_CalcHumidity(rawValueHumi);
//   }

    return error;
}

//------------------------------------------------------------------------------
int SHTC3_GetTempAndHumiPolling(float *temp, float *humi)
{
    int  error = 0;         // error code
    // uint8_t  maxPolling = 20; // max. retries to read the measurement (polling)
    // uint16_t rawValueTemp;    // temperature raw value from sensor
    // uint16_t rawValueHumi;    // humidity raw value from sensor

    // error  = SHTC3_StartWriteAccess();

    // // measure, read temperature first, clock streching disabled (polling)
    // error |= SHTC3_WriteCommand(MEAS_T_RH_POLLING);

    // // if no error, ...
    // if (error == NO_ERROR) {
    //     // poll every 1ms for measurement ready
    //     while (maxPolling--) {
    //         // check if the measurement has finished
    //         error = SHTC3_StartReadAccess();

    //         // if measurement has finished -> exit loop
    //         if (error == NO_ERROR) {
    //             break;
    //         }

    //         // delay 1ms
    //         DelayMicroSeconds(1000);
    //     }

    //     // if no error, read temperature and humidity raw values
    //     if (error == NO_ERROR) {
    //         error |= SHTC3_Read2BytesAndCrc(&rawValueTemp);
    //         error |= SHTC3_Read2BytesAndCrc(&rawValueHumi);
    //     }
    // }

    // SHTC3_StopAccess();

    // // if no error, calculate temperature in °C and humidity in %RH
    // if (error == NO_ERROR) {
    //     *temp = SHTC3_CalcTemperature(rawValueTemp);
    //     *humi = SHTC3_CalcHumidity(rawValueHumi);
    // }

    return error;
}

static int sht3cx_sendCommand(uint16_t cmd)
{
    int8_t res = 0;
    i2c_set_address(g_i2c_bus, SHTC3_ADDR_7BIT);
    uint8_t dat[5];
    dat[0] = (cmd >> 8);
    dat[1] = (cmd & 0xFF);
    res = i2c_master_write_slave(g_i2c_bus, dat, 2);
    return res;
}

//------------------------------------------------------------------------------
int SHTC3_GetId(uint16_t *id)
{
    int error = 0; // error code

    {
        uint16_t data_wr = READ_ID;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_Address << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, data_wr >> 8, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, data_wr, ACK_CHECK_EN);
        esp_err_t ret = i2c_master_cmd_begin(g_i2c_bus->i2c_port, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        if (ESP_OK != ret) {
            ESP_LOGE(TAG, "i2c master encounter error[%s]", esp_err_to_name(ret));
        }

        uint8_t bytes[6]; // read data array
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (_Address << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
        i2c_master_read(cmd, bytes, 6, I2C_MASTER_ACK);
        i2c_master_stop(cmd);
        ret = i2c_master_cmd_begin(g_i2c_bus->i2c_port, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);
        if (ESP_OK != ret) {
            ESP_LOGE(TAG, "i2c master encounter error[%s]", esp_err_to_name(ret));
        }

        if (NO_ERROR != SHTC3_CheckCrc(&bytes[0], 2, bytes[2])) { // verify checksum
            ESP_LOGE(TAG, "crc check failed");
        }

        // combine the two bytes to a 16-bit value
        *id = (bytes[0] << 8) | bytes[1];
    }

    // error = SHTC3_StartWriteAccess();

    // // write ID read command
    // error |= SHTC3_WriteCommand(READ_ID);

    // // if no error, read ID
    // if (error == NO_ERROR) {
    //     SHTC3_StartReadAccess();
    //     error = SHTC3_Read2BytesAndCrc(id);
    // }

    // SHTC3_StopAccess();

    return error;
}

//------------------------------------------------------------------------------
int SHTC3_Sleep(void)
{
    return sht3cx_sendCommand(SLEEP);
}

//------------------------------------------------------------------------------
int SHTC3_Wakeup(void)
{
    int error = sht3cx_sendCommand(WAKEUP);

    delay_us(100); // wait 100 us

    return error;
}

//------------------------------------------------------------------------------
int SHTC3_SoftReset(void)
{
    return sht3cx_sendCommand(SOFT_RESET);
}

// //------------------------------------------------------------------------------
// int SHTC3_StartWriteAccess(void)
// {
//     int error; // error code

//     // write a start condition
//     I2c_StartCondition();

//     // write the sensor I2C address with the write flag
//     error = I2c_WriteByte(_Address << 1);

//     return error;
// }

// //------------------------------------------------------------------------------
// int SHTC3_StartReadAccess(void)
// {
//     int error; // error code

//     // write a start condition
//     I2c_StartCondition();

//     // write the sensor I2C address with the read flag
//     error = I2c_WriteByte((_Address << 1) | 0x01);

//     return error;
// }

// //------------------------------------------------------------------------------
// void SHTC3_StopAccess(void)
// {
//     // write a stop condition
//     I2c_StopCondition();
// }

// //------------------------------------------------------------------------------
// static int SHTC3_WriteCommand(etCommands cmd)
// {
//     int error; // error code

//     // write the upper 8 bits of the command to the sensor
//     error = I2c_WriteByte(cmd >> 8);

//     // write the lower 8 bits of the command to the sensor
//     error |= I2c_WriteByte(cmd & 0xFF);

//     return error;
// }

// //------------------------------------------------------------------------------
// static int SHTC3_Read2BytesAndCrc(uint16_t *data)
// {
//     int error;    // error code
//     uint8_t bytes[2]; // read data array
//     uint8_t checksum; // checksum byte

//     // read two data bytes and one checksum byte
//     bytes[0] = I2c_ReadByte(ACK);
//     bytes[1] = I2c_ReadByte(ACK);
//     checksum = I2c_ReadByte(ACK);

//     // verify checksum
//     error = SHTC3_CheckCrc(bytes, 2, checksum);

//     // combine the two bytes to a 16-bit value
//     *data = (bytes[0] << 8) | bytes[1];

//     return error;
// }



