#ifndef __IIC__H
#define __IIC__H

#include "esp_log.h"
#include "driver/i2c.h"






typedef struct
{

	float tempreture;
	float humidity;

} SHT20_INFO;

extern SHT20_INFO sht20Info;





void SHT20_GetValue(void);
void SHT20_reset(void);
esp_err_t sht20_init(void);

#endif

