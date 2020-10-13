#ifndef __WEATHER_H_
#define __WEATHER_H_


#include <stdint.h>

typedef enum{
    WEATHER_TYPE_NOW,
    WEATHER_TYPE_DAY,
}weather_type_t;

struct weather_now{
	uint8_t code;
	int8_t temp;
};

struct weather_day{
	uint8_t code;
	int8_t temp_low;
	int8_t temp_high;
	uint16_t wind_direction_degree;
	float wind_speed;
	uint8_t wind_scale;
	uint8_t humidity;
};

typedef struct{

	struct weather_now now;
	struct weather_day day[3];
}weather_info_t;




uint8_t weather_get(const char *cityid, weather_type_t type);

#endif