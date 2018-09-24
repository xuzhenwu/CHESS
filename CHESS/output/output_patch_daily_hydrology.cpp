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
					 struct	date current_date,
					 FILE *outfile)
{
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	int  layer; //var_flag,

	double aevaporation,asublimation=0.;
	double atranspiration=0.;
	double asat_area=0.;
	double RSMC=0;
	double asoilevap=0.,acrain=0.;

	aevaporation      = (patch->evaporation + patch->exfiltration_sat_zone  + patch->exfiltration_unsat_zone);
	atranspiration    = (patch->transpiration_sat_zone + patch->transpiration_unsat_zone);
	asoilevap         =	(patch->exfiltration_sat_zone  + patch->exfiltration_unsat_zone);			 
	RSMC              = max(1-max((patch->sat_deficit-patch->rz_storage-patch->unsat_storage),0)/patch->soil_defaults->soil_water_cap,0);
	/*
	if ((patch->sat_deficit - patch->rz_storage - patch->unsat_storage) / patch->soil_defaults->soil_water_cap > 1.){
		cout << patch->sat_deficit << " " << patch->soil_defaults->soil_water_cap << endl;
		cout << patch->unsat_storage << " " << patch->rz_storage << endl;
		cout << patch->ID << endl;
		cout << current_date.year << " " << current_date.month << " " << current_date.day << endl;
		getchar();

	}
	*/
	asublimation      = patch->snowpack.sublimation;

	for ( layer=0 ; layer<patch->num_layers; layer++ ){
		//for ( c=0 ; c<patch->layers[layer].count; c++ ){
			acrain       += (patch->canopy_strata->rain_stored + patch->canopy_strata->snow_stored);
			//asublimation += patch->canopy_strata->sublimation;
		//}
	}

	fprintf(outfile,"%4.0d",current_date.year);
	fprintf(outfile,"%4.0d",current_date.month);
	fprintf(outfile,"%4.0d",current_date.day);
	fprintf(outfile,"%9.0d", patch->ID);
	fprintf(outfile,"%11.1f",patch->x);
	fprintf(outfile,"%12.1f",patch->y);
	fprintf(outfile,"%9.3f",patch->rain*1000.);
	fprintf(outfile,"%9.3f",patch->snow*1000.);
	fprintf(outfile,"%9.3f",patch->rain_throughfall_final*1000.);
	fprintf(outfile,"%9.3f",patch->snow_throughfall_final*1000.);
	fprintf(outfile,"%9.3f",patch->detention_store *1000.);
	fprintf(outfile,"%9.3f",patch->litter.rain_stored *1000.);
	fprintf(outfile,"%9.3f",(patch->canopy_strata->rain_stored+patch->canopy_strata->snow_stored) *1000.); 
	fprintf(outfile,"%9.3f",(patch->canopy_strata->rain_interception + patch->canopy_strata->snow_interception) *1000.);
	fprintf(outfile,"%9.3f",patch->canopy_strata->rain_interception*1000.);
	fprintf(outfile,"%9.3f",patch->canopy_strata->snow_interception*1000.);
	fprintf(outfile,"%9.3f",(patch->subsurface_Qout + patch->surface_Qout+ patch->gw.Qout)/patch->acc_area*1000.);//+ patch->gw.Qout
	fprintf(outfile,"%9.3f",(patch->subsurface_Qout + patch->gw.Qout)/patch->acc_area*1000.); //+ patch->gw.Qout
	fprintf(outfile,"%9.3f",patch->surface_Qout/patch->acc_area*1000.);
	fprintf(outfile,"%9.3f",patch->PET*1000.);
	fprintf(outfile,"%9.3f",patch->AET*1000.);
	fprintf(outfile,"%9.3f",atranspiration*1000.);
	fprintf(outfile,"%9.3f",aevaporation *1000.);
	fprintf(outfile,"%9.3f",asoilevap *1000.);
	fprintf(outfile,"%9.3f",(patch->canopy_strata->evaporation+patch->canopy_strata->sublimation)*1000.);
	fprintf(outfile,"%9.3f",(patch->evaporation_surf_litter+patch->evaporation_surf_soil)*1000.);
	fprintf(outfile,"%9.3f",patch->snowpack.water_equivalent_depth *1000.);
	fprintf(outfile,"%9.3f",patch->snowpack.sublimation *1000.);
	fprintf(outfile,"%9.3f",patch->snow_melt*1000); 
	fprintf(outfile,"%9.3f",patch->sat_deficit_z *1000.);
	fprintf(outfile,"%9.3f",patch->sat_deficit *1000.);
	fprintf(outfile,"%9.3f",patch->rz_storage *1000.);
	fprintf(outfile,"%9.3f",patch->unsat_storage *1000.);
	fprintf(outfile,"%9.3f",patch->rz_drainage *1000.);
	fprintf(outfile,"%9.3f",patch->unsat_drainage *1000.);
	fprintf(outfile,"%9.3f",patch->cap_rise *1000.);
	fprintf(outfile,"%9.3f",patch->gw_drainage*1000);
	fprintf(outfile,"%9.3f",patch->gw.storage *1000.);	
	fprintf(outfile,"%9.3f",patch->gw.NO3 *1000.);
	fprintf(outfile,"%9.3f",patch->gw.Qout*1000.);       
	fprintf(outfile,"%9.3f",patch->gw.Nout *1000.);		 
	fprintf(outfile,"%9.3f",RSMC*100);
	//fprintf(outfile,"%9.3f",asat_area*100);
	//fprintf(outfile,"%9.3f",patch->canopy_strata->dC13); 
	fprintf(outfile,"\n");

	return;
} /*end output_patch*/
