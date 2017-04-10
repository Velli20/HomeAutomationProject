#ifndef __BITMAPS_H
#define __BITMAPS_H

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/* Structure for bitmap items */
typedef struct {
	const GUI_BITMAP * pBitmap;
	const GUI_BITMAP * pBitmapSelected;
	const char * pText;
} BITMAP_ITEM;

extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_console;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_console_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_lights;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_lights_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_lights_off;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_lights_on;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_settings;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_settings_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_temperature;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_temperature_selected;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_sd_card;
extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_save;


extern GUI_CONST_STORAGE GUI_BITMAP bmBITMAP_return;

#endif /* __BITMAPS_H */

/* End of file */
