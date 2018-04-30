// Includes

#include "DTH11.h"
#include "main.h"

static TIM_HandleTypeDef TIM_TimBaseStructure;

//DTH11_TimerStart

static void DTH11_TimerStart(void) 
{
	DTH11_TIMx_CLOCK_ENABLE();
	DTH11_GPIO_CLK_ENABLE();

	TIM_TimBaseStructure.Instance=           DTH11_TIMx;
	TIM_TimBaseStructure.Init.Period=        59999;
	TIM_TimBaseStructure.Init.Prescaler=     215;
	TIM_TimBaseStructure.Init.ClockDivision= 0;
	TIM_TimBaseStructure.Init.CounterMode=   TIM_COUNTERMODE_UP;

	if ( HAL_TIM_Base_Init(&TIM_TimBaseStructure) != HAL_OK )
		ErrorHandler();

	if ( HAL_TIM_Base_Start(&TIM_TimBaseStructure) != HAL_OK )
	    ErrorHandler();
}

// DTH11_TimerStop

static void DTH11_TimerStop(void) 
{
	DTH11_TIMx_CLOCK_DISABLE();
	DTH11_GPIO_CLK_ENABLE();

	if ( HAL_TIM_Base_DeInit(&TIM_TimBaseStructure) != HAL_OK )
	    ErrorHandler();
		
	if ( HAL_TIM_Base_Stop(&TIM_TimBaseStructure) != HAL_OK )
	    ErrorHandler();
}

// DTH11_ReadTemperature

void DTH11_ReadTemperature(uint8_t* temperature,
                           uint8_t* humidity) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t          i, j, temp;
	uint8_t          data[5];

	// Initialize and start DTH11 TIMx.

	DTH11_TimerStart();

	// Initialize DTH11 GPIO pin.

	GPIO_InitStructure.Pin=   DTH11_DATA_PIN;
	GPIO_InitStructure.Mode=  GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed= GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull=  GPIO_NOPULL;

	HAL_GPIO_Init(DTH11_DATA_PIN_GPIO_PORT, &GPIO_InitStructure);

	// Set pin low.

	HAL_GPIO_WritePin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN, GPIO_PIN_RESET);

	// Wait 18 ms.

	TIM11->CNT= 0;
	while( (TIM11->CNT) <= 18000 ) 
	{
		;;
	}

	HAL_GPIO_WritePin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN, GPIO_PIN_SET);

	// Wait 40 us

	TIM11->CNT= 0;
	while ( (TIM11->CNT) <= 40 )
	{
		;;
	}

	// Input mode to receive data.
	
	GPIO_InitStructure.Mode= GPIO_MODE_INPUT;

	HAL_GPIO_Init(DTH11_DATA_PIN_GPIO_PORT, &GPIO_InitStructure);

	// Wait for ACK (low for 80 us and high for 80 us)

	TIM11->CNT = 0;
	while ( !HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN) ) 
	{
		if (TIM11->CNT > 100)
			return;
	}

	TIM11->CNT= 0;
	while ( HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN) ) 
	{
		if (TIM11->CNT > 100)
			return;
	}

	// Read data.

	memset(data, 0, sizeof(data));
	for ( j = 0; j < 5; ++j ) 
	{
		for (i = 0; i < 8; ++i) {
			TIM11->CNT = 0;
			/* LOW for 50 us */
			while (!HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN)){
				if (TIM11->CNT > 100) {
					return;
				}
			}

			TIM11->CNT = 0;

			/* HIGH for 26-28 us = 0 / 70 us = 1 */
			while (HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN)) {
				if (TIM11->CNT > 100) {
					return;
				}
			}

			/* Calc amount of time passed */
			temp = TIM11->CNT;

			/* shift 0 */
			data[j] = data[j] << 1;

			/* if > 30 us it's 1 */
			if (temp > 40) {
				data[j] = data[j] + 1;
			}
		}
	}


	// Verify the Checksum.

	if ( data[4] != (data[0] + data[2]) ) 
	{
		*temperature= 0;
	    *humidity=    0;
	}
	else
	{
		*temperature= data[2];
	    *humidity=    data[0];
	}

	// Deinitialize and stop DTH11 timer.

	DTH11_TimerStop();
}

