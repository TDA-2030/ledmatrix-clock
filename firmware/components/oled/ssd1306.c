// Copyright 2020 Espressif Systems (Shanghai) Co. Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "ssd1306.h"

static const char *TAG = "lcd ssd1306";

#define LCD_CHECK(a, str, ret)  if(!(a)) {                               \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);   \
        return (ret);                                                           \
    }

#define LCD_NAME "OLED SSD1306"
#define LCD_BPP  1

// Some fundamental define for screen controller
#define SSD1306_PAGES                          8
#define SSD1306_HEIGHT                         64
#define SSD1306_COLUMNS                        128

// Control byte
#define SSD1306_CONTROL_BYTE_CMD_SINGLE        0x80
#define SSD1306_CONTROL_BYTE_CMD_STREAM        0x00
#define SSD1306_CONTROL_BYTE_DATA_STREAM       0x40

// Fundamental commands (pg.28)
#define SSD1306_CMD_SET_CONTRAST               0x81    // follow with 0x7F
#define SSD1306_CMD_DISPLAY_RAM                0xA4
#define SSD1306_CMD_DISPLAY_ALLON              0xA5
#define SSD1306_CMD_DISPLAY_NORMAL             0xA6
#define SSD1306_CMD_DISPLAY_INVERTED           0xA7
#define SSD1306_CMD_DISPLAY_OFF                0xAE
#define SSD1306_CMD_DISPLAY_ON                 0xAF

// Display Scrolling Parameters
#define SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL              0x26  // Init rt scroll
#define SSD1306_CMD_LEFT_HORIZONTAL_SCROLL               0x27  // Init left scroll
#define SSD1306_CMD_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29  // Init diag scroll
#define SSD1306_CMD_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A  // Init diag scroll
#define SSD1306_CMD_DEACTIVATE_SCROLL                    0x2E  // Stop scroll
#define SSD1306_CMD_ACTIVATE_SCROLL                      0x2F  // Start scroll
#define SSD1306_CMD_SET_VERTICAL_SCROLL_AREA             0xA3  // Set scroll range

// Addressing Command Table (pg.30)
#define SSD1306_CMD_SET_LOWER_COLUMN_ADDR      0x00    // Set Lower Column Start Address for Page Addressing Mode, using X[3:0]
#define SSD1306_CMD_SET_HIGHER_COLUMN_ADDR     0x10    // Set Higher Column Start Address for Page Addressing Mode, using X[3:0]
#define SSD1306_CMD_SET_MEMORY_ADDR_MODE       0x20    // follow with 00b= Horizontal Addressing Mode; 01b=Vertical Addressing Mode; 10b= Page Addressing Mode (RESET)
#define SSD1306_CMD_SET_COLUMN_RANGE           0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define SSD1306_CMD_SET_PAGE_RANGE             0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7
#define SSD1306_CMD_SET_PAGE_ADDR              0xB0

// Hardware Config (pg.31)
#define SSD1306_CMD_SET_DISPLAY_START_LINE     0x40
#define SSD1306_CMD_SET_SEGMENT_REMAP          0xA1
#define SSD1306_CMD_SET_MUX_RATIO              0xA8    // follow with 0x3F = 64 MUX
#define SSD1306_CMD_SET_COM_SCAN_MODE_NORMAL   0xC0
#define SSD1306_CMD_SET_COM_SCAN_MODE_REMAP    0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET         0xD3    // follow with 0x00
#define SSD1306_CMD_SET_COM_PIN_MAP            0xDA    // follow with 0x12
#define SSD1306_CMD_NOP                        0xE3    // NOP

// Timing and Driving Scheme (pg.32)
#define SSD1306_CMD_SET_DISPLAY_CLK_DIV        0xD5    // follow with 0x80
#define SSD1306_CMD_SET_PRECHARGE              0xD9    // follow with 0xF1
#define SSD1306_CMD_SET_VCOMH_DESELCT          0xDB    // follow with 0x30

// Charge Pump (pg.62)
#define OLED_CMD_SET_CHARGE_PUMP               0x8D    // follow with 0x14

#define SSD1306_COLUMN_ADDR 0x00
#define SSD1306_LOWER_ADDRESS   (SSD1306_CMD_SET_LOWER_COLUMN_ADDR + (SSD1306_COLUMN_ADDR&0x0f))
#define SSD1306_HIGHER_ADDRESS  (SSD1306_CMD_SET_HIGHER_COLUMN_ADDR + ((SSD1306_COLUMN_ADDR&0xf0)>>4))

typedef struct {
    uint16_t original_width;
    uint16_t original_height;
    uint16_t width;
    uint16_t height;
    uint16_t offset_hor;
    uint16_t offset_ver;
    scr_dir_t dir;
} scr_handle_t;

static uint8_t g_gram[8][128]={0};
static scr_handle_t g_lcd_handle;

//--------interface start-------------
#define SSD1306_WRITE_CMD           0x00
#define SSD1306_WRITE_DAT           0x40

#define ACK_CHECK_EN                1                   /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS               0                   /*!< I2C master will not check ack from slave */

#include "i2c_bus.h"
static i2c_bus_device_handle_t g_i2c_dev;
static esp_err_t i2c_lcd_driver_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 18,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 19,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    i2c_bus_handle_t i2c0_bus_1 = i2c_bus_create(I2C_NUM_1, &conf);

    g_i2c_dev = i2c_bus_device_create(i2c0_bus_1, 0x3C, conf.master.clk_speed);
    LCD_CHECK(NULL != g_i2c_dev, "I2C bus initial failed", ESP_FAIL);
    return ESP_OK;
}

static esp_err_t i2c_lcd_driver_deinit(void)
{
    i2c_bus_device_delete(&g_i2c_dev);
    return ESP_OK;
}

static esp_err_t i2c_lcd_write_byte(i2c_bus_device_handle_t i2c_dev, uint8_t ctrl, uint8_t data)
{
    esp_err_t ret;

    uint8_t buffer[2];
    buffer[0] = ctrl;
    buffer[1] = data;
    ret = i2c_bus_write_bytes(i2c_dev, NULL_I2C_MEM_ADDR, 2, buffer);
    if (ESP_OK != ret) {
        ESP_LOGE(TAG, "i2c send failed [%s]", esp_err_to_name(ret));
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t i2c_lcd_write_cmd(uint16_t cmd)
{
    uint8_t v = cmd;
    return i2c_lcd_write_byte(g_i2c_dev, SSD1306_WRITE_CMD, v);
}

static esp_err_t i2c_lcd_write_data(uint16_t data)
{
    uint8_t v = data;
    return i2c_lcd_write_byte(g_i2c_dev, SSD1306_WRITE_DAT, v);
}

static esp_err_t i2c_lcd_write(const uint8_t *data, uint32_t length)
{
    esp_err_t ret;
    ret = i2c_bus_write_bytes(g_i2c_dev, SSD1306_WRITE_DAT, length, (uint8_t *)data);
    LCD_CHECK(ESP_OK == ret, "i2C send failed", ESP_FAIL);
    return ESP_OK;
}
#define LCD_WRITE_CMD i2c_lcd_write_cmd
#define LCD_WRITE_DATA i2c_lcd_write_data
#define LCD_WRITE i2c_lcd_write
//--------interface end---------------

static esp_err_t lcd_ssd1306_refresh(void);

static void oled_refresh_task(void *args)
{
    while (1)
    {
        lcd_ssd1306_refresh();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

esp_err_t lcd_ssd1306_init(void)
{
    i2c_lcd_driver_init();

    g_lcd_handle.original_width = 128;
    g_lcd_handle.original_height = 64;

    LCD_WRITE_CMD(0xAE); //--turn off oled panel
    LCD_WRITE_CMD(0x00); //---set low column address
    LCD_WRITE_CMD(0x10); //---set high column address
    LCD_WRITE_CMD(0x40); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    LCD_WRITE_CMD(0x81); //--set contrast control register
    LCD_WRITE_CMD(0xCF); // Set SEG Output Current Brightness
    LCD_WRITE_CMD(0xA1); //--Set SEG/Column Mapping
    LCD_WRITE_CMD(0xC0); //Set COM/Row Scan Direction
    LCD_WRITE_CMD(0xA6); //--set normal display
    LCD_WRITE_CMD(0xA8); //--set multiplex ratio(1 to 64)
    LCD_WRITE_CMD(0x3f); //--1/64 duty
    LCD_WRITE_CMD(0xD3); //-set display offset  Shift Mapping RAM Counter (0x00~0x3F)
    LCD_WRITE_CMD(0x00); //-not offset
    LCD_WRITE_CMD(0xd5); //--set display clock divide ratio/oscillator frequency
    LCD_WRITE_CMD(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    LCD_WRITE_CMD(0xD9); //--set pre-charge period
    LCD_WRITE_CMD(0xF1); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    LCD_WRITE_CMD(0xDA); //--set com pins hardware configuration
    LCD_WRITE_CMD(0x12);
    LCD_WRITE_CMD(0xDB); //--set vcomh
    LCD_WRITE_CMD(0x40); //Set VCOM Deselect Level
    LCD_WRITE_CMD(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
    LCD_WRITE_CMD(0x02);
    LCD_WRITE_CMD(0x8D); //--set Charge Pump enable/disable
    LCD_WRITE_CMD(0x14); //--set(0x10) disable
    LCD_WRITE_CMD(0xA4); // Disable Entire Display On (0xa4/0xa5)
    LCD_WRITE_CMD(0xA6); // Disable Inverse Display On (0xa6/a7)
    LCD_WRITE_CMD(0xAF); //--turn on oled panel

    lcd_ssd1306_set_rotate(SCR_DIR_RLTB);
    xTaskCreate(&oled_refresh_task, "oled refresh", 1024 * 2, NULL, 2, NULL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_deinit(void)
{
    memset(&g_lcd_handle, 0, sizeof(scr_handle_t));
    return ESP_OK;
}

esp_err_t lcd_ssd1306_set_rotate(scr_dir_t dir)
{
    esp_err_t ret = ESP_OK;
    if (SCR_DIR_MAX < dir) {
        dir >>= 5;
    }
    LCD_CHECK(dir < 8, "Unsupport rotate direction", ESP_ERR_INVALID_ARG);
    switch (dir) {
    case SCR_DIR_LRTB:
        g_lcd_handle.width = g_lcd_handle.original_width;
        g_lcd_handle.height = g_lcd_handle.original_height;
        ret |= LCD_WRITE_CMD(0xA0);
        ret |= LCD_WRITE_CMD(0xC0);
        break;
    case SCR_DIR_LRBT:
        ret |= LCD_WRITE_CMD(0xA0);
        ret |= LCD_WRITE_CMD(0xC8);
        g_lcd_handle.width = g_lcd_handle.original_width;
        g_lcd_handle.height = g_lcd_handle.original_height;
        break;
    case SCR_DIR_RLTB:
        ret |= LCD_WRITE_CMD(0xA1);
        ret |= LCD_WRITE_CMD(0xC0);
        g_lcd_handle.width = g_lcd_handle.original_width;
        g_lcd_handle.height = g_lcd_handle.original_height;
        break;
    case SCR_DIR_RLBT:
        ret |= LCD_WRITE_CMD(0xA1);
        ret |= LCD_WRITE_CMD(0xC8);
        g_lcd_handle.width = g_lcd_handle.original_width;
        g_lcd_handle.height = g_lcd_handle.original_height;
        break;
    default:
        ESP_LOGE(TAG, "Unsupport rotate direction");
        g_lcd_handle.width = g_lcd_handle.original_width;
        g_lcd_handle.height = g_lcd_handle.original_height;
        return ESP_ERR_INVALID_ARG;
        break;
    }
    LCD_CHECK(ESP_OK == ret, "Set screen rotate failed", ESP_FAIL);
    g_lcd_handle.dir = dir;
    return ESP_OK;
}

esp_err_t lcd_ssd1306_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    esp_err_t ret = ESP_OK;
    y1 += 1;
    LCD_CHECK((0 == (y0 % 8)), "y0 should be a multiple of 8", ESP_ERR_INVALID_ARG);
    LCD_CHECK((0 == (y1 % 8)), "(y1+1) should be a multiple of 8", ESP_ERR_INVALID_ARG);
    uint8_t row1 = y0 >> 3;
    uint8_t row2 = y1 / 8;
    ret |= LCD_WRITE_CMD(SSD1306_CMD_SET_MEMORY_ADDR_MODE);
    ret |= LCD_WRITE_CMD(0); /**< Set to Horizontal Addressing Mode */
    ret |= LCD_WRITE_CMD(SSD1306_CMD_SET_COLUMN_RANGE);
    ret |= LCD_WRITE_CMD(x0);
    ret |= LCD_WRITE_CMD(x1 - 1);
    ret |= LCD_WRITE_CMD(SSD1306_CMD_SET_PAGE_RANGE);
    ret |= LCD_WRITE_CMD(row1);
    ret |= LCD_WRITE_CMD(row2 - 1);
    LCD_CHECK(ESP_OK == ret, "Set window failed", ESP_FAIL);
    return ESP_OK;
}

static esp_err_t lcd_ssd1306_write_ram_data(uint16_t color)
{
    ESP_LOGW(TAG, "lcd ssd1306 unsupport write ram data");
    return ESP_ERR_NOT_SUPPORTED;
}

static esp_err_t lcd_ssd1306_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    uint8_t pos,bx,temp=0;
	if(x>127||y>63)return ESP_FAIL;
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(color)g_gram[pos][x]|=temp;
	else g_gram[pos][x]&=~temp;
    return ESP_OK;
}

esp_err_t lcd_ssd1306_draw_4pixel(uint16_t x, uint16_t y, uint16_t color)
{
    x*=2;
    y*=2;
    // lcd_ssd1306_draw_pixel(x, y, color);
    lcd_ssd1306_draw_pixel(x+1, y, color);
    lcd_ssd1306_draw_pixel(x, y+1, color);
    // lcd_ssd1306_draw_pixel(x+1, y+1, color);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *bitmap)
{
    LCD_CHECK((x + w <= g_lcd_handle.width) && (y + h <= g_lcd_handle.height), "The set coordinates exceed the screen size", ESP_ERR_INVALID_ARG);

    uint16_t i, j;
    uint16_t x1 = x + w ;
    uint16_t y1 = y + h ;

    for (j = y; j < y1; j++) {
        for (i = x; i < x1; i++) {
            lcd_ssd1306_draw_4pixel(i, j, *bitmap++);
        }
    }
    return ESP_OK;
}

static esp_err_t lcd_ssd1306_refresh(void)
{
    esp_err_t ret = ESP_OK;
    uint8_t *p = (uint8_t *)g_gram;

    ret = lcd_ssd1306_set_window(0, 0, 127, 63);
    if (ESP_OK != ret) {
        return ESP_FAIL;
    }

    ret = LCD_WRITE(p, 128 * 8);
    LCD_CHECK(ESP_OK == ret, "Draw bitmap failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_display_on(void)
{
    esp_err_t ret;
    ret = LCD_WRITE_CMD(OLED_CMD_SET_CHARGE_PUMP); // SET DCDC
    ret |= LCD_WRITE_CMD(0X14); // Enable charge pump during display on
    ret |= LCD_WRITE_CMD(SSD1306_CMD_DISPLAY_ON); // DISPLAY ON
    LCD_CHECK(ESP_OK == ret, "Set display ON failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_display_off(void)
{
    esp_err_t ret;
    ret = LCD_WRITE_CMD(OLED_CMD_SET_CHARGE_PUMP); //SET DCDC
    ret |= LCD_WRITE_CMD(0X10); // Disable charge pump
    ret |= LCD_WRITE_CMD(SSD1306_CMD_DISPLAY_OFF); //DISPLAY OFF
    LCD_CHECK(ESP_OK == ret, "Set display OFF failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_set_contrast(uint8_t contrast)
{
    esp_err_t ret;
    ret = LCD_WRITE_CMD(SSD1306_CMD_SET_CONTRAST);
    ret |= LCD_WRITE_CMD(contrast);
    LCD_CHECK(ESP_OK == ret, "Set contrast failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_start_horizontal_scroll(uint8_t dir, uint8_t start, uint8_t stop, uint8_t interval)
{
    LCD_CHECK(start < SSD1306_PAGES, "Start page address invalid", ESP_ERR_INVALID_ARG);
    LCD_CHECK(stop < SSD1306_PAGES, "End page address invalid", ESP_ERR_INVALID_ARG);
    LCD_CHECK(interval < 8, "Time interval invalid", ESP_ERR_INVALID_ARG);

    uint8_t cmd = dir ? SSD1306_CMD_LEFT_HORIZONTAL_SCROLL : SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL;
    esp_err_t ret = ESP_OK;
    ret |= LCD_WRITE_CMD(cmd);
    ret |= LCD_WRITE_CMD(0x00);
    ret |= LCD_WRITE_CMD(start);
    ret |= LCD_WRITE_CMD(interval);
    ret |= LCD_WRITE_CMD(stop);

    ret |= LCD_WRITE_CMD(0x00);
    ret |= LCD_WRITE_CMD(0xff);
    ret |= LCD_WRITE_CMD(SSD1306_CMD_ACTIVATE_SCROLL);
    LCD_CHECK(ESP_OK == ret, "Start horizontal scroll failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_start_vertical_scroll(uint8_t start, uint8_t stop)
{
    LCD_CHECK(start < SSD1306_PAGES, "Start page address invalid", ESP_ERR_INVALID_ARG);
    LCD_CHECK(stop < SSD1306_PAGES, "End page address invalid", ESP_ERR_INVALID_ARG);
    esp_err_t ret = ESP_OK;

    ret |= LCD_WRITE_CMD(SSD1306_CMD_SET_VERTICAL_SCROLL_AREA);
    ret |= LCD_WRITE_CMD(start);
    ret |= LCD_WRITE_CMD(stop);
    ret |= LCD_WRITE_CMD(SSD1306_CMD_ACTIVATE_SCROLL);
    LCD_CHECK(ESP_OK == ret, "Start vertical scroll failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_start_scroll_diagRight(uint8_t start, uint8_t stop)
{
    esp_err_t ret = ESP_OK;
    ret |= LCD_WRITE_CMD(SSD1306_CMD_SET_VERTICAL_SCROLL_AREA);
    ret |= LCD_WRITE_CMD(0x00);
    ret |= LCD_WRITE_CMD(32);
    ret |= LCD_WRITE_CMD(SSD1306_CMD_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
    ret |= LCD_WRITE_CMD(0x00);
    ret |= LCD_WRITE_CMD(start);
    ret |= LCD_WRITE_CMD(0x00);
    ret |= LCD_WRITE_CMD(stop);
    ret |= LCD_WRITE_CMD(0x01);
    ret |= LCD_WRITE_CMD(SSD1306_CMD_ACTIVATE_SCROLL);
    LCD_CHECK(ESP_OK == ret, "Start diagRight scroll failed", ESP_FAIL);
    return ESP_OK;
}

esp_err_t lcd_ssd1306_stop_scroll(void)
{
    esp_err_t ret = ESP_OK;
    ret |= LCD_WRITE_CMD(SSD1306_CMD_DEACTIVATE_SCROLL);
    LCD_CHECK(ESP_OK == ret, "Stop scroll failed", ESP_FAIL);
    return ESP_OK;
}
