#ifndef __ROOMWIDGETPARSER_H
#define __ROOMWIDGETPARSER_H

// Includes

#include "ezxml.h"
#include "RoomWidget.h"
#include "RoomWidgetConstants.h"

// widget_parser_parse_xml_string

struct RoomList* widget_parser_parse_xml_string(char * xmlString, size_t lenght);

// widget_parser_parse_xml_node

struct RoomList* widget_parser_parse_xml_node(const ezxml_t  a_node);

// widget_parser_parse_request_code

uint32_t widget_parser_parse_request_code(const ezxml_t roomConfiguration);

#endif
