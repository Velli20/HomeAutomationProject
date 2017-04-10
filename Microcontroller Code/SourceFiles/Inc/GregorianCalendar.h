#ifndef __GREGORIAN_CALENDAR_H
#define __GREGORIAN_CALENDAR_H

/* Includes */
#include "UI_resources.h"

/* Defines */
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

/* Public function prototypes */
int GregorianCalendar_GetCountOfDayInMonth(int year, int month);
int GregorianCalendar_IsLeapYear(int year);

#endif /* __GREGORIAN_CALENDAR_H */


/* End of file */
