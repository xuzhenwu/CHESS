#include <stdio.h>

#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"

struct patch_default *construct_patch_defaults(
											 int	nytpes,
											 FILE	*default_file,
											 struct command_line_object *command_line,
											 struct patch_default * patch_default_object_list)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	void	*alloc(	size_t, char *, char *);
	
	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	char	record[MAXSTR];

	
	/*--------------------------------------------------------------*/
	/*	Loop through the default files list.						*/
	/*--------------------------------------------------------------*/
	for (int i=0 ; i<nytpes; i++) {
		/*--------------------------------------------------------------*/
		/*		read the ith default file into the ith object.			*/
		/*--------------------------------------------------------------*/
		read_record(default_file, record);
		//printf("record %s \n",record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].latitude));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].mean_elev));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].std));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].cover_fraction));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].gap_fraction));
		read_record(default_file, record);
		
		fscanf(default_file,"%lf",&(patch_default_object_list[i].root_depth));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].snowpack.energy_deficit));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].snowpack.T));
		read_record(default_file, record);

		fscanf(default_file, "%lf", &(patch_default_object_list[i].maximum_snow_energy_deficit));
		read_record(default_file, record);

		fscanf(default_file, "%lf", &(patch_default_object_list[i].snow_water_capacity));
		read_record(default_file, record);

		fscanf(default_file, "%lf", &(patch_default_object_list[i].snow_light_ext_coef));
		read_record(default_file, record);

		fscanf(default_file, "%lf", &(patch_default_object_list[i].snow_melt_Tcoef));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].screen_height));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].dewpoint_lapse_rate));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].lapse_rate));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].pptmin));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].sea_level_clear_sky_trans));
		read_record(default_file, record);

		fscanf(default_file, "%lf", &(patch_default_object_list[i].atm_trans_lapse_rate));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].temcf));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].trans_coeff1));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].trans_coeff2));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].wind));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].max_snow_temp));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].min_rain_temp));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].ndep_NO3));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].lapse_rate_tmin));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].lapse_rate_tmax));
		read_record(default_file, record);

		fscanf(default_file,"%lf",&(patch_default_object_list[i].lapse_rate_prec));
		read_record(default_file, record);

		read_record(default_file, record);  //end of records

		patch_default_object_list[i].wet_lapse_rate = 0.0049; 

		patch_default_object_list[i].gw_loss_coeff=1.;
		if (command_line->gw_flag > 0) {
			patch_default_object_list[i].gw_loss_coeff *= command_line->gw_loss_coeff_mult;
		}


	} /*end for*/

	fclose(default_file);
	return(patch_default_object_list);
} /*end construct_zone_defaults*/
