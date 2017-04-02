#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "main.h"

/* Function protoypes */
void BSP_Init(void);
void BSP_TouchUpdate(void);
void BSP_Background(void);

HAL_StatusTypeDef BSP_RTC_SetDate(uint8_t day, uint8_t month, uint8_t year);
HAL_StatusTypeDef BSP_RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

HAL_StatusTypeDef BSP_RTC_Init(void);
RTC_TimeTypeDef BSP_RTC_GetTime(void);
RTC_DateTypeDef BSP_RTC_GetDate(void);


/* Defines related to RTC configuration */

#define RTC_CLOCK_SOURCE_LSE
/*#define RTC_CLOCK_SOURCE_LSI*/


#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif


#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
