/*--------------------------------------------------------------*/
/* 								*/
/*		update_septic					*/
/*								*/
/*								*/
/*	NAME							*/
/*	update_septic -  					*/
/*		adds leakage from septic to soil N		*/
/*								*/
/*	SYNOPSIS						*/
/*	int update_septic(					*/
/*			struct date,				*/
/*			struct	soil_n_object	*		*/
/*			double)					*/
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

void update_septic(
				  struct	date	current_date,
				  struct  patch_object   *patch)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.			*/
	/*------------------------------------------------------*/
	
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 			*/
	/*------------------------------------------------------*/
	patch->soil_ns.nitrate += patch->landuse_defaults->septic_NO3_load; 
	patch->unsat_storage += patch->landuse_defaults->septic_water_load;
		
	return;
} /* end update_septic.c */
