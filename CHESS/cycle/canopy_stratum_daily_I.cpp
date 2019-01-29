//-------------------------------------------------------------------------------------------------------------------------------
//		canopy_stratum.I				
//								
// added seasonal leafon/off adjustment routine  
//								
//	Sep 15 97 RAF						
//	Took out call to compute lwp predawn and set it		
//	to default at LWP_min_spring for now.			
//	Due to modification of porosity with depth.		 
//-------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
#include <iostream>
using namespace std;

void	canopy_stratum_daily_I(
							   struct	patch_object	*patch,
							   struct 	canopy_strata_object 	*stratum,
							   struct 	command_line_object	    *command_line,
							   struct 	date 			current_date)
{
    //---------------------------------------------------------------------------------------------------------------------------
	//	Local function declaration				
    //---------------------------------------------------------------------------------------------------------------------------
	double	compute_lwp_predawn(
		int,
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);
	
	int 	compute_annual_turnover(
		struct epconst_struct,
		struct epvar_struct *,
		struct cstate_struct *);
	
	int	update_rootzone_moist(
		struct patch_object	*,
		struct	rooting_zone_object	*,
		struct command_line_object *);
	
	void	update_mortality(
		struct epconst_struct,
		struct cstate_struct *,
		struct cdayflux_struct *,
		struct cdayflux_patch_struct *,
		struct nstate_struct *,
		struct ndayflux_struct *,
		struct ndayflux_patch_struct *,
		struct litter_c_object *,
		struct litter_n_object *,
		double, double);
	
	
	void	update_phenology(
		struct patch_object *,
		struct epvar_struct *,
		struct epconst_struct,
		struct phenology_struct *,
		struct cstate_struct *,
		struct cdayflux_struct *,
		struct cdayflux_patch_struct *,
		struct nstate_struct *,
		struct ndayflux_struct *,
		struct ndayflux_patch_struct *,
		struct litter_c_object *,
		struct litter_n_object *,
		struct soil_c_object *,
		struct soil_n_object *,
		struct rooting_zone_object *,
		double soil_depth,
		double,
		double,
		double,
		struct date,
		int);
	
	void	zero_stratum_daily_flux(struct cdayflux_struct *,
		struct ndayflux_struct *);
	
	
    //---------------------------------------------------------------------------------------------------------------------------
	//  Local variable definition.                                  
    //---------------------------------------------------------------------------------------------------------------------------
	struct cstate_struct *cs;
	struct nstate_struct *ns;
	double wilting_point;


    //---------------------------------------------------------------------------------------------------------------------------
	// no processing at present for non-veg types			
    //---------------------------------------------------------------------------------------------------------------------------
	if (stratum->defaults->epc.veg_type != NON_VEG) {
		//--------------------------------------------------------------
		//  zero all of the carbon daily flux variables.		
		//--------------------------------------------------------------
		zero_stratum_daily_flux(&(stratum->cdf), &(stratum->ndf));

		if ((patch->sat_deficit - patch->rz_storage - patch->unsat_storage) <= ZERO)
			stratum->rootzone.S = 1.0;
		//else if (patch->sat_deficit_z > patch->rootzone.depth)  	
		//	stratum->rootzone.S = min(patch->rz_storage  / patch->rootzone.potential_sat, 1.0);	
		//else  
		//	stratum->rootzone.S = min((patch->rz_storage + patch->rootzone.potential_sat - patch->sat_deficit)
		//		/ patch->rootzone.potential_sat, 1.0);	
		else
			stratum->rootzone.S = max(1-max(patch->sat_deficit-patch->rz_storage-patch->unsat_storage,0)/patch->soil_defaults->soil_water_cap,0);
			//stratum->rootzone.S = min((patch->rz_storage + patch->rootzone.potential_sat - min(patch->sat_deficit, patch->rootzone.potential_sat)) / patch->rootzone.potential_sat, 1.0);

		//cout<<patch->rootzone.potential_sat<<" "<<patch->soil_defaults->soil_water_cap<<endl;
		//cout << stratum->rootzone.S << endl;
		//getchar();

		//-------------------------------------------------------------------------------------------------------------
		//      Compute canopy predawn LWP        			
		//	Currently defaulted at non-stressed value.		
		//-------------------------------------------------------------------------------------------------------------
		stratum->epv.psi =	compute_lwp_predawn(
			command_line->verbose_flag,
			patch->soil_defaults->theta_psi_curve,
			patch->metv.tsoil,
			stratum->defaults->epc.psi_open,
			stratum->defaults->epc.psi_close,
			patch->soil_defaults->psi_air_entry,
			patch->soil_defaults->pore_size_index,
			patch->soil_defaults->porosity_0,
			patch->soil_defaults->porosity_decay,
			stratum->rootzone.S);

		wilting_point = exp(-1.0*log(-1.0*stratum->defaults->epc.psi_close/patch->soil_defaults->psi_air_entry) 
				* patch->soil_defaults->pore_size_index) * patch->soil_defaults->porosity_0;

		//printf(" wilting point is %f  %f\n",wilting_point,stratum->defaults->epc.psi_close);
		//getchar();
		if (stratum->rootzone.S < wilting_point) 
			stratum->epv.psi = stratum->defaults->epc.psi_close;

		//-----------------------------------------------------------------------------------------------
		//	keep track of water stress days for annual allocation   
		//-----------------------------------------------------------------------------------------------
		if ( (command_line->grow_flag > 0) && (stratum->epv.psi <= stratum->defaults->epc.psi_close )) {
			stratum->epv.wstress_days += 1;
		}
		

		//-----------------------------------------------------------------------------------------------
		//	perform plant mortality losses	(if grow flag is on)					
		//-----------------------------------------------------------------------------------------------
		if (command_line->grow_flag > 0)  {
			cs = &(stratum->cs);
			ns = &(stratum->ns);

			stratum->cs.preday_totalc = (cs->cpool + cs->cwdc
				+ cs->leafc       + cs->leafc_store      + cs->leafc_transfer
				+ cs->dead_leafc  + cs->gresp_transfer   + cs->gresp_store
				+ cs->frootc      + cs->frootc_store     + cs->frootc_transfer
				+ cs->live_stemc  + cs->livestemc_store  + cs->livestemc_transfer
				+ cs->dead_stemc  + cs->deadstemc_store  + cs->deadstemc_transfer
				+ cs->live_crootc + cs->livecrootc_store + cs->livecrootc_transfer
				+ cs->dead_crootc + cs->deadcrootc_store + cs->deadcrootc_transfer);
			//
			stratum->ns.preday_totaln = (ns->npool + ns->cwdn + ns->retransn
				+ ns->dead_leafn          + ns->leafn       + ns->leafn_store
				+ ns->leafn_transfer      + ns->frootn      + ns->frootn_store
				+ ns->frootn_transfer     + ns->live_stemn  + ns->livestemn_store
				+ ns->livestemn_transfer  + ns->dead_stemn  + ns->deadstemn_store
				+ ns->deadstemn_transfer  + ns->live_crootn + ns->livecrootn_store
				+ ns->livecrootn_transfer + ns->dead_crootn + ns->deadcrootn_store
				+ ns->deadcrootn_transfer);


			update_mortality(stratum->defaults->epc,
				&(stratum->cs),
				&(stratum->cdf),
				&(patch->cdf),
				&(stratum->ns),
				&(stratum->ndf),
				&(patch->ndf),
				&(patch->litter_cs),
				&(patch->litter_ns),
				stratum->cover_fraction,
				stratum->defaults->epc.daily_mortality_turnover);

		}

		//--------------------------------------------------------------
		//  perform seasonal leaf senescence and budding						
		//--------------------------------------------------------------
		update_phenology(patch, &(stratum->epv),
			stratum->defaults->epc,
			&(stratum->phen),
			&(stratum->cs),
			&(stratum->cdf),
			&(patch->cdf),
			&(stratum->ns),
			&(stratum->ndf),
			&(patch->ndf),
			&(patch->litter_cs),
			&(patch->litter_ns),
			&(patch->soil_cs),
			&(patch->soil_ns),
			&(stratum->rootzone),
			patch->soil_defaults->effective_soil_depth,
			stratum->cover_fraction,
			stratum->gap_fraction,
			patch->theta_noon,
			current_date,
			command_line->grow_flag);
		
		//===========================================================================================================================================
		//following is used to check nitrogen balance
		//patch->litter_ns.preday_totaln += (patch->litter_ns.leaf_to_littern+patch->litter_ns.froot_to_littern+patch->litter_ns.cwdn_to_littern);//added leaf turnover to litter N for nitrogen balance by guoping
		patch->soil_cs.totalc   = patch->soil_cs.soil1c + patch->soil_cs.soil2c + patch->soil_cs.soil3c + patch->soil_cs.soil4c;
		patch->litter_cs.totalc = patch->litter_cs.litr1c + patch->litter_cs.litr2c + patch->litter_cs.litr3c + patch->litter_cs.litr4c;
		patch->preday_soil_totalc = patch->soil_cs.totalc;
		patch->preday_litter_totalc = patch->litter_cs.totalc;
		
		patch->soil_ns.totaln   = patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n;
		patch->litter_ns.totaln = patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n;
		patch->litter_ns.preday_totaln = patch->litter_ns.totaln;
		patch->soil_ns.preday_totaln   = patch->soil_ns.totaln;


		//if ((patch->soil_ns.preday_totaln - patch->soil_ns.totaln)> 0.00000001 || (patch->soil_ns.preday_totaln - patch->soil_ns.totaln)<-0.00000001){
		//	printf("01 patch->soil_ns.preday_totaln is %12.8f %12.8f \n", patch->soil_ns.preday_totaln, patch->soil_ns.totaln);
		//	getchar();
		//
		//}
		//if ((patch->litter_ns.preday_totaln - patch->litter_ns.totaln)> 0.00000001 || (patch->litter_ns.preday_totaln - patch->litter_ns.totaln)<-0.00000001){
		//	printf("01 patch->litter_ns.preday_totaln is %12.8f %12.8f \n", patch->litter_ns.preday_totaln, patch->litter_ns.totaln);
		//	getchar();

		//}


        //---------------------------------------------------------------------------------
		// if it is the last day of litterfall, perform carbon/nitrogen	allocations						
		//---------------------------------------------------------------------------------
		if (command_line->grow_flag > 0) {
			if ( stratum->phen.annual_allocation == 1) {
				//--------------------------------------------------------------
				//	livewood and leaf turnover				                    
				//--------------------------------------------------------------
				if (compute_annual_turnover(stratum->defaults->epc,
					&(stratum->epv),
					&(stratum->cs) ) ){
					fprintf(stderr,
						"FATAL ERROR: in compute_annual_turnover() ... Exiting\n");
					exit(1);
				}
				
				//--------------------------------------------------------------
				//	zero annual (season) accumulation variables			        
				//--------------------------------------------------------------
				stratum->epv.wstress_days = 0;
				stratum->epv.max_fparabs  = 0.0;
				stratum->epv.min_vwc      = 1.0;
			} // end litterfall end of season calculations

		} // end grow flag 
	}	// end NON_VEG conditional 

	return;
} //end canopy_stratum_I.c

