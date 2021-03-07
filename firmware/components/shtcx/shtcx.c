/*

An Arduino library for the Sensirion SHTC3 humidity and temerature sensor

*/
#include "esp_system.h"
#include "esp_log.h"
#include "shtcx.h"

static const char *TAG = "SHTCx";

SHTC3_MeasurementModes_TypeDef _mode;
static i2c_bus_handle_t g_i2c_bus = NULL;
static i2c_bus_device_handle_t g_i2c_dev = NULL;

bool _inProcess;    // True when a macro-scale function is in progress in the library - blocks small functions from messing with sleep functions
bool _staySleeping; // Used to indicate if the sensor should be kept in sleep mode
bool _isAsleep;     // Used to indicate if a wake() command is needed before talking

SHTC3_Status_TypeDef sht3cx_sendCommand(uint16_t cmd);
SHTC3_Status_TypeDef abortUpdate(SHTC3_Status_TypeDef status, const char *file, uint16_t line); // Used to bail from an update. Sets reading values to known fail state
SHTC3_Status_TypeDef exitOp(SHTC3_Status_TypeDef status, const char *file, uint16_t line);      // Used to bail from any other operation - puts the sensor back to sleep

SHTC3_Status_TypeDef startProcess(void); // Used to wake up the sensor and set the _inProcess variable to true
SHTC3_Status_TypeDef endProcess(void);   // Used to end processes as the user desires

void SHTC3_exitOp_Callback(SHTC3_Status_TypeDef status, bool inProcess, const char *file, uint16_t line) {} // Empty implementation. You can make your own implementation

void inline delay(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void inline delay_us(uint32_t us)
{
    ets_delay_us(us);
}


static void sht3cx_read(uint16_t *data, uint8_t *crc)
{
    uint8_t dat[5];
    i2c_bus_read_bytes(g_i2c_dev, NULL_I2C_MEM_ADDR, 3, dat);

    *data = ((dat[0] << 8) | dat[1]);
    *crc = dat[2];
}


SHTC3_Status_TypeDef sht3cx_sendCommand(uint16_t cmd)
{
    int res = 0;
    uint8_t dat[5];
    dat[0] = (cmd >> 8);
    dat[1] = (cmd & 0xFF);
    res = i2c_bus_write_bytes(g_i2c_dev, NULL_I2C_MEM_ADDR, 2, dat);

    if (res) {
        return SHTC3_Status_Error;
    }

    return SHTC3_Status_Nominal;
}


SHTC3_Status_TypeDef _sleep(void)
{
    _isAsleep = true; // It is fail-safe to always assume the sensor is asleep (whether or not the sleep command actually took effect)

    return exitOp(sht3cx_sendCommand(SHTC3_CMD_SLEEP), __FILE__, __LINE__);
}

SHTC3_Status_TypeDef wake(void)
{
    SHTC3_Status_TypeDef retval = sht3cx_sendCommand(SHTC3_CMD_WAKE);
    if (retval == SHTC3_Status_Nominal) {
        _isAsleep = false; // Only when the wake command was sent successfully can you assume the sensor is awake
    }

    delay_us(240);
    return exitOp(retval, __FILE__, __LINE__);
}

SHTC3_Status_TypeDef exitOp(SHTC3_Status_TypeDef status, const char *file, uint16_t line)
{
    if (status != SHTC3_Status_Nominal) {
        ESP_LOGE(TAG, "%s:%d error", file, line);
    }

    return status;
}

SHTC3_Status_TypeDef checkCRC(uint16_t packet, uint8_t cs)
{
    uint8_t upper = packet >> 8;
    uint8_t lower = packet & 0x00FF;
    uint8_t data[2] = {upper, lower};
    uint8_t crc = 0xFF;
    uint8_t poly = 0x31;

    for (uint8_t indi = 0; indi < 2; indi++) {
        crc ^= data[indi];

        for (uint8_t indj = 0; indj < 8; indj++) {
            if (crc & 0x80) {
                crc = (uint8_t)((crc << 1) ^ poly);
            } else {
                crc <<= 1;
            }
        }
    }

    if (cs ^ crc) {
        return exitOp(SHTC3_Status_CRC_Fail, __FILE__, __LINE__);
    }
    return exitOp(SHTC3_Status_Nominal, __FILE__, __LINE__);
}

SHTC3_Status_TypeDef sht3cx_init(void)
{
    SHTC3_Status_TypeDef retval = SHTC3_Status_Nominal;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 5,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 4,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    g_i2c_bus = i2c_bus_create(I2C_NUM_0, &conf);
    g_i2c_dev = i2c_bus_device_create(g_i2c_bus, SHTC3_ADDR_7BIT, conf.master.clk_speed);

    retval = sht3cx_checkID(); // Verify that the sensor is actually an SHTC3
    if (retval != SHTC3_Status_Nominal) {
        return exitOp(retval, __FILE__, __LINE__);
    }

    return exitOp(retval, __FILE__, __LINE__);
}

SHTC3_Status_TypeDef sht3cx_softReset()
{
    return exitOp(sht3cx_sendCommand(SHTC3_CMD_SFT_RST), __FILE__, __LINE__);
    delay_us(500);
}

SHTC3_Status_TypeDef sht3cx_checkID(void)
{
    SHTC3_Status_TypeDef retval = SHTC3_Status_Nominal;

    retval = wake(); // There will be several commands sent before returning control to the user
    if (retval != SHTC3_Status_Nominal) {

    }
    delay_us(240);
    retval = sht3cx_sendCommand(SHTC3_CMD_READ_ID);
    if (retval != SHTC3_Status_Nominal) {
        return exitOp(retval, __FILE__, __LINE__);
    }

    uint8_t IDcrc;
    uint16_t ID;
    sht3cx_read(&ID, &IDcrc);
    if (checkCRC(ID, IDcrc) != SHTC3_Status_Nominal) {
        ESP_LOGE(TAG, "checkCRC failed");
    }
    ESP_LOGI(TAG, "sht3cx ID=%x", ID);

    if ((ID & 0b0000100000111111) != 0b0000100000000111) {       // Checking the form of the ID
        // Bits 11 and 5-0 must match
        return exitOp(SHTC3_Status_ID_Fail, __FILE__, __LINE__); // to identify an SHTC3
    }

    retval = _sleep(); // We are about to return to user-land
    if (retval != SHTC3_Status_Nominal) {
        return exitOp(retval, __FILE__, __LINE__);
    }

    return exitOp(retval, __FILE__, __LINE__);
}

SHTC3_Status_TypeDef sht3cx_setMode(SHTC3_MeasurementModes_TypeDef mode)
{
    SHTC3_Status_TypeDef retval = SHTC3_Status_Nominal;
    _mode = mode;
    retval = sht3cx_sendCommand(_mode);
    return exitOp(retval, __FILE__, __LINE__);
}

SHTC3_MeasurementModes_TypeDef sht3cx_getMode(void)
{
    return _mode;
}

float SHTC3_raw2DegC(uint16_t T)
{
    return -45 + 175 * ((float)T / 65535);
}

float SHTC3_raw2DegF(uint16_t T)
{
    return SHTC3_raw2DegC(T) * (9.0 / 5) + 32.0;
}

float SHTC3_raw2Percent(uint16_t RH)
{
    return 100 * ((float)RH / 65535);
}

SHTC3_Status_TypeDef sht3cx_get_data(float *humi, float *temp)
{
    SHTC3_Status_TypeDef retval = SHTC3_Status_Nominal;
    uint8_t dat[6];

    retval = wake();
    if (retval != SHTC3_Status_Nominal) {
        return exitOp(retval, __FILE__, __LINE__);
    }
    delay_us(400);
    _mode = SHTC3_CMD_CSE_RHF_LPM;
    retval = sht3cx_sendCommand(_mode); // Send the appropriate command - Note: incorrect commands are excluded by the 'setMode' command and _mode is a protected variable
    if (retval != SHTC3_Status_Nominal) {
        return exitOp(retval, __FILE__, __LINE__);
    }
    delay(100);
    switch (_mode) { // Handle the two different ways of waiting for a measurement (polling or clock stretching)
    case SHTC3_CMD_CSE_RHF_NPM:
    case SHTC3_CMD_CSE_RHF_LPM:
    case SHTC3_CMD_CSE_TF_NPM:
    case SHTC3_CMD_CSE_TF_LPM:     // Address+read will yield an ACK and then clock stretching will occur
        if (0 != i2c_bus_read_bytes(g_i2c_dev, NULL_I2C_MEM_ADDR, 6, dat)) {
            exitOp(SHTC3_Status_Error, __FILE__, __LINE__);
        }

        break;

    case SHTC3_CMD_CSD_RHF_NPM:
    case SHTC3_CMD_CSD_RHF_LPM:
    case SHTC3_CMD_CSD_TF_NPM:
    case SHTC3_CMD_CSD_TF_LPM: // These modes not yet supported (polling - need to figure out how to repeatedly send just address+read and look for ACK)
    default:
        return exitOp(SHTC3_Status_Error, __FILE__, __LINE__); // You really should never get to this code because setMode disallows non-approved values of _mode (type SHTC3_MeasurementModes_TypeDef)
        break;
    }

    // Update values
    uint16_t RH = ((uint16_t)dat[0] << 8) | ((uint16_t)dat[1] << 0);
    uint16_t T = ((uint16_t)dat[3] << 8) | ((uint16_t)dat[4] << 0);

    if (checkCRC(RH, dat[2]) != SHTC3_Status_Nominal) {
        exitOp(SHTC3_Status_Error, __FILE__, __LINE__);
    }
    if (checkCRC(T, dat[5]) != SHTC3_Status_Nominal) {
        exitOp(SHTC3_Status_Error, __FILE__, __LINE__);
    }

    *humi = SHTC3_raw2Percent(RH);
    *temp = SHTC3_raw2DegC(T);

    retval = _sleep(); // We are about to return to user-land
    if (retval != SHTC3_Status_Nominal) {
        return exitOp(retval, __FILE__, __LINE__);
    }

    return exitOp(retval, __FILE__, __LINE__);
}



