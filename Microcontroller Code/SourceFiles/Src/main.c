/* Includes */
#include "main.h"


/* Volatile variables */
__IO ITStatus UpdateTemperature = RESET;
__IO ITStatus SdCardInserted = RESET;
__IO ITStatus UpdateStatusBarTime = RESET;
__IO ITStatus BspBackground= RESET;

/* Declared in SerialCommandWriterReader.c */
extern __IO ITStatus UartRxReady;
extern __IO ITStatus UartTxReady;

/* Timer3 handler declaration */
TIM_HandleTypeDef htim3;

/* Timer7 handler declaration */
TIM_HandleTypeDef htim7;

/* Variable to store latest reading of the temperature sensor */
uint8_t temperatureReading = 0;

/* LINKED_LIST declaration */
struct LINKED_LIST UartCommands;


/* Function prototypes */
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Main_UpdateTemperatureReading(void);




/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();
	/* STM32F7xx HAL library initialization:
	 - Configure the Flash ART accelerator on ITCM interface
	 - Configure the Systick to generate an interrupt each 1 msec
	 - Set NVIC Group Priority to 4
	 - Global MSP (MCU Support Package) initialization
	 */
	HAL_Init();

	/* Configure the system clock to 200 MHz */
	SystemClock_Config();

	BSP_Init();
	BSP_LED_Init(LED_GREEN);

	/*  Configure Interrupt mode for SD detection pin
     *  and enable and set SD detect EXTI Interrupt
     */
	BSP_SD_ITConfig();

	/* Configure the Real Time Clock */
	BSP_RTC_Init();

	/* Read RoomConfiguration */
	RoomConfiguration_ReadFromSdCard();

	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
	uint32_t uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

	/* Initialize TIM3 peripheral as follows:
	 + Period = 500 - 1
	 + Prescaler = ((SystemCoreClock/2)/10000) - 1
	 + ClockDivision = 0
	 + Counter direction = Up
	 */
	htim3.Instance = TIM3;
	htim3.Init.Period = 500 - 1;
	htim3.Init.Prescaler = uwPrescalerValue;
	htim3.Init.ClockDivision = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	if (HAL_TIM_Base_Init(&htim3) == HAL_OK) {
		/* Start the TIM3 Base generation in interrupt mode */
		HAL_TIM_Base_Start_IT(&htim3);
	}


	/* This timer is being used to generate an interrupt every 500 ms
	 * to update main window clock and temperature reading
	 */

	htim7.Instance = TIM7;
	htim7.Init.Period = 5000;
	htim7.Init.Prescaler = uwPrescalerValue;
	htim7.Init.ClockDivision = 0;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;

	if (HAL_TIM_Base_Init(&htim7) == HAL_OK) {
		/* Start the TIM7 Base generation in interrupt mode */
		HAL_TIM_Base_Start_IT(&htim7);
	}

	/* Configure UART */
	SerialCommandWriterReader_UartConfig();

	/* Init the STemWin GUI Library */
	GUI_Init();

	/* Add motion support for Window Manager */
	WM_MOTION_Enable(1);

	/* Activate the use of memory device feature */
	WM_SetCreateFlags(WM_CF_MEMDEV);

	/* Get temperature reading */
	Main_UpdateTemperatureReading();

	/* Create main Menu window */
	Menu_CreateMenuWindow();


	while (1) {
		WM_Exec();
		if(UpdateTemperature == SET) {
			Main_UpdateTemperatureReading();
			UpdateTemperature = RESET;
		}
		if(SdCardInserted == SET) {
			RoomConfiguration_ReadFromSdCard();
			Menu_StatusBarUpdateSdCardIcon();
			SdCardInserted = RESET;
		}
		if(UartRxReady == SET) {
			SerialCommandWriterReader_RxReadMessage();
			UartRxReady = RESET;
		}
		if(UartTxReady == SET) {
			SerialCommandWriterReader_ClearTxBuffer();
			UartTxReady = RESET;
		}
		if(UpdateStatusBarTime == SET) {
			/* Update date and time text in menu window */
			Menu_StatusBarUpdateDateTime();
			UpdateStatusBarTime = RESET;
		}
		if(BspBackground == SET) {
			BSP_Background();
			BspBackground = RESET;
		}
	}
}



/**
 * @brief  Period elapsed callback in non blocking mode
 * @param  htim: TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance == TIM3) {
		BspBackground = SET;
	} else if(htim->Instance == TIM7) {

		UpdateTemperature = SET;
		UpdateStatusBarTime = SET;
	}
}

/**
 * @brief  uSD card inserted or removed callback in non blocking mode.
 * @param  htim: SD_HandleTypeDef handle
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == SD_DETECT_PIN) {
		SdCardInserted = SET;
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 200000000
 *            HCLK(Hz)                       = 200000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 400
 *            PLL_P                          = 2
 *            PLL_Q                          = 8
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 6
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		while (1) {
			;
		}
	}

	/* Activate the OverDrive to reach the 216 MHz Frequency */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		while (1) {
			;
		}
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
		while (1) {
			;
		}
	}

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
	PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
		while (1) {
			;
		}
	}

}



static void CPU_CACHE_Enable(void) {
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	//SCB_EnableDCache();
}

/**
 * @brief  Update temperature reading and save it to uSD card.
 * @param  None
 * @retval None
 */
static void Main_UpdateTemperatureReading(void) {
	static  uint8_t updateTicks = -1;

	temperatureReading = DTH11_ReadTemperature();

	/* Count updateTicks up because this function is called every 500 ms from
	 * HAL_TIM_PeriodElapsedCallback() function and desired
	 * update period is 30 s
	 */
	if(updateTicks == -1 || updateTicks >= 60){
		updateTicks = 0;
		TemperatureDataLogger_WriteTemperatureValue(temperatureReading);
	} else {
		updateTicks++;
	}

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

/* End of file */
