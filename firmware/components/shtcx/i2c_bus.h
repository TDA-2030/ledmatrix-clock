
#include "esp_log.h"
#include "driver/i2c.h"


typedef struct {
    i2c_config_t i2c_conf;   /*!<I2C bus parameters*/
    i2c_port_t i2c_port;     /*!<I2C port number */
    uint16_t slave_Address;
} i2c_bus_t;


#define DEFAULT_I2C_BUS_MASTER(sda_io, scl_io) {  \
    .mode = I2C_MODE_MASTER,  \
    .sda_io_num = (sda_io),  \
    .sda_pullup_en = GPIO_PULLUP_ENABLE,  \
    .scl_io_num = (scl_io),  \
    .scl_pullup_en = GPIO_PULLUP_ENABLE,  \
    .master.clk_speed = 100000,  \
}

i2c_bus_t *i2c_bus_create(i2c_port_t port, i2c_config_t* conf);
esp_err_t i2c_bus_delete(i2c_bus_t *bus);
esp_err_t i2c_set_address(i2c_bus_t *bus, uint16_t addr);
esp_err_t i2c_master_read_slave(i2c_bus_t *bus, uint8_t *data_rd, size_t size);
esp_err_t i2c_master_write_slave(i2c_bus_t *bus, uint8_t *data_wr, size_t size);

