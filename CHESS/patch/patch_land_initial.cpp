
#include <iostream>
#include <math.h>
#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"

using namespace std;

void		patch_land_initial(struct patch_object *patch,
	struct command_line_object *command_line,
	struct	date current_date)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//  Local variable definition.                                  
	//---------------------------------------------------------------------------------------------------------------------------
	int	layer, stratum;
	double	rzm;
	struct  canopy_strata_object *strata;


	//---------------------------------------------------------------------------------------------------------------------------
	//  Compute potential saturation for rootzone layer
	//---------------------------------------------------------------------------------------------------------------------------			
	if (patch->rootzone.depth > ZERO)
		patch->rootzone.potential_sat = compute_delta_water(
			command_line->verbose_flag,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->soil_depth,
			patch->rootzone.depth, 0.0);
	else patch->rootzone.potential_sat = 0.0;

	//---------------------------------------------------------------------------------------------------------------------------
	//	Note: before computing new field capacity, it should always recompute the water table depth first. The compute rootzone.field
	//  capacity and field capacity on the basis of water table depth ---- comment from Guoping
	//---------------------------------------------------------------------------------------------------------------------------
	patch->sat_deficit_z = compute_z_final(
		command_line->verbose_flag,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		0.0,
		-1.0 * patch->sat_deficit);


	if (patch->sat_deficit_z < patch->rootzone.depth) {
		patch->rootzone.field_capacity = compute_layer_field_capacity(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->psi_air_entry,
			patch->soil_defaults->pore_size_index,
			patch->soil_defaults->p3,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->sat_deficit,
			patch->sat_deficit_z, 0.0);

		// =0??
		patch->field_capacity = 0.0;
	}
	else {
		patch->rootzone.field_capacity = compute_layer_field_capacity(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->psi_air_entry,
			patch->soil_defaults->pore_size_index,
			patch->soil_defaults->p3,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->sat_deficit,
			patch->rootzone.depth, 0.0);

		patch->field_capacity = compute_layer_field_capacity(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->psi_air_entry,
			patch->soil_defaults->pore_size_index,
			patch->soil_defaults->p3,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->sat_deficit,
			patch->sat_deficit_z, 0.0) - patch->rootzone.field_capacity;
	}


	//---------------------------------------------------------------------------------------------------------------------------
	//	Estimate potential cap rise. limited to water in sat zone.	
	//---------------------------------------------------------------------------------------------------------------------------
	patch->potential_cap_rise = compute_capillary_rise(
		command_line->verbose_flag,
		patch->sat_deficit_z,
		patch->soil_defaults->psi_air_entry,
		patch->soil_defaults->pore_size_index,
		patch->soil_defaults->m_z,
		patch->soil_defaults->Ksat_0);

	if (patch->potential_cap_rise < ZERO)
		patch->potential_cap_rise = 0.0;


	//---------------------------------------------------------------------------------------------------------------------------
	//	Compute the max exfiltration rate. 
	//  First check if we are saturated.  If so the potential exfiltration rate is the capillary rise rate.
	//	If we are within the active unsat zone then we assume that the potential exfiltration rate is the minimum of
	//	the computed exfiltration rate and the potential caprise - i.e. hydrologic connectivity between surface and	water table.
	//---------------------------------------------------------------------------------------------------------------------------
	if (patch->sat_deficit_z <= patch->soil_defaults->psi_air_entry) {
		patch->potential_exfiltration = patch->potential_cap_rise;
	}
	else {
		if (patch->soil_defaults->active_zone_z < patch->sat_deficit_z) {
			//--------------------------------------------------------------
			//	Estimate potential exfiltration from active zone 
			//--------------------------------------------------------------
			patch->potential_exfiltration = compute_potential_exfiltration(
				command_line->verbose_flag,
				patch->rootzone.S,
				patch->soil_defaults->active_zone_z,
				patch->soil_defaults->Ksat_0,
				patch->soil_defaults->m_z,
				patch->soil_defaults->psi_air_entry,
				patch->soil_defaults->pore_size_index,
				patch->soil_defaults->porosity_decay,
				patch->soil_defaults->porosity_0);
		}
		else {
			//--------------------------------------------------------------
			//	Estimate potential exfiltration from active zone
			//--------------------------------------------------------------
			patch->potential_exfiltration = compute_potential_exfiltration(
				command_line->verbose_flag,
				patch->rootzone.S,
				patch->sat_deficit_z,
				patch->soil_defaults->Ksat_0,
				patch->soil_defaults->m_z,
				patch->soil_defaults->psi_air_entry,
				patch->soil_defaults->pore_size_index,
				patch->soil_defaults->porosity_decay,
				patch->soil_defaults->porosity_0);
		}
	}

	/*
	patch->soil_ns.totaln = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
	patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
	if ((patch->soil_ns.preday_totaln - patch->soil_ns.totaln)> 0.00000001 || (patch->soil_ns.preday_totaln - patch->soil_ns.totaln)<-0.00000001){
		printf("00 patch->soil_ns.preday_totaln is %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->soil_ns.totaln);
		getchar();

	}
	if ((patch->litter_ns.preday_totaln - patch->litter_ns.totaln)> 0.00000001 || (patch->litter_ns.preday_totaln - patch->litter_ns.totaln)<-0.00000001){
		printf("00 patch->litter_ns.preday_totaln is %12.8f %12.8f \n", patch->litter_ns.preday_totaln, patch->litter_ns.totaln);
		getchar();
	}
	*/
	//---------------------------------------------------------------------------------------------------------------------------
	//	Cycle through the canopy layers.			
	//---------------------------------------------------------------------------------------------------------------------------
	for (layer = 0; layer < patch->num_layers; layer++) {
		//---------------------------------------------------------------------------
		//	Cycle through the canopy strata
		//---------------------------------------------------------------------------
		for (stratum = 0; stratum < patch->layers[layer].count; stratum++) {
			//strata = patch->canopy_strata[(patch->layers[layer].strata[stratum])];
			strata = patch->canopy_strata;

			//following two are uncommented by guoping becasue patch->preday_rain_stored was initialized before this statement
			//patch->preday_rain_stored += strata->cover_fraction * strata->rain_stored;  
			//patch->preday_snow_stored += strata->cover_fraction * strata->snow_stored;
			canopy_stratum_daily_I(
				patch,
				//patch->canopy_strata[(patch->layers[layer].strata[stratum])],
				patch->canopy_strata,
				command_line,
				current_date);
		}
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	Calculate effective patch lai from stratum					
	//	Accumulate root biomass for patch soil - required for N updake from soil                        
	//	also determine total plant carbon if grow option is specified
	//---------------------------------------------------------------------------------------------------------------------------
	patch->soil_cs.frootc = 0.0;

	for (stratum = 0; stratum < patch->num_canopy_strata; stratum++) {
		if (command_line->grow_flag > 0) {
			patch->soil_cs.frootc += patch->canopy_strata->cover_fraction * patch->canopy_strata->cs.frootc;
			//patch->preday_totalc  += patch->canopy_strata->cover_fraction * patch->canopy_strata->cs.preday_totalc;
			//patch->preday_totaln  += patch->canopy_strata->cover_fraction * patch->canopy_strata->ns.preday_totaln;
		}
		patch->rootzone.depth = max(patch->rootzone.depth, patch->canopy_strata->rootzone.depth);
	}


	//printf("Soil N balance is %12.8f %12.8f \n", patch->preday_totaln - patch->totaln, patch->canopy_strata->cover_fraction * patch->canopy_strata->ns.preday_totaln);
	//getchar();

	//---------------------------------------------------------------------------------------------------------------------------
	//	re-sort patch layers to account for any changes in 	height	
	//---------------------------------------------------------------------------------------------------------------------------
	sort_patch_layers(patch);

	//---------------------------------------------------------------------------------------------------------------------------
	//	compute current soil moisture potential and do this before nitrogen updake occurs later in the day
	//---------------------------------------------------------------------------------------------------------------------------
	if (patch->sat_deficit > patch->rootzone.potential_sat)
		rzm = patch->rz_storage;
	else
		rzm = patch->rz_storage + patch->rootzone.potential_sat - patch->sat_deficit;

	patch->psi = compute_soil_water_potential(
		command_line->verbose_flag,
		patch->soil_defaults->theta_psi_curve,
		patch->soil_defaults->psi_air_entry,
		patch->soil_defaults->pore_size_index,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->psi_max,
		patch->rootzone.potential_sat,
		rzm,
		patch->metv.tsoil);


	if (command_line->grow_flag > 0) {
		update_litter_interception_capacity(patch->litter.moist_coef, &(patch->litter_cs), &(patch->litter));
	}

	compute_potential_decomp(patch->metv.tsoil, patch->soil_defaults->psi_max, patch->soil_defaults->psi_air_entry, patch->rootzone.S,
		patch->std, &(patch->soil_cs), &(patch->soil_ns), &(patch->litter_cs), &(patch->litter_ns), &(patch->cdf), &(patch->ndf));


	return;
}
