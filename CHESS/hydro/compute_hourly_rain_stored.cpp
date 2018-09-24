/*--------------------------------------------------------------*/
/* 								*/
/*		compute_hourly_rain_stored				*/
/*								*/
/*								*/
/*	NAME							*/
/*	compute_hourly_rain_stored -  rain intercepted by stratum and 	*/
/*		updates rain flaaing through.			*/
/*								*/
/*	SYNOPSIS						*/
/*	double	compute_hourly_rain_stored(				*/
/*			double	*,				*/
/*			struct	canopy_strata_object	*);	*/
/*								*/
/*	returns:						*/
/*	rain_storage (m) - amount of rain left in canopy	*/
/*								*/
/*	also updates the following state variables:		*/
/*								*/
/*	*rain (m) - rain falling on next stratum		*/
/*								*/
/*	OPTIONS							*/
/*								*/
/*	DESCRIPTION						*/
/*								*/
/*	updates interception - for a storm event		*/
/*								*/
/*	PROGRAMMER NOTES					*/
/*								*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
double	compute_hourly_rain_stored(
								   int	verbose_flag,
								   double	*rain,
								   struct	canopy_strata_object	*stratum)
{
	/*--------------------------------------------------------------*/
	/*	Local function declaration				*/
	/*--------------------------------------------------------------*/
	double	compute_potential_rain_interception(
		int,
		double	,
		struct	canopy_strata_object *);
	/*--------------------------------------------------------------*/
	/*	Local variable definition.				*/
	/*--------------------------------------------------------------*/
	double	potential_interception;
	double	rain_storage;
	double	throughfall;
	/*--------------------------------------------------------------*/
	/*	Compute amount potentially intercepted.			*/
	/*	m = m2PlANT / m2ground *  ( (kg  / m2 * day * m2PLANT )	*/
	/*		* ( 1 m3 H20 / 1000 kg H20 )			*/
	/*--------------------------------------------------------------*/
	potential_interception = compute_potential_rain_interception(
		verbose_flag,
		*rain,
		stratum );
	if( verbose_flag >2) printf("%8.6f ",potential_interception);
	/*--------------------------------------------------------------*/
	/*	Add amount not potentially intercepted to throughfall.	*/
	/*	m = m - m 						*/
	/*--------------------------------------------------------------*/
	throughfall  = *rain - potential_interception;
	/*--------------------------------------------------------------*/
	/*  Update rain storage.										*/
	/*	m = m							*/
	/*--------------------------------------------------------------*/
	rain_storage = min( stratum->rain_stored + potential_interception,
		stratum->epv.all_pai
		* stratum->defaults->specific_rain_capacity );

	/*--------------------------------------------------------------*/
	/*	Update rain throughfall.									*/
	/*	m += m							*/
	/*--------------------------------------------------------------*/
	throughfall += max(potential_interception
		- (rain_storage - stratum->rain_stored),0);

	*rain = throughfall;
	return( rain_storage);
} /*end compute_hourly_rain_stored*/
