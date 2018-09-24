//---------------------------------------------------------------------------------------------------------------------
//					compute_subsurface_routing			
//											
//	Descripiton: route surface and subsurface flow and dissolved nutrients
//---------------------------------------------------------------------------------------------------------------------

#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
#include <iostream>
using std::cout;
using std::endl;

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

void  compute_subsurface_routing_new(patch_object *patch,struct command_line_object *command_line,
						 int n_timesteps,
						 struct	date	current_date)
{
	//-----------------------------------------------------------------------------------------------------------------
	//	Local function definition.				
	//-----------------------------------------------------------------------------------------------------------------
	void update_drainage_stream(struct patch_object *,int,double,int,int);
	void update_drainage_land(struct patch_object *,int,double,int,	int);
	double  compute_z_final(
		int,
		double,
		double,
		double,
		double,
		double);
	//-----------------------------------------------------------------------------------------------------------------
	//	Local variable definition.				
	//-----------------------------------------------------------------------------------------------------------------
	double time_int=24;
	double water_balance;
	
	//-----------------------------------------------------------------------------------------------------------------
	//	1. for roads, saturated throughflow beneath road cut is routed to downslope patches; saturated     
	//     throughflow above the cut and overland flow is routed to the stream                             
	//  2. for streams, no routing - all exported from basin	                                           
	//	3. regular land patches - route to downslope neighbours                                            
	//-----------------------------------------------------------------------------------------------------------------
	if (patch->drainage_type == STREAM){
		update_drainage_stream(patch, command_line->routing_flag, time_int, command_line->verbose_flag,
			command_line->grow_flag);
	}
	else{
		update_drainage_land(patch, command_line->routing_flag, time_int, command_line->verbose_flag,
			command_line->grow_flag);
	}
	

	//Keep update sat_deficit_z when soil saturation deficit is changed
	patch->sat_deficit_z = compute_z_final(
		command_line->verbose_flag,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		0.0,
		-1.0 * patch->sat_deficit);

	/*	
	//Finally check water balance
	water_balance = patch->rain + patch->snow
			+ patch->preday_rain_stored        - patch->rain_stored                     // rain stored by canopy
			+ patch->preday_snow_stored        - patch->snow_stored                     // snow stored by canopy
			+ patch->litter.preday_rain_stored - patch->litter.rain_stored              // rain stored by litter
			+ patch->preday_snowpack           - patch->snowpack.water_equivalent_depth // snowpack on the ground
			+ patch->detention_store_preday    - patch->detention_store                 // water detened on soil surface
			+ patch->rz_storage_preday         - patch->rz_storage                      // water stored in root zone layer
			+ patch->unsat_storage_preday      - patch->unsat_storage                   // water stored in unsat zone layer
			- patch->sat_deficit_preday        + patch->sat_deficit                     // water needed to saturate the whole soil
			- patch->AET                                                                // water evapotranspirated
			- patch->gw_drainage                                                        // groundwater drainage
			- patch->subsurface_Qout - patch->surface_Qout
			+ patch->subsurface_Qin  + patch->surface_Qin;                                                      
	                  

	if ((water_balance > 0.00000001)|| (water_balance < -0.00000001)){
		printf("water_balance in patch_daily_f_new.cpp is NOT ZERO %12.8f\n",water_balance);
		printf("patch->rain %12.8f \n",patch->rain);
		printf("patch->snow %12.8f \n",patch->snow);

		printf("\n \n");
		printf("total AET %12.8f \n", patch->PET);
		
		printf("Changes in rain_stored %12.8f \n", patch->rain_stored - patch->preday_rain_stored);
		printf("Changes in snow_stored %12.8f \n", patch->snow_stored - patch->preday_snow_stored);
		printf("Changes in litter.rain_stored %12.8f \n", patch->litter.preday_rain_stored - patch->litter.rain_stored);
		printf("Changes in rz_storage %12.8f \n", patch->rz_storage - patch->rz_storage_preday);
		printf("Changes in unsat_storage %12.8f \n", patch->unsat_storage - patch->unsat_storage_preday);
		printf("Changes in sat_deficit %12.8f \n", patch->sat_deficit_preday - patch->sat_deficit);
		printf("Changes in snowpack %12.8f \n", patch->snowpack.water_equivalent_depth-patch->preday_snowpack);
		printf("Changes in surface detention_store %12.8f \n", patch->detention_store-patch->detention_store_preday);
		printf("patch->gw_drainage is %12.8f \n",patch->gw_drainage);
		printf("subsurface and surface Qin  %12.8f %12.8f \n", patch->subsurface_Qin, patch->surface_Qin);
		printf("subsurface and surface Qout %12.8f %12.8f \n", patch->subsurface_Qout, patch->surface_Qout);
		printf("time and id is %d %d %d %d \n",current_date.year,current_date.month,current_date.day,patch->ID);
		getchar();
	}
	*/

return;
}
