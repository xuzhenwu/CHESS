/*------------------------------------------------------------------------------------------------------------------------------
					update_drainage_stream			                                            
											                                                    
Description of this module: This module calculates the generation of surface and subsurface flow for stream-types cells.
The generated flow was channalized and not routed.
					                                                        
Created by Guoping Tang								        
-------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <iostream>
#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"

using std::cout;
using std::endl;

void  update_drainage_stream(
struct patch_object *patch,
	int routing_flag,
	double time_int,
	int verbose_flag,
	int grow_flag)
{

	//---------------------------------------------------------------------------------------------------------------------------
	//	Local variable definition.				                                                
	//---------------------------------------------------------------------------------------------------------------------------
	int  d=0;
	double return_flow = 0;                      //Unit: m
	double NO3_leached_to_subsurface=0.,NO3_leached_to_surface=0.;                 //Unit: kg/m2, N_leached_total,
	double NH4_leached_to_surface = 0.;
	double available_sat_water = 0., route_to_stream = 0., route_to_surface = 0.; //Unit: changed into m from m3
	double Qout = 0., Qin = 0.;  // Unit:: m
	double gamma, total_gamma; // percent_tobe_routed;
	double Nout;  //Unit: kg/m2 //Nin, 


	time_int = 1;
	patch->DOC_loss += patch->cdf.total_DOC_loss;
	patch->DON_loss += patch->ndf.total_DON_loss;
	
	double water = patch->sat_deficit -patch->rz_storage - patch->unsat_storage; //-patch->detention_store-patch->detention_store
	//---------------------------------------------------------------------------------------------------------------------------
	//	for now there should be no recomputing of gamma for 	                           
	//	streams because they do not route water to downslope neighbours					
	//---------------------------------------------------------------------------------------------------------------------------
	total_gamma         = patch->gamma;
	available_sat_water = max(patch->soil_defaults->soil_water_cap - water, 0.0);
	//---------------------------------------------------------------------------------------------------------------------------
	//	calculate amuount of water output to stream as baseflow                             
	//---------------------------------------------------------------------------------------------------------------------------
	if (total_gamma <= ZERO) {
		gamma = patch->soil_defaults->Ksat_0 * patch->soil_defaults->m_z * 2.0;
	}
	else {
		gamma = total_gamma;
	}
	route_to_stream = compute_varbased_flow(patch->std, gamma, patch->soil_defaults->m_z,
		patch->soil_defaults->active_zone_z,
		water, patch->soil_defaults->soil_water_cap);

	if (route_to_stream < 0.0)                 route_to_stream = 0.0;
	if (route_to_stream > available_sat_water) route_to_stream = available_sat_water;

	//---------------------------------------------------------------------------------------------------------------------------
	// compute Nitrogen leaching amount with baseflow		                                
	//---------------------------------------------------------------------------------------------------------------------------
	if (grow_flag > 0) {
		NO3_leached_to_subsurface = compute_N_leached(
			verbose_flag,
			patch->soil_ns.nitrate,
			route_to_stream,
			patch->sat_deficit,
			patch->soil_defaults->soil_water_cap,
			patch->soil_defaults->m_z,
			gamma,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->N_decay_rate,
			patch->soil_defaults->active_zone_z,
			patch->soil_defaults->soil_depth,
			patch->soil_defaults->mobile_N_proportion);
		patch->surface_leach_NO3_out += NO3_leached_to_subsurface;
		patch->soil_ns.nitrate       -= NO3_leached_to_subsurface;
	}

	patch->subsurface_Qout       += route_to_stream;  //added by guoping for water balance later

	//---------------------------------------------------------------------------------------------------------------------------
	// calculate any return flow to the stream in this patch and route any infiltration excess 
	//---------------------------------------------------------------------------------------------------------------------------
	if ((patch->sat_deficit - patch->rz_storage - patch->unsat_storage) < -1.0*ZERO) {
		return_flow = compute_varbased_returnflow(patch->std,
			patch->rz_storage + patch->unsat_storage,
			patch->sat_deficit, &(patch->litter));
		patch->detention_store += return_flow;
		patch->sat_deficit = 0;
		patch->rz_storage = 0.;
		patch->unsat_storage = 0.;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	calculated any N-transport associated with return flow                                  
	//	-note available N reduced by what has already been lost due to subsurface routing above 
	// 	note only nitrate is assumed to follow return flow		                                
	//---------------------------------------------------------------------------------------------------------------------------
	if (return_flow > ZERO) {
		Nout = compute_N_leached(
			verbose_flag,
			patch->soil_ns.nitrate,
			return_flow,
			0.0,
			0.0,
			patch->soil_defaults->m_z,
			gamma,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->N_decay_rate,
			patch->soil_defaults->active_zone_z,
			patch->soil_defaults->soil_depth,
			patch->soil_defaults->mobile_N_proportion);
		patch->surface_NO3     += Nout;
		patch->soil_ns.nitrate -= Nout;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	route water and nitrogen lossed due to infiltration excess                              
	//	note we assume that this happens before return_flow losses                              
	//---------------------------------------------------------------------------------------------------------------------------
	if ((patch->detention_store > patch->soil_defaults->detention_store_size) &&
		(patch->detention_store > ZERO)) {
		Qout = (patch->detention_store - patch->soil_defaults->detention_store_size);
		Nout = (min(1.0, Qout / patch->detention_store)) * patch->surface_NO3;
		NO3_leached_to_surface = Nout;
		patch->surface_NO3    -= Nout;
		patch->surface_leach_NO3_out += Nout;
		
		Nout = (min(1.0, Qout / patch->detention_store)) * patch->surface_NH4;
		NH4_leached_to_surface        = Nout;
		patch->surface_NH4           -= Nout;
		patch->surface_leach_NH4_out += Nout;

		patch->detention_store -= Qout;
		patch->surface_Qout    += Qout; //added by guoping for water balance check later
		route_to_surface        = Qout;
	}

	double unsat_out = min(patch->unsat_storage, 4. / 5.*route_to_stream);
	patch->unsat_storage -= unsat_out;
	double rz_out = min(patch->rz_storage, 1. / 5.*route_to_stream);
	patch->rz_storage -= rz_out;
	patch->sat_deficit += (route_to_stream - unsat_out - rz_out);

	patch->NO3_leached += (NO3_leached_to_subsurface + NO3_leached_to_surface);

	/*
	for (j = 0; j < patch->num_neighbours; j++) {
	neigh = patch->neighbours[j].patch;
	//---------------------------------------------------------------------------------------------------------------------------
	//	- now surface water
	//	surface stores should be updated to facilitate transfer added net surface water transfer to detention store
	//---------------------------------------------------------------------------------------------------------------------------
	Qin = (patch->neighbours[j].gamma * route_to_stream);
	neigh->subsurface_Qin  += Qin; // updates patch surface_qin
	neigh->sat_deficit     -= Qin;
	Qout = (patch->neighbours[j].gamma *route_to_surface);
	neigh->detention_store += Qout;
	neigh->surface_Qin     += Qout;
	}
	*/

//	if (patch->sat_deficit_z < 0. && patch->ID==7322){
	//	cout << patch->sat_deficit_z << " " << patch->sat_deficit<< endl;
		//cout << current_date.year << " " << current_date.month << " " << current_date.day << endl;
	//	cout << patch->ID << endl;
	//	getchar();
	//}

} /*end update_drainage_stream.c*/

