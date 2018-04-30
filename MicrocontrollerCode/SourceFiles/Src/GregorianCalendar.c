// Includes

#include "GregorianCalendar.h"

static const uint8_t DAYS_IN_MONTH[]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// GregorianCalendar_GetCountOfDayInMonth

uint8_t GregorianCalendar_GetCountOfDayInMonth(int year, int month)
{
    if  ( month == FEBRUARY && IS_LEAP_YEAR(year) )
        return DAY_COUNT_FEBRUARY_LEAP_YEAR;
	  else
		    return DAYS_IN_MONTH[month];
}
