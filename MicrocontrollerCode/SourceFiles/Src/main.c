// Includes.

#include "main.h"
#include "RoomConfiguration.h"
#include "SerialCommandWriterReader.h"
#include "real_time_clock.h"

// Volatile variables.

__IO ITStatus UpdateTemperature;
__IO ITStatus SdCardInserted;
__IO ITStatus UpdateStatusBarTime;
__IO ITStatus BspBackground;
__IO ITStatus UartRxReady;
__IO ITStatus UartTxReady;

UART_HandleTypeDef UartHandle;
TIM_HandleTypeDef  htim3;
TIM_HandleTypeDef  htim7;

uint8_t temperatureReading= 0;

APP_DATA* application_data;

// Main_InitApplicationData

static void Main_InitApplicationData(void)
{
	UpdateTemperature=   RESET;
    SdCardInserted=      RESET;
    UpdateStatusBarTime= RESET;
    BspBackground=       RESET;
    UartRxReady=         RESET;
    UartTxReady=         RESET;

	application_data= calloc(1, sizeof(APP_DATA));
	if ( !application_data )
		ErrorHandler();
}

// SystemClock_Config

static void SystemClock_Config(void) 
{
	RCC_ClkInitTypeDef       RCC_ClkInitStruct;
	RCC_OscInitTypeDef       RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	// Enable HSE Oscillator and activate PLL with HSE as source.

	RCC_OscInitStruct.OscillatorType= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState=       RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState=   RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource=  RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM=       25;
	RCC_OscInitStruct.PLL.PLLN=       432;
	RCC_OscInitStruct.PLL.PLLP=       RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ=       9;

	if ( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK ) 
		ErrorHandler();

	// Activate the OverDrive to reach the 216 MHz Frequency.

	if ( HAL_PWREx_EnableOverDrive() != HAL_OK ) 
		ErrorHandler();

	// Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers.

	RCC_ClkInitStruct.ClockType=      (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource=   RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider=  RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider= RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider= RCC_HCLK_DIV2;

	if ( HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK ) 
		ErrorHandler();

	PeriphClkInitStruct.PeriphClockSelection= RCC_PERIPHCLK_SDMMC1;
	PeriphClkInitStruct.Sdmmc1ClockSelection= RCC_SDMMC1CLKSOURCE_CLK48;

	if ( HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK ) 
	    ErrorHandler();
}

// UartConfig

static void UartConfig(void) 
{
	// Configure the UART peripheral.

	/*
	 - Word Length = 8 Bits
	 - Stop Bit = One Stop bit
	 - Parity = None
	 - BaudRate = 9600 baud
	 - Hardware flow control disabled (RTS and CTS signals) 
	*/
	UartHandle.Instance=          USART6;
	UartHandle.Init.BaudRate=     9600;
	UartHandle.Init.WordLength=   UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits=     UART_STOPBITS_1;
	UartHandle.Init.Parity=       UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl=    UART_HWCONTROL_NONE;
	UartHandle.Init.Mode=         UART_MODE_TX_RX;

	if ( HAL_UART_DeInit(&UartHandle) != HAL_OK) 
		 ErrorHandler();

	// Init the UART.

	if ( HAL_UART_Init(&UartHandle) != HAL_OK)
		 ErrorHandler();

	// Put UART peripheral in reception process.

	if ( HAL_UART_Receive_DMA(&UartHandle, &(application_data->rx_buffer), 1) != HAL_OK)
		ErrorHandler();
}

// HardwareConfig

static void HardwareConfig(void)
{
	BSP_LED_Init(LED1);
	BSP_LED_Init(LED_GREEN);
	
	// Initialize the SDRAM.
	
	BSP_SDRAM_Init();
	
	// Initialize the Touch screen.

	BSP_TS_Init(420, 272);

	// Enable CRC to Unlock GUI.
	
	__HAL_RCC_CRC_CLK_ENABLE();

	// Enable Back up SRAM.
	
	__HAL_RCC_BKPSRAM_CLK_ENABLE();

	// Configure the Real Time Clock.

	BSP_RTC_Init();

	// Configure Interrupt mode for SD detection pin and enable and set SD detect EXTI Interrupt.

	if ( BSP_SD_ITConfig() != MSD_OK )
	    ErrorHandler();
}

// main_read_temperature_sensor

static void main_read_temperature_sensor(void) 
{
	static uint8_t updateTicks = -1;
	uint8_t humidity;

	DTH11_ReadTemperature(&temperatureReading, &humidity);
	
	temperatureReading= 0;
	
	// Log temperature every 30 s.

	if ( updateTicks == -1 || updateTicks >= 60 )
	{
		updateTicks = 0;
		temperature_data_logger_log_temperature(application_data, temperatureReading);
	} 
	else 
	{
		updateTicks++;
	}

	event_handler_send_event(application_data, 0, EVENT_TEMPERATURE_UPDATED);

}

// Main_TouchUpdate

static void Main_TouchUpdate(APP_DATA* data)
{
	TS_StateTypeDef ts;

	if ( !data )
	    return;

	BSP_TS_GetState(&ts);

	if ( (ts.touchX[0] >= LCD_GetXSize()) || (ts.touchY[0] >= LCD_GetYSize()) )
	{
		ts.touchX[0]=     0;
		ts.touchY[0]=     0;
		ts.touchDetected= 0;
	}

	data->previous_touch.Pressed= ts.touchDetected;
	data->previous_touch.Layer=   0;
		
	if ( ts.touchDetected ) 
	{
		data->previous_touch.x= ts.touchX[0];
		data->previous_touch.y= ts.touchY[0];
		GUI_TOUCH_StoreStateEx(&data->previous_touch);
	} 
	else 
	{
		GUI_TOUCH_StoreStateEx(&data->previous_touch);
		data->previous_touch.x= 0;
		data->previous_touch.y= 0;
	}
}

// main

int main(void) 
{
	uint32_t prescaler;

	// Init app data.

	Main_InitApplicationData();
	
	// Enable the CPU Cache .

	SCB_EnableICache();

	// STM32F7xx HAL library initialization.

	if ( HAL_Init() != HAL_OK )
	    ErrorHandler();

	// Configure the system clock to 200 MHz.

	SystemClock_Config();

	// Configure the Debug console.

	Log_Config();
	
	// Configure the UART.

	UartConfig();

	// Configure the touch screen, RTC, SDRAM etc.

	HardwareConfig();

	// Compute the prescaler value to have TIM3 counter clock equal to 10 KHz.

	prescaler= (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

	// Initialize TIM3 peripheral.

	htim3.Instance=           TIM3;
	htim3.Init.Period=        500 - 1;
	htim3.Init.Prescaler=     prescaler;
	htim3.Init.ClockDivision= 0;
	htim3.Init.CounterMode=   TIM_COUNTERMODE_UP;
	
	// Initialize timer.

	if ( HAL_TIM_Base_Init(&htim3) != HAL_OK ) 
	    ErrorHandler();

	// Start the TIM3 Base generation in interrupt mode.
		
	if ( HAL_TIM_Base_Start_IT(&htim3) != HAL_OK )
	    ErrorHandler();
	
	// This timer is being used to generate an interrupt every 500 ms to update main window clock and temperature reading.

	htim7.Instance=           TIM7;
	htim7.Init.Period=        5000;
	htim7.Init.Prescaler=     prescaler;
	htim7.Init.ClockDivision= 0;
	htim7.Init.CounterMode=   TIM_COUNTERMODE_UP;

	// Initialize timer.

	if ( HAL_TIM_Base_Init(&htim7) != HAL_OK ) 
		ErrorHandler();
		
	// Start the TIM7 Base generation in interrupt mode.

	if ( HAL_TIM_Base_Start_IT(&htim7) != HAL_OK )
	    ErrorHandler();

	// Init the STemWin GUI Library.

	GUI_Init();

	// Add motion support for Window Manager.

	WM_MOTION_Enable(1);

	// Activate the use of memory device feature.

	WM_SetCreateFlags(WM_CF_MEMDEV);

	// Get temperature reading.

	main_read_temperature_sensor();

	// Create main Menu window.

	window_menu_create_window();

	// Read RoomConfiguration if sd card is detected on startup.

	if ( BSP_SD_IsDetected() == SD_PRESENT )
	    room_configuration_read_from_sd(application_data);

	while (1) 
	{
		WM_Exec();

		if ( UpdateTemperature == SET ) 
		{
			UpdateTemperature= RESET;
			main_read_temperature_sensor();
		}
		if ( SdCardInserted == SET )
		{
			SdCardInserted= RESET;

			room_configuration_read_from_sd(application_data);
		    event_handler_send_event(application_data, 0, EVENT_SD_CARD_DETECTED);
		}
		if ( UartRxReady == SET ) 
		{
			SerialCommandWriterReader_RxReadMessage();
			UartRxReady= RESET;
		}
		if ( UartTxReady == SET ) 
		{
			//SerialCommandWriterReader_ClearTxBuffer();
			UartTxReady= RESET;
		}
		if ( UpdateStatusBarTime == SET ) 
		{
			event_handler_send_event(application_data, 0, EVENT_REAL_TIME_CLOCK_TICK);
			UpdateStatusBarTime= RESET;
		}
		if ( BspBackground == SET ) 
		{
			// Read touch state.

			if ( GUI_IsInitialized() ) 
			    Main_TouchUpdate(application_data);

			BspBackground= RESET;
		}
	}

	return(0);
}

// HAL_TIM_PeriodElapsedCallback

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) 
{

	if ( htim->Instance == TIM3 ) 
	{
		BspBackground = SET;
	} 
	else if ( htim->Instance == TIM7 ) 
	{
		UpdateTemperature=   SET;
		UpdateStatusBarTime= SET;
	}
}

// HAL_GPIO_EXTI_Callback

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if ( GPIO_Pin == SD_DETECT_PIN ) 
		SdCardInserted = SET;
}

// HAL_UART_TxCpltCallback

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* hUart) 
{
	UartTxReady= SET;
}

// HAL_UART_RxCpltCallback

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* hUart) 
{
	UartRxReady= SET;
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{}
}
#endif
