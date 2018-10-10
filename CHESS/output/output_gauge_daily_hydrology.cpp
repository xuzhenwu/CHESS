/*--------------------------------------------------------------*/
/* 																*/
/*					output_basin						*/
/*																*/
/*	output_basin - creates output files objects.		*/
/*																*/
/*	NAME														*/
/*	output_basin - outputs current contents of a basin.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_basin( int routing_flag,										*/
/*					struct	basin_object	*basin,				*/
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

void	output_gauge_daily_hydrology(struct	patch_object *patch,
	struct	date	current_date,
	FILE *outfile, struct command_line_object *command_line,int cellsize)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/

	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	int p = 0; //h,z,,c
	int  layer = 1; //var_flag,
	double astreamflow_N = 0.;
	double arain = 0., asnow = 0.;
	double arain_throughfall = 0.;
	double asnow_throughfall = 0.;
	double asat_deficit_z = 0.;
	double asat_deficit = 0.;
	double aunsat_storage = 0., arz_storage = 0.;
	double aunsat_drainage = 0., arz_drainage = 0.;
	double acap_rise = 0.;
	double areturn_flow = 0.;
	double aevaporation = 0., acan_evap = 0.;
	double asnowpack = 0.;
	double atranspiration = 0., asoil_evap = 0.;
	double astreamflow = 0.;
	double asat_area = 0.;
	double abase_flow = 0., arsmc = 0.;

	double aaet = 0., adC13 = 0., apet = 0.;
	double gw_drain = 0., gw_Nstore = 0., gw_store = 0.;
	double gw_discharge = 0., gw_Nout = 0.;
	double aarea = 0., basin_area = 0.;

	double asnow_melt = 0.;
	double CaIntTot = 0., CaInRain = 0., CaInSnow = 0., acan_sublimation = 0., asnowpack_sub = 0.;
	double alitter_store = 0., asoil_store = 0., acanopy_store = 0., asurf_evap = 0., APET = 0.;
	int snownumber = 0;


	arain += patch->rain;
	asnow += patch->snow;
	arain_throughfall += patch->rain_throughfall;
	asnow_throughfall += patch->snow_throughfall;

	alitter_store += patch->litter.rain_stored;
	asoil_store += patch->detention_store;
	//acanopy_store     += patch->rain_stored + patch->snow_stored;

	asat_deficit_z += patch->sat_deficit_z;
	asat_deficit += patch->sat_deficit;
	arz_storage += patch->rz_storage;
	aunsat_storage += patch->unsat_storage;
	arz_drainage += patch->rz_drainage;
	aunsat_drainage += patch->unsat_drainage;
	acap_rise += patch->cap_rise;
	arsmc += max(1 - max(patch->sat_deficit - patch->rz_storage - patch->unsat_storage, 0) / patch->soil_defaults->soil_water_cap, 0);

	apet += patch->PET; //Potential evapotranspiration
	aaet += patch->AET; //actual evapotranspiration
	aevaporation += (patch->evaporation + patch->exfiltration_sat_zone + patch->exfiltration_unsat_zone);
	atranspiration += (patch->transpiration_sat_zone + patch->transpiration_unsat_zone); //
	asoil_evap += (patch->exfiltration_sat_zone + patch->exfiltration_unsat_zone); //soil evaporation  patch->exfiltration_sat_zone + patch->exfiltration_unsat_zone
	acan_evap += (patch->canopy_strata->evaporation + patch->canopy_strata->sublimation); //canopy strata evaporation + sublimation 
	asurf_evap += patch->evaporation_surf_litter + patch->evaporation_surf_soil; //evaporation from soil detention and litter detention //

	asnowpack += patch->snowpack.water_equivalent_depth;
	asnowpack_sub += patch->snowpack.sublimation;
	asnow_melt += patch->snow_melt;
	//snownumber+=1;

	if (patch->sat_deficit <= ZERO)
		asat_area += 1.0;

	if (command_line->routing_flag == 1) {
		//=====================================================================
		// if water is routed
		//=====================================================================
		astreamflow += (patch->subsurface_Qout + patch->surface_Qout + patch->gw.Qout);
		abase_flow += patch->subsurface_Qout + patch->gw.Qout;
		areturn_flow += patch->surface_Qout;
	}
	else {
		//=======================================================================
		// if water is not routed
		//=======================================================================
		astreamflow += patch->subsurface_Qout + patch->surface_Qout;
		abase_flow += patch->subsurface_Qout;
		areturn_flow += patch->surface_Qout;
		//=======================================================================
	}

	for (layer = 0; layer < patch->num_layers; layer++) {
		//for ( c=0 ; c<patch->layers[layer].count; c++ ){
		acanopy_store += (patch->canopy_strata->rain_stored + patch->canopy_strata->snow_stored);
		CaIntTot += patch->canopy_strata->rain_interception + patch->canopy_strata->snow_interception;
		CaInRain += patch->canopy_strata->rain_interception;
		CaInSnow += patch->canopy_strata->snow_interception;
		acan_sublimation += patch->canopy_strata->sublimation;
		adC13 += patch->canopy_strata->dC13;
		//}
	}

	gw_drain += patch->gw_drainage;
	gw_store += patch->gw.storage;
	gw_Nstore += patch->gw.NO3;
	gw_discharge += patch->gw.Qout;
	gw_Nout += patch->gw.Nout;



	fprintf(outfile, "%4.0d", current_date.year);
	fprintf(outfile, "%4.0d", current_date.month);
	fprintf(outfile, "%4.0d", current_date.day);
	fprintf(outfile, "%9.3f", arain *1000.);
	fprintf(outfile, "%9.3f", asnow *1000.);
	fprintf(outfile, "%9.3f", arain_throughfall *1000.);
	fprintf(outfile, "%9.3f", asnow_throughfall *1000.);
	fprintf(outfile, "%9.3f", asoil_store *1000.);
	fprintf(outfile, "%9.3f", alitter_store *1000.);
	fprintf(outfile, "%9.3f", acanopy_store *1000.);
	fprintf(outfile, "%9.3f", CaIntTot *1000.);
	fprintf(outfile, "%9.3f", CaInRain *1000.);
	fprintf(outfile, "%9.3f", CaInSnow *1000.);

	//xu. m^3/day
	fprintf(outfile, "%9.3f", (astreamflow)*pow(cellsize, 2) / seconds_per_day); //+ gw_discharge
	fprintf(outfile, "%9.3f", (abase_flow) *pow(cellsize, 2) / seconds_per_day); //+ gw_discharge
	fprintf(outfile, "%9.3f", areturn_flow *pow(cellsize, 2) / seconds_per_day);
	fprintf(outfile, "%9.3f", apet *1000.);
	fprintf(outfile, "%9.3f", aaet *1000.);
	fprintf(outfile, "%9.3f", atranspiration *1000.);
	fprintf(outfile, "%9.3f", aevaporation *1000.);
	fprintf(outfile, "%9.3f", asoil_evap *1000.);
	fprintf(outfile, "%9.3f", acan_evap *1000.); //include canopy_sublimation
	fprintf(outfile, "%9.3f", asurf_evap *1000.);
	fprintf(outfile, "%9.3f", asnowpack *1000.);
	fprintf(outfile, "%9.3f", asnowpack_sub *1000.);
	fprintf(outfile, "%9.3f", asnow_melt *1000.);
	fprintf(outfile, "%9.3f", asat_deficit_z *1000.);
	fprintf(outfile, "%9.3f", asat_deficit *1000.);
	fprintf(outfile, "%9.3f", arz_storage *1000.);
	fprintf(outfile, "%9.3f", aunsat_storage *1000.);
	fprintf(outfile, "%9.3f", arz_drainage *1000.);
	fprintf(outfile, "%9.3f", aunsat_drainage *1000.);
	fprintf(outfile, "%9.3f", acap_rise *1000.);
	fprintf(outfile, "%9.3f", gw_drain  * 1000);
	fprintf(outfile, "%9.3f", gw_store *1000.);
	fprintf(outfile, "%9.3f", gw_Nstore *1000.);
	fprintf(outfile, "%9.3f", gw_discharge *1000.);
	fprintf(outfile, "%9.3f", gw_Nout *1000.);
	fprintf(outfile, "%9.3f", arsmc  * 100);
	//fprintf(outfile,"%9.3f",asat_area/double(num_patches)*100);
	//fprintf(outfile,"%9.3f",adC13/double(num_patches)); 

	//if((aevaporation-asoil_evap-acan_evap-asurf_evap-asnowpack_sub)>ZERO){
	//	printf("evaporation not equal %12.8f %12.8f %12.8f %12.8f\n",aevaporation,asoil_evap,acan_evap,asurf_evap);
	//	getchar();
	//}
	//if(APET-apet>ZERO) {
	//	printf("%12.8f %12.8f \n",APET,apet);
	//		getchar();
	//}
	fprintf(outfile, "\n");

	return;
} /*end output_basin*/
