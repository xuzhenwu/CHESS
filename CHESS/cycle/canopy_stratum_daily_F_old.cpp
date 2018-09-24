//===============================================================================================================================
// 								
//			canopy_stratum_daily_F			
//								
//	NAME							
//	canopy_statrum_daily_F 					
//								
//								
//	SYNOPSIS						
//	void canopy_stratum_daily_F 				
//								
//	OPTIONS							
//								
//	DESCRIPTION						
//								
//								
//								
//	PROGRAMMER NOTES					
//===============================================================================================================================

#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

#include <iostream>
using namespace std;

void	canopy_stratum_daily_F(
struct	patch_object		*patch,
struct   layer_object		*layer,
struct 	canopy_strata_object 	*stratum,
struct 	command_line_object	*command_line,
struct 	date 			current_date)
{
	//--------------------------------------------------------------
	//	Local function declaration				
	//--------------------------------------------------------------
	long	yearday(struct date);

	long	julday(struct date calendar_date);

	double	compute_diffuse_radiative_fluxes(
		int,
		double *,
		double,
		double,
		double,
		double,
		double,
		double);

	double	compute_diffuse_radiative_PAR_fluxes(
		int,
		double	*,
		double,
		double,
		double,
		double,
		double,
		double);

	double	compute_direct_radiative_fluxes(
		int,
		double	*,
		double,
		double,
		double,
		double,
		double,
		double);


	void	compute_maint_resp(double,
		double,
	struct  cstate_struct *,
	struct  nstate_struct *,
	struct epconst_struct *,
	struct  metvar_struct *,
	struct cdayflux_struct *);

	double	 compute_snow_stored(
		int,
		double,
		double,
		double	*,
		double	*,
	struct	canopy_strata_object *,double *);

	double	 compute_rain_stored(
		int,
		double	*,
	struct	canopy_strata_object *,double *);

	double  compute_ra_overstory(
		int,
		double,
		double *,
		double,
		double,
		double,
		double *);

	double  compute_ra_understory(
		int,
		double,
		double *,
		double,
		double,
		double  *);

	double  compute_ra_surface(
		int,
		double,
		double *,
		double,
		double,
		double  *);

	double	compute_vascular_stratum_conductance(
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
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double, int,
	struct canopy_strata_object *,
	struct patch_object *);

	double	compute_nonvascular_stratum_conductance(
		int,
		double,
		double,
		double,
		double,
		double);

	double	compute_surface_heat_flux(
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

	double	penman_monteith(
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		int);

	int	compute_farq_psn(
	struct psnin_struct *,
	struct psnout_struct *,
		int);

	double	compute_potential_N_uptake_Dickenson(
		double,
	struct	epconst_struct,
	struct	epvar_struct *,
	struct cstate_struct *cs,
	struct nstate_struct *ns,
	struct cdayflux_struct *);

	double	compute_potential_N_uptake_Waring(
	struct	epconst_struct,
	struct	epvar_struct *,
	struct cstate_struct *cs,
	struct nstate_struct *ns,
	struct cdayflux_struct *);

	double	compute_potential_N_uptake(
	struct	epconst_struct,
	struct	epvar_struct *,
	struct cstate_struct *cs,
	struct nstate_struct *ns,
	struct cdayflux_struct *);
	//===========================================================================================================================
	//  Local variable definition.                                  
	//===========================================================================================================================
	//double tmid;
	double  assim_sunlit;
	double  assim_shade;
	double	dC13_sunlit, dC13_shade;
	//double  APAR_direct_sunlit;
	double	dry_evaporation;
	double	ga;
	double	Kdown_direct;
	double	Kdown_diffuse;
	double	PAR_diffuse;
	double	PAR_direct;
	double  total_incoming_PAR;
	double	perc_sunlit;
	double	potential_evaporation_rate;
	double	potential_rainy_evaporation_rate;
	double	rainy_evaporation;
	double	rain_throughfall;
	double	rnet_evap;
	double	rnet_trans_sunlit, rnet_trans_shade; //rnet_trans,
	double	snow_throughfall;
	double	transpiration;
	double	transpiration_rate;
	double	transpiration_rate_sunlit;
	double	transpiration_rate_shade;
	double	potential_transpiration;
	double	potential_transpiration_rate;
	double	potential_transpiration_rate_sunlit;
	double	potential_transpiration_rate_shade;
	double  wind;


	struct	psnin_struct	psnin  = { 0 };
	struct	psnout_struct	psnout = { 0 };
	double  rain_balance, snow_balance, balance;
	//double  direct, diffuse;
	int     day; //added by guoping to control maintenace respiration

	day = yearday(current_date);
	
	//===========================================================================================================================
	//	Initialize stratum variables.				
	//===========================================================================================================================
	stratum->Kstar_diffuse = 0.0;
	stratum->Kstar_direct  = 0.0;
	stratum->APAR_direct   = 0.0;
	stratum->APAR_diffuse  = 0.0;
	
	stratum->Lstar         = patch->Lstar_canopy;         //added by guoping to initialize stratum[0].Lstar
	stratum->potential_evaporation = 0.0;

	//===========================================================================================================================
	//	Initialize temporary variables for transmitted fluxes.	
	//===========================================================================================================================
	Kdown_diffuse = patch->Kdown_diffuse;
	Kdown_direct  = patch->Kdown_direct;
	PAR_diffuse   = patch->PAR_diffuse;
	PAR_direct    = patch->PAR_direct;
	total_incoming_PAR      = PAR_diffuse + PAR_direct;


	rain_throughfall = patch->rain_throughfall;
	snow_throughfall = patch->snow_throughfall;
	ga   = patch->ga;
	wind = patch->wind;
	transpiration           = 0.0;
	potential_transpiration = 0.0;
	rainy_evaporation       = 0.;
	dry_evaporation         = 0.;

	rain_balance = rain_throughfall + stratum->preday_rain_stored; //used for checking water balance
	snow_balance = snow_throughfall + stratum->preday_snow_stored; //used for checking water balance


	/*
	if (current_date.year == 1962 && current_date.month == 6 && current_date.day >= 5 && patch->ID == 4419){
		printf("patch->kdown direct and diffuse is %12.8f %12.8f \n", patch->Kdown_diffuse, patch->Kdown_direct);
		printf("reflectance is %12.8f %12.8f \n", stratum->defaults->K_reflectance, stratum->defaults->PAR_reflectance);
		printf("theta_noon is %12.8f %12.8f \n", patch->theta_noon, stratum->gap_fraction);
		printf("projection pai lai %12.8f %12.8f \n", stratum->epv.proj_pai, stratum->epv.proj_lai);
		getchar();
		printf(" \n");
	}
	*/

	//===========================================================================================================================
	//  Intercept diffuse radiation.                                
	//  We assume that the zone slope == patch slope.               
	//  We also assume that radiation reflected into the upper hemisphere is lost.                                     
	//  We do not make adjustements for chaanging gap fraction over the diurnal cycle - using a suitable mean gap fraction instead.                                                
	//  We do take into account the patch level horizon which will allow simulation of clear-cuts/clearings with low sza's 
	//  Note that for zone level temperature and radiation computation we took into account only zone level horizon
	//      since we figured that climate above the zone was well mixed.                                                  
	//===========================================================================================================================
	stratum->Kstar_diffuse = compute_diffuse_radiative_fluxes(
		command_line->verbose_flag,
		&(Kdown_diffuse),
		Kdown_direct,
		-1 * stratum->defaults->epc.ext_coef,
		stratum->gap_fraction,
		stratum->epv.proj_pai,
		patch->theta_noon,
		stratum->defaults->K_reflectance);

	stratum->APAR_diffuse = compute_diffuse_radiative_PAR_fluxes(
		command_line->verbose_flag,
		&(PAR_diffuse),
		PAR_direct,
		-1 * stratum->defaults->epc.ext_coef,
		stratum->gap_fraction,
		stratum->epv.proj_lai,
		patch->theta_noon,
		stratum->defaults->PAR_reflectance);

	//===========================================================================================================================
	//  Intercept direct radiation.                                 
	//      hard to measure for each strata.  We could use top of canopy albedo but this integrates the effect of the  
	//      entire canopy.  Furthermore, it requires in general knowledge of the canopy BRDF - which we want to avoid.  
	//      Instead we assume a certain reflectance and transmittance for each strata's canopy elements.        
	//  We assume that the zone slope == patch slope.               
	//  We also assume that radiation reflected into the upper hemisphere is lost.                                     
	//  We do not make adjustements for changing gap fraction over the diurnal cycle - using a suitable mean gap fraction  
	//      instead.                                                
	//  We do take into account the patch level horizon which will allow simulation of clear-cuts/clearings with low sza's 
	//  Note that for zone level temperature and radiation computation we took into account only zone level horizon
	//      since we figured that climate above the zone was well mixed.                                                  
	//===========================================================================================================================

	stratum->Kstar_direct = compute_direct_radiative_fluxes(
		command_line->verbose_flag,
		&(Kdown_direct),
		-1 * stratum->defaults->epc.ext_coef,
		stratum->gap_fraction,
		stratum->epv.proj_pai,
		patch->theta_noon,
		stratum->defaults->K_reflectance,
		stratum->defaults->K_reflectance);


	stratum->APAR_direct = compute_direct_radiative_fluxes( //double_check if compute_direct_radiative_fluxes is the same as 
		// compute_direct_radiative_par_fluxes????
		command_line->verbose_flag,
		&(PAR_direct),
		-1 * stratum->defaults->epc.ext_coef,
		stratum->gap_fraction,
		stratum->epv.proj_pai,
		patch->theta_noon,
		stratum->defaults->PAR_reflectance,
		stratum->defaults->PAR_reflectance);

	/*
	if (current_date.year == 1962 && current_date.month == 6 && current_date.day == 6 && patch->ID == 457){
		printf("patch->kdown direct and diffuse is %12.8f %12.8f \n", patch->Kdown_diffuse, patch->Kdown_direct);
		printf("stratum diffuse and direct is %12.8f %12.8f \n", stratum->Kstar_diffuse, stratum->Kstar_direct);
		printf(" ex_coef is %12.8f %12.8f \n", stratum->defaults->epc.ext_coef, stratum->epv.proj_pai);
		//getchar();
	}
	*/



	//===========================================================================================================================
	//	record fraction of PAR absorption and seasonal low water content (for use in dynamic version to limit allocation)	
	//===========================================================================================================================
	stratum->epv.min_vwc = min((patch->unsat_storage / patch->sat_deficit), stratum->epv.min_vwc);

	//===========================================================================================================================
	//	Compute conductance aerodynamic.			
	//===========================================================================================================================
	if (patch->snowpack.height <= stratum->epv.height){
		//------------------------------------------------------------------
		//		Compute by brute force if usrat_overu given.	
		//------------------------------------------------------------------
		if (stratum->defaults->ustar_overu == -999.9){
			//--------------------------------------------------------------
			//		Highest layer in patch.				
			//--------------------------------------------------------------
			if (stratum->epv.height == patch->layers->height){
				stratum->ga = 1.0 / compute_ra_overstory(

					command_line->verbose_flag,
					stratum->defaults->wind_attenuation_coeff,
					&(wind),
					patch->screen_height,
					stratum->epv.height,
					layer->base,
					&(ga));
			}
			//--------------------------------------------------------------
			//		Layer is not the highest and is >0.1highest ht.	
			//--------------------------------------------------------------
			else if (stratum->epv.height > (patch->layers->height * 0.1)){
				stratum->ga = 1.0 / compute_ra_understory(
					command_line->verbose_flag,
					stratum->defaults->wind_attenuation_coeff,
					&(wind),
					stratum->epv.height,
					layer->base,
					&(ga));
			}
			//--------------------------------------------------------------
			//		Layer is <0.1highest ht. in height.		
			//--------------------------------------------------------------
			else{
				stratum->ga = 1.0 /
					compute_ra_surface(
					command_line->verbose_flag,
					stratum->defaults->wind_attenuation_coeff,
					&(wind),
					stratum->epv.height,
					layer->base,
					&(ga));
			}
		}
		else{

			stratum->ga =
				pow(patch->wind * stratum->defaults->ustar_overu, 2.0);
			ga = stratum->ga;
		}
	}
	else{
		stratum->ga = 0.0;
	}

	//===========================================================================================================================
	//	Factor in stability correction.				
	//===========================================================================================================================
	stratum->ga = stratum->ga * patch->stability_correction;

	//===========================================================================================================================
	//	Determine non-vascular condductance to evaporation.	This conductance represnets the inverse of an additional 	
	//	resistance to vapour flux from the stratum rian storage	surface over and above aerodynamic resistances that also	
	//	affect turbulent heat transfer.  						 	
	//	                            								
	//	A linear relationship is currently assumed with the amount	of relative rain stored - or with the patch unsat zone storage  
	//	relative to water equiv depth top water table if roots are present.  Parameters for the relationship	
	//	are supplied via the stratum default file.					
	//===========================================================================================================================
	if (stratum->epv.all_pai > 0){
		stratum->gsurf = compute_nonvascular_stratum_conductance(
			command_line->verbose_flag,
			stratum->rain_stored + rain_throughfall,
			stratum->epv.all_pai
			* stratum->defaults->specific_rain_capacity,
			stratum->defaults->epc.gl_c,
			stratum->defaults->gsurf_slope,
			stratum->defaults->gsurf_intercept);
	}
	else{
		stratum->gsurf = compute_nonvascular_stratum_conductance(
			command_line->verbose_flag,
			patch->unsat_storage + rain_throughfall,
			patch->sat_deficit,
			stratum->defaults->epc.gl_c,
			stratum->defaults->gsurf_slope,
			stratum->defaults->gsurf_intercept);
	}

	//===========================================================================================================================
	//	Update snow storage ( this also updates the patch level snow throughfall and the stratum level Kstar )	 	
	//	Snow on the canopy is first sublimated before any ET can take place.  This may seem like we are hobbling	
	//	the energy budget since the sublimation reduces the	energy used for stomatal conductance etc. however we	
	//	suggest that when it snows it is likely ET is small.	
	//===========================================================================================================================
	stratum->snow_stored = compute_snow_stored(
		command_line->verbose_flag,
		patch->metv.tday,
		1000.0,
		&(snow_throughfall),
		&(rain_throughfall),
		stratum,&patch->potential_evapotranspiration);


	if (stratum->epv.proj_lai > ZERO)
		perc_sunlit = (stratum->epv.proj_lai_sunlit) / (stratum->epv.proj_lai_sunlit+ stratum->epv.proj_lai_shade);
	else
		perc_sunlit = 1.0;

	if (stratum->epv.proj_lai_shade > ZERO && patch->metv.dayl > ZERO)
		stratum->ppfd_shade = (stratum->APAR_diffuse * (1 - perc_sunlit)) /
		patch->metv.dayl/ stratum->epv.proj_lai_shade; //  //
	else
		stratum->ppfd_shade = 0.0;

	if (stratum->epv.proj_lai_sunlit > ZERO && patch->metv.dayl > ZERO)
		stratum->ppfd_sunlit = (stratum->APAR_direct + stratum->APAR_diffuse * perc_sunlit) /
		patch->metv.dayl / stratum->epv.proj_lai_sunlit; //;
	else
		stratum->ppfd_sunlit = 0.0;

	//===========================================================================================================================
	//	Compute stratum conductance	m/s			
	//	Note that the APAR supplied reduces to the rate of APAR (kJ/sec) absorbed during the daytime.  We do this since:							
	//								
	//	gs is only meaningful in the day gs responds to the instantaneous rather than daily	averaged APAR.  				
	//								
	//	Also note that we will not void a shorwtwave energy	budget since stratum conductance is used to scale 	
	//	potnetial_evaporation; which we make sure is limited by the available energy. 				
	//								
	//	Note: PAR values are converted fro umol photon/m2*day to umol photon/m2*sec				
	//===========================================================================================================================

	//------------------------------------------------------------------
	//	if there is still snow sitting on the canopy gs should be zero 
	//------------------------------------------------------------------
	if (stratum->snow_stored < ZERO)  {
		stratum->gs_sunlit = compute_vascular_stratum_conductance(
			command_line->verbose_flag,
			stratum->defaults->epc.ppfd_coef,
			stratum->defaults->epc.gl_c,
			stratum->defaults->lai_stomatal_fraction,
			stratum->defaults->epc.psi_open,
			stratum->defaults->epc.psi_close,
			stratum->defaults->epc.gl_smax,
			stratum->defaults->epc.topt,
			stratum->defaults->epc.tcoef,
			stratum->defaults->epc.tmax,
			stratum->defaults->epc.vpd_open,
			stratum->defaults->epc.vpd_close,
			stratum->ppfd_sunlit,
			stratum->epv.proj_lai_sunlit,
			stratum->epv.psi,
			patch->metv.tmin,
			patch->metv.tday,
			patch->metv.vpd, patch->CO2,
			stratum->defaults->epc.coef_CO2,
			stratum->ID,
			stratum, patch);

		stratum->potential_gs_sunlit = compute_vascular_stratum_conductance(
			command_line->verbose_flag,
			stratum->defaults->epc.ppfd_coef,
			stratum->defaults->epc.gl_c,
			stratum->defaults->lai_stomatal_fraction,
			stratum->defaults->epc.psi_open,
			stratum->defaults->epc.psi_close,
			stratum->defaults->epc.gl_smax,
			stratum->defaults->epc.topt,
			stratum->defaults->epc.tcoef,
			stratum->defaults->epc.tmax,
			stratum->defaults->epc.vpd_open,
			stratum->defaults->epc.vpd_close,
			stratum->ppfd_sunlit,
			stratum->epv.proj_lai_sunlit,
			9999.0,
			patch->metv.tmin,
			stratum->defaults->epc.topt,
			stratum->defaults->epc.vpd_open - 1.0, patch->CO2,
			stratum->defaults->epc.coef_CO2,
			stratum->ID,
			stratum, patch);

		stratum->gs_shade = compute_vascular_stratum_conductance(
			command_line->verbose_flag,
			stratum->defaults->epc.ppfd_coef,
			stratum->defaults->epc.gl_c,
			stratum->defaults->lai_stomatal_fraction,
			stratum->defaults->epc.psi_open,
			stratum->defaults->epc.psi_close,
			stratum->defaults->epc.gl_smax,
			stratum->defaults->epc.topt,
			stratum->defaults->epc.tcoef,
			stratum->defaults->epc.tmax,
			stratum->defaults->epc.vpd_open,
			stratum->defaults->epc.vpd_close,
			stratum->ppfd_shade,
			stratum->epv.proj_lai_shade,
			stratum->epv.psi,
			patch->metv.tmin,
			patch->metv.tday,
			patch->metv.vpd, patch->CO2,
			stratum->defaults->epc.coef_CO2,
			stratum->ID,
			stratum, patch);

		stratum->potential_gs_shade = compute_vascular_stratum_conductance(
			command_line->verbose_flag,
			stratum->defaults->epc.ppfd_coef,
			stratum->defaults->epc.gl_c,
			stratum->defaults->lai_stomatal_fraction,
			stratum->defaults->epc.psi_open,
			stratum->defaults->epc.psi_close,
			stratum->defaults->epc.gl_smax,
			stratum->defaults->epc.topt,
			stratum->defaults->epc.tcoef,
			stratum->defaults->epc.tmax,
			stratum->defaults->epc.vpd_open,
			stratum->defaults->epc.vpd_close,
			stratum->ppfd_shade,
			stratum->epv.proj_lai_shade,
			9999.0,
			patch->metv.tmin,
			stratum->defaults->epc.topt,
			stratum->defaults->epc.vpd_open - 1.0, patch->CO2,
			stratum->defaults->epc.coef_CO2,
			stratum->ID,
			stratum, patch);
	}
	else {
		stratum->gs_sunlit = 0.0;
		stratum->gs_shade = 0.0;
		stratum->potential_gs_sunlit = 0.0;
		stratum->potential_gs_shade = 0.0;
	}

	stratum->gs = stratum->gs_sunlit + stratum->gs_shade;

	//===========================================================================================================================
	//	Determine heat flux between stratum and surface. representative of the surface temperature.  Obviously   
	//	for strata NOT at the surface one would have 0 heat cap	(kJ/day)							
	//	We had the current input of rain as well.		
	//===========================================================================================================================

	if (stratum->epv.height == 0) {
		stratum->surface_heat_flux =
			-1 * compute_surface_heat_flux(
			command_line->verbose_flag,
			stratum->snow_stored,
			stratum->rain_stored + patch->rain_throughfall,
			stratum->epv.all_pai *
			stratum->defaults->specific_rain_capacity,
			patch->metv.tmin,
			patch->metv.tnightmax,
			patch->metv.tsoil,
			patch->soil_defaults->deltaz,
			stratum->defaults->min_heat_capacity,
			stratum->defaults->max_heat_capacity);

	}
	else stratum->surface_heat_flux = 0.; //added by guoping for stratum->epv.height >0

	//===========================================================================================================================
	//	Compute evaporation and transpiration RATES (m/s) for daylight period .					
	//	The rainy rate assumes a vpd of 10Pa.			
	//	Note if surface heat flux makes evap negative we have condensation.  					
	//===========================================================================================================================
	if (patch->metv.dayl > ZERO)
		rnet_evap = 1000 * (stratum->Kstar_direct + stratum->Kstar_diffuse
		+ stratum->Lstar + stratum->surface_heat_flux) / patch->metv.dayl;
	else
		rnet_evap = 0.0;

	/*
	if (current_date.year == 1962 && current_date.month == 6 && current_date.day == 6 && patch->ID == 457){
		printf("transpiration sat_ zone is %12.8f %12.8f \n", stratum->Kstar_direct, stratum->Kstar_diffuse);
		printf("transpiration sat_ zone is %12.8f %12.8f %12.8f \n", stratum->Lstar, patch->Lstar_canopy,stratum->surface_heat_flux);
		printf("transpiration sat_ zone is %12.8f %12.8f \n", patch->metv.dayl, patch->daytime_rain_duration);
		printf("renet_evap is %12.8f %12.8f %12.8f \n", stratum->gsurf, stratum->ga, rnet_evap);
		printf("patch->ID£é£ó %12.8f¡¡%d \n", stratum->potential_evaporation, patch->ID);
		getchar();

	}
   */


	//--------------------------------------------------------------
	//	Estimate potential evap rates.				
	//--------------------------------------------------------------
	if ((stratum->gsurf > ZERO) && (stratum->ga > ZERO) && (rnet_evap > ZERO)) {
		potential_evaporation_rate = penman_monteith(
			command_line->verbose_flag,
			patch->metv.tday,
			patch->metv.pa,
			patch->metv.vpd,
			rnet_evap,
			1 / stratum->gsurf,
			1 / stratum->ga,
			2);
		potential_rainy_evaporation_rate = penman_monteith(
			command_line->verbose_flag,
			patch->metv.tday,
			patch->metv.pa,
			0,
			rnet_evap,
			1 / stratum->gsurf,
			1 / stratum->ga,
			2);
	}
	else {
		potential_evaporation_rate = 0.0;
		potential_rainy_evaporation_rate = 0.0;
	}

	potential_evaporation_rate = max(0, potential_evaporation_rate);
	potential_rainy_evaporation_rate = max(0, potential_rainy_evaporation_rate);

	//---------------------------------------------------------------------------------------------------------------------------
	//	Do not allow negative potential evap if it raining since condensation/dew dep is the same as rain		
	//---------------------------------------------------------------------------------------------------------------------------
	if (patch->rain > 0){
		potential_evaporation_rate = max(0, potential_evaporation_rate);
		potential_rainy_evaporation_rate = max(0, potential_rainy_evaporation_rate);
	}


	//--------------------------------------------------------------
	//	Transpiration rate.					
	//--------------------------------------------------------------
	//--------------------------------------------------------------
	//	Reduce rnet for transpiration by ratio of lai to pai	
	//--------------------------------------------------------------
	if (patch->metv.dayl > ZERO) {
		rnet_trans_sunlit = 1000 * ((stratum->Kstar_direct + (perc_sunlit)*stratum->Kstar_diffuse)
			* (stratum->epv.proj_lai / stratum->epv.proj_pai) +
			perc_sunlit* (stratum->Lstar + stratum->surface_heat_flux)) / patch->metv.dayl;

		rnet_trans_shade = 1000 * (((1.0 - perc_sunlit)*stratum->Kstar_diffuse)
			* (stratum->epv.proj_lai / stratum->epv.proj_pai) +
			(1.0 - perc_sunlit) * (stratum->Lstar + stratum->surface_heat_flux)) / patch->metv.dayl;
	}
	else {
		rnet_trans_sunlit = 0.0;
		rnet_trans_shade = 0.0;
	}



	rnet_trans_shade  = max(rnet_trans_shade, 0.0);
	rnet_trans_sunlit = max(rnet_trans_sunlit, 0.0);


	if ((rnet_trans_sunlit > ZERO) &&
		(stratum->defaults->lai_stomatal_fraction > ZERO) && (stratum->gs_sunlit > ZERO) && (stratum->ga > ZERO)){
		transpiration_rate_sunlit = penman_monteith(
			command_line->verbose_flag,
			patch->metv.tday,
			patch->metv.pa,
			patch->metv.vpd ,
			rnet_trans_sunlit,
			1 / stratum->gs_sunlit,
			1 / stratum->ga,
			2);
		potential_transpiration_rate_sunlit = penman_monteith(
			command_line->verbose_flag,
			patch->metv.tday,
			patch->metv.pa,
			patch->metv.vpd,
			rnet_trans_sunlit,
			1 / stratum->potential_gs_sunlit,
			1 / stratum->ga,
			2);
	}
	else{
		transpiration_rate_sunlit = 0.0;
		potential_transpiration_rate_sunlit = 0.0;
	}


	if ((rnet_trans_shade > ZERO) && (stratum->defaults->lai_stomatal_fraction > ZERO) &&
		(stratum->gs_shade > ZERO) && (stratum->ga > ZERO)){

		transpiration_rate_shade = penman_monteith(
			command_line->verbose_flag,
			patch->metv.tday,
			patch->metv.pa,
			patch->metv.vpd,
			rnet_trans_shade,
			1 / stratum->gs_shade,
			1 / stratum->ga,
			2);

		potential_transpiration_rate_shade = penman_monteith(
			command_line->verbose_flag,
			patch->metv.tday,
			patch->metv.pa,
			patch->metv.vpd,
			rnet_trans_shade,
			1 / stratum->potential_gs_shade,
			1 / stratum->ga,
			2);
	}
	else{
		transpiration_rate_shade = 0.0;
		potential_transpiration_rate_shade = 0.0;
	}

	transpiration_rate = (transpiration_rate_sunlit + transpiration_rate_shade); 
	potential_transpiration_rate = (potential_transpiration_rate_sunlit + potential_transpiration_rate_shade); 

	//===========================================================================================================================
	//	Compute potential evaporation of stratum. Weighted by rain and non rain periods of the daytime	
	//	m/day = m/s * (sec/day)					
	//								
	//	Note that Kstar is converted from Kj/m2*day to W/m2	
	//===========================================================================================================================
	stratum->potential_evaporation = potential_evaporation_rate * (patch->metv.dayl - patch->daytime_rain_duration)
		+ potential_rainy_evaporation_rate * patch->daytime_rain_duration;


	//===========================================================================================================================
	//	If this stratum is on the surface and has a non-zero rooting depth we assume it first takes water from soil. 
	//	In this case we move some of the potential evaporation to porential transpiration.  The criteria 	
	//	used to determine how much is based on the estimated cap rise  during the day.		
	//===========================================================================================================================
	if ((stratum->epv.height == 0) && (stratum->rootzone.depth > 0) && (stratum->epv.proj_lai > ZERO)){
		if (stratum->potential_evaporation > ZERO){
			transpiration = min(stratum->potential_evaporation, patch->cap_rise);
			potential_transpiration = min(stratum->potential_evaporation, patch->cap_rise);
			stratum->potential_evaporation -= transpiration;
		}
		else{
			transpiration = 0.0;
			potential_transpiration = 0.0;
		}
	}


	//===========================================================================================================================
	//	Update rain storage ( this also updates the patch level rain_throughfall and stratum->potential_evaporation	
	//===========================================================================================================================
	stratum->rain_stored = compute_rain_stored(
		command_line->verbose_flag,
		&(rain_throughfall),
		stratum,&patch->potential_evapotranspiration);

	//===========================================================================================================================
	//	Separate the evaporation into rainy and dry assuming rainy happens as much as it can.		
	//===========================================================================================================================
	if (stratum->evaporation > ZERO){
		rainy_evaporation = min(patch->daytime_rain_duration * potential_rainy_evaporation_rate, stratum->evaporation);
		dry_evaporation = stratum->evaporation - rainy_evaporation;
	}
	else{
		rainy_evaporation = 0;
		dry_evaporation = stratum->evaporation;
	}

	//===========================================================================================================================
	//	Compute Canopy transpiration.	m/day			
	//								
	//	We assume the potential_evaporation has been reduced by the amount need to evaporate the storage 	
	//	and incident precipitation.  				
	//								
	//	we  assume that part of the day	which had a dry canopy which was transpiring.		
	//								
	//	The advantage of doing it via potentia evaporation include:						
	//								
	//	1. the potential evaporation is computed once using	the vpd and Rnet and ra of the WHOLE day.  We do not	
	//	reduce Rnet after we evaporate water stored but	reduce the potential evap. directly.			
	//								
	//	2. more importantly, the leaf conductance is computed on the full value average daytime Rnet and the 		
	//	Rnet used in transpiration is the daily averaged value	which properly reflects the balance in demand between	
	//	Rnet and vpd.						
	//								
	//	we assume no transpiration happens during evaporation or rain hours.						
	//								
	//	All of this is only done for vascular strata.		
	//===========================================================================================================================


	if (stratum->defaults->lai_stomatal_fraction > ZERO){
		if (stratum->potential_evaporation > ZERO){
			transpiration = transpiration_rate * (patch->metv.dayl - patch->daytime_rain_duration -
				dry_evaporation / potential_evaporation_rate);

			potential_transpiration = potential_transpiration_rate * (patch->metv.dayl - patch->daytime_rain_duration -
				dry_evaporation / potential_evaporation_rate);
		}

		else{
			transpiration = 0.0;
			potential_transpiration = 0.0;
		}
	}
	else {
		transpiration = 0.0;  //added by guoping
		potential_transpiration = 0.0;  //added by guoping
	}


	stratum->PET = potential_transpiration;

	//===========================================================================================================================
	//	Separate the transpiration into unsat and sat zone	transpiration demands based on the rooting depth and	
	//	the patch depth to water table.				
	//								
	//	This transpiration may actually be reduced if there is	not enough water in the respective zone.  But that 	
	//	event should indicate that maybe the leaf water potential control on gs or the soil moisture control on 
	//	gsurf (for bryophytes) is not well calibrated.		
	//===========================================================================================================================

	if ( stratum->rootzone.depth > ZERO ){
		stratum->transpiration_sat_zone   = transpiration * max(0, 1 - (max(patch->sat_deficit_z,0) / stratum->rootzone.depth));
		stratum->transpiration_unsat_zone = transpiration - stratum->transpiration_sat_zone;
	}
	else{
		if ( patch->sat_deficit_z > ZERO ){
			stratum->transpiration_unsat_zone = transpiration;
			stratum->transpiration_sat_zone   = 0;
		}
		else{
			stratum->transpiration_unsat_zone = 0;
			stratum->transpiration_sat_zone = transpiration;
		}
	}

	//if (current_date.year == 1962 && current_date.month == 6 && current_date.day == 6){
	//	printf("transpiration sat_ zone is %12.8f %12.8f \n", stratum->transpiration_sat_zone, stratum->transpiration_unsat_zone);
	//	printf("patch->ID£é£ó %12.8f¡¡%d \n", stratum->potential_evaporation, patch->ID);
	//		getchar();
	
	//}


	//===========================================================================================================================
	//***************************************************************************************************************************
	//	Do respiration and photosynthesis only for plants	
	//===========================================================================================================================

	if (stratum->defaults->lai_stomatal_fraction > ZERO && patch->leaf_on_flag==1) { //
		//-----------------------------------------------------------------------------------------
		//	perform maintenance respiration				
		//	only fluxes are computed here; stores are updated later	in canopy_stratum_daily_growth				
		//-----------------------------------------------------------------------------------------

        compute_maint_resp(                                                                      
				stratum->defaults->mrc.q10,
				stratum->defaults->mrc.per_N,
				&(stratum->cs),
				&(stratum->ns),
				&(stratum->defaults->epc),
				&(patch->metv),
				&(stratum->cdf));

		if (stratum->epv.all_lai > ZERO && stratum->snow_stored < ZERO)  {  //
			//===================================================================================================================
			//	convert maintenance resp from kg/m2*day to umol/m2*s only fluxes are computed here; stores are updated later	
			//	in canopy_stratum_daily_growth				
			//===================================================================================================================

			//===================================================================================================================
			//	Set up input array for stratum assimilation		
			//===================================================================================================================

			//===================================================================================================================
			// this all needs to be repeated for sunlit and for shaded and for potential and actual psn`			
			//  note that at present potential only takes into account water (i.e not nitrogen) limitations			
			//===================================================================================================================
			// potential sunlit psn						
			//===================================================================================================================
			if (stratum->defaults->epc.veg_type == C4GRASS)
				psnin.c3 = 0;
			else 
				psnin.c3 = 1;

			if (patch->metv.dayl > ZERO)
				psnin.Rd = stratum->cdf.leaf_day_mr /(stratum->epv.proj_lai * patch->metv.dayl*12.011e-9);
			else
				psnin.Rd = 0.0;

			psnin.pa   = patch->metv.pa;
			psnin.co2  = patch->CO2;
			psnin.flnr = stratum->defaults->epc.flnr;
			psnin.t    = patch->metv.tday;
			psnin.irad = stratum->ppfd_sunlit;

			if (stratum->cs.leafc > ZERO)
				psnin.lnc = stratum->ns.leafn / (stratum->cs.leafc * 1.0) / stratum->epv.proj_sla_sunlit;
			else
				psnin.lnc = 0.0;
			
			//===================================================================================================================
			// note multiply by 1000; accounted for in compute_farq_psn by a /1000 
			//	this is done for numerical precision			
			//===================================================================================================================
			psnin.g = min(stratum->defaults->epc.gl_smax , stratum->defaults->epc.gl_c ) * 	stratum->defaults->lai_stomatal_fraction*1000 / 1.6;  
			
			if ((psnin.lnc > 0.0) && (psnin.irad > 0.0))
				compute_farq_psn(&psnin, &psnout, 1);
			else
				psnout.A = 0.0;

			assim_sunlit = psnout.A;

			//===================================================================================================================
			// potential shade psn						
			//===================================================================================================================
			if (patch->metv.dayl > ZERO)
				psnin.Rd = stratum->cdf.leaf_day_mr / (stratum->epv.proj_lai * patch->metv.dayl*12.011e-9);
			else
				psnin.Rd = 0.0;

			psnin.irad = stratum->ppfd_shade;

			if (stratum->cs.leafc > ZERO)
				psnin.lnc = stratum->ns.leafn / (stratum->cs.leafc * 1.0) / stratum->epv.proj_sla_shade ;
			else
				psnin.lnc = 0.0;

			if ((psnin.lnc > 0.0) && (psnin.irad > 0.0))
				compute_farq_psn(&psnin, &psnout, 1);
			else
				psnout.A = 0.0;

			assim_shade = psnout.A;

			//===================================================================================================================
			// total potential psn:	We add the daytime leaf mr to the assim since it has	
			//	been subtracted in both the farq and accounted for in total mr.						
			//===================================================================================================================
			stratum->cdf.potential_psn_to_cpool = (assim_sunlit*stratum->epv.proj_lai_sunlit
				+ assim_shade * stratum->epv.proj_lai_shade) * patch->metv.dayl*12.011e-9+ stratum->cdf.leaf_day_mr ;//
			
		
			//===================================================================================================================
			// actual sunlit psn						
			//===================================================================================================================
			//===================================================================================================================
			//	convert water vapour to co2 conductance.		
			//===================================================================================================================
			if (patch->metv.dayl > ZERO)
				psnin.Rd = stratum->cdf.leaf_day_mr /(stratum->epv.proj_lai * patch->metv.dayl*12.011e-9);
			else
				psnin.Rd = 0.0;

			//===================================================================================================================
			// note multiply by 1000; accounted for in compute_farq_psn by a /1000. this is done for numerical precision			
			//===================================================================================================================
			psnin.g    = stratum->gs_sunlit*1000 / 1.6 / stratum->epv.proj_lai_sunlit; 
			psnin.irad = stratum->ppfd_sunlit;
			
			//patch->assim_sunlight = stratum->epv.psi;

			if (stratum->cs.leafc > ZERO)
				psnin.lnc = stratum->ns.leafn / (stratum->cs.leafc * 1.0) / stratum->epv.proj_sla_sunlit;
			else
				psnin.lnc = 0.0;

			if ((psnin.lnc > 0.0) && (psnin.irad > 0.0)) {
				if ( compute_farq_psn(&psnin, &psnout, 1)){
					fprintf(stderr,
						"FATAL ERROR: in canopy_stratum_daily_F error in farquhar");
					exit(1);
				}
			}
			else
				psnout.A = 0.0;

			assim_sunlit = psnout.A;
			dC13_sunlit  = psnout.dC13;

			//===================================================================================================================
			// actual shade psn						
			//===================================================================================================================
			//===================================================================================================================
			//	convert water vapour to co2 conductance.		
			// double irad;            // (umol/m2/s) shortwave irradiance 
	        // double g;               // (m * 10**3/s) conductance to CO2 
	        // double Rd;              // (umol/m2/s) dark respiration rate 
	        // double lnc;             // (kg Nleaf/m2) leaf nitrogen per unit area 
	        // double flnr;            // (kg NRub/kg Nleaf) fract. of leaf N in Rubisco 
			// double A;               // (umol/m2/s) final assimilation rate 
			//===================================================================================================================

			if (patch->metv.dayl > ZERO)
				psnin.Rd = stratum->cdf.leaf_day_mr  /	(stratum->epv.proj_lai * patch->metv.dayl*12.011e-9);
			else
				psnin.Rd = 0.0;

			//===================================================================================================================
			// note multiply by 1000; accounted for in compute_farq_psn by a /1000.	This is done for numerical precision			
			//===================================================================================================================
			psnin.g    = stratum->gs_shade*1000 /1.6/stratum->epv.proj_lai_shade;   
			psnin.irad = stratum->ppfd_shade;	
			//patch->assim_shade = psnin.g;

			if (stratum->cs.leafc > ZERO)
				psnin.lnc = stratum->ns.leafn / (stratum->cs.leafc * 1.0) / stratum->epv.proj_sla_shade;
			else
				psnin.lnc = 0.0;

			if ((psnin.lnc > 0.0) && (psnin.irad > 0.0)) {
				if ( compute_farq_psn(&psnin, &psnout, 1)){
					fprintf(stderr,
						"FATAL ERROR: in canopy_stratum_daily_F error in farquhar");
					exit(1);
				}
			}
			else
				psnout.A = 0.0;

			assim_shade = psnout.A;
			dC13_shade  = psnout.dC13;

			//===================================================================================================================
			// total actual psn						
			//===================================================================================================================
			//stratum->cdf.psn_to_cpool = (assim_sunlit*stratum->epv.proj_lai_sunlit+ assim_shade * stratum->epv.proj_lai_shade)	
			//	*patch->metv.dayl*12.011e-9 + stratum->cdf.leaf_day_mr;
			//patch->assim_shade = stratum->ppfd_shade;
			//patch->assim_sunlight = stratum->ppfd_sunlit;
			//patch->proj_lai_sunlit_initial = patch->metv.tday;
			//patch->proj_lai_shade_initial = stratum->epv.proj_sla_sunlit;
			//patch->leaf_day_mr_initial = stratum->epv.proj_sla_shade;

			stratum->cdf.psn_to_cpool = (assim_sunlit*stratum->epv.proj_lai_sunlit
				+ assim_shade * stratum->epv.proj_lai_shade)*patch->metv.dayl*12.011e-9 + stratum->cdf.leaf_day_mr ;//+ stratum->cdf.leaf_day_mr
			//patch->gpp_initial = stratum->cdf.psn_to_cpool;
			if ((assim_sunlit + assim_shade) > ZERO)
				stratum->dC13 = (assim_sunlit * dC13_sunlit + assim_shade * dC13_shade)/(assim_sunlit+assim_shade);
			else 
				stratum->dC13 = 0.0;
		} // end if LAI > O  ** snow stored < 0
		else {
			stratum->cdf.psn_to_cpool           = 0.0;
			stratum->cdf.potential_psn_to_cpool = 0.0;
			patch->assim_shade    = 0.;;
			patch->assim_sunlight = 0.;
			patch->proj_lai_sunlit_initial = 0.;
			patch->proj_lai_shade_initial = 0.;
			patch->leaf_day_mr_initial = 0.;
		}
	} // end if stomatal_fraction > 0 


	//===================================================================================================================
	//	perform growth related computations (if grow flag is on) 
	//===================================================================================================================
	if ((command_line->grow_flag > 0) && (stratum->defaults->epc.veg_type != NON_VEG)) {
		//--------------------------------------------------------------
		//	compute N uptake from the soil 				
		//--------------------------------------------------------------

		switch(stratum->defaults->epc.allocation_flag) {
			case CONSTANT: // constant allocation 
				stratum->ndf.potential_N_uptake =	compute_potential_N_uptake(
					stratum->defaults->epc,
					&(stratum->epv),
					&(stratum->cs),
					&(stratum->ns),
					&(stratum->cdf));
				break;
			case DICKENSON:
				stratum->ndf.potential_N_uptake =compute_potential_N_uptake_Dickenson(
					1,
					stratum->defaults->epc,
					&(stratum->epv),
					&(stratum->cs),
					&(stratum->ns),
					&(stratum->cdf));
				break;
			case WARING:
				stratum->ndf.potential_N_uptake =compute_potential_N_uptake_Waring(
					stratum->defaults->epc,
					&(stratum->epv),
					&(stratum->cs),
					&(stratum->ns),
					&(stratum->cdf));
				break;
		} // end switch 
	}

	//===================================================================================================================
	//	Increment the transmitted fluxes from this patch layer by weighting the fluxes in this stratum by its cover	
	//	fraction - we have check cover fractions sum to 1 in a layer when constructing the patch.			
	//===================================================================================================================
	patch->Kdown_direct_final  += Kdown_direct * stratum->cover_fraction;
	patch->PAR_direct_final    += PAR_direct * stratum->cover_fraction;
	patch->Kdown_diffuse_final += Kdown_diffuse * stratum->cover_fraction;
	patch->PAR_diffuse_final   += PAR_diffuse * stratum->cover_fraction;


	//patch->rain_throughfall_final += (rain_throughfall * stratum->cover_fraction);
	//patch->snow_throughfall_final += (snow_throughfall * stratum->cover_fraction);
	//patch->ga_final               += (ga   * stratum->cover_fraction);
	//patch->wind_final             += (wind * stratum->cover_fraction);
	
	patch->rain_throughfall_final   += rain_throughfall;
	patch->snow_throughfall_final   += snow_throughfall;
	patch->ga_final                 += ga; 
	patch->wind_final               += wind;

	//check water balance by Guoping
	balance = rain_balance -(rain_throughfall+stratum->rain_stored+stratum->evaporation)
			+ snow_balance -(snow_throughfall+stratum->snow_stored+stratum->sublimation);

	if(balance >0.00000001 || balance <-0.00000001){
	    printf("blance is not zero canopy_stratum_daily.f %12.8f \n",balance);
		getchar();
	}

	return;
} //end canopy_stratum_daily_F.c
