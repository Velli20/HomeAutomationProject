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

/* LINKED_LIST declared in main.c */
extern struct LINKED_LIST UartCommands;

/* Private function prototypes */
static void SerialCommandWriterReader_HandleRxData(uint8_t rxData[]);

void SerialCommandWriterReader_UartConfig(void) {
	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
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
		LinkedList_add("ERROR1", true, &UartCommands);
	}
	if (HAL_UART_Init(&UartHandle) != HAL_OK) {
		LinkedList_add("ERROR2", true, &UartCommands);
	}

	__HAL_UART_FLUSH_DRREGISTER(&UartHandle);
	/* Put UART peripheral in reception process */
	if(HAL_UART_Receive_DMA(&UartHandle, &rxBuffer, 1) != HAL_OK) {
		LinkedList_add("ERROR3", true, &UartCommands);
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
	__HAL_UART_FLUSH_DRREGISTER(&UartHandle);
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
	LinkedList_add("ERROR", true, &UartCommands);
}


/**
 * @brief  Read RX message in non blocking mode
 * @param  None
 * @retval None
 */
void SerialCommandWriterReader_RxReadMessage() {
	/* Clear the buffer to prevent overrun */
	__HAL_UART_FLUSH_DRREGISTER(&UartHandle);

	if (rxindex < RXBUFFERSIZE) {
		/* Add that character to the string */
		rxString[rxindex] = rxBuffer;
		rxindex++;
	}


	if (rxBuffer == '\r' || rxBuffer == '\n') {
		/* Handle received data */
		SerialCommandWriterReader_HandleRxData(rxString);

		/* Clear the string buffer */
		SerialCommandWriterReader_ClearRxBuffer();
	}

	if (rxindex >= (RXBUFFERSIZE)) {
		SerialCommandWriterReader_ClearRxBuffer();
	}
}

/**
 * @brief  Write a message asynchronously to client device over UART
 * @param  data: Pointer to data to write
 * @param  lenght: Lenght of data to write in bytes
 * @retval None
 */
void SerialCommandWriterReader_TxWriteMessage(char * data, size_t lenght) {
	if(!data || lenght > TXBUFFERSIZE) {
		return;
	}

	if(HAL_UART_Transmit_DMA(&UartHandle, data, lenght) != HAL_OK) {
		LinkedList_add("ERROR4", true, &UartCommands);
	}
}

/**
 * @brief  Clear Tx buffer
 * @param  None
 * @retval None
 */
void SerialCommandWriterReader_ClearTxBuffer(void) {
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
static void SerialCommandWriterReader_HandleRxData(uint8_t rxData[]) {
	if(!rxData) {
		return;
	}

	/* Create ezxml structure out of the data string */
	ezxml_t node = ezxml_parse_str(rxString, strlen(rxData));

	if (node) {
		/* Try to extract a command from the data string */
		SERIAL_COMMAND command = SerialCommandParser_ParseCommand(node);
		/* Char array where we store our response string */
		char message[20];
		if (command == SERIAL_COMMAND_UPDATE_WIDGET_STATUS) {
			/* Message starts with <Data> tag. Try to update RoomConfiguration */
			int result = RoomConfiguration_UpdateRoomState(node);

			if(result == ROOM_UPDATE_RESULT_OK) {
				/* RoomConfiguration was updated successfully */
				sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_OK);
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
				/* Assign configuration string to txBuffer, so we can free it when writing is done */
				txBuffer = configuration;
			} else {
				/* Internal error has occurred */
				sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_DEVICE_INTERNAL_ERROR);
				SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
			}
		} else if (command == SERIAL_COMMAND_UNKNOWN_COMMAND) {
			/* Command was not recognized */
			sprintf(message, TAG_RESPONSE_FORMATTER_STRING, TAG_RESPONSE_UNKNOWN_COMMAND);
			SerialCommandWriterReader_TxWriteMessage(message, strlen(message));
		}

		/* Free allocated ezxml structure */
		ezxml_free(node);
	}
}

/* End of file */
