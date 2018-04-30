/* Includes */
#include "SerialCommandWriterReader.h"
#include "SerialCommandParser.h"
#include "RoomConfiguration.h"


/* UART handler declaration */
extern UART_HandleTypeDef UartHandle;

/* DMA handle declaration */
DMA_HandleTypeDef DmaTxHandle;
DMA_HandleTypeDef DmaRxHandle;

/* UART RX buffer where we store that one character that just came in */
uint8_t rxString[RXBUFFERSIZE];
uint8_t rxindex = 0;

extern APP_DATA* application_data;

// SerialCommandWriterReader_HandleRxData

static void SerialCommandWriterReader_HandleRxData(char* rxDataBuffer, 
                                                   int   bufferLenght) 
{
	ezxml_t        node;
	char*          message;
	char*          configuration;
	SERIAL_COMMAND command;
	uint8_t        result;
	uint32_t       requestCode;
	
	if ( !rxDataBuffer || bufferLenght == 0 ) 
		return;
	
	// Char array where we store our response string.

	message= calloc(50, sizeof(char));
	if ( !message )
		return;

	// Create ezxml structure out of the data string.

	node= ezxml_parse_str(rxDataBuffer, bufferLenght);
	if ( !node )
	{
		// Command was not recognized.

		sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
		SerialCommandWriterReader_TxWriteMessage(message, strlen(message));

		free(message);
		return;
	}

	// Try to extract a command from the data string.

	command= SerialCommandParser_ParseCommand(node);

	// Message starts with <RoomConfigurationUpdate> tag. Try to update RoomConfiguration.

	if ( command == SERIAL_COMMAND_UPDATE_WIDGET_STATUS ) 
	{

		result=      room_configuration_update_widgets(application_data, node);
		requestCode= widget_parser_parse_request_code(node);

		// RoomConfiguration was updated successfully.

		if ( result == ROOM_UPDATE_RESULT_OK ) 
		{
			if ( requestCode != -1 ) 
				sprintf(message, TAG_RESPONSE_FORMATTER_STRING_WITH_REQUEST_CODE, TAG_RESPONSE_OK, requestCode);
			else 
				sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_OK);

		} 

		// Failed to update RoomConfiguration.

		else 
		{
			sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
		}

		// Write response.

		SerialCommandWriterReader_TxWriteMessage(message, strlen(message));

	} 

	// Client device is requesting RoomConfiguration.

	else if ( command == SERIAL_COMMAND_GET_ROOM_COFIGURATION ) 
	{
		// Get RoomConfiguration in XML format String.
		
		configuration= room_configuration_to_xml(application_data);
		if ( configuration ) 
		{
			//SerialCommandWriterReader_TxWriteMessage(configuration, strlen(configuration));
			application_data->tx_buffer= configuration;
		} 

		// Internal error has occurred.

		else 
		{
			sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_DEVICE_INTERNAL_ERROR);
			SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
		}
	}

	// Command was not recognized.

	else 
	{
		sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
		SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
	}

	// Free allocated ezxml structure.

	ezxml_free(node);
	
	application_data->tx_buffer= message;

	free(rxDataBuffer);
}

// HAL_UART_ErrorCallback

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) 
{
	Log("HAL_UART_ErrorCallback()");
}

// SerialCommandWriterReader_RxReadMessage

void SerialCommandWriterReader_RxReadMessage(void) 
{
	
	// Clear the buffer to prevent overrun.

	//__HAL_UART_FLUSH_DRREGISTER(&UartHandle);

	// Start of the incoming data. Make sure buffer is clear.

	if ( application_data->rx_buffer == SERIAL_START_FLAG ) 
		goto clearBuffer;

	// End of data string. Try to process data.

	else if ( application_data->rx_buffer == SERIAL_END_FLAG ) 
		goto read;

	if (rxindex < RXBUFFERSIZE
			&& application_data->rx_buffer != '\0'
			&& application_data->rx_buffer != '\n'
			&& application_data->rx_buffer != '\r'
			&& application_data->rx_buffer != '\t') 
	{
		// Add incoming character to the string buffer.

		rxString[rxindex]= (uint8_t)application_data->rx_buffer;
		rxindex++;
	} 
	else 
	{
		// Data is lost or has reached limit of the rx buffer.
		goto read;
	}

	return;

	read:

	printf("Read buffer=%s\n", rxString);

	if (rxindex > 1) {
		char * rxDataBuffer= calloc(rxindex, sizeof(char));
		if (rxDataBuffer) {
			strncpy(rxDataBuffer, (const char*)rxString, rxindex);

			/* Handle received data */
			SerialCommandWriterReader_HandleRxData(rxDataBuffer, rxindex);
		}
	}
	clearBuffer:
	
	/* Clear the string buffer */
	SerialCommandWriterReader_ClearRxBuffer();

}

/**
 * @brief  Write a message asynchronously to client device over UART
 * @param  data: Pointer to data to write
 * @param  lenght: Lenght of data to write in bytes
 * @retval None
 */
void SerialCommandWriterReader_TxWriteMessage(char * data, size_t lenght) {
	if ( !data ) {
		return;
	}

	while ( HAL_UART_GetState(&UartHandle) == HAL_UART_STATE_BUSY_TX ) {
		; /* TODO: */
	}



	if ( HAL_UART_Transmit_IT(&UartHandle, data, lenght) != HAL_OK ) {
		/* Handle error */
	}


}

/**
 * @brief  Clear Tx buffer
 * @param  None
 * @retval None
 */
void SerialCommandWriterReader_ClearTxBuffer(void) {
	/* Clear the buffer to prevent overrun */
	__HAL_UART_FLUSH_DRREGISTER(&UartHandle);
	
	if ( application_data->tx_buffer ) {
		free(application_data->tx_buffer);
		application_data->tx_buffer= 0;
	}
}

/**
 * @brief  Clear Rx buffer
 * @param  None
 * @retval None
 */
void SerialCommandWriterReader_ClearRxBuffer(void) {

	printf("Clear buffer=%s\n", rxString);

	rxindex = 0;
	for (int i = 0; i < RXBUFFERSIZE; i++) {
		rxString[i] = 0; // Clear the string buffer
	}
}


/* End of file */
