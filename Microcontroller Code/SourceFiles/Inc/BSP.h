#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported functions ------------------------------------------------------- */
void BSP_Init(void);
void BSP_TouchUpdate(void);
HAL_StatusTypeDef BSP_RTC_SetDate(uint8_t day, uint8_t month, uint8_t year);
HAL_StatusTypeDef BSP_RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

HAL_StatusTypeDef BSP_RTC_Init(void);
RTC_TimeTypeDef BSP_RTC_GetTime();
RTC_DateTypeDef BSP_RTC_GetDate();


#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
