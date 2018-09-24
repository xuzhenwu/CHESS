/*--------------------------------------------------------------*/
/* 																*/
/*					output_basin						*/
/*																*/
/*	output_basin - creates output files objects.		*/
/*																*/
/*	NAME														*/
/*	output_basin - outputs current contents of a basin.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_basin( int routing_flag,										*/	
/*					struct	basin_object	*basin,				*/
/*					struct	date	date,  						*/
/*					FILE 	*outfile)							*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	outputs spatial structure according to commandline			*/
/*	specifications to specific files							*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	We only permit one fileset per spatial modelling level.     */
/*	Each fileset has one file for each timestep.  				*/
/*																*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

void	output_basin_daily_hydrology(int num_patches,struct	patch_object *patch,
					 struct	date	current_date,
					 FILE *outfile,struct command_line_object *command_line)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/
	
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	int p=0; //h,z,,c
	int  layer=1; //var_flag,
	double astreamflow_N=0.;
	double arain=0.,asnow=0.;
	double arain_throughfall=0.;
	double asnow_throughfall=0.;
	double asat_deficit_z=0.;
	double asat_deficit=0.;
	double aunsat_storage=0., arz_storage=0.;
	double aunsat_drainage=0., arz_drainage=0.;
	double acap_rise=0.;
	double areturn_flow=0.;
	double aevaporation=0.,acan_evap=0.;
	double asnowpack=0.;
	double atranspiration=0.,asoil_evap=0.;
	double astreamflow=0.;
	double asat_area=0.;
	double abase_flow=0.,  arsmc=0.;

	double aaet=0., adC13=0.,apet=0.;
	double gw_drain=0., gw_Nstore=0.,gw_store=0.;
	double gw_discharge=0., gw_Nout=0.;
	double aarea=0.,  basin_area=0.; 

	double asnow_melt=0.;
	double CaIntTot=0.,CaInRain=0.,CaInSnow=0.,acan_sublimation=0.,asnowpack_sub=0.;
	double alitter_store=0., asoil_store=0., acanopy_store=0.,asurf_evap=0.,APET=0.;
	int snownumber=0;
	
	for (p=0; p< num_patches; p++){
		arain             += patch[p].rain;
		asnow             += patch[p].snow;
		arain_throughfall += patch[p].rain_throughfall;
		asnow_throughfall += patch[p].snow_throughfall;
		
		alitter_store     += patch[p].litter.rain_stored;
		asoil_store       += patch[p].detention_store;
		//acanopy_store     += patch[p].rain_stored + patch[p].snow_stored;
		
		asat_deficit_z    += patch[p].sat_deficit_z;
		asat_deficit      += patch[p].sat_deficit;
		arz_storage       += patch[p].rz_storage;		
		aunsat_storage    += patch[p].unsat_storage;
		arz_drainage      += patch[p].rz_drainage;	
		aunsat_drainage   += patch[p].unsat_drainage;
		acap_rise         += patch[p].cap_rise;
		arsmc             += max(1-max(patch[p].sat_deficit-patch[p].rz_storage-patch[p].unsat_storage,0)/patch[p].soil_defaults->soil_water_cap,0);

		apet              += patch[p].PET; //Potential evapotranspiration
		aaet              += patch[p].AET; //actual evapotranspiration
		aevaporation      += (patch[p].evaporation + patch[p].exfiltration_sat_zone  + patch[p].exfiltration_unsat_zone);
		atranspiration    += (patch[p].transpiration_sat_zone + patch[p].transpiration_unsat_zone); //
		asoil_evap        += (patch[p].exfiltration_sat_zone + patch[p].exfiltration_unsat_zone); //soil evaporation  patch[p].exfiltration_sat_zone + patch[p].exfiltration_unsat_zone
		acan_evap         += (patch[p].canopy_strata->evaporation + patch[p].canopy_strata->sublimation); //canopy strata evaporation + sublimation 
		asurf_evap        += patch[p].evaporation_surf_litter+ patch[p].evaporation_surf_soil  ; //evaporation from soil detention and litter detention //

		asnowpack         += patch[p].snowpack.water_equivalent_depth;
		asnowpack_sub     += patch[p].snowpack.sublimation;
		asnow_melt        += patch[p].snow_melt;
		//snownumber+=1;
			
		if (patch[p].sat_deficit <= ZERO)
			asat_area += 1.0;

		if(command_line->routing_flag ==1){
			//=====================================================================
			// if water is routed
			//=====================================================================
			if (patch[p].drainage_type == STREAM && patch[p].ID == patch[num_patches - 1].ID) { //patch[num_patches - 1].ID 30180
				astreamflow   += (patch[p].subsurface_Qout + patch[p].surface_Qout+patch[p].gw.Qout);
				abase_flow    += patch[p].subsurface_Qout+patch[p].gw.Qout;
				areturn_flow  += patch[p].surface_Qout;
			}
		}
		else {
			//=======================================================================
			// if water is not routed
			//=======================================================================
			astreamflow   += patch[p].subsurface_Qout+patch[p].surface_Qout;
			abase_flow    += patch[p].subsurface_Qout;
			areturn_flow  += patch[p].surface_Qout;
			//=======================================================================
		}

		for ( layer=0 ; layer<patch[p].num_layers; layer++ ){
			//for ( c=0 ; c<patch[p].layers[layer].count; c++ ){
				acanopy_store      += (patch[p].canopy_strata->rain_stored + patch[p].canopy_strata->snow_stored);
				CaIntTot           += patch[p].canopy_strata->rain_interception + patch[p].canopy_strata->snow_interception;
				CaInRain           += patch[p].canopy_strata->rain_interception;
				CaInSnow           += patch[p].canopy_strata->snow_interception;
				acan_sublimation   += patch[p].canopy_strata->sublimation;
				adC13              += patch[p].canopy_strata->dC13;
			//}
		}

		gw_drain     += patch[p].gw_drainage;
		gw_store     += patch[p].gw.storage;
		gw_Nstore    += patch[p].gw.NO3;
		gw_discharge += patch[p].gw.Qout;
		gw_Nout      += patch[p].gw.Nout;
		
	}

	fprintf(outfile,"%4.0d",current_date.year);
	fprintf(outfile,"%4.0d",current_date.month);
	fprintf(outfile,"%4.0d",current_date.day);
	fprintf(outfile,"%9.3f",arain/double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asnow/double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",arain_throughfall/double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asnow_throughfall /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asoil_store /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",alitter_store /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",acanopy_store /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",CaIntTot /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",CaInRain /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",CaInSnow /double(num_patches)*1000.); 
	fprintf(outfile,"%9.3f",(astreamflow )/double(num_patches)*1000.); //+ gw_discharge
	fprintf(outfile,"%9.3f",(abase_flow )/double(num_patches)*1000.); //+ gw_discharge
	fprintf(outfile,"%9.3f",areturn_flow /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",apet /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",aaet/double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",atranspiration/double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",aevaporation /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asoil_evap /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",acan_evap /double(num_patches)*1000.); //include canopy_sublimation
	fprintf(outfile,"%9.3f",asurf_evap /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asnowpack /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asnowpack_sub /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asnow_melt/double(num_patches)*1000.); 
	fprintf(outfile,"%9.3f",asat_deficit_z /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",asat_deficit /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",arz_storage /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",aunsat_storage /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",arz_drainage /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",aunsat_drainage /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",acap_rise /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",gw_drain/double(num_patches)*1000);
	fprintf(outfile,"%9.3f",gw_store /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",gw_Nstore /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",gw_discharge/double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",gw_Nout /double(num_patches)*1000.);
	fprintf(outfile,"%9.3f",arsmc/double(num_patches)*100);
	//fprintf(outfile,"%9.3f",asat_area/double(num_patches)*100);
	//fprintf(outfile,"%9.3f",adC13/double(num_patches)); 

	//if((aevaporation-asoil_evap-acan_evap-asurf_evap-asnowpack_sub)>ZERO){
	//	printf("evaporation not equal %12.8f %12.8f %12.8f %12.8f\n",aevaporation,asoil_evap,acan_evap,asurf_evap);
	//	getchar();
	//}
	//if(APET-apet>ZERO) {
	//	printf("%12.8f %12.8f \n",APET,apet);
	//		getchar();
	//}
	fprintf(outfile,"\n");

	return;
} /*end output_basin*/
