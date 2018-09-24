//=====================================================================================================================
//	compute_growth_resp					
//								
//	NAME							
//		compute_growth_resp				
//								
//	SYNOPSIS						
//	double	compute_growth_resp(				
//								
//	returns:						
//								
//	OPTIONS							
//								
//	DESCRIPTION						
//								
//								
//	PROGRAMMER NOTES					
//								
//		from Peter Thornton, 1997 - 1d_bgc		
//=====================================================================================================================

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "CHESS.h"
#include "Constants.h"

void compute_growth_resp(int alloc, struct epconst_struct epc,
						struct cdayflux_struct* cdf)
{
	//------------------------------------------------------
	//	Local Variable Definition. 							
	//------------------------------------------------------
	//double g1;   // RATIO   C respired for growth : C grown  
	//g1 = epc.gr_perc;

	// leaf and fine root growth respiration for both trees and grass 
	cdf->cpool_leaf_gr            = cdf->cpool_to_leafc * epc.gr_perc;
	cdf->cpool_froot_gr           = cdf->cpool_to_frootc * epc.gr_perc;
	cdf->cpool_to_leafc_store_gr  = cdf->cpool_to_leafc_store*epc.gr_perc;
	cdf->cpool_to_frootc_store_gr = cdf->cpool_to_frootc_store*epc.gr_perc;

	cdf->transfer_leaf_gr  = cdf->leafc_transfer_to_leafc * epc.gr_perc;
	cdf->transfer_froot_gr = cdf->frootc_transfer_to_frootc * epc.gr_perc;


	cdf->total_gr = (cdf->cpool_leaf_gr + cdf->cpool_froot_gr + cdf->cpool_to_leafc_store_gr + cdf->cpool_to_frootc_store_gr +
		           cdf->transfer_leaf_gr + cdf->transfer_froot_gr);
	//printf("epc.gr_perc is %f \n", cdf->cpool_to_leafc, epc.gr_perc);
	//getchar();

	// woody tissue growth respiration only for trees 
	if (epc.veg_type == TREE){
		cdf->cpool_livestem_gr     = cdf->cpool_to_livestemc * epc.gr_perc;
		cdf->cpool_deadstem_gr     = cdf->cpool_to_deadstemc * epc.gr_perc;
		cdf->cpool_livecroot_gr    = cdf->cpool_to_livecrootc * epc.gr_perc;
		cdf->cpool_deadcroot_gr    = cdf->cpool_to_deadcrootc * epc.gr_perc;
		
		cdf->cpool_livestem_storage_gr = cdf->cpool_to_livestemc_store * epc.gr_perc;
		cdf->cpool_deadstem_storage_gr = cdf->cpool_to_deadstemc_store * epc.gr_perc;
		cdf->cpool_livecroot_storage_gr = cdf->cpool_to_livecrootc_store * epc.gr_perc;
		cdf->cpool_deadcroot_storage_gr = cdf->cpool_to_deadcrootc_store * epc.gr_perc;

		cdf->transfer_livestem_gr = cdf->livestemc_transfer_to_livestemc * epc.gr_perc;
		cdf->transfer_deadstem_gr = cdf->deadstemc_transfer_to_deadstemc * epc.gr_perc;
		cdf->transfer_livecroot_gr = cdf->livecrootc_transfer_to_livecrootc * epc.gr_perc;
		cdf->transfer_deadcroot_gr = cdf->deadcrootc_transfer_to_deadcrootc * epc.gr_perc;

	}
	else{
		cdf->cpool_livestem_gr = 0.;
		cdf->cpool_deadstem_gr = 0.;
		cdf->cpool_livecroot_gr = 0.;
		cdf->cpool_deadcroot_gr = 0.;

		cdf->cpool_livestem_storage_gr = 0.;
		cdf->cpool_deadstem_storage_gr = 0.;
		cdf->cpool_livecroot_storage_gr = 0.;
		cdf->cpool_deadcroot_storage_gr = 0.;

		cdf->transfer_livestem_gr = 0.;
		cdf->transfer_deadstem_gr = 0.;
		cdf->transfer_livecroot_gr = 0.;
		cdf->transfer_deadcroot_gr = 0.;
	}
	cdf->total_gr += (cdf->cpool_livestem_gr + cdf->cpool_deadstem_gr
		+ cdf->cpool_livecroot_gr + cdf->cpool_deadcroot_gr
		+ cdf->transfer_livecroot_gr + cdf->transfer_deadcroot_gr
		+ cdf->transfer_livestem_gr + cdf->transfer_deadstem_gr
		+ cdf->cpool_livestem_storage_gr + cdf->cpool_deadstem_storage_gr + cdf->cpool_livecroot_storage_gr+cdf->cpool_deadcroot_storage_gr);

	return ;
}// end compute_growth_resp.c 
