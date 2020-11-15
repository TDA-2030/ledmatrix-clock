// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <sys/param.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "lcd_paint.h"
#include "show_text.h"
#include "cc936.h"

static const char *TAG = "lcd paint";

#define LCD_PAINT_CHECK(a, str)  if(!(a)) {                               \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);   \
        return ;                                                           \
    }


#define LED_MATRIX_MAX_WIDTH   64
#define LED_MATRIX_MAX_HEIGHT  32

//屏幕的画笔颜色和背景色
static uint16_t g_point_color = 0x01; //画笔颜色
static uint16_t g_back_color  = 0x00;  //背景色

lcd_driver_fun_t g_lcd;

esp_err_t iot_paint_init(lcd_driver_fun_t *driver)
{
    g_lcd = *driver;
    return ESP_OK;
}

void iot_paint_clear(int color)
{
    uint8_t i, j;

    for (i = 0; i < LED_MATRIX_MAX_HEIGHT; i++) { //把R,G缓冲区全部数据清0
        for (j = 0; j < LED_MATRIX_MAX_WIDTH; j++) {
            g_lcd.draw_pixel(j, i, COLOR_BLACK);
        }
    }
}

void iot_paint_Set_point_color(uint16_t color)
{
    g_point_color = color;
}
uint16_t iot_paint_Get_point_color(void)
{
    return g_point_color;
}

void iot_paint_Set_back_color(uint16_t color)
{
    g_back_color = color;
}
uint16_t iot_paint_Get_back_color(void)
{
    return g_back_color;
}

void iot_paint_draw_gbk_char_offset(int x, int y, uint8_t offset, uint8_t width, const char *text_char, const sFONT *font)
{
    LCD_PAINT_CHECK(NULL != font, "Font pointer invalid");
    int i, j;
    int x0 = x;
    uint16_t char_size = 0;
    uint8_t ptr[72];
    if (*text_char < 0x80 && font == &Font16_gbk) {
        font = &Font16;
        text_char++;
        ESP_LOGW(TAG, "force to ascii");
    }

    int ret = font->get_fontdata(text_char, &ptr, &char_size);
    if (0 != ret) {
        ESP_LOGE(TAG, "get font data failed");
    }

    for (j = 0; j < char_size; j++) {
        uint8_t temp = ptr[j];
        for (i = 0; i < 8; i++) {
            if ((x - x0) >= offset && ((x - x0 - offset)<width)) {
                if (temp & 0x80) {
                    g_lcd.draw_pixel(x-offset, y, g_point_color);
                } else {
                    g_lcd.draw_pixel(x-offset, y, g_back_color);
                }
            }

            temp <<= 1;
            x++;
            if ((x - x0) == font->Width) {
                x = x0;
                y++;
                break;
            }
        }
    }
}

void iot_paint_draw_gbk_char(int x, int y, const char *text_char, const sFONT *font)
{
    LCD_PAINT_CHECK(NULL != font, "Font pointer invalid");
    int i, j;
    int x0 = x;
    uint16_t char_size = 0;
    uint8_t ptr[72];
    if (*text_char < 0x80 && font == &Font16_gbk) {
        font = &Font16;
        text_char++;
    }

    int ret = font->get_fontdata(text_char, &ptr, &char_size);
    if (0 != ret) {
        ESP_LOGE(TAG, "get font data failed");
    }

    for (j = 0; j < char_size; j++) {
        uint8_t temp = ptr[j];
        for (i = 0; i < 8; i++) {
            if (temp & 0x80) {
                g_lcd.draw_pixel(x, y, g_point_color);
            } else {
                g_lcd.draw_pixel(x, y, g_back_color);
            }
            temp <<= 1;
            x++;
            if ((x - x0) == font->Width) {
                x = x0;
                y++;
                break;
            }
        }
    }
}

void iot_paint_draw_string(int x, int y, const char *text, const sFONT *font)
{
    LCD_PAINT_CHECK(NULL != text, "string pointer invalid");
    LCD_PAINT_CHECK(NULL != font, "Font pointer invalid");
    const char *p_text = text;
    uint16_t x0 = x;
    uint16_t y0 = y;

    char gbk[2];
    char unicode[2];

    while (*p_text != 0) {
        if (x > (x0 + 64 - font->Width)) { //换行
            y += font->Height;
            x = x0;
        }
        if (y > (y0 + 32 - font->Height)) {
            break;
            ESP_LOGW(TAG, "exceed area");
        }

        p_text = Utf8ToUnicode(p_text, unicode);
        font_unicode2gbk(unicode, gbk);
        char ascii = gbk[1];

        if (ascii == '\n') {
            y += font->Height;
            x = x0;
        } else {
            iot_paint_draw_gbk_char(x, y, gbk, font);
            x += font->Width;
        }

    }
}

static int8_t my_i2a(int32_t in_num, char *out_str)
{
    uint8_t remainder;
    uint8_t len = 0;

    if (NULL == out_str) {
        return 0;
    }

    do {
        remainder = in_num % 10;
        in_num = in_num / 10;
        *(out_str++) = remainder + '0';
        len++;
    } while (in_num);

    return len;
}

void iot_paint_draw_num(int x, int y, uint32_t num, uint8_t len, const sFONT *font)
{
    LCD_PAINT_CHECK(len < 10, "The length of the number is too long");
    LCD_PAINT_CHECK(NULL != font, "Font pointer invalid");
    char buf[10];
    int8_t num_len;

    num_len = my_i2a(num, buf);
    x += (font->Width * (len - 1));

    for (size_t i = 0; i < len; i++) {
        if (i < num_len) {
            iot_paint_draw_gbk_char(x, y, &buf[i], font);
        } else {
            // iot_paint_draw_gbk_char(x, y, '0', font);
        }

        x -= font->Width;
    }
}

void iot_paint_draw_float(int x, int y, uint32_t num, uint8_t len, uint8_t point, const sFONT *font)
{
    LCD_PAINT_CHECK(len < 10, "The length of the number is too long");
    LCD_PAINT_CHECK(NULL != font, "Font pointer invalid");
    char buf[10];
    int8_t num_len;

    num_len = my_i2a(num, buf);
    x += (font->Width * len);

    for (size_t i = 0; i < len; i++) {
        if (i == point) {
            // iot_paint_draw_gbk_char(x, y, '.', font);
            x -= font->Width;
        }

        if (i < num_len) {
            iot_paint_draw_gbk_char(x, y, &buf[i], font);
        } else {
            // iot_paint_draw_gbk_char(x, y, '0', font);
        }

        x -= font->Width;
    }
}

/**
*  @brief: this displays a image on the frame buffer but not refresh
*/
void iot_paint_draw_image(int x, int y, int width, int height, uint16_t *img)
{
    LCD_PAINT_CHECK(NULL != img, "Image pointer invalid");
    g_lcd.draw_bitmap(x, y, width, height, img);
}

/**
*  @brief: this draws a horizontal line on the frame buffer
*/
void iot_paint_draw_horizontal_line(int x, int y, int line_length)
{
    int i;

    for (i = x; i < x + line_length; i++) {
        g_lcd.draw_pixel(i, y, g_point_color);
    }
}

/**
*  @brief: this draws a vertical line on the frame buffer
*/
void iot_paint_draw_vertical_line(int x, int y, int line_length)
{
    int i;

    for (i = y; i < y + line_length; i++) {
        g_lcd.draw_pixel(x, i, g_point_color);
    }
}


void iot_paint_draw_line(int x1, int y1, int x2, int y2)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if (delta_x > 0) {
        incx = 1;    //设置单步方向
    } else if (delta_x == 0) {
        incx = 0;    //垂直线
    } else {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0) {
        incy = 1;
    } else if (delta_y == 0) {
        incy = 0;    //水平线
    } else {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y) {
        distance = delta_x;    //选取基本增量坐标轴
    } else {
        distance = delta_y;
    }

    for (t = 0; t <= distance + 1; t++) { //画线输出
        g_lcd.draw_pixel(uRow, uCol, g_point_color); //画点
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance) {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

/**
*  @brief: this draws a rectangle
*/
void iot_paint_draw_rectangle(int x0, int y0, int x1, int y1)
{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    iot_paint_draw_horizontal_line(min_x, min_y, max_x - min_x + 1);
    iot_paint_draw_horizontal_line(min_x, max_y, max_x - min_x + 1);
    iot_paint_draw_vertical_line(min_x, min_y, max_y - min_y + 1);
    iot_paint_draw_vertical_line(max_x, min_y, max_y - min_y + 1);
}

/**
*  @brief: this draws a filled rectangle
*/
void iot_paint_draw_filled_rectangle(int x0, int y0, int x1, int y1)
{
    int min_x, min_y, max_x, max_y;
    int i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (i = min_x; i <= max_x; i++) {
        iot_paint_draw_vertical_line(i, min_y, max_y - min_y + 1);
    }
}

/**
*  @brief: this draws a circle
*/
void iot_paint_draw_circle(int x, int y, int radius)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        g_lcd.draw_pixel(x - x_pos, y + y_pos, g_point_color);
        g_lcd.draw_pixel(x + x_pos, y + y_pos, g_point_color);
        g_lcd.draw_pixel(x + x_pos, y - y_pos, g_point_color);
        g_lcd.draw_pixel(x - x_pos, y - y_pos, g_point_color);
        e2 = err;

        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;

            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }

        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

/**
*  @brief: this draws a filled circle
*/
void iot_paint_draw_filled_circle(int x, int y, int radius)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        g_lcd.draw_pixel(x - x_pos, y + y_pos, g_point_color);
        g_lcd.draw_pixel(x + x_pos, y + y_pos, g_point_color);
        g_lcd.draw_pixel(x + x_pos, y - y_pos, g_point_color);
        g_lcd.draw_pixel(x - x_pos, y - y_pos, g_point_color);
        iot_paint_draw_horizontal_line(x + x_pos, y + y_pos, 2 * (-x_pos) + 1);
        iot_paint_draw_horizontal_line(x + x_pos, y - y_pos, 2 * (-x_pos) + 1);
        e2 = err;

        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;

            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }

        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

