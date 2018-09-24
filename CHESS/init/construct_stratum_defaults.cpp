//===============================================================================================================================
//	Read stratum default file															
//===============================================================================================================================
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
#include "Functions.h"

struct stratum_default *construct_stratum_defaults(int	ntypes, FILE * default_file, struct command_line_object *command_line,
struct stratum_default *stratum_default_object_list)
{
	//--------------------------------------------------------------
	//	Local function definition.									
	//--------------------------------------------------------------
	void	*alloc(size_t, char *, char *);
	int	parse_veg_type(char *);
	int	parse_phenology_type(char *);
	int	parse_dyn_flag(char *);

	//--------------------------------------------------------------
	//	Local variable definition.									
	//--------------------------------------------------------------
	int		i;
	char		record[MAXSTR];
	char		stmp[MAXSTR];
	double		ftmp, lig_cel_ratio;
	struct	    epconst_struct	*epc = { nullptr };


	read_record(default_file, record);
	//printf("record is %s \n",record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%d", &(stratum_default_object_list[i].ID));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%s", stmp);
		stratum_default_object_list[i].epc.veg_type = parse_veg_type(stmp);
	}
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].K_absorptance));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].K_reflectance));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].K_transmittance));
		if ((float)(stratum_default_object_list[i].K_absorptance + stratum_default_object_list[i].K_reflectance
			+ stratum_default_object_list[i].K_transmittance) != 1.0){
			fprintf(stderr, "\nFATAL ERROR construct_stratum_defaults");
			fprintf(stderr, "\n K_absorptance+K_reflectance+K_transmittance must sum to 1.0");
			exit(0);
		}
	}
	read_record(default_file, record);
	


	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].PAR_absorptance));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].PAR_reflectance));
	read_record(default_file, record);

	for (i = 0; i < ntypes; i++){
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].PAR_transmittance));
		if ((float)(stratum_default_object_list[i].PAR_absorptance + stratum_default_object_list[i].PAR_reflectance
			+ stratum_default_object_list[i].PAR_transmittance) != 1.0){
			fprintf(stderr, "\nFATAL ERROR construct_stratum_defaults");
			fprintf(stderr, "\n PAR_absorptance+PAR_reflectance+PAR_transmittance must sum to 1.0");
			exit(0);
		}
	}
	read_record(default_file, record);



	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.ext_coef));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].specific_rain_capacity));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].specific_snow_capacity));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].wind_attenuation_coeff));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].ustar_overu));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].mrc.q10));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].mrc.per_N));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.gr_perc));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].lai_stomatal_fraction));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.flnr));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.ppfd_coef));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.topt));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.tmax));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.tcoef));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.psi_open));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.psi_close));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.vpd_open));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.vpd_close));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.gl_smax));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.gl_c));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].gsurf_slope));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].gsurf_intercept));
	read_record(default_file, record);

	//for (i = 0; i<ntypes; i++){
	//	fscanf(default_file, "%s", stmp);
	//	stratum_default_object_list[i].epc.phenology_flag = parse_dyn_flag(stmp);
	//	//printf("phenology is %s \n",stmp);
	//}
	//read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		fscanf(default_file, "%s", stmp);
		stratum_default_object_list[i].epc.phenology_type = parse_phenology_type(stmp);
	}
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.max_lai));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.proj_sla));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.lai_ratio));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.proj_swa));
	read_record(default_file, record);

	//for (i = 0; i<ntypes; i++)
	//	fscanf(default_file, "%d", &(stratum_default_object_list[i].epc.day_leafon));
	//read_record(default_file, record);

	//for (i = 0; i<ntypes; i++)
	//	fscanf(default_file, "%d", &(stratum_default_object_list[i].epc.day_leafoff));
	//read_record(default_file, record);

	//for (i = 0; i<ntypes; i++)
	//	fscanf(default_file, "%d", &(stratum_default_object_list[i].epc.ndays_expand));
	//read_record(default_file, record);

	//for (i = 0; i<ntypes; i++)
	//	fscanf(default_file, "%d", &(stratum_default_object_list[i].epc.ndays_litfall));
	//read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.leaf_cn));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.leaflitr_cn));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		if ((stratum_default_object_list[i].epc.leaflitr_cn <
			stratum_default_object_list[i].epc.leaf_cn) && (command_line->grow_flag > 0)){
			fprintf(stderr, "\nWARNING construct_stratum_defaults");
			fprintf(stderr, "\n  leaf litter C:N < leaf C:N");
		}
	}

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.froot_cn));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.livewood_cn));
	read_record(default_file, record);


	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].min_heat_capacity));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].max_heat_capacity));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		fscanf(default_file, "%s", stmp);
		stratum_default_object_list[i].epc.allocation_flag = parse_dyn_flag(stmp);
		//printf("allocatin flag is %s \n",stmp);
	}
	read_record(default_file, record);


	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.storage_transfer_prop));
	read_record(default_file, record);
	
	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.leaf_turnover));
	read_record(default_file, record);
	
	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.froot_turnover));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		if ((stratum_default_object_list[i].epc.veg_type == GRASS) || (stratum_default_object_list[i].epc.veg_type == C4GRASS)) {
			fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.deadleaf_turnover));
		}
		else {
			fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.livewood_turnover));
		}
	}
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.daily_fire_turnover));
	read_record(default_file, record);

	
	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.kfrag_base));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		fscanf(default_file, "%lf", &(ftmp));
		stratum_default_object_list[i].epc.daily_mortality_turnover = ftmp / 365;
	}
	read_record(default_file, record);


	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.leaflitr_flab));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.leaflitr_fscel));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.leaflitr_flig));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		epc = &(stratum_default_object_list[i].epc);
		if ((float)(epc->leaflitr_flig + epc->leaflitr_flab + epc->leaflitr_fscel) != 1.0)	{
			fprintf(stderr, "\nFATAL ERROR construct_stratum_defaults");
			fprintf(stderr, "\n  litter proportions of labile, cell. and lignin must sum to 1.0");
			fprintf(stderr, " \n for default ID %d \n", stratum_default_object_list[i].ID);
			exit(0);
		}

		lig_cel_ratio = epc->leaflitr_flig / epc->leaflitr_fscel;
		/* calculate shielded and unshielded cellulose fraction */
		if (lig_cel_ratio < 0.45){
			epc->leaflitr_fscel = 0.0;
			epc->leaflitr_fucel = epc->leaflitr_fscel;
		}
		else{
			if ((lig_cel_ratio > 0.45) && (lig_cel_ratio < 0.7)){
				epc->leaflitr_fscel = (lig_cel_ratio - 0.45)*3.2*epc->leaflitr_fscel;
				epc->leaflitr_fucel = (1.0 - 3.2*(lig_cel_ratio - 0.45))*epc->leaflitr_fscel;
			}
			else {
				epc->leaflitr_fscel = 0.8*epc->leaflitr_fscel;
				epc->leaflitr_fucel = 0.2*epc->leaflitr_fscel;
			}
		}
	}

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.frootlitr_flab));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.frootlitr_fscel));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.frootlitr_flig));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		epc = &(stratum_default_object_list[i].epc);
		if ((float)(epc->frootlitr_flab + epc->frootlitr_flig + epc->frootlitr_fscel) != 1.0){
			fprintf(stderr, "\nFATAL ERROR construct_stratum_defaults");
			fprintf(stderr, "\n  froot litter proportions of labile, cell. and lignin must sum to 1.0");
			exit(0);
		}

		lig_cel_ratio = epc->frootlitr_flig / epc->frootlitr_fscel;
		/* calculate shielded and unshielded cellulose fraction */
		if (lig_cel_ratio < 0.45) {
			epc->frootlitr_fscel = 0.0;
			epc->frootlitr_fucel = epc->frootlitr_fscel;
		}
		else{
			if ((lig_cel_ratio > 0.45) && (lig_cel_ratio < 0.7)){
				epc->frootlitr_fscel = (lig_cel_ratio - 0.45)*3.2*epc->frootlitr_fscel;
				epc->frootlitr_fucel = (1.0 - 3.2*(lig_cel_ratio - 0.45))*epc->frootlitr_fscel;
			}
			else{
				epc->frootlitr_fscel = 0.8*epc->frootlitr_fscel;
				epc->frootlitr_fucel = 0.2*epc->frootlitr_fscel;
			}
		}
	}

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.deadwood_fscel));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.deadwood_flig));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		epc = &(stratum_default_object_list[i].epc);
		if (epc->veg_type == TREE) {
			if ((float)(epc->deadwood_flig + epc->deadwood_fscel) != 1.0){
				fprintf(stderr, "\nFATAL ERROR construct_stratum_defaults");
				fprintf(stderr, "\n  dead wood proportions of labile, cell. and lignin must sum to 1.0");
				exit(0);
			}
			lig_cel_ratio = epc->deadwood_flig / epc->deadwood_fscel;
			// calculate shielded and unshielded cellulose fraction 
			if (lig_cel_ratio < 0.45) {
				epc->deadwood_fscel = 0.0;
				epc->deadwood_fucel = epc->deadwood_fscel;
			}
			else{
				if ((lig_cel_ratio > 0.45) && (lig_cel_ratio < 0.7)){
					epc->deadwood_fscel = (lig_cel_ratio - 0.45)*3.2*epc->deadwood_fscel;
					epc->deadwood_fucel = (1.0 - 3.2*(lig_cel_ratio - 0.45))*epc->deadwood_fscel;
				}
				else{
					epc->deadwood_fscel = 0.8*epc->deadwood_fscel;
					epc->deadwood_fucel = 0.2*epc->deadwood_fscel;
				}
			}
			/*
			epc->deadwood_cn = ((epc->deadwood_fucel + epc->deadwood_fscel
			+ epc->deadwood_flig) * CEL_CN * LIG_CN )
			/ (LIG_CN * (epc->deadwood_fucel + epc->deadwood_fscel)
			+ CEL_CN * epc->deadwood_flig);
			if (epc->deadwood_cn < epc->livewood_cn){
			fprintf(stderr,"\nFATAL ERROR: construct_canopy_stratum");
			fprintf(stderr,"\ndeadwood C:N must be > livewood C:N");
			exit(0);
			}
			*/
			epc->deadwood_cn = (epc->deadwood_fucel + epc->deadwood_fscel) * CEL_CN
				+ (epc->deadwood_flig) * LIG_CN;

		} // end if tree
		else {
			epc->deadwood_flig = 0.0;
			epc->deadwood_fucel = 0.0;
			epc->deadwood_fscel = 0.0;
			epc->deadwood_cn = 0.0;
		}
	}

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.alloc_frootc_leafc));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.alloc_crootc_stemc));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.alloc_stemc_leafc));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.alloc_livewoodc_woodc));
	read_record(default_file, record);

	//// double check the following syntax and intialization
	if (epc->veg_type != TREE){
		epc->alloc_crootc_stemc = 0.0;
		epc->phloemcsa_per_alllai = 0.0;
	}

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.alloc_prop_day_growth));
	read_record(default_file, record);


	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.alloc_maxlgf));
	read_record(default_file, record);



	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.height_to_stem_exp));
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++)
		fscanf(default_file, "%lf", &(stratum_default_object_list[i].epc.height_to_stem_coef));
	read_record(default_file, record);
	read_record(default_file, record);

	for (i = 0; i<ntypes; i++){
		//-----------------------------------------------------------------------------------------
		//	optionally read in parameters on re-sprouting		
		// 	and other newly implemented vegetation routines		
		//-----------------------------------------------------------------------------------------
		stratum_default_object_list[i].epc.min_leaf_carbon = 0.0005;
		stratum_default_object_list[i].epc.max_years_resprout = 100;
		stratum_default_object_list[i].epc.resprout_leaf_carbon = 0.002;
		stratum_default_object_list[i].epc.litter_gsurf_slope = 0.0;
		stratum_default_object_list[i].epc.litter_gsurf_intercept = 100000000;
		stratum_default_object_list[i].epc.coef_CO2 = 0.8;
		stratum_default_object_list[i].epc.root_growth_direction = 0.95;
		stratum_default_object_list[i].epc.root_distrib_parm = 1.0;
		//--------------------------------------------------------------
		// default values for phenology (leaf onset/offset) model parameters 
		// are set based on Jolly et al., 2005, Global Change Biology   
		// who defined a globally uniform parameter set					
		//--------------------------------------------------------------
		stratum_default_object_list[i].epc.gs_tmin = -2.0;
		stratum_default_object_list[i].epc.gs_tmax = 5.0;
		stratum_default_object_list[i].epc.gs_vpd_min = 400;
		stratum_default_object_list[i].epc.gs_vpd_max = 4100;

		stratum_default_object_list[i].epc.gs_dayl_min = 36000; //10 hours
		stratum_default_object_list[i].epc.gs_dayl_max = 43200; //12 hours
		/*---------------------------------------------------------------
		/	litter is assumed to have a mositure capacity of 	
		/	given by litter_moist_coef default assumes			
		/	200% moisture content by weight following		
		/	Helvey, 1964 (deciduous forest in Coweeta) and 		
		/	Ogee and Brunet (2002) J of Hydrology, for a pine	
		/	forest - 						
		/ 	so capacity in m is 2*litter carbon * 2(carbon to biomass 
		/		 density of water				
		//--------------------------------------------------------------*/
		stratum_default_object_list[i].epc.litter_moist_coef = 2.0 / 10000.0;

		stratum_default_object_list[i].epc.gs_dayl_range = stratum_default_object_list[i].epc.gs_dayl_max - stratum_default_object_list[i].epc.gs_dayl_min;
		stratum_default_object_list[i].epc.gs_vpd_range = stratum_default_object_list[i].epc.gs_vpd_max - stratum_default_object_list[i].epc.gs_vpd_min;
		stratum_default_object_list[i].epc.gs_trange = stratum_default_object_list[i].epc.gs_tmax - stratum_default_object_list[i].epc.gs_tmin;

		/*--------------------------------------------------------------
		/ set sunlit sla multiplier	this should be an input		
		/--------------------------------------------------------------*/
		stratum_default_object_list[i].epc.shade_sla_mult = 1;
	} 

	fclose(default_file);
	return(stratum_default_object_list);
} /*end construct_stratum_defaults*/
