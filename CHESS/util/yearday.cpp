/*--------------------------------------------------------------*/
/*	yearday - computes yearday given date		*/
/*																*/
/*																*/
/*	PROGRAMMERS NOTES											*/
/*   assumes that leap years do not exist (so 1 day possible error)																*/
/*--------------------------------------------------------------*/
#include	<math.h>

#include	"CHESS.h"

long 	yearday(struct date current_date)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/
	
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	long day=0,DaysOfYear=0;
	long cummulative_day_by_normal_month[12]
		= {0,31,59,90,120,151,181,212,243,273,304,334};
	long cummulative_day_by_leap_month[12]
		= { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

	// determine the number of days in a year
	if ((current_date.year % 4 == 0 && current_date.year % 100 != 0) || current_date.year % 400 == 0){
		day = cummulative_day_by_leap_month[current_date.month-1]
			+ current_date.day;
	}
	else{
		day = cummulative_day_by_normal_month[current_date.month - 1]
			+ current_date.day;
	}

	//day = cummulative_day_by_month[calender_date.month-1]
	//	+ calender_date.day;
	return(day);
}/*end yearday.c*/
