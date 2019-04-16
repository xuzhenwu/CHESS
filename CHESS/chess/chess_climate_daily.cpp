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
#include <Constants.h>
#include <string.h>
#include "CHESS.h"

void chess_climate_daily(struct input_Clim_Files inClimFiles,
	struct	date	current_date,
	struct  daily_clim_object *daily_clim,
	int climate_num,
	int start_year,
	int start_month,
	int start_day, int *CO2_flag
)
{
	int   yr=0,mo=0,dy=0;

	static int annual_order=0;
	static double CO2_PPM;
	//---------------------------------------------------------------------------------------------------------------------------
	//1. READING CLIMATE DATA	
	//xu. in current part of simulation the subbasins will own a separated climate data
	//---------------------------------------------------------------------------------------------------------------------------

	fscanf(inClimFiles.pPrec,"%d %d %d",&yr,&mo,&dy);
	for (int  clim_inx = 0;  clim_inx != climate_num;  clim_inx++) {
		fscanf(inClimFiles.pPrec, "%lf", &daily_clim[clim_inx].rain);
	}
	if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
		printf("precipitation data don't match \n");
		printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
		printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
		getchar();
		exit (0);
	}

	//reading maximum temperature data
	fscanf(inClimFiles.pTmax, "%d %d %d", &yr, &mo, &dy);
	for (int basin_inx = 0; basin_inx != climate_num; basin_inx++) {
		fscanf(inClimFiles.pTmax, "%lf", &daily_clim[basin_inx].tmax);
	}
	if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
		printf("Maximum temperature data don't match \n");
		printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
		printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
		getchar();
		exit (0);
	}
    //reading minimum temperature data
	fscanf(inClimFiles.pTmin, "%d %d %d", &yr, &mo, &dy);
	for (int  clim_inx = 0;  clim_inx != climate_num;  clim_inx++) {
		fscanf(inClimFiles.pTmin, "%lf", &daily_clim[clim_inx].tmin);
	}
	if(current_date.year !=yr || current_date.month!=mo ||current_date.day!=dy){
		printf("minimum temperature data don't match \n");
		printf("year,month,day %d %d %d \n",current_date.year,current_date.month,current_date.day);
		printf("yr,mo,dy %d %d %d \n",yr,mo,dy);
		getchar();
		exit (0);
	}

    if(yr == current_date.year && *CO2_flag==1){
		//reading annual atmospheric CO2 data
		//xu. for CO2, we assume all basins has a same annual CO2 input
		fscanf(inClimFiles.pCO2,"%d %lf ",&yr,&(daily_clim->CO2));
		for (int  clim_inx = 0;  clim_inx != climate_num;  clim_inx++) {
			daily_clim[clim_inx].CO2 = daily_clim->CO2;
		}
		*CO2_flag=0;
		if(current_date.year !=yr ){
			printf("CO2 data do not match \n");
			printf("year %d \n",current_date.year);
			printf("yr,mo,dy %d \n",yr);
			getchar();
			exit (0);
		}
		//printf("co2 is %d %d %12.8f \n",yr,current_date.year,CO2_PPM);
	}


	//---------------------------------------------------------------------------------------------------------------------------
	//2. INITIAL REST OF CLIM SECQUENCES WITH NULL DATA
	//xu. in current part of simulation the subbasins will own a separated climate data
	//---------------------------------------------------------------------------------------------------------------------------
	for (int basin_inx = 0; basin_inx != climate_num; basin_inx++) {

		daily_clim[basin_inx].snow = -999.;
		daily_clim[basin_inx].daytime_rain_duration = -999.;
		daily_clim[basin_inx].cloud_fraction = -999.;
		daily_clim[basin_inx].cloud_opacity = -999.;
		daily_clim[basin_inx].Delta_T = -999.;
		daily_clim[basin_inx].Kdown_direct = -999.;
		daily_clim[basin_inx].Kdown_diffuse = -999.;
		daily_clim[basin_inx].PAR_diffuse = -999.;
		daily_clim[basin_inx].PAR_direct = -999.;
		daily_clim[basin_inx].atm_trans = -999.;
		daily_clim[basin_inx].tsoil = -999.;
		daily_clim[basin_inx].wind = -999.;
		daily_clim[basin_inx].ndep_NO3 = -999.;
		daily_clim[basin_inx].ndep_NH4 = -999.;
		daily_clim[basin_inx].vpd = -999.;
		daily_clim[basin_inx].relative_humidity = -999.;
		daily_clim[basin_inx].tdewpoint = -999.;
		daily_clim[basin_inx].tavg = -999.;
		daily_clim[basin_inx].Ldown = -999.;
		daily_clim[basin_inx].dayl = -999.;
		daily_clim[basin_inx].tday = -999.;
		daily_clim[basin_inx].tnight = -999.;
		daily_clim[basin_inx].tnightmax = -999.;
	}

	return;
} /*end construct_daily_clim*/
