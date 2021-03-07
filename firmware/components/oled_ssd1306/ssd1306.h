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
#ifndef __IOT_LCD_SSD1306_H__
#define __IOT_LCD_SSD1306_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Define all screen direction
 *
 */
typedef enum {
    /* @---> X
       |
       Y
    */
    SCR_DIR_LRTB,   /**< From left to right then from top to bottom, this consider as the original direction of the screen */

    /*  Y
        |
        @---> X
    */
    SCR_DIR_LRBT,   /**< From left to right then from bottom to top */

    /* X <---@
             |
             Y
    */
    SCR_DIR_RLTB,   /**< From right to left then from top to bottom */

    /*       Y
             |
       X <---@
    */
    SCR_DIR_RLBT,   /**< From right to left then from bottom to top */

    /* @---> Y
       |
       X
    */
    SCR_DIR_TBLR,   /**< From top to bottom then from left to right */

    /*  X
        |
        @---> Y
    */
    SCR_DIR_BTLR,   /**< From bottom to top then from left to right */

    /* Y <---@
             |
             X
    */
    SCR_DIR_TBRL,   /**< From top to bottom then from right to left */

    /*       X
             |
       Y <---@
    */
    SCR_DIR_BTRL,   /**< From bottom to top then from right to left */

    SCR_DIR_MAX,

    /* Another way to represent rotation with 3 bit*/
    SCR_MIRROR_X = 0x40, /**< Mirror X-axis */
    SCR_MIRROR_Y = 0x20, /**< Mirror Y-axis */
    SCR_SWAP_XY  = 0x80, /**< Swap XY axis */
} scr_dir_t;

/**
 * @brief The types of colors that can be displayed on the screen
 * 
 */
typedef enum {
    SCR_COLOR_TYPE_MONO,     /**< The screen is monochrome */
    SCR_COLOR_TYPE_GRAY,     /**< The screen is gray */
    SCR_COLOR_TYPE_RGB565,   /**< The screen is colorful */
} scr_color_type_t;

/**
 * @brief   device initialization
 *
 * @param   lcd_conf configuration struct of ssd1306
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_init(void);

/**
 * @brief   Deinitial screen
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_deinit(void);


/**
 * @brief Set screen direction of rotation
 *
 * @param dir Pointer to a scr_dir_t structure.
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_set_rotate(scr_dir_t dir);

/**
 * @brief Set screen window
 *
 * @param x0 Starting point in X direction
 * @param y0 Starting point in Y direction
 * @param x1 End point in X direction
 * @param y1 End point in Y direction
 * 
 * @return
 *      - ESP_OK on success
 *      - ESP_FAIL Failed
 */
esp_err_t lcd_ssd1306_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/**
 * @brief Draw one pixel in screen with color
 * 
 * @param x X co-ordinate of set orientation
 * @param y Y co-ordinate of set orientation
 * @param color New color of the pixel
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Fill the pixels on LCD screen with bitmap
 * 
 * @param x Starting point in X direction
 * @param y Starting point in Y direction
 * @param w width of image in bitmap array
 * @param h height of image in bitmap array
 * @param bitmap pointer to bitmap array
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *bitmap);

/**
 * @brief Set the contrast of screen
 * 
 * @param contrast Contrast to set
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_set_contrast(uint8_t contrast);

/**
 * @brief Turn on the screen
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_display_on(void);

/**
 * @brief Turn off the screen
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_display_off(void);

/**
 * @brief Start horizontal scroll
 * 
 * @param dir Direction of horizontal scroll
 * @param start start page
 * @param stop end page
 * @param interval time interval of scroll
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_start_horizontal_scroll(uint8_t dir, uint8_t start, uint8_t stop, uint8_t interval);

/**
 * @brief 
 * 
 * @param start 
 * @param stop 
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_start_vertical_scroll(uint8_t start, uint8_t stop);

/**
 * @brief 
 * 
 * @param start 
 * @param stop 
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_start_scroll_diagRight(uint8_t start, uint8_t stop);

/**
 * @brief Stop screen scroll
 * 
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t lcd_ssd1306_stop_scroll(void);

#ifdef __cplusplus
}
#endif

#endif
