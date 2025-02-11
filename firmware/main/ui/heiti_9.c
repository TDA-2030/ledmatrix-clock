/*******************************************************************************
 * Size: 9 px
 * Bpp: 1
 * Opts: --bpp 1 --size 9 --no-compress --font SimHei.ttf --symbols 一二三四五六七日℃ --range 32-127 --format lvgl -o heiti_9.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef HEITI_9
#define HEITI_9 1
#endif

#if HEITI_9

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf4,

    /* U+0022 "\"" */
    0xf0,

    /* U+0023 "#" */
    0x55, 0xf6, 0xfa,

    /* U+0024 "$" */
    0x5f, 0xe7, 0xfa,

    /* U+0025 "%" */
    0xde, 0xe3, 0x37,

    /* U+0026 "&" */
    0x6a, 0x4d, 0xbf,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0xa, 0x49, 0x10,

    /* U+0029 ")" */
    0x8, 0x92, 0x50,

    /* U+002A "*" */
    0x23, 0x9c, 0x40,

    /* U+002B "+" */
    0x21, 0x3e, 0x40,

    /* U+002C "," */
    0xc0,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x12, 0x24, 0x48,

    /* U+0030 "0" */
    0x69, 0x99, 0x96,

    /* U+0031 "1" */
    0x75, 0x50,

    /* U+0032 "2" */
    0xea, 0x22, 0x4f,

    /* U+0033 "3" */
    0x69, 0x21, 0x96,

    /* U+0034 "4" */
    0x11, 0x94, 0xaf, 0x88,

    /* U+0035 "5" */
    0x78, 0xe1, 0x9e,

    /* U+0036 "6" */
    0x24, 0xe9, 0x96,

    /* U+0037 "7" */
    0xf2, 0x22, 0x44,

    /* U+0038 "8" */
    0xe9, 0xe9, 0x9f,

    /* U+0039 "9" */
    0x69, 0x96, 0x24,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x98,

    /* U+003C "<" */
    0x12, 0x44, 0x21,

    /* U+003D "=" */
    0xf0, 0xf0,

    /* U+003E ">" */
    0x84, 0x31, 0x68,

    /* U+003F "?" */
    0x71, 0x12, 0x2,

    /* U+0040 "@" */
    0x6b, 0xdf, 0xc4,

    /* U+0041 "A" */
    0x26, 0x67, 0x99,

    /* U+0042 "B" */
    0xf9, 0xe9, 0x9e,

    /* U+0043 "C" */
    0x69, 0x88, 0x96,

    /* U+0044 "D" */
    0xe9, 0x99, 0x9e,

    /* U+0045 "E" */
    0xf8, 0xe8, 0x8f,

    /* U+0046 "F" */
    0xf8, 0x8e, 0x88,

    /* U+0047 "G" */
    0x69, 0x8b, 0x97,

    /* U+0048 "H" */
    0x99, 0xf9, 0x99,

    /* U+0049 "I" */
    0xfc,

    /* U+004A "J" */
    0x11, 0x11, 0x96,

    /* U+004B "K" */
    0x9a, 0xca, 0xa9,

    /* U+004C "L" */
    0x88, 0x88, 0x8f,

    /* U+004D "M" */
    0x9f, 0xff, 0xdd,

    /* U+004E "N" */
    0x9d, 0xdb, 0xb9,

    /* U+004F "O" */
    0x69, 0x99, 0x96,

    /* U+0050 "P" */
    0xe9, 0x9e, 0x88,

    /* U+0051 "Q" */
    0x69, 0x99, 0xb7,

    /* U+0052 "R" */
    0xe9, 0x9e, 0xa9,

    /* U+0053 "S" */
    0x69, 0xc3, 0x9e,

    /* U+0054 "T" */
    0xe9, 0x24, 0x80,

    /* U+0055 "U" */
    0x99, 0x99, 0x9f,

    /* U+0056 "V" */
    0x99, 0x56, 0x62,

    /* U+0057 "W" */
    0xbb, 0xfd, 0x55,

    /* U+0058 "X" */
    0x96, 0x26, 0x69,

    /* U+0059 "Y" */
    0x92, 0x98, 0x42, 0x10,

    /* U+005A "Z" */
    0xf2, 0x44, 0x8f,

    /* U+005B "[" */
    0xf2, 0x49, 0x27,

    /* U+005C "\\" */
    0x84, 0x42, 0x22, 0x10,

    /* U+005D "]" */
    0xe4, 0x92, 0x4f,

    /* U+005E "^" */
    0x26,

    /* U+005F "_" */
    0xf0,

    /* U+0060 "`" */
    0x90,

    /* U+0061 "a" */
    0x63, 0xdf,

    /* U+0062 "b" */
    0x88, 0xe9, 0x9e,

    /* U+0063 "c" */
    0x68, 0x96,

    /* U+0064 "d" */
    0x11, 0x79, 0x97,

    /* U+0065 "e" */
    0x6f, 0x96,

    /* U+0066 "f" */
    0x74, 0xe4, 0x44,

    /* U+0067 "g" */
    0x7a, 0x98, 0xe7, 0x0,

    /* U+0068 "h" */
    0x88, 0xf9, 0x99,

    /* U+0069 "i" */
    0xbc,

    /* U+006A "j" */
    0x20, 0x92, 0x78,

    /* U+006B "k" */
    0x88, 0xac, 0xa9,

    /* U+006C "l" */
    0xfc,

    /* U+006D "m" */
    0xfd, 0x6b, 0x50,

    /* U+006E "n" */
    0xf9, 0x99,

    /* U+006F "o" */
    0x69, 0x96,

    /* U+0070 "p" */
    0xe9, 0x9e, 0x80,

    /* U+0071 "q" */
    0x79, 0x97, 0x10,

    /* U+0072 "r" */
    0xf2, 0x40,

    /* U+0073 "s" */
    0xf1, 0xf0,

    /* U+0074 "t" */
    0x4e, 0x44, 0x70,

    /* U+0075 "u" */
    0x99, 0x9f,

    /* U+0076 "v" */
    0x95, 0x62,

    /* U+0077 "w" */
    0xbf, 0x55,

    /* U+0078 "x" */
    0x56, 0x65,

    /* U+0079 "y" */
    0x96, 0x62, 0x40,

    /* U+007A "z" */
    0xe9, 0x70,

    /* U+007B "{" */
    0xea, 0xab,

    /* U+007C "|" */
    0xff, 0x80,

    /* U+007D "}" */
    0xd5, 0x57,

    /* U+007E "~" */
    0x43,

    /* U+2103 "℃" */
    0xc0, 0xce, 0x11, 0x20, 0x20, 0x11, 0xe,

    /* U+4E00 "一" */
    0xff,

    /* U+4E03 "七" */
    0x10, 0x10, 0x13, 0x7c, 0x90, 0x10, 0x11, 0x1f,

    /* U+4E09 "三" */
    0x7e, 0x0, 0x0, 0x7e, 0x0, 0x0, 0xff,

    /* U+4E8C "二" */
    0x7e, 0x0, 0x0, 0x0, 0x0, 0xff,

    /* U+4E94 "五" */
    0xfe, 0x40, 0x83, 0xe2, 0x44, 0xbf, 0x80,

    /* U+516D "六" */
    0x10, 0x8, 0xff, 0x0, 0x24, 0x62, 0x42, 0x80,

    /* U+56DB "四" */
    0xff, 0x66, 0xcd, 0x9c, 0xf0, 0x7f, 0xc1,

    /* U+65E5 "日" */
    0xfe, 0x18, 0x7f, 0x86, 0x1f, 0xe1
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 72, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 72, .box_w = 1, .box_h = 6, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 72, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 3, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 72, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 9, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 15, .adv_w = 72, .box_w = 1, .box_h = 2, .ofs_x = 2, .ofs_y = 4},
    {.bitmap_index = 16, .adv_w = 72, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 19, .adv_w = 72, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 22, .adv_w = 72, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 25, .adv_w = 72, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 28, .adv_w = 72, .box_w = 1, .box_h = 2, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 29, .adv_w = 72, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 30, .adv_w = 72, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 34, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 72, .box_w = 2, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 39, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 42, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 45, .adv_w = 72, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 52, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 55, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 61, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 72, .box_w = 1, .box_h = 4, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 65, .adv_w = 72, .box_w = 1, .box_h = 5, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 66, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 69, .adv_w = 72, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 71, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 74, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 83, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 72, .box_w = 1, .box_h = 6, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 108, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 117, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 123, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 135, .adv_w = 72, .box_w = 3, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 144, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 150, .adv_w = 72, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 154, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 157, .adv_w = 72, .box_w = 3, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 160, .adv_w = 72, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 164, .adv_w = 72, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 167, .adv_w = 72, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 168, .adv_w = 72, .box_w = 4, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 169, .adv_w = 72, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 170, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 172, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 185, .adv_w = 72, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 189, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 192, .adv_w = 72, .box_w = 1, .box_h = 6, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 72, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 196, .adv_w = 72, .box_w = 4, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 199, .adv_w = 72, .box_w = 1, .box_h = 6, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 72, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 203, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 210, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 213, .adv_w = 72, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 72, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 72, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 228, .adv_w = 72, .box_w = 4, .box_h = 5, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 231, .adv_w = 72, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 72, .box_w = 2, .box_h = 8, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 235, .adv_w = 72, .box_w = 1, .box_h = 9, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 237, .adv_w = 72, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 239, .adv_w = 72, .box_w = 4, .box_h = 2, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 240, .adv_w = 144, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 144, .box_w = 8, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 248, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 256, .adv_w = 144, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 144, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 144, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 276, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 284, .adv_w = 144, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 291, .adv_w = 144, .box_w = 6, .box_h = 8, .ofs_x = 2, .ofs_y = -1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x2cfd, 0x2d00, 0x2d06, 0x2d89, 0x2d91, 0x306a, 0x35d8,
    0x44e2
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 8451, .range_length = 17635, .glyph_id_start = 96,
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
const lv_font_t heiti_9 = {
#else
lv_font_t heiti_9 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 9,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
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



#endif /*#if HEITI_9*/

