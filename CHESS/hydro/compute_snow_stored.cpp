/*--------------------------------------------------------------*/
/* 								*/
/*		compute_snow_stored				*/
/*								*/
/*	NAME							*/
/*	compute_snow_stored - snow intercepted by stratum and 	*/
/*				updates snow falling through.	*/ 	
/*								*/
/*	SYNOPSIS						*/
/*	double	compute_snow_stored( 	double	*,		*/
/*			struct canopy_strata_object	*);	*/
/*								*/
/*	OPTIONS							*/
/*	*snow (m) - amount of snow falling on stratum.		*/
/*	*stratum  - canopy stratum object			*/
/*								*/
/*	returns:						*/
/*	snow_storage (m) - snow in canopy at end of day		*/
/*								*/
/*	DESCRIPTION						*/
/*								*/
/*	This routine computes the change in snow storage on the	*/
/*	stratum by sublimation or melt  and snowfall.  It	*/
/*	assumes that potential sublimation/melt is only limited */
/*	available Kstar and actual sublimation/melt is limited 	*/
/*	potnetial sublimation and available snow from storage 	*/
/*	and snowfall.						*/
/*								*/
/*	It assumes that if mean daily air temp > 0 the 		*/
/*	snow melts rather than sublimates and this melt is 	*/
/*	then considered throughfall of rain!! Note that for	*/
/*	convenience we have set the snowpack density to	that	*/
/*	of water.  Thgis could be fed in later.			*/
/*								*/
/*	PROGRAMMER NOTES					*/
/*								*/
/*	May 1, 1997 RAF						*/
/*	Added checks to make sure that if no part of an 	*/
/*	irradiance stream was used the stream is not updated.	*/
/*	May 16, 1997 C.Tague					*/
/*	Updates of stratum radiation moved to after calc. of	*/
/*	radiation fractions   					*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

double	compute_snow_stored(
							int	verbose_flag,
							double	Tair_mean_day,
							double	snow_density,
							double	*snow,
							double  *rain,
							struct	canopy_strata_object	*stratum,double *potential_evapotranspiration)
{
	/*--------------------------------------------------------------*/
	/*	Local function declaration.				*/
	/*--------------------------------------------------------------*/
	double	compute_potential_snow_interception(
		int,
		double,
		struct  canopy_strata_object    *);
	/*--------------------------------------------------------------*/
	/*	Local variable definition.				*/
	/*--------------------------------------------------------------*/
	double	fraction_direct_K_used;
	double	fraction_diffuse_K_used;
	double	fraction_direct_APAR_used;
	double	fraction_diffuse_APAR_used;
	double	latent_heat;
	double	potential_interception;
	double	potential_sublimation;
	double	snow_storage;
	double	storage_sublimated;
	double	potential_interception_sublimated;
	double	Kstar_used;
	double	APAR_used;
	double	throughfall;
	
	/*--------------------------------------------------------------*/
	/*	Determine if we are sublimating or melting this snow	*/
	/*	based on the daytime average temperature.		*/
	/*--------------------------------------------------------------*/
	if ( Tair_mean_day > 0 ){
		latent_heat = 335.0;
	}
	else{
		latent_heat = 2845.0;
	}
	/*--------------------------------------------------------------*/
	/*	Compute amount potentially intercepted.			*/
	/*								*/
	/*	Defined as the minimum of the amount the stratum can cat*/
	/*	and the amount that fell from the stratum (or sky) above*/
	/*--------------------------------------------------------------*/
	potential_interception = compute_potential_snow_interception(
		verbose_flag,
		*snow,
		stratum);
	stratum->snow_interception = potential_interception;
	/*--------------------------------------------------------------*/
	/*	Sublimate the snow already stored and the potential intercep*/
	/*	tion.  Note that we made use of yesterday's albedo for this	*/
	/*	computations radiation input so there may be small problems */
	/*	with the first day of snowfall.				*/
	/*								*/
	/*	mm snow / day = (Kj/m2*day + Kj/m2*day)/(Kj/kg snow)	*/
	/*					* (m3 snow/Kgsnow)	*/
	/*								*/
	/*	Note we assume 1 m3 snow = snow_density kg snow		*/
	/*--------------------------------------------------------------*/
	//snow_density=250;
	potential_sublimation = ((stratum->Kstar_direct + stratum->Kstar_diffuse)
		/ latent_heat ) * (1/snow_density);

	/*--------------------------------------------------------------*/
	/*	Compute amount of storage sublimated.			*/
	/*--------------------------------------------------------------*/
	storage_sublimated = min(potential_sublimation,stratum->preday_snow_stored);
	storage_sublimated = min(storage_sublimated, *potential_evapotranspiration);
	*potential_evapotranspiration -= storage_sublimated;

	/*--------------------------------------------------------------*/
	/*	Update amount of snow in storage after sublimation.	*/
	/*--------------------------------------------------------------*/
	stratum->snow_stored  = stratum->preday_snow_stored -  storage_sublimated;

	/*--------------------------------------------------------------*/
	/*	Update potentail sublimation.				*/
	/*--------------------------------------------------------------*/
	potential_sublimation -= storage_sublimated;
	/*--------------------------------------------------------------*/
	/*	Remove snow potentially intercepted from throughfall.	*/
	/*								*/
	/*	We will later add back what could not be actually intercepte*/
	/*--------------------------------------------------------------*/
	throughfall  = *snow - potential_interception;
	/*--------------------------------------------------------------*/
	/*	Compute amount of potential interception sublimated.	*/
	/*--------------------------------------------------------------*/
	potential_interception_sublimated  =
		min( potential_sublimation, potential_interception);
	potential_interception_sublimated = min(potential_interception_sublimated, *potential_evapotranspiration);
	*potential_evapotranspiration -= potential_interception_sublimated;

	/*--------------------------------------------------------------*/
	/*	Update the potential interception after sublimation.	*/
	/*--------------------------------------------------------------*/
	potential_interception -= potential_interception_sublimated;

	/*--------------------------------------------------------------*/
	/*	Update snow throughfall.				*/
	/*								*/
	/*	Update snow storage					*/
	/*--------------------------------------------------------------*/
	if (stratum->defaults->epc.veg_type != NON_VEG) {
		snow_storage = min( stratum->snow_stored
			+ potential_interception, stratum->epv.all_pai
			* stratum->defaults->specific_snow_capacity );
		throughfall += max(potential_interception - (snow_storage - stratum->snow_stored),0.0);
		*snow = throughfall;
	}
	else {
		snow_storage = min( stratum->snow_stored
			+ potential_interception, stratum->defaults->specific_snow_capacity );
		throughfall += max(potential_interception - (snow_storage - stratum->snow_stored),0.0);
		*snow = throughfall;
	}
	/*--------------------------------------------------------------*/
	/*	Compute amount of sublimation that happened.		*/
	/*--------------------------------------------------------------*/
	stratum->sublimation =  storage_sublimated +
		potential_interception_sublimated;

	/*--------------------------------------------------------------*/
	/*	If in fact the sublimation was melt add the amount	*/
	/*	melted to the rain throughfall.				*/
	/*--------------------------------------------------------------*/
	if ( Tair_mean_day > 0 ){
		*rain += stratum->sublimation;
		stratum->sublimation = 0.0;
	}
	/*--------------------------------------------------------------*/
	/*	Update amount of energy available for this stratum.	*/
	/*	We first get rid of longwave.				*/
	/*--------------------------------------------------------------*/
	Kstar_used =  stratum->sublimation * latent_heat * snow_density;
	
	if ( Kstar_used > 0 ){
		APAR_used = (Kstar_used / ( stratum->Kstar_direct
			+ stratum->Kstar_diffuse )) * (stratum->APAR_direct
			+ stratum->APAR_diffuse);

	}
	else{
		APAR_used = 0;
	}

	if ( stratum->Kstar_direct > 0 ){
		fraction_direct_K_used = 1.0
			/ (1 + stratum->Kstar_diffuse/stratum->Kstar_direct);
	}
	else
		fraction_direct_K_used = 0.0;
	
	if ( stratum->Kstar_diffuse > 0 ){
		fraction_diffuse_K_used = 1.0
			/ ( 1 + stratum->Kstar_direct/stratum->Kstar_diffuse);
	}
	else
		fraction_diffuse_K_used = 0.0;
	
	if ( stratum->APAR_direct > 0 ){
		fraction_direct_APAR_used = 1.0
			/ ( 1 + stratum->APAR_diffuse/stratum->APAR_direct);
	}
	else
		fraction_direct_APAR_used = 0.0;
	
	if (stratum->APAR_diffuse > 0 ){
		fraction_diffuse_APAR_used = 1.0
			/ ( 1 + stratum->APAR_direct/stratum->APAR_diffuse);
	}
	else
		fraction_diffuse_APAR_used = 0.0;

	stratum->Kstar_direct  -= Kstar_used * fraction_direct_K_used;
	stratum->Kstar_diffuse -= Kstar_used * fraction_diffuse_K_used;
	stratum->APAR_direct   -= APAR_used * fraction_direct_APAR_used;
	stratum->APAR_diffuse  -= APAR_used * fraction_diffuse_APAR_used;

	return( snow_storage);
} /*end compute_snow_stored*/
