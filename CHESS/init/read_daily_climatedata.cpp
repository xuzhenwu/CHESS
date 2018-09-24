/*--------------------------------------------------------------*/
/* 																*/
/*					construct_daily_clim	 					*/
/*																*/
/*	construct_daily_clim.c - makes a daily clim object			*/
/*																*/
/*	NAME														*/
/*	construct_daily_clim.c - makes a daily clim object			*/
/*																*/
/*	SYNOPSIS													*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	Allocates a climate object.		  							*/
/*	Computes the number of entries based on start and end dates.*/
/*	Allocates an array of critical clim data for n entries.		*/
/*																*/
/*  A clim object is simply a list of doubleing point values     */
/*  however, by checking the index in the file containgin the   */
/*  list, the code insures that the list matches the number of  */
/*  dayss in the simulation and extends from start to end 		*/
/*	julian day.													*/
/*                                                              */
/*  PROGRAMMER NOTES                                            */
/*                                                              */
/*  A clim sequence file should be named with a prefix          */
/*  matching that in its base station file.  It will be         */
/*  ignored if its suffix is not in the list of parameters      */
/*  required at that time step.                                 */
/*                                                              */
/*  The clim_sequence is constructed by calling construct_      */
/*  clim_sequence.  Refer to that code for documentation of     */
/*  what a valid clim sequence is.                              */
/*                                                              */
/*  Feb 28/95 - Richard Fernandes				*/
/*	Took out warnings that optional clim squences were not	*/
/*	found (this should error fatally instead).		*/
/*	It will now do so in the contruct_clim_sequence code	*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <string.h>
#include "CHESS.h"
#include "Functions.h"

void read_daily_climate(struct input_Clim_Files inClimFiles,struct Input_Data_Range InRange,
	double DailyRain[][12][31],double DailyTmax[][12][31],double DailyTmin[][12][31],double AnnCO2[])
{
	int   yr=0,mo=0,dy=0;
	int  year,month,day;
	static int annual_order=0;
	static double CO2_PPM;
	struct date current_date;

	for (year=InRange.start_year; year<=InRange.end_year; year++){
		current_date.year=year;

		for(month=InRange.start_month; month<=InRange.end_month; month++){
			current_date.month=month;

			//determine if the year is leap year and thus the end day of February can be different
			InRange.end_day=end_day_norm_leap(year,month-1);

			for(day=InRange.start_day; day<=InRange.end_day; day++){ 
				/*--------------------------------------------------------------*/
				/*	reading daily climate data             .					*/
				/*--------------------------------------------------------------*/
				current_date.day=day;
				fscanf(inClimFiles.pPrec,"%d %d %d %lf ",&yr,&mo,&dy,&DailyRain[year-InRange.start_year][month-InRange.start_month][day-InRange.start_day]);
				//printf("year month day is %d %d %d \n",yr,mo,dy);

				if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
					printf("precipitation data don't match \n");
					printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
					printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
					exit (0);
				}

				//reading maximum temperature data
				fscanf(inClimFiles.pTmax,"%d %d %d %lf ",&yr,&mo,&dy,&DailyTmax[year-InRange.start_year][month-InRange.start_month][day-InRange.start_day]);
				if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
					printf("Maximum temperature data don't match \n");
					printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
					printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
					exit (0);
				}
				//reading minimum temperature data
				fscanf(inClimFiles.pTmin,"%d %d %d %lf ",&yr,&mo,&dy,&DailyTmin[year-InRange.start_year][month-InRange.start_month][day-InRange.start_day]);
				if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
					printf("minimum temperature data don't match \n");
					printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
					printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
					exit (0);
				}

			}
		}
	}
	fclose(inClimFiles.pPrec);
	fclose(inClimFiles.pTmin);
	fclose(inClimFiles.pTmax);

	for (year=InRange.start_year; year<=InRange.end_year; year++){
		current_date.year=year;
		//reading annual atmospheric CO2 data
		fscanf(inClimFiles.pCO2,"%d %lf ",&yr,&AnnCO2[year-InRange.start_year]);
		//printf("co2 is %f \n",AnnCO2[year-InRange.start_year]);
		//getchar();

		if(current_date.year !=yr ){
			printf("CO2 data do not match \n");
			printf("year %d \n",current_date.year);
			printf("yr,mo,dy %d \n",yr);
			exit (0);
		}
	}
	fclose(inClimFiles.pCO2);
} /*end construct_daily_clim*/
