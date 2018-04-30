
// Includes 

#include "stm32f7xx_it.h"
#include "GUI.h"

extern volatile GUI_TIMER_TIME OS_TimeMS;

extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef  htim3;
extern TIM_HandleTypeDef  htim7;
extern TIM_HandleTypeDef  htim2;
extern UART_HandleTypeDef UartHandle;
extern DMA_HandleTypeDef  DmaTxHandle;
extern DMA_HandleTypeDef  DmaRxHandle;
extern SD_HandleTypeDef   uSdHandle;

// HardFault_Handler

void HardFault_Handler(void) 
{
    while (1) 
    {
        ;;
    }
}

// SysTick_Handler

void SysTick_Handler(void) 
{
  	HAL_IncTick();
    OS_TimeMS++;
}

// TIM3_IRQHandler

void TIM3_IRQHandler(void) 
{
    HAL_TIM_IRQHandler(&htim3);
}

// TIM7_IRQHandler

void TIM7_IRQHandler(void) 
{
    HAL_TIM_IRQHandler(&htim7);
}

// USARTx_DMA_RX_IRQHandler

void DMA2_Stream1_IRQHandler(void) 
{
    HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

// DMA2_Stream6_IRQHandler

void DMA2_Stream6_IRQHandler(void) 
{
    HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

// USART6_IRQHandler

void USART6_IRQHandler(void)
{
    HAL_UART_IRQHandler(&UartHandle);
}

// LTDC_IRQHandler

void LTDC_IRQHandler(void) 
{
    HAL_LTDC_IRQHandler(&hltdc);
}

// EXTI15_10_IRQHandler

void EXTI15_10_IRQHandler(void) 
{
    HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN);
}

// BSP_SDMMC_IRQHandler

void BSP_SDMMC_IRQHandler(void) 
{
    HAL_SD_IRQHandler(&uSdHandle);
}

// BSP_SDMMC_DMA_Tx_IRQHandler

void BSP_SDMMC_DMA_Tx_IRQHandler(void) 
{
    HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

// BSP_SDMMC_DMA_Rx_IRQHandler

void BSP_SDMMC_DMA_Rx_IRQHandler(void) 
{
    HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}
