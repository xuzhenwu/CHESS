//---------------------------------------------------------------------------------------------------------------------------
//xu. CHANNEL.. Channel flow routing process the route out the water
//A similar process as in Wigmosta et al.(2002)
//A static or dynamic "k" are used
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <iostream>
#include <math.h>
#include "CHESS.h"
#include "Constants.h"
#include "Functions.h"

using std::cout;
using std::endl;

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

void  chess_channel_daily(patch_object *patch, struct reservoir_object reservoir, struct command_line_object *command_line,
	struct	date current_date, int num_patches, double cellsize)
{
	double  compute_z_final(
		int,
		double,
		double,
		double,
		double,
		double);

	struct patch_object	*neigh;
	int i, j, k = 0, kk = 0;
	static double route_to_streamflow = 0, route_to_baseflow = 0;
	double sumstreamflow = 0, sumbaseflow = 0;
	static double volume = 0;
	static bool startDischarge = false;
	double Qout = 0., water = 0.;
	double surface_Qout = 0., return_flow = 0.;

	//xu.
	double V1 = 0, V2 = 0, OUT_all = 0, INOUT_ratio = 0;


	if (command_line->routing_flag == 1) {
		for (i = 0; i < num_patches; i++) {
			if (patch[i].drainage_type == STREAM) {

				//For river channel, soil should be mostly saturated. So, we assume one-tenth of
				//surface and subsurface flow are used to reduce soil saturation deficit in channel

				double partial_surface = patch[i].surface_Qout*1. / 100.; //20
				double partial_subsurface = patch[i].subsurface_Qout*1. / 100.;//20

				if (patch[i].sat_deficit > 0.) {
					double min_surface = min(partial_surface, patch[i].sat_deficit);
					patch[i].sat_deficit -= min_surface;
					patch[i].surface_Qout -= min_surface;
				}

				if (patch[i].sat_deficit > 0.) {
					double min_subsurface = min(partial_subsurface, patch[i].sat_deficit);
					patch[i].sat_deficit -= min_subsurface;
					patch[i].subsurface_Qout -= min_subsurface;
				}


				patch[i].sat_deficit_z = compute_z_final(
					command_line->verbose_flag,
					patch[i].soil_defaults->porosity_0,
					patch[i].soil_defaults->porosity_decay,
					patch[i].soil_defaults->soil_depth,
					0.0,
					-1.0 * patch[i].sat_deficit);


				//===============================================================================================================================
				// CHANNEL ROUTING SIMULATIONS		
				//===============================================================================================================================
				for (j = 0; j < patch[i].num_neighbours; j++) {
					neigh = patch[i].neighbours[j].patch;

					if (neigh->drainage_type == STREAM) {

						//---------------------------------------------------------------------------------------------------------------------------
						//S1. For large basins, the channel routing and reservoirs
						//---------------------------------------------------------------------------------------------------------------------------
						if (command_line->cf == TRUE) {


							//Q_IN
							patch[i].channel->Q_in = patch[i].surface_Qout + patch[i].subsurface_Qout + patch[i].gw.Qout;// m/day

							if (patch[i].channel->Q_in > 0.0) {
								patch[i].channel->surface_ratio = patch[i].surface_Qout / patch[i].channel->Q_in;
								patch[i].channel->subsurface_ratio = patch[i].subsurface_Qout / patch[i].channel->Q_in;
								patch[i].channel->gw_ratio = patch[i].gw.Qout / patch[i].channel->Q_in;
							}
							else patch[i].channel->surface_ratio = patch[i].channel->subsurface_ratio = patch[i].channel->gw_ratio = 1 / 3.0;

							//CHANGE UNIT
							patch[i].channel->Q_in *= pow(cellsize, 2);//QOUT m^3/day


							//---------------------------------------------------------------------------------------------------------------------------
							//1. For Reservoirs (Target Release)
							//---------------------------------------------------------------------------------------------------------------------------
							if (command_line->re == TRUE && patch[i].channel->ID == RESERVOIR && current_date.year>=patch[i].channel->reservoir->StartYear)
							{
								//STORAGE
								patch[i].channel->storage+= patch[i].channel->Q_in;

								//i30 input
								if(patch[i].channel->reservoir->i30_days<30){
									patch[i].channel->reservoir->i30[patch[i].channel->reservoir->i30_days] = patch[i].channel->Q_in;
									patch[i].channel->reservoir->i30_days += 1;
								}
								else {	
									patch[i].channel->reservoir->i30[29]= patch[i].channel->Q_in;//add to the last one 
								}
								
								//i30 mean
								patch[i].channel->reservoir->i30_all = 0;
								for (int inx = 0; inx != patch[i].channel->reservoir->i30_days; inx++) {
									patch[i].channel->reservoir->i30_all+= patch[i].channel->reservoir->i30[inx];
								}
								patch[i].channel->reservoir->i30_mean = patch[i].channel->reservoir->i30_all / patch[i].channel->reservoir->i30_days;
								//i30 std
								patch[i].channel->reservoir->i30_std = 0;
								if (patch[i].channel->reservoir->i30_days == 1) {
									patch[i].channel->reservoir->i30_std = 2000000;// not sure how to set this value
								}
								else
								for (int inx = 0; inx != patch[i].channel->reservoir->i30_days; inx++) {
									patch[i].channel->reservoir->i30_std += pow(patch[i].channel->reservoir->i30[inx]- patch[i].channel->reservoir->i30_mean,2);		
								}
								patch[i].channel->reservoir->i30_std /= patch[i].channel->reservoir->i30_days;
								patch[i].channel->reservoir->i30_std = pow(patch[i].channel->reservoir->i30_std,1/2.0);

								//move i30 to next
								if(patch[i].channel->reservoir->i30_days == 30)
								for (int inx = 0; inx != 29; inx++){//0-28 , 29 in total days to be reserved
									patch[i].channel->reservoir->i30[inx] = patch[i].channel->reservoir->i30[inx + 1];
								}


								//modificaion of power、supply、impound
								//note that month start from 0
								patch[i].channel->reservoir->npow = (patch[i].channel->storage - patch[i].channel->reservoir->Vc[current_date.month - 1]) /
											max(patch[i].channel->reservoir->Vp[current_date.month - 1] - patch[i].channel->reservoir->Vc[current_date.month - 1],
											patch[i].channel->reservoir->Vc[current_date.month - 1] - patch[i].channel->reservoir->Vd);

								int year_day = yearday(current_date)-1;//start from 0
								patch[i].channel->reservoir->nsup = (patch[i].channel->reservoir->longterm_i30[year_day] - patch[i].channel->reservoir->i30_mean)*
									(patch[i].channel->storage - patch[i].channel->reservoir->Vd) /
									(patch[i].channel->reservoir->Vp[current_date.month - 1] - patch[i].channel->reservoir->Vd) /
									patch[i].channel->reservoir->i30_std;

								patch[i].channel->reservoir->nimp = -(patch[i].channel->reservoir->Vp[current_date.month - 1] - patch[i].channel->storage) /
									(patch[i].channel->reservoir->Vp[current_date.month - 1] - patch[i].channel->reservoir->Vd);


								//final eqation
								int year_inx = current_date.year- patch[i].channel->reservoir->StartYear;
								patch[i].channel->Q_out = (1 +
									(patch[i].channel->reservoir->npow*patch[i].channel->reservoir->alpha[year_inx] +
										patch[i].channel->reservoir->nsup*patch[i].channel->reservoir->beta[year_inx] +
										patch[i].channel->reservoir->nimp*patch[i].channel->reservoir->gamma[year_inx]
										)*patch[i].channel->reservoir->kmon[current_date.month-1])*
									patch[i].channel->reservoir->longterm_qout[year_day];


								//mass balance and limitation
								if (patch[i].channel->storage < patch[i].channel->reservoir->Vd)
									patch[i].channel->Q_out = 0;
								//if(patch[i].channel->storage > patch[i].channel->reservoir->Ve)
									//patch[i].channel->Q_out = min(patch[i].channel->Q_out, (patch[i].channel->storage- patch[i].channel->reservoir->Ve)/5.0);

								patch[i].channel->Q_out = max(patch[i].channel->Q_out, 0);


								//RENEW STORAGE
								patch[i].channel->storage -= patch[i].channel->Q_out;

							}
							//---------------------------------------------------------------------------------------------------------------------------
							//2. For principal channel patch
							//---------------------------------------------------------------------------------------------------------------------------
							else {
								
								//---------------------------------------------------------------------------------------------------------------------------
								// Alternative Method 1:Linear Storage Algorithm (Based on Manning's Equation)
								//---------------------------------------------------------------------------------------------------------------------------
								// initial the parameters for preparing parameters of k
								// length
								if (pow(patch[i].x - neigh->x, 2) < 0.01 || pow(patch[i].y - neigh->y, 2) < 0.01)
									patch[i].channel->channel_length = cellsize;
								else patch[i].channel->channel_length = cellsize * pow(2, 1 / 2.0);//a problem with sqrt 2 and 1;

								//slope
								patch[i].channel->channel_slope = (patch[i].z - neigh->z) / patch[i].channel->channel_length;
								patch[i].channel->channel_slope = max(patch[i].channel->channel_slope, 0.0001);

								//h (in current stage we assume that the channel_width are static)
								patch[i].channel->crosssectional_area = patch[i].channel->storage / patch[i].channel->channel_length;

								//(B*B-4AC)^1/2 -B/2A
								patch[i].channel->h = (pow(patch[i].channel->channel_width*patch[i].channel->channel_width + 4 * patch[i].channel->crosssectional_area*patch[i].channel->inverse_side_slope, 1 / 2.0)
									- patch[i].channel->channel_width)
									/ (2.0*patch[i].channel->inverse_side_slope);

								patch[i].channel->wetted_parameter = patch[i].channel->channel_width + 2 * patch[i].channel->h*pow(1 + pow(patch[i].channel->inverse_side_slope, 2), 1.0 / 2);

								//Rr
								patch[i].channel->Rr = patch[i].channel->crosssectional_area / patch[i].channel->wetted_parameter;

								//k
								patch[i].channel->k = pow(patch[i].channel->Rr, 2.0 / 3)*pow(patch[i].channel->channel_slope, 1 / 2.0)
									/ (patch[i].channel->hydraulic_roughness * patch[i].channel->channel_length);

								//V1
								V1 = patch[i].channel->storage ;

								//V2 a key function for computing channel storage change
								if (patch[i].channel->k > 0.0)
									V2 = patch[i].channel->Q_in / (seconds_per_day * patch[i].channel->k) +
									(V1 - patch[i].channel->Q_in / (patch[i].channel->k*seconds_per_day)) * exp(-patch[i].channel->k*seconds_per_day);
								else V2 = V1 + patch[i].channel->Q_in;


								//CHECK BALANCE
								if (V2 < 0.0) V2 = 0;
								else if (V2 > V1 + patch[i].channel->Q_in) V2 = V1 + patch[i].channel->Q_in;

								//Q_OUT
								patch[i].channel->Q_out = V1 + patch[i].channel->Q_in - V2;


								//RENEW STORAGE
								patch[i].channel->storage = V2 ;

							}//END OF PRICIPLE CHANNEL


							//CHANGE UNIT AND DILIVER
							OUT_all = patch[i].channel->Q_out / pow(cellsize, 2);// m/day
							patch[i].surface_Qout = OUT_all * patch[i].channel->surface_ratio;
							patch[i].subsurface_Qout = OUT_all * patch[i].channel->subsurface_ratio;
							patch[i].gw.Qout = OUT_all * patch[i].channel->gw_ratio;

							//CN
							if (patch[i].channel->Q_in > 0.0)
								INOUT_ratio = patch[i].channel->Q_out / patch[i].channel->Q_in;
							else INOUT_ratio = 0;
							patch[i].DON_loss *= INOUT_ratio;
							patch[i].DOC_loss *= INOUT_ratio;
							patch[i].surface_leach_NH4_out *= INOUT_ratio;
							patch[i].subsurface_leach_NH4_out *= INOUT_ratio;
							patch[i].surface_leach_NO3_out *= INOUT_ratio;
							patch[i].subsurface_leach_NO3_out *= INOUT_ratio;


							//---------------------------------------------------------------------------------------------------------------------------
							//DELIVER WATER AND NUTRIENT TO NEIGHBORS
							//---------------------------------------------------------------------------------------------------------------------------
							neigh->surface_Qout += patch[i].neighbours[j].gamma * patch[i].surface_Qout;
							neigh->subsurface_Qout += patch[i].neighbours[j].gamma * patch[i].subsurface_Qout;
							neigh->gw.Qout += patch[i].neighbours[j].gamma * patch[i].gw.Qout;
							neigh->DON_loss += patch[i].neighbours[j].gamma*patch[i].DON_loss;
							neigh->DOC_loss += patch[i].neighbours[j].gamma*patch[i].DOC_loss;
							neigh->subsurface_leach_NH4_out += patch[i].neighbours[j].gamma*patch[i].subsurface_leach_NH4_out;
							neigh->surface_leach_NH4_out += patch[i].neighbours[j].gamma*patch[i].surface_leach_NH4_out;
							neigh->subsurface_leach_NO3_out += patch[i].neighbours[j].gamma*patch[i].subsurface_leach_NO3_out;
							neigh->surface_leach_NO3_out += patch[i].neighbours[j].gamma*patch[i].surface_leach_NO3_out;
						}//END OF CHANNEL FLOW 
						
						//---------------------------------------------------------------------------------------------------------------------------
						//S2. For small basins, we force to routed  all water out
						//---------------------------------------------------------------------------------------------------------------------------
						else {

							neigh->surface_Qout += patch[i].neighbours[j].gamma *patch[i].surface_Qout;
							neigh->subsurface_Qout += patch[i].neighbours[j].gamma *patch[i].subsurface_Qout;
							neigh->gw.Qout += patch[i].neighbours[j].gamma *patch[i].gw.Qout;
							neigh->DON_loss += patch[i].neighbours[j].gamma*patch[i].DON_loss;
							neigh->DOC_loss += patch[i].neighbours[j].gamma*patch[i].DOC_loss;
							neigh->subsurface_leach_NH4_out += patch[i].neighbours[j].gamma*patch[i].subsurface_leach_NH4_out;
							neigh->surface_leach_NH4_out += patch[i].neighbours[j].gamma*patch[i].surface_leach_NH4_out;
							neigh->subsurface_leach_NO3_out += patch[i].neighbours[j].gamma*patch[i].subsurface_leach_NO3_out;
							neigh->surface_leach_NO3_out += patch[i].neighbours[j].gamma*patch[i].surface_leach_NO3_out;

						}

					}
				}//xu.end of a stream patch

			}//xu.end of all stream patch
		}
	}

	/*
	if(command_line->routing_flag ==1){
	for (i=0;i<num_patches;i++){
	if(patch[i].ID ==15065) {//reservoir.ID
	kk=i;
	break;
	}
	}
	//printf("kk is %d \n",kk);

	//First，route stream water to the reservoir
	for (i=0;i<kk;i++){
	if(patch[i].drainage_type==STREAM){
	for(j=0;j<patch[i].num_neighbours; j++){
	neigh = patch[i].neighbours[j].patch;
	if(neigh->drainage_type == STREAM ){
	if(neigh->ID==reservoir.ID) {
	neigh->surface_Qout     += (patch[i].neighbours[j].gamma * patch[i].surface_Qout
	+ volume);
	neigh->subsurface_Qout  += patch[i].neighbours[j].gamma * patch[i].subsurface_Qout;
	volume=(neigh->surface_Qout+neigh->subsurface_Qout);
	}
	else{
	neigh->surface_Qout     += patch[i].neighbours[j].gamma * patch[i].surface_Qout;
	neigh->subsurface_Qout  += patch[i].neighbours[j].gamma * patch[i].subsurface_Qout;
	}
	}
	}
	}
	}

	// second, based on reservoir's volume condition to determine how to route water down the reservoir
	if(volume>=reservoir.max_volume) startDischarge=true;
	if(startDischarge) {
	Qout   = max(volume-reservoir.max_volume,0.); // water exceed the reservoir's maximum volumen
	volume-= Qout;                                // flow down the reservoir directly

	volume-= reservoir.discharge;                 // start human discharge
	Qout   = (Qout + reservoir.discharge);
	if(volume<=reservoir.min_volume) startDischarge=false; //close the dam if volume is less than
	// minimum reservoir's volume
	for(i=kk;i<num_patches;i++){
	if(patch[i].drainage_type==STREAM){
	for(j=0;j<patch[i].num_neighbours; j++){
	neigh = patch[i].neighbours[j].patch;
	if(neigh->drainage_type == STREAM ){
	if(i==kk){
	neigh->surface_Qout     += patch[i].neighbours[j].gamma * Qout; //surface out
	neigh->subsurface_Qout  += patch[i].neighbours[j].gamma * 0.;   //no baseflow due to dam
	}
	else{
	neigh->surface_Qout     += patch[i].neighbours[j].gamma * patch[i].surface_Qout;
	neigh->subsurface_Qout  += patch[i].neighbours[j].gamma * patch[i].subsurface_Qout;
	}
	}
	}
	}
	}
	}
	else {
	for(i=kk+1;i<num_patches;i++){
	if(patch[i].drainage_type==STREAM){
	for(j=0;j<patch[i].num_neighbours; j++){
	neigh = patch[i].neighbours[j].patch;
	if(neigh->drainage_type == STREAM ){
	neigh->surface_Qout     += patch[i].neighbours[j].gamma * patch[i].surface_Qout;
	neigh->subsurface_Qout  += patch[i].neighbours[j].gamma * patch[i].subsurface_Qout;
	}
	}
	}
	}
	}
	}
	*/
	return;
}