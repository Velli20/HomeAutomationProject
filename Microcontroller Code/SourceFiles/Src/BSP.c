/* Includes */
#include "BSP.h"


/* Private variables */
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;
/* LCD layer declaration */
uint8_t SelLayer;


/**
 * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
 * @param  None
 * @retval None
 */
void BSP_Init(void) {
	/* Configure LED1 */
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED_GREEN);
	/* Initialize the SDRAM */
	BSP_SDRAM_Init();
	/* Initialize the Touch screen */
	BSP_TS_Init(420, 272);

	/* Enable CRC to Unlock GUI */__HAL_RCC_CRC_CLK_ENABLE();

	/* Enable Back up SRAM */__HAL_RCC_BKPSRAM_CLK_ENABLE();
	SelLayer = 0;

}

/**
 * @brief  BSP_Background.
 * @param  None
 * @retval None
 */
void BSP_Background(void) {
	/* Capture input event and update cursor */
	if (GUI_IsInitialized()) {
		BSP_TouchUpdate();
	}
}

/**
 * @brief  Configure the RTC Date
 * @param  day:   Specifies the RTC Date
 * @param  month: Specifies the RTC Date Month (in BCD format)
 * @param  year: Specifies the RTC Date Year
 * @retval HAL_StatusTypeDef: HAL_OK if setting date succeeded
 */
HAL_StatusTypeDef BSP_RTC_SetDate(uint8_t day, uint8_t month, uint8_t year) {
	RTC_DateTypeDef sdatestructure;

	sdatestructure.Year = 17;
	sdatestructure.Month = month;
	sdatestructure.Date = day;

	if (HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BIN) != HAL_OK) {
		return HAL_ERROR;
	}

	/* Writes a data in a RTC Backup data Register1 */
	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);

	return HAL_OK;
}

/**
 * @brief  Configure the RTC Time
 * @param  hours: Specifies the RTC Time Hour
 * @param  minutes: Specifies the RTC Time Minutes
 * @param  seconds: Specifies the RTC Time Seconds
 * @retval HAL_StatusTypeDef: HAL_OK if setting time succeeded
 */
HAL_StatusTypeDef BSP_RTC_SetTime(int hours, int minutes, int seconds) {
  RTC_TimeTypeDef stimestructure;

  stimestructure.Hours = hours;
  stimestructure.Minutes = minutes;
  stimestructure.Seconds = seconds;
  stimestructure.TimeFormat = RTC_HOURFORMAT_24;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN) != HAL_OK) {
	  return HAL_ERROR;
  }

  /* Writes a data in a RTC Backup data Register1 */
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);

  return HAL_OK;
}


/**
 * @brief  Read the coordinate of the point touched and assign their
 *         value to the variables u32_TSXCoordinate and u32_TSYCoordinate
 * @param  None
 * @retval None
 */
void BSP_TouchUpdate(void) {
	static GUI_PID_STATE TS_State = { 0, 0, 0, 0 };
	TS_StateTypeDef ts;
	uint16_t xDiff, yDiff;

	BSP_TS_GetState((TS_StateTypeDef *) &ts);

	if ((ts.touchX[0] >= LCD_GetXSize()) || (ts.touchY[0] >= LCD_GetYSize())) {
		ts.touchX[0] = 0;
		ts.touchY[0] = 0;
		ts.touchDetected = 0;
	}

	xDiff = (TS_State.x > ts.touchX[0]) ?
			(TS_State.x - ts.touchX[0]) : (ts.touchX[0] - TS_State.x);
	yDiff = (TS_State.y > ts.touchY[0]) ?
			(TS_State.y - ts.touchY[0]) : (ts.touchY[0] - TS_State.y);

	if ((TS_State.Pressed != ts.touchDetected) || (xDiff > 30)
			|| (yDiff > 30)) {
		TS_State.Pressed = ts.touchDetected;
		TS_State.Layer = SelLayer;
		if (ts.touchDetected) {
			TS_State.x = ts.touchX[0];
			TS_State.y = ts.touchY[0];
			GUI_TOUCH_StoreStateEx(&TS_State);
		} else {
			GUI_TOUCH_StoreStateEx(&TS_State);
			TS_State.x = 0;
			TS_State.y = 0;
		}
	}
}

/**
 * @brief  Read the RTC current Time
 * @param  None
 * @retval HAL_StatusTypeDef: HAL Status structure
 *         HAL_OK if initialization succeeded
 */
HAL_StatusTypeDef BSP_RTC_Init(void) {
	/*##-1- Configure the RTC peripheral #######################################*/
	RtcHandle.Instance = RTC;

	/* Configure RTC prescaler and RTC data registers */
	/* RTC configured as follows:
	 - Hour Format    = Format 24
	 - Asynch Prediv  = Value according to source clock
	 - Synch Prediv   = Value according to source clock
	 - OutPut         = Output Disable
	 - OutPutPolarity = High Polarity
	 - OutPutType     = Open Drain */
	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&RtcHandle) != HAL_OK) {
		/* Initialization Error */
		return HAL_ERROR;
	}
	BSP_RTC_SetTime(4, 50, 0);
	BSP_RTC_SetDate(20, 1, 17);
	return HAL_OK;
}


/**
 * @brief  Read the RTC current Time
 * @param  None
 * @retval RTC_TimeTypeDef: RTC Time structure
 */
RTC_TimeTypeDef BSP_RTC_GetTime(void) {
    RTC_TimeTypeDef stimestructureget;

	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	return stimestructureget;
}

/**
 * @brief  Read the RTC current Date
 * @param  None
 * @retval RTC_DateTypeDef: RTC Date structure
 */
RTC_DateTypeDef BSP_RTC_GetDate(void) {
	RTC_DateTypeDef sdatestructureget;

	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	return sdatestructureget;
}


/* End of file */
