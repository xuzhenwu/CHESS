#include <stdio.h>
#include "Functions.h"
#include <stdlib.h>
#include "CHESS.h"
#include <iostream>
using namespace std;
void out_patch_level_daily(int num_patches,
				struct patch_object *patch,
			   struct date current_date,
			   struct out_date_range outdate,
			   struct output_hydro_plant *DM_outfiles,
				struct OutArray_object *OutArray,
			   struct command_line_object *command_line)
{
	static int out_flag=0;

	if (current_date.year == outdate.first_year && current_date.month == outdate.first_month && current_date.day == outdate.first_day)	
		out_flag = 1;	

	//if(out_flag ==1 && command_line->p !=NULL && (current_date.year>=1997 && current_date.year<=2005 )){//
	if(out_flag ==1 && command_line->p !=NULL ){

		for(int pch=0; pch <num_patches; pch++){
			
			output_patch_daily_hydrology(&patch[pch], pch, OutArray->HydroMon,current_date,DM_outfiles->fPatchDailyHydro,command_line);
			output_patch_daily_growth(&patch[pch], pch, OutArray->PlantMon, current_date, DM_outfiles->fPatchDailyPlant, command_line);
		}
		//}
	}

    if(current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		out_flag=0;

	return;
}