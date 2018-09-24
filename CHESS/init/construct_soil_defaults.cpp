//===============================================================================================================================
// reading soil default files
//===============================================================================================================================
#include <stdio.h>

#include "Functions.h"
#include "CHESS.h"
#include "Constants.h"

struct soil_default *construct_soil_defaults(int ntypes, FILE * default_file, struct command_line_object *command_line,
struct soil_default *soil_default_object_list)
{
	//--------------------------------------------------------------
	//	Local function definition.									
	//--------------------------------------------------------------
	void	*alloc(size_t, char	*, char	*);
	double  compute_delta_water(int, double, double, double, double, double);

	//--------------------------------------------------------------
	//	Local variable definition.									
	//--------------------------------------------------------------
	int		i;
	double 	soil;
	char	record[MAXSTR];

	//--------------------------------------------------------------
	//		read the default file into the ith object.			
	//--------------------------------------------------------------
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%s", &(soil_default_object_list[i].Soil_Name));
	}
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%d", &(soil_default_object_list[i].ID));
		//printf(" soil ID is %d \n", soil_default_object_list[i].ID);
		//getchar();
	}
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%d", &(soil_default_object_list[i].theta_psi_curve));
	}
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].Ksat_0));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].m_z));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].porosity_0));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].porosity_decay));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].p3));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].pore_size_index));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].psi_air_entry));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].psi_max));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].soil_depth));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].detention_store_size));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].deltaz));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].active_zone_z));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].max_heat_capacity));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].min_heat_capacity));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].albedo));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].mobile_N_proportion));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(soil_default_object_list[i].N_decay_rate));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%lf", &(soil_default_object_list[i].soil_type.sand));
		//printf(" silt is %f \n", soil_default_object_list[i].soil_type.sand);
		//getchar();
	}
	read_record(default_file, record);


	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%lf", &(soil_default_object_list[i].soil_type.clay));
		//printf(" silt is %f \n", soil_default_object_list[i].soil_type.clay);
		//getchar();
	}
	read_record(default_file, record);
	
	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%lf", &(soil_default_object_list[i].soil_type.silt));
		//printf(" silt is %f \n", soil_default_object_list[i].soil_type.silt);
		//getchar();
	}
	read_record(default_file, record);

	for(i=0; i<ntypes; i++){
		soil =  soil_default_object_list[i].soil_type.sand + soil_default_object_list[i].soil_type.silt
				+ soil_default_object_list[i].soil_type.clay;

		if  (fabs(soil - 1.0) > ZERO) {
			fprintf(stderr,
				"FATAL ERROR:in construct_soil_defaults\n proportion sand, silt, clay = %f\n\n", soil);
			printf("\n %d -  %f %f %f %f \n",
				soil_default_object_list[i].ID,
				soil_default_object_list[i].N_decay_rate,
				soil_default_object_list[i].soil_type.sand,
				soil_default_object_list[i].soil_type.silt,
				soil_default_object_list[i].soil_type.clay);
		} 
	}

	for(i=0; i<ntypes; i++)
		fscanf(default_file,"%lf",&(soil_default_object_list[i].sat_to_gw_coeff));
	read_record(default_file, record);

	for(i=0; i<ntypes; i++){
		if (command_line->gw_flag > 0) {
			soil_default_object_list[i].sat_to_gw_coeff *= command_line->sat_to_gw_coeff_mult;
		}
	}

	for(i=0; i<ntypes; i++){
		fscanf(default_file,"%lf",&(soil_default_object_list[i].DON_loss_rate));
		//printf("DON_loss_rate is %f \n",soil_default_object_list[i].DON_loss_rate);
	}
	read_record(default_file, record);
	read_record(default_file, record);

	for(i=0; i<ntypes; i++){
		//---------------------------------------------------------------------------------------------------------------
		// sensitivity adjustment of vertical drainage  soil paramters	
		// an alternative is to scale Pore size index and psi air entry	
		//---------------------------------------------------------------------------------------------------------------
			if (soil_default_object_list->pore_size_index >= 1.0) {
				printf("\n Sensitivity analysis giving Pore Size Index > 1.0, not allowed, setting to 1.0\n");
				soil_default_object_list[i].pore_size_index = 0.999;
			}


		//---------------------------------------------------------------------------------------------------------------
		//      calculate water_equivalent depth of soil             
		//---------------------------------------------------------------------------------------------------------------
		soil_default_object_list[i].soil_water_cap = compute_delta_water(
			0, soil_default_object_list[i].porosity_0,
			soil_default_object_list[i].porosity_decay,
			soil_default_object_list[i].soil_depth,
			soil_default_object_list[i].soil_depth,
			0.0);

		//---------------------------------------------------------------------------------------------------------------
		//	for the moment assign values for soil conductance computations
		//---------------------------------------------------------------------------------------------------------------
		soil_default_object_list[i].gl_c            = 0.0062;
		soil_default_object_list[i].gsurf_slope     = 0.01;
		soil_default_object_list[i].gsurf_intercept = 0.001;
	} 
    
	fclose(default_file);

	return(soil_default_object_list);
} 
