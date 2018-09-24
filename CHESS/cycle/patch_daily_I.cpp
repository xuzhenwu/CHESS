//-------------------------------------------------------------------------------------------------------------------------------
//	patch_daily_I 						
//		 - performs cycling of patch state vars		
//			at the START OF THE DAY			 
//								
//	DESCRIPTION						
//								
//	This routine performs simulation cycles on an identified canopy_stata in the patch.				
//								
//-------------------------------------------------------------------------------------------------------------------------------



#include <iostream>
#include <math.h>
#include "CHESS.h"
#include "Constants.h"

using namespace std;

void		patch_daily_I(struct patch_object *patch,
						  struct daily_clim_object daily_clim,
						  struct command_line_object *command_line,
						  struct	date current_date)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//  Local Function Declarations.                                
	//---------------------------------------------------------------------------------------------------------------------------
	void   canopy_stratum_daily_I(
		struct	patch_object *,
		struct canopy_strata_object *,
		struct command_line_object *,
		struct date);
	
	double	compute_layer_field_capacity(
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
	
	
	double	compute_delta_water(
		int,
		double,
		double,
		double,
		double,
		double);

	double	compute_z_final(
		int,
		double,
		double,
		double,
		double,
		double);

	//int	update_rootzone_moist(
	//	struct patch_object	*,
	//	struct	rooting_zone_object	*,
	//	struct command_line_object *);
	
	double	compute_capillary_rise(
		int,
		double,
		double,
		double,
		double,
		double);


	double  compute_potential_exfiltration(
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);
	
	double  compute_soil_water_potential(
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
	
	int  compute_potential_decomp(
		double,
		double,
		double,
		double,
		double,
		struct  soil_c_object   *,
		struct  soil_n_object   *,
		struct  litter_c_object *,
		struct  litter_n_object *,
		struct  cdayflux_patch_struct *,
		struct  ndayflux_patch_struct *);
	
	void    sort_patch_layers(struct patch_object *);
	
	void	zero_patch_daily_flux(
		struct	patch_object *,
		struct  cdayflux_patch_struct *,
		struct  ndayflux_patch_struct *);
	
	void	update_litter_interception_capacity(
		double,
		struct  litter_c_object *,
		struct  litter_object *);
	

   void zone_daily_I(
				  struct 	patch_object 	*patch,
				  struct    daily_clim_object daily_clim,
				  struct 	command_line_object *command_line,
				  struct 	date 	current_date);

	//---------------------------------------------------------------------------------------------------------------------------
	//  Local variable definition.                                  
	//---------------------------------------------------------------------------------------------------------------------------
	int	layer;
	//int	stratum;
	double	rzm;
	struct  canopy_strata_object *strata;
	int 	compute_year_day( struct date );
	int     index,stratum;
    double	cosegeom,sinegeom;
	double	coshss,	hss;
	double	Io_array[13] = { 0.0, 1445.0, 1431.0, 1410.0, 1389.0, 1368.0,1354.0,
							1354.0, 1375.0, 1403.0, 1424.0, 1438.0, 1445.0 };


	double	declination_array[47] = { 0.0, -23.0, -22.0, -21.0, -19.0, -17.0, -15.0, -12.0, -9.0, -6.0, -3.0, 0.0, 3.0,
		6.0, 9.0, 12.0, 14.0, 17.0, 19.0, 21.0, 22.0, 23.0, 23.5, 23.5, 23.0, 21.5, 20.0, 18.0, 16.0, 14.0,
		12.0, 9.0, 6.0, 3.0, 0.0, -3.0, -6.0, -9.0, -12.0, -15.0, -17.0, -19.0, -21.0, -22.0, -23.0, -23.5,-23.5 };


	//===========================================================================================================================
	//First calculate  the length of day
	//---------------------------------------------------------------------------------------------------------------------------
	//  Compute the solar constant. Total broadband solar flux (W/m2)                           
	//  Look up table of values based on C version of Rhessys. 
	//---------------------------------------------------------------------------------------------------------------------------
	patch->Io = Io_array[current_date.month];

	//---------------------------------------------------------------------------------------------------------------------------
	//	Determine the year day (i.e. days from start of year)												
	//---------------------------------------------------------------------------------------------------------------------------
	patch->year_day =  compute_year_day(current_date );

	//---------------------------------------------------------------------------------------------------------------------------
	//  Compute the solar declination. Based on a numerical approximation to the equation:         
	//      declination = 23.45 sin(0.986 Nm) degrees               
	//      where Nm = number of days since March 21 assuming day and night are of equal length.          
 	//  Equation from Linacre, Climate Data and Resources, p. 148. Approximation taken from C version of rhessys.              
	//	We only store cos(declination) which we need on zone level.	
	//---------------------------------------------------------------------------------------------------------------------------
	index             = (int) ( 1.0 + ((double)patch->year_day)/8);
	
	patch->declin     = declination_array[index]*DtoR;
	patch->cos_declin = cos(declination_array[index] * DtoR);
	patch->sin_declin = sin(declination_array[index] * DtoR);

	
	patch->gsurf=1/70.;                          //added by guoping
	patch->ga=patch->patch_defaults->wind/208;   //added by guoping



	//---------------------------------------------------------------------------------------------------------------------------
	//	Daylength (seconds). Now using GBGC computation taken from Jones, Plans and Microclimate.				
	//	Note that I am assuming this is the time when the solar	zenith angle has a cosine >0.   						
	//	Note that we may want to consider daylengths with both diffuse and direct irradiance and only diffuse irradianc.
	//		This can be computed using the zone level horizons.
	//---------------------------------------------------------------------------------------------------------------------------
	cosegeom = cos(patch->latitude*DtoR) * patch->cos_declin;
	sinegeom = sin(patch->latitude*DtoR) * patch->sin_declin;
	coshss   = -(sinegeom) / cosegeom;
	if (coshss < -1.0) coshss = -1.0; // 24-hr daylight 
	if (coshss > 1.0)  coshss = 1.0;  // 0-hr daylight 
	hss              = acos(coshss);  // hour angle at sunset (radians) 
	patch->daylength = 2.0 * hss * SECPERRAD;
	

	//---------------------------------------------------------------------------------------------------------------------------
	//	Find solar zenith angle at noon (not elevation angle) Form Linacre.		
	//---------------------------------------------------------------------------------------------------------------------------
	patch->theta_noon =  patch->latitude*DtoR - patch->declin;
	
	//===========================================================================================================================
	// Calculate daily meteorological variables, include critical + uncritical
	//---------------------------------------------------------------------------------------------------------------------------
	zone_daily_I(patch,daily_clim,command_line,current_date);
	

	//===========================================================================================================================
	//We put patch_hourly here because it should calculate 
	//===========================================================================================================================
	//	initialzie (zero out) daily Carbon and Nitrogen fluxes		
	//---------------------------------------------------------------------------------------------------------------------------
	zero_patch_daily_flux(patch, &(patch->cdf), &(patch->ndf));

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


	//---------------------------------------------------------------------------------------------------------------------------
	//	Estimate potential cap rise. limited to water in sat zone.	
	//---------------------------------------------------------------------------------------------------------------------------
	patch->potential_cap_rise =	compute_capillary_rise(
		command_line->verbose_flag,
		patch->sat_deficit_z,
		patch->soil_defaults->psi_air_entry,
		patch->soil_defaults->pore_size_index,
		patch->soil_defaults->m_z,
		patch->soil_defaults->Ksat_0 );

	if (patch->potential_cap_rise < ZERO)
		patch->potential_cap_rise = 0.0;


	//---------------------------------------------------------------------------------------------------------------------------
	//	Compute the max exfiltration rate. 
	//  First check if we are saturated.  If so the potential exfiltration rate is the capillary rise rate.
	//	If we are within the active unsat zone then we assume that the potential exfiltration rate is the minimum of
	//	the computed exfiltration rate and the potential caprise - i.e. hydrologic connectivity between surface and	water table.
	//---------------------------------------------------------------------------------------------------------------------------
	if ( patch->sat_deficit_z <= patch->soil_defaults->psi_air_entry ){
		patch->potential_exfiltration = patch->potential_cap_rise;
	}
	else{
		if ( patch->soil_defaults->active_zone_z < patch->sat_deficit_z ){
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
	for ( layer=0 ; layer<patch->num_layers; layer++ ){
		//---------------------------------------------------------------------------
		//	Cycle through the canopy strata
		//---------------------------------------------------------------------------
		for ( stratum=0 ; stratum<patch->layers[layer].count; stratum++ ){
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
				current_date );
		}
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	Calculate effective patch lai from stratum					
	//	Accumulate root biomass for patch soil - required for N updake from soil                        
	//	also determine total plant carbon if grow option is specified
	//---------------------------------------------------------------------------------------------------------------------------
	patch->soil_cs.frootc = 0.0;

	for (stratum=0 ; stratum<patch->num_canopy_strata; stratum++){
		if (command_line->grow_flag > 0) {
			patch->soil_cs.frootc += patch->canopy_strata->cover_fraction * patch->canopy_strata->cs.frootc;
			//patch->preday_totalc  += patch->canopy_strata->cover_fraction * patch->canopy_strata->cs.preday_totalc;
			//patch->preday_totaln  += patch->canopy_strata->cover_fraction * patch->canopy_strata->ns.preday_totaln;
		}
		patch->rootzone.depth = max(patch->rootzone.depth,patch->canopy_strata->rootzone.depth);
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
		rzm = patch->rz_storage+patch->rootzone.potential_sat-patch->sat_deficit;

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
		update_litter_interception_capacity(
			patch->litter.moist_coef,
			&(patch->litter_cs),
			&(patch->litter));

		//xu. The decomp was set as 0 ?
		//return ok and ok remained to be 0
		//then this lines has no means
		if (compute_potential_decomp(
			patch->metv.tsoil,
			patch->soil_defaults->psi_max,
			patch->soil_defaults->psi_air_entry,
			patch->rootzone.S,
			patch->std,
			&(patch->soil_cs),
			&(patch->soil_ns),
			&(patch->litter_cs),
			&(patch->litter_ns),
			&(patch->cdf),
			&(patch->ndf)
			) != 0){
			fprintf(stderr,"fATAL ERROR: in compute_potential_decomp() ... Exiting\n");
			exit(1);
		}
	}

	return;
}//end patch_daily_I.c
