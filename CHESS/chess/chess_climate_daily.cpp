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

void chess_climate_daily(struct input_Clim_Files inClimFiles,
	struct	date	current_date,
	struct  daily_clim_object *daily_clim,
	int start_year,
	int start_month,
	int start_day, int *CO2_flag
)
{
	int   yr=0,mo=0,dy=0;

	static int annual_order=0;
	static double CO2_PPM;
	/*--------------------------------------------------------------*/
	/*	reading daily climate data             .					*/
	/*--------------------------------------------------------------*/
	fscanf(inClimFiles.pPrec,"%d %d %d %lf ",&yr,&mo,&dy,&daily_clim->rain);
	if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
		printf("precipitation data don't match \n");
		printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
		printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
		exit (0);
	}

	//reading maximum temperature data
	fscanf(inClimFiles.pTmax,"%d %d %d %lf ",&yr,&mo,&dy,&daily_clim->tmax);
	if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
		printf("Maximum temperature data don't match \n");
		printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
		printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
		exit (0);
	}
    //reading minimum temperature data
	fscanf(inClimFiles.pTmin,"%d %d %d %lf ",&yr,&mo,&dy,&daily_clim->tmin);
	if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
		printf("minimum temperature data don't match \n");
		printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
		printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
		exit (0);
	}

    if(yr == current_date.year && *CO2_flag==1){
		//reading annual atmospheric CO2 data
		fscanf(inClimFiles.pCO2,"%d %lf ",&yr,&(daily_clim->CO2));
		*CO2_flag=0;
		if(current_date.year !=yr ){
			printf("CO2 data do not match \n");
			printf("year %d \n",current_date.year);
			printf("yr,mo,dy %d \n",yr);
			exit (0);
		}
		//printf("co2 is %d %d %12.8f \n",yr,current_date.year,CO2_PPM);
	}

	/*--------------------------------------------------------------*/
	/*	initialize the rest of the clim sequences as null	*/
	/*--------------------------------------------------------------*/
	daily_clim->snow = -999.;
	daily_clim->daytime_rain_duration = -999.;
	daily_clim->cloud_fraction = -999.;
	daily_clim->cloud_opacity = -999.;
	daily_clim->Delta_T = -999.;
	daily_clim->Kdown_direct = -999.;
	daily_clim->Kdown_diffuse = -999.;
	daily_clim->PAR_diffuse = -999.;
	daily_clim->PAR_direct = -999.;
	daily_clim->atm_trans = -999.;
	daily_clim->tsoil = -999.;
	daily_clim->wind = -999.;
	daily_clim->ndep_NO3 = -999.;
	daily_clim->ndep_NH4 = -999.;
	daily_clim->vpd = -999.;
	daily_clim->relative_humidity = -999.;
	daily_clim->tdewpoint = -999.;
	daily_clim->tavg = -999.;
	daily_clim->Ldown = -999.;
	daily_clim->dayl = -999.;
	daily_clim->tday = -999.;
	daily_clim->tnight = -999.;
	daily_clim->tnightmax = -999.;

	return;
} /*end construct_daily_clim*/
