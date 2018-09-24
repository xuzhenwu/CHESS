//--------------------------------------------------------------
// 								
//		compute_rain_stored				
//								
//								
//	NAME							
//	compute_rain_stored -  rain intercepted by stratum and 	
//		updates rain flaaing through.			
//								
//	SYNOPSIS						
//	double	compute_rain_stored(				
//			double	*,				
//			struct	canopy_strata_object	*);	
//								
//	returns:						
//	rain_storage (m) - amount of rain left in canopy	
//								
//	also updates the following state variables:		
//								
//	*rain (m) - rain falling on next stratum		
//	stratum->potential_evaporation (m/day) - amount	
//		of water that can be still evaporated that day	
//	OPTIONS							
//	*rain (m) - amount of rain on stratum for whole day	
//	canopy_strata_object - state of canopy strata		
//								
//	DESCRIPTION						
//								
//	Updates amount of rain stored after using supplied 	
//	daytime potential evaporation,  rainfall 			 
//	to evaporate and input respectively to the storage.	
//	Evaporation from the storage is performed first.	
//								
//	PROGRAMMER NOTES					
//								
//								
//--------------------------------------------------------------
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

double	compute_rain_stored(
							int	verbose_flag,
							double	*rain,
							struct	canopy_strata_object	*stratum, double *potential_evapotranspiration) 
{
	//----------------------------------------------------------------------------------------------------
	//	Local function declaration				              
	//----------------------------------------------------------------------------------------------------
	double	compute_potential_rain_interception(
		int	,
		double	,
		struct	canopy_strata_object *);
	//----------------------------------------------------------------------------------------------------
	//	Local variable definition.				
	//----------------------------------------------------------------------------------------------------
	double	potential_interception;
	double	potential_evaporation;
	double	rain_storage;
	double	storage_evaporated;
	double	potential_interception_evaporated;
	double	throughfall;

	//----------------------------------------------------------------------------------------------------
	//	Transfer potential evaporation to a working variable.	
	//----------------------------------------------------------------------------------------------------
	potential_evaporation = stratum->potential_evaporation;

	//----------------------------------------------------------------------------------------------------
	//	Compute amount potentially intercepted.			
	//	m = m2PlANT / m2ground *  ( (kg  / m2 * day * m2PLANT )	
	//		* ( 1 m3 H20 / 1000 kg H20 )			
	//----------------------------------------------------------------------------------------------------
	 potential_interception = compute_potential_rain_interception(verbose_flag, *rain, stratum );
     stratum->rain_interception = potential_interception;
	//if(stratum->rain_interception>0 && *rain>0.005){
	//		printf("1potential interception %f %f\n", *rain,stratum->gap_fraction);
	//		printf("2potential interception %f %f\n", stratum->epv.proj_pai,stratum->defaults->specific_rain_capacity);
	//		printf("3potential interception %f \n",stratum->preday_rain_stored);
	//	printf("potential interception %f %f \n",stratum->rain_interception, *rain);
	//	getchar();
	//}

	//----------------------------------------------------------------------------------------------------
	//	Add amount not potentially intercepted to throughfall.	m = m - m 						
	//----------------------------------------------------------------------------------------------------
	throughfall  = *rain - potential_interception;


	//----------------------------------------------------------------------------------------------------
	//	Compute amount of storage evaporated.	m = m							
	//----------------------------------------------------------------------------------------------------
	storage_evaporated = min(potential_evaporation,stratum->preday_rain_stored);
	storage_evaporated = min(storage_evaporated, *potential_evapotranspiration);
	*potential_evapotranspiration -= storage_evaporated;

	//----------------------------------------------------------------------------------------------------
	//	Update amount of rain in storage after evaporation.	m = m							
	//----------------------------------------------------------------------------------------------------
	stratum->rain_stored  =  stratum->preday_rain_stored - storage_evaporated;

	//----------------------------------------------------------------------------------------------------
	//	Update potentail evaporation. m = m							
	//----------------------------------------------------------------------------------------------------
	potential_evaporation -= storage_evaporated;

	//----------------------------------------------------------------------------------------------------
	//	Compute amount of potential interception evaporated. m = m							
	//----------------------------------------------------------------------------------------------------
	potential_interception_evaporated  = min( potential_evaporation, potential_interception);
	potential_interception_evaporated  = min(potential_interception_evaporated, *potential_evapotranspiration);
	*potential_evapotranspiration -= potential_interception_evaporated;

	//----------------------------------------------------------------------------------------------------
	//	Update the potential interception after evaporation. m -= m							
	//----------------------------------------------------------------------------------------------------
	potential_interception -= potential_interception_evaporated;
	
	//----------------------------------------------------------------------------------------------------
	//	Compute amount of evaporation that happened. 	m = m + m						
	//----------------------------------------------------------------------------------------------------
	stratum->evaporation =  storage_evaporated + potential_interception_evaporated;
	
	//----------------------------------------------------------------------------------------------------
	//	Adjust the amount of remaining potential evaporation	
	//----------------------------------------------------------------------------------------------------
	stratum->potential_evaporation -= stratum->evaporation;

	//----------------------------------------------------------------------------------------------------
	//  Update rain storage.	m = m							
	//----------------------------------------------------------------------------------------------------
	rain_storage = min( stratum->rain_stored + potential_interception,
		stratum->epv.all_pai * stratum->defaults->specific_rain_capacity );

	//----------------------------------------------------------------------------------------------------
	//	Update rain throughfall. m += m							
	//----------------------------------------------------------------------------------------------------
	throughfall += max(potential_interception - (rain_storage - stratum->rain_stored),0);
	*rain = throughfall;

	return( rain_storage);
} //end compute_rain_stored
