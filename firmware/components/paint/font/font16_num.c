/**
  ******************************************************************************
  * @file    font16.cpp
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   This file provides text font16.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iot_fonts.h"

//
//  Font data 
//

static const uint8_t Font16_num_Table[] = {
0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"0",0*/

0x00,0x18,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,/*"1",1*/

0x00,0x38,0x6C,0xC6,0xC6,0x06,0x0C,0x0C,0x18,0x30,0x30,0x60,0xC0,0xFE,0x00,0x00,/*"2",2*/

0x00,0x38,0x6C,0xC6,0xC6,0x06,0x0C,0x38,0x0C,0x06,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"3",3*/

0x00,0x0C,0x0C,0x1C,0x1C,0x3C,0x3C,0x6C,0x6C,0xCC,0xFE,0x0C,0x0C,0x0C,0x00,0x00,/*"4",4*/

0x00,0xFE,0xC0,0xC0,0xC0,0xF8,0xEC,0xC6,0x06,0x06,0x06,0xC6,0x6C,0x38,0x00,0x00,/*"5",5*/

0x00,0x38,0x6C,0xC6,0xC6,0xC0,0xF8,0xEC,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"6",6*/

0x00,0xFE,0x06,0x06,0x0C,0x0C,0x0C,0x18,0x18,0x18,0x30,0x30,0x30,0x30,0x00,0x00,/*"7",7*/

0x00,0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"8",8*/

0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,0x6E,0x3E,0x06,0xC6,0xC6,0x6C,0x38,0x00,0x00,/*"9",9*/
};

#define WIDTH 8
#define HEIGHT 16

static int get_ascii_fontdata(const char *code, uint8_t *mat, uint16_t *len)
{
    if (*code > '9' || *code < '0') {
        return -1;
    }

    char ascii_char = *code - '0';
    uint16_t i;
    uint16_t char_size = HEIGHT * (WIDTH / 8 + (WIDTH % 8 ? 1 : 0));
    uint32_t char_offset = ascii_char * char_size;
    const uint8_t *ptr = &Font16_num_Table[char_offset];

    for (i = 0; i < char_size; i++) {
        mat[i] = ptr[i];
    }
    *len = char_size;
    return 0;
}

const sFONT Font16_num = {
    .get_fontdata = get_ascii_fontdata,
    .Width = WIDTH, /* Width */
    .Height = HEIGHT, /* Height */
    .data_size = HEIGHT * (WIDTH / 8 + (WIDTH % 8 ? 1 : 0)),
};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
