#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H

#include <stdint.h>

#define LED_MATRIX_MAX_WIDTH   64
#define LED_MATRIX_MAX_HEIGHT  32

#define COLOR_BLACK  0
#define COLOR_RED    1
#define COLOR_GREEN  2
#define COLOR_YELLOW 3

void LedMatrix_init(void);

void LedMatrix_Clear(void);
void LedMatrix_DrawPoint(uint16_t x,uint16_t y,uint16_t t);
void LedMatrix_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t dot);
void LedMatrix_ShowChar(uint16_t x,uint16_t y,uint8_t chr,uint8_t size);
void LedMatrix_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
void LedMatrix_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint8_t size);
void LedMatrix_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LedMatrix_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LedMatrix_Draw_Circle(uint16_t x0,uint16_t y0,uint16_t r);
void LedMatrix_DrawBMP(uint16_t x0, uint16_t y0,uint16_t x1, uint16_t y1,const uint8_t BMP[]);

void LedMatrix_SetLight(uint32_t val);


#endif