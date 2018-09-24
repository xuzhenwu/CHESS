#include <stdio.h>

#include "Functions.h"

int end_day_norm_leap(int year,int month)
{
    int end_day;
	const int daysofmonth_norm[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 
	const int daysofmonth_leap[12]={31,29,31,30,31,30,31,31,30,31,30,31}; 
	
	if(year %4==0 && year %100!=0 || year % 400==0)
		end_day=daysofmonth_leap[month];
	else
		end_day=daysofmonth_norm[month];

return (end_day);
}
