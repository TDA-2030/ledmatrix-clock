
#include <stdio.h>
#include "esp_log.h"
#include "i2c_bus.h"

static const char *TAG = "i2c bus";

#define I2C_BUS_CHECK(a, str, ret)  if(!(a)) {                                             \
    ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);      \
    return (ret);                                                                   \
    }

#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define ACK_CHECK_EN 1                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0                       /*!< I2C master will not check ack from slave */


i2c_bus_t *i2c_bus_create(i2c_port_t port, i2c_config_t* conf)
{
    I2C_BUS_CHECK(port < I2C_NUM_MAX, "I2C port error", NULL);
    I2C_BUS_CHECK(conf != NULL, "Pointer error", NULL);
    i2c_bus_t* bus = (i2c_bus_t*) malloc(sizeof(i2c_bus_t));
    I2C_BUS_CHECK(NULL != bus, "bus struct malloc failed", NULL);

    bus->i2c_conf = *conf;
    bus->i2c_port = port;
    esp_err_t ret = i2c_param_config(bus->i2c_port, &(bus->i2c_conf));
    if(ret != ESP_OK) {
        goto error;
    }
    ret = i2c_driver_install(bus->i2c_port, bus->i2c_conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if(ret != ESP_OK) {
        goto error;
    }
    return bus;

    error:
    if(bus) {
        free(bus);
    }
    return NULL;
}

esp_err_t i2c_bus_delete(i2c_bus_t *bus)
{
    I2C_BUS_CHECK(bus != NULL, "Handle error", ESP_FAIL);
    i2c_driver_delete(bus->i2c_port);
    free(bus);
    return ESP_OK;
}

esp_err_t i2c_set_address(i2c_bus_t *bus, uint16_t addr)
{
    bus->slave_Address = addr;
    return ESP_OK;
}

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
esp_err_t i2c_master_read_slave(i2c_bus_t *bus, uint8_t *data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (bus->slave_Address << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, I2C_MASTER_ACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(bus->i2c_port, cmd, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if(ESP_OK != ret){
        ESP_LOGE(TAG, "i2c master encounter err %s", esp_err_to_name(ret));
    }
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
esp_err_t i2c_master_write_slave(i2c_bus_t *bus, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (bus->slave_Address << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(bus->i2c_port, cmd, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if(ESP_OK != ret){
        ESP_LOGE(TAG, "i2c master encounter err %s", esp_err_to_name(ret));
    }
    return ret;
}

