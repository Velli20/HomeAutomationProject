#ifndef __ROOMWIDGETPARSER_H
#define __ROOMWIDGETPARSER_H

#include "ezxml.h"

struct ROOMLIST* RoomWidgetParser_ParseString(char * xmlString, size_t lenght);
void RoomWidgetParser_ParseData(ezxml_t  a_node, struct ROOMLIST *list);
struct ROOM* RoomWidgetParser_ParseRoom(ezxml_t roomNode);
struct RoomWidget* RoomWidgetParser_ParseWidget(ezxml_t widgetNode);

#endif /* __ROOMWIDGETPARSER_H */
