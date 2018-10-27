#include <stdio.h>
#include "Functions.h"
#include "CHESS.h"

void zero_patch_object(struct patch_object *patch) {

	//---------------------------------------------------------------------------------------------
	// 1.. inter-Patch fluxes					
	//---------------------------------------------------------------------------------------------
	patch->detention_store_preday = patch->detention_store;
	patch->rz_storage_preday = patch->rz_storage;
	patch->unsat_storage_preday = patch->unsat_storage;
	patch->sat_deficit_preday = patch->sat_deficit;


	patch->subsurface_Qout = 0.0;
	patch->subsurface_Qin = 0.0;
	patch->surface_Qout = 0.0;
	patch->surface_Qin = 0.0;
	patch->gw_drainage = 0.0;
	patch->gw.Qout = 0.0;
	patch->gw.Nout = 0.0;
	patch->infiltration_excess = 0.0;

	patch->soil_ns.leach = 0.0;

	patch->surface_NO3 = 0.0;
	patch->surface_NH4 = 0.0;
	patch->NO3_leached = 0.0;
	/* kg/m2/day 	*/

	patch->subsurface_leach_NO3_out = 0.;   /* kg/m2/day 	*/
	patch->surface_leach_NO3_out = 0.;      /* kg/m2/day 	*/
	patch->subsurface_leach_NH4_out = 0.;   /* kg/m2/day 	*/
	patch->surface_leach_NH4_out = 0.;      /* kg/m2/day 	*/
	patch->DON_loss = 0.0;
	patch->DOC_loss = 0.0;

	//---------------------------------------------------------------------------------------------
	// 2.. Canopy  fluxes					
	//---------------------------------------------------------------------------------------------
	patch->canopy_strata->cdf.net_psn = 0.0;
	patch->canopy_strata->cdf.cpool_deadcroot_gr = 0.0;
	patch->canopy_strata->cdf.cpool_deadcroot_gr = 0.0;
	patch->canopy_strata->cdf.cpool_froot_gr = 0.0;
	patch->canopy_strata->cdf.cpool_leaf_gr = 0.0;
	patch->canopy_strata->cdf.cpool_livecroot_gr = 0.0;
	patch->canopy_strata->cdf.cpool_livestem_gr = 0.0;
	patch->canopy_strata->cdf.total_gr = 0.0;

	patch->canopy_strata->cdf.froot_mr = 0.0;
	patch->canopy_strata->cdf.leaf_day_mr = 0.0;
	patch->canopy_strata->cdf.leaf_night_mr = 0.0;
	patch->canopy_strata->cdf.livecroot_mr = 0.0;
	patch->canopy_strata->cdf.livestem_mr = 0.0;
	patch->canopy_strata->cdf.total_mr = 0.0;

	patch->canopy_strata->cdf.psn_to_cpool = 0.0;
	patch->canopy_strata->cdf.potential_psn_to_cpool = 0.0;

	patch->canopy_strata->cdf.transfer_deadcroot_gr = 0.0;
	patch->canopy_strata->cdf.transfer_deadstem_gr = 0.0;
	patch->canopy_strata->cdf.transfer_froot_gr = 0.0;
	patch->canopy_strata->cdf.transfer_gr = 0.0;
	patch->canopy_strata->cdf.transfer_leaf_gr = 0.0;
	patch->canopy_strata->cdf.transfer_livecroot_gr = 0.0;
	patch->canopy_strata->cdf.transfer_livestem_gr = 0.0;

	patch->canopy_strata->cdf.cpool_to_leafc = 0;
	patch->canopy_strata->cdf.cpool_to_frootc = 0;
	patch->canopy_strata->cdf.leafc_transfer_to_leafc = 0;
	patch->canopy_strata->cdf.frootc_transfer_to_frootc = 0;

	patch->canopy_strata->cdf.cpool_to_livestemc = 0;
	patch->canopy_strata->cdf.cpool_to_deadstemc = 0;
	patch->canopy_strata->cdf.cpool_to_livecrootc = 0;
	patch->canopy_strata->cdf.cpool_to_deadcrootc = 0;
	patch->canopy_strata->cdf.livestemc_transfer_to_livestemc = 0;
	patch->canopy_strata->cdf.deadstemc_transfer_to_deadstemc = 0;
	patch->canopy_strata->cdf.livecrootc_transfer_to_livecrootc = 0;
	patch->canopy_strata->cdf.deadcrootc_transfer_to_deadcrootc = 0;


	patch->assim_shade = 0;
	patch->assim_sunlight = 0.;
	patch->canopy_strata->APAR_diffuse = 0.;
	patch->canopy_strata->APAR_direct = 0.;
	patch->canopy_strata->Kstar_diffuse = 0.;
	patch->canopy_strata->Kstar_direct = 0.;
	patch->PAR_diffuse = 0.;
	patch->PAR_direct = 0.;
	patch->Kdown_diffuse = 0.;
	patch->Kdown_direct = 0.;


	//---------------------------------------------------------------------------------------------
	// 3.. daily hydro  fluxes					
	//---------------------------------------------------------------------------------------------
	
	//pre data for checking water balance
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



	//---------------------------------------------------------------------------------------------
	// Soil, litter fluxes					
	//---------------------------------------------------------------------------------------------
	patch->preday_soil_totalc = patch->soil_cs.soil1c + patch->soil_cs.soil2c + patch->soil_cs.soil3c + patch->soil_cs.soil4c;
	patch->preday_litter_totalc = patch->litter_cs.litr1c + patch->litter_cs.litr2c + patch->litter_cs.litr3c + patch->litter_cs.litr4c;

	patch->preday_totalc = patch->totalc;

	patch->soil_ns.totaln = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
	patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
	patch->preday_totaln = patch->totaln;

	patch->soil_ns.preday_totaln = patch->soil_ns.totaln;
	patch->litter_ns.preday_totaln = patch->litter_ns.totaln;
	patch->soil_ns.preday_nitrate = patch->soil_ns.nitrate; //NO3
	patch->soil_ns.preday_sminn = patch->soil_ns.sminn;   //NH4
	patch->preday_NO3_leached = patch->NO3_leached;


	//---------------------------------------------------------------------------------------------
	// Carbon and Nitrogen fluxes					
	//---------------------------------------------------------------------------------------------
	// daily  DOC fluxes 
	patch->cdf.do_litr1c_loss = 0.0;
	patch->cdf.do_litr2c_loss = 0.0;
	patch->cdf.do_litr3c_loss = 0.0;
	patch->cdf.do_litr4c_loss = 0.0;
	patch->cdf.do_soil1c_loss = 0.0;
	patch->cdf.do_soil2c_loss = 0.0;
	patch->cdf.do_soil3c_loss = 0.0;
	patch->cdf.do_soil4c_loss = 0.0;
	patch->cdf.total_DOC_loss = 0.0;

	// daily turnover fluxes 
	patch->cdf.leafc_to_litr1c = 0.0;
	patch->cdf.leafc_to_litr2c = 0.0;
	patch->cdf.leafc_to_litr3c = 0.0;
	patch->cdf.leafc_to_litr4c = 0.0;
	patch->cdf.frootc_to_litr1c = 0.0;
	patch->cdf.frootc_to_litr2c = 0.0;
	patch->cdf.frootc_to_litr3c = 0.0;
	patch->cdf.frootc_to_litr4c = 0.0;

	// decomposition and heterotroph respiration fluxes 
	patch->cdf.litr1c_hr = 0.0;
	patch->cdf.litr2c_hr = 0.0;
	patch->cdf.litr4c_hr = 0.0;

	patch->cdf.litr1c_to_soil1c = 0.0;
	patch->cdf.litr2c_to_soil2c = 0.0;
	patch->cdf.litr3c_to_litr2c = 0.0;
	patch->cdf.litr4c_to_soil3c = 0.0;

	patch->cdf.soil1c_hr = 0.0;
	patch->cdf.soil2c_hr = 0.0;
	patch->cdf.soil3c_hr = 0.0;
	patch->cdf.soil4c_hr = 0.0;

	patch->cdf.soil1c_to_soil2c = 0.0;
	patch->cdf.soil2c_to_soil3c = 0.0;
	patch->cdf.soil3c_to_soil4c = 0.0;


	//---------------------------------------------------------------------------------------------
	// daily nitrogen fluxes 
	//---------------------------------------------------------------------------------------------
	// daily  DOC fluxes 
	patch->ndf.do_litr1n_loss = 0.0;
	patch->ndf.do_litr2n_loss = 0.0;
	patch->ndf.do_litr3n_loss = 0.0;
	patch->ndf.do_litr4n_loss = 0.0;
	patch->ndf.do_soil1n_loss = 0.0;
	patch->ndf.do_soil2n_loss = 0.0;
	patch->ndf.do_soil3n_loss = 0.0;
	patch->ndf.do_soil4n_loss = 0.0;
	patch->ndf.total_DON_loss = 0.0;
	// daily turnover fluxes 
	patch->ndf.leafn_to_litr1n = 0.0;
	patch->ndf.leafn_to_litr2n = 0.0;
	patch->ndf.leafn_to_litr3n = 0.0;
	patch->ndf.leafn_to_litr4n = 0.0;
	patch->ndf.frootn_to_litr1n = 0.0;
	patch->ndf.frootn_to_litr2n = 0.0;
	patch->ndf.frootn_to_litr3n = 0.0;
	patch->ndf.frootn_to_litr4n = 0.0;

	// daily N immobilization & mineralization fluxes 
	patch->ndf.litr1n_to_soil1n = 0.0;
	patch->ndf.sminn_to_soil1n_l1 = 0.0;
	patch->ndf.litr2n_to_soil2n = 0.0;
	patch->ndf.sminn_to_soil2n_l2 = 0.0;
	patch->ndf.litr3n_to_litr2n = 0.0;
	patch->ndf.litr4n_to_soil3n = 0.0;
	patch->ndf.sminn_to_soil3n_l4 = 0.0;
	patch->ndf.soil1n_to_soil2n = 0.0;
	patch->ndf.sminn_to_soil2n_s1 = 0.0;
	patch->ndf.soil2n_to_soil3n = 0.0;
	patch->ndf.sminn_to_soil3n_s2 = 0.0;
	patch->ndf.soil3n_to_soil4n = 0.0;
	patch->ndf.sminn_to_soil4n_s3 = 0.0;
	patch->ndf.soil4n_to_sminn = 0.0;
	patch->ndf.sminn_to_npool = 0.0;
	patch->ndf.plant_potential_ndemand = 0.0;
	// deposition, fixation and leaching fluxes 
	patch->ndf.nfix_to_sminn = 0.0;
	patch->ndf.ndep_to_sminn = 0.0;

	// denitrification fluxes 
	patch->ndf.sminn_to_nvol = 0.0;
	patch->ndf.denitrif = 0.0;
	patch->ndf.sminn_to_nitrate = 0.0;
	patch->ndf.N_to_gw = 0.0;
	patch->ndf.preday_sminn_to_npool = patch->ndf.sminn_to_npool;

	return;
}