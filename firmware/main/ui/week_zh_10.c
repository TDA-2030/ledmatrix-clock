/*******************************************************************************
 * Size: 10 px
 * Bpp: 1
 * Opts: --bpp 1 --size 10 --no-compress --font STHeiti.ttf --symbols 一二三四五六七日周 --range 32-127 --format lvgl -o week_zh_10.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef WEEK_ZH_10
#define WEEK_ZH_10 1
#endif

#if WEEK_ZH_10

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfa,

    /* U+0022 "\"" */
    0xb4,

    /* U+0023 "#" */
    0x52, 0xbe, 0xaf, 0xaa, 0x40,

    /* U+0024 "$" */
    0x4e, 0xcc, 0x65, 0xde, 0x44,

    /* U+0025 "%" */
    0xfd, 0x33, 0xe0, 0xa2, 0xa5, 0x51, 0x20,

    /* U+0026 "&" */
    0x71, 0x46, 0x18, 0x96, 0x2f, 0x40,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x5a, 0xaa, 0x90,

    /* U+0029 ")" */
    0xa5, 0x55, 0x60,

    /* U+002A "*" */
    0x52, 0x40,

    /* U+002B "+" */
    0x21, 0x3e, 0x42, 0x0,

    /* U+002C "," */
    0xc0,

    /* U+002D "-" */
    0xe0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x11, 0x22, 0x24, 0x48, 0x80,

    /* U+0030 "0" */
    0x76, 0xe3, 0x18, 0xcd, 0xc0,

    /* U+0031 "1" */
    0xd5, 0x54,

    /* U+0032 "2" */
    0x74, 0x42, 0x22, 0x23, 0xe0,

    /* U+0033 "3" */
    0x70, 0x4c, 0x10, 0xc5, 0xc0,

    /* U+0034 "4" */
    0x10, 0x9d, 0x2f, 0x88, 0x40,

    /* U+0035 "5" */
    0x7a, 0x1c, 0x10, 0xc5, 0xc0,

    /* U+0036 "6" */
    0x11, 0x10, 0xe8, 0xc6, 0x2e,

    /* U+0037 "7" */
    0xf1, 0x22, 0x44, 0x88,

    /* U+0038 "8" */
    0x74, 0x5f, 0x28, 0xc5, 0xc0,

    /* U+0039 "9" */
    0x74, 0x63, 0x17, 0x8, 0x88,

    /* U+003A ":" */
    0x88,

    /* U+003B ";" */
    0x8c,

    /* U+003C "<" */
    0x9, 0xb0, 0x83, 0x80,

    /* U+003D "=" */
    0xf8, 0x3e,

    /* U+003E ">" */
    0x83, 0x6, 0x2e, 0x0,

    /* U+003F "?" */
    0x69, 0x12, 0x20, 0x20,

    /* U+0040 "@" */
    0x3c, 0x42, 0xb9, 0xc9, 0xcb, 0x7e, 0x3c,

    /* U+0041 "A" */
    0x10, 0x30, 0xa1, 0x47, 0xc8, 0xa0, 0x80,

    /* U+0042 "B" */
    0xe9, 0x9e, 0x99, 0xe0,

    /* U+0043 "C" */
    0x3c, 0x86, 0x4, 0x8, 0x8, 0x4f, 0x0,

    /* U+0044 "D" */
    0xf2, 0x28, 0x61, 0x86, 0x3f, 0x80,

    /* U+0045 "E" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0046 "F" */
    0xf8, 0x8f, 0x88, 0x80,

    /* U+0047 "G" */
    0x3c, 0x42, 0x80, 0x86, 0x82, 0x42, 0x3c,

    /* U+0048 "H" */
    0x8c, 0x63, 0xf8, 0xc6, 0x20,

    /* U+0049 "I" */
    0xfe,

    /* U+004A "J" */
    0x11, 0x11, 0x11, 0x70,

    /* U+004B "K" */
    0x95, 0x31, 0x8a, 0x4a, 0x20,

    /* U+004C "L" */
    0x88, 0x88, 0x88, 0xf0,

    /* U+004D "M" */
    0x82, 0x82, 0xc6, 0xca, 0xaa, 0xb2, 0x91,

    /* U+004E "N" */
    0x87, 0x1c, 0x69, 0x96, 0x38, 0x40,

    /* U+004F "O" */
    0x3c, 0x42, 0x81, 0x81, 0x81, 0x42, 0x3c,

    /* U+0050 "P" */
    0xf4, 0x63, 0xe8, 0x42, 0x0,

    /* U+0051 "Q" */
    0x3c, 0x42, 0x81, 0x81, 0x89, 0x46, 0x3e, 0x1,

    /* U+0052 "R" */
    0xf4, 0x63, 0xea, 0x4a, 0x20,

    /* U+0053 "S" */
    0xe9, 0x86, 0x19, 0xe0,

    /* U+0054 "T" */
    0xf9, 0x8, 0x42, 0x10, 0x80,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc5, 0xc0,

    /* U+0056 "V" */
    0x84, 0x89, 0x11, 0x42, 0x82, 0x4, 0x0,

    /* U+0057 "W" */
    0x88, 0xa4, 0x55, 0x4a, 0xa5, 0x31, 0x18, 0x80,

    /* U+0058 "X" */
    0x49, 0x23, 0x4, 0x31, 0x24, 0x80,

    /* U+0059 "Y" */
    0x8a, 0x94, 0x42, 0x10, 0x80,

    /* U+005A "Z" */
    0x71, 0x22, 0x44, 0x70,

    /* U+005B "[" */
    0xea, 0xaa, 0xc0,

    /* U+005C "\\" */
    0x81, 0x4, 0x8, 0x10, 0x40, 0x82, 0x4,

    /* U+005D "]" */
    0xd5, 0x55, 0xc0,

    /* U+005E "^" */
    0x21, 0x15, 0x29, 0x44,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0xa0,

    /* U+0061 "a" */
    0x77, 0x38, 0x73, 0x74,

    /* U+0062 "b" */
    0x82, 0xb, 0xb3, 0x87, 0x3b, 0x80,

    /* U+0063 "c" */
    0x7b, 0x8, 0x30, 0x78,

    /* U+0064 "d" */
    0x4, 0x17, 0x73, 0x87, 0x37, 0x40,

    /* U+0065 "e" */
    0x7b, 0xe8, 0x32, 0x70,

    /* U+0066 "f" */
    0x6b, 0xa4, 0x90,

    /* U+0067 "g" */
    0x77, 0x38, 0x73, 0x77, 0x37, 0x80,

    /* U+0068 "h" */
    0x84, 0x2d, 0x98, 0xc6, 0x20,

    /* U+0069 "i" */
    0xc5, 0x54,

    /* U+006A "j" */
    0xc5, 0x55, 0xc0,

    /* U+006B "k" */
    0x84, 0x25, 0x4c, 0x52, 0x40,

    /* U+006C "l" */
    0xfe,

    /* U+006D "m" */
    0xaf, 0xa6, 0x4c, 0x99, 0x20,

    /* U+006E "n" */
    0xb6, 0x63, 0x18, 0x80,

    /* U+006F "o" */
    0x7b, 0x38, 0x73, 0x78,

    /* U+0070 "p" */
    0xbb, 0x38, 0x63, 0xba, 0x8, 0x0,

    /* U+0071 "q" */
    0x77, 0x38, 0x73, 0x74, 0x10, 0x40,

    /* U+0072 "r" */
    0xf2, 0x48,

    /* U+0073 "s" */
    0xd1, 0x1e,

    /* U+0074 "t" */
    0x4b, 0xa4, 0x98,

    /* U+0075 "u" */
    0x8c, 0x63, 0x36, 0x80,

    /* U+0076 "v" */
    0x8a, 0x54, 0x62, 0x0,

    /* U+0077 "w" */
    0x91, 0x99, 0x5a, 0x26, 0x24,

    /* U+0078 "x" */
    0x93, 0x8, 0xa9, 0x0,

    /* U+0079 "y" */
    0x8a, 0x54, 0x62, 0x11, 0x0,

    /* U+007A "z" */
    0xf2, 0x44, 0xf0,

    /* U+007B "{" */
    0x69, 0x28, 0x92, 0x60,

    /* U+007C "|" */
    0xff, 0x80,

    /* U+007D "}" */
    0xc9, 0x22, 0x92, 0xc0,

    /* U+007E "~" */
    0xc9, 0x80,

    /* U+007F "" */
    0x0,

    /* U+4E00 "一" */
    0xff, 0x80,

    /* U+4E03 "七" */
    0x10, 0x8, 0x4, 0x63, 0xcf, 0x0, 0x80, 0x40,
    0x23, 0x1f, 0x0,

    /* U+4E09 "三" */
    0x7e, 0x0, 0x0, 0x7e, 0x0, 0x0, 0xff,

    /* U+4E8C "二" */
    0x7e, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff,

    /* U+4E94 "五" */
    0xff, 0x10, 0x7e, 0x12, 0x12, 0x22, 0xff,

    /* U+516D "六" */
    0x8, 0x0, 0x3f, 0xe0, 0x3, 0x21, 0x11, 0x84,
    0x83, 0x0, 0x0,

    /* U+5468 "周" */
    0x7f, 0x49, 0x5f, 0x49, 0x55, 0x5d, 0x55, 0x5d,
    0x81, 0x0,

    /* U+56DB "四" */
    0xff, 0xa9, 0xa9, 0xa9, 0xcf, 0x81, 0xff, 0x81,

    /* U+65E5 "日" */
    0xfe, 0x18, 0x7f, 0x86, 0x1f, 0xe1
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 40, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 47, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 2, .adv_w = 49, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 3, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 8, .adv_w = 89, .box_w = 4, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 13, .adv_w = 124, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 121, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 26, .adv_w = 32, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 27, .adv_w = 59, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 30, .adv_w = 59, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 33, .adv_w = 68, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 35, .adv_w = 97, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 39, .adv_w = 44, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 53, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 41, .adv_w = 44, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 42, .adv_w = 70, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 47, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 52, .adv_w = 89, .box_w = 2, .box_h = 7, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 54, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 59, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 64, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 69, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 74, .adv_w = 89, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 79, .adv_w = 89, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 83, .adv_w = 89, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 88, .adv_w = 89, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 93, .adv_w = 44, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 94, .adv_w = 44, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 97, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 99, .adv_w = 97, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 101, .adv_w = 97, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 105, .adv_w = 79, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 109, .adv_w = 139, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 116, .adv_w = 118, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 123, .adv_w = 92, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 127, .adv_w = 130, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 134, .adv_w = 119, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 140, .adv_w = 86, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 144, .adv_w = 78, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 148, .adv_w = 140, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 155, .adv_w = 109, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 160, .adv_w = 36, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 161, .adv_w = 77, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 165, .adv_w = 95, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 170, .adv_w = 74, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 174, .adv_w = 147, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 181, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 187, .adv_w = 139, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 194, .adv_w = 95, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 199, .adv_w = 139, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 97, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 212, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 216, .adv_w = 68, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 221, .adv_w = 105, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 226, .adv_w = 112, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 233, .adv_w = 154, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 241, .adv_w = 97, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 247, .adv_w = 95, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 252, .adv_w = 77, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 256, .adv_w = 56, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 259, .adv_w = 97, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 266, .adv_w = 56, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 269, .adv_w = 108, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 273, .adv_w = 80, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 274, .adv_w = 60, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 275, .adv_w = 109, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 279, .adv_w = 109, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 285, .adv_w = 104, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 289, .adv_w = 110, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 295, .adv_w = 104, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 299, .adv_w = 50, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 302, .adv_w = 108, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 308, .adv_w = 98, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 313, .adv_w = 32, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 315, .adv_w = 32, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 318, .adv_w = 80, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 323, .adv_w = 32, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 324, .adv_w = 150, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 329, .adv_w = 98, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 333, .adv_w = 105, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 337, .adv_w = 109, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 343, .adv_w = 109, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 349, .adv_w = 48, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 351, .adv_w = 62, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 353, .adv_w = 54, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 356, .adv_w = 97, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 360, .adv_w = 89, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 364, .adv_w = 133, .box_w = 8, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 369, .adv_w = 77, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 373, .adv_w = 86, .box_w = 5, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 378, .adv_w = 68, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 381, .adv_w = 56, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 385, .adv_w = 108, .box_w = 1, .box_h = 9, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 387, .adv_w = 56, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 391, .adv_w = 97, .box_w = 5, .box_h = 2, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 393, .adv_w = 80, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 160, .box_w = 9, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 396, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 407, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 160, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 428, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 439, .adv_w = 160, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 449, .adv_w = 160, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 457, .adv_w = 160, .box_w = 6, .box_h = 8, .ofs_x = 2, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x3, 0x9, 0x8c, 0x94, 0x36d, 0x668, 0x8db,
    0x17e5
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 96, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 19968, .range_length = 6118, .glyph_id_start = 97,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 9, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t week_zh_10 = {
#else
lv_font_t week_zh_10 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if WEEK_ZH_10*/

