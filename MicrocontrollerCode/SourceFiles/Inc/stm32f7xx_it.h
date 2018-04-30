#ifndef __STM32F7xx_IT_H
#define __STM32F7xx_IT_H

// Includes

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery.h"

// HardFault_Handler

void HardFault_Handler(void);

// SysTick_Handler

void SysTick_Handler(void);

// DMA2_Stream1_IRQHandler

void DMA2_Stream1_IRQHandler(void);

// DMA2_Stream6_IRQHandler

void DMA2_Stream6_IRQHandler(void);

// USART6_IRQHandler

void USART6_IRQHandler(void);

// BSP_SDMMC_IRQHandler

void BSP_SDMMC_IRQHandler(void);

// BSP_SDMMC_DMA_Tx_IRQHandler

void BSP_SDMMC_DMA_Tx_IRQHandler(void);

// BSP_SDMMC_DMA_Rx_IRQHandler

void BSP_SDMMC_DMA_Rx_IRQHandler(void);

#endif
