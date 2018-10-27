//--------------------------------------------------------------------------------------------------------------------------------
//	update_drainage_land_new.cpp - creates a patch object				
//  Description of this module: This module simulates land-type surface and subsurface flow generattion and route
//  flow generated from a cell to its downslope cells
//  
//  Created by Guoping Tagn
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include "Functions.h"
#include "CHESS.h"
#include "Constants.h"

void  update_drainage_land(struct patch_object *patch,
	int routing_flag,
	double time_int,
	int verbose_flag,
	int grow_flag)
{

	//---------------------------------------------------------------------------------------------------------------------------
	//	Local variable definition.				
	//---------------------------------------------------------------------------------------------------------------------------
	int j, d=0;
	double NO3_leached_to_subsurface = 0, N_leached_to_stream = 0; // kg/m2
	double NO3_leached_to_surface = 0; // kg/m2 
	double NH4_leached_to_surface = 0; // kg/m2 
	double N_leached_total = 0;    // kg/m2 
	double return_flow = 0, route_to_surface = 0, route_to_subsurface = 0.;  // m3 
	double available_sat_water = 0; // m3 
	double Qin = 0, Qout = 0;   // m 
	double innundation_depth = 0, infiltration = 0; // m 
	double total_gamma = 0;
	double Nin = 0, Nout = 0; // kg/m2  
	struct patch_object *neigh;
	double water_balance = 0;


	patch->DOC_loss += patch->cdf.total_DOC_loss;
	patch->DON_loss += patch->ndf.total_DON_loss;
	//---------------------------------------------------------------------------------------------------------------------------
	//	calculate realistic soil saturation deficit 
	//---------------------------------------------------------------------------------------------------------------------------
	double water = patch->sat_deficit -patch->rz_storage - patch->unsat_storage; //patch->detention_store;

	//---------------------------------------------------------------------------------------------------------------------------
	//	recalculate gamma based on current saturation deficits to account the effect of changes in water table slope 	
	//---------------------------------------------------------------------------------------------------------------------------
	//total_gamma         = recompute_gamma(patch, patch->gamma);
	total_gamma = patch->gamma;
	available_sat_water = max(patch->soil_defaults->soil_water_cap - water, 0.0);

	//---------------------------------------------------------------------------------------------------------------------------
	//	calculate amuount of water output to patches this only computes subsurface flow, not overland flow	
	//---------------------------------------------------------------------------------------------------------------------------
	time_int = 1.;
	route_to_subsurface = compute_varbased_flow(patch->std, total_gamma, patch->soil_defaults->m_z,
		patch->soil_defaults->active_zone_z,water, patch->soil_defaults->soil_water_cap);

	if (route_to_subsurface < 0.0) route_to_subsurface = 0.0;
	if (route_to_subsurface > available_sat_water)	route_to_subsurface = available_sat_water;
	patch->subsurface_Qout += route_to_subsurface;

	//---------------------------------------------------------------------------------------------------------------------------
	// compute Nitrogen leaching amount				
	//---------------------------------------------------------------------------------------------------------------------------
	if (grow_flag > 0) {
		NO3_leached_to_subsurface = compute_N_leached(
			verbose_flag,
			patch->soil_ns.nitrate,
			route_to_subsurface,
			patch->sat_deficit,
			patch->soil_defaults->soil_water_cap,
			patch->soil_defaults->m_z,
			total_gamma,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->N_decay_rate,
			patch->soil_defaults->active_zone_z,
			patch->soil_defaults->soil_depth,
			patch->soil_defaults->mobile_N_proportion);
		patch->subsurface_leach_NO3_out += NO3_leached_to_subsurface;
		patch->soil_ns.nitrate -= NO3_leached_to_subsurface;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	calculate any return flow associated with this patch and route any infiltration excess.	return flow is flow leaving patch
	//  (i.e surface_Qout).	note that return flow that becomes detention storage is added to surface_Qin. Similarly with
	//  associated nitrogen	note we move unsat_storage into saturated storage in this case 
	//	saturated zone will be updated in compute_subsurface_routing, i.e becomes part of Qout				
	//---------------------------------------------------------------------------------------------------------------------------
	if ((patch->sat_deficit - patch->rz_storage - patch->unsat_storage) < -1.0*ZERO) {
		return_flow = compute_varbased_returnflow(patch->std,
			patch->rz_storage + patch->unsat_storage,
			patch->sat_deficit, &(patch->litter));
		patch->detention_store += return_flow;
		patch->sat_deficit   = 0.;
		patch->rz_storage    = 0.;
		patch->unsat_storage = 0.;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	calculated any N-transport associated with return flow. note available N reduced by what has already been 	
	//	we assume that only nitrate follows return flow lost in subsurface flow routing				
	//---------------------------------------------------------------------------------------------------------------------------
	if (grow_flag > 0) {
		Nout = compute_N_leached(
			verbose_flag,
			patch->soil_ns.nitrate,
			return_flow,
			0.0,
			0.0,
			patch->soil_defaults->m_z,
			total_gamma,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->N_decay_rate,
			patch->soil_defaults->active_zone_z,
			patch->soil_defaults->soil_depth,
			patch->soil_defaults->mobile_N_proportion);
		patch->surface_NO3  += Nout;
		patch->soil_ns.nitrate -= Nout;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//	calcualte surface overland or return flow 
	//---------------------------------------------------------------------------------------------------------------------------
	Qout = 0;
	if ((patch->detention_store > patch->soil_defaults->detention_store_size) && (patch->detention_store > ZERO))
	{
		Qout = (patch->detention_store - patch->soil_defaults->detention_store_size);

		if (grow_flag > 0) {
			Nout = (min(1.0, (Qout / patch->detention_store))) * patch->surface_NO3;
			patch->surface_leach_NO3_out += Nout;
			NO3_leached_to_surface = Nout;
			patch->surface_NO3    -= Nout;
			
			Nout = (min(1.0, (Qout / patch->detention_store))) * patch->surface_NH4;
			NH4_leached_to_surface = Nout;
			patch->surface_NH4    -= Nout;
			patch->surface_leach_NH4_out += Nout;
		}
		route_to_surface        = Qout;
		patch->detention_store -= Qout;
		patch->surface_Qout    += Qout;
	}

	double unsat_out = min(patch->unsat_storage, 4. / 5.*route_to_subsurface);
	patch->unsat_storage -= unsat_out;
	double rz_out = min(patch->rz_storage, 1.0 / 5.*route_to_subsurface);
	patch->rz_storage -= rz_out;
	patch->sat_deficit += (route_to_subsurface - unsat_out - rz_out);

	//Note:: The following is used for explicit routing simulation of water and nutrients movement
	if (routing_flag == 1){
		//-----------------------------------------------------------------------------------------------------------------------
		//  Now start water routing. If you do not want routing the water, turn off this following part.
		//	route flow to neighbours and route n_leaching if grow flag specfied 
		//-----------------------------------------------------------------------------------------------------------------------
		for (j = 0; j < patch->num_neighbours; j++) {
			neigh = patch->neighbours[j].patch;
			//--------------------------------------------------------------
			// first transfer subsurface water and nitrogen 
			//--------------------------------------------------------------
			Qin = (patch->neighbours[j].gamma * route_to_subsurface); //
			if (grow_flag > 0) {
				Nin = (patch->neighbours[j].gamma * NO3_leached_to_subsurface);
				neigh->subsurface_leach_NO3_out += Nin;
			}
			neigh->subsurface_Qin += Qin; //this will update patch->qin
			neigh->sat_deficit    -= Qin;
			neigh->gw.Qout += patch->neighbours[j].gamma*patch[j].gw.Qout; //added for ground water discharge

			neigh->DON_loss += patch->DON_loss*patch->neighbours[j].gamma; //we only route DOC and DON once
			neigh->DOC_loss += patch->DOC_loss*patch->neighbours[j].gamma;
		}

		//-----------------------------------------------------------------------------------------------------------------------
		// surface downslope routing 
		// First, determine which innundation depth to consider		
		//-----------------------------------------------------------------------------------------------------------------------
		if (patch->num_innundation_depths > 0) {
			innundation_depth = patch->detention_store + route_to_surface;
			d = 0;
			while ((innundation_depth > patch->critical_depth)
				&& (d < patch->num_innundation_depths - 1)) {
				d++;
			}
		}
		else d = 0;

		for (j = 0; j < patch->num_neighbours; j++) {
			neigh = patch->neighbours[j].patch;
			//-------------------------------------------------------------------------------------------------------------------
			// now transfer surface water and nitrogen	- first nitrogen					
			//-------------------------------------------------------------------------------------------------------------------
			if (grow_flag > 0) {
				Nin = (patch->neighbours[j].gamma * NO3_leached_to_surface);
				neigh->surface_NO3           += Nin;
				neigh->surface_leach_NO3_out += Nin;
				
				Nin = (patch->neighbours[j].gamma * NH4_leached_to_surface);
				neigh->surface_NH4           += Nin;
				neigh->surface_leach_NH4_out += Nin;
			}

			//-------------------------------------------------------------------------------------------------------------------
			//	- now surface water 					
			//	surface stores should be updated to facilitate transfer added net surface water transfer to detention store		
			//-------------------------------------------------------------------------------------------------------------------
			Qin = (patch->neighbours[j].gamma * route_to_surface);
			neigh->surface_Qin     += Qin; // updates patch surface_qin
			neigh->detention_store += Qin;

			/*
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//-------------------------------------------------------------------------------------------------------------------
			// try to infiltrate this water	use time_int as duration        /
			//-------------------------------------------------------------------------------------------------------------------
			if (neigh->detention_store > ZERO) {
				if (neigh->rootzone.depth > ZERO) {
					infiltration = compute_infiltration(
						verbose_flag,
						neigh->sat_deficit_z,
						neigh->rootzone.S,
						neigh->Ksat_vertical,
						neigh->soil_defaults->Ksat_0,
						neigh->soil_defaults->m_z,
						neigh->soil_defaults->porosity_0,
						neigh->soil_defaults->porosity_decay,
						(neigh->detention_store),
						time_int,
						neigh->soil_defaults->theta_psi_curve);
				}
				else {
					infiltration = compute_infiltration(
						verbose_flag,
						neigh->sat_deficit_z,
						neigh->S,
						neigh->Ksat_vertical,
						neigh->soil_defaults->Ksat_0,
						neigh->soil_defaults->m_z,
						neigh->soil_defaults->porosity_0,
						neigh->soil_defaults->porosity_decay,
						(neigh->detention_store),
						time_int,
						neigh->soil_defaults->theta_psi_curve);
				}
			}
			else infiltration = 0.0;

			infiltration = min(infiltration, neigh->detention_store / 40.);
			//-------------------------------------------------------------------------------------------------------------------
			// added an surface N flux to surface N pool	and		
			// allow infiltration of surface N				
			//-------------------------------------------------------------------------------------------------------------------
			if ((grow_flag > 0) && (infiltration > ZERO)) {
				//neigh->soil_ns.Qin += ((infiltration / neigh->detention_store) * neigh->surface_NO3);
				neigh->surface_NO3 -= ((infiltration / neigh->detention_store) * neigh->surface_NO3);
				neigh->soil_ns.sminn += ((infiltration / neigh->detention_store) * neigh->surface_NH4);
				neigh->surface_NH4 -= ((infiltration / neigh->detention_store) * neigh->surface_NH4);
			}

			if (infiltration > neigh->sat_deficit - neigh->unsat_storage - neigh->rz_storage) {
				neigh->sat_deficit -= (infiltration + neigh->unsat_storage + neigh->rz_storage);
				neigh->unsat_storage = 0.0;
				neigh->rz_storage = 0.0;
				neigh->field_capacity = 0.0;
				neigh->rootzone.field_capacity = 0.0;
			}

			else if ((neigh->sat_deficit > neigh->rootzone.potential_sat) &&
				(infiltration > neigh->rootzone.potential_sat - neigh->rz_storage)) {
				//---------------------------------------------------------------------------------------------------------------
				//		Just add the infiltration to the rz_storage and unsat_storage	
				//---------------------------------------------------------------------------------------------------------------
				neigh->unsat_storage += infiltration - (neigh->rootzone.potential_sat - neigh->rz_storage);
				neigh->rz_storage = neigh->rootzone.potential_sat;
			}
			// Only rootzone layer saturated - perched water table case 
			else if ((neigh->sat_deficit > neigh->rootzone.potential_sat) &&
				(infiltration <= neigh->rootzone.potential_sat - neigh->rz_storage)) {
				//---------------------------------------------------------------------------------------------------------------
				//		Just add the infiltration to the rz_storage	
				//---------------------------------------------------------------------------------------------------------------
				neigh->rz_storage += infiltration;
			}
			else if ((neigh->sat_deficit <= neigh->rootzone.potential_sat) &&
				(infiltration <= neigh->sat_deficit - neigh->rz_storage - neigh->unsat_storage)) {
				neigh->rz_storage += neigh->unsat_storage;
				// transfer left water in unsat storage to rootzone layer
				neigh->unsat_storage = 0;
				neigh->rz_storage += infiltration;
				neigh->field_capacity = 0;
			}
			neigh->detention_store -= infiltration;
			if (neigh->sat_deficit < 0.) {
				neigh->detention_store -= neigh->sat_deficit;
				neigh->sat_deficit = 0.;
			}

			*/
		}
	}

	patch->NO3_leached+=(NO3_leached_to_subsurface + NO3_leached_to_surface);

} //end update_drainage_land.c

