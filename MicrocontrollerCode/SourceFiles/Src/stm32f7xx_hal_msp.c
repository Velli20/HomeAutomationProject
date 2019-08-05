
// Includes

#include "stm32f7xx_hal.h"

extern DMA_HandleTypeDef DmaTxHandle;
extern DMA_HandleTypeDef DmaRxHandle;

// HAL_MspInit

void HAL_MspInit(void)
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // MemoryManagement_IRQn interrupt configuration.

    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);

    // BusFault_IRQn interrupt configuration.

    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

    // UsageFault_IRQn interrupt configuration.

    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

    // DebugMonitor_IRQn interrupt configuration.

    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);

    // SysTick_IRQn interrupt configuration.

    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// HAL_TIM_Base_MspInit

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    // Enable peripherals and GPIO Clocks.

	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM7_CLK_ENABLE();

	// Set the TIMx interrupt priority.

	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 1);

	// Enable the TIM3 and TIM7 global Interrupt.

	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

// HAL_TIM_Base_MspDeInit

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim)
{
	// Disable GPIO Clocks.

	__HAL_RCC_TIM3_CLK_DISABLE();
	__HAL_RCC_TIM7_CLK_DISABLE();

	// Disable the TIM3 and TIM7 global Interrupt.

	HAL_NVIC_DisableIRQ(TIM3_IRQn);
	HAL_NVIC_DisableIRQ(TIM7_IRQn);
}

// HAL_RTC_MspInit

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    // Enable access on RTC registers.

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    RCC_OscInitStruct.OscillatorType= RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState=   RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState=       RCC_LSI_ON;
    RCC_OscInitStruct.LSEState=       RCC_LSE_OFF;

	HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection= RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection=    RCC_RTCCLKSOURCE_LSI;

	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    __HAL_RCC_RTC_ENABLE();
}

// HAL_RTC_MspDeInit

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    __HAL_RCC_RTC_DISABLE();
}

// HAL_UART_MspInit

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

	// Enable TX/RX GPIO clock.

	__GPIOC_CLK_ENABLE();

	// Enable USART6 clock.

	__USART6_CLK_ENABLE();

	// Enable DMA2 clock.

	__HAL_RCC_DMA2_CLK_ENABLE();

	// UART TX GPIO pin configuration.

	GPIO_InitStruct.Pin=       GPIO_PIN_6;
	GPIO_InitStruct.Mode=      GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull=      GPIO_PULLUP;
	GPIO_InitStruct.Speed=     GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate= GPIO_AF8_USART6;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// UART RX GPIO pin configuration.

	GPIO_InitStruct.Pin= GPIO_PIN_7;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Configure the DMA handler for Transmission process.

	DmaTxHandle.Instance=                 DMA2_Stream6;
	DmaTxHandle.Init.Channel=             DMA_CHANNEL_5;
	DmaTxHandle.Init.Direction=           DMA_MEMORY_TO_PERIPH;
	DmaTxHandle.Init.PeriphInc=           DMA_PINC_DISABLE;
	DmaTxHandle.Init.MemInc=              DMA_MINC_ENABLE;
	DmaTxHandle.Init.PeriphDataAlignment= DMA_PDATAALIGN_BYTE;
	DmaTxHandle.Init.MemDataAlignment=    DMA_MDATAALIGN_BYTE;
	DmaTxHandle.Init.Mode=                DMA_NORMAL;
	DmaTxHandle.Init.Priority=            DMA_PRIORITY_LOW;

	HAL_DMA_Init(&DmaTxHandle);

    // Associate the initialized DMA handle to the UART handle.

	__HAL_LINKDMA(huart, hdmatx, DmaTxHandle);

	// Configure the DMA handler for reception process.

	DmaRxHandle.Instance=                 DMA2_Stream1;
	DmaRxHandle.Init.Channel=             DMA_CHANNEL_5;
	DmaRxHandle.Init.Direction=           DMA_PERIPH_TO_MEMORY;
	DmaRxHandle.Init.PeriphInc=           DMA_PINC_DISABLE;
	DmaRxHandle.Init.MemInc=              DMA_MINC_ENABLE;
	DmaRxHandle.Init.PeriphDataAlignment= DMA_PDATAALIGN_BYTE;
	DmaRxHandle.Init.MemDataAlignment=    DMA_MDATAALIGN_BYTE;
	DmaRxHandle.Init.Mode=                DMA_CIRCULAR;
	DmaRxHandle.Init.Priority=            DMA_PRIORITY_HIGH;

	HAL_DMA_Init(&DmaRxHandle);

	// Associate the initialized DMA handle to the the UART handle.

	__HAL_LINKDMA(huart, hdmarx, DmaRxHandle);

	// NVIC configuration for DMA transfer complete interrupt (USART6_TX).

	HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

	// NVIC configuration for DMA transfer complete interrupt (USART6_RX).

	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

	// NVIC for USART, to catch the TX complete.

	HAL_NVIC_SetPriority(USART6_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
}

// HAL_UART_MspDeInit

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	// Reset peripherals.

	__USART6_FORCE_RESET();
	__USART6_RELEASE_RESET();

	// Disable peripherals and GPIO Clocks.

	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);

	// De-Initialize the DMA channel associated to reception process.

	if ( huart && huart->hdmarx )
		  HAL_DMA_DeInit(huart->hdmarx);

    // De-Initialize the DMA channel associated to transmission process.

	if ( huart && huart->hdmatx )
	   	HAL_DMA_DeInit(huart->hdmatx);

	// Disable the NVIC for DMA.

	HAL_NVIC_DisableIRQ(DMA2_Stream7_IRQn);
	HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);
}
