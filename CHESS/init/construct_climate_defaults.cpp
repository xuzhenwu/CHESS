#include <stdio.h>

#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"

struct climate_default *construct_climate_defaults(
	int	nytpes,
	FILE	*default_file,
	struct command_line_object *command_line,
	struct climate_default * climate_default_object_list)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	void	*alloc(size_t, char *, char *);

	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	char	record[MAXSTR];

	/*--------------------------------------------------------------*/
	/*	Loop through the default files list.						*/
	/*--------------------------------------------------------------*/

		/*--------------------------------------------------------------*/
		/*		read the ith default file into the ith object.			*/
		/*--------------------------------------------------------------*/
	read_record(default_file, record);
	//printf("record %s \n",record);
	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%d", &(climate_default_object_list[i].ID));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].mean_elev));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].std));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].cover_fraction));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].gap_fraction));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].root_depth));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].snowpack.energy_deficit));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].snowpack.T));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].maximum_snow_energy_deficit));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].snow_water_capacity));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].snow_light_ext_coef));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].snow_melt_Tcoef));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].screen_height));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].dewpoint_lapse_rate));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].lapse_rate));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].pptmin));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].sea_level_clear_sky_trans));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].atm_trans_lapse_rate));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].temcf));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].trans_coeff1));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].trans_coeff2));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].wind));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].max_snow_temp));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].min_rain_temp));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].ndep_NO3));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].lapse_rate_tmin));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].lapse_rate_tmax));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(climate_default_object_list[i].lapse_rate_prec));
	}
	read_record(default_file, record);//end of 28 var

	read_record(default_file, record);  //end of records

	for (int i = 0; i < nytpes; i++) {
		climate_default_object_list[i].wet_lapse_rate = 0.0049;

		climate_default_object_list[i].gw_loss_coeff = 1.;
		if (command_line->gw_flag > 0) {
			climate_default_object_list[i].gw_loss_coeff *= command_line->gw_loss_coeff_mult;
		}
	}

	fclose(default_file);
	return(climate_default_object_list);
} /*end construct_climate_defaults*/
