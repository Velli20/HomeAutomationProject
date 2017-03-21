#ifndef __MAIN_H
#define __MAIN_H

/* Includes */
#include "stm32f7xx_hal.h"
#include "GUI.h"
#include "BSP.h"
#include "WM.h"
#include "LinkedList.h"
#include "ezxml.h"
#include "RoomConfiguration.h"

#include <stdio.h>
#include <string.h>

/* EVAL includes component */
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_eeprom.h"
#include "stm32746g_discovery_sd.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"


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

#define USARTx_IRQn                      USART6_IRQn
#define USARTx_IRQHandler                USART6_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      1024
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

typedef enum {
	UART_ENABLED, UART_DISABLED, UART_NEW_COMMAND
} UART_CALLBACK_EVENT;

typedef void (*uart_cb_t)(UART_CALLBACK_EVENT event, char *newCommand);
typedef void (*callbackUpdateClock)(void);

/* Exported functions */
void Main_register_clockUpdateCallback(callbackUpdateClock callback);
void UART_register_commandReceivedCallback(uart_cb_t cb);



/* Defines related to Clock configuration */

#define RTC_CLOCK_SOURCE_LSE
/*#define RTC_CLOCK_SOURCE_LSI*/


#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV    0x7F
#define RTC_SYNCH_PREDIV     0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00FF
#endif

#endif /* __MAIN_H */

/* END OF FILE */
