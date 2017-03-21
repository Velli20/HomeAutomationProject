/* Includes */
#include "main.h"
/* Private variables */
FATFS SDFatFs;                  /* File system object for SD card logical drive */
FIL TemperatureDataFile;        /* File object */
char SDPath[4];                 /* SD card logical drive path */

/* LINKED_LIST declared in main.c */
extern struct LINKED_LIST UartCommands;

/* Function prototypes */
FRESULT TemperatureDataLogger_WriteTemperatureValue(uint8_t temperature);


/* Public code */


/**
 * @brief  Write temperature value to uSD card
 * @param  temperature: Temperature reading to save to the uSD card
 * @retval res: FatFs function common result code. FR_OK if succeeded
 */
FRESULT TemperatureDataLogger_WriteTemperatureValue(uint8_t temperature) {
	if (BSP_SD_IsDetected() != SD_PRESENT) {
		LinkedList_add("Log temperature Error: no uSD card inserted", true, &UartCommands);
		return FR_NOT_READY;
	}

	FRESULT res;                                    	/* FatFs function common result code */
	UINT byteswritten;               					/* File write count */
	char * fileNameTemp = malloc(sizeof(char) * 20); 		/* Temporary filename string */
	char * dataStringTemp = malloc(sizeof(char) * 50);

	/* Get current RTC date and time */
	RTC_TimeTypeDef stimestructureget = BSP_RTC_GetTime();
	RTC_DateTypeDef sdatestructureget = BSP_RTC_GetDate();

	if(fileNameTemp == NULL || dataStringTemp == NULL) {
		/* Memory allocation has been failed, return error */
		return FR_INT_ERR;
	}

	/* Create the filename string */
	sprintf(fileNameTemp, "%d_%d_%d.xml", sdatestructureget.Date, sdatestructureget.Month, (sdatestructureget.Year));

	const TCHAR * fileName = fileNameTemp;

	/* Link the micro SD disk I/O driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
		/* Register the file system object to the FatFs module */
		res = f_mount(&SDFatFs, SDPath, 0);
		if (res != FR_OK) {
			LinkedList_add("ERROR SD 1", true, &UartCommands);
			goto end;
		}

		/* Open a file if exists, if not the create new xml file object with write access */
		res = f_open(&TemperatureDataFile, fileName, FA_OPEN_ALWAYS | FA_WRITE);

		if (res == FR_OK) {
			/* Seek to end of the file to append data */
			res = f_lseek(&TemperatureDataFile, f_size(&TemperatureDataFile));
			if(res != FR_OK) {
				res = FR_NO_FILE;
				goto end;
			}
		}


		/* Append data at the end of the file */
		/* Create the data string containing current date, time and temperature */
		sprintf(dataStringTemp, "<Temp date=\"%02d.%02d.%d\" time=\"%02d.%02d.%02d\">%d</Temp>\n",
					sdatestructureget.Date,
					sdatestructureget.Month,
					(sdatestructureget.Year + 2000),
					stimestructureget.Hours,
					stimestructureget.Minutes,
					stimestructureget.Seconds, temperature);

		const char * dataString = dataStringTemp;

		f_write(&TemperatureDataFile, dataString, strlen(dataString), &byteswritten);

		if (res != FR_OK) {
			LinkedList_add("Log temperature Error: Failed to write on uSD", true, &UartCommands);
		}

	}

	end:

	f_close(&TemperatureDataFile);
	FATFS_UnLinkDriver(SDPath);

	free(fileNameTemp);
	free(dataStringTemp);

	return res;
}

/* END OF FILE */
