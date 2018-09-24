#include <stdio.h>

#include <stdlib.h>
#include "CHESS.h"


void out_basin_level_daily(int num_patches,
				struct patch_object *patch,
			   struct date current_date,
			   struct out_date_range outdate,
			   struct output_hydro_plant *DM_outfiles,
			   struct command_line_object *command_line)
{
	//local functions
	void	output_basin_daily_hydrology(int,struct patch_object *,struct date ,FILE *,struct command_line_object *);
	void	output_basin_daily_growth(int, struct patch_object *, struct date, FILE *, struct command_line_object *);
	static int outflag=0;

	if(current_date.year == outdate.first_year && current_date.month ==outdate.first_month && current_date.day == outdate.first_day)
		outflag=1;
	//printf("outflag is %d \n",outflag);

	if(outflag ==1 && command_line->b !=NULL){
		output_basin_daily_hydrology(num_patches,patch,current_date,DM_outfiles->fBasinDailyHydro,command_line);
		output_basin_daily_growth(num_patches, patch, current_date, DM_outfiles->fBasinDailyPlant, command_line);
	}

    if(current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		outflag=0;

	return;
}