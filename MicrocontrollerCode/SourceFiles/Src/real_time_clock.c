// Includes

#include "real_time_clock.h"
#include "main.h"

// Defines

#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF

// RTC handle declaration.

static RTC_HandleTypeDef RtcHandle;

// BSP_RTC_SetDate

void BSP_RTC_SetDate(uint8_t  day, 
                     uint8_t  month, 
                     uint16_t year)
{
	RTC_DateTypeDef sdatestructure;

	// Max year is 99.
	
	if ( year > 2000 )
	    year-= 2000;

	sdatestructure.Year=  year;
	sdatestructure.Month= month;
	sdatestructure.Date=  day;

	if ( HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BIN) != HAL_OK ) 
		ErrorHandler();

	// Writes a data in a RTC Backup data Register1.

	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

// BSP_RTC_SetTime

void BSP_RTC_SetTime(uint8_t hours, 
                     uint8_t minutes, 
                     uint8_t seconds)
{
	RTC_TimeTypeDef stimestructure;

	stimestructure.Hours=          hours;
	stimestructure.Minutes=        minutes;
	stimestructure.Seconds=        seconds;
	stimestructure.TimeFormat=     RTC_HOURFORMAT_24;
	stimestructure.DayLightSaving= RTC_DAYLIGHTSAVING_NONE ;
	stimestructure.StoreOperation= RTC_STOREOPERATION_RESET;

	if ( HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN) != HAL_OK ) 
	    ErrorHandler();

	// Writes a data in a RTC Backup data Register1.

	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}

// BSP_RTC_Init

void BSP_RTC_Init(void) 
{
	// Configure the RTC peripheral.
	
	RtcHandle.Instance= RTC;

	RtcHandle.Init.HourFormat=     RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv=   RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv=    RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut=         RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity= RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType=     RTC_OUTPUT_TYPE_OPENDRAIN;
	
	if ( HAL_RTC_Init(&RtcHandle) != HAL_OK ) 
		ErrorHandler();
	
	// Check if backup register is empty.

	if ( HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2 )
	{
		BSP_RTC_SetTime(0, 0, 0);
	    BSP_RTC_SetDate(1, 1, 2018);
	}
}

// BSP_RTC_GetTime

RTC_TimeTypeDef BSP_RTC_GetTime(void) 
{
    RTC_TimeTypeDef stimestructureget;

	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	return stimestructureget;
}

// BSP_RTC_GetDate

RTC_DateTypeDef BSP_RTC_GetDate(void) 
{
	RTC_DateTypeDef sdatestructureget;

	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	return sdatestructureget;
}
