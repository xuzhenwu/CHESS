//Note: this program is derived from hillslope_daly_f.c subroutien in original rhessys
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

void		compute_ground_water_loss(
							  struct	patch_object *patch,
							  struct 	command_line_object *command_line,
							  struct 	date current_date)
{
	/*----------------------------------------------------------------------*/
	/*	compute groundwater losses					*/
	/*----------------------------------------------------------------------*/
	if ((command_line->gw_flag > 0) && (patch->gw.storage > ZERO)) {
		patch->gw.Qout = patch->gw.storage * patch->slope / 1.571 *patch->climate_defaults->gw_loss_coeff;
		patch->gw.Nout = patch->gw.Qout * patch->gw.NO3 / patch->gw.storage;
	}
	else{
		patch->gw.Qout = 0.;
		patch->gw.Nout = 0.;

	}
	patch->subsurface_leach_NO3_out      += patch->gw.Nout;
	//patch->subsurface_Qout   += patch->gw.Qout;  //??
	//patch->sat_deficit       += patch->gw.Qout;
	patch->gw.storage        -= patch->gw.Qout;
	patch->gw.NO3            -= patch->gw.Nout;

} 