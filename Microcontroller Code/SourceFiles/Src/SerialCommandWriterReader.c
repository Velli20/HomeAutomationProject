/* Includes */
#include "SerialCommandWriterReader.h"
#include "SerialCommandParser.h"

/* Variables */
__IO ITStatus UartRxReady = RESET;
__IO ITStatus UartTxReady = RESET;

/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* DMA handle declaration */
DMA_HandleTypeDef DmaTxHandle;
DMA_HandleTypeDef DmaRxHandle;

/* UART RX buffer where we store that one character that just came in */
uint8_t rxBuffer = '\000';
uint8_t rxString[RXBUFFERSIZE];
uint8_t rxindex = 0;

char * txBuffer;


/* Private function prototypes */
static void SerialCommandWriterReader_HandleRxData(char * rxDataBuffer, int bufferLenght);

void SerialCommandWriterReader_UartConfig(void) {
	/* Configure the UART peripheral
	 * Put the USART peripheral in the Asynchronous mode (UART Mode)
	 * UART configured as follows:
	 - Word Length = 8 Bits
	 - Stop Bit = One Stop bit
	 - Parity = None
	 - BaudRate = 9600 baud
	 - Hardware flow control disabled (RTS and CTS signals) */
	UartHandle.Instance = USARTx;

	UartHandle.Init.BaudRate = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_DeInit(&UartHandle) != HAL_OK) {
		/* Handle error */
	}
	if (HAL_UART_Init(&UartHandle) != HAL_OK) {
		/* Handle error */
	}
	/* Put UART peripheral in reception process */
	if(HAL_UART_Receive_DMA(&UartHandle, &rxBuffer, 1) != HAL_OK) {
		/* Handle error */
	}
}



/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUart) {
	/* Set transmission flag: trasfer complete*/
	UartTxReady = SET;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUart) {
	/* Set transmission flag: trasfer complete */
	UartRxReady = SET;
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
	/* Handle error */
}

/**
 * @brief  Read RX message in non blocking mode
 * @param  None
 * @retval None
 */
void SerialCommandWriterReader_RxReadMessage() {
	/* Clear the buffer to prevent overrun */
	__HAL_UART_FLUSH_DRREGISTER(&UartHandle);

	if (rxBuffer == SERIAL_START_FLAG) {
		/* Start of the incoming data. Make sure buffer is clear */
		goto clearBuffer;
	} else if(rxBuffer == SERIAL_END_FLAG) {
		/* End of data string. Try to process data */
		goto read;
	}

	if (rxindex < RXBUFFERSIZE
			&& rxBuffer != '\0'
			&& rxBuffer != '\n'
			&& rxBuffer != '\r'
			&& rxBuffer != '\t') {
		/* Add incoming character to the string buffer */

		rxString[rxindex] = rxBuffer;
		rxindex++;
	} else {
		/* Data is lost or has reached limit of the rx buffer */
		goto read;
	}


	return;

	read:

	if (rxindex > 1) {
		char * rxDataBuffer = calloc(rxindex, sizeof(char));
		if (rxDataBuffer) {
			strncpy(rxDataBuffer, rxString, rxindex);

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
	if(!data) {
		return;
	}

	while(HAL_UART_GetState(&UartHandle) == HAL_USART_STATE_BUSY_TX) {
		; /* TODO: */
	}



	if (HAL_UART_Transmit_IT(&UartHandle, data, lenght) != HAL_OK) {
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
	if(txBuffer) {
		free(txBuffer);
		txBuffer = NULL;
	}
}

/**
 * @brief  Clear Rx buffer
 * @param  None
 * @retval None
 */
void SerialCommandWriterReader_ClearRxBuffer(void) {
	rxindex = 0;
	for (int i = 0; i < RXBUFFERSIZE; i++) {
		rxString[i] = 0; // Clear the string buffer
	}
}

/**
 * @brief  Handle serial incoming data.
 * @param  rxData: Pointer to the incoming serial data
 * @retval None
 */
static void SerialCommandWriterReader_HandleRxData(char * rxDataBuffer, int bufferLenght) {
	if(!rxDataBuffer || bufferLenght == 0) {
		return;
	}

	/* Create ezxml structure out of the data string */
	ezxml_t node = ezxml_parse_str(rxDataBuffer, bufferLenght);

	/* Char array where we store our response string */
	char * message = calloc(50, sizeof(char));
	if (node) {
		/* Try to extract a command from the data string */
		SERIAL_COMMAND command = SerialCommandParser_ParseCommand(node);

		if (command == SERIAL_COMMAND_UPDATE_WIDGET_STATUS) {
			/* Message starts with <RoomConfigurationUpdate> tag. Try to update RoomConfiguration */
			int result = RoomConfiguration_UpdateRoomState(node);
			int requestCode = RoomWidgetParser_ParseRequestCode(node);

			if(result == ROOM_UPDATE_RESULT_OK) {
				/* RoomConfiguration was updated successfully */
				if(requestCode != -1) {
					sprintf(message, TAG_RESPONSE_FORMATTER_STRING_WITH_REQUEST_CODE, TAG_RESPONSE_OK, requestCode);
				} else {
					sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_OK);
				}

			} else {
				/* Failed to update RoomConfiguration */
				sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
			}
			/* Write response */
			SerialCommandWriterReader_TxWriteMessage(message, strlen(message));

		} else if (command == SERIAL_COMMAND_GET_ROOM_COFIGURATION) {
			/* Client device is requesting RoomConfiguration */
			/* Get RoomConfiguration in XML format String */
			char * configuration = RoomConfiguration_GetRoomConfiguartionInXmlFormat();
			if(configuration) {
				SerialCommandWriterReader_TxWriteMessage(configuration, strlen(configuration));
				txBuffer = configuration;
			} else {
				/* Internal error has occurred */
				sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_DEVICE_INTERNAL_ERROR);
				SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
			}
		} else {
			/* Command was not recognized */
			sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
			SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
		}

		/* Free allocated ezxml structure */
		ezxml_free(node);
	} else {
		/* Command was not recognized */
		sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
		SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
	}
	txBuffer = message;
	free(rxDataBuffer);
}

/* End of file */
