#ifndef __ROOMWIDGETWRITER_H
#define __ROOMWIDGETWRITER_H

/* Includes */
#include "ezxml.h"
#include "RoomWidget.h"
#include "RoomWidgetConstants.h"

#include <stdio.h>
#include <string.h>


/* Public function prototypes */
ezxml_t RoomWidgetWriter_WriteXML(struct RoomList * list);
ezxml_t RoomWidgetWriter_GetRoomWidgetUpdateXML(struct RoomList * list, struct RoomWidget * widget);



#endif /* __ROOMWIDGETWRITER_H */

/* End of file */
