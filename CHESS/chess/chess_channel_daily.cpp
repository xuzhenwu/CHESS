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
	double surface_v1 = 0, surface_v2 = 0, subsurface_v1 = 0, subsurface_v2 = 0;
	double surface_Q_out_ratio = 0, subsurface_Q_out_ratio = 0;

	double channel_slope = 0., channel_length = 0.;//xu.



	if (command_line->routing_flag == 1){
		for (i = 0; i<num_patches; i++){
			if (patch[i].drainage_type == STREAM){

				//For river channel, soil should be mostly saturated. So, we assume one-tenth of
				//surface and subsurface flow are used to reduce soil saturation deficit in channel

				double partial_surface = patch[i].surface_Qout*1. / 100.; //20
				double partial_subsurface = patch[i].subsurface_Qout*1. / 100.;//20

				if (patch[i].sat_deficit>0.){
					double min_surface = min(partial_surface, patch[i].sat_deficit);
					patch[i].sat_deficit -= min_surface;
					patch[i].surface_Qout -= min_surface;
				}

				if (patch[i].sat_deficit>0.){
					double min_subsurface = min(partial_subsurface, patch[i].sat_deficit);
					patch[i].sat_deficit -= min_subsurface;
					patch[i].subsurface_Qout -= min_subsurface;
				}

				/*
				if (patch[i].surface_Qout > 0.001){
					cout << "patch[i].surface_Qout" << endl;
					cout << patch[i].ID << "\t" << patch[i].surface_Qout << endl;
					cout << endl;
				}
				*/


				/*
				patch[i].unsat_storage=0.;
				patch[i].rz_storage=0.;

				if(patch[i].sat_deficit <= 0.){
				patch[i].surface_Qout     -= patch[i].sat_deficit;
				patch[i].sat_deficit=0.;
				}
				else{
				if(patch[i].surface_Qout>=patch[i].sat_deficit) {
				patch[i].surface_Qout -= patch[i].sat_deficit;
				patch[i].sat_deficit=0.;
				}
				else {
				patch[i].sat_deficit-=patch[i].surface_Qout;
				patch[i].surface_Qout=0.;

				//if(patch[i].subsurface_Qout>=patch[i].sat_deficit) {
				//	patch[i].subsurface_Qout -= patch[i].sat_deficit;
				//	patch[i].sat_deficit =0.;
				//}
				//else{
				//	patch[i].sat_deficit -= patch[i].subsurface_Qout;
				//	patch[i].subsurface_Qout=0.;
				//}
				}
				}
				*/
				patch[i].sat_deficit_z = compute_z_final(
					command_line->verbose_flag,
					patch[i].soil_defaults->porosity_0,
					patch[i].soil_defaults->porosity_decay,
					patch[i].soil_defaults->soil_depth,
					0.0,
					-1.0 * patch[i].sat_deficit);

				//xu. start main changes
				//routing the river flow
				for (j = 0; j<patch[i].num_neighbours; j++){
					neigh = patch[i].neighbours[j].patch;
					if (neigh->drainage_type == STREAM){

						//xu.
						//Qin 
						patch[i].channel->surface_Q_in = patch[i].surface_Qout;
						patch[i].channel->subsurface_Q_in = patch[i].subsurface_Qout;

						//start of computing dynamic k
						if (patch[i].channel->surface_Q_in != patch[i].channel->surface_Q_in) { cout << "FATAL ERROR IN: Q_in" << patch[i].channel->surface_Q_in << endl; getchar(); }
						//compute local parameters
						if (pow(patch[i].x - neigh->x, 2) < 0.01 || pow(patch[i].y - neigh->y, 2)<0.01)
							patch[i].channel->channel_length = cellsize;
						else patch[i].channel->channel_length = cellsize* pow(2, 1 / 2);//a problem with sqrt 2 and 1;


						patch[i].channel->channel_slope = (patch[i].z - neigh->z) / patch[i].channel->channel_length;
						
						patch[i].channel->channel_slope = max(patch[i].channel->channel_slope, 0.0001);


						if (patch[i].channel->channel_slope < 0.0000000001) { cout << "FATAL ERROR IN: slope" << patch[i].channel->channel_slope << endl; getchar(); }
						if (patch[i].channel->channel_width < 0.0000000001) { cout << "FATAL ERROR IN: width" << patch[i].channel->channel_width << endl; getchar(); }


						patch[i].channel->h = patch[i].channel->surface_storage * pow(cellsize, 2) / (patch[i].channel->channel_length * patch[i].channel->channel_width);

						if (patch[i].channel->h >  0.0000000001)
							patch[i].channel->Rr = patch[i].channel->channel_width*patch[i].channel->h / (patch[i].channel->channel_width + 2 * patch[i].channel->h);
						else patch[i].channel->Rr = 77.14; // An alternative value

						patch[i].channel->k = pow(patch[i].channel->Rr, 2 / 3)*pow(patch[i].channel->channel_slope, 1 / 2) / patch[i].channel->hydraulic_roughness / patch[i].channel->channel_length;
						//end of computing dynamic k
						
						//Static Mode
						patch[i].channel->k = 500;


						//Storage has been changed here BY k
						//surface
						surface_v1 = patch[i].channel->surface_storage;

						if (surface_v1 + patch[i].channel->surface_Q_in > 0.001){
							surface_v2 = patch[i].channel->surface_Q_in / patch[i].channel->k + (surface_v1 - patch[i].channel->surface_Q_in / patch[i].channel->k)*exp(-patch[i].channel->k*1.0);//define det t as 1 day
							if (surface_v2 != surface_v2) {
								cout << "FATAL ERROR IN: v2: " << surface_v2 << endl; getchar();
								cout << "FATAL ERROR IN: k: " << surface_v2 << endl; getchar();
								surface_v2 = 0;
							}
							if (surface_v2 >surface_v1 + patch[i].channel->surface_Q_in) surface_v2 = surface_v1 + patch[i].channel->surface_Q_in;
							if (surface_v2 <  0.0000000001) surface_v2 = 0;
						}
						else surface_v2 = surface_v1 + patch[i].channel->surface_Q_in;

						//subsurface
						subsurface_v1 = patch[i].channel->subsurface_storage;

						if (subsurface_v1 + patch[i].channel->subsurface_Q_in > 0.001){
							subsurface_v2 = patch[i].channel->subsurface_Q_in / patch[i].channel->k + (subsurface_v1 - patch[i].channel->subsurface_Q_in / patch[i].channel->k)*exp(-patch[i].channel->k*1.0);//define det t as 1 day
							if (subsurface_v2 != subsurface_v2) {
								cout << "FATAL ERROR IN: v2: " << subsurface_v2 << endl; getchar();
								cout << "FATAL ERROR IN: k: " << subsurface_v2 << endl; getchar();
								subsurface_v2 = 0;
							}
							if (subsurface_v2 >subsurface_v1 + patch[i].channel->subsurface_Q_in) subsurface_v2 = subsurface_v1 + patch[i].channel->subsurface_Q_in;
							if (subsurface_v2 <  0.0000000001) subsurface_v2 = 0;
						}
						else subsurface_v2 = subsurface_v1 + patch[i].channel->subsurface_Q_in;

						//Renew the storage
						patch[i].channel->surface_storage = surface_v2;
						patch[i].channel->subsurface_storage = subsurface_v2;



						//Compute Real Q_out
						patch[i].channel->surface_Q_out = patch[i].channel->surface_Q_in - (surface_v2 - surface_v1) / 1.0;//define det t as 1 day
						patch[i].channel->subsurface_Q_out = patch[i].channel->subsurface_Q_in - (subsurface_v2 - subsurface_v1) / 1.0;//define det t as 1 day
						
						//Deliver values
						patch[i].surface_Qout = patch[i].channel->surface_Q_out;
						patch[i].subsurface_Qout = patch[i].channel->subsurface_Q_out;

						//computing ratio
						if (patch[i].channel->surface_Q_out > 0. && patch[i].channel->surface_Q_in > 0.) surface_Q_out_ratio = patch[i].channel->surface_Q_out / patch[i].channel->surface_Q_in;
						else   surface_Q_out_ratio = 0;
						if (patch[i].channel->subsurface_Q_out > 0. && patch[i].channel->subsurface_Q_in > 0.) subsurface_Q_out_ratio = patch[i].channel->subsurface_Q_out / patch[i].channel->subsurface_Q_in;
						else   subsurface_Q_out_ratio = 0;


						//compute by ratio
						//surface_Qout for a neibor stream is also been calculated channel flow
						neigh->surface_Qout += patch[i].neighbours[j].gamma *patch[i].surface_Qout;
						neigh->subsurface_Qout += patch[i].neighbours[j].gamma *patch[i].subsurface_Qout;
						neigh->gw.Qout += patch[i].neighbours[j].gamma *patch[i].gw.Qout;//added for ground water
						neigh->DON_loss += patch[i].neighbours[j].gamma*patch[i].DON_loss * surface_Q_out_ratio;//we only route DOC and DON once
						neigh->DOC_loss += patch[i].neighbours[j].gamma*patch[i].DOC_loss * surface_Q_out_ratio;
						neigh->subsurface_leach_NH4_out += patch[i].neighbours[j].gamma*patch[i].subsurface_leach_NH4_out* subsurface_Q_out_ratio;
						neigh->surface_leach_NH4_out += patch[i].neighbours[j].gamma*patch[i].surface_leach_NH4_out * surface_Q_out_ratio;
						neigh->subsurface_leach_NO3_out += patch[i].neighbours[j].gamma*patch[i].subsurface_leach_NO3_out* subsurface_Q_out_ratio;
						neigh->surface_leach_NO3_out += patch[i].neighbours[j].gamma*patch[i].surface_leach_NO3_out * surface_Q_out_ratio;

						//xu. end
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