
#include "esp_log.h"
#include "driver/gpio.h"

void board_init(void)
{
    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1ULL << 2;
    gpio_config(&io_conf);
    gpio_set_level(2, 1);
}
