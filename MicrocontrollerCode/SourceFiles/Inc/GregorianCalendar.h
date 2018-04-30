#ifndef __GREGORIAN_CALENDAR_H
#define __GREGORIAN_CALENDAR_H

// Includes

#include "UI_resources.h"

// Defines

#define JANUARY 	0
#define FEBRUARY	1
#define MARCH		2
#define APRIL		3
#define MAY			4
#define JUNE		5
#define JULY		6
#define AUGUST		7
#define SEPTEMBER	8
#define OCTOBER		9
#define NOVEMBER	10
#define DECEMBER	11

// Defines

#define DAY_COUNT_FEBRUARY_LEAP_YEAR 29
#define IS_LEAP_YEAR(year)(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))

// GregorianCalendar_GetCountOfDayInMonth

uint8_t GregorianCalendar_GetCountOfDayInMonth(int year, int month);

#endif 
