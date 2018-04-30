// Includes.

#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "main.h"
#include "real_time_clock.h"

// String formatter for temperature data tag.

#define TEMPERATURE_DATA_TAG_FORMATTER_STRING 			"<Temp date=\"%02d.%02d.%d\" time=\"%02d.%02d.%02d\">%d</Temp>\n"

// String formatter for temperature data file name.

#define TEMPERATURE_DATA_FILE_NAME_FORMATTER_STRING  	"%d_%d_%d.XML"

#define FILE_NAME_MAX_LENGTH 20
#define XML_ELEM_MAX_LENGTH  50

// temperature_data_logger_log_temperature

uint8_t temperature_data_logger_log_temperature(APP_DATA* app_data, 
                                                uint8_t   temperature) 
{
	FATFS           sd_fat_fs;
	FIL             temperature_data_file;
	RTC_TimeTypeDef stimestructureget;
	RTC_DateTypeDef sdatestructureget;
	UINT            byteswritten;               				
	char            fileName[FILE_NAME_MAX_LENGTH]; 							
	char            dataStringTemp[XML_ELEM_MAX_LENGTH];
	char            sd_path[4];
	uint8_t         result;

	if ( !app_data )
	    return(RESULT_ERROR);

	// Check that SD card is inserted.

	if ( BSP_SD_IsDetected() != SD_PRESENT ) 
		return(RESULT_ERROR);                   

	result= RESULT_ERROR;

	// Link the micro SD disk I/O driver.

	memset(sd_path, 0, sizeof(sd_path));
	if ( FATFS_LinkDriver(&SD_Driver, sd_path) != FR_OK ) 
		return(RESULT_ERROR);

	// Register the file system object to the FatFs module.

	if ( f_mount(&sd_fat_fs, (const TCHAR*)sd_path, 0) != FR_OK )
		goto end;

	// Create the filename string.

	memset(fileName, 0, sizeof(fileName));
	snprintf(fileName, FILE_NAME_MAX_LENGTH, TEMPERATURE_DATA_FILE_NAME_FORMATTER_STRING, sdatestructureget.Date, sdatestructureget.Month, (sdatestructureget.Year));

	// Open temperature log file or reate new xml file object with write access.

	if ( f_open(&temperature_data_file, (const TCHAR*)fileName, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK ) 
		goto end;
	
	// Seek to end of the file to append data.

	if ( f_lseek(&temperature_data_file, f_size(&temperature_data_file)) != FR_OK ) 
		goto end;
	
	// Get current RTC date and time.

	stimestructureget= BSP_RTC_GetTime();
	sdatestructureget= BSP_RTC_GetDate();

	// Create the data string containing current date, time and temperature.

	memset(dataStringTemp, 0, sizeof(dataStringTemp));
	snprintf(dataStringTemp, XML_ELEM_MAX_LENGTH,
			TEMPERATURE_DATA_TAG_FORMATTER_STRING,
			sdatestructureget.Date, sdatestructureget.Month,
			(sdatestructureget.Year + 2000), stimestructureget.Hours,
			stimestructureget.Minutes, stimestructureget.Seconds, temperature);

	// Append data at the end of the file.

	if ( f_write(&temperature_data_file, dataStringTemp, strnlen(dataStringTemp, XML_ELEM_MAX_LENGTH), &byteswritten) != FR_OK )
		result= RESULT_OK;

	end:

	f_close(&temperature_data_file);

	FATFS_UnLinkDriver(sd_path);

	return(result);
}
