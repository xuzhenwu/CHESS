//===============================================================================================================================
//						zone_daily_F.cpp							
//																
//	DESCRIPTION													
//																
//	This routine performs simulation cycles on a zone and 		
//	patches in the zone.  The routine also prints out results	
//	where specified by current tec events files.				
//																
//	We assume that all patches owned by this zone share the		
//	same top of canopy fluxes.  Also, we assume that any input	
//	daily fluxes for the zone can apply to the whole patch		
//	and that adjustment of these input fluxes is a function		
// 	of a linear aggergation of patch parameters.				
//																
//	For example, the daytime max temperature is a function of	
//	both base station max and min over 24hrs  & surface slope &	
//	surface LAI.  If this max temp is actually supplied we		
//	will assume that it is appliable to the entire zone EVEN	
//	if there are variations in patch level slope's and LAI's 	
//	A reason for this is that the mixing lengths for maxtemp	
//	is assumed to be at the zone level.  It is , of course,		
//	up to the user to disaggregate zones if this is not the case
//																
//	As a corollary, if the model is being run at a daily time 	
//	step and we need to synthesize day max temp we do so with 	
//	patch level parameters.  In this case we would compute 		
//	zone level flat and sloped radiation and zone mean LAI		
//	and use these in the equations for max temp.  While it is	
//	true that the equation is non-linear we leave it up to the	
//	user to ensure that the variation in the patch level param-	
//	eteres will not result in significant bias when aggregated.	
//																
//	We divide parameters into critical and non-critical.		
//	Critical parameters must be present to avoid a fatal error.	
//	Non-critical parameters may be missing for some or all days.
//	MISSING DATA IS ASSUMED TO BE FLAGGED AS -999.0 FOR FLOAT 	
// 	PARAMETERS AND  9999 FOR INTEGRAL PARAMETERS.				
//																
//	Finally, there are some special cases which we need to make	
//	simplifying assumptions for:								
//																
//	Kdown_direct and Kdown_diffuse supplied for the zone.		
//	 	- these are likely measured as "flat surface" 			
//			irradiance where the sun is above the horizon		
//			since it is probably from a levelled platform.		
//		- it would be tiresome to compute sloped surface		
//			versions of these.  								
//		- given this we will assume it is in fact sloped surface
//			values we are getting (i.e based on slope and 		
//			aspect of zone).									
//		- our problem then is how to adjust Tmax_day without 	
//			computing the RADRATE (which is an hourly thing).	
//			Again, we have to assume that if Kdown_direct and	
//			Kdown_diffuse are good enough for this zone the		
//			Tmax_day is good enough for this zone.  			
//	 	- note that we will have to make use of a base station	
//			LAI to allow modelling of LAI disturbances on 		
//			Tmax_day.											
//																
//	only one of Kdown_direct and Kdown_diffuse					
//		- at the moment I treat this case as neither are given  
//			since it would mean									
//			inferring the atmospheric attenuation based on the	
//			supplied variable.  								
//																
//	trans supplied for the zone.								
//		- this refers to daily total atmospheric transmittance	
//		- if it is given and we dont have Kdown's we synthesize 
//			the Kdowns on an hourly basis using this value		
//		- otherwise we deal with corrections for cloudiness to	
//			the clear sky transmissivity						
			
// 	Sept 11 1997 CT						
//	added parameters for snow/rain transition		
//===============================================================================================================================
#include <stdio.h>

#include <iostream>

#include <math.h>
#include "CHESS.h"
#include "Constants.h"
using namespace std;


void		zone_daily_F(struct 	patch_object 	*patch, struct command_line_object *command_line,
						struct 	date 	current_date)
{
	//===========================================================================================================================
	//	Make use of more accurate basin daylength if it is not given and the zone horizons are 0
	//	If the zone horizons are non zero we use the daylength computed from hourly solar geometry.
	//===========================================================================================================================


	if ((patch->daylength_flag == 0) && (patch->e_horizon == 0) && (patch->w_horizon == 0)){
		//=======================================================================================================================
		//		adjust the simulated radiation for new dayl
		//=======================================================================================================================
		if (patch->metv.dayl == 0) {
			printf(" patch->metv.dayl is zero %f %f \n", patch->metv.dayl, patch->daylength);
			getchar();
		}
		if ((patch->Kdown_direct_flag == 0) || (patch->Kdown_diffuse_flag == 0)){
			patch->Kdown_direct  = patch->Kdown_direct	* (patch->daylength / patch->metv.dayl);//xu. daylength dayl?
			patch->Kdown_diffuse = patch->Kdown_diffuse	* (patch->daylength / patch->metv.dayl);
		}
		patch->metv.dayl = patch->daylength;
	}

	if (patch->metv.dayl == 0 ){
		//cout << "daylength is zero" << endl;
		//cout << patch->metv.dayl << " " << patch->daylength << endl;
		//cout << patch->e_horizon << " " << patch->w_horizon << endl;
		//cout << patch->ID << endl;
		//patch->metv.dayl = patch->daylength;
		//getchar();
	}

	//===========================================================================================================================
	//	Deretmine if we need to adjust Kdowns or metv.tmax.
	//
	//	We assume that if either Kdown_diffuse or Kdown_direct was not supplied then themetv.tmax needs adjustment.
	//	This is because the adjustment should not be needed
	//		if we had good enough data to get Kdowns.
	//===========================================================================================================================
	if ((patch->Kdown_direct_flag == 0) || (patch->Kdown_diffuse_flag == 0)){
		//=======================================================================================================================
		//	Adjust the computed Kdown's for cloud fraction if they were not read in directly.
		//=======================================================================================================================
		patch->Kdown_direct  = patch->Kdown_direct  * patch->Kdown_direct_adjustment;
		patch->Kdown_diffuse = patch->Kdown_diffuse	* patch->Kdown_diffuse_adjustment;

		//=======================================================================================================================
		//	radrat (unitless)
		//
		//	Equation 2a & 2b , Page 5, "MTCLIM"
		//=======================================================================================================================
		patch->radrat = (patch->Kdown_direct + patch->Kdown_diffuse) / (patch->Kdown_direct_flat + patch->Kdown_diffuse_flat);//xu. difference between with or without flat?
	}

	//===========================================================================================================================
	//	If no PAR or only diffuse PAR is given get the other PAR's,	We assume the ratio of diffuse to direct PAR is a closed
	//		form equation of the ratio of diffuse to direct Kdown.
	//===========================================================================================================================
	if ((patch->PAR_direct == -999.0) && (patch->PAR_diffuse == -999.0)){
		patch->PAR_direct  = 1000.0 * patch->Kdown_direct * RAD2PAR * EPAR;
		patch->PAR_diffuse = 1000.0 * patch->Kdown_diffuse * RAD2PAR * EPAR;
	}
	else if (patch->PAR_direct == -999.0){
		patch->PAR_direct = patch->PAR_diffuse * (patch->Kdown_direct / patch->Kdown_diffuse);
	}
	else if (patch->PAR_diffuse == -999.0){
		patch->PAR_diffuse = patch->PAR_direct * (patch->Kdown_diffuse / patch->Kdown_direct);
	}

	//if (current_date.year == 1962 && current_date.month == 6 && current_date.day == 6 && patch->ID == 457){
	//	printf("patch->kdown direct and diffuse is %12.8f %12.8f \n", patch->Kdown_diffuse, patch->Kdown_direct);
	//	printf("zone_daily_f se is %12.8f %12.8f \n", patch->Kdown_diffuse, patch->Kdown_direct);
	//	printf("daylength se is %12.8f %12.8f \n", patch->daylength , initial_daylength);
	//}

	//patch->PAR_diffuse_initial = patch->PAR_diffuse;
	//patch->PAR_direct_initial  = patch->PAR_direct;

	//=====================================================================================================================================
	//	daytime rain duration - (seconds)
	//	This is the daytime time over which rain fell.	It is used to adjust the vpd to the saturation
	//	vpd during the rain hours and recompute the vpd	for the rest of the day by adjusting the mean value
	//=====================================================================================================================================
	// if i put daytime rain duration somewhere else, the potential evapotranspiration can be less than zero
	patch->daily_clim->daytime_rain_duration = -999.; //xu. ???
	if (patch->daily_clim->daytime_rain_duration != -999.){
		patch->daytime_rain_duration = patch->daily_clim->daytime_rain_duration * 3600;
	}
	else{
		if (patch->rain == 0 || (patch->snow != 0)){
			patch->daytime_rain_duration = 0;
		}
		else{
			//---------------------------------------------------------------------------------------------------------------------------
			//The daytime_rain_duration was maintained as 1/10 of dayl, for further version the observed data can be inputed.
			//---------------------------------------------------------------------------------------------------------------------------
			patch->daytime_rain_duration = patch->metv.dayl/10.; //guoping double check
		}
	}

	return;
}
