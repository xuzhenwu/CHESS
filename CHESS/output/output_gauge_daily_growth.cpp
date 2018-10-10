/*--------------------------------------------------------------*/
/* 																*/
/*					output_growth_basin						*/
/*																*/
/*	output_growth_basin - creates output files objects.		*/
/*																*/
/*	NAME														*/
/*	output_growth_basin - outputs current contents of a basin.			*/
/*																*/
/*	SYNOPSIS													*/
/*	void	output_growth_basin(										*/
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

void	output_gauge_daily_growth(struct patch_object *patch,
	struct	date current_date,
	FILE *outfile, struct command_line_object *command_line,int cellsize)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/

	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	int p; //h,z,,c
	int  layer;
	double agpp, anpp;
	double aresp, arespm, arespg;
	double alai, apai;
	double aleafc, afrootc, awoodc;
	double aleafn, afrootn, awoodn;

	double alitrc;
	double asoilhr, asoilar;
	double asoilc, asminn, anitrate, asurfaceN;
	double alitrn, asoiln;
	double astemc, astemn;
	double adstemc, adstemn;
	double acarbon_balance, anitrogen_balance;
	double atotaln, atotalc, adenitrif, astreamflow_N;
	double anitrif, aDOC, aDON, arootdepth;
	double acrootc, adcrootc, acrootn, adcrootn;

	//struct	patch_object  *patch;
	//struct	zone_object	*zone;
	//struct hillslope_object *hillslope;
	struct  canopy_strata_object    *strata;
	double aasim_sunlight = 0., aasim_shade = 0., sun_lai = 0., shade_lai = 0.;
	/*--------------------------------------------------------------*/
	/*	Initialize Accumlating variables.								*/
	/*--------------------------------------------------------------*/
	alai = 0.0;
	apai = 0.0;
	agpp = 0.0;
	anpp = 0.0;
	aresp = 0.0;
	arespm = 0.0;
	arespg = 0.0;
	asoilhr = 0.0;
	asoilar = 0.0;

	aleafc = 0.0;
	astemc = 0.0;
	afrootc = 0.0;
	alitrc = 0.0;
	asoilc = 0.0;
	awoodc = 0.0;
	adstemc = 0.0;
	acrootc = 0.0;
	adcrootc = 0.0;

	aleafn = 0.0;
	afrootn = 0.0;
	astemn = 0.0;
	alitrn = 0.0;
	asoiln = 0.0;
	awoodn = 0.0;
	adstemn = 0.0;
	acrootn = 0.0;
	adcrootn = 0.0;
	atotalc = 0.0;

	anitrate = 0.0;
	asurfaceN = 0.0;
	asminn = 0.0;
	atotaln = 0.0;
	adenitrif = 0.0;
	anitrif = 0.0;
	aDOC = 0.0;
	aDON = 0.0;
	arootdepth = 0.0;

	acarbon_balance = 0.0;
	anitrogen_balance = 0.0;
	astreamflow_N = 0.0;




	alitrn += (patch->litter_ns.litr1n + patch->litter_ns.litr2n + patch->litter_ns.litr3n + patch->litter_ns.litr4n);
	asoiln += (patch->soil_ns.soil1n + patch->soil_ns.soil2n + patch->soil_ns.soil3n + patch->soil_ns.soil4n);
	alitrc += (patch->litter_cs.litr1c + patch->litter_cs.litr2c + patch->litter_cs.litr3c + patch->litter_cs.litr4c);
	asoilc += (patch->soil_cs.soil1c + patch->soil_cs.soil2c + patch->soil_cs.soil3c + patch->soil_cs.soil4c);


	asurfaceN += (patch->surface_NO3 + patch->surface_NH4);
	anitrate += patch->soil_ns.nitrate;
	asminn += patch->soil_ns.sminn;
	adenitrif += patch->ndf.denitrif;
	anitrif += patch->ndf.sminn_to_nitrate;

	asoilhr += (patch->cdf.litr1c_hr + patch->cdf.litr2c_hr + patch->cdf.litr4c_hr +
		patch->cdf.soil1c_hr + patch->cdf.soil2c_hr + patch->cdf.soil3c_hr + patch->cdf.soil4c_hr);
	//aasim_shade += patch->assim_shade;
	//aasim_sunlight += patch->assim_sunlight;
	//sun_lai+=patch->canopy_strata->epv.proj_lai_sunlit;
	//shade_lai+=patch->canopy_strata->epv.proj_lai_shade;

	if (command_line->routing_flag == 1) {
		//=====================================================================
		// if water is routed
		//=====================================================================
		aDON += patch->DON_loss;
		aDOC += patch->DOC_loss;
		astreamflow_N += patch->surface_leach_NO3_out + patch->subsurface_leach_NO3_out +
			patch->surface_leach_NH4_out + patch->subsurface_leach_NH4_out;
	}
	else {
		//=======================================================================
		// if water is not routed
		//=======================================================================
		aDON += patch->DON_loss;
		aDOC += patch->DOC_loss;
		astreamflow_N += patch->surface_leach_NO3_out + patch->subsurface_leach_NO3_out +
			patch->surface_leach_NH4_out + patch->subsurface_leach_NH4_out;
	}

	//Note:: all strata_cover_fraction* were deleted. need double check against original source code
	for (layer = 0; layer < patch->num_layers; layer++) {
		//for ( c=0 ; c<patch->layers[layer].count; c++ ){
		strata = patch->canopy_strata;
		agpp += strata->cdf.psn_to_cpool;
		anpp += strata->cdf.net_psn;
		aresp += (strata->cdf.total_gr + strata->cdf.total_mr);
		arespm += strata->cdf.total_mr;
		arespg += strata->cdf.total_gr;
		//aresp  += patch->PAR_diffuse_initial ;
		//arespm += strata->epv.proj_lai_shade;
		//arespg += strata->epv.proj_lai_sunlit;


		//anpp +=patch->assim_shade/100;
		//aresp += patch->assim_sunlight / 1000;
		//arespm += patch->proj_lai_sunlit_initial/1000 ;
		//arespg += patch->proj_lai_shade_initial/100;
		//asoilar += patch->leaf_day_mr_initial/100;

		//arespm  += (strata->cdf.leaf_day_mr + strata->cdf.leaf_night_mr
		//	      + strata->cdf.livestem_mr + strata->cdf.livecroot_mr  + strata->cdf.froot_mr);

		//arespg += strata->cdf.total_gr;
		//arespg  += (cdf->cpool_leaf_gr         + cdf->cpool_froot_gr       + cdf->cpool_livecroot_gr
		//          + cdf->cpool_deadcroot_gr    + cdf->cpool_livestem_gr    + cdf->cpool_deadstem_gr
		//			+ cdf->transfer_leaf_gr      + cdf->transfer_froot_gr    + cdf->transfer_livecroot_gr
		//          + cdf->transfer_deadcroot_gr + cdf->transfer_livestem_gr + cdf->transfer_deadstem_gr);
		//arespm += strata->cdf.livecroot_mr;
		//arespg += strata->cdf.froot_mr;

		asoilar += (strata->cdf.cpool_livecroot_gr + strata->cdf.cpool_deadcroot_gr
			+ strata->cdf.cpool_froot_gr + strata->cdf.transfer_froot_gr
			+ strata->cdf.transfer_livecroot_gr + strata->cdf.transfer_deadcroot_gr
			+ strata->cdf.livecroot_mr + strata->cdf.froot_mr); //need check for definition

	//aleafc   += (strata->cs.leafc + strata->cs.leafc_store + strata->cs.leafc_transfer);
	//astemc   += (strata->cs.live_stemc + strata->cs.livestemc_store + strata->cs.livestemc_transfer);
	//adstemc  += (strata->cs.dead_stemc + strata->cs.deadstemc_store + strata->cs.deadstemc_transfer);
	//afrootc  += (strata->cs.frootc + strata->cs.frootc_store + strata->cs.frootc_transfer);
	//acrootc  += (strata->cs.live_crootc + strata->cs.livecrootc_store + strata->cs.livecrootc_transfer);
	//adcrootc += (strata->cs.dead_crootc + strata->cs.deadcrootc_store + strata->cs.deadcrootc_transfer);

		aleafc += (strata->cs.leafc);
		astemc += (strata->cs.live_stemc);
		adstemc += (strata->cs.dead_stemc);
		afrootc += (strata->cs.frootc);
		acrootc += (strata->cs.live_crootc);
		adcrootc += (strata->cs.dead_crootc);

		//aleafn   += (strata->ns.leafn + strata->ns.leafn_store + strata->ns.leafn_transfer);
		//astemn   += (strata->ns.live_stemn + strata->ns.livestemn_store + strata->ns.livestemn_transfer);
		//adstemn  += (strata->ns.dead_stemn + strata->ns.deadstemn_store + strata->ns.deadstemn_transfer);
		//afrootn  += (strata->ns.frootn     + strata->ns.frootn_store + strata->ns.frootn_transfer);
		//acrootn  += (strata->ns.live_crootn + strata->ns.livecrootn_store + strata->ns.livecrootn_transfer);
		//adcrootn += (strata->ns.dead_crootn + strata->ns.deadcrootn_store + strata->ns.deadcrootn_transfer);

		aleafn += (strata->ns.leafn);
		astemn += (strata->ns.live_stemn);
		adstemn += (strata->ns.dead_stemn);
		afrootn += (strata->ns.frootn);
		acrootn += (strata->ns.live_crootn);
		adcrootn += (strata->ns.dead_crootn);
		/*
		awoodn  +=  (strata->ns.live_crootn         + strata->ns.dead_crootn
		+ strata->ns.live_stemn          + strata->ns.dead_stemn
		+ strata->ns.livecrootn_store    + strata->ns.deadcrootn_store
		+ strata->ns.livestemn_store     + strata->ns.deadstemn_store
		+ strata->ns.livecrootn_transfer + strata->ns.deadcrootn_transfer
		+ strata->ns.cwdn + strata->ns.npool);

		awoodc  +=  (strata->cs.live_crootc         + strata->cs.dead_crootc
		+ strata->cs.live_stemc          + strata->cs.dead_stemc
		+ strata->cs.livecrootc_store    + strata->cs.deadcrootc_store
		+ strata->cs.livestemc_store     + strata->cs.deadstemc_store
		+ strata->cs.livecrootc_transfer + strata->cs.deadcrootc_transfer
		+ strata->cs.cwdc + strata->cs.cpool);

		awoodn  +=  (strata->ns.dead_crootn
		+ strata->ns.dead_stemn
		+ strata->ns.deadcrootn_store
		+ strata->ns.deadstemn_store
		+ strata->ns.deadcrootn_transfer);

		awoodc  +=  ( + strata->cs.dead_crootc
		+ strata->cs.dead_stemc
		+ strata->cs.deadcrootc_store
		+ strata->cs.deadstemc_store
		+ strata->cs.deadcrootc_transfer);
		*/

		//aleafc  += strata->cs.dead_stemc;
		//astemc  += strata->cs.dead_crootc;
		//afrootc += strata->cs.deadstemc_store;

		//adstemc += (strata->cs.dead_stemc + strata->cs.deadstemc_store + strata->cs.deadstemc_transfer);

		//rootc = cs->frootc+cs->live_crootc+cs->dead_crootc;

		//aleafn  += strata->ns.dead_stemn;
		//astemn  += strata->ns.deadstemn_store;
		//afrootn += strata->ns.deadcrootn_store;
		//adstemn += (strata->ns.dead_stemn + strata->ns.deadstemn_store + strata->ns.deadstemn_transfer);

		/*
		printf("%7.2f %7.2f\n",strata->cs.live_crootc         , strata->cs.live_stemc);
		printf("%7.2f %7.2f\n",strata->cs.dead_crootc         , strata->cs.dead_stemc);
		printf("%7.2f %7.2f\n",strata->cs.livecrootc_store    , strata->cs.livestemc_store);
		printf("%7.2f %7.2f\n",strata->cs.deadcrootc_store    , strata->cs.deadstemc_store);
		printf("%7.2f %7.2f\n",strata->cs.livecrootc_transfer , strata->cs.livestemc_transfer);
		printf("%7.2f %7.2f\n",strata->cs.deadcrootc_transfer , strata->cs.deadstemc_transfer);
		printf("%7.2f %7.2f\n",strata->cs.cwdc , strata->cs.cpool);
		//getchar();
		*/

		arootdepth += strata->rootzone.depth;
		apai += strata->epv.proj_pai;
		alai += strata->epv.proj_lai;
		//}
	}

	atotaln = aleafn + astemn + adstemn + afrootn + acrootn + adcrootn + alitrn + asoiln;
	atotalc = aleafc + astemc + adstemc + afrootc + acrootc + adcrootc + alitrc + asoilc;

	fprintf(outfile, "%4.0d", current_date.year);
	fprintf(outfile, "%4.0d", current_date.month);
	fprintf(outfile, "%4.0d", current_date.day);
	fprintf(outfile, "%9.3f", alai);                        // average LAI
	fprintf(outfile, "%9.3f", apai);                        // average PAI
	fprintf(outfile, "%9.3f", agpp * 1000);                   // GPP converted from kgC/m2 to gc/m2
	fprintf(outfile, "%9.3f", anpp * 1000);                   // NPP converted from kgC/m2 to gc/m2
	//fprintf(outfile, "%9.1f", aasim_sunlight  * 1000);                   // GPP converted from kgC/m2 to gc/m2
	//fprintf(outfile, "%9.1f", aasim_shade  * 1000);                   // NPP converted from kgC/m2 to gc/m2
	fprintf(outfile, "%9.3f", aresp * 1000);                  // maintance respiration + growth respiration
	fprintf(outfile, "%9.3f", arespm * 1000);                 // maintance respiration
	//fprintf(outfile, "%9.3f", sun_lai  * 1000);                  // maintance respiration + growth respiration
	//fprintf(outfile, "%9.3f", shade_lai  * 1000);                 // maintance respiration
	fprintf(outfile, "%9.3f", arespg * 1000);                 // growth respiration
	fprintf(outfile, "%9.3f", asoilhr * 1000);                // soil heterotrophic respiration
	fprintf(outfile, "%9.3f", asoilar * 1000);                // soil autotrophic respiration
	fprintf(outfile, "%9.1f", aleafc * 1000);                 // leaf carbon
	fprintf(outfile, "%9.1f", astemc * 1000);                 // stem carbon
	fprintf(outfile, "%9.0f", adstemc * 1000);                // dead stem carbon
	fprintf(outfile, "%9.1f", afrootc * 1000);                // fine root carbon
	fprintf(outfile, "%9.1f", acrootc * 1000);                // coarse root carbon
	fprintf(outfile, "%9.0f", adcrootc * 1000);               // dead coarse root carbon
	fprintf(outfile, "%9.1f", alitrc * 1000);                 // liter carbon
	fprintf(outfile, "%9.0f", asoilc * 1000);                 // soil carbon
	fprintf(outfile, "%9.1f", atotalc * 1000);                 //total carbon:soil+litter+vegetation
	fprintf(outfile, "%9.3f", aleafn * 1000);                 // leaf nitorgen
	fprintf(outfile, "%9.3f", astemn * 1000);                 // wood stem nitrogen
	fprintf(outfile, "%9.3f", adstemn * 1000);                // dead stem nitrogen
	fprintf(outfile, "%9.3f", afrootn * 1000);                // fine root nitrogen     
	fprintf(outfile, "%9.3f", acrootn * 1000);                // coarse root nitrogen
	fprintf(outfile, "%9.3f", adcrootn * 1000);               // dead coarse root nitrogen
	fprintf(outfile, "%9.1f", alitrn * 1000);                 // liter nitrogen
	fprintf(outfile, "%9.1f", asoiln * 1000);                 // soil nitrogen
	fprintf(outfile, "%9.1f", atotaln * 1000);                 // total Nitrogen: soil+litter+vegetation
	fprintf(outfile, "%9.0f", anitrate * 1000);               // (gN/m2) soil mineral N in nitrate form */
	fprintf(outfile, "%9.3f", asminn * 1000);                 // soil mineral nitrogen (gN/m2)
	fprintf(outfile, "%9.3f", asurfaceN * 1000);              // surface no3 + surface nh4 +N_leached to stream (Unit: g/m2/day)
	
	//xu. kg
	fprintf(outfile, "%9.3f", astreamflow_N);          // surface no3 + surface nh4 +N_leached to stream (Unit: KG/m2/day)
	fprintf(outfile, "%9.3f", adenitrif * 1000);              // annual denitrification
	fprintf(outfile, "%9.3f", anitrif * 1000);                // annual nitrification
	fprintf(outfile, "%9.3f", aDOC * 1000);                   // dessolved organic carbon (unit: gC/m2/day)
	fprintf(outfile, "%9.3f", aDON * 1000);                   // dessolved organic nitrogen(unit:gN/m2/day)
	fprintf(outfile, "%9.3f", arootdepth);                    // annual root zone depth
	fprintf(outfile, "\n");

	//
	return;
} /*end output_daily_growth_basin*/
