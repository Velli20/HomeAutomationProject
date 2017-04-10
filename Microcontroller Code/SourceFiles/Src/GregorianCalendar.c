/* Includes */
#include "GregorianCalendar.h"

/* Defines */
#define DAY_COUNT_FEBRUARY_LEAP_YEAR 29

/* Private variables */
static int DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/* Public code */

/**
  * @brief Returns count of days in month
  * @param month
  * @param year
  * @retval Count of days in month
  */
int GregorianCalendar_GetCountOfDayInMonth(int year, int month) {
	if(month == FEBRUARY && GregorianCalendar_IsLeapYear(year)) {
		return DAY_COUNT_FEBRUARY_LEAP_YEAR;
	} else {
		return DAYS_IN_MONTH[month];
	}
}

/**
  * @brief Determinate if input year is laep year
  * @param year
  * @retval 1 if is leap year otherwise 0
  */
int GregorianCalendar_IsLeapYear(int year) {
	if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
		return 1;
	} else {
		return 0;
	}
}

/* End of file */

