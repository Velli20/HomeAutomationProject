/* Includes */
#include "TemperatureDataLogger.h"


/* Private variables */
FATFS SDFatFs;                  /* File system object for SD card logical drive */
FIL TemperatureDataFile;        /* File object */
char SDPath[4];                 /* SD card logical drive path */




/* Public code */


/**
 * @brief  Write temperature value to uSD card
 * @param  temperature: Temperature reading to save to the uSD card
 * @retval res: FatFs function common result code. FR_OK if succeeded
 */
TEMPERATURE_DATA_LOGGER_RESULT TemperatureDataLogger_WriteTemperatureValue(uint8_t temperature) {
	if (BSP_SD_IsDetected() != SD_PRESENT) {
		return TEMPERATURE_DATA_LOGGER_RESULT_NO_SD;
	}

	TEMPERATURE_DATA_LOGGER_RESULT result;				/* Result code */
	UINT byteswritten;               					/* File write count */
	char fileName[20]; 								/* Temporary filename string */
	char dataStringTemp[50];

	/* Get current RTC date and time */
	RTC_TimeTypeDef stimestructureget = BSP_RTC_GetTime();
	RTC_DateTypeDef sdatestructureget = BSP_RTC_GetDate();

	/* Create the filename string */
	sprintf(fileName, TEMPERATURE_DATA_FILE_NAME_FORMATTER_STRING, sdatestructureget.Date, sdatestructureget.Month, (sdatestructureget.Year));



	/* Link the micro SD disk I/O driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) != 0) {
		/* Failed to link SD driver */
		return TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_LINK_SD_DRIVER;
	}

	/* Register the file system object to the FatFs module */
	if (f_mount(&SDFatFs, SDPath, 0) != FR_OK) {
		/* SD mounting failed */
		result = TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_MOUNT_SD_CARD;
		goto end;
	}

	/* Open a file if exists, if not the create new xml file object with write access */
	if(f_open(&TemperatureDataFile, fileName, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK) {
		result = TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_CREATE_OR_OPEN_FILE;
		goto end;
	}

	/* Seek to end of the file to append data */
	if (f_lseek(&TemperatureDataFile, f_size(&TemperatureDataFile)) != FR_OK) {
		result = TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_CREATE_OR_OPEN_FILE;
		goto end;
	}


	/* Append data at the end of the file */
	/* Create the data string containing current date, time and temperature */
	sprintf(dataStringTemp,
			TEMPERATURE_DATA_TAG_FORMATTER_STRING,
			sdatestructureget.Date, sdatestructureget.Month,
			(sdatestructureget.Year + 2000), stimestructureget.Hours,
			stimestructureget.Minutes, stimestructureget.Seconds, temperature);

	if(f_write(&TemperatureDataFile, dataStringTemp, strlen(dataStringTemp), &byteswritten) != FR_OK) {
		/* Data logging failed */
		result = TEMPERATURE_DATA_LOGGER_RESULT_WRITE_OPERATION_FAILED;
	}




	end:

	f_close(&TemperatureDataFile);
	FATFS_UnLinkDriver(SDPath);

	return result;
}

/* END OF FILE */
