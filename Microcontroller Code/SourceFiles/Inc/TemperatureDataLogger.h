#ifndef __TEMPERATURE_DATA_LOGGER_H
#define __TEMPERATURE_DATA_LOGGER_H

/* Includes */
#include <stdio.h>
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "BSP.h"

/* String formatter for temperature data tag*/
#define TEMPERATURE_DATA_TAG_FORMATTER_STRING 			"<Temp date=\"%02d.%02d.%d\" time=\"%02d.%02d.%02d\">%d</Temp>\n"
/* String formatter for temperature data file name */
#define TEMPERATURE_DATA_FILE_NAME_FORMATTER_STRING  	"%d_%d_%d.XML"

typedef enum {
	TEMPERATURE_DATA_LOGGER_RESULT_NO_SD,
	TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_LINK_SD_DRIVER,
	TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_MOUNT_SD_CARD,
	TEMPERATURE_DATA_LOGGER_RESULT_UNABLE_TO_CREATE_OR_OPEN_FILE,
	TEMPERATURE_DATA_LOGGER_RESULT_WRITE_OPERATION_FAILED
	
	
} TEMPERATURE_DATA_LOGGER_RESULT;


/* Function prototypes */
TEMPERATURE_DATA_LOGGER_RESULT TemperatureDataLogger_WriteTemperatureValue(uint8_t temperature);

#endif /* __TEMPERATURE_DATA_LOGGER_H */