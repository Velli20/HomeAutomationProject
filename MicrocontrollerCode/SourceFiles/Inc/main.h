#ifndef __MAIN_H
#define __MAIN_H

// Includes

#include "stm32f7xx_hal.h"
#include "GUI.h"
#include "WM.h"
#include "ezxml.h"
#include "DTH11.h"
#include "ff.h"
#include "log.h"
#include "RoomWidget.h"

#include <stdio.h>
#include <string.h>

#include "stm32746g_discovery.h"
#include "stm32746g_discovery_sdram.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_eeprom.h"
#include "stm32746g_discovery_sd.h"

#define EVENT_ROOM_CONFIGURATION_DELETED      0x0
#define EVENT_ROOM_CONFIGURATION_CREATED      0x1
#define EVENT_ROOM_CONFIGURATION_UPDATED      0x2
#define EVENT_ROOM_WIDGET_UPDATED_FROM_REMOTE 0x3
#define EVENT_ROOM_WIDGET_UPDATED_LOCALLY     0x4
#define EVENT_REAL_TIME_CLOCK_TICK            0x5
#define EVENT_SD_CARD_DETECTED                0x6
#define EVENT_TEMPERATURE_UPDATED             0x7

#define RESULT_OK    0
#define RESULT_ERROR 1

#define MAX_EVENT_CALLBACK_COUNT 10

typedef struct application_data APP_DATA;

typedef void (*EventCallback)(APP_DATA* data, struct RoomWidget* widget, uint8_t event);

// application_data

typedef struct application_data
{
	WM_HWIN*           active_window;

	struct RoomList*   room_configuration;
	EventCallback      event_callback[MAX_EVENT_CALLBACK_COUNT];

	uint8_t            event_callback_count;
	uint8_t            selected_light_widget;
	uint8_t            selected_light_widget_id;

	uint8_t            rx_buffer;
	char*              tx_buffer;

	GUI_PID_STATE      previous_touch;
}
APP_DATA;

// ErrorHandler

#define ErrorHandler() \
{                      \
	while ( 1 )        \
	{                  \
                       \
	}                  \
}                      \

// temperature_data_logger_log_temperature

uint8_t temperature_data_logger_log_temperature(APP_DATA* app_data, 
                                                uint8_t   temperature);

// event_handler_send_event

uint8_t event_handler_send_event(APP_DATA*          data,
                               struct RoomWidget* widget,
                               uint8_t            event);

// event_handler_register_callback

void event_handler_register_callback(APP_DATA*     data,
		                             EventCallback c);

// event_handler_unregister_callback

void event_handler_unregister_callback(APP_DATA*     data,
		                               EventCallback c);

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      400
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
/* Transmission timeout */
#define TX_TIMEOUT						  1000

#endif 
