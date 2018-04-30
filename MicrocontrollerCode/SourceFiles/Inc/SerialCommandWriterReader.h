

#ifndef __SERIAL_COMMAND_READER_WRITER_H
#define __SERIAL_COMMAND_READER_WRITER_H

/* Includes */
#include "main.h"

/* String formatter for serial command responses */
#define TAG_RESPONSE_FORMATTER_STRING	 					"#<%s/>*"
/* String formatter for serial command responses */
#define TAG_RESPONSE_FORMATTER_STRING_WITH_REQUEST_CODE	  	"#<%s requestCode=\"%d\"/>*"
/* Response that is used when we don't recoqnize the command that client device sent */
#define TAG_RESPONSE_UNKNOWN_COMMAND	  					"UnknownCommand"
/* Response that is used to inform client device that command was successfull */
#define TAG_RESPONSE_OK					  					"OK"
/* Response that is used when we have internal error (e.g programming error, out of memory etc.) */
#define TAG_DEVICE_INTERNAL_ERROR		  					"DeviceInternalError"
/* Start character for serial communication */
#define SERIAL_START_FLAG									'#'
/* End character for serial communication */
#define SERIAL_END_FLAG										'*'



/* Function prototypes */
void SerialCommandWriterReader_UartConfig(void);
void SerialCommandWriterReader_RxReadMessage(void);
void SerialCommandWriterReader_TxWriteMessage(char * data, size_t lenght);

void SerialCommandWriterReader_ClearRxBuffer(void);
void SerialCommandWriterReader_ClearTxBuffer(void);

#endif /* __SERIAL_COMMAND_READER_WRITER_H */

/* END OF FILE */
