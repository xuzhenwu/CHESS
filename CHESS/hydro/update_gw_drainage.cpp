/*--------------------------------------------------------------*/
/* 								*/
/*		update_gw_drainage					*/
/*								*/
/*								*/
/*	NAME							*/
/*	update_gw_drainage -  					*/
/* 		drainage shallow subsurface saturation zone	*/
/*		from each patch to a regional (hillslope scale)	*/
/*		groundwater store				*/
/*		nitrogen is also drained using assumption 	*/
/*		of an exponential decay of N with depth		*/
/*								*/
/*	SYNOPSIS						*/
/*	int update_gw_drainage(					*/
/*			struct patch_object *			*/
/*			struct hillslope_object *		*/
/*			struct command_line_object *		*/
/*			struct date,				*/
/*			)					*/
/*								*/
/*	returns:						*/
/*								*/
/*	OPTIONS							*/
/*								*/
/*	DESCRIPTION						*/
/*								*/
/*								*/
/*	PROGRAMMER NOTES					*/
/*	preset code just uses a user assigned loading rate	*/
/*	and all of it is nitrate				*/
/*								*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "CHESS.h"
#include "Constants.h"

void update_gw_drainage(
				  struct  patch_object   *patch,
				  struct  command_line_object *command_line,
				  struct	date	current_date)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.			*/
	/*------------------------------------------------------*/
	
	double  compute_z_final(
		int,
		double,
		double,
		double,
		double,
		double);


	double compute_N_leached(int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);

	/*------------------------------------------------------*/
	/*	Local Variable Definition. 			*/
	double drainage,N_loss; //sat_store,
	//double preday_sat_deficit_z, add_field_capacity;

	/*------------------------------------------------------*/
	/*		assume percent of incoming precip	*/
	/*------------------------------------------------------*/
	drainage = patch->soil_defaults->sat_to_gw_coeff * patch->rain_throughfall;
	patch->rain_throughfall -= drainage;
	patch->gw_drainage = drainage;
	patch->gw.storage += drainage;
	//printf("sat_to_gw_coeff is %f \n",patch->soil_defaults->sat_to_gw_coeff);

	/*------------------------------------------------------*/
	/*	determine associated N leached			*/
	/*------------------------------------------------------*/
	if (patch->surface_NO3 > ZERO) {
		N_loss = patch->soil_defaults->sat_to_gw_coeff * patch->surface_NO3;
		patch->gw.NO3      += N_loss;
		patch->ndf.N_to_gw  = N_loss;
		patch->surface_NO3 -= N_loss;
	}
	
	return;
} /* end update_gw_drainage.c */
