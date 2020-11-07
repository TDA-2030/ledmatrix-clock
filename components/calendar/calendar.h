#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#include "stdint.h"

//时间结构体
typedef struct {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    //公历或农历日月年
    uint16_t w_year;
    uint8_t  w_month;
    uint8_t  w_date;
    uint8_t  week;
} calendar_t;


void calendar_get_gregorian(uint32_t timecount, calendar_t *Calendar);
void calendar_get_lunar_str(const calendar_t *calendar, char *str);
void calendar_get_jieqi_str(const calendar_t *calendar, char *str);

#endif



















