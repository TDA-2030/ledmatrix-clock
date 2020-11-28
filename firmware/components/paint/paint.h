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
#ifndef _LCD_PAINT_H
#define _LCD_PAINT_H

#include "iot_fonts.h"
#include "show_text.h"


#ifdef __cplusplus
extern "C" {
#endif



#define COLOR_BLACK  0
#define COLOR_RED    1
#define COLOR_GREEN  2
#define COLOR_YELLOW 3

typedef struct {
    esp_err_t (*draw_pixel)(uint16_t x, uint16_t y, uint16_t color);                                     /*!<  */
    esp_err_t (*draw_bitmap)(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
}lcd_driver_fun_t;

/**
 * @brief LCD paint initial
 * 
 * @param driver Output a lcd_driver_fun_t structure.
 * 
 * @return 
 *      - ESP_OK on success
 *      - ESP_FAIL Failed
 */
esp_err_t iot_paint_init(lcd_driver_fun_t *driver);

/**
 * @brief Clear screen to one color
 * 
 * @param color Set the screen to this color
 */
void iot_paint_clear(int color);


void iot_paint_Set_point_color(uint16_t color);

uint16_t iot_paint_Get_point_color(void);

void iot_paint_Set_back_color(uint16_t color);

uint16_t iot_paint_Get_back_color(void);

void iot_paint_draw_gbk_char_offset(int x, int y, uint8_t offset, uint8_t width, const char *text_char, const sFONT *font);
void iot_paint_draw_gbk_char(int x, int y, const char *text_char, const sFONT *font);

/**
 * @brief Draw a character on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param ascii_char ASCII characters to display
 * @param font Pointer to a font
 */
void iot_paint_draw_char(int x, int y, char ascii_char, const sFONT* font);

/**
 * @brief Draw a string on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param text ASCII string to display
 * @param font Pointer to a font
 */
void iot_paint_draw_string(int x, int y, const char* text, const sFONT* font);

/**
 * @brief Draw a number on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param num Number to display
 * @param len The length of the number
 * @param font Pointer to a font
 */
void iot_paint_draw_num(int x, int y, uint32_t num, uint8_t len, const sFONT *font);

/**
 * @brief Draw a float number on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param num Number to display
 * @param len The length of the number
 * @param point Position of decimal point
 * @param font Pointer to a font
 */
void iot_paint_draw_float(int x, int y, uint32_t num, uint8_t len, uint8_t point, const sFONT *font);

/**
 * @brief Draw a image on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param width width of image
 * @param height height of image
 * @param img pointer to bitmap array
 */
void iot_paint_draw_image(int x, int y, int width, int height, uint16_t *img);

/**
 * @brief Draw a horizontal line on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param line_length length of line
 */
void iot_paint_draw_horizontal_line(int x, int y, int line_length);

/**
 * @brief Draw a vertical line on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param line_length length of line
 */
void iot_paint_draw_vertical_line(int x, int y, int line_length);

/**
 * @brief draw a line on sccreen
 * 
 * @param x0 Starting point in X direction
 * @param y0 Starting point in Y direction
 * @param x1 End point in X direction
 * @param y1 End point in Y direction
 */
void iot_paint_draw_line(int x0, int y0, int x1, int y1);

/**
 * @brief draw a rectangle on sccreen
 * 
 * @param x0 Starting point in X direction
 * @param y0 Starting point in Y direction
 * @param x1 End point in X direction
 * @param y1 End point in Y direction
 */
void iot_paint_draw_rectangle(int x0, int y0, int x1, int y1);

/**
 * @brief draw a filled rectangle on sccreen
 * 
 * @param x0 Starting point in X direction
 * @param y0 Starting point in Y direction
 * @param x1 End point in X direction
 * @param y1 End point in Y direction
 */
void iot_paint_draw_filled_rectangle(int x0, int y0, int x1, int y1);

/**
 * @brief Draw a hollow circle on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param radius radius of circle
 */
void iot_paint_draw_circle(int x, int y, int radius);

/**
 * @brief Draw a filled circle on screen
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param radius radius of circle
 */
void iot_paint_draw_filled_circle(int x, int y, int radius);


#ifdef __cplusplus
}
#endif

#endif