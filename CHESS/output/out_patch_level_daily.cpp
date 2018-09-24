#include <stdio.h>

#include <stdlib.h>
#include "CHESS.h"
#include <iostream>
using namespace std;
void out_patch_level_daily(int num_patches,
				struct patch_object *patch,
			   struct date current_date,
			   struct out_date_range outdate,
			   struct output_hydro_plant *DM_outfiles,
			   struct command_line_object *command_line)
{
	int i;

	//local functions
	void    output_patch_daily_hydrology(struct patch_object *,struct date ,FILE *);
	void	output_patch_daily_growth(struct patch_object *,struct date ,FILE *);
	static int out_flag=0;

	if(current_date.year == outdate.first_year && current_date.month == outdate.first_month && current_date.day == outdate.first_day)
		out_flag=1;

	//if(out_flag ==1 && command_line->p !=NULL && (current_date.year>=1997 && current_date.year<=2005 )){//
	if(out_flag ==1 && command_line->p !=NULL ){
		//if ((current_date.year >=2013 && current_date.year <=2020) || (current_date.year >=2026 && current_date.year <=2029) || (current_date.year >=2032 && current_date.year <=2035)){
		for(i=0;i<num_patches;i++){ //&& (current_date.year ==1992 || current_date.year ==1995 || current_date.year ==2002 || current_date.year ==2005)
			//if(patch[i].ID==1563) {
			output_patch_daily_hydrology(&patch[i],current_date,DM_outfiles->fPatchDailyHydro);
			output_patch_daily_growth(&patch[i],current_date,DM_outfiles->fPatchDailyPlant);
		}
		//}
	}

    if(current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		out_flag=0;

	return;
}