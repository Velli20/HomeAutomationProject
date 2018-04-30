#ifndef __REAL_TIME_CLOCK
#define __REAL_TIME_CLOCK

// Includes

#include "stm32f7xx_hal.h"

// BSP_RTC_SetDate

void BSP_RTC_SetDate(uint8_t  day, 
                     uint8_t  month, 
                     uint16_t year);

// BSP_RTC_SetTime

void BSP_RTC_SetTime(uint8_t hours, 
                     uint8_t minutes, 
                     uint8_t seconds);

// BSP_RTC_Init

void BSP_RTC_Init(void);

// BSP_RTC_GetTime

RTC_TimeTypeDef BSP_RTC_GetTime(void);

// BSP_RTC_GetDate

RTC_DateTypeDef BSP_RTC_GetDate(void);


#endif 
