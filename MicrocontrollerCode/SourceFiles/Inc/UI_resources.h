#ifndef __UI_RESOURCES_H
#define __UI_RESOURCES_H

/* Includes */
#include "BITMAPS.h"
#include "fonts.h"
#include "WM.h"

/* Defines */
#define __TRANSLATION_EN							/* Use finnish translation for UI */

/* Colors */
#define LEMON       								0x00d6d3
#define DARK_BLUE   								0x613600

/* Dimensions */
#define LIGHTS_LIST_ITEM_HEIGHT 					56  /* Swipe list item height in px declared in Window_lights.c list */
#define ROOMS_LIST_ITEM_HEIGHT 						56  /* Swipe list item height in px declared in Window_temperature.c list */
#define SETTINGS_LIST_ITEM_HEIGHT 					48  /* Swipe list item height in px declared in Window_settings.c list */
#define TEMPERATURE_SLIDER_HANDLE_WIDTH 			20  /* Target temperature slider handle width in px declared in Window_temperature.c */


/* Units */

/* Temperature units that are used across UI */
#define __USE_CELSIUS
#ifdef  __USE_CELSIUS

/* Maximum and minimum room temperature */
#define MAX_THERMOSTAT_TEMPERATURE					30
#define MIN_THERMOSTAT_TEMPERATURE					2
#define TEMPERATURE_UNIT							"°C"

#endif /* __USE_CELSIUS */

/* Hour format that is used in UI and data logging*/
#define __USE_24_HOUR_FORMAT
#ifdef  __USE_24_HOUR_FORMAT

#define HOUR_RANGE_MAX								23
#define MINUTE_RANGE_MAX							59

#define YEAR_RANGE_MIN								2017
#define YEAR_RANGE_MAX								YEAR_RANGE_MIN + 100


#endif /* __USE_24_HOUR_FORMAT */

#define WINDOW_TITLE_FONT							GUI_FONT_13B_1

#define THERMOSTAT_WINDOW_LIST_HEADER_FONT			&GUI_Font23SL_AA4
#define THERMOSTAT_WINDOW_TARGET_TEMPERATURE_FONT   &GUI_Font23SL_AA4
#define THERMOSTAT_WINDOW_CURRENT_TEMPERATURE_FONT  &GUI_Font32SL_AA4

#define LIGHTS_WINDOW_LIST_HEADER_FONT				&GUI_Font23SL_AA4
#define LIGHTS_WINDOW_LIGTH_TOGGLE_FONT				GUI_FONT_13B_1

#define MENU_WINDOW_TIME_TEXT_FONT					GUI_FONT_13B_1
#define MENU_WINDOW_DATE_TEXT_FONT					GUI_FONT_13B_1
#define MENU_WINDOW_TEMPERATURE_TEXT_FONT			GUI_FONT_13B_1
#define MENU_WINDOW_MENU_ITEM_TEXT_FONT				GUI_FONT_13B_1

#define TIME_SETTINGS_HOUR_FONT						GUI_FONT_16B_1
#define TIME_SETTINGS_MINUTE_FONT					GUI_FONT_16B_1
#define TIME_SETTINGS_SAVE_ICON_TEXT_FONT			GUI_FONT_13B_1

#define DATE_SETTINGS_DAY_FONT						GUI_FONT_16B_1
#define DATE_SETTINGS_MONTH_FONT					GUI_FONT_16B_1
#define DATE_SETTINGS_YEAR_FONT						GUI_FONT_16B_1
#define DATE_SETTINGS_SAVE_ICON_TEXT_FONT			GUI_FONT_13B_1

/* Translations */
#ifdef __TRANSLATION_FIN

/* Window_lights.c translations */
#define LIGHTS_WINDOW_TITLE							"Valaistus"
#define LIGHTS_ITEM_TEXT_LIGHTS_ON					"P��ll�"
#define LIGHTS_ITEM_TEXT_LIGHTS_OFF					"Pois p��lt�"
#define LIGHTS_NO_NAME								"(NIMET�N)" 		/* List item text if light widget name is null or empty */

/* Window_temperature.c translations */
#define THERMOSTAT_WINDOW_TITLE						"L�mp�tila"
#define THERMOSTAT_NO_NAME							"(NIMET�N)"	    /* List item text if thermostat widget name is null or empty */
#define THERMOSTAT_TARGET_TEMPERATURE   			"Asetus "
#define THERMOSTAT_MEASURED_TEMPERATURE   			"Mitattu "

/* Window_settings.c translations */
#define SETTINGS_WINDOW_TITLE 						"Asetukset"
#define SETTINGS_MENU_ITEM_DATE						"P�iv�m��r�"
#define SETTINGS_MENU_ITEM_TIME						"Aika"
#define SETTINGS_MENU_ITEM_WIFI						"WiFi"
#define SETTINGS_MENU_ITEM_BLUETOOTH				"Bluetooth"
#define SETTINGS_MENU_ITEM_ETHERNET					"Ethernet"
#define SETTINGS_MENU_ITEM_ROOM_CONFIGURATION 		"Huone konfiguraatio"

/* Window_settings_time.c translations */
#define WINDOW_TIME_SETTINGS_WINDOW_TITLE 			"Aseta aika"
#define WINDOW_DATE_SETTINGS_WINDOW_TITLE 			"Aseta p�iv�m��r�"
#define WINDOW_TIME_SETTINGS_BUTTON_SAVE_TITLE		"Tallenna"
#define WINDOW_DATE_SETTINGS_BUTTON_SAVE_TITLE		"Tallenna"

/* Window_menu.c translations */
#define WINDOW_MENU_MENU_ITEM_LIGHTS				"Valaistus"
#define WINDOW_MENU_MENU_ITEM_TMEPERATURE			"L�mp�tila"
#define WINDOW_MENU_MENU_ITEM_COMMANDS				"Komennot"
#define WINDOW_MENU_MENU_ITEM_SETTINGS				"Asetukset"

/* Window_room_configuration.c translations */
#define WINDOW_ROOM_CONFIGURATION_TITLE				"Huone konfiguraatio"
#define WINDOW_ROOM_CONFIGURATION_WIDGET_NO_NAME    "(NIMET�N)"
#define WINDOW_ROOM_CONFIGURATION_ROOM_NO_NAME      "(NIMET�N)"



#endif /* __TRANSLATION_FIN */

#ifdef __TRANSLATION_EN

/* Window_lights.c translations */
#define LIGHTS_WINDOW_TITLE							"Lights"
#define LIGHTS_ITEM_TEXT_LIGHTS_ON					"ON"
#define LIGHTS_ITEM_TEXT_LIGHTS_OFF					"OFF"
#define LIGHTS_NO_NAME								"(NO NAME)" 		/* List item text if light widget name is null or empty */

/* Window_temperature.c translations */
#define THERMOSTAT_WINDOW_TITLE						"Temperature"
#define THERMOSTAT_NO_NAME							"(NO NAME)"	    /* List item text if thermostat widget name is null or empty */
#define THERMOSTAT_TARGET_TEMPERATURE   			"Target"
#define THERMOSTAT_MEASURED_TEMPERATURE   			"Measured"

/* Window_settings.c translations */
#define SETTINGS_WINDOW_TITLE 						"Settings"
#define SETTINGS_MENU_ITEM_DATE						"Date"
#define SETTINGS_MENU_ITEM_TIME						"Time"
#define SETTINGS_MENU_ITEM_WIFI						"WiFi"
#define SETTINGS_MENU_ITEM_BLUETOOTH				"Bluetooth"
#define SETTINGS_MENU_ITEM_ETHERNET					"Ethernet"
#define SETTINGS_MENU_ITEM_ROOM_CONFIGURATION 		"Room configuration"

/* Window_settings_time.c translations */
#define WINDOW_TIME_SETTINGS_WINDOW_TITLE 			"Set time"
#define WINDOW_DATE_SETTINGS_WINDOW_TITLE 			"Set date"
#define WINDOW_TIME_SETTINGS_BUTTON_SAVE_TITLE		"Save"
#define WINDOW_DATE_SETTINGS_BUTTON_SAVE_TITLE		"Save"

/* Window_menu.c translations */
#define WINDOW_MENU_MENU_ITEM_LIGHTS				"Lights"
#define WINDOW_MENU_MENU_ITEM_TMEPERATURE			"Temperature"
#define WINDOW_MENU_MENU_ITEM_COMMANDS				"Commands"
#define WINDOW_MENU_MENU_ITEM_SETTINGS				"Settrings"

/* Window_room_configuration.c translations */
#define WINDOW_ROOM_CONFIGURATION_TITLE				"Room configuration"
#define WINDOW_ROOM_CONFIGURATION_WIDGET_NO_NAME    "(NO NAME)"
#define WINDOW_ROOM_CONFIGURATION_ROOM_NO_NAME      "(NO NAME)"



#endif /* __TRANSLATION_EN */

// window_menu_create_window

WM_HWIN window_menu_create_window(void);

// window_lights_create_window

WM_HWIN window_lights_create_window(void);

/* Declared in Window_temperature.c */
WM_HWIN Temperature_CreateTemperatureWindow(void);

/* Declared in Window_settings.c */
WM_HWIN Settings_CreateSettingsWindow(void);

/* Declared in Window_settings_time.c */
WM_HWIN SettingsTime_CreateSettingsTimeWindow(void);

/* Declared in Window_room_configuration.c */
WM_HWIN WindowRoomConfiguration_CreateWindow(void);

/* Declared in Window_settings_date.c */
WM_HWIN SettingsDate_CreateSettingsDateWindow(void);

#endif /* __UI_RESOURCES_H */

/* End of file */
