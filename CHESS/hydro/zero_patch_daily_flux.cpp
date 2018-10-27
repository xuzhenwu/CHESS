//-------------------------------------------------------------------------------------------------
//		zero_patch_daily_flux				
//								
//								
//	NAME							
//		zero_patch_daily_flux
//								
//	SYNOPSIS						
//	double	zero_patch_daily_flux(				
//			struct cdayflux_patch_struct *,		
//			struct ndayflux_patch_struct *,		
//				)					
//								
//	returns:						
//								
//	OPTIONS							
//								
//	DESCRIPTION						
//								
//	zeros all daily patch fluxes (carbon and nitrogen)	
//								
//	PROGRAMMER NOTES					
//-------------------------------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "CHESS.h"
#include "Constants.h"

void zero_patch_daily_flux(
				struct patch_object *patch,
				struct cdayflux_patch_struct* cdf,
				struct ndayflux_patch_struct* ndf)
{
    //---------------------------------------------------------------------------------------------
	// daily hydro  fluxes					
    //---------------------------------------------------------------------------------------------
	//First, initialize preday variables for checking water balances added by Guoping
	patch->preday_rain_stored = patch->rain_stored;
	patch->preday_snow_stored = patch->snow_stored;
	patch->litter.preday_rain_stored = patch->litter.rain_stored;
	patch->preday_detention_store = patch->detention_store;
	patch->preday_snowpack = patch->snowpack.water_equivalent_depth;
	patch->preday_sat_deficit_z = patch->sat_deficit_z;
	patch->preday_sat_deficit = patch->sat_deficit;
	patch->preday_rz_storage = patch->rz_storage;
	patch->preday_unsat_storage = patch->unsat_storage;

	//patch->preday_subsurface_out     = patch->subsurface_out;
	patch->canopy_strata->preday_rain_stored = patch->canopy_strata->rain_stored;
	patch->canopy_strata->preday_snow_stored = patch->canopy_strata->snow_stored;
	
	patch->canopy_strata->rain_stored = 0;
	patch->canopy_strata->snow_stored = 0;

	//following is added by guoping
	patch->canopy_strata->rain_interception = 0.;
	patch->canopy_strata->snow_interception = 0.;
	patch->canopy_strata->sublimation = 0.;
	patch->canopy_strata->evaporation = 0.;
	patch->canopy_strata->PET = 0.;
	patch->canopy_strata->transpiration_sat_zone = 0.;
	patch->canopy_strata->transpiration_unsat_zone = 0.;

	patch->cap_rise = 0.;
	patch->evaporation_surf_litter = 0.;
	patch->evaporation_surf_soil = 0.;
	patch->exfiltration_unsat_zone = 0.;
	patch->exfiltration_sat_zone = 0.;
	patch->transpiration_sat_zone = 0.;
	patch->transpiration_unsat_zone = 0.;
	patch->snowpack.sublimation = 0.;
	patch->evaporation = 0.;
	patch->gw.Qout = 0.;
	patch->gw.Nout = 0.;


	patch->preday_soil_totalc   = patch->soil_cs.soil1c + patch->soil_cs.soil2c + patch->soil_cs.soil3c + patch->soil_cs.soil4c;
	patch->preday_litter_totalc = patch->litter_cs.litr1c + patch->litter_cs.litr2c + patch->litter_cs.litr3c + patch->litter_cs.litr4c;

	patch->preday_totalc = patch->totalc;

	patch->soil_ns.totaln   = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
	patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
	patch->preday_totaln = patch->totaln;

	patch->soil_ns.preday_totaln   = patch->soil_ns.totaln;
	patch->litter_ns.preday_totaln = patch->litter_ns.totaln;
	patch->soil_ns.preday_nitrate  = patch->soil_ns.nitrate; //NO3
	patch->soil_ns.preday_sminn    = patch->soil_ns.sminn;   //NH4
	patch->preday_NO3_leached      = patch->NO3_leached;

    //---------------------------------------------------------------------------------------------
	// daily carbon fluxes 
    //---------------------------------------------------------------------------------------------
	
	
	
	
	
	
	
	
	
	
	//============================================================================================
	// daily  DOC fluxes 
	cdf->do_litr1c_loss = 0.0;
	cdf->do_litr2c_loss = 0.0;
	cdf->do_litr3c_loss = 0.0;
	cdf->do_litr4c_loss = 0.0;
	cdf->do_soil1c_loss = 0.0;
	cdf->do_soil2c_loss = 0.0;
	cdf->do_soil3c_loss = 0.0;
	cdf->do_soil4c_loss = 0.0;
	cdf->total_DOC_loss = 0.0;

	// daily turnover fluxes 
	cdf->leafc_to_litr1c  = 0.0;
	cdf->leafc_to_litr2c  = 0.0;
	cdf->leafc_to_litr3c  = 0.0;
	cdf->leafc_to_litr4c  = 0.0;
	cdf->frootc_to_litr1c = 0.0;
	cdf->frootc_to_litr2c = 0.0;
	cdf->frootc_to_litr3c = 0.0;
	cdf->frootc_to_litr4c = 0.0;

	// decomposition and heterotroph respiration fluxes 
	cdf->litr1c_hr = 0.0;
	cdf->litr2c_hr = 0.0;
	cdf->litr4c_hr = 0.0;

	cdf->litr1c_to_soil1c = 0.0;
	cdf->litr2c_to_soil2c = 0.0;
	cdf->litr3c_to_litr2c = 0.0;
	cdf->litr4c_to_soil3c = 0.0;

	cdf->soil1c_hr = 0.0;
	cdf->soil2c_hr = 0.0;
	cdf->soil3c_hr = 0.0;
	cdf->soil4c_hr = 0.0;

	cdf->soil1c_to_soil2c = 0.0;
	cdf->soil2c_to_soil3c = 0.0;
	cdf->soil3c_to_soil4c = 0.0;


    //---------------------------------------------------------------------------------------------
	// daily nitrogen fluxes 
    //---------------------------------------------------------------------------------------------
	// daily  DOC fluxes 
	ndf->do_litr1n_loss = 0.0;
	ndf->do_litr2n_loss = 0.0;
	ndf->do_litr3n_loss = 0.0;
	ndf->do_litr4n_loss = 0.0;
	ndf->do_soil1n_loss = 0.0;
	ndf->do_soil2n_loss = 0.0;
	ndf->do_soil3n_loss = 0.0;
	ndf->do_soil4n_loss = 0.0;
	ndf->total_DON_loss = 0.0;
	// daily turnover fluxes 
	ndf->leafn_to_litr1n  = 0.0;
	ndf->leafn_to_litr2n  = 0.0;
	ndf->leafn_to_litr3n  = 0.0;
	ndf->leafn_to_litr4n  = 0.0;
	ndf->frootn_to_litr1n = 0.0;
	ndf->frootn_to_litr2n = 0.0;
	ndf->frootn_to_litr3n = 0.0;
	ndf->frootn_to_litr4n = 0.0;

	// daily N immobilization & mineralization fluxes 
	ndf->litr1n_to_soil1n        = 0.0;
	ndf->sminn_to_soil1n_l1      = 0.0;
	ndf->litr2n_to_soil2n        = 0.0;
	ndf->sminn_to_soil2n_l2      = 0.0;
	ndf->litr3n_to_litr2n        = 0.0;
	ndf->litr4n_to_soil3n        = 0.0;
	ndf->sminn_to_soil3n_l4      = 0.0;
	ndf->soil1n_to_soil2n        = 0.0;
	ndf->sminn_to_soil2n_s1      = 0.0;
	ndf->soil2n_to_soil3n        = 0.0;
	ndf->sminn_to_soil3n_s2      = 0.0;
	ndf->soil3n_to_soil4n        = 0.0;
	ndf->sminn_to_soil4n_s3      = 0.0;
	ndf->soil4n_to_sminn         = 0.0;
	ndf->sminn_to_npool          = 0.0;
	ndf->plant_potential_ndemand = 0.0;
	// deposition, fixation and leaching fluxes 
	ndf->nfix_to_sminn    = 0.0;
	ndf->ndep_to_sminn    = 0.0;

	// denitrification fluxes 
	ndf->sminn_to_nvol    = 0.0;
	ndf->denitrif         = 0.0;
	ndf->sminn_to_nitrate = 0.0;
	ndf->N_to_gw          = 0.0;
	
    //---------------------------------------------------------------------------------------------
	//ndf->sminn_to_nvol_l1s1 = 0.0;
	//ndf->sminn_to_nvol_l2s2 = 0.0;
	//ndf->sminn_to_nvol_l4s3 = 0.0;
	//ndf->sminn_to_nvol_s1s2 = 0.0;
	//ndf->sminn_to_nvol_s2s3 = 0.0;
	//ndf->sminn_to_nvol_s3s4 = 0.0;
	//ndf->sminn_to_nvol_s4   = 0.0;
    //---------------------------------------------------------------------------------------------

	patch->ndf.preday_sminn_to_npool = patch->ndf.sminn_to_npool;
	return ;
}// end zero_patch_daily_flux 





