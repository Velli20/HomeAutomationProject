#include "main.h"

#ifndef __SERIAL_COMMAND_READER_WRITER_H
#define __SERIAL_COMMAND_READER_WRITER_H

/* Definition for USARTx clock resources */
#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_6
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF8_USART6

/* Definition for USARTx's DMA */
#define USARTx_TX_DMA_STREAM              DMA2_Stream7
#define USARTx_RX_DMA_STREAM              DMA2_Stream1
#define USARTx_TX_DMA_CHANNEL             DMA_CHANNEL_5
#define USARTx_RX_DMA_CHANNEL             DMA_CHANNEL_5


/* Definition for USARTx's NVIC */
#define USARTx_DMA_TX_IRQn                DMA2_Stream7_IRQn
#define USARTx_DMA_RX_IRQn                DMA2_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler          DMA2_Stream7_IRQHandler
#define USARTx_DMA_RX_IRQHandler          DMA2_Stream1_IRQHandler

#define USARTx_IRQn                       USART6_IRQn
#define USARTx_IRQHandler                 USART6_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      1024
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* String formatter for serial command responses */
#define TAG_RESPONSE_FORMATTER_STRING	  "<%s/>\r"
/* Response that is used when we don't recoqnize the command that client device sent */
#define TAG_RESPONSE_UNKNOWN_COMMAND	  "UnknownCommand"
/* Response that is used to inform client device that command was successfull */
#define TAG_RESPONSE_OK					  "OK"
/* Response that is used when we have internal error (e.g programming error, out of memory etc.) */
#define TAG_DEVICE_INTERNAL_ERROR		  "DeviceInternalError"



/* Function prototypes */
void SerialCommandWriterReader_UartConfig(void);
void SerialCommandWriterReader_RxReadMessage(void);
void SerialCommandWriterReader_TxWriteMessage(char * data, size_t lenght);

void SerialCommandWriterReader_ClearRxBuffer(void);
void SerialCommandWriterReader_ClearTxBuffer(void);

#endif /* __SERIAL_COMMAND_READER_WRITER_H */

/* END OF FILE */
