//=========================================================================================================================================
//						zone_daily_I						
//																
//	DESCRIPTION													
//																
//	This routine performs simulation cycles on a zone and patches in the zone.  The routine also prints out results	
//	where specified by current tec events files.				
//																
//	For example, the daytime max temperature is a function of both base station max and min over 24hrs  & surface slope &	
//	surface LAI.  If this max temp is actually supplied we will assume that it is appliable to the entire zone EVEN	
//	if there are variations in patch level slope's and LAI's A reason for this is that the mixing lengths for maxtemp	
//	is assumed to be at the zone level.  It is , of course, up to the user to disaggregate zones if this is not the case
//																
//	As a corollary, if the model is being run at a daily time step and we need to synthesize day max temp we do so with 	
//	patch level parameters.  In this case we would compute zone level flat and sloped radiation and zone mean LAI		
//	and use these in the equations for max temp.  While it is true that the equation is non-linear we leave it up to the	
//	user to ensure that the variation in the patch level param-eteres will not result in significant bias when aggregated.	
//																
//	We divide parameters into critical and non-critical. Critical parameters must be present to avoid a fatal error.	
//	Non-critical parameters may be missing for some or all days.
//	MISSING DATA IS ASSUMED TO BE FLAGGED AS -999.0 FOR FLOAT 	
// 	PARAMETERS AND  9999 FOR INTEGRAL PARAMETERS.				
//																
//	Finally, there are some special cases which we need to make	
//	simplifying assumptions for:								
//																
//	Kdown_direct and Kdown_diffuse supplied for the zone.		
//	 	- these are likely measured as "flat surface" irradiance where the sun is above the horizon		
//			since it is probably from a levelled platform.		
//		- it would be tiresome to compute sloped surface versions of these.  								
//		- given this we will assume it is in fact sloped surface values we are getting (i.e based on slope and 		
//			aspect of zone).									
//		- our problem then is how to adjust Tmax_day without computing the RADRATE (which is an hourly thing).	
//			Again, we have to assume that if Kdown_direct and Kdown_diffuse are good enough for this zone the		
//			Tmax_day is good enough for this zone.  			
//	 	- note that we will have to make use of a base station LAI to allow modelling of LAI disturbances on 		
//			Tmax_day.											
//																
//	only one of Kdown_direct and Kdown_diffuse					
//		- at the moment I treat this case as neither are given since it would mean inferring the atmospheric attenuation based on the	
//			supplied variable.  								
//																
//	trans supplied for the zone.								
//		- this refers to daily total atmospheric transmittance if it is given and we dont have Kdown's we synthesize 
//			the Kdowns on an hourly basis using this value		
//		- otherwise we deal with corrections for cloudiness to the clear sky transmissivity						
//	Sept 16 - RIchard Fernandes				
//	atm_trans - the estimators for clear sky atmospheric transmissivity with no cloud fractio data is not good. 
// We have for now put in code which always uses the clear sky value - wether or not either the delta_T or cloud 
// fraction esitmator is first used.
//=========================================================================================================================================
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "CHESS.h"
#include "Constants.h"

using std::cout;
using std::endl;

void zone_daily_initial(struct 	patch_object 	*patch,struct    daily_clim_object daily_clim,
				  struct 	command_line_object *command_line, struct 	date 	current_date)
{
	//=====================================================================================================================================
	//  Local variable definition.
	//=====================================================================================================================================
	double	trans_coeff1, es;

	patch->daily_clim = &daily_clim;

	//=====================================================================================================================================
	// elevationa difference between weather station and simulated patch
	// adjust for lapse rate right away
	

	if (patch->daily_clim->rain>0)
		patch->rain = max(0, patch->daily_clim->rain + patch->z_delta*patch->climate_defaults->lapse_rate_prec);
	else
		patch->rain = 0.;

	if (patch->rain > ZERO)
		patch->metv.tmin = patch->daily_clim->tmin - patch->z_delta * patch->climate_defaults->wet_lapse_rate;
	else
		patch->metv.tmin = patch->daily_clim->tmin - patch->z_delta * patch->climate_defaults->lapse_rate_tmin;

	if (patch->rain > ZERO)
		patch->metv.tmax = patch->daily_clim->tmax - patch->z_delta * patch->climate_defaults->wet_lapse_rate;
	else
		patch->metv.tmax = patch->daily_clim->tmax - patch->z_delta * patch->climate_defaults->lapse_rate_tmax;

	//=====================================================================================================================================
	//	CO2 -ppm - atmospheric CO2  concentration time series	
	//=====================================================================================================================================
	patch->CO2 = patch->daily_clim->CO2;

	//=====================================================================================================================================
	//	Now, estiamte non-critical parameters
	//	Non-critical parameters are a luxury.  If present they may avoid us from doing synthesis of their values (at possibly
	//	an hourly time step to avoid non-linearities).  So, we expend a bit of overhead to see if non-critical parameters
	//	are present.
	//
	//	Since it is	likely that either we will or will not have a given non-critical parameter for the base stations assigned
	//  to the zone I propose that we ASSUME that if the first base station does not have the climate sequence for the non-critical
	//	parameters it is not worth checking the rest of the base stations.
	//=====================================================================================================================================
	patch->metv.tsoil = -999.0;
	patch->relative_humidity = -999.0;
	patch->metv.dayl = 0.; //note:: this is calculated in patch hours.cpp


	//=====================================================================================================================================
	//	tavg
	//	Mean recorded temperature over the entire 24 hrs OR	Arithmetic mean of daily tmax and tmin.
	//=====================================================================================================================================
	if (patch->daily_clim->tavg != -999.){
		patch->metv.tavg = patch->daily_clim->tavg - patch->z_delta	* patch->climate_defaults->lapse_rate;
	}
	else{
		patch->metv.tavg = (patch->metv.tmin + patch->metv.tmax) / 2.0;
	}


	//=====================================================================================================================================
	//	tday
	//	Mean recorded temperature for daylight hours and adjust for	lapse rate OR
	//	wait until the surface radiation terms are computed so we can use adjusted temperatures with
	//	via Equation 1 and Appendix B "MTCLIM"; in this case note that all temperatures are already adjusted for lapse	rate.										
	//=====================================================================================================================================
	if (patch->daily_clim->tday != -999.){
		patch->metv.tday = patch->daily_clim->tday - patch->z_delta * patch->climate_defaults->lapse_rate;
	}
	else{
		patch->metv.tday = patch->climate_defaults->temcf	* (patch->metv.tmax - patch->metv.tavg) + patch->metv.tavg;
	}


	//=====================================================================================================================================
	//	tnight
	//	Mean recorded temperature for hours without daylight which we adjust for lapse rate OR
	//	wait if tday is read in compute based on it and	tmin using method given in zcomp.c of C Rhessys code
	//	OR	if tday is not given we wait until it is computed
	//=====================================================================================================================================
	if (patch->daily_clim->tnight != -999.){
		patch->metv.tnight = patch->daily_clim->tnight - patch->z_delta *patch->climate_defaults->lapse_rate;
	}
	else {
		patch->metv.tnight = (patch->metv.tday + patch->metv.tmin) / 2.0;
	}

	//=====================================================================================================================================
	//	Tnight max - nighttime temperature at sundown used for	heat flux computation - if not present we use the
	//	minimum nighttime temp and essential turn off heat flux.
	//=====================================================================================================================================
	if (patch->daily_clim->tnightmax != -999.){
		patch->metv.tnightmax = patch->daily_clim->tnightmax - patch->z_delta * patch->climate_defaults->lapse_rate;
	}
	else{
		patch->metv.tnightmax = patch->metv.tmin;
	}

	//=====================================================================================================================================
	//	If snow is not available we estimate it at the end of the day based on end of day temperatures
	//=====================================================================================================================================

		if (patch->daily_clim->snow != -999.){
			patch->snow = patch->daily_clim->snow + patch->z_delta* patch->climate_defaults->lapse_rate_prec;
		}
		else{
			//=================================================================================================================================
			//	If we have no snow data determine if the rain should be snow
			//
			//	Based on U.S Army Corps of ENgineers (1956) Snow Hydrology use a min/max to get range in which there 
			// is a mix of snow/rain
			if (patch->metv.tavg < patch->climate_defaults->max_snow_temp){ //

				if (patch->metv.tavg <= patch->climate_defaults->min_rain_temp){
					patch->snow = patch->rain;
					patch->rain = 0.0;
				}
				else{
					double snow_rain_range = patch->climate_defaults->max_snow_temp
						- patch->climate_defaults->min_rain_temp;
					if (snow_rain_range < 0.001){
						patch->snow = patch->rain;
						patch->rain = 0.0;
					}
					else{
						patch->snow = min((patch->climate_defaults->max_snow_temp
							- patch->metv.tavg) * patch->rain / snow_rain_range, patch->rain);
						patch->rain = patch->rain - patch->snow;
					}
				}
			}
			else{
				patch->snow = 0.0;
			}
		}
	

	//=====================================================================================================================================
	//	daylength	(sec)
	//=====================================================================================================================================
	if (patch->daily_clim->dayl != -999.){
		patch->metv.dayl = patch->daily_clim->dayl;
		patch->daylength_flag = 1;
	}
	else{
		patch->metv.dayl = patch->daylength; //
		patch->daylength_flag = 0;
	}


	//=====================================================================================================================================
	//	daytime rain duration - (seconds)
	//	This is the daytime time over which rain fell.	It is used to adjust the vpd to the saturation
	//	vpd during the rain hours and recompute the vpd	for the rest of the day by adjusting the mean value
	//=====================================================================================================================================
	if (patch->daily_clim->daytime_rain_duration != -999.){
		patch->daytime_rain_duration = patch->daily_clim->daytime_rain_duration * 3600;
	}
	else{
		if (patch->rain == 0 || (patch->snow != 0)){
			patch->daytime_rain_duration = 0;
		}
		else{
			patch->daytime_rain_duration = patch->daylength/10.; //guoping double check
		}
	}


	//if (patch->ID == 3842){
	//	printf("patch dayl and patch->daytime_rain_duration %f %f %f \n", patch->metv.dayl, patch->daytime_rain_duration, patch->daylength);
	//	printf("ldown is %12.8f \n");
	//}

	//=====================================================================================================================================
	//	cloud fraction
	//=====================================================================================================================================
	if (patch->daily_clim->cloud_fraction != -999.){
		patch->cloud_fraction = patch->daily_clim->cloud_fraction;
	}
	else{
		//		Also, Linacre's formula requires cloud fraction in 	oktas of cloud cover; where 1 okta = 1/8 th of sky
		//		covered by clouds.
		//
		//		If we dont have cloud fraction data we assume 4.0oktas of full cloudyness if it is a rainy day or a rain or
		//		snow day.
		if ((patch->snow + patch->rain) > patch->climate_defaults->pptmin){
			patch->cloud = 4.0;
			patch->cloud_fraction = 1.0;
		}
		else{
			patch->cloud = 0;
			patch->cloud_fraction = 0.0;
		}
	}

	//=====================================================================================================================================
	//	cloud opacity
	//
	//	Defined as the upper hemispherical albedo + bulk absorptivity of the clouds.
	//	Opacity defaults as 0.8 for clouds.
	//=====================================================================================================================================
	if (patch->daily_clim->cloud_opacity != -999.){
		patch->cloud_opacity = patch->daily_clim->cloud_opacity;
	}
	else{
		if (patch->cloud_fraction = 1.0)
			patch->cloud_opacity = 0.7;
		else
			patch->cloud_opacity = 0.;
	}

	//=====================================================================================================================================
	//	Delta_T
	//
	//	Replaces the MTCLIM logic for computing adjustment to clear sky atmospheric transmissivity based on synoptic
	//  air temperatures and percip as given in Appendix D, EQ. 3 of "MTCLIM" by a pre-computed Delta_T.
	//
	//	If delta T is not given we approximate it by synoptic temp and precip for the current day alone.  This is possibly
	//	sub-optimal however it smooths the coding.  In addition, the use of a cloud fraction input overrides the use of
	//	Delta_T in computing bulk atmospheric transmissivity.
	//
	//	Note :  we also use Delta_T to adjust Ldown if no cloud	Fraction data is present.
	//=====================================================================================================================================
	if (patch->daily_clim->Delta_T != -999.){
		patch->Delta_T = patch->daily_clim->Delta_T;
	}
	else{
		patch->Delta_T = patch->metv.tmax - patch->metv.tmin;

		//=================================================================================================================================
		//		We only make rainy day tranmissivity corrections if	there is a substantial amoun of rain (>1inch)
		//=================================================================================================================================
		if ((patch->rain + max(0.0, patch->snow)) > patch->climate_defaults->pptmin){
			patch->Delta_T = patch->Delta_T * 0.75;
		}
	}

	//=====================================================================================================================================
	// Correct data input errors such that min T is greater than max but warn user of problem with their climate inputs
	//=====================================================================================================================================
	if (patch->Delta_T < -ZERO) {
		printf("\n WARNING: Maximum temperature is less than minimum temperature on %d %d %d",
			current_date.day, current_date.month, current_date.year);
		getchar();
		patch->Delta_T = patch->Delta_T * -1.0;
	}

	//=====================================================================================================================================
	//	bulk atmospheric transmissivity	(includes clouds)
	//	assumed for So and Do (I dont think this is valid for PAR)
	//=====================================================================================================================================
	if (patch->daily_clim->atm_trans != -999.){
		patch->atm_trans = patch->daily_clim->atm_trans;
	}
	else{
		//=================================================================================================================================
		//			compute the clear sky transmissivity.					
		//																
		//			Appendix D Eq. 1 "MTCLIM"								
		//			Bristow and Campbell 1984.								
		//=================================================================================================================================
		patch->atm_trans = patch->climate_defaults->sea_level_clear_sky_trans
			+ patch->z_delta * patch->climate_defaults->atm_trans_lapse_rate;
		//=================================================================================================================================
		//			convert clear sky transmissivity to bulk transmissivity	
		//																
		//			We use two methods:										
		//=================================================================================================================================
		//=================================================================================================================================
		//			Method 1 : no cloud fraction data						
		//																
		//			We use Eq. 2 of Appendix D of "MTCLIM: which in turn	
		//			is based on "Bristow and Campbell(1984)".     			
		//																
		//			Note that patch->defaults->trans_coeff1 varies with	
		//			seasons in "Bristow and Campbell(1984)" while we have	
		//			it as a default variable for each zone.  We might be 	
		//			able to change the value by a tec event.  				
		//=================================================================================================================================
		if (patch->cloud_fraction == -999.0){

			//=============================================================================================================================
			// following Bristol and Campbell (1984) we are going to override 
			//	trans_coeff1 and make it a function of delta T		
			//=============================================================================================================================

			trans_coeff1 = 0.036 * exp(-0.152 * patch->Delta_T);
			patch->atm_trans = patch->atm_trans	*(1.0 - exp(-1 * trans_coeff1*pow(patch->Delta_T, patch->climate_defaults->trans_coeff2)));
			patch->Kdown_direct_adjustment = 1.0;
			patch->Kdown_diffuse_adjustment = 1.0;
		}
		else{
			//=============================================================================================================================
			//			Method 2:  cloud fraction data							
			//																
			//			Here we ignore the Delta_T adjustment method and assume
			//			clouds have a uniform transmissivity (or opacity) and	
			//			we ignore multiple scattering between surface and clouds
			//			at this stage.  Multiple scattering may make a          
			//			difference for low clouds in winter.					
			//																
			//-------------------------------------------------------------
			patch->Kdown_direct_adjustment = (1.0 - patch->cloud_fraction *patch->cloud_opacity);
			patch->Kdown_diffuse_adjustment = (1.0 - (1.-patch->cloud_fraction)*(1.0-patch->cloud_opacity)); //patch->Kdown_diffuse_adjustment
			// is modified by guoping, otherise kdown_diffuse and later patch->APAR_diffuse is always zero; double check this 
			patch->atm_trans -= patch->atm_trans*patch->cloud_opacity; //added by guoping
		}
		//=================================================================================================================================
		//			For now we replace estimated atm trans	by the clear sky since we dont believe
		//			that the estimators are good.		
		//=================================================================================================================================

	}

	
	//=====================================================================================================================================
	//	tsoil		(deg C)
	// not sure what depth is
	//=====================================================================================================================================
	if (patch->daily_clim->tsoil != -999.){
		patch->metv.tsoil = patch->daily_clim->tsoil;
	}
	else{
		//=================================================================================================================================
		//	metv.tsoil - soil temperature 		(degrees C)
		//
		//	We always update a running average for metv.tsoil in case we get a missing value.
		//	Note: a soil heat model may be a better way .
		//=================================================================================================================================
		if (patch->metv.tsoil == -999.0){
			patch->metv.tsoil_sum = 0.9 * patch->metv.tsoil_sum + 0.1 *	patch->metv.tavg;
			patch->metv.tsoil = patch->metv.tsoil_sum;
		}
		else{
			patch->metv.tsoil_sum = patch->metv.tsoil;
		}
	}


	//=====================================================================================================================================
	//	Wind speed at screen height.
	//
	//	It is up to the user to ensure that the windspeed makes sense with whatever surface resistance formulation is
	//	used for fluxes.  I have ensured that the formulations make sense for a windspeed 2m above the vegetated canopy.
	//
	//	Note that for wind speed we should assume it is applicable for a canopy of the mean patch LAI for this zone so, just
	//	as albedo at patch level can modify APAR; LAI at patch level can modify wind speed.
	//=====================================================================================================================================
	if (patch->daily_clim->wind != -999.)
		patch->wind = patch->daily_clim->wind;
	else
		patch->wind = patch->climate_defaults->wind;


	//=====================================================================================================================================
	//	Daily Nitrogen deposition				
	//=====================================================================================================================================
	if (patch->daily_clim->ndep_NO3 != -999.)
		patch->ndep_NO3 = patch->daily_clim->ndep_NO3;
	else
		patch->ndep_NO3 = patch->climate_defaults->ndep_NO3;


	if (patch->daily_clim->ndep_NH4 != -999.)
		patch->ndep_NH4 = patch->daily_clim->ndep_NH4;
	else
		patch->ndep_NH4 = 0.0;

	//=====================================================================================================================================
	//	Dewpoint temperature
	//
	//	I have moved the default dewpoint from the base station to the zone default since it applies to zones.  In the event
	//	that dewpoints recorded at a base_station is available we adjust for differences in elevations as per Appendix G 	of
	//	"MTNCLIM"; otherwise we use Tmin_air for dewpoint.
	//=====================================================================================================================================
	if (patch->daily_clim->tdewpoint != -999.)
		patch->tdewpoint = patch->daily_clim->tdewpoint - patch->z_delta* patch->climate_defaults->dewpoint_lapse_rate;
	else
		patch->tdewpoint = patch->metv.tmin;


	//=====================================================================================================================================
	//      vpd - Pa - daylight mean value.
	//=====================================================================================================================================
	if (patch->daily_clim->vpd != -999.){
		patch->metv.vpd = patch->daily_clim->vpd;
	}
	else{
		//=================================================================================================================================
		//	Saturation Vapour Pressure	(Pa)
		//
		//	Note the original rehssys code supplied es in mbar.
		//	c.f. eq. 1 Running and Coughlan , 1987, p. 133.
		//
		//	Since 1 bar = 100 kpa (approx) ; a millibar = 100 Pa approx.
		//	This explains why the es from the original code was:
		//
		//	6.1078 * exp((17.269*z->metv.tday)/(237.3 +	z->metv.tday))
		//
		//	Which is approx 100 times that of the es here.
		//
		//	Eq. 5.12 p. 110, Jones, "Plants and Microclimate"
		//=================================================================================================================================

		es = 613.75 * exp((17.502 * patch->metv.tavg) / (240.97 + patch->metv.tavg));

		//=================================================================================================================================
		//	Make use of relative humidity if available.
		//=================================================================================================================================
		if (patch->relative_humidity == -999.){
			//=============================================================================================================================
			//	Dew Point Vapour Pressure (Pa)
			//
			//	Note the original rehssys code supplied es in mbar.
			//	c.f. eq. 1 Running and Coughlan , 1987, p. 133.
			//
			//	Since 1 bar = 100 kpa (approx) ; a millibar = 100 Pa approx.
			//	This explains why the es from the original code was:
			//
			//	6.1078 * exp((17.269*z->tdewpoint)/(237.3 +	z->tdewpoint))
			//
			//	Which is approx 100 times that of the es here.
			//
			//	Eq. 5.12 p. 110, Jones, "Plants and Microclimate"
			//	Assuming that tdewpoint is valid for the whole day.
			//
			//	We cannot make a correction for rain_duration here.
			//	Instead we hope that  the dewpoint vapour pressure is measured by a temperature value (night min)
			//=============================================================================================================================
			patch->e_dewpoint = 613.750 * exp((17.502	* patch->tdewpoint) / (240.97 + patch->tdewpoint));
		}
		else{
			//=============================================================================================================================
			//      Dew Point Vapour Pressure (Pa)
			//
			//      ONly for dayligh conditions with no rain.
			//      Eq. 5.13 and 5.14 , p. 110, Jones, "Plants and Microclimate"
			//=============================================================================================================================
			patch->e_dewpoint = patch->relative_humidity * es;
		}
		patch->metv.vpd = max(es - patch->e_dewpoint, 0.0);
	}

	//=====================================================================================================================================
	//	relative humidity
	//
	//	Assumed to apply as daylight mean value.
	//	Used in place of dewpoint temperature to compute vpd if it is available.
	//=====================================================================================================================================
	if (patch->daily_clim->relative_humidity != -999.)
		patch->relative_humidity = patch->daily_clim->relative_humidity / 100.0;
	else{
		es = 613.75 * exp((17.502 * patch->metv.tavg) / (240.97 + patch->metv.tavg));
		if (es > ZERO)
			patch->relative_humidity = 1.0 - patch->metv.vpd / es;
		else
			patch->relative_humidity = 1.0;
	}
	/*
	printf("patch is %f %f\n", patch->metv.tsoil, patch->metv.tsoil_sum);
	printf("patch is %f \n", patch->snow);
	printf("patch is %f %f\n", patch->daytime_rain_duration, patch->daylength);
	printf("patch is %f %f\n", patch->climate_defaults->pptmin, patch->cloud_fraction);
	printf("patch is %f %f\n", patch->cloud_opacity, patch->Delta_T);
	printf("patch is %f %f\n", patch->atm_trans, patch->metv.tsoil);
	printf("patch is %f %f\n", patch->wind, patch->ndep_NO3);
	printf("patch is %f %f\n", patch->ndep_NH4, patch->CO2);
	printf("patch is %f %f\n", patch->metv.vpd, patch->relative_humidity);
	getchar();
	*/
	//=====================================================================================================================================
	//	Kdown_direct		(kJ/(m2*day))
	//	Assumed to be applicable to this zone's slope and aspect!!
	//=====================================================================================================================================
	if (patch->daily_clim->Kdown_direct != -999.){
		patch->Kdown_direct = patch->daily_clim->Kdown_direct;
		patch->Kdown_direct_flag = 1;
	}
	else{
		patch->Kdown_direct = 0.;
		patch->Kdown_direct_flag = 0;
		patch->Kdown_direct_flat = 0.0;
	}

	//=====================================================================================================================================
	//	Kdown_diffuse	(kJ/(m2*day))
	//	Assumed to be applicable to this zone's slope and aspect!!
	//=====================================================================================================================================
	if (patch->daily_clim->Kdown_diffuse != -999.){
		patch->Kdown_diffuse = patch->daily_clim->Kdown_diffuse;
		patch->Kdown_diffuse_flag = 1;
	}
	else{
		patch->Kdown_diffuse = 0.;
		patch->Kdown_diffuse_flag = 0;
		patch->Kdown_diffuse_flat = 0.0;
	}

	//=====================================================================================================================================
	//	PAR_diffuse			(umol/(m2*day))							
	//																
	//	Assumed to be applicable to this zone's slope and aspect!!	
	//	If not available it will be computed as a fixed fraction	
	//	of Kdown_diffuse which is suspect if the Kdown_diffuse is	
	//	synthesized since the synthesis uses atmospheric transmiss	
	//	ivity which may not be the same for Kdown and PAR.			
	//=====================================================================================================================================
	if (patch->daily_clim->PAR_diffuse != -999.){
		patch->PAR_diffuse = patch->daily_clim->PAR_diffuse * 1000000;
	}
	else{
		patch->PAR_diffuse = -999.;
	}

	//=====================================================================================================================================
	//	PAR_direct			(umol/(m2*day))??							
	//																
	//	Assumed to be applicable to this zone's slope and aspect!!	
	//	If not available it will be computed as a fixed fraction	
	//	of Kdown_direct which is suspect if the Kdown_direct is		
	//	synthesized since the synthesis uses atmospheric transmiss	
	//	ivity which may not be the same for Kdown and PAR.			
	//=====================================================================================================================================
	if (patch->daily_clim->PAR_direct != -999.){
		patch->PAR_direct = patch->daily_clim->PAR_direct * 1000000;
	}
	else{
		patch->PAR_direct = -999.;
	}


	//=====================================================================================================================================
	//	Ldown
	//compute the daily downwelling long wave based on .metv.tavg. 	(kJ/(m2*day))
	//=====================================================================================================================================
	if (patch->daily_clim->Ldown != -999.){
		patch->Ldown = patch->daily_clim->Ldown;
	}
	else{
		//=================================================================================================================================
		//		compute the daily downwelling long wave based on .metv.tavg. 	(kJ/(m2*day))
		//
		//		Base on Linacre 1992 as found in C version of rhessys.
		//
		//		Also, Linacre's formula requires cloud fraction in 	oktas of cloud cover; where 1 okta = 1/8 th of sky
		//		covered by clouds.
		//
		//		If we dont have cloud fraction data we assume 4.0oktas of full cloudyness if it is a rainy day or a rain or
		//		snow day.
		//=================================================================================================================================
		patch->Ldown = (208 + 6 * patch->metv.tavg) * (1.0 + 0.0034*pow(patch->cloud, 2.0))* 86400.0 / 1000.0;
	}

	//===========================================================================================================================
	//      update met running averages variables
	//===========================================================================================================================
	patch->tmin21[20] = patch->metv.tmin;
	patch->vpd21[20] = patch->metv.vpd;
	patch->dayl21[20] = patch->metv.dayl;

	patch->metv.tmin_ravg = patch->tmin21[0] / 21.;
	patch->metv.vpd_ravg = patch->vpd21[0] / 21.;
	patch->metv.dayl_ravg = patch->dayl21[0] / 21.;
	for (int jj = 1; jj<21; jj++){
		patch->metv.tmin_ravg += patch->tmin21[jj] / 21.;
		patch->metv.vpd_ravg += patch->vpd21[jj] / 21.;
		patch->metv.dayl_ravg += patch->dayl21[jj] / 21.;

		patch->tmin21[jj - 1] = patch->tmin21[jj];
		patch->vpd21[jj - 1] = patch->vpd21[jj];
		patch->dayl21[jj - 1] = patch->dayl21[jj];
	}

	patch->metv.dayl = 0; //note we set dayl as zero, because it is calculated later
} 
