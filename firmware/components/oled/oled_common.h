#ifndef __OLED_COMMON_H__
#define __OLED_COMMON_H__

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

#endif
