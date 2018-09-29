#include <stdio.h>
#include "Functions.h"
#include "CHESS.h"

void zero_patch_object(struct patch_object *patch){

	
		
		patch->detention_store_preday    = patch->detention_store; 
		patch->rz_storage_preday         = patch->rz_storage;     
		patch->unsat_storage_preday      = patch->unsat_storage; 
		patch->sat_deficit_preday        = patch->sat_deficit;

		//patch->streamflow           = 0.0;
		//patch->base_flow            = 0.0;
		patch->subsurface_Qout      = 0.0;
		patch->subsurface_Qin       = 0.0;
		patch->surface_Qout         = 0.0;
		patch->surface_Qin          = 0.0;
		patch->gw_drainage          = 0.0;
		patch->gw.Qout              = 0.0;
		patch->gw.Nout              = 0.0;
		patch->infiltration_excess  = 0.0;

		patch->soil_ns.leach        = 0.0;

		patch->surface_NO3          = 0.0;
		patch->surface_NH4          = 0.0;
		patch->NO3_leached          = 0.0;
		/* kg/m2/day 	*/
	
		patch->subsurface_leach_NO3_out=0.;   /* kg/m2/day 	*/
		patch->surface_leach_NO3_out=0.;      /* kg/m2/day 	*/
		patch->subsurface_leach_NH4_out=0.;   /* kg/m2/day 	*/
		patch->surface_leach_NH4_out=0.;      /* kg/m2/day 	*/
		patch->DON_loss = 0.0;
		patch->DOC_loss = 0.0;
		////////////////////////////////////////////
		patch->canopy_strata->cdf.net_psn = 0.0;
		patch->canopy_strata->cdf.cpool_deadcroot_gr=0.0;
		patch->canopy_strata->cdf.cpool_deadcroot_gr=0.0;
		patch->canopy_strata->cdf.cpool_froot_gr=0.0;
		patch->canopy_strata->cdf.cpool_leaf_gr=0.0;
		patch->canopy_strata->cdf.cpool_livecroot_gr=0.0;
		patch->canopy_strata->cdf.cpool_livestem_gr=0.0;
		patch->canopy_strata->cdf.total_gr=0.0;

		patch->canopy_strata->cdf.froot_mr=0.0;
		patch->canopy_strata->cdf.leaf_day_mr=0.0;
		patch->canopy_strata->cdf.leaf_night_mr=0.0;
		patch->canopy_strata->cdf.livecroot_mr=0.0;
		patch->canopy_strata->cdf.livestem_mr=0.0;
		patch->canopy_strata->cdf.total_mr=0.0;

		patch->canopy_strata->cdf.psn_to_cpool=0.0;
		patch->canopy_strata->cdf.potential_psn_to_cpool=0.0;

		patch->canopy_strata->cdf.transfer_deadcroot_gr=0.0;
		patch->canopy_strata->cdf.transfer_deadstem_gr=0.0;
		patch->canopy_strata->cdf.transfer_froot_gr=0.0;
		patch->canopy_strata->cdf.transfer_gr=0.0;
		patch->canopy_strata->cdf.transfer_leaf_gr=0.0;
		patch->canopy_strata->cdf.transfer_livecroot_gr=0.0;
		patch->canopy_strata->cdf.transfer_livestem_gr=0.0;

		//=================================================================
		//double check if we need initialize these variables
		patch->canopy_strata->cdf.cpool_to_leafc =0;
		patch->canopy_strata->cdf.cpool_to_frootc=0;
		patch->canopy_strata->cdf.leafc_transfer_to_leafc =0;
		patch->canopy_strata->cdf.frootc_transfer_to_frootc=0;

		patch->canopy_strata->cdf.cpool_to_livestemc = 0;
		patch->canopy_strata->cdf.cpool_to_deadstemc = 0;
		patch->canopy_strata->cdf.cpool_to_livecrootc =0;
		patch->canopy_strata->cdf.cpool_to_deadcrootc =0;
		patch->canopy_strata->cdf.livestemc_transfer_to_livestemc =0;
		patch->canopy_strata->cdf.deadstemc_transfer_to_deadstemc =0;
		patch->canopy_strata->cdf.livecrootc_transfer_to_livecrootc=0;
		patch->canopy_strata->cdf.deadcrootc_transfer_to_deadcrootc=0;


		patch->assim_shade = 0;
		patch->assim_sunlight = 0.;
		patch->canopy_strata->APAR_diffuse = 0.;
		patch->canopy_strata->APAR_direct = 0.;
		patch->canopy_strata->Kstar_diffuse=0.;
		patch->canopy_strata->Kstar_direct = 0.;
		patch->PAR_diffuse = 0.;
		patch->PAR_direct = 0.;
		patch->Kdown_diffuse = 0.;
		patch->Kdown_direct = 0.;

	
	
	return;
}