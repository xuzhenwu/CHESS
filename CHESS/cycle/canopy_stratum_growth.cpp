//=====================================================================================================================
// 								
//			canopy_stratum_growth		
//								
//	NAME							
//	canopy_statrum_daily_growth 				
//								
//								
//	SYNOPSIS						
//	void canopy_stratum_growth 			
//								
//	OPTIONS							
//								
//	DESCRIPTION						
//								
//	performs final end of day processing for the 		
//	strata including CN allocations, growth respiration	
//								
//								
//	PROGRAMMER NOTES					
//=====================================================================================================================
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
#include <iostream>
using namespace std;

void	canopy_stratum_growth(
							  struct	patch_object		*patch,
							  struct 	canopy_strata_object 	*stratum,
							  struct 	command_line_object	*command_line,
							  struct 	date 			current_date)
{
	//--------------------------------------------------------------
	//	Local function declaration				
	//--------------------------------------------------------------
	void	compute_growth_resp(	 int,
		struct epconst_struct,
		struct cdayflux_struct *);
	
	int update_C_stratum_daily(
		struct epconst_struct,
		struct cstate_struct *,
		struct cdayflux_struct *);
	
	int	update_N_stratum_daily(
		struct epconst_struct,
		struct nstate_struct *,
		struct ndayflux_struct *,
		struct soil_n_object *);
	
	int	allocate_daily_growth_Waring(
		int,
		double,
		double,
		double,
		struct cdayflux_struct *,
		struct cstate_struct *,
		struct ndayflux_struct *,
		struct nstate_struct *,
		struct ndayflux_patch_struct *,
		struct	epvar_struct *,
		struct	epconst_struct,
		struct	date);

	int	allocate_daily_growth_Dickenson(
		int,
		double,
		double,
		double,
		double,
		struct cdayflux_struct *,
		struct cstate_struct *,
		struct ndayflux_struct *,
		struct nstate_struct *,
		struct ndayflux_patch_struct *,
		struct	epvar_struct *,
		struct	epconst_struct,
		struct	date);

	int	allocate_daily_growth(
		int,
		double,
		double,
		double,
		struct cdayflux_struct *,
		struct cstate_struct *,
		struct ndayflux_struct *,
		struct nstate_struct *,
		struct ndayflux_patch_struct *,
		struct	epvar_struct *,
		struct	epconst_struct,
		struct	date);

	int	allocate_annual_growth(
		int,
		int,
		int,
		double,
		struct epvar_struct *,
		struct cdayflux_struct *,
		struct cstate_struct *,
		struct ndayflux_struct *,
		struct nstate_struct *,
		struct cdayflux_patch_struct *,
		struct ndayflux_patch_struct *,
		struct	litter_c_object *,
		struct	litter_n_object *,
		struct	epconst_struct);
	//--------------------------------------------------------------
	//  Local variable definition.                                  
	//--------------------------------------------------------------
	struct cstate_struct *cs;
	struct nstate_struct *ns;
		

	//=================================================================================================================
	//	perform daily carbon and nitrogen allocations		
	//=================================================================================================================
	switch(stratum->defaults->epc.allocation_flag) {

		case CONSTANT: // constant allocation 


		if (allocate_daily_growth(
			patch->soil_ns.nlimit,
			stratum->phen.daily_allocation * stratum->defaults->epc.storage_transfer_prop,
			patch->soil_cs.frootc,
			stratum->cover_fraction,
			&(stratum->cdf),
			&(stratum->cs),
			&(stratum->ndf),
			&(stratum->ns),
			&(patch->ndf),
			&(stratum->epv),
			stratum->defaults->epc,
			current_date) != 0){
			fprintf(stderr,"FATAL ERROR: in allocate_daily_growth");
			exit(1);
			}
		/*
		if (current_date.year == 1977 && current_date.month == 11 && current_date.day >= 4 && patch->ID==5540){
		cout << stratum->cdf.psn_to_cpool <<" "<<patch->ID<< endl;
		cout << stratum->epv.proj_lai_sunlit <<" "<< endl;
		cout << stratum->epv.proj_lai_shade << " " <<  endl;
		cout << stratum->cdf.leaf_day_mr << " " << patch->metv.dayl << endl;
		cout << stratum->cdf.leaf_day_mr << " " << patch->metv.dayl << endl;
		cout << current_date.month << " 2 " <<current_date.day << endl;
		getchar();
		}
		*/

		break;
		


		case WARING:  // Waring allocation 

		if (allocate_daily_growth_Waring(
			patch->soil_ns.nlimit,
			stratum->phen.daily_allocation * stratum->defaults->epc.storage_transfer_prop,
			patch->soil_cs.frootc,
			stratum->cover_fraction,
			&(stratum->cdf),
			&(stratum->cs),
			&(stratum->ndf),
			&(stratum->ns),
			&(patch->ndf),
			&(stratum->epv),
			stratum->defaults->epc,
			current_date) != 0){
			fprintf(stderr,"FATAL ERROR: in allocate_daily_growth");
			exit(1);
			}

		break;

		case DICKENSON:  // Dickenson allocation 

		if (allocate_daily_growth_Dickenson(
			patch->soil_ns.nlimit,
			1,
			stratum->phen.daily_allocation * stratum->defaults->epc.storage_transfer_prop,
			patch->soil_cs.frootc,
			stratum->cover_fraction,
			&(stratum->cdf),
			&(stratum->cs),
			&(stratum->ndf),
			&(stratum->ns),
			&(patch->ndf),
			&(stratum->epv),
			stratum->defaults->epc,
			current_date) != 0){
			fprintf(stderr,"FATAL ERROR: in allocate_daily_growth");
			exit(1);
			}

		break;

		case STATIC:
		break;

	} // end switch 


	//=================================================================================================================
	//	compute growth respiration (if grow option is on_)	
	//=================================================================================================================
	compute_growth_resp(stratum->phen.annual_allocation,
		stratum->defaults->epc,
		&(stratum->cdf)	) ;


	//=================================================================================================================
	//	allocate annual growth - once per year that will then be expressed during leaf out				
	//=================================================================================================================
	if ( (stratum->phen.annual_allocation == 1) ){
		if (allocate_annual_growth(
			stratum->ID,
			stratum->defaults->ID,
			command_line->vmort_flag,
			stratum->cover_fraction,
			&(stratum->epv),
			&(stratum->cdf),
			&(stratum->cs),
			&(stratum->ndf),
			&(stratum->ns),
			&(patch->cdf),
			&(patch->ndf),
			&(patch->litter_cs),
			&(patch->litter_ns),
			stratum->defaults->epc) != 0){
			fprintf(stderr,"FATAL ERROR: in allocate_annual_growth");
			exit(1);
		}

	}

	//=================================================================================================================
	//	update carbon state variables 				
	//=================================================================================================================
	if (update_C_stratum_daily(stratum->defaults->epc,	&(stratum->cs),	&(stratum->cdf))!= 0){
		fprintf(stderr,"FATAL ERROR: in update_C_stratum_daily");
		exit(1);
	}

	//=================================================================================================================
	//	update nitrogen state variables 				
	//=================================================================================================================
	if (update_N_stratum_daily(stratum->defaults->epc,	&(stratum->ns),	&(stratum->ndf), &(patch->soil_ns))!= 0){
		fprintf(stderr,"FATAL ERROR: in update_N_stratum_daily");
		exit(1);
	}


	if (stratum->phen.annual_allocation == 1){
		stratum->cdf.leafc_store_to_leafc_transfer = stratum->cs.leafc_store;
		stratum->cs.leafc_transfer                += stratum->cdf.leafc_store_to_leafc_transfer;
		stratum->cs.leafc_store                   -= stratum->cdf.leafc_store_to_leafc_transfer;
		stratum->ndf.leafn_store_to_leafn_transfer = stratum->ns.leafn_store;
		stratum->ns.leafn_transfer                += stratum->ndf.leafn_store_to_leafn_transfer;
		stratum->ns.leafn_store                   -= stratum->ndf.leafn_store_to_leafn_transfer;
	}


	cs = &(stratum->cs);
	stratum->cs.totalc = (cs->cpool + cs->cwdc + cs->dead_leafc
		+ cs->leafc + cs->leafc_store +  cs->leafc_transfer
		+ cs->gresp_transfer + cs->gresp_store
		+ cs->frootc + cs->frootc_store +  cs->frootc_transfer
		+ cs->live_stemc + cs->livestemc_store +  cs->livestemc_transfer
		+ cs->dead_stemc + cs->deadstemc_store +  cs->deadstemc_transfer
		+ cs->live_crootc + cs->livecrootc_store +  cs->livecrootc_transfer
		+ cs->dead_crootc + cs->deadcrootc_store +  cs->deadcrootc_transfer);

	ns = &(stratum->ns);
	stratum->ns.totaln = (ns->npool + ns->cwdn + ns->retransn + ns->dead_leafn
		+ ns->leafn + ns->leafn_store +  ns->leafn_transfer
		+ ns->frootn + ns->frootn_store +  ns->frootn_transfer
		+ ns->live_stemn + ns->livestemn_store +  ns->livestemn_transfer
		+ ns->dead_stemn + ns->deadstemn_store +  ns->deadstemn_transfer
		+ ns->live_crootn + ns->livecrootn_store +  ns->livecrootn_transfer
		+ ns->dead_crootn + ns->deadcrootn_store +  ns->deadcrootn_transfer);
	return;
} //end canopy_stratum_daily_growth.c
