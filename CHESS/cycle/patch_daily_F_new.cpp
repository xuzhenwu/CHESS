//------------------------------------------------------------------------------------------------------------------------------
//				 		patch_daily_F
//	PROGRAMMER NOTES
//
//	Sep 3 1997
//	Took out condition that strata under snowpack	must be at z>0
//
//	Sept 29 1997 CT
//	unsat drainage and cap rise are now based on actual depth to water table which is
//	determined by assuming a porosity decay with depth;  transpiration is now sep.
//	into sat_zone and unsat_zone trans
//
//	Oct 22 1997 CT
//	included calculation of water balance
//
//	Oct 22 1997 CT
//	canopy_strata sublimation now added to patch level sum of canopy evaporation
//	added check_zero_stores
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <math.h>
#include "CHESS.h"
#include "Constants.h"
#include <iostream>
using namespace std;


void		patch_daily_F(struct 	patch_object *patch,
						  struct 	command_line_object *command_line,
						  struct	date 			current_date)
{
	//--------------------------------------------------------------------------------------------------------------------------
	//	Local Function Declarations.
	//--------------------------------------------------------------------------------------------------------------------------
	void zone_daily_F(struct patch_object *,struct command_line_object *,struct date);
	void compute_Lstar(struct patch_object *);
	double compute_delta_water(int, double, double,	double, double, double);


	double compute_layer_field_capacity(
		int,
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);

	void canopy_stratum_daily_F(
		struct patch_object *,
		struct layer_object *,
		struct canopy_strata_object *,
		struct command_line_object *,
		struct date);

	void   surface_daily_F(
		struct patch_object *,
		struct command_line_object *,
		struct date);

	double	snowpack_daily_F (
		struct date,
		int,
		struct	snowpack_object	*,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double *,
		double *,
		double *,
		double *,
		double,
		double,
		double,
		double,
		double);

	double	compute_infiltration(
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);

	double  compute_surface_heat_flux(
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);

	double	compute_unsat_zone_drainage(
		int,
		int,
		double,
		double,
		double,
		double,
		double,
		double);

	double  compute_radiative_fluxes(
		int,
		double *,
		double,
		double,
		double);


//--------------------------------------------
//		double  compute_stability_correction(
//		int ,
//		double,
//		double,
//		double,
//		double,
//		double,
//		double);
//	----------------------------------------

	double  compute_z_final(
		int,
		double,
		double,
		double,
		double,
		double);

	void 	check_zero_stores(
		struct  soil_c_object   *,
		struct  soil_n_object   *,
		struct  litter_c_object *,
		struct  litter_n_object *
		);

	void	update_decomp(
		struct	date,
		struct  soil_c_object   *,
		struct  soil_n_object   *,
		struct  litter_c_object *,
		struct  litter_n_object *,
		struct cdayflux_patch_struct *,
		struct ndayflux_patch_struct *,
		struct patch_object *);

	void	update_dissolved_organic_losses(
		struct	date,
		double,
		struct  soil_c_object   *,
		struct  soil_n_object   *,
		struct  litter_c_object *,
		struct  litter_n_object *,
		struct cdayflux_patch_struct *,
		struct ndayflux_patch_struct *);

	void	update_septic(
		struct	date,
		struct  patch_object   *);

	void	update_nitrif(
		struct  soil_c_object   *,
		struct  soil_n_object   *,
		struct cdayflux_patch_struct *,
		struct ndayflux_patch_struct *,
		struct soil_class,
		double,
		double,
		double,
		double,
		double,
		double);

	void	update_denitrif(
		struct  soil_c_object   *,
		struct  soil_n_object   *,
		struct cdayflux_patch_struct *,
		struct ndayflux_patch_struct *,
		struct soil_class,
		double,
		double);

	int	resolve_sminn_competition(
		struct  soil_n_object   *,
		double, double,
		struct ndayflux_patch_struct *);

	void   canopy_stratum_growth(
		struct patch_object *,
		struct canopy_strata_object *,
		struct command_line_object *,
		struct date);

	void update_gw_drainage(
			struct patch_object *,
			struct command_line_object *,
			struct date);

	long julday( struct date);

	void compute_ground_water_loss(
			struct	patch_object *,
			struct 	command_line_object *,
			struct 	date );

	//--------------------------------------------------------------------------------------------------------------------------
	//  Local variable definition.
	//--------------------------------------------------------------------------------------------------------------------------
	int	    layer;
	int	    vegtype;
	double	cap_rise; //, tmp, wilting_point
	double	delta_unsat_zone_storage;
	double  detention_store_evaporation;
	double  infiltration;
	double	net_inflow;
	double	preday_snowpack_height=0;
	double	sat_zone_patch_demand;
	double	sat_zone_patch_demand_initial;
	double	unsat_zone_patch_demand;
	double	unsat_zone_patch_demand_initial;

	double	available_sat_water;
	double	temp; //,scale

	double  add_field_capacity;
	double	water_above_field_cap;
	double	water_below_field_cap;
	double 	duration, irrigation;
	double  fertilizer_NO3, fertilizer_NH4=0.;
	double	resp;
	double 	surfaceN_to_soil=0.;
	double	FERT_TO_SOIL;
	struct	canopy_strata_object	*strata;
	double  water_balance,nitrate_balance,NH4_balance;


	//---------------------------------------------------------------------------------------------------------------------------
	// Zone_daily_F
	// Daylength, Radiation, Rain_duration
	//---------------------------------------------------------------------------------------------------------------------------
	zone_daily_F(patch,command_line,current_date);
	
	//--------------------------------------------------------------------------------------------------------------------------
	//	Set the patch rain and snow throughfall equivalent to the rain and snow coming down over the zone.
	//--------------------------------------------------------------------------------------------------------------------------
	patch->rain_throughfall = patch->rain;
	patch->snow_throughfall = patch->snow;
	patch->wind = patch->wind;//xu. no means

	//--------------------------------------------------------------------------------------------------------------------------
	//	Add these septic loads for unsat_storage and soil_ns.nitrate
	//--------------------------------------------------------------------------------------------------------------------------
	if ((patch->landuse_defaults->septic_water_load > ZERO)	|| (patch->landuse_defaults->septic_NO3_load > ZERO)) {
		update_septic(current_date, patch);
	}
	//--------------------------------------------------------------------------------------------------------------------------
	//	Compute the stability correction factor for aero cond
	//--------------------------------------------------------------------------------------------------------------------------
	patch->stability_correction = 1.0;

	//--------------------------------------------------------------------------------------------------------------------------
	//  Determine patch SOIL heat flux. This is ignored if there is a 0 height stratum.
	//--------------------------------------------------------------------------------------------------------------------------
	patch->surface_heat_flux = -1 * compute_surface_heat_flux(
		command_line->verbose_flag,
		patch->snow_stored,
		patch->unsat_storage,
		patch->sat_deficit,
		patch->metv.tavg,
		patch->metv.tnightmax,
		patch->metv.tsoil,
		patch->soil_defaults->deltaz,
		patch->soil_defaults->min_heat_capacity,
		patch->soil_defaults->max_heat_capacity);
	//--------------------------------------------------------------------------------------------------------------------------
	//	Compute patch level long wave radiation processes.
	//--------------------------------------------------------------------------------------------------------------------------
	compute_Lstar(patch);

	//--------------------------------------------------------------------------------------------------------------------------
	//	Cycle through patch layers with height greater than the	snowpack.
	//--------------------------------------------------------------------------------------------------------------------------
	for (layer=0 ; layer<patch->num_layers; layer++ ){
		patch->snowpack.overstory_fraction = 0.0;
		patch->snowpack.overstory_height   = patch->screen_height;
		if ( (patch->layers[layer].height > patch->snowpack.height) && (patch->layers[layer].height > -1 * patch->sat_deficit_z) ){
			patch->snowpack.overstory_fraction = 1.0;
			patch->snowpack.overstory_height   = min(patch->snowpack.overstory_height,patch->layers[layer].height);
			patch->Kdown_direct_final          = patch->layers[layer].null_cover * patch->Kdown_direct;
			patch->Kdown_diffuse_final         = patch->layers[layer].null_cover * patch->Kdown_diffuse;
			patch->PAR_direct_final            = patch->layers[layer].null_cover * patch->PAR_direct;
			patch->PAR_diffuse_final           = patch->layers[layer].null_cover * patch->PAR_diffuse;
			patch->rain_throughfall_final      = patch->layers[layer].null_cover * patch->rain_throughfall;
			patch->snow_throughfall_final      = patch->layers[layer].null_cover * patch->snow_throughfall;
			patch->ga_final                    = patch->layers[layer].null_cover * patch->ga;
			patch->wind_final                  = patch->layers[layer].null_cover * patch->wind;

			patch->rain_throughfall           -= patch->rain_throughfall_final;  //added by guoping for water balance
			patch->snow_throughfall           -= patch->snow_throughfall_final;  //added by guoping for water balance


			//--------------------------------------------------------------
			//		Cycle through the canopy strata in this layer
			//--------------------------------------------------------------
			//for ( stratum=0 ; stratum<patch->layers[layer].count; stratum++ ){
				canopy_stratum_daily_F(
					patch,
					&(patch->layers[layer]),
					//patch->canopy_strata[(patch->layers[layer].strata[stratum])], //by guoping
					patch->canopy_strata,
					command_line,
					current_date );
			//}
			patch->Kdown_direct     = patch->Kdown_direct_final;
			patch->Kdown_diffuse    = patch->Kdown_diffuse_final;
			patch->PAR_direct       = patch->PAR_direct_final;
			patch->PAR_diffuse      = patch->PAR_diffuse_final;
			patch->rain_throughfall = patch->rain_throughfall_final;
			patch->snow_throughfall = patch->snow_throughfall_final;
			patch->ga               = patch->ga_final;
			patch->wind             = patch->wind_final;
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------
	//	We assume the snowpack is conceptually over the	current ponded water.
	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------
	//	Now add the throughfall of snow	to the snowpack	rain is added to the snowpack if it exists and snowpack melt allowed to
	//  occur this means that in rain on snow - rain is included as snowmelt
	//--------------------------------------------------------------------------------------------------------------------------
	preday_snowpack_height                  = patch->snowpack.height;
	patch->snowpack.water_equivalent_depth  = patch->preday_snowpack + patch->snow_throughfall;

	if (patch->snow_throughfall > 0.0 )
		patch->snowpack.surface_age = 0.0;
	if ( patch->snowpack.water_equivalent_depth > ZERO ) {
		//--------------------------------------------------------------
		//	Calculate snowmelt
		//--------------------------------------------------------------
		patch->snow_melt = snowpack_daily_F(
			current_date,
			command_line->verbose_flag,
			&patch->snowpack,
			patch->metv.tavg,
			patch->e_dewpoint,
			patch->wind,
			patch->metv.pa,
			patch->cloud_fraction,
			patch->rain_throughfall,
			patch->snow_throughfall,
			&patch->Kdown_direct,
			&patch->Kdown_diffuse,
			&patch->PAR_direct,
			&patch->PAR_diffuse,
			patch->patch_defaults->maximum_snow_energy_deficit,
			patch->patch_defaults->snow_water_capacity,
			patch->patch_defaults->snow_light_ext_coef,
			patch->patch_defaults->snow_melt_Tcoef,
			patch->Lstar_snow);

		patch->rain_throughfall += patch->snow_melt;
		patch->snow_throughfall  = 0.0;
		//patch->snowpack.water_equivalent_depth -= patch->snowpack.sublimation;
	}
	else{
		//--------------------------------------------------------------
		//	Just to create symmetrical output for snow and no snow
		//	days we do some fake calls which snowpack_daily_F does
		//--------------------------------------------------------------
		temp = 1;
		temp = compute_radiative_fluxes(0, &patch->Kdown_direct, 0, 0, 1);
		temp = compute_radiative_fluxes(0, &patch->Kdown_diffuse, 0, 0, 1);
		temp = compute_radiative_fluxes(0, &patch->PAR_direct, 0, 0, 1);
		temp = compute_radiative_fluxes(0, &patch->PAR_diffuse, 0, 0, 1);
		patch->snow_melt = 0.0;
		patch->snowpack.energy_deficit = 0.001;
	}

	//----------------------------------------------------------------------------------------------------------------------------
	//	Cycle through patch layers with height less than the snowpack but more than 0. Note that the rain throughfall should
	//  equal the rain and snow melt getting through the snowpack. There should be no snow_throughfall if there is a snow
	//	pack (i.e. only moss layers will have any incident snow throughfall in the loop below). We then conceptually look at it
	//  as a snowpack "HOVERING" above any patch layers lower than its current maximum height. This is fine since if we assume
	//	that the snowpack does not transmit shortwave or par, no vapour fluxes will take place anyways and only respiration will
	//  which we want. Patches under the snowpack but over the pond. need to use previous day (or beginning of the day)
	//	snowpack height to avoid processing some strata	twice
	//----------------------------------------------------------------------------------------------------------------------------

	for ( layer=0 ; layer<patch->num_layers; layer++ ){
		if ( (patch->layers[layer].height <= preday_snowpack_height) &&	(patch->layers[layer].height > -1 * patch->sat_deficit_z) ){

			patch->Kdown_direct_final        = patch->layers[layer].null_cover * patch->Kdown_direct;
			patch->Kdown_diffuse_final       = patch->layers[layer].null_cover * patch->Kdown_diffuse;
			patch->PAR_direct_final          = patch->layers[layer].null_cover * patch->PAR_direct;
			patch->PAR_diffuse_final         = patch->layers[layer].null_cover * patch->PAR_diffuse;
			patch->rain_throughfall_final    = patch->layers[layer].null_cover * patch->rain_throughfall;
			patch->snow_throughfall_final    = patch->layers[layer].null_cover * patch->snow_throughfall;


			patch->ga_final                  = patch->layers[layer].null_cover * patch->ga;
			patch->wind_final                = patch->layers[layer].null_cover * patch->wind;

			patch->rain_throughfall         -= patch->rain_throughfall_final;  //added by guoping for water balance
			patch->snow_throughfall         -= patch->snow_throughfall_final;  //added by guoping for water balance

			//for ( stratum=0 ;stratum<patch->layers[layer].count; stratum++ ){
				canopy_stratum_daily_F(
					patch,
					&(patch->layers[layer]),
					//patch->canopy_strata[(patch->layers[layer].strata[stratum])],
					patch->canopy_strata,
					command_line,
					current_date );
			//}
			patch->Kdown_direct          = patch->Kdown_direct_final;
			patch->Kdown_diffuse         = patch->Kdown_diffuse_final;
			patch->PAR_direct            = patch->PAR_direct_final;
			patch->PAR_diffuse           = patch->PAR_diffuse_final;
			patch->rain_throughfall      = patch->rain_throughfall_final;
			patch->snow_throughfall      = patch->snow_throughfall_final;
			patch->ga                    = patch->ga_final;
			patch->wind                  = patch->wind_final;
		}
	}

	//*
	//--------------------------------------------------------------------------------------------------------------------------
	//	Patches below the pond.
	//--------------------------------------------------------------------------------------------------------------------------
	for ( layer=0 ; layer<patch->num_layers; layer++ ){
		if (patch->layers[layer].height <= -1 * patch->sat_deficit_z){
			patch->Kdown_direct_final     = patch->layers[layer].null_cover * patch->Kdown_direct;
			patch->Kdown_diffuse_final    = patch->layers[layer].null_cover * patch->Kdown_diffuse;
			patch->PAR_direct_final       = patch->layers[layer].null_cover * patch->PAR_direct;
			patch->PAR_diffuse_final      = patch->layers[layer].null_cover * patch->PAR_diffuse;
			patch->rain_throughfall_final = patch->layers[layer].null_cover * patch->rain_throughfall;
			patch->snow_throughfall_final = patch->layers[layer].null_cover * patch->snow_throughfall;
			patch->ga_final               = patch->layers[layer].null_cover * patch->ga;
			patch->wind_final             = patch->layers[layer].null_cover * patch->wind;

			patch->rain_throughfall      -= patch->rain_throughfall_final;  //added by guoping for water balance
			patch->snow_throughfall      -= patch->snow_throughfall_final;  //added by guoping for water balance

			//for ( stratum=0 ; stratum<patch->layers[layer].count; stratum++ ){
				canopy_stratum_daily_F(
					patch,
					&(patch->layers[layer]),
					//patch->canopy_strata[(patch->layers[layer].strata[stratum])],
					patch->canopy_strata,
					command_line,
					current_date );
			//}
			patch->Kdown_direct        = patch->Kdown_direct_final;
			patch->Kdown_diffuse       = patch->Kdown_diffuse_final;
			patch->PAR_direct          = patch->PAR_direct_final;
			patch->PAR_diffuse         = patch->PAR_diffuse_final;
			patch->rain_throughfall    = patch->rain_throughfall_final;
			patch->snow_throughfall    = patch->snow_throughfall_final;
			patch->ga                  = patch->ga_final;
			patch->wind                = patch->wind_final;
		}
	}
	
	//*/
	//--------------------------------------------------------------------------------------------------------------------------
	//	Need to account for snow "throughfall" that is a result	Of leaves dropping, reducing potential interception,
	//	and thus reducing snow storage.	This should be small enough (and generally in the fall) to ignore melting of this snow.
	//--------------------------------------------------------------------------------------------------------------------------
	patch->snowpack.water_equivalent_depth += patch->snow_throughfall;

	//--------------------------------------------------------------------------------------------------------------------------
	//  added in nitrogen deposition will consider N deposition as a concentration in throughfall - at moment
	//	simply added N deposition as a total flux
	//--------------------------------------------------------------------------------------------------------------------------
	fertilizer_NO3         = patch->landuse_defaults->fertilizer_NO3;
	patch->surface_NO3    += patch->ndep_NO3;
	patch->surface_NH4    += patch->ndep_NH4;
	//--------------------------------------------------------------------------------------------------------------------------
	//	a certain amount of surface_N is incorporated into the soil each day - we used 66% based on fertilizer experiments
	//	Agronomy Guide 1989-1990 pg 17, Penn State web site fact sheet
	//--------------------------------------------------------------------------------------------------------------------------

	FERT_TO_SOIL = 100;
	if (patch->fertilizer_NH4 > ZERO) {
		surfaceN_to_soil       = FERT_TO_SOIL * patch->fertilizer_NH4;
		patch->fertilizer_NH4 -= surfaceN_to_soil;
		patch->soil_ns.sminn  += surfaceN_to_soil;
	}

	if (patch->fertilizer_NO3 > ZERO) {
		surfaceN_to_soil = FERT_TO_SOIL * patch->fertilizer_NO3;
		patch->fertilizer_NO3  -= surfaceN_to_soil;
		patch->soil_ns.nitrate += surfaceN_to_soil;
	}

	patch->rain_throughfall += patch->preday_detention_store;
	//--------------------------------------------------------------------------------------------------------------------------
	//	finally process surface or litter layer
	//--------------------------------------------------------------------------------------------------------------------------
	surface_daily_F(patch,command_line,current_date);

	//--------------------------------------------------------------------------------------------------------------------------
	// 	Above ground Hydrologic Processes compute infiltration into the soil from snowmelt or rain_throughfall for now
	//  assume that all water infilatrates and some may be lost to deeper groundwater
	//--------------------------------------------------------------------------------------------------------------------------
	net_inflow = 0.0;
	duration   = 0.0;
	if (patch->rain_throughfall > ZERO) {

		//------------------------------------------------------------------------
		//	drainage to a deeper groundwater store move both nitrogen and water
		//------------------------------------------------------------------------
		if (command_line->gw_flag > 0) {
			update_gw_drainage(patch, command_line, current_date);
		}
		else patch->gw_drainage=0.;

		net_inflow = patch->rain_throughfall;
		//-------------------------------------------------------------------------------------------------------------
		// - if rain duration is zero, then input is from snow melt  assume full daytime duration
		//-------------------------------------------------------------------------------------------------------------
		if (patch->daytime_rain_duration <= ZERO)
			duration = patch->metv.dayl/(86400);
		else
			duration = patch->daytime_rain_duration/(86400);

		if (patch->rootzone.depth > ZERO)	{
	        infiltration = compute_infiltration(
	        	command_line->verbose_flag,
	        	patch->sat_deficit_z,
	        	patch->rootzone.S,
	        	patch->Ksat_vertical,
	        	patch->soil_defaults->Ksat_0,
	        	patch->soil_defaults->m_z,
	        	patch->soil_defaults->porosity_0,
	        	patch->soil_defaults->porosity_decay,
	        	net_inflow,
	        	duration,
	        	patch->soil_defaults->theta_psi_curve);
		}
		else {
	        infiltration = compute_infiltration(
	        	command_line->verbose_flag,
	        	patch->sat_deficit_z,
	        	patch->S,
	        	patch->Ksat_vertical,
	        	patch->soil_defaults->Ksat_0,
	        	patch->soil_defaults->m_z,
	        	patch->soil_defaults->porosity_0,
	        	patch->soil_defaults->porosity_decay,
	        	net_inflow,
	        	duration,
	        	patch->soil_defaults->theta_psi_curve);
		}
	}
	else infiltration = 0.0;

	//patch->rain_throughfall += patch->preday_detention_store;
	compute_ground_water_loss(patch,command_line,current_date); //added by guoping for ground water loss

   	//--------------------------------------------------------------------------------------------------------------------------
	// determine fate of hold infiltration excess in detention store infiltration excess will removed during routing portion
	//--------------------------------------------------------------------------------------------------------------------------
	patch->detention_store = (net_inflow - infiltration);
	patch->rain_throughfall = max(0, patch->rain_throughfall - patch->preday_detention_store);//just have throughfall to make it sense

	//--------------------------------------------------------------------------------------------------------------------------
	// allow evaporation of any detention store
	//--------------------------------------------------------------------------------------------------------------------------
	if(patch->rain+patch->snow<0.002)
		detention_store_evaporation = min(patch->potential_evapotranspiration, min(patch->detention_store, patch->soil_defaults->detention_store_size));
	else
		detention_store_evaporation   =0.;

	patch->evaporation_surf_soil         = detention_store_evaporation;
	patch->potential_evapotranspiration -= detention_store_evaporation;
	patch->detention_store              -= detention_store_evaporation;
	
	patch->preday_NO3 = patch->surface_NO3; //added by guoping for nitrogen balance 
	patch->preday_NH4 = patch->surface_NH4; //added by guoping for nitrogen balance 

	//--------------------------------------------------------------------------------------------------------------------------
	// allow infiltration of surface N
	//--------------------------------------------------------------------------------------------------------------------------
	if ((command_line->grow_flag > 0) && (infiltration > ZERO)) {
		patch->soil_ns.nitrate += infiltration/net_inflow * patch->surface_NO3;
		patch->soil_ns.sminn   += infiltration/net_inflow * patch->surface_NH4;
		patch->surface_NO3     -= infiltration/net_inflow * patch->surface_NO3;
		patch->surface_NH4     -= infiltration/net_inflow * patch->surface_NH4;
	}

	/*
	nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln + patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
		+patch->ndf.preday_sminn_to_npool- patch->soil_ns.totaln - patch->litter_ns.totaln - patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4-patch->ndf.sminn_to_npool);
	if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
		printf("01 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
		printf("02 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
		printf("03 nitrate is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
		printf("04 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
		printf("05 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
		printf("06 nitrate is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
		printf("07 nitrate is not balanced %12.8f \n", patch->ndf.sminn_to_npool);
		getchar();
	}
	*/

	//--------------------------------------------------------------------------------------------------------------------------
	//	Determine if the infifltration will fill up the unsat zone or not.
	//	We use the strict assumption that sat deficit is the amount of water needed to saturate the soil.
	//--------------------------------------------------------------------------------------------------------------------------
 	if ( infiltration > patch->sat_deficit - patch->unsat_storage - patch->rz_storage) {
		//----------------------------------------------------------------------------------------------
		// Yes the unsat zone will be filled so we may as well treat the unsat_storage and infiltration
		// as water added to the water table.
		//----------------------------------------------------------------------------------------------
		patch->sat_deficit = patch->sat_deficit -
			(infiltration + patch->unsat_storage + patch->rz_storage);
		//--------------------------------------------------------------
		// There is no unsat_storage left.
		//--------------------------------------------------------------
		patch->unsat_storage  = 0;
		patch->rz_storage     = 0;
		patch->field_capacity = 0;
		//--------------------------------------------------------------
		// Reverse the cap rise as it likely did not happen
		//--------------------------------------------------------------
		patch->potential_cap_rise += patch->cap_rise;
		patch->cap_rise            = 0;
	}
	else if ((patch->sat_deficit > patch->rootzone.potential_sat) &&
		(infiltration > patch->rootzone.potential_sat - patch->rz_storage)) {
		//------------------------------------------------------------------------------
		// Just add the infiltration to the rz_storage and unsat_storage
		//------------------------------------------------------------------------------
		patch->unsat_storage = patch->unsat_storage + infiltration -
			(patch->rootzone.potential_sat - patch->rz_storage);
		patch->rz_storage    = patch->rootzone.potential_sat;
	}
		// Only rootzone layer saturated - perched water table case
	else if ((patch->sat_deficit > patch->rootzone.potential_sat) &&
		(infiltration <= patch->rootzone.potential_sat - patch->rz_storage)) {
		//-----------------------------------------------------------------------------
		// Just add the infiltration to the rz_storage
		//-----------------------------------------------------------------------------
		patch->rz_storage = patch->rz_storage+infiltration;
	}
	else if ((patch->sat_deficit <= patch->rootzone.potential_sat) &&
		(infiltration <= patch->sat_deficit - patch->rz_storage - patch->unsat_storage)) {
		patch->rz_storage     = patch->rz_storage + patch->unsat_storage; // transfer left water in unsat storage to rootzone layer
		patch->unsat_storage  = 0;
		patch->rz_storage    += infiltration;
		patch->field_capacity = 0;
	}

	//--------------------------------------------------------------------------------------------------------------------------
	//	Calculate patch level transpiration
	//--------------------------------------------------------------------------------------------------------------------------
	patch->transpiration_sat_zone   = 0.0;
	patch->transpiration_unsat_zone = 0.0;
	unsat_zone_patch_demand         = patch->exfiltration_unsat_zone;
	sat_zone_patch_demand           = patch->exfiltration_sat_zone;
	
	patch->evaporation   = patch->snowpack.sublimation + patch->evaporation_surf_litter+patch->evaporation_surf_soil;
	patch->AET           = patch->evaporation;

	patch->rain_stored   = 0.;
	patch->snow_stored   = 0.;
	patch->ndf.plant_potential_ndemand = 0.0;
	patch->net_plant_psn    = 0.0;
	patch->totalc           = 0.0;
	patch->totaln           = 0.0;
	patch->lai              = 0.0;


	for ( layer=0 ; layer<patch->num_layers; layer++ ){
		//---------------------------------------------------------------------------------------------------
		//	Cycle through the canopy strata in this layer. We assume that the stratum already has computed a
		//	unsat_zone and sat_zone transpiration demand based on its rooting depth profile.
		//---------------------------------------------------------------------------------------------------
		//for ( stratum=0 ; stratum<patch->layers[layer].count; stratum++ ){  //uncommented by guoping
			strata =
				//patch->canopy_strata[(patch->layers[layer].strata[stratum])];
				patch->canopy_strata;

			//--------------------------------------------------------------
			//	Add up nitrogen demand
			//--------------------------------------------------------------
			patch->ndf.plant_potential_ndemand += strata->cover_fraction * (strata->ndf.potential_N_uptake);
			//--------------------------------------------------------------
			//	Add up evaporation.
			//--------------------------------------------------------------
			//patch->evaporation += strata->cover_fraction * (strata->evaporation + strata->sublimation) ;
			//patch->AET         += strata->cover_fraction * (strata->evaporation + strata->sublimation) ;

			patch->evaporation += (strata->evaporation + strata->sublimation);
			patch->AET         += (strata->evaporation + strata->sublimation);

			//--------------------------------------------------------------
			// Add up canopy snow and rain stored for water balance.
			//--------------------------------------------------------------
			patch->rain_stored += strata->rain_stored ;
			patch->snow_stored += strata->snow_stored ;

			//--------------------------------------------------------------
			//	Add up transpiration demand
			//  Now unsat_zone_patch_demand=patch->exfiltration_unsat_zone+strata->transpiration_unsat_zone
			//      sat_zone_patch_demand  =patch->exfiltration_sat_zone+strata->transpiration_sat_zone
			//--------------------------------------------------------------
			unsat_zone_patch_demand += strata->cover_fraction * strata->transpiration_unsat_zone;
			sat_zone_patch_demand   += strata->cover_fraction * strata->transpiration_sat_zone;

			//The following is added by Guoping. Otherwise they are always zero
			patch->transpiration_sat_zone   += strata->cover_fraction * strata->transpiration_sat_zone;
			patch->transpiration_unsat_zone += strata->cover_fraction * strata->transpiration_unsat_zone;

		//} //end of for uncommented by guoping
	}

	//--------------------------------------------------------------------------------------------------------------------------
	// 	Fulfill transpiration demands
	//--------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------------
	//	Remember the transpiration demands.
	//--------------------------------------------------------------------------------------------------------------------------
	unsat_zone_patch_demand_initial = unsat_zone_patch_demand;
	sat_zone_patch_demand_initial   = sat_zone_patch_demand;

	//--------------------------------------------------------------------------------------------------------------------------
	//	Figure out how much of the demand for unsat and sat zone is met by supply from the zone.
	//	Update the storages while we are at it.	We do not allow excess unsat storage demand beyond
	//	current storage and what is available through cap rise. Sat zone demand is not strictly limited here,
	//	although it is limited previously in previously by rooting depth/ sat_deficit_z)
	//
	//	this follows because
	//	ii) if the sat_zone demand is high it is because the water table is high - which means there will
	//		is no problem meeting the sat_zone demand
	//	iii) if the unsat_zone demand is high the end of day cap rise will fill up the the unsat zone anyways
	//		(note however that demands over field capacity of the unsat zone will not even be satisfied
	//		by cap rise but thats too bad).
	//
	//	Note that the demands were decided based on water table depths BEFORE infiltration and may bias	demands on unsat zone.
	//  However given that infiltration	is big during a rain and demand small during rain it is likely not a big deal.
	//--------------------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------------------
	//	Note: before computing new field capacity, it should always recompute the water table depth first. The compute rootzone.field
	//  capacity and field capacity dependent on the basis of water table depth ---- comment from Guoping
	//--------------------------------------------------------------------------------------------------------------------------
	patch->sat_deficit_z = compute_z_final(
		command_line->verbose_flag,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		0.0,
		-1.0 * patch->sat_deficit);

	if (patch->sat_deficit_z < patch->rootzone.depth)  {
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
		patch->field_capacity = 0.0;
	}
	else  {
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
	
	available_sat_water = min((compute_delta_water(
		0,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		patch->rootzone.depth,
		patch->sat_deficit_z)), sat_zone_patch_demand);

	available_sat_water = max(available_sat_water, 0.0);
	sat_zone_patch_demand -= available_sat_water;

	//--------------------------------------------------------------------------------------------------------------------------
	// 	leave behind field capacity if sat deficit has been lowered. this should be an interactive process, we will use
	//	0th order approximation. We do not do this once sat def is below 0.9 soil depth, we use 0.9 to prevent numerical instability
	//--------------------------------------------------------------------------------------------------------------------------
	if (available_sat_water > ZERO) {
		add_field_capacity = compute_layer_field_capacity(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->psi_air_entry,
			patch->soil_defaults->pore_size_index,
			patch->soil_defaults->p3,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->sat_deficit,
			patch->sat_deficit,
			patch->sat_deficit - available_sat_water);

		patch->sat_deficit += add_field_capacity;

		if ((patch->sat_deficit_z > patch->rootzone.depth) && (patch->sat_deficit_z > patch->rootzone.depth))
			patch->unsat_storage += add_field_capacity;

		else if ((patch->sat_deficit_z <= patch->rootzone.depth) && (patch->sat_deficit_z <= patch->rootzone.depth))
			patch->rz_storage += add_field_capacity;
		else  {
			patch->rz_storage += add_field_capacity * (patch->rootzone.depth - patch->sat_deficit_z)
				/ (patch->sat_deficit_z - patch->sat_deficit_z);
			patch->unsat_storage += add_field_capacity * (patch->sat_deficit_z - patch->rootzone.depth)
				/ (patch->sat_deficit_z - patch->sat_deficit_z);
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------
	//	See how much of  unsat zone demand can be met and still field capacity.
	//--------------------------------------------------------------------------------------------------------------------------
	water_above_field_cap = max((patch->rz_storage - patch->rootzone.field_capacity), 0);
	water_above_field_cap = min(unsat_zone_patch_demand, water_above_field_cap);
	unsat_zone_patch_demand -= water_above_field_cap;


	if (patch->sat_deficit_z > patch->rootzone.depth)
		water_below_field_cap = patch->rootzone.field_capacity - patch->rz_storage;
	else
		water_below_field_cap = patch->rootzone.field_capacity - patch->rz_storage - (patch->rootzone.potential_sat-patch->sat_deficit);

	//--------------------------------------------------------------------------------------------------------------------------
	//	fill the leftover demand with cap rise.
	//--------------------------------------------------------------------------------------------------------------------------
	cap_rise = max(min(patch->potential_cap_rise, min(unsat_zone_patch_demand, water_below_field_cap)), 0.0);
	cap_rise = min((compute_delta_water(
			0,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->soil_depth,
			patch->soil_defaults->soil_depth,
			patch->sat_deficit_z)), cap_rise);

	unsat_zone_patch_demand   -= min(cap_rise, unsat_zone_patch_demand);
	patch->cap_rise           += cap_rise;
	patch->potential_cap_rise -= cap_rise;

	//--------------------------------------------------------------------------------------------------------------------------
	//	Now supply the remaining demand with water left in the unsat zone.  We are going below field cap now!!
	//	First guess at change in sat storage to meet demand.
	//--------------------------------------------------------------------------------------------------------------------------
	delta_unsat_zone_storage = -1 * min(unsat_zone_patch_demand, patch->rz_storage);
	unsat_zone_patch_demand  = unsat_zone_patch_demand + delta_unsat_zone_storage;

	//--------------------------------------------------------------------------------------------------------------------------
	//	Reduce the stratum actual transpiration and compute final N-uptake and daily allocation as a function
	//	of available C and N (if growth flag is on). Note that this will not reduce CO2 flux as well.
	//--------------------------------------------------------------------------------------------------------------------------
	if ( unsat_zone_patch_demand_initial > 0 ){
		patch->transpiration_unsat_zone =min( patch->transpiration_unsat_zone
			* (1 - unsat_zone_patch_demand / unsat_zone_patch_demand_initial), patch->potential_evapotranspiration);
		patch->potential_evapotranspiration -= patch->transpiration_unsat_zone;

		patch->exfiltration_unsat_zone =min(patch->exfiltration_unsat_zone
			* (1 - unsat_zone_patch_demand / unsat_zone_patch_demand_initial), patch->potential_evapotranspiration);
		patch->potential_evapotranspiration -= patch->exfiltration_unsat_zone;
	}
	
	if ( sat_zone_patch_demand_initial > 0 ) {
		patch->transpiration_sat_zone =min( patch->transpiration_sat_zone
			* (1 - sat_zone_patch_demand / sat_zone_patch_demand_initial), patch->potential_evapotranspiration);
		patch->potential_evapotranspiration -= patch->transpiration_sat_zone;
		
		patch->exfiltration_sat_zone =min( patch->exfiltration_sat_zone
			* (1 - sat_zone_patch_demand / sat_zone_patch_demand_initial), patch->potential_evapotranspiration);
		patch->potential_evapotranspiration -= patch->exfiltration_sat_zone;
	}

	patch->AET += (patch->transpiration_unsat_zone + patch->transpiration_sat_zone
				+ patch->exfiltration_unsat_zone + patch->exfiltration_sat_zone);

	//--------------------------------------------------------------------------------------------------------------------------
	// 	Resolve plant uptake and soil microbial N demands
	//--------------------------------------------------------------------------------------------------------------------------
	if (command_line->grow_flag > 0)  {
		resolve_sminn_competition(&(patch->soil_ns), patch->surface_NO3, patch->surface_NH4, &(patch->ndf));
	}

	//--------------------------------------------------------------------------------------------------------------------------
	// in order to restrict denitri/nitrific on non-veg patches type tag vegtype
	//--------------------------------------------------------------------------------------------------------------------------
	vegtype = 0;
	for ( layer=0 ; layer<patch->num_layers; layer++ ){
		//for ( stratum=0 ; stratum < patch->layers[layer].count; stratum++ ){ uncommentted by guoping
			strata =
				//patch->canopy_strata[(patch->layers[layer].strata[stratum])]; //uncommentted by guoping
                patch->canopy_strata;
			if ( (strata->defaults->epc.veg_type != NON_VEG) && command_line->grow_flag>0 ){
				vegtype=1;
				canopy_stratum_growth(
					patch,
					strata,
					command_line,
					current_date );
			}
			else {
				if ( strata->phen.annual_allocation == 1){
					strata->cdf.leafc_store_to_leafc_transfer =	strata->cs.leafc_store;
					strata->cs.leafc_transfer                += strata->cdf.leafc_store_to_leafc_transfer;
					strata->cs.leafc_store                   -= strata->cdf.leafc_store_to_leafc_transfer;
					strata->ndf.leafn_store_to_leafn_transfer = strata->ns.leafn_store;
					strata->ns.leafn_transfer                += strata->ndf.leafn_store_to_leafn_transfer;
					strata->ns.leafn_store                   -= strata->ndf.leafn_store_to_leafn_transfer;
				}
			}

			patch->totalc        += strata->cover_fraction * strata->cs.totalc;
			patch->totaln        += strata->cover_fraction * strata->ns.totaln;
			patch->net_plant_psn += strata->cover_fraction * strata->cdf.net_psn;
			patch->lai           += strata->cover_fraction * strata->epv.proj_lai;
		//} //end of for
	}

	
	
	//==============================================================================================================
	patch->ndf.preday_sminn_to_npool += patch->ndf.sminn_to_npool;// used to check nitrogen balance

	// The following line is added by guopoing for water balace. Becasue transpirated water should incrase saturation deficit
	// loss of water from saturation zone will directly increase saturation deficit. Loss of water from unsaturation will first 
	// decrease the content of water in unsaturated zone and then content in root zone storage. Finally, it may increase the
	// saturation deficit.
	patch->sat_deficit   += (patch->transpiration_sat_zone   + patch->exfiltration_sat_zone);
	patch->unsat_storage -= (patch->transpiration_unsat_zone + patch->exfiltration_unsat_zone);
	
	if(patch->unsat_storage < 0) {
		patch->rz_storage   +=patch->unsat_storage;
		patch->unsat_storage =0.;
		if(patch->rz_storage<0) {
			patch->sat_deficit -= patch->rz_storage;
			patch->rz_storage   = 0.;
		}
	}


	//--------------------------------------------------------------------------------------------------------------------------
	//  Compute potential saturation for rootzone layer
	//--------------------------------------------------------------------------------------------------------------------------
	if (patch->rootzone.depth > ZERO)
		patch->rootzone.potential_sat = compute_delta_water(
		command_line->verbose_flag,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		patch->rootzone.depth, 0.0);
	else
		patch->rootzone.potential_sat = 0.; //added by guoping

	//--------------------------------------------------------------------------------------------------------------------------
	//      Recompute patch soil moisture storage
	//--------------------------------------------------------------------------------------------------------------------------
	if (patch->sat_deficit < ZERO) {
		patch->S = 1.0;
		patch->rootzone.S = 1.0;
		patch->rz_drainage = 0.0;
		patch->unsat_drainage = 0.0;
	}
	else if (patch->sat_deficit_z > patch->rootzone.depth)  {		// Constant vertical profile of soil porosity 
		//-------------------------------------------------------
		//	soil drainage and storage update
		//-------------------------------------------------------
		patch->rootzone.S  = min(patch->rz_storage / patch->rootzone.potential_sat, 1.0);
		patch->rz_drainage = compute_unsat_zone_drainage(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->pore_size_index,
			patch->rootzone.S,
			patch->soil_defaults->m_z,
			patch->rootzone.depth,
			patch->soil_defaults->Ksat_0 / 2,
			patch->rz_storage - patch->rootzone.field_capacity);

		patch->rz_storage    -=  patch->rz_drainage;
		patch->unsat_storage +=  patch->rz_drainage;

		patch->S              = patch->unsat_storage / (patch->sat_deficit - patch->rootzone.potential_sat);
		patch->rootzone.S     = min(patch->rz_storage / patch->rootzone.potential_sat, 1.0);

		patch->unsat_drainage = compute_unsat_zone_drainage(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->pore_size_index,
			patch->S,
			patch->soil_defaults->m_z,
			patch->sat_deficit_z,
			patch->soil_defaults->Ksat_0 / 2,
			patch->unsat_storage - patch->field_capacity);

		patch->unsat_storage -=  patch->unsat_drainage;
		patch->sat_deficit   -=  patch->unsat_drainage;
	}
	else  {
		patch->rz_storage   += patch->unsat_storage;	// transfer left water in unsat storage to rootzone layer 
		patch->unsat_storage = 0.0;

		patch->S           = min(patch->rz_storage / patch->sat_deficit, 1.0);
		patch->rootzone.S  = min(patch->rz_storage / patch->rootzone.potential_sat, 1.0);
		patch->rz_drainage = compute_unsat_zone_drainage(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->soil_defaults->pore_size_index,
			patch->S,
			patch->soil_defaults->m_z,
			patch->sat_deficit_z,
			patch->soil_defaults->Ksat_0 / 2,
			patch->rz_storage - patch->rootzone.field_capacity);

		patch->unsat_drainage = 0.0;
		patch->rz_storage    -= patch->rz_drainage;
		patch->sat_deficit   -= patch->rz_drainage;
	}

	//--------------------------------------------------------------------------------------------------------------------------
	//     Final rootzone saturation calculation
	//--------------------------------------------------------------------------------------------------------------------------
	if (patch->sat_deficit > patch->rootzone.potential_sat)
		patch->rootzone.S = min(patch->rz_storage / patch->rootzone.potential_sat, 1.0);
	else
		patch->rootzone.S = min((patch->rz_storage + patch->rootzone.potential_sat - patch->sat_deficit)
							/ patch->rootzone.potential_sat, 1.0);

	//--------------------------------------------------------------------------------------------------------------------------
	//	finalized soil and litter decomposition	and any septic losses
	//--------------------------------------------------------------------------------------------------------------------------
	if ((command_line->grow_flag > 0) && (vegtype == 1)) {
		patch->ndf.plant_Nuptake = 0.; //added by guoping for check nitrogen balance
		/*
		patch->soil_ns.totaln = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
		patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
		nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln +
			patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
			+ patch->ndf.preday_sminn_to_npool
			- patch->soil_ns.totaln - patch->litter_ns.totaln -
			patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4
			- patch->ndf.sminn_to_npool - patch->ndf.plant_Nuptake - patch->ndf.denitrif - patch->ndf.total_DON_loss);

		if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
			printf("21 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
			printf("22 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
			printf("23 nitrate is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
			printf("24 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
			printf("25 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
			printf("26 nitrate is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
			printf("27 nitrate is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_npool, patch->ndf.plant_Nuptake);
			printf("27 nitrate is not balanced %12.8f %12.8f \n", patch->ndf.denitrif, patch->ndf.total_DON_loss);
			printf("balance is %12.8f  \n", nitrate_balance);
			printf("28 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
			getchar();
		}
	   */

		update_decomp(current_date,	&(patch->soil_cs),	&(patch->soil_ns),	&(patch->litter_cs), &(patch->litter_ns),
			&(patch->cdf),	&(patch->ndf),	patch);

		/*
		patch->soil_ns.totaln   = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
		patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
		nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln + 
						patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
						+ patch->ndf.preday_sminn_to_npool 
						- patch->soil_ns.totaln - patch->litter_ns.totaln - 
						patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4
						- patch->ndf.sminn_to_npool-patch->ndf.plant_Nuptake-patch->ndf.denitrif-patch->ndf.total_DON_loss);
		
		if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
			printf("21 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
			printf("22 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
			printf("23 nitrate is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
			printf("24 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
			printf("25 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
			printf("26 nitrate is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
			printf("27 nitrate is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_npool, patch->ndf.plant_Nuptake);
			printf("27 nitrate is not balanced %12.8f %12.8f \n", patch->ndf.denitrif, patch->ndf.total_DON_loss);
			printf("balance is %12.8f  \n", nitrate_balance);
			printf("282 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
			getchar();
		}
		*/

		update_dissolved_organic_losses(current_date, patch->soil_defaults->DON_loss_rate,
				&(patch->soil_cs),
				&(patch->soil_ns),
				&(patch->litter_cs),
				&(patch->litter_ns),
				&(patch->cdf),
				&(patch->ndf));
		/*
		patch->soil_ns.totaln = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
		patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
		nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln +
			patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
			+ patch->ndf.preday_sminn_to_npool
			- patch->soil_ns.totaln - patch->litter_ns.totaln -
			patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4
			- patch->ndf.sminn_to_npool - patch->ndf.plant_Nuptake - patch->ndf.denitrif - patch->ndf.total_DON_loss);

		if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
			printf("21 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
			printf("22 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
			printf("23 nitrate is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
			printf("24 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
			printf("25 nitrate is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
			printf("26 nitrate is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
			printf("27 nitrate is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_npool, patch->ndf.plant_Nuptake);
			printf("27 nitrate is not balanced %12.8f %12.8f \n", patch->ndf.denitrif, patch->ndf.total_DON_loss);
			printf("balance is %12.8f  \n", nitrate_balance);
			printf("281 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
			getchar();
		}
		*/

		update_nitrif(&(patch->soil_cs),&(patch->soil_ns), &(patch->cdf), &(patch->ndf),
			patch->soil_defaults->soil_type,
			patch->rootzone.S,
			patch->metv.tsoil,
			patch->soil_defaults->porosity_0,
			0.25,
			patch->soil_defaults->mobile_N_proportion, patch->std);
		
		/*
		patch->soil_ns.totaln = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
		patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
		nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln +
			patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
			+ patch->ndf.preday_sminn_to_npool
			- patch->soil_ns.totaln - patch->litter_ns.totaln -
			patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4
			- patch->ndf.sminn_to_npool - patch->ndf.plant_Nuptake  - patch->ndf.denitrif - patch->ndf.total_DON_loss);
		
		if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
			printf("1 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
			printf("2 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
			printf("3 nitrogen is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
			printf("4 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
			printf("5 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
			printf("6 nitrogen is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
			printf("7 nitrogen is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_npool, patch->ndf.plant_Nuptake);
			printf("8 nitrogen is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_nitrate, patch->ndf.denitrif);
			printf("nitrogen balance is %12.8f %12.8f \n", patch->ndf.total_DON_loss, nitrate_balance);
			printf("84 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
			getchar();
		}
		*/

		update_denitrif(&(patch->soil_cs),	&(patch->soil_ns),	&(patch->cdf),	&(patch->ndf),
			patch->soil_defaults->soil_type,
			patch->rootzone.S, patch->std);
		
		/*
		patch->soil_ns.totaln = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
		patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
		nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln +
			patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
			+ patch->ndf.preday_sminn_to_npool
			- patch->soil_ns.totaln - patch->litter_ns.totaln -
			patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4
			- patch->ndf.sminn_to_npool - patch->ndf.plant_Nuptake -patch->ndf.denitrif
			- patch->ndf.total_DON_loss);
		if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
			printf("1 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
			printf("2 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
			printf("3 nitrogen is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
			printf("4 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
			printf("5 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
			printf("6 nitrogen is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
			printf("7 nitrogen is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_npool, patch->ndf.plant_Nuptake);
			printf("8 nitrogen is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_nitrate, patch->ndf.denitrif);
			printf("nitrogen balance is %12.8f %12.8f \n", patch->ndf.total_DON_loss, nitrate_balance);
			printf("83 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
			getchar();
		}
		*/
	}
	

	//=============================================================================================================================
	//check carbon balance
	//=============================================================================================================================
	patch->soil_cs.totalc                 = patch->soil_cs.soil1c + patch->soil_cs.soil2c + patch->soil_cs.soil3c + patch->soil_cs.soil4c;
	patch->litter_cs.totalc               = patch->litter_cs.litr1c + patch->litter_cs.litr2c + patch->litter_cs.litr3c + patch->litter_cs.litr4c;
	patch->totalc                         = patch->soil_cs.totalc + patch->litter_cs.totalc;
	patch->heterol_respiration = patch->cdf.litr1c_hr + patch->cdf.litr2c_hr + patch->cdf.litr4c_hr;
	patch->heteros_respiration = patch->cdf.soil1c_hr + patch->cdf.soil2c_hr + patch->cdf.soil3c_hr + patch->cdf.soil4c_hr;
	
	/*
	patch->carbon_balance = patch->preday_soil_totalc + patch->preday_litter_totalc - patch->soil_cs.totalc - patch->litter_cs.totalc 
		- patch->heteros_respiration - patch->heterol_respiration - patch->cdf.total_DOC_loss;

	if (patch->carbon_balance > 0.00000001 || patch->carbon_balance < -0.0000001){
		printf("1 carbon is not balanced %12.8f  \n", patch->carbon_balance);
		printf("2 Scarbon is not balanced %12.8f %12.8f %12.8f\n", patch->soil_cs.totalc, patch->preday_soil_totalc, patch->heteros_respiration);
		printf("2 Lcarbon is not balanced %12.8f %12.8f %12.8f\n", patch->litter_cs.totalc, patch->preday_litter_totalc, patch->heterol_respiration);
		printf("c carbon is not balanced %12.8f %12.8f \n", patch->cdf.total_DOC_loss);
		printf("83 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
		getchar();
	}
	*/


	//===========================================================================================================================
	//check nitrogen balance see if it is balanced
	// note: we need double check see if gw parameter is used. In addition, how to changed patch->preday_NO3 and patch->preday_NH4
	// to patch->ndep_NO3 and patch->ndep_NH4 respectively 
	// Guoping
	//===========================================================================================================================
	patch->soil_ns.totaln   = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n ;
	patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
	patch->totaln           = patch->soil_ns.totaln + patch->litter_ns.totaln+ patch->soil_ns.nitrate + patch->soil_ns.sminn;
	nitrate_balance         = patch->preday_totaln +  patch->ndep_NO3 + patch->ndep_NH4 + fertilizer_NO3 + fertilizer_NH4
		                      - patch->totaln - patch->ndf.N_to_gw  - patch->ndf.denitrif; 
	/*
	// check patch->ndf.N_to_gw if gw is used
	nitrate_balance = (patch->soil_ns.preday_totaln + patch->litter_ns.preday_totaln +
		patch->soil_ns.preday_nitrate + patch->soil_ns.preday_sminn + patch->preday_NO3 + patch->preday_NH4
		+ patch->ndf.preday_sminn_to_npool
		- patch->soil_ns.totaln - patch->litter_ns.totaln -
		patch->soil_ns.nitrate - patch->soil_ns.sminn - patch->surface_NO3 - patch->surface_NH4
		- patch->ndf.sminn_to_npool - patch->ndf.plant_Nuptake - patch->ndf.denitrif
		- patch->ndf.total_DON_loss);
	
	if (nitrate_balance>0.00000001 || nitrate_balance<-0.00000001) {
		printf("1 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->litter_ns.preday_totaln);
		printf("2 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.preday_nitrate, patch->soil_ns.preday_sminn);
		printf("3 nitrogen is not balanced %12.8f %12.8f \n", patch->preday_NO3, patch->preday_NH4);
		printf("4 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.totaln, patch->litter_ns.totaln);
		printf("5 nitrogen is not balanced %12.8f %12.8f \n", patch->soil_ns.nitrate, patch->soil_ns.sminn);
		printf("6 nitrogen is not balanced %12.8f %12.8f \n", patch->surface_NO3, patch->surface_NH4);
		printf("7 nitrogen is not balanced %12.8f %12.8f \n", patch->ndf.sminn_to_npool, patch->ndf.plant_Nuptake);
		printf("8 nitrogen is not balanced %12.8f %12.8f \n", patch->ndf.denitrif,patch->ndf.total_DON_loss);
		printf("nitrogen balance is %12.8f \n", nitrate_balance);
		printf("82 patch id %d %d %d %d \n", patch->ID, current_date.year, current_date.month, current_date.day);
		getchar();
	}
	*/

	/*
	//==========================================================================================================================
	// Checking water balance to see if it is zero
	//==========================================================================================================================
	water_balance = patch->rain + patch->snow
	+ patch->preday_rain_stored        - patch->rain_stored                     // rain stored by canopy
	+ patch->preday_snow_stored        - patch->snow_stored                     // snow stored by canopy
	+ patch->preday_snowpack           - patch->snowpack.water_equivalent_depth // snowpack on the ground
	+ patch->litter.preday_rain_stored - patch->litter.rain_stored              // rain stored by litter
	+ patch->preday_detention_store    - patch->detention_store                 // water detened on soil surface
	+ patch->preday_rz_storage         - patch->rz_storage                      // water stored in root zone layer
	+ patch->preday_unsat_storage      - patch->unsat_storage                   // water stored in unsat zone layer
	- patch->preday_sat_deficit        + patch->sat_deficit                     // water needed to saturate the whole soil
	- patch->AET                                                                // water evapotranspirated
	- patch->gw_drainage;                                                       // groundwater drainage

	if ((water_balance > 0.00000001)|| (water_balance < -0.00000001)){
	printf("water_balance in patch_daily_f_new.cpp is NOT ZERO %12.8f\n",water_balance);
	printf("patch->rain %12.8f \n",patch->rain);
	printf("patch->snow %12.8f \n",patch->snow);
	printf("\n \n");

	printf("Changes in canopy rain_stored %12.8f,%12.8f \n", patch->preday_rain_stored,patch->rain_stored);
	printf("Changes in canopy snow_stored %12.8f,%12.8f \n", patch->preday_snow_stored,patch->snow_stored);
	printf("Changes in snowpack %12.8f,%12.8f \n", patch->preday_snowpack,patch->snowpack.water_equivalent_depth);
	printf("Changes in litter.rain_stored %12.8f,%12.8f \n", patch->litter.preday_rain_stored , patch->litter.rain_stored);
	printf("Changes in surface detention_store %12.8f,%12.8f \n",patch->preday_detention_store, patch->detention_store);
	printf("Changes in rz_storage %12.8f,%12.8f \n",patch->preday_rz_storage, patch->rz_storage);
	printf("Changes in unsat_storage %12.8f,%12.8f \n",  patch->preday_unsat_storage,patch->unsat_storage);
	printf("Changes in sat_deficit %12.8f,%12.8f \n", patch->sat_deficit, patch->preday_sat_deficit);
	printf("Actual total evapotranspiration %12.8f \n", patch->AET);
	printf("patch->gw_drainage is %12.8f \n",patch->gw_drainage);

	printf("time and id is %d %d %d %d \n",current_date.year,current_date.month,current_date.day,patch->ID);
	//getchar();
	}
	*/

	//---------------------------------------------------------------------------------------------------------------------------
	//	get rid of any negative soil or litter stores
	//---------------------------------------------------------------------------------------------------------------------------
	if (command_line->grow_flag > 0)
		check_zero_stores(&(patch->soil_cs), &(patch->soil_ns), &(patch->litter_cs), &(patch->litter_ns));


	return;
} //end patch_daily_F.c
