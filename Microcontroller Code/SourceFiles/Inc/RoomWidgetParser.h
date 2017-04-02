#ifndef __ROOMWIDGETPARSER_H
#define __ROOMWIDGETPARSER_H

/* Includes */
#include "ezxml.h"
#include "RoomWidget.h"
#include "RoomWidgetConstants.h"

#include <stdio.h>
#include <string.h>

/* Public function prototypes */
struct RoomList* RoomWidgetParser_ParseString(char * xmlString, size_t lenght);
struct RoomList* RoomWidgetParser_ParseData(const ezxml_t  a_node);
struct Room* RoomWidgetParser_ParseRoom(const ezxml_t roomNode);
struct RoomWidget* RoomWidgetParser_ParseWidget(const ezxml_t widgetNode);

#endif /* __ROOMWIDGETPARSER_H */
