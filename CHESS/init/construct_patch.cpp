#include <malloc.h>
#include <stdio.h> 

#include <stdlib.h> 
#include <string.h>
#include "CHESS.h"
#include "Constants.h"
#include <iostream>
#include "Functions.h"
using namespace std;

void construct_patch(struct patch_object * patch, struct command_line_object * command_line, int rows, int cols, char *filename, char *prefix, int num_patches)
{
	//defaults filename

	//xu.
	char fnclimatedef[MAXS], fnsoildef[MAXS], fnlandusedef[MAXS], fnvegdef[MAXS], fnchanneldef[MAXS];
	//char fcanopytemp[MAXS],fpatchtemp[MAXS];

	//local parameters
	FILE  *default_file;
	int    soiltypes, landusetypes, vegtypes, climatetypes, channeltypes;
	int    nvars, p, j;

	//7 local functions to read four defaults and two template files 
	struct soil_default *soil_default_object_list{};
	struct soil_default *construct_soil_defaults(int, FILE *, struct command_line_object *, struct soil_default *);

	struct stratum_default *stratum_default_object_list{};
	struct stratum_default *construct_stratum_defaults(int, FILE	*, struct command_line_object *, struct stratum_default *);

	struct landuse_default *landuse_default_object_list{};
	struct landuse_default *construct_landuse_defaults(int, FILE	*, struct command_line_object *, struct landuse_default *);

	struct climate_default *climate_default_object_list{};
	struct climate_default *construct_climate_defaults(int, FILE	*, struct command_line_object *, struct climate_default *);

	//xu.
	struct channel_default *channel_default_object_list{};
	struct channel_default *construct_channel_defaults(int, FILE *, struct command_line_object *, struct channel_default *);


	struct canopy_strata_object *canopy_strata{};
	void   construct_canopy_strata(struct command_line_object	*, double, double, double, struct canopy_strata_object *);

	void    *print(struct command_line_object *);
	char    record[MAXSTR];
	double	compute_z_final(int, double, double, double, double, double);
	void    sort_patch_layers(struct patch_object *);
	void	update_litter_interception_capacity(double, struct litter_c_object *, struct litter_object *);
	void	*alloc(size_t size, const char *array_name, const char *calling_function);
	double  atm_pres(double);
	float   spaveg(float, float);

	//local variable
	double soil_depth, air_entry, pore_size;

	strcat(filename, prefix);
	strcpy(fnclimatedef, filename);
	strcpy(fnsoildef, filename);
	strcpy(fnvegdef, filename);
	strcpy(fnlandusedef, filename);
	strcpy(fnchanneldef, filename);

	// append '.' extensions to each filename (these should be generalized) 
	strcat(fnsoildef, "_soil.def");
	strcat(fnvegdef, "_veg.def");
	strcat(fnclimatedef, "_climate.def");
	strcat(fnlandusedef, "_landuse.def");
	strcat(fnchanneldef, "_channel.def");

	//===========================================================================================================================
	// 1. reading soil default files
	//===========================================================================================================================
	if ((default_file = fopen(fnsoildef, "r")) == NULL) {
		fprintf(stderr, "cannot open soil default file %s\n", fnsoildef);
		return;
	}
	else {
		fscanf(default_file, "%d ", &soiltypes);
		read_record(default_file, record);
		fscanf(default_file, "%d ", &nvars);
		read_record(default_file, record);
		printf("\n");
		printf("The number of soil types are %d \n", soiltypes);
		printf("The number of soil variables are %d \n", nvars);
	}

	soil_default_object_list = (struct soil_default *) calloc(soiltypes, sizeof(struct soil_default));
	if (soil_default_object_list == NULL) {
		printf("out of memory for assinging soil default_object_list in construct_patch.c 1\n");
		return;
	}
	construct_soil_defaults(soiltypes, default_file, command_line, soil_default_object_list);
	printf("Finishing reading soil default file  \n");


	//===========================================================================================================================
	// 2. reading vegetation default files
	//===========================================================================================================================
	if ((default_file = fopen(fnvegdef, "r")) == NULL) {
		fprintf(stderr, "cannot open vegetation default file %s\n", fnvegdef);
		return;
	}
	else {
		fscanf(default_file, "%d ", &vegtypes);
		read_record(default_file, record);
		fscanf(default_file, "%d ", &nvars);
		read_record(default_file, record);
		printf("\n");
		printf("The number of vegeation types are %d \n", vegtypes);
		printf("The number of veg-default variables are %d \n", nvars);
	}

	stratum_default_object_list = (struct stratum_default *) calloc(vegtypes, sizeof(struct stratum_default));
	if (stratum_default_object_list == NULL) {
		printf("out of memory for assinging vegetation default in construct_patch.c 1\n");
		return;
	}
	construct_stratum_defaults(vegtypes, default_file, command_line, stratum_default_object_list);
	printf("Finishing reading vegetation_default file  \n");


	//===========================================================================================================================
	// 3. reading landuse default files
	//===========================================================================================================================
	if ((default_file = fopen(fnlandusedef, "r")) == NULL) {
		fprintf(stderr, "cannot open soil default file %s\n", fnlandusedef);
		return;
	}
	else {
		fscanf(default_file, "%d ", &landusetypes);
		read_record(default_file, record);
		fscanf(default_file, "%d ", &nvars);
		read_record(default_file, record);
		printf("\n");
		printf("The number of landuse types are %d \n", landusetypes);
		printf("The number of landuse-default variables are %d \n", nvars);
	}

	landuse_default_object_list = (struct landuse_default *) calloc(landusetypes, sizeof(struct landuse_default));
	if (landuse_default_object_list == NULL) {
		printf("out of memory for assinging landuse default in construct_patch.c 1\n");
		return;
	}
	construct_landuse_defaults(landusetypes, default_file, command_line, landuse_default_object_list);
	printf("Finishing reading landuse_default file  \n");

	//===========================================================================================================================
	// 4. reading climate default files
	//===========================================================================================================================
	if ((default_file = fopen(fnclimatedef, "r")) == NULL) {
		fprintf(stderr, "cannot open climate default file %s\n", fnclimatedef);
		return;
	}
	else {
		fscanf(default_file, "%d ", &climatetypes);
		read_record(default_file, record);
		fscanf(default_file, "%d ", &nvars);
		read_record(default_file, record);
		printf("\n", climatetypes);
		printf("The number of climate types are %d \n", climatetypes);
		printf("The number of climate-default variables are %d \n", nvars);
	}

	climate_default_object_list = (struct climate_default *) calloc(climatetypes, sizeof(struct climate_default));
	if (climate_default_object_list == NULL) {
		printf("out of memory for assinging climate default in construct_patch.c 1\n");
		return;
	}
	construct_climate_defaults(climatetypes, default_file, command_line, climate_default_object_list);
	printf("Finishing reading climate_default file  \n");


	//===========================================================================================================================
	// 5. read channel default files
	//===========================================================================================================================
	if (command_line->cf == TRUE) {

		if ((default_file = fopen(fnchanneldef, "r")) == NULL) {
			fprintf(stderr, "cannot open channel default file %s\n", fnchanneldef);
			return;
		}
		else {
			fscanf(default_file, "%d ", &channeltypes);
			read_record(default_file, record);
			fscanf(default_file, "%d ", &nvars);
			read_record(default_file, record);
			printf("\n");
			printf("The number of channel types are %d \n", channeltypes);
			printf("The number of channel variables are %d \n", nvars);
		}

		channel_default_object_list = (struct channel_default *) calloc(channeltypes, sizeof(struct channel_default));
		if (channel_default_object_list == NULL) {
			printf("out of memory for assinging landuse default in construct_patch.c 1\n");
			return;
		}

		construct_channel_defaults(channeltypes, default_file, command_line, channel_default_object_list);
		printf("Finishing reading channel_default file  \n");
	}


	//===========================================================================================================================
	// Starting parameters initialization
	//===========================================================================================================================

	//xu. I delete the if patch[p].ID>0 
	//and there's no need to change anything in this part
	for (p = 0; p < num_patches; p++) {

		//=============================================================================================================================
		//1.Assign PATCH defaults for this patch						
		//=============================================================================================================================
		patch[p].climate_defaults = &climate_default_object_list[0];
		for (j = 0; j < climatetypes; j++) {
			if (patch[p].climatetype == climate_default_object_list[j].ID) {
				patch[p].climate_defaults = &climate_default_object_list[j];
			}
		}
		//--------------------------------------------------------------
		//	Create cosine of latitude to save future computations.		
		//--------------------------------------------------------------
		//xu. latitude is an geo image input in read_geo_image.cpp 
		//patch[p].latitude = patch[p].climate_defaults->latitude;  //averaged latitude for the whole watershed
		patch[p].cos_latitude = cos(patch[p].latitude*DtoR);
		patch[p].sin_latitude = sin(patch[p].latitude*DtoR);

		//--------------------------------------------------------------
		//	convert from degrees to radians for slope and aspects 	
		//--------------------------------------------------------------
		//Note: spaveg convert radian angle to degreess. if your input aspect
		//is in degree, sapveg should not be used

		//patch[p].aspect = spaveg(patch[p].aspect,patch[p].slope);
		patch[p].aspect = patch[p].aspect * (float)DtoR;
		patch[p].slope = patch[p].slope * (float)DtoR;

		//--------------------------------------------------------------
		//	Define cos and sine of slopes and aspects.					
		//--------------------------------------------------------------
		patch[p].cos_aspect = cos(patch[p].aspect);
		patch[p].cos_slope = cos(patch[p].slope);
		patch[p].sin_aspect = sin(patch[p].aspect);
		patch[p].sin_slope = sin(patch[p].slope);

		//--------------------------------------------------------------
		//	Initialize any variables that should be initialized at the start of a simulation run for the zone.				
		//--------------------------------------------------------------
		patch[p].metv.pa = atm_pres(patch[p].z);
		patch[p].metv.tsoil_sum = 0.0;
		patch[p].metv.tsoil = 0.0;
		patch[p].metv.tmin_ravg = 3.0;
		patch[p].metv.vpd_ravg = 900;
		patch[p].metv.dayl_ravg = 38000;

		//=============================================================================================================================
		//Initialize watershed defaults variables
		//=============================================================================================================================
		patch[p].mean_z = patch[p].climate_defaults->mean_elev;
		patch[p].screen_height = patch[p].climate_defaults->screen_height;

		patch[p].std = patch[p].climate_defaults->std;

		patch[p].Ksat_vertical = 0.95;
		patch[p].rz_storage = 0.0;
		patch[p].unsat_storage = 0.0;
		patch[p].rootzone.S = 0.0;

		patch[p].sat_deficit = 0.0;
		patch[p].snowpack.water_equivalent_depth = 0.0;
		patch[p].snowpack.water_depth = 0.0;
		patch[p].snowpack.T = patch[p].climate_defaults->snowpack.T;
		patch[p].snowpack.surface_age = 0.0;
		patch[p].snowpack.energy_deficit = patch[p].climate_defaults->snowpack.energy_deficit;
		patch[p].snow_redist_scale = 0.0;

		patch[p].gw_drainage = 0.0;
		patch[p].infiltration_excess = 0.0;
		patch[p].snowpack.height = patch[p].snowpack.water_equivalent_depth *10.0;
		patch[p].detention_store = 0.0;

		//canopy related parameter
		patch[p].rootzone.depth = patch[p].climate_defaults->root_depth;

		//patch[p].surface_Tday   = -999.9;
		//patch[p].surface_Tnight = -999.9;
		//patch[p].deltaS         = 0.0;
		//--------------------------------------------------------------
		//	Variables for the dynamic version are included here     
		//--------------------------------------------------------------
		patch[p].litter.rain_stored = 0.0;
		patch[p].litter_cs.litr1c = 0.0;
		patch[p].litter_cs.litr2c = 0.0;
		patch[p].litter_cs.litr3c = 0.0;
		patch[p].litter_cs.litr4c = 0.0;

		patch[p].litter_ns.litr1n = 0.0;
		patch[p].litter_ns.litr2n = patch[p].litter_cs.litr2c / CEL_CN;
		patch[p].litter_ns.litr3n = patch[p].litter_cs.litr3c / CEL_CN;
		patch[p].litter_ns.litr4n = patch[p].litter_cs.litr4c / LIG_CN;

		patch[p].soil_cs.soil1c = 0.0;
		patch[p].soil_cs.soil2c = 0.0;
		patch[p].soil_cs.soil3c = 0.0;
		patch[p].soil_cs.soil4c = 0.0;

		patch[p].soil_ns.soil1n = patch[p].soil_cs.soil1c / SOIL1_CN;
		patch[p].soil_ns.soil2n = patch[p].soil_cs.soil2c / SOIL2_CN;
		patch[p].soil_ns.soil3n = patch[p].soil_cs.soil3c / SOIL3_CN;
		patch[p].soil_ns.soil4n = patch[p].soil_cs.soil4c / SOIL4_CN;

		patch[p].soil_ns.sminn = 0.0;
		patch[p].soil_ns.nitrate = 0.0;


		//--------------------------------------------------------------
		//	initialize sinks					
		//--------------------------------------------------------------
		patch[p].litter_cs.litr1c_hr_snk = 0.0;
		patch[p].litter_cs.litr2c_hr_snk = 0.0;
		patch[p].litter_cs.litr4c_hr_snk = 0.0;

		patch[p].soil_cs.soil1c_hr_snk = 0.0;
		patch[p].soil_cs.soil2c_hr_snk = 0.0;
		patch[p].soil_cs.soil4c_hr_snk = 0.0;

		patch[p].soil_ns.nfix_src = 0.0;
		patch[p].soil_ns.ndep_src = 0.0;
		patch[p].soil_ns.nvolatilized_snk = 0.0;

		patch[p].surface_NO3 = 0.0;
		patch[p].surface_NH4 = 0.0;
		patch[p].fertilizer_NO3 = 0.0;
		patch[p].fertilizer_NH4 = 0.0;

		//=============================================================================================================================
		//2.Assign SOIL defaults for this patch						
		//=============================================================================================================================

		//intial other value such as -9999 
		patch[p].soil_defaults = &soil_default_object_list[0];
		for (j = 0; j < soiltypes; j++) {
			if (patch[p].soiltype == soil_default_object_list[j].ID) {
				patch[p].soil_defaults = &soil_default_object_list[j];
			}
		}

		patch[p].soil_defaults->m_z = patch[p].soil_defaults->m_z;

		//--------------------------------------------------------------
		// compute a biological soil depth based on the minimum of soil depth 
		// and m, K parameters defining conductivity < 0.1% original value 
		//--------------------------------------------------------------
		patch[p].soil_defaults->effective_soil_depth = min(patch[p].soil_defaults->soil_depth,
			6.9*patch[p].soil_defaults->m_z);

		//printf("%12.8f %12.8f \n",patch[p].soil_defaults->soil_water_cap);
		patch[p].sat_deficit = patch[p].soil_defaults->soil_water_cap;

		//=============================================================================================================================
		//3.Assign	LANDUSE defaults for this patch						
		//=============================================================================================================================
		patch[p].landuse_defaults = &landuse_default_object_list[0];
		for (j = 0; j < landusetypes; j++) {
			if (patch[p].vegtype == landuse_default_object_list[j].ID)
				patch[p].landuse_defaults = &landuse_default_object_list[j];
		}

		//--------------------------------------------------------------------------------------------------
		// detention store size can vary with both soil and landuse	use the maximum of the two									
		//--------------------------------------------------------------------------------------------------
		patch[p].soil_defaults->detention_store_size = max(patch[p].landuse_defaults->detention_store_size,
			patch[p].soil_defaults->detention_store_size);

		//--------------------------------------------------------------------------------------------------
		//	if landuse default files include a percent impervious use this to over-ride Ksat vertical			
		//--------------------------------------------------------------------------------------------------
		if (patch[p].landuse_defaults->percent_impervious > ZERO)
			patch[p].Ksat_vertical = 1.0 - patch[p].landuse_defaults->percent_impervious;

		//--------------------------------------------------------------
		//      Allocate the patch hourly object.	  
		//--------------------------------------------------------------
		if ((patch[p].hourly = (struct patch_hourly_object *) calloc(1,
			sizeof(struct patch_hourly_object))) == NULL) {
			fprintf(stderr, "FATAL ERROR: in patch_hourly\n");
			exit(0);
		}

		//--------------------------------------------------------------
		//      Initialize patch level rainand snow stored              
		//--------------------------------------------------------------
		patch[p].rain_stored = 0.0;
		patch[p].snow_stored = 0.0;
		patch[p].soil_defaults->daily_fire_litter_turnover = 0.0;
		patch[p].litter.gl_c = 0.0;
		patch[p].litter.gsurf_slope = 0.0;
		patch[p].litter.gsurf_intercept = 0.0;

		//=============================================================================================================================
		//	Construct the canopy strata in this patch, including initialization of all canopy related parameters.						    
		//=============================================================================================================================
		//	Allocate list of pointers to stratum objects .				
		patch[p].canopy_strata = (struct canopy_strata_object *) malloc(sizeof(struct canopy_strata_object));
		patch[p].num_canopy_strata = 1; //added by guoping to assume that only one vegetation types exist


										/*--------------------------------------------------------------*/
										/*	Assign	defaults for this canopy_strata						*/
										/*--------------------------------------------------------------*/
										//canopy_strata->defaults=&stratum_default_object_list[0];

										//fixed by xu.
										//when there is -9999, forcing it use first default
										//a same procedule is in soil defaults
		patch[p].canopy_strata->defaults = &stratum_default_object_list[0];
		for (j = 0; j < vegtypes; j++) {
			if (patch[p].vegtype == stratum_default_object_list[j].ID) {
				patch[p].canopy_strata->defaults = &stratum_default_object_list[j];
			}
		}


		//*(patch[p].canopy_strata->defaults) =*(canopy_strata->defaults) ; //important
		//patch[p].canopy_strata = &canopy_strata->defaults; //important

		soil_depth = patch[p].soil_defaults->effective_soil_depth;
		air_entry = patch[p].soil_defaults->psi_air_entry;
		pore_size = patch[p].soil_defaults->pore_size_index;

		patch[p].canopy_strata->cover_fraction = patch[p].climate_defaults->cover_fraction;
		patch[p].canopy_strata->gap_fraction = patch[p].climate_defaults->gap_fraction;

		patch[p].canopy_strata->dC13 = 10.; //initialization added by guoping

		patch[p].canopy_strata->cs.DayMaxLeafc = 0.1;
		patch[p].canopy_strata->cs.DayMaxFrootc = 0.1;

		//patch[p].canopy_strata->defaults->epc.phenology_type=

		for (j = 0; j < patch[p].num_canopy_strata; j++) { //only one strata occurs
														  //patch[p].canopy_strata = construct_canopy_strata(command_line,soil_depth,air_entry,pore_size,
														  //	canopy_strata,patch[p].canopy_strata->defaults,p);stratum.cover_fraction

			construct_canopy_strata(command_line, soil_depth, air_entry, pore_size, patch[p].canopy_strata);

			// initialize other canopy related variables
			patch[p].rain_stored += patch[p].canopy_strata->rain_stored	* patch[p].canopy_strata->cover_fraction;
			patch[p].snow_stored += patch[p].canopy_strata->snow_stored	* patch[p].canopy_strata->cover_fraction;

			patch[p].soil_defaults->daily_fire_litter_turnover += patch[p].canopy_strata->defaults->epc.daily_fire_turnover
				* patch[p].canopy_strata->cover_fraction;


			patch[p].litter.gl_c += patch[p].canopy_strata->defaults->epc.gl_c * patch[p].canopy_strata->cover_fraction;

			patch[p].litter.gsurf_slope += patch[p].canopy_strata->defaults->epc.litter_gsurf_slope
				* patch[p].canopy_strata->cover_fraction;

			patch[p].litter.gsurf_intercept += patch[p].canopy_strata->defaults->epc.litter_gsurf_intercept
				* patch[p].canopy_strata->cover_fraction;

			patch[p].litter.moist_coef += patch[p].canopy_strata->defaults->epc.litter_moist_coef
				* patch[p].canopy_strata->cover_fraction;

			if (command_line->grow_flag > 0) //for vegetation simulation Guoping
				patch[p].canopy_strata->rootzone.depth = 0.;

			patch[p].rootzone.depth = max(patch[p].rootzone.depth, patch[p].canopy_strata->rootzone.depth);

		} //end for


		  //--------------------------------------------------------------
		  //	initialize litter capacity				
		  //--------------------------------------------------------------
		update_litter_interception_capacity(patch[p].litter.moist_coef, &(patch[p].litter_cs), &(patch[p].litter));

		//--------------------------------------------------------------
		//	Define a list of canopy strata layers that can at least	fit all of the canopy strata.				
		//--------------------------------------------------------------
		patch[p].layers = (struct layer_object *) alloc(patch[p].num_canopy_strata * sizeof(struct layer_object), "layers", "construct_patch");
		patch[p].num_layers = 0;


		//xu. a little confused with this function
		sort_patch_layers(&patch[p]);

		//--------------------------------------------------------------
		//	compute actual depth to water tablke			
		//--------------------------------------------------------------
		patch[p].unsat_zone_volume = patch[p].sat_deficit + patch[p].unsat_storage;
		patch[p].sat_deficit_z = compute_z_final(
			command_line->verbose_flag,
			patch[p].soil_defaults->porosity_0,
			patch[p].soil_defaults->porosity_decay,
			patch[p].soil_defaults->soil_depth,
			0,
			-1 * patch[p].sat_deficit);

		//ground water object
		patch[p].gw.storage = 0.;
		patch[p].gw.NO3 = 0.;
		patch[p].gw.Nout = 0.;
		patch[p].gw.Qout = 0.;

		patch[p].preyear_litfall_stopday = 300;
		for (int kk = 0; kk < 21; kk++) {
			patch[p].tmin21[kk] = 0.;
			patch[p].vpd21[kk] = 0.;
			patch[p].dayl21[kk] = 0.;
		}

		//xu.
		//=============================================================================================================================
		//	Assign channel defaults for this patch						
		//=============================================================================================================================


		if (command_line->cf == TRUE) {
			if (patch[p].streamorder > 0) {

				//xu. assigning size
				patch[p].channel = new struct channel_object;

				patch[p].channel_defaults = &channel_default_object_list[0];
				for (j = 0; j < channeltypes; j++) {
					if (patch[p].streamorder == channel_default_object_list[j].strahler_order) {
						patch[p].channel_defaults = &channel_default_object_list[j];
					}
				}
				patch[p].channel->storage = {};

				patch[p].channel->channel_width = patch[p].channel_defaults->channel_width;//1 default

				patch[p].channel->hydraulic_roughness = patch[p].channel_defaults->hydraulic_roughness;//2

				patch[p].channel->Q_in = {};
				patch[p].channel->Q_out = {};

				patch[p].channel->k = {};
				patch[p].channel->Rr = {};
				patch[p].channel->channel_slope = {};
				patch[p].channel->channel_length = {};
				patch[p].channel->strahler_order = patch[p].channel_defaults->strahler_order;//3
			}//end
		}

	}

	printf("\nFinishing initialization of all parameter in construct_patch.cpp  \n");

	return;
};


void *print(struct command_line_object *command_line) {
	printf("command_line %d \n", command_line->grow_flag);
	getchar();
	return 0;
};