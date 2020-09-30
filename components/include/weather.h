#ifndef __WEATHER_H_
#define __WEATHER_H_


#include <stdint.h>

typedef enum{
    WEATHER_TYPE_NOW,
    WEATHER_TYPE_DAY,
}weather_type_t;

typedef struct{

	char mask;    //状态标志
	char now[2];  //实时天气状况代码、气温
	
	char day_0[3]; //分别天气状况代码、最高气温、最低气温
	char day_1[3];
	char day_2[3];
}Weather_TypeDef;
extern Weather_TypeDef Weather;//天气结构体




uint8_t weather_get(const char *cityid,weather_type_t type);

#endif