/*--------------------------------------------------------------*/
/*                                                              */
/*		compute_growingseason_index				*/
/*                                                              */
/*  NAME                                                        */
/*		compute_growingseason_index				*/
/*                                                              */
/*                                                              */
/*  SYNOPSIS                                                    */
/*  void compute_growingseason_index(    			*/
/*			struct patch *,		*/
/*                                                              */
/*  OPTIONS                                                     */
/*                                                              */
/*                                                              */
/*  DESCRIPTION                                                 */
/*  computation of growing season index to drive phenology     */
/*  leaf onset and offset timing				*/
/*  based on Jolly et al., 2005, Global Change Biology 		*/
/*                                                              */
/*								*/
/*  PROGRAMMER NOTES                                            */
/*                                                              */
/*                                                              */
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <math.h>
#include "CHESS.h"
#include "Constants.h"
double	compute_growingseason_index(struct patch_object *patch,
				struct epconst_struct epc) 
			
	{ 
		
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	//	stratum_default_object_list[i].epc.gs_tmin = -2.0;
	//	stratum_default_object_list[i].epc.gs_tmax = 5.0;
	//	stratum_default_object_list[i].epc.gs_vpd_min = 900;
	//	stratum_default_object_list[i].epc.gs_vpd_max = 4100;
	//  stratum_default_object_list[i].epc.gs_dayl_min = 36000;
	//	stratum_default_object_list[i].epc.gs_dayl_max = 39600;	
	double gsi, itmin, ivpd, idayl;

	itmin = 0.0;
	if (patch->metv.tmin_ravg >= epc.gs_tmax)
		itmin = 1.0;
	else
		itmin = (patch->metv.tmin_ravg - epc.gs_tmin)/(epc.gs_trange); //gs_trange 7
	
	ivpd = 0.0;
	if (patch->metv.vpd_ravg <= epc.gs_vpd_min)
		ivpd = 1.0;
	else
		ivpd = 1.0 - ((patch->metv.vpd_ravg - epc.gs_vpd_min)/(epc.gs_vpd_range)); //gs_vpd_range 3300 
	
	idayl = 0.0;
	if (patch->metv.dayl_ravg >= epc.gs_dayl_max)
		idayl = 1.0;
	else
		idayl = (patch->metv.dayl_ravg - epc.gs_dayl_min)/(epc.gs_dayl_range); //3600
	
	itmin = max(itmin, 0.0);
	ivpd = max(ivpd, 0.0);
	idayl = max(idayl, 0.0);
	
	gsi = idayl*itmin*ivpd;
	if (ivpd>1. || itmin>1. || idayl>1.0){
	printf("\n itmin %lf ivpd %lf idayl %lf gsi %lf", itmin,ivpd,idayl,gsi);
	getchar();
	}

	//printf("\n tmin %lf %lf vpd %lf %lf dayl %lf %lf gsi %lf", itmin,
	//patch->metv.tmin_ravg, ivpd, patch->metv.vpd_ravg, idayl, patch->metv.dayl_ravg, gsi); 
	//getchar();
	return(gsi);
} /* end compute_growingseason_index */

