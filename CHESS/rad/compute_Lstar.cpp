/*--------------------------------------------------------------*/
/* 																*/
/*		 		compute_Lstar						*/
/*																*/
/*	compute_Lstar - updates longwave balances in a patch			*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <math.h>
#include "Constants.h"
#include "CHESS.h"

void	compute_Lstar(struct	patch_object	*patch) 
{
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	double	daylength;
	double	nightlength;
	/*--------------------------------------------------------------*/
	/*	Use daylength for longwave computations.	*/
	/*--------------------------------------------------------------*/
	daylength   = patch->metv.dayl;
	nightlength = seconds_per_day - daylength;
	/*--------------------------------------------------------------*/
	/*	Compute patch level long wave radiation processes.			*/
	/*	We use yesterdays snowpack.									*/
	/*																*/
	/*	Also, we assume Ldown == Lup for layers in the patch so		*/
	/*	we only compute Lup.										*/
	/* 									*/
	/*kJ/m**2 = J/m **2/s/K **4 * (K ** 4) * sec_per_day/ day / KJ/1000 J		*/
	/*								*/
	/*	appropriate emissivities should be subsitiuted where possilbe */
	/*								*/
	/*--------------------------------------------------------------*/
	patch->Lup_soil         = ess_soil * SBC * pow( patch->metv.tsoil + 273.0 , 4.0) * seconds_per_day / 1000.0;
	patch->Lup_canopy_night = ess_veg  * SBC * pow( patch->metv.tnight + 273.0, 4.0 ) * nightlength / 1000.0;
	patch->Lup_canopy_day   = ess_veg  * SBC * pow( patch->metv.tday +	273.0, 4.0 ) * daylength / 1000.0;
	patch->Lup_canopy       = patch->Lup_canopy_day  + patch->Lup_canopy_night;

	if ( patch->snowpack.water_equivalent_depth > 0 ){
		patch->Lup_snow = ess_snow * SBC * pow( 273.0 + patch->snowpack.T ,4.0) * seconds_per_day / 1000.0;
	}
	else{
		patch->Lup_snow = 0.0;
	}

	/*--------------------------------------------------------------*/
	/*	Compute daily Lstars.										*/
	/*																*/
	/*	We consider the canopy as a homogenous slab with temp		*/
	/*	equal to the air temp of the canopy and do not distinguish	*/
	/*	between strata.												*/
	/*--------------------------------------------------------------*/
	if ( patch->snowpack.water_equivalent_depth > 0 ){
		/*--------------------------------------------------------------*/
		/*	Compute with a snow layer									*/
		/*--------------------------------------------------------------*/
		patch->Lstar_soil   = patch->Lup_snow   - patch->Lup_soil;
		patch->Lstar_snow   = patch->Lup_canopy + patch->Lup_soil - patch->Lup_snow;
		patch->Lstar_canopy = patch->Lup_snow + patch->Ldown      -  patch->Lup_canopy;
	}
	else{
		/*--------------------------------------------------------------*/
		/*	Compute without a snow layer								*/
		/*--------------------------------------------------------------*/
		patch->Lstar_soil   = patch->Lup_canopy - patch->Lup_soil;
		patch->Lstar_canopy = patch->Lup_soil + patch->Ldown -  patch->Lup_canopy;
		patch->Lstar_snow   = 0.0;
	} /*end if - else snowpack */

	//if (patch->ID == 457){
	//	printf("patch->Ldown is %12.8f %12.8f %12.8f\n", nightlength, daylength);
	//	printf("patch->Ldown is %12.8f %12.8f %12.8f\n", patch->Ldown,patch->Lup_snow,patch->Lup_canopy);
	//	printf("patch->Ldown is %12.8f \n", patch->snowpack.water_equivalent_depth);
	//	getchar();
	//}

	//if (patch->Lstar_soil < 0) patch->Lstar_soil = 0;
	//if (patch->Lstar_canopy < 0) patch->Lstar_canopy = 0;
	//if (patch->Lstar_snow < 0) patch->Lstar_snow = 0;

	//printf("patch->Ldown is %f %f %f\n", patch->Ldown,nightlength, daylength);
	//printf("patch->Ldown is %f %f %f\n", patch->Lstar_soil, patch->Lstar_canopy, patch->Lstar_snow);
	//getchar();
} /*end compute_Lstar*/
