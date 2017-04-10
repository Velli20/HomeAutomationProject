
/* Includes */
#include "main.h"
#include "stm32f7xx_it.h"
#include "GUI.h"

/* Private variables ---------------------------------------------------------*/
extern volatile GUI_TIMER_TIME OS_TimeMS;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef UartHandle;
extern DMA_HandleTypeDef DmaTxHandle;
extern DMA_HandleTypeDef DmaRxHandle;

/* SD handler declared in "stm32746g_discovery_sd.c" file */
extern SD_HandleTypeDef uSdHandle;


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void) {
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void) {
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1) {
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void) {
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1) {
	}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void) {
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1) {
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void) {
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1) {
	}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void) {
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void) {
	/* Update the LocalTime by adding 1 ms each SysTick interrupt */
	HAL_IncTick();
  
    OS_TimeMS++;
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void) {
}*/

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim3);
}

/**
  * @brief  This function handles TIM7 interrupt request.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim7);
}


/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "SerialCommandWriterReader.h" and related to DMA
  *         used for USART data transmission
  */
void USARTx_DMA_RX_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmarx);
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "SerialCommandWriterReader.h" and related to DMA
  *         used for USART data reception
  */
void USARTx_DMA_TX_IRQHandler(void) {
	HAL_DMA_IRQHandler(UartHandle.hdmatx);
}

/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "SerialCommandWriterReader.h" and related to DMA
  *         used for USART data transmission
  */
void USARTx_IRQHandler(void) {
	HAL_UART_IRQHandler(&UartHandle);
}


/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void) {
	HAL_LTDC_IRQHandler(&hltdc);
}

/**
  * @brief  This function handles external line 15_10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN);
}

/**
  * @brief  This function handles SDMMC1 global interrupt request.
  * @param  None
  * @retval None
  */
void BSP_SDMMC_IRQHandler(void) {
	HAL_SD_IRQHandler(&uSdHandle);
}

/**
* @brief  This function handles DMA2 Stream 6 interrupt request.
* @param  None
* @retval None
*/
void BSP_SDMMC_DMA_Tx_IRQHandler(void) {
	HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

/**
* @brief  This function handles DMA2 Stream 3 interrupt request.
* @param  None
* @retval None
*/
void BSP_SDMMC_DMA_Rx_IRQHandler(void) {
	HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}


/* END OF FILE */
