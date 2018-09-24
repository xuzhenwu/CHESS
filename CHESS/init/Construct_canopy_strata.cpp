/*--------------------------------------------------------------*/
/* 																*/
/*					construct_canopy_strata						*/
/*																*/
/*	construct_canopy_strata->c - creates a canopy_strata object	*/
/*																*/
/*	NAME														*/
/*	construct_canopy_strata->c - creates a canopy_strata object	*/
/*																*/
/*	SYNOPSIS													*/
/*	struct canopy_strata_object  construct_canopy_strata( 		*/
/*				struct	command_line_object	*command_line,		*/
/*				FILE	*world_file)						*/
/*																*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	Allocates memory for a canopy strata object and reads in 	*/
/*	parameters for the object from an open hillslopes file.		*/
/*																*/
/*	Refer to construct_basin.c for a specification of the 		*/
/*	hillslopes file.											*/
/*																*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	We assume that the FILE pointers to the 					*/
/*	hillslope file are positioned properly.						*/
/*	 															*/
/*	We assume that the basin and hillslope files have correct	*/
/*	syntax.														*/
/*																*/
/*	Original code, January 16, 1996.							*/
/*																*/
/*	Sep 2 1997	RAF												*/
/*	Removed all references to grow or extended state variables.	*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"


void construct_canopy_strata(
	struct	command_line_object	*command_line,
	double soil_depth,double air_entry,double pore_size,
	struct	canopy_strata_object	*canopy_strata)

{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	struct base_station_object *assign_base_station(
		int ,
		int ,
		struct base_station_object **);
	
	
	int compute_annual_turnover(struct epconst_struct,
		struct epvar_struct *,
		struct cstate_struct *);


	int	compute_annual_litfall(
		struct epconst_struct,
		struct phenology_struct *,
		struct cstate_struct *,
		int);
	
	int	update_rooting_depth(
		struct rooting_zone_object *,
		double,
		double,
		double,
		double);

	void	*alloc(size_t, char *, char *);
	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	//int	base_stationID;
	//int	n,k;
	double	sai, rootc;
	//int	default_object_ID;
	//char	record[MAXSTR];


	//=============================================================================================================================
	//Initialize all canopy related parameters as zero before simulations
	//=============================================================================================================================
	canopy_strata->snow_stored = 0.;								   // snow_stored		  
	canopy_strata->rain_stored = 0.;								   // rain_stored		  
	canopy_strata->cs.cpool = 0.;									   // cs_pool			  
	canopy_strata->cs.leafc = 0.0;									   // cs_leafc		  
	canopy_strata->cs.dead_leafc = 0.0;								   // cs_dead_leafc		  
	canopy_strata->cs.leafc_store = 0.;							       // cs_leafc_store		  
	canopy_strata->cs.leafc_transfer = 0.;							   // cs_leafc_transfer	  
	canopy_strata->cs.live_stemc = 0.;								   // cs_live_stemc		  
	canopy_strata->cs.livestemc_store = 0.;						       // cs_live_stemc_store	  
	canopy_strata->cs.livestemc_transfer = 0.;						   // cs_live_stemc_transfer    
	canopy_strata->cs.dead_stemc = 0.;								   // cs_dead_stemc		  
	canopy_strata->cs.deadstemc_store = 0.;						       // cs_dead_stemc_store	  
	canopy_strata->cs.deadstemc_transfer = 0.;						   // cs_dead_stemc_transfer	  
	canopy_strata->cs.live_crootc = 0.;							       // cs_live_crootc		  
	canopy_strata->cs.livecrootc_store = 0.;						   // cs_live_crootc_store	  
	canopy_strata->cs.livecrootc_transfer = 0.;					       // cs_live_crootc_transfer	  
	canopy_strata->cs.dead_crootc = 0.;							       // cs_dead_crootc		  
	canopy_strata->cs.deadcrootc_store = 0.;						   // cs_dead_crootc_store	  
	canopy_strata->cs.deadcrootc_transfer = 0.;					       // cs_dead_crootc_transfer	  
	canopy_strata->cs.frootc = 0.;									   // cs_frootc		  
	canopy_strata->cs.frootc_store = 0.;							   // cs_frootc_store		  
	canopy_strata->cs.frootc_transfer = 0.;						       // cs_frootc_transfer	  
	canopy_strata->cs.cwdc = 0.;									   // cs_cwdc			  
	canopy_strata->epv.prev_leafcalloc = 0.;						   // epv.prev_leafcalloc	  
	
	canopy_strata->ns.npool = 0.;									   // ns_pool			  
	canopy_strata->ns.leafn = 0.;									   // ns_leafn		  
	canopy_strata->ns.dead_leafn = 0.;								   // ns_dead_leafn		  
	canopy_strata->ns.leafn_store = 0.;							       // ns_leafn_store		  
	canopy_strata->ns.leafn_transfer = 0.;							   // ns_leafn_transfer	  
	canopy_strata->ns.live_stemn = 0.;								   // ns_live_stemn		  
	canopy_strata->ns.livestemn_store = 0.;						       // ns_live_stemn_store	  
	canopy_strata->ns.livestemn_transfer = 0.;						   // ns_live_stemn_transfer	  
	canopy_strata->ns.dead_stemn = 0.;								   // ns_dead_stemn		  
	canopy_strata->ns.deadstemn_store = 0.;						       // ns_dead_stemn_store	  
	canopy_strata->ns.deadstemn_transfer = 0.;						   // ns_dead_stemn_transfer	  
	canopy_strata->ns.live_crootn = 0.;							       // ns_live_crootn		  
	canopy_strata->ns.livecrootn_store = 0.;						   // ns_live_crootn_store	  
	canopy_strata->ns.livecrootn_transfer = 0.;					       // ns_live_crootn_transfer	  
	canopy_strata->ns.dead_crootn = 0.;							       // ns_dead_crootn		  
	canopy_strata->ns.deadcrootn_store = 0.;						   // ns_dead_crootn_store	  
	canopy_strata->ns.deadcrootn_transfer = 0.;					       // ns_dead_crootn_transfer	  
	canopy_strata->ns.frootn = 0.;									   // ns_frootn		  
	canopy_strata->ns.frootn_store = 0.;							   // ns_frootn_store		  
	canopy_strata->ns.frootn_transfer = 0.;						       // ns_frootn_transfer	  
	canopy_strata->ns.cwdn = 0.;									   // ns_cwdn			  
	canopy_strata->ns.retransn = 0.;								   // ns_retransn		  
 																		  
	canopy_strata->epv.wstress_days = 0;                              // epv_wstress		  
	canopy_strata->epv.max_fparabs = 0.;							   // epv_min_fparabs		
	canopy_strata->epv.min_vwc = 0.;								   // epv_min_vwc		  
	

	canopy_strata->cs.gresp_transfer   = 0.0; //added by guoping for initialization
	canopy_strata->cs.gpsn_src         = 0.0;
	canopy_strata->cs.leaf_mr_snk      = 0.0;
	canopy_strata->cs.leaf_gr_snk      = 0.0;
	canopy_strata->cs.livestem_mr_snk  = 0.0;
	canopy_strata->cs.livestem_gr_snk  = 0.0;
	canopy_strata->cs.deadstem_gr_snk  = 0.0;
	canopy_strata->cs.livecroot_mr_snk = 0.0;
	canopy_strata->cs.livecroot_gr_snk = 0.0;
	canopy_strata->cs.deadcroot_gr_snk = 0.0;
	canopy_strata->cs.froot_mr_snk     = 0.0;
	canopy_strata->cs.froot_gr_snk     = 0.0;
	

	canopy_strata->epv.proj_sla_sunlit = canopy_strata->defaults->epc.proj_sla;
	canopy_strata->epv.proj_sla_shade  = canopy_strata->defaults->epc.proj_sla *canopy_strata->defaults->epc.shade_sla_mult;



	if ( canopy_strata->cs.leafc <= 1.0/canopy_strata->epv.proj_sla_sunlit) {

		canopy_strata->epv.proj_lai        = canopy_strata->cs.leafc *	canopy_strata->epv.proj_sla_sunlit;
		canopy_strata->epv.proj_lai_sunlit = canopy_strata->epv.proj_lai;
		canopy_strata->epv.proj_lai_shade  = 0.0;
	}
	else  {
		canopy_strata->epv.proj_lai = 1.0 + ( canopy_strata->cs.leafc -	1.0/canopy_strata->epv.proj_sla_sunlit) *
				canopy_strata->epv.proj_sla_shade;
		canopy_strata->epv.proj_lai_sunlit = 1.0;
		canopy_strata->epv.proj_lai_shade = canopy_strata->epv.proj_lai - 1.0;
	}

	canopy_strata->epv.all_lai      = canopy_strata->epv.proj_lai *	canopy_strata->defaults->epc.lai_ratio;
	canopy_strata->epv.max_proj_lai =  canopy_strata->epv.proj_lai;
	
	if (canopy_strata->defaults->epc.veg_type == TREE)
		canopy_strata->epv.height = canopy_strata->defaults->epc.height_to_stem_coef
		* pow((canopy_strata->cs.live_stemc + canopy_strata->cs.dead_stemc),
		canopy_strata->defaults->epc.height_to_stem_exp);
	else
		canopy_strata->epv.height = canopy_strata->defaults->epc.height_to_stem_coef
		* pow((canopy_strata->cs.leafc + canopy_strata->cs.dead_leafc),
		canopy_strata->defaults->epc.height_to_stem_exp);
 

	/*--------------------------------------------------------------*/
	/*	calculate all sided  and project pai from max projected lai	*/
	/*--------------------------------------------------------------*/
	if (canopy_strata->defaults->epc.veg_type == TREE) {
		sai = 0.55*(1.0-exp(-0.175*(canopy_strata->cs.live_stemc+canopy_strata->cs.dead_stemc)));
		canopy_strata->epv.proj_pai = max(canopy_strata->epv.proj_lai + sai, 0.0);
		canopy_strata->epv.all_pai = max(canopy_strata->epv.all_lai + sai, 0.0);
	}
	else {
		canopy_strata->epv.proj_pai =  canopy_strata->epv.proj_lai;
		canopy_strata->epv.all_pai =  canopy_strata->epv.all_lai;
	}

	/*--------------------------------------------------------------*/
	/*	initializae turnovers and litterfall 			*/
	/*--------------------------------------------------------------*/
	if (compute_annual_turnover(canopy_strata->defaults->epc,
		&(canopy_strata->epv),
		&(canopy_strata->cs)) ){
		fprintf(stderr,"FATAL ERROR: in compute_annual_turnover() ... Exiting\n");
		exit(1);
	}

	if (compute_annual_litfall(canopy_strata->defaults->epc,
		&(canopy_strata->phen),
		&(canopy_strata->cs), command_line->grow_flag) ){
		fprintf(stderr,"FATAL ERROR: in compute_annual_litfall() ... Exiting\n");
		exit(1);
	}


	/*--------------------------------------------------------------*/
	/*	compute new rooting depth based on current root carbon  */
	/* 	for static non-grow version use the worldfile rooting depth */
	/*	as read in above					*/
	/*--------------------------------------------------------------*/
	if (( command_line->grow_flag != 0) )  {
		rootc = canopy_strata->cs.frootc+canopy_strata->cs.live_crootc+canopy_strata->cs.dead_crootc;
		if (rootc > ZERO){
			if (update_rooting_depth(
				&(canopy_strata->rootzone), 
				rootc, 
				canopy_strata->defaults->epc.root_growth_direction, 
				canopy_strata->defaults->epc.root_distrib_parm,
				soil_depth)){   //double soil_depth replaced patch[i].soil_defaults->effective_soil_depth
				fprintf(stderr,
					"FATAL ERROR: in compute_rooting_depth() from construct_canopy_strata()\n");
				exit(0);
				}
		}
	}


	/*--------------------------------------------------------------*/
	/*	initialize leaf out for non-grow version		*/
	/*--------------------------------------------------------------*/
	if (( command_line->grow_flag == 0) && 
		(canopy_strata->defaults->epc.veg_type != NON_VEG) ){
		/*
		canopy_strata->cs.leafc_transfer = canopy_strata->phen.leaflitfallc;
		canopy_strata->ns.leafn_transfer = canopy_strata->phen.leaflitfallc
			* canopy_strata->ns.leafn
			/ canopy_strata->cs.leafc;
		canopy_strata->cs.leafc_store = 0.0;
		canopy_strata->ns.leafn_store = 0.0;
		*/
	}
	
	/*------------------------------------------------------------------------------------------*/
	/*	set phenology timing if static allocation and initialize for dynamic runs				*/
	/*-------------------------------------------------------------------------------------------*/
		canopy_strata->phen.expand_startday = -999;
		canopy_strata->phen.expand_stopday  = -999;
		canopy_strata->phen.litfall_startday =-999;
		canopy_strata->phen.litfall_stopday  = 300;
		if (canopy_strata->phen.expand_stopday > 365)
			canopy_strata->phen.expand_stopday -= 365;
		if (canopy_strata->phen.litfall_stopday > 365)
			canopy_strata->phen.litfall_stopday -= 365;
		/*---------------------------------------------------------------*/
		/* assume this is 365 for now since we don't know when next      */
		/* year's growing season will start                              */
		/*---------------------------------------------------------------*/
		canopy_strata->phen.nretdays = 365;
		canopy_strata->phen.gwseasonday = -1;
		canopy_strata->phen.lfseasonday = -1;

	/*--------------------------------------------------------------*/
	/*	set critical soil moisture (at stomatal closure)	*/
	/*      psi_close is converted to m water tension from MPa using     */
	/*      1m water tension = 10000 Pa                             */
	/*                      = 0.01 Mpa                              */
	/*--------------------------------------------------------------*/
	canopy_strata->epv.crit_vwc =  pow(
		((-1.0 * 100.0 * canopy_strata->defaults->epc.psi_close)
		 /air_entry),   // air_entry replaced pore_size patch[i].soil_defaults->psi_air_entry
		pore_size );  // pore_size replated patch[i].soil_defaults->pore_size_index
	

	canopy_strata->cs.num_resprout  = 0;
	canopy_strata->cs.age           = 0;
	canopy_strata->epv.wstress_days = 0;
	canopy_strata->epv.max_fparabs  = 0.;
	canopy_strata->epv.min_vwc      = 1.;
	canopy_strata->rain_stored      = 0.;
	canopy_strata->snow_stored      = 0.;
	canopy_strata->cs.gresp_transfer= 0.; //added by guoping for initialization
	canopy_strata->cs.gresp_store   = 0.; //added by guoping for initialization
	//	printf("sla_sunlit is %12.8f %12.8f \n",canopy_strata->epv.proj_sla_sunlit,canopy_strata->epv.proj_sla_shade);
	//getchar();
	return;
} /*end construct_canopy_strata->c*/
