//------------------------------------------------------------------------------------------------------------------------------
//	A parallel way for computing daily F
//	Designed by Xu.
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include <iostream>

using namespace std;


void		parallel_land_daily(struct 	patch_object *patch,
	struct 	command_line_object *command_line,
	struct	date 			current_date,
	struct  daily_clim_object *daily_clim,
	int patch_pch[][PATCH_NUM],
	int thread_patch_num[],
	int thread_inx
)
{
	int start_hour=1;
	int end_hour=24;

	for (int patch_inx = 0; patch_inx != thread_patch_num[thread_inx]; patch_inx++) {
	

		int pch = patch_pch[thread_inx][patch_inx];
		int climate_inx = patch[pch].climatetype - 1;//since it start from 1
		//---------------------------------------------------------------------------------------------------------------------------
		//xu. Initial some parameters as Zero before new computation
		//---------------------------------------------------------------------------------------------------------------------------
		zero_patch_object(&patch[pch]);

		//---------------------------------------------------------------------------------------------------------------------------
		//Computation of Daylength, FieldCapacity, Capillary Rise, Max Exfiltration Rate,
		//Cycles in Canopy layers, Soil water potential
		//---------------------------------------------------------------------------------------------------------------------------
		patch_daily_initial(&patch[pch], daily_clim[climate_inx], command_line, current_date);

		//---------------------------------------------------------------------------------------------------------------------------
		//Kdirect and Kdiffuse
		//---------------------------------------------------------------------------------------------------------------------------
		patch_radiation_daily(&patch[pch], command_line, current_date);
		//---------------------------------------------------------------------------------------------------------------------------
		//A key complex function involing:
		//
		//---------------------------------------------------------------------------------------------------------------------------
		patch_daily_final(&patch[pch], command_line, current_date);

		compute_subsurface_routing_new(&patch[pch], command_line, 24, current_date);
	
	
	}

}//end of thread_line