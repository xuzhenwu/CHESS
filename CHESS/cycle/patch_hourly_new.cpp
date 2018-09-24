//=====================================================================================================================
//						patch_hourly							
//	DESCRIPTION													
//																
//	This routine performs simulation cycles on an identified	
//	canopy_stata in the patch.									
//																
//	PROGRAMMER NOTES											
//=====================================================================================================================
#include "CHESS.h"

#include "Constants.h"
#include <iostream>

using std::cout;
using std::endl;

void  patch_hourly(struct patch_object *patch, struct command_line_object *command_line, struct	date current_date)
{
	//=================================================================================================================
	//	Local Variable Declarations.						
	//=================================================================================================================
	int	ML;
	double	air_mass_array[22] = { 0.0, 2.90, 3.05, 3.21, 3.39, 3.69, 3.82, 4.07, 4.37, 4.72, 5.12,
		5.60, 6.18, 6.88, 7.77, 8.90, 10.39, 12.44, 15.36, 19.79, 26.96, 30.00 };
	double	Kdown_direct_flat_toa, temp;

	//=================================================================================================================
	//	Figure out solar geometry and air mass number even if we are reading in radiation's.  
	//  Hour angle (in radians)                                 
	//                                                      
	//  Eq. 8, Appendix D, "MTCLIM"                             
	//                                                          
	//  hour_angle =  (hour*3600-43200)*0.0041667*DtoR;        
	//                                                            
	//  If I understand the equation properly it states that that at noon (43200 seconds in to the day) the hour
	//  angle is zero (the sun is at zenith). Although the solar noon changes with date the maximum
	//  deviation is 16 minutes so we ignore it for now. e.g. Table A7.1 Appendix &, Jones, "Plants and
	//                                  Microclimate."              */
	//=================================================================================================================
	patch->hourly->hour_angle = (current_date.hour * 3600 - 43200)*0.0041667*DtoR;
	patch->hourly->cos_hour_angle = cos(patch->hourly->hour_angle);
	patch->hourly->sin_hour_angle = sin(patch->hourly->hour_angle);

	//=================================================================================================================
	//  Cosine of Solar Zenith Angle (no units)              
	//  Eq. 11, Appendix D, "MTCLIM"                         
	//=================================================================================================================
	patch->hourly->cos_sza = patch->cos_declin	* patch->cos_latitude * patch->hourly->cos_hour_angle
		+ patch->sin_declin * patch->sin_latitude;
	
	//if (patch->ID == 7315 && current_date.month==1){
	//	cout << patch->hourly->cos_sza <<" "<< patch->e_horizon << " " << patch->w_horizon << endl;
	//	cout << current_date.year << " " << current_date.month << " " << current_date.day << endl;
	//	cout << current_date.hour << endl;
	//	getchar();
	//}
	
	//=================================================================================================================
	//	We only bother to compute solar geometry if the sun is up 
	//=================================================================================================================
	if (patch->hourly->cos_sza > 0){
		//=============================================================================================================
		//		day length		(seconds) 						
		//		increment daylight length by one hour.			
		//		basin->daylength += 3600; 
		//=============================================================================================================
		//		Optical Air Mass (no units)                           
		// 		Eq. 6., Appendix D "MTCLIM"                           
		//=============================================================================================================
		patch->hourly->optical_air_mass = 1.0 / (patch->hourly->cos_sza + 1.0e-7);

		if (patch->hourly->optical_air_mass > 2.9) {
			ML = ((int)(acos(patch->hourly->cos_sza) / 0.0174533)) - 69;
			if (ML < 1) ML = 1;
			if (ML > 21) ML = 21;
			patch->hourly->optical_air_mass = air_mass_array[ML];
		}
		//=============================================================================================================
		//		Precompute 	some angle formulae					
		//=============================================================================================================
		patch->hourly->cos_declin_cos_hourangle = patch->cos_declin * patch->hourly->cos_hour_angle;
		patch->hourly->cos_declin_sin_hourangle = patch->cos_declin * patch->hourly->sin_hour_angle;
	}

	//=================================================================================================================
	//	Note that solar geometry except for cos_sza may be garbage	
	//	if cos_sza < 0 (no daylight).								
	//=================================================================================================================

	//=================================================================================================================
	//	Compute hourly radiation forcings.							
	//	Kdown_direct or Kdown_diffuse.								
	//=================================================================================================================
	if ((patch->Kdown_direct_flag == 0) || (patch->Kdown_diffuse_flag == 0) || (patch->daylength_flag == 0)){
		//=============================================================================================================
		//  Accumulate incoming Kdown if the sun is up.                 
		//                                                              
		//  We define the day length as the period where the cos_sza >0.
		//  It is likely that Kdown_diffuse is non-zero even if cos_sza < 0 but we ignore this for now.                
		//=============================================================================================================

		if (patch->hourly->cos_sza  > 0){

			//=========================================================================================================
			/*		Atmospheric attenuation to direct radiation	(no units)  */
			//=========================================================================================================
			patch->hourly->direct_attenuation = pow(patch->atm_trans, patch->hourly->optical_air_mass);

			//=========================================================================================================
			/*		Downwelling total irradiance at BOA along sza			*/
			/*		W / m ** 2  = W /m ** 2  								*/
			//=========================================================================================================
			patch->hourly->Kdown_BOA = patch->Io * patch->hourly->direct_attenuation;

			//=========================================================================================================
			//  Kdown_direct    (W/(m2)) 	                               
			//  Eq. 5, Appendix D, "MTCLIM"                                
			//                                                             
			//  We add in the constraint that the sun must be above the  horizon of the zone where the horizon is 
			//  defined by the east and west horizons (assumes sun moves from east to west with refernce to the zone).
			//  Note that we define the horizons as the angle from a flat surface at the zone and  NOT as the slope of
			//  the zone as zero.  We do this so that the horizon can also be used to define a view factor.      
			//                                                              
			//  Note that we only accumulate the 24hr total Kdown_direct so if you need the hourly Kdown_direct
			//  you have to make a new state variable.                                         
			//																
			//	Note that cos_sza = 1.0 if the sun is at a 90 degree angle to the flat surface at the zone.							
			//																
			// 	It is possible that the zone's east and west horizons do not match that of the basins.  At present,
			//  the basin's east and west horizons define length of daylight.  SO it is possible that, we consider
			//  that daylight is available evcen though the zone is still in shade.  We should decide if we should
			//  define daylength (and horizons) based on basin or zone.	
			//=========================================================================================================

			if ((patch->hourly->cos_sza > patch->e_horizon) && (patch->hourly->cos_sza > patch->w_horizon)){
				//=====================================================================================================
				//	Increment zone daylength				
				//=====================================================================================================
				if (patch->daylength_flag == 0) patch->metv.dayl += 3600.0;
                
				/*
				if (patch->ID == 3842)
				{
					cout << "current_date.hour is " << current_date.hour << endl;
					cout << patch->cos_declin << " " << patch->sin_declin << endl;
					cout << patch->cos_latitude << " " << patch->sin_latitude << endl;
					cout << patch->hourly->cos_sza << endl;
					getchar();
				}
				*/


				//=====================================================================================================
				//	Only do the following if we dont have direct radiation		
				//=====================================================================================================
				if ((patch->Kdown_direct_flag == 0) || (patch->Kdown_diffuse_flag == 0)){
					//-------------------------------------------------------------------------------------------------
					//     Kdown_direct_flat   (W/(m2))                           
					//     Required to adjust max temp.                          
					//-------------------------------------------------------------------------------------------------
					patch->hourly->Kdown_direct_flat = patch->hourly->cos_sza * patch->hourly->Kdown_BOA;

					//-------------------------------------------------------------------------------------------------
					//     Cosine of beam slope angle. (no units)                  
					//     Eq. 8 Appendix D of "MTCLIM"                            
					//-------------------------------------------------------------------------------------------------
					patch->hourly->cos_beam_slope
						= -1 * patch->sin_slope * patch->sin_aspect
						* patch->hourly->cos_declin_sin_hourangle
						+ (-1 * patch->cos_aspect * patch->sin_slope
						* patch->sin_latitude + patch->cos_slope
						* patch->cos_latitude)
						* patch->hourly->cos_declin_cos_hourangle
						+ (patch->cos_aspect * patch->sin_slope
						* patch->cos_latitude + patch->cos_slope
						* patch->sin_latitude) * patch->sin_declin;

					//-------------------------------------------------------------------------------------------------
					//	if sun angle is below the slope angle then hill is blocking sun and Kdown_direct is zero		
					//-------------------------------------------------------------------------------------------------
					if (patch->hourly->cos_beam_slope < 0.0)
						patch->hourly->cos_beam_slope = 0.0;

					//-------------------------------------------------------------------------------------------------
					//		Kdown_direct	(W/(m2*day))							
					//-------------------------------------------------------------------------------------------------
					patch->hourly->Kdown_direct = patch->hourly->cos_beam_slope	* patch->hourly->Kdown_BOA;


					//-------------------------------------------------------------------------------------------------
					//		Diffuse Radiation Calculations			              
					//		Source Daymet - Peter Thorto			
					//		Further adjusts for horizon view factor.				
					//-------------------------------------------------------------------------------------------------
					if (patch->hourly->direct_attenuation > 0.000000001)
						Kdown_direct_flat_toa = patch->hourly->Kdown_direct_flat / patch->hourly->direct_attenuation;
					else
						Kdown_direct_flat_toa = 0.0;

					
					temp = sqrt(Kdown_direct_flat_toa	* patch->hourly->Kdown_direct_flat);
					if (Kdown_direct_flat_toa < 0.0000000001)
						patch->hourly->Kdown_diffuse_flat = 0.0;
					else
						patch->hourly->Kdown_diffuse_flat = temp * (1.0 - temp / Kdown_direct_flat_toa);

					patch->hourly->Kdown_diffuse = patch->hourly->Kdown_diffuse_flat * pow(cos(patch->slope / 2.0), 2.0);


					//-------------------------------------------------------------------------------------------------
					//		Convert calculation fro W/m^2 to Kj/(m^2*hr)
					//-------------------------------------------------------------------------------------------------
					//if (patch->ID==8018){
					//	printf("cos_sza is less than %f \n", patch->hourly->cos_sza);
					//	printf("current_date.hour is %d %d %d %d \n", patch->ID,current_date.month,current_date.day,current_date.hour);
					//	printf("current_date.hour is %f %f \n", patch->Kdown_direct_flat, patch->Kdown_diffuse_flat);
					//	printf("current_date.hour is %f %f  \n", patch->Kdown_direct, patch->Kdown_diffuse);
					//	getchar();
					//}

					patch->Kdown_direct_flat  += patch->hourly->Kdown_direct_flat * 3600 / 1000;
					patch->Kdown_direct       += patch->hourly->Kdown_direct * 3600 / 1000;
					patch->Kdown_diffuse_flat += patch->hourly->Kdown_diffuse_flat * 3600 / 1000;
					patch->Kdown_diffuse      += patch->hourly->Kdown_diffuse * 3600 / 1000;
				}
			}
		}
	}

	//printf("0 patch->direct %f %f \n", patch->Kdown_direct, patch->Kdown_diffuse);
	//printf("0 patch->direct %f %f \n", patch->PAR_direct, patch->PAR_diffuse);
	return;
}