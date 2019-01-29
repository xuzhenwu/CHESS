/*--------------------------------------------------------------*/
/* 																*/
/*					output_patch						        */
/*																*/
/*	output_patch - creates output files objects.		        */
/*																*/
/*	NAME														*/
/*	output_patch - outputs current contents of a patch.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_patch(										*/
/*					struct	patch_object	*patch,				*/
/*					struct	date	date,  						*/
/*					FILE 	*outfile)							*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	outputs spatial structure according to commandline			*/
/*	specifications to specific files							*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	We only permit one fileset per spatial modelling level.     */
/*	Each fileset has one file for each timestep.  				*/
/*																*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
#include <iostream>

using namespace std;


void	output_patch_daily_hydrology(struct	patch_object *patch,
	int pch,
	double(*HydroMon)[HYDRO_NUM],
	struct	date current_date,
	FILE *outfile,
	struct command_line_object *command_line)
{
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	int  layer; //var_flag,

	int last_day_of_month[13]{ 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	// rectified for leap year
	if ((current_date.year % 4 == 0 && current_date.year % 100 != 0) || current_date.year % 400 == 0) {
		last_day_of_month[2] += 1;//add one day for Feb
	}

	double aevaporation, asublimation = 0.;
	double atranspiration = 0.;
	double asat_area = 0.;
	double RSMC = 0;
	double asoilevap = 0., acrain = 0.;

	aevaporation = (patch->evaporation + patch->exfiltration_sat_zone + patch->exfiltration_unsat_zone);
	atranspiration = (patch->transpiration_sat_zone + patch->transpiration_unsat_zone);
	asoilevap = (patch->exfiltration_sat_zone + patch->exfiltration_unsat_zone);
	RSMC = max(1 - max((patch->sat_deficit - patch->rz_storage - patch->unsat_storage), 0) / patch->soil_defaults->soil_water_cap, 0);
	/*
	if ((patch->sat_deficit - patch->rz_storage - patch->unsat_storage) / patch->soil_defaults->soil_water_cap > 1.){
		cout << patch->sat_deficit << " " << patch->soil_defaults->soil_water_cap << endl;
		cout << patch->unsat_storage << " " << patch->rz_storage << endl;
		cout << patch->ID << endl;
		cout << current_date.year << " " << current_date.month << " " << current_date.day << endl;
		getchar();

	}
	*/
	asublimation = patch->snowpack.sublimation;

	for (layer = 0; layer < patch->num_layers; layer++) {
		//for ( c=0 ; c<patch->layers[layer].count; c++ ){
		acrain += (patch->canopy_strata->rain_stored + patch->canopy_strata->snow_stored);
		//asublimation += patch->canopy_strata->sublimation;
	//}
	}






	//FILE OUT FUCTION

	if (command_line->pday == true) {

		fprintf(outfile, "%4.0d", current_date.year);
		fprintf(outfile, "%4.0d", current_date.month);
		fprintf(outfile, "%4.0d", current_date.day);
		fprintf(outfile, "%9.0d", patch->ID);
		fprintf(outfile, "%11.1f", patch->x);
		fprintf(outfile, "%12.1f", patch->y);
		fprintf(outfile, "%9.3f", patch->rain*1000.);
		fprintf(outfile, "%9.3f", patch->snow*1000.);
		fprintf(outfile, "%9.3f", patch->rain_throughfall_final*1000.);
		fprintf(outfile, "%9.3f", patch->snow_throughfall_final*1000.);
		fprintf(outfile, "%9.3f", patch->detention_store *1000.);
		fprintf(outfile, "%9.3f", patch->litter.rain_stored *1000.);
		fprintf(outfile, "%9.3f", (patch->canopy_strata->rain_stored + patch->canopy_strata->snow_stored) *1000.);
		fprintf(outfile, "%9.3f", (patch->canopy_strata->rain_interception + patch->canopy_strata->snow_interception) *1000.);
		fprintf(outfile, "%9.3f", patch->canopy_strata->rain_interception*1000.);
		fprintf(outfile, "%9.3f", patch->canopy_strata->snow_interception*1000.);
		fprintf(outfile, "%9.3f", (patch->subsurface_Qout + patch->surface_Qout + patch->gw.Qout) / patch->acc_area*1000.);//+ patch->gw.Qout
		fprintf(outfile, "%9.3f", (patch->subsurface_Qout + patch->gw.Qout) / patch->acc_area*1000.); //+ patch->gw.Qout
		fprintf(outfile, "%9.3f", patch->surface_Qout / patch->acc_area*1000.);
		fprintf(outfile, "%9.3f", patch->PET*1000.);
		fprintf(outfile, "%9.3f", patch->AET*1000.);
		fprintf(outfile, "%9.3f", atranspiration*1000.);
		fprintf(outfile, "%9.3f", aevaporation *1000.);
		fprintf(outfile, "%9.3f", asoilevap *1000.);
		fprintf(outfile, "%9.3f", (patch->canopy_strata->evaporation + patch->canopy_strata->sublimation)*1000.);
		fprintf(outfile, "%9.3f", (patch->evaporation_surf_litter + patch->evaporation_surf_soil)*1000.);
		fprintf(outfile, "%9.3f", patch->snowpack.water_equivalent_depth *1000.);
		fprintf(outfile, "%9.3f", patch->snowpack.sublimation *1000.);
		fprintf(outfile, "%9.3f", patch->snow_melt * 1000);
		fprintf(outfile, "%9.3f", patch->sat_deficit_z *1000.);
		fprintf(outfile, "%9.3f", patch->sat_deficit *1000.);
		fprintf(outfile, "%9.3f", patch->rz_storage *1000.);
		fprintf(outfile, "%9.3f", patch->unsat_storage *1000.);
		fprintf(outfile, "%9.3f", patch->rz_drainage *1000.);
		fprintf(outfile, "%9.3f", patch->unsat_drainage *1000.);
		fprintf(outfile, "%9.3f", patch->cap_rise *1000.);
		fprintf(outfile, "%9.3f", patch->gw_drainage * 1000);
		fprintf(outfile, "%9.3f", patch->gw.storage *1000.);
		fprintf(outfile, "%9.3f", patch->gw.NO3 *1000.);
		fprintf(outfile, "%9.3f", patch->gw.Qout*1000.);
		fprintf(outfile, "%9.3f", patch->gw.Nout *1000.);
		fprintf(outfile, "%9.3f", RSMC * 100);
		//fprintf(outfile,"%9.3f",asat_area*100);
		//fprintf(outfile,"%9.3f",patch->canopy_strata->dC13); 
		fprintf(outfile, "\n");

	}
	//we use else if since daily and monthly share a same file 
	//it need to altered if both of which are needed to bu output
	else if (command_line->pmon == true) {

		HydroMon[pch][0] = current_date.year;
		HydroMon[pch][1] = current_date.month;
		HydroMon[pch][2] = current_date.day;//it can be used as mutiplyer for rain accumulated value
		HydroMon[pch][3] = patch->ID;
		HydroMon[pch][4] = patch->x;
		HydroMon[pch][5] = patch->y;

		HydroMon[pch][6] += patch->rain*1000.;
		HydroMon[pch][7] += patch->snow*1000.;
		HydroMon[pch][8] += patch->rain_throughfall_final*1000.;
		HydroMon[pch][9] += patch->snow_throughfall_final*1000.;
		HydroMon[pch][10] += patch->detention_store *1000.;
		HydroMon[pch][11] += patch->litter.rain_stored *1000.;
		HydroMon[pch][12] += (patch->canopy_strata->rain_stored + patch->canopy_strata->snow_stored) *1000.;
		HydroMon[pch][13] += (patch->canopy_strata->rain_interception + patch->canopy_strata->snow_interception) *1000.;
		HydroMon[pch][14] += patch->canopy_strata->rain_interception*1000.;
		HydroMon[pch][15] += patch->canopy_strata->snow_interception*1000.;
		HydroMon[pch][16] += (patch->subsurface_Qout + patch->surface_Qout + patch->gw.Qout) / patch->acc_area*1000.;//+ patch->gw.Qout
		HydroMon[pch][17] += (patch->subsurface_Qout + patch->gw.Qout) / patch->acc_area*1000.; //+ patch->gw.Qout
		HydroMon[pch][18] += patch->surface_Qout / patch->acc_area * 1000;
		HydroMon[pch][19] += patch->PET*1000.;
		HydroMon[pch][20] += patch->AET*1000.;
		HydroMon[pch][21] += atranspiration*1000.;
		HydroMon[pch][22] += aevaporation *1000.;
		HydroMon[pch][23] += asoilevap *1000.;
		HydroMon[pch][24] += (patch->canopy_strata->evaporation + patch->canopy_strata->sublimation)*1000.;
		HydroMon[pch][25] += (patch->evaporation_surf_litter + patch->evaporation_surf_soil)*1000.;
		HydroMon[pch][26] += patch->snowpack.water_equivalent_depth *1000.;
		HydroMon[pch][27] += patch->snowpack.sublimation *1000.;
		HydroMon[pch][28] += patch->snow_melt * 1000;
		HydroMon[pch][29] += patch->sat_deficit_z *1000.;
		HydroMon[pch][30] += patch->sat_deficit *1000.;
		HydroMon[pch][31] += patch->rz_storage *1000.;
		HydroMon[pch][32] += patch->unsat_storage *1000.;
		HydroMon[pch][33] += patch->rz_drainage *1000.;
		HydroMon[pch][34] += patch->unsat_drainage *1000.;
		HydroMon[pch][35] += patch->cap_rise *1000.;
		HydroMon[pch][36] += patch->gw_drainage * 1000;
		HydroMon[pch][37] += patch->gw.storage *1000.;
		HydroMon[pch][38] += patch->gw.NO3 *1000.;
		HydroMon[pch][39] += patch->gw.Qout*1000.;
		HydroMon[pch][40] += patch->gw.Nout *1000.;
		HydroMon[pch][41] += RSMC * 100;

		//PRINT VALUE IN THE LAST DAY OF MONTH
		if (current_date.day == last_day_of_month[current_date.month]) {
		
			

			//we chose enum all values since it's better efficiency
			fprintf(outfile, "%4.0d", current_date.year);
			fprintf(outfile, "%4.0d", current_date.month);
			fprintf(outfile, "%4.0d", current_date.day);
			fprintf(outfile, "%9.0d", patch->ID);
			fprintf(outfile, "%11.1f", patch->x);
			fprintf(outfile, "%12.1f", patch->y);


			fprintf(outfile, "%9.3f", HydroMon[pch][6] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][7] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][8] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][9] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][10] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][11] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][12] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][13] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][14] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][15] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][16] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][17] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][18] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][19] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][20] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][21] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][22] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][23] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][24] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][25] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][26] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][27] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][28] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][29] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][30] / HydroMon[pch][2]);;
			fprintf(outfile, "%9.3f", HydroMon[pch][31] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][32] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][33] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][34] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][35] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][36] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][37] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][38] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][39] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][40] / HydroMon[pch][2]);
			fprintf(outfile, "%9.3f", HydroMon[pch][41] / HydroMon[pch][2]);
			fprintf(outfile, "\n");
		
			//REINIT AS 0
			for (int inx = 0; inx != HYDRO_NUM; inx++) {
				HydroMon[pch][inx] = 0;
			}
		}

	}

	return;
} /*end output_patch*/


