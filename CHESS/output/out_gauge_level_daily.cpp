#include <stdio.h>

#include <stdlib.h>
#include "CHESS.h"
#include "Functions.h"
#include <iostream>

using namespace std;
void out_gauge_level_daily(int num_patches,
	struct patch_object *patch,
	struct date current_date,
	struct out_date_range outdate,
	struct output_hydro_plant *DM_outfiles,
	struct command_line_object *command_line,
	int *gauge_list,
	int cellsize
)
{
	int i;

	static int out_flag = 0;

	if (current_date.year == outdate.first_year && current_date.month == outdate.first_month && current_date.day == outdate.first_day)
		out_flag = 1;


	if (out_flag == 1 && command_line->gg != NULL) {

		for (int gauge_inx = 0; gauge_inx != gauge_num; gauge_inx++) {
			output_gauge_daily_hydrology(&patch[(gauge_list[gauge_inx])], current_date, DM_outfiles->fGaugeDailyHydro[gauge_inx], command_line,cellsize);
			output_gauge_daily_growth(&patch[(gauge_list[gauge_inx])], current_date, DM_outfiles->fGaugeDailyPlant[gauge_inx], command_line, cellsize);
		}
	}

	if (current_date.year == outdate.last_year && current_date.month == outdate.last_month && current_date.day == outdate.last_day)
		out_flag = 0;

	return;
}