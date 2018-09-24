/*--------------------------------------------------------------*/
/* 											*/
/*					update_drainage_road			*/
/*											*/
/*	update_drainage_road.c - creates a patch object				*/
/*											*/
/*	NAME										*/
/*	update_drainage_road.c - creates a patch object				*/
/*											*/
/*	SYNOPSIS									*/
/*	void update_drainage_road( 							*/
/*					struct patch_object *patch			*/
/*				 			double,			 	*/
/*				 			double,			 	*/
/*				 			double,			 	*/
/*							int,				*/
/*							int)				*/
/*											*/
/* 											*/
/*											*/
/*	OPTIONS										*/
/*											*/
/*											*/
/*	DESCRIPTION									*/
/*											*/
/*											*/
/*											*/
/*											*/
/*	PROGRAMMER NOTES								*/
/*											*/
/*											*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

void  update_drainage_road(  
								 struct patch_object *patch,
								 double time_int,
								 int verbose_flag,
								 int grow_flag)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.				*/
	/*--------------------------------------------------------------*/
	
	double  compute_delta_water(
		int,
		double,
		double,
		double,
		double,
		double);


	double compute_N_leached(int,
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
		double);
	
	double compute_transmissivity_curve(
		double,
		double,
		double,
		double);

	double recompute_gamma(
		struct patch_object *,
		double);


	double compute_varbased_returnflow(
		double,
		double,
		double,
		struct litter_object *);
	
	/*--------------------------------------------------------------*/
	/*	Local variable definition.				*/
	/*--------------------------------------------------------------*/
	int  j,d;
	double m, Ksat, return_flow;
	double N_leached_to_patch, N_leached_to_stream, N_surface_leached_to_stream; /* kg/m2 */
	double N_leached_total; /* kg/m2 */
	double route_to_stream;  /* m3 */
	double route_to_patch;  /* m3 */
	double road_int_depth;  /* m of H2O */
	double available_sat_water; //route_total; /* m3 */
	double  Qin, Qout;  /* m *///, Qstr_total
	double total_gamma; // percent_loss;
	double Nin, Nout; /* kg/m2 */ 
	//double percent_tobe_routed;

	struct patch_object *neigh;

	N_leached_to_patch = 0.0;
	N_leached_to_stream = 0.0;
	N_surface_leached_to_stream = 0.0;
	route_to_stream = 0.0;
	route_to_patch = 0.0;


	/*--------------------------------------------------------------*/
	/*	m and K are multiplied by sensitivity analysis variables */
	/*--------------------------------------------------------------*/
	m = patch->soil_defaults->m_z  ;
	Ksat = patch->soil_defaults->Ksat_0 ;
	d=0;
	/*--------------------------------------------------------------*/
	/*	recalculate gamma based on current saturation deficits  */
	/*      to account the effect of changes in water table slope 	*/
	/*--------------------------------------------------------------*/
	total_gamma = recompute_gamma(patch, patch->gamma);

	available_sat_water = max(((patch->soil_defaults->soil_water_cap
			- max(patch->sat_deficit,0.0))),0.0);

	/*--------------------------------------------------------------*/
	/*	for roads, saturated throughflow beneath road cut	*/
	/*	is routed to downslope patches; saturated throughflow	*/
	/*	above the cut and overland flow is routed to the stream	*/
	/*--------------------------------------------------------------*/
	/*--------------------------------------------------------------*/
	/*	calculate water_equivalent depth of road		*/
	/*--------------------------------------------------------------*/
	road_int_depth = compute_delta_water(
		verbose_flag,
		patch->soil_defaults->porosity_0,
		patch->soil_defaults->porosity_decay,
		patch->soil_defaults->soil_depth,
		patch->road_cut_depth,
		0.0);
	if (road_int_depth > patch->sat_deficit) {
	/*------------------------------------------------------------*/
	/*	calculate amuount of water output to patches			*/
	/*-----------------------------------------------------------*/
		route_to_patch = total_gamma
			* compute_transmissivity_curve( m, 
				patch->soil_defaults->active_zone_z,
				road_int_depth,
				patch->soil_defaults->soil_water_cap) 
			* time_int;
		/*-----------------------------------------------------------*/
		/*	calculate amuount of water output to stream		*/
		/*-----------------------------------------------------------*/
		route_to_stream = total_gamma
			* compute_transmissivity_curve( m, 
				patch->soil_defaults->active_zone_z,
				patch->sat_deficit,
				patch->soil_defaults->soil_water_cap) 
			* time_int - route_to_patch;
		if (route_to_patch < 0.0) route_to_patch = 0.0;
		if (route_to_stream < 0.0) route_to_stream = 0.0;
		if ((route_to_stream + route_to_patch) > available_sat_water) {
			route_to_patch*= (available_sat_water)/(route_to_patch + route_to_stream);
			route_to_stream *= (available_sat_water)/(route_to_patch + route_to_stream);
		}
		/*--------------------------------------------------------------*/
		/* compute Nitrogen leaching amount				*/
		/*--------------------------------------------------------------*/
		if (grow_flag > 0) {
			N_leached_to_patch = compute_N_leached(
				verbose_flag,
				patch->soil_ns.nitrate,
				route_to_patch,
				road_int_depth,
				patch->soil_defaults->soil_water_cap,
				m,
				total_gamma,
				patch->soil_defaults->porosity_0,
				patch->soil_defaults->porosity_decay,
				patch->soil_defaults->N_decay_rate,
				patch->soil_defaults->active_zone_z,
				patch->soil_defaults->soil_depth,
				patch->soil_defaults->mobile_N_proportion);
			N_leached_to_stream = compute_N_leached(
				verbose_flag,
				patch->soil_ns.nitrate,
				route_to_stream,
				patch->sat_deficit,
				patch->soil_defaults->soil_water_cap,
				m,
				total_gamma,
				patch->soil_defaults->porosity_0,
				patch->soil_defaults->porosity_decay,
				patch->soil_defaults->N_decay_rate,
				patch->soil_defaults->active_zone_z,
				patch->soil_defaults->soil_depth,
				patch->soil_defaults->mobile_N_proportion) - 
				N_leached_to_patch;
			patch->soil_ns.Qout += (N_leached_to_patch + N_leached_to_stream);
					 
		}
		patch->subsurface_Qout += ((route_to_patch + route_to_stream));
	}
	/*--------------------------------------------------------------*/
	/* if road is below the water_table - no routing to the stream  */
	/*--------------------------------------------------------------*/
	else {
		route_to_stream = 0.0;
		route_to_patch = total_gamma
			* compute_transmissivity_curve( m, 
				patch->soil_defaults->active_zone_z,
				patch->sat_deficit,
				patch->soil_defaults->soil_water_cap) 
			* time_int;
		if (route_to_patch < 0.0) route_to_patch = 0.0;
		if (route_to_patch > available_sat_water) 
			route_to_patch = available_sat_water;
	/*--------------------------------------------------------------*/
	/* compute Nitrogen leaching amount				*/
	/*--------------------------------------------------------------*/
		if (grow_flag > 0) {
			N_leached_to_patch = compute_N_leached(
				verbose_flag,
				patch->soil_ns.nitrate,
				route_to_patch,
				patch->sat_deficit,
				patch->soil_defaults->soil_water_cap,
				m,
				total_gamma,
				patch->soil_defaults->porosity_0,
				patch->soil_defaults->porosity_decay,
				patch->soil_defaults->N_decay_rate,
				patch->soil_defaults->active_zone_z,
				patch->soil_defaults->soil_depth,
				patch->soil_defaults->mobile_N_proportion);
			N_leached_to_stream = 0.0;
			patch->soil_ns.Qout += (N_leached_to_patch + N_leached_to_stream);
		}

		patch->subsurface_Qout += ((route_to_patch + route_to_stream));
	}

	/*--------------------------------------------------------------*/
	/*	calculate any return flow associated with this patch	*/
	/*	and route any infiltration excess			*/
	/*	return flow is flow leaving patch (i.e surface_Qout)  	*/
	/*	note that return flow that becomes detention storage   */
	/*	is added to surface_Qin					*/
	/*	similarly with associated nitrogen			*/
	/* 	note we move unsat_storage into saturated storage in this case */
	/*	saturated zone will be updated in compute_subsurface_routing	*/
	/*	i.e becomes part of Qout				*/
	/*--------------------------------------------------------------*/
	if ((patch->sat_deficit-patch->rz_storage-patch->unsat_storage) < -1.0*ZERO) {
		return_flow = compute_varbased_returnflow(patch->std, 
			patch->rz_storage+patch->unsat_storage,
			patch->sat_deficit, &(patch->litter));
		patch->detention_store += return_flow;  
		patch->sat_deficit = 0.0;
		patch->unsat_storage = 0.0;
		patch->rz_storage = 0.0;
	}
	/*--------------------------------------------------------------*/
	/*	calculated any N-transport associated with return flow  */
	/*	-note available N reduced by what has already been 	*/
	/*	lost in subsurface flow routing				*/
	/*	- note only nitrate is assumed to follow return flow	*/
	/*--------------------------------------------------------------*/
	if (grow_flag > 0) {
		Nout = compute_N_leached(
			verbose_flag,
			patch->soil_ns.nitrate - N_leached_to_patch,
			return_flow,
			0.0,
			0.0,
			m,
			total_gamma,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			patch->soil_defaults->N_decay_rate,
			patch->soil_defaults->active_zone_z,
			patch->soil_defaults->soil_depth,
			patch->soil_defaults->mobile_N_proportion);
		patch->surface_NO3 += Nout;
		patch->soil_ns.Qout += Nout;
		}
	
	/*--------------------------------------------------------------*/
	/*	route water and nitrogen lossed due to infiltration excess */
	/*	note we assume that this happens before return_flow losses */
	/*--------------------------------------------------------------*/
	if ((patch->detention_store > patch->soil_defaults->detention_store_size) &&
		(patch->detention_store > ZERO) ) {
		Qout = (patch->detention_store - patch->soil_defaults->detention_store_size);
		if (grow_flag > 0) {
			Nout = (min(1.0, (Qout/ patch->detention_store))) * patch->surface_NO3;
			patch->surface_NO3  -= Nout;
			patch->next_stream->streamflow_N += Nout;
			Nout = (min(1.0, (Qout/ patch->detention_store))) * patch->surface_NH4;
			patch->surface_NH4  -= Nout;
			patch->next_stream->streamflow_N += Nout;
			}
		patch->next_stream->streamflow += Qout;
		patch->detention_store -= Qout;
		}



	/*--------------------------------------------------------------*/
	/*	route flow to neighbours				*/
	/*	route n_leaching if grow flag specfied			*/
	/*--------------------------------------------------------------*/

	/*--------------------------------------------------------------*/
	/* routing to stream i.e. diversion routing */
	/*	note all surface flows go to the stream			*/
	/*--------------------------------------------------------------*/
	//patch->next_stream->streamflow += (route_to_stream) / patch->next_stream->area;
	//patch->next_stream->surface_Qin  += (route_to_stream) / patch->next_stream->area;

	if (patch->next_stream->ID == 14)
			printf("\n in is %lf", route_to_stream);
	if (grow_flag > 0) {
		Nin = N_leached_to_stream;
		patch->next_stream->streamflow_N += Nin;
		}

	/*--------------------------------------------------------------*/
	/*	route flow to neighbours				*/
	/*	route n_leaching if grow flag specfied			*/
	/*--------------------------------------------------------------*/
	/*--------------------------------------------------------------*/
	/* regular downslope routing */
	/*--------------------------------------------------------------*/
	d=0;
	for (j = 0; j < patch->num_neighbours; j++) {
		neigh = patch->neighbours[j].patch;  
		/*--------------------------------------------------------------*/
		/* first transfer subsurface water and nitrogen */
		/*--------------------------------------------------------------*/
		Qin =	(patch->neighbours[j].gamma * route_to_patch);
		if (grow_flag > 0) {
			Nin = (patch->neighbours[j].gamma * N_leached_to_patch);
			neigh->soil_ns.Qin += Nin;
			neigh->ndf.sminn_leached -= Nin;
			}
		neigh->subsurface_Qin += Qin;

			if ((neigh->ID == 49) && (neigh->detention_store < -ZERO))
				printf("\n  Qin %lf  dt %d, sat_def %lf, detention store %lf, dts %lf, returnflow %lf Qout %lf",
					Qin,
					neigh->drainage_type,
					neigh->sat_deficit,
					neigh->detention_store,
					neigh->soil_defaults->detention_store_size,
					neigh->surface_Qout,
					neigh->subsurface_Qout);

	}

	N_leached_total = (N_leached_to_patch + N_leached_to_stream  );
	patch->soil_ns.nleached_snk += N_leached_total;
	patch->ndf.sminn_leached +=  N_leached_total; 


} /*end update_drainage_road.c*/

