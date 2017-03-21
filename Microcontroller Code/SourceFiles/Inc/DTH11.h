#ifndef __DTH11_H
#define __DTH11_H

/* Includes */
#include "main.h"


#define DTH11_MAX_TICS 10000

/* Definition for DTH11 sensor Pins */
#define DTH11_DATA_PIN                   GPIO_PIN_15
#define DTH11_DATA_PIN_GPIO_PORT         GPIOA
#define DTH11_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define DTH11_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOA_CLK_DISABLE()

/* Definition for DTH11 sensor timer */
#define DTH11_TIMx        				 TIM11
#define DTH11_TIMx_CLOCK_ENABLE()		 __HAL_RCC_TIM11_CLK_ENABLE();
#define DTH11_TIMx_CLOCK_DISABLE()		 __HAL_RCC_TIM11_CLK_DISABLE()

/* Exported funtion prototypes */
uint8_t DTH11_ReadTemperature(void);

#endif __DTH11_H
/* END OF FILE */
