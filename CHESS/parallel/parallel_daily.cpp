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


void		parallel_daily(struct 	patch_object *patch,
	struct 	command_line_object *command_line,
	struct	date 			current_date,
	struct  daily_clim_object *daily_clim,
	int patch_pch[][patch_num],
	int thread_patch_num[],
	int thread_inx
)
{
	int start_hour=1;
	int end_hour=24;

	for (int patch_inx = 0; patch_inx != thread_patch_num[thread_inx]; patch_inx++) {
	

		int pch = patch_pch[thread_inx][patch_inx];

		//---------------------------------------------------------------------------------------------------------------------------
		//xu. Initial some parameters as Zero before computation of a BRAND NEW DAY!
		//---------------------------------------------------------------------------------------------------------------------------
		init_patch_object(&patch[pch]);

		//---------------------------------------------------------------------------------------------------------------------------
		//Computation of Daylength, FieldCapacity, Capillary Rise, Max Exfiltration Rate,
		//Cycles in Canopy layers, Soil water potential
		//---------------------------------------------------------------------------------------------------------------------------
		patch_daily_I(&patch[pch], daily_clim[thread_inx], command_line, current_date);

		//---------------------------------------------------------------------------------------------------------------------------
		//Kdirect and Kdiffuse
		//---------------------------------------------------------------------------------------------------------------------------
		for (current_date.hour = start_hour; current_date.hour <= end_hour; current_date.hour++) {
			patch_hourly(&patch[pch], command_line, current_date);
		}

		//---------------------------------------------------------------------------------------------------------------------------
		//A key complex function involing:
		//
		//---------------------------------------------------------------------------------------------------------------------------
		patch_daily_F(&patch[pch], command_line, current_date);

		compute_subsurface_routing_new(&patch[pch], command_line, 24, current_date);
	
	
	}

}//end of thread_line