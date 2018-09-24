//--------------------------------------------------------------
// 								
//		compute_litter_rain_stored				
//								
//								
//	NAME							
//	compute_litter_rain_stored -  rain intercepted by litter and 	
//		updates rain falling through.			
//								
//	SYNOPSIS						
//	double	compute_litter_rain_stored(				
//			struct	canopy_strata_object	*);	
//								
//	returns:						
//	rain_storage (m) - amount of rain left in canopy	
//								
//								
//	also updates rain_throughfall, and litter evaporation 	
//		for the patch					
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
#include <iostream>
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"


using std::cout;
using std::endl;

double	compute_litter_rain_stored(
								   int	verbose_flag,
								   struct	patch_object	*patch) 
{
    //---------------------------------------------------------------------------------------------------------------------------
	//	Local variable definition.				
    //---------------------------------------------------------------------------------------------------------------------------
	double	potential_interception;
	double	potential_evaporation;
	double	rain_storage;
	double	storage_evaporated;
	double	potential_interception_evaporated;
	double	throughfall;
	struct	litter_object *litter;
	double  litter_preday_rain_stored;
	
	litter = &(patch->litter);

	litter_preday_rain_stored=litter->preday_rain_stored; //added by guoping for check waterbalance
    //---------------------------------------------------------------------------------------------------------------------------
	//	Transfer potential evaporation to a working variable.	
	//--------------------------------------------------------------
	potential_evaporation = patch->potential_evapotranspiration;
			
    //---------------------------------------------------------------------------------------------------------------------------
	//	Compute amount potentially intercepted.			
	//	m = m2PlANT / m2ground *  ( (kg  / m2 * day * m2PLANT )	* ( 1 m3 H20 / 1000 kg H20 )			
    //---------------------------------------------------------------------------------------------------------------------------
	//potential_interception = min(patch->rain_throughfall, litter->rain_capacity - litter->rain_stored);
	potential_interception = min(patch->rain_throughfall, litter->rain_capacity - litter_preday_rain_stored); //updated by guoping

	if (potential_interception < 0.0) 
		potential_interception = 0.0;

    //---------------------------------------------------------------------------------------------------------------------------
	//	Add amount not potentially intercepted to throughfall. Unites conversion:: m = m - m 						
    //---------------------------------------------------------------------------------------------------------------------------
	throughfall  = patch->rain_throughfall - potential_interception;

    //---------------------------------------------------------------------------------------------------------------------------
	//	Compute amount of storage evaporated. Unites conversion:: m = m							
    //---------------------------------------------------------------------------------------------------------------------------
	//storage_evaporated = min(potential_evaporation,litter->rain_stored); //Guoping:: original
	storage_evaporated = min(potential_evaporation,litter->preday_rain_stored); //Guoping updated for water balance

    //---------------------------------------------------------------------------------------------------------------------------
	//	Update amount of rain in storage after evaporation. Unites conversion:: m = m							
    //---------------------------------------------------------------------------------------------------------------------------
	litter_preday_rain_stored  -= storage_evaporated;

    //---------------------------------------------------------------------------------------------------------------------------
	//	Update potentail evaporation. Unites conversion:: m = m							
    //---------------------------------------------------------------------------------------------------------------------------
	potential_evaporation -= storage_evaporated;

    //---------------------------------------------------------------------------------------------------------------------------
	//	Compute amount of potential interception evaporated. Unites conversion:: m = m							
    //---------------------------------------------------------------------------------------------------------------------------
	potential_interception_evaporated  = min(potential_evaporation, potential_interception);
	//potential_interception_evaporated  = max(0.0, potential_interception_evaporated);

    //---------------------------------------------------------------------------------------------------------------------------
	//	Update the potential interception after evaporation. Unites conversion:: m -= m							
    //---------------------------------------------------------------------------------------------------------------------------
	potential_interception -= potential_interception_evaporated;

    //---------------------------------------------------------------------------------------------------------------------------
	//	Compute amount of evaporation that happened. Unites conversion:: m = m + m						
    //---------------------------------------------------------------------------------------------------------------------------
	patch->evaporation_surf_litter = storage_evaporated +	potential_interception_evaporated;
	/*
	if (patch->evaporation_surf_litter < 0.){
		cout << patch->evaporation_surf_litter << " " << storage_evaporated << " " << potential_interception_evaporated << endl;
		cout << patch->potential_evapotranspiration << endl;
		cout << patch->ID << " " << endl;
		getchar();
	}
	*/
    //---------------------------------------------------------------------------------------------------------------------------
	//	Adjust the amount of remaining potential evaporation	
    //---------------------------------------------------------------------------------------------------------------------------
	patch->potential_evapotranspiration -= patch->evaporation_surf_litter;

    //---------------------------------------------------------------------------------------------------------------------------
	//  Update rain storage. Unites conversion:: m = m							
    //---------------------------------------------------------------------------------------------------------------------------
	//rain_storage = min( litter->rain_stored + potential_interception, litter->rain_capacity );  //guoping:: original
	if((litter_preday_rain_stored+potential_interception-litter->rain_capacity)<0.)
		rain_storage =litter_preday_rain_stored + potential_interception;
	else {
	    rain_storage = litter->rain_capacity;
		throughfall += (litter_preday_rain_stored+potential_interception-litter->rain_capacity);
	}

    //---------------------------------------------------------------------------------------------------------------------------
	//	Update rain throughfall. Unites conversion:: m += m							
    //---------------------------------------------------------------------------------------------------------------------------
	//throughfall            += max(potential_interception - (rain_storage - litter->rain_stored),0);
	patch->rain_throughfall = throughfall;

	return( rain_storage);
} //end compute_litter_rain_stored
