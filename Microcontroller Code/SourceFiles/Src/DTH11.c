#include "DTH11.h"



/* Function protorypes */
static void DTH11_TimerStart(void);
static void DTH11_TimerStop(void);

TIM_HandleTypeDef TIM_TimBaseStructure;

uint8_t DTH11_ReadTemperature(void) {

	/* Initialize and start DTH11 TIMx */
	DTH11_TimerStart();


	uint8_t i, j, temp;
	uint8_t data[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Generate START condition */

	GPIO_InitStructure.Pin = DTH11_DATA_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(DTH11_DATA_PIN_GPIO_PORT, &GPIO_InitStructure);


	/* Put LOW for at least 18ms */
	HAL_GPIO_WritePin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN, GPIO_PIN_RESET);


	/* Wait 18 ms */
	TIM11->CNT = 0;
	while((TIM11->CNT) <= 18000) {
		;
	}

	HAL_GPIO_WritePin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN, GPIO_PIN_SET);

	//wait 40 us
	TIM11->CNT = 0;
	while ((TIM11->CNT) <= 40) {
		;
	}
	/* End start condition */


	/* Input mode to receive data */
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

	HAL_GPIO_Init(DTH11_DATA_PIN_GPIO_PORT, &GPIO_InitStructure);


	/* DHT11 ACK */
	/* should be LOW for at least 80 us */

	TIM11->CNT = 0;
	while (!HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN)) {
		if (TIM11->CNT > 100)
			return 0;
	}

	/* should be HIGH for at least 80 us */
	TIM11->CNT = 0;
	while (HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN)) {
		if (TIM11->CNT > 100)
			return 0;
	}

	/* Read 40 bits (8*5) */
	for (j = 0; j < 5; ++j) {
		for (i = 0; i < 8; ++i) {
			TIM11->CNT = 0;
			/* LOW for 50 us */
			while (!HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN)){
				if (TIM11->CNT > 100) {
					return 0;
				}
			}

			TIM11->CNT = 0;

			/* HIGH for 26-28 us = 0 / 70 us = 1 */
			while (HAL_GPIO_ReadPin(DTH11_DATA_PIN_GPIO_PORT, DTH11_DATA_PIN)) {
				if (TIM11->CNT > 100) {
					return 0;
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


	/* verify the Checksum */
	if (data[4] != (data[0] + data[2])) {
		return 0;
	}

	//dev->temparature = data[2];
	//dev->humidity = data[0];

	/* Deinitialize and stop DTH11 TIMx */
	DTH11_TimerStop();
	return data[2];
}


static void DTH11_TimerStart(void) {

	DTH11_TIMx_CLOCK_ENABLE();
	DTH11_GPIO_CLK_ENABLE();

	/* Initialise TIM11 */
	TIM_TimBaseStructure.Instance = DTH11_TIMx;

	TIM_TimBaseStructure.Init.Period = 59999;
	TIM_TimBaseStructure.Init.Prescaler = 215;
	TIM_TimBaseStructure.Init.ClockDivision = 0;
	TIM_TimBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;

	if(HAL_TIM_Base_Init(&TIM_TimBaseStructure) == HAL_OK) {
		HAL_TIM_Base_Start(&TIM_TimBaseStructure);
	}

}

static void DTH11_TimerStop(void) {

	DTH11_TIMx_CLOCK_DISABLE();
	DTH11_GPIO_CLK_ENABLE();

	if(HAL_TIM_Base_DeInit(&TIM_TimBaseStructure) == HAL_OK) {
		HAL_TIM_Base_Stop(&TIM_TimBaseStructure);
	}

}

