#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H

#include <stdint.h>

#define LED_MATRIX_MAX_WIDTH   64
#define LED_MATRIX_MAX_HEIGHT  32



//字体信息保存地址,占33个字节,第1个字节用于标记字库是否存在.后续每8个字节一组,分别保存起始地址和文件大小														   
//extern uint32_t FONTINFOADDR;	
//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
typedef struct 
{
	uint8_t fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
	uint32_t ugbkaddr; 			//unigbk的地址
	uint32_t ugbksize;			//unigbk的大小	 
	uint32_t f12addr;			//gbk12地址	
	uint32_t gbk12size;			//gbk12的大小
	uint32_t f16addr;			//gbk16地址
	uint32_t gbk16size;			//gbk16的大小		 
	uint32_t f24addr;			//gbk24地址
	uint32_t gbk24size;			//gbk24的大小 
	uint32_t f32addr;			//gbk32地址
	uint32_t gkb32size;			//gbk32的大小 
}FONT_Info_TypeDef; 

extern FONT_Info_TypeDef ftinfo;	//字库信息结构体





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

void LedMatrix_RollStringExce(void);
int8_t LedMatrix_RollStart(uint8_t x,uint8_t y,uint8_t length,uint8_t *p,uint8_t size);
int8_t LedMatrix_RollEnd(void);


#endif