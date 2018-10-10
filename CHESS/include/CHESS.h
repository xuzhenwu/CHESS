//===============================================================================================================================
//						hess.h								
//																
//	Description: this header file is based on RHESSys model but with great modifications
//  Created by Dr. Guoping Tang														
//===============================================================================================================================
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Constants.h"

//===============================================================================================================================
//	Define input climate files							
//===============================================================================================================================
struct In_File_Path
{
	char  prefix[20];
	char  inDefFile[120];
	char  inImgFile[120];
	char  inFlowFile[120];
	char  inClimPath[120];
	char  outPutPath[120];
	char  FlowTableName[30];
};

struct Input_Grid_Data
{
	long rows;
	long cols;
	double xll;
	double yll;
	double cellsize;
	double no_data_value;
};

struct Input_Data_Range
{
	long start_year;
	long end_year;
	long start_month;
	long end_month;
	long start_day;
	long end_day;
	long start_hour;
	long end_hour;
};

//===============================================================================================================================
//	Define a calendar out date object.					
//===============================================================================================================================
struct out_date_range	{
	long  first_year;
	long  last_year;
	long  first_month;
	long  last_month;
	long  first_day;
	long  last_day;
	long  first_hour;
	long  last_hour;
};

struct Spin_Up
{
	long spin_years;
	long spin_interval;
	long spin_flag;
};

//===============================================================================================================================
//	Define input climate files							
//===============================================================================================================================
struct input_Clim_Files
{
	FILE * pPrec;
	FILE * pTmax;
	FILE * pTmin;
	FILE * pCO2;
};

//===============================================================================================================================
//	Define a calendar date object.						
//===============================================================================================================================
struct date
{
	long year;
	long month;
	long day;
	long hour;
};

//===============================================================================================================================
//	Define output data files	 						  
//===============================================================================================================================
struct output_hydro_plant
{
	FILE *fBasinDailyHydro;
	FILE *fBasinDailyPlant;
	FILE *fPatchDailyHydro;
	FILE *fPatchDailyPlant;
	FILE *fGaugeDailyHydro[gauge_num];
	FILE *fGaugeDailyPlant[gauge_num];
};


//===============================================================================================================================
//	rooting zone object for patch		
//===============================================================================================================================
struct rooting_zone_object
{
	double field_capacity;
	double potential_sat;
	double depth;
	double S;
};

//===============================================================================================================================
//	Define base station daily climate record .				
//===============================================================================================================================
struct	daily_clim_object
{
	double	tmax;			/*   degrees C	*/
	double	tmin;			/*   degrees C	*/
	double	rain;			/*   mm	 water	*/

	//===============================================================================================================================
	// 	 Non - Critical data.							
	//===============================================================================================================================
	double	atm_trans;		            /*	0 - 1		*/
	double	CO2;			            /*  ppm */
	double	cloud_fraction;	            /*	0 - 1		*/
	double	cloud_opacity;		        /*	0 - 1		*/
	double	dayl;			            /*  seconds / day */
	double	daytime_rain_duration;	    /*  hours/day	*/
	double	Delta_T;		            /*	degrees C / day		*/
	double	Kdown_diffuse;			    /*  kJ/(m2*day)	*/
	double	Kdown_direct;			    /*  kJ/(m2*day) */
	double	Ldown;				        /*  kJ/(m2*day)	*/
	double	ndep_NO3;			        /*  kgN/(m2*day) */
	double	ndep_NH4;			        /*  kgN/(m2*day) */
	double	PAR_diffuse;		        /*	molm-2day-1		*/
	double	PAR_direct;			        /*	molm-2day-1		*/
	double	relative_humidity;	        /*	0 - 1 ; input 0 - 100	*/
	double	snow;				        /*	mm	*/
	double	tdewpoint;			        /*  degrees C	*/
	double	tday;				        /*	degrees C	*/
	double	tnight;			            /*	degrees C	*/
	double  tnightmax;			        /* 	degrees C 	*/
	double	tavg;				        /*	degrees C	*/
	double	tsoil;				        /* 	degrees C	*/
	double	vpd;				        /* 	Pa		*/
	double	wind;				        /*	m/s		*/
};


//===============================================================================================================================
//	Define a deep groundwater object		
//===============================================================================================================================
struct 	gw_object
{
	double	storage;		/* m (water) */
	double 	NO3;		    /* kgN/m2	*/
	double	Qout;			/* m/m2/day	*/
	double	Nout;			/* kgN/m2/day	*/
};

//===============================================================================================================================
// daily values that are passed to daily model subroutines 
//===============================================================================================================================
struct metvar_struct
{
	//double abshd;          /* (kg/m3) absolute humidity deficit */
	double dayl;           /* (s)     daylength */
	double dayl_ravg;      /* (s) 21 day running average of daylength */
	double pa;             /* (Pa)    atmospheric pressure */
	//double ppfd;           /* (umol/m2/s) PAR photon flux density */
	//double rootup_temp;    /* temperature multiplier for root N uptake */
	//double swabs;          /* (W/m2)  canopy absorbed shortwave flux */
	//double swtrans;        /* (W/m2)  transmitted shortwave flux */
	//double temp_mult;      /* (DIM) runningsavg temperature for decomp (NEW) */
	double tmax;           /* (deg C) daily maximum air temperature */
	double tmin;           /* (deg C) daily minimum air temperature */
	double tmin_ravg;      /* (def C) 21 day running avg of daily min air temperature */
	double tavg;           /* (deg C) daily average air temperature */
	double tday;           /* (deg C) daylight average air temperature */
	double tnight;         /* (deg C) nightime average air temperature */
	double tnightmax;      /* (deg C) nighttime maximum air temperature */
	double tsoil;          /* (deg C) daily soil temperature, avg, top 10 cm */
	double tsoil_avg;      /* avg soil temp over the branching period */
	double tsoil_sum;      /* (deg C) daily summation of soil temp     (NEW) */
	double tnight_max;     /* (deg C) nighttime max average air temperature */
	double vpd;            /* (Pa)    vapor pressure deficit */
	double vpd_ravg;				/* (Pa) 21 day running average of vpd */
};


struct soil_class
{
	double sand;	/* 0 to 1 */
	double silt;	/* 0 to 1 */
	double clay;	/* 0 to 1 */
};

//===============================================================================================================================
//	Define a land use 	default object.				
//===============================================================================================================================
struct	landuse_default
{
	int ID;
	double  fertilizer_NO3;			    /* kg/m2/day	*/
	double  fertilizer_NH4;				/* kg/m2/day	*/
	double  irrigation;					/* m/day	*/
	double  septic_NO3_load;			/* kg/m2/day */
	double  septic_water_load;			/* m/m2/day */
	double	detention_store_size;		/* m */
	double	lai_cut;				    /* m2/m2 */
	double	percent_impervious;			/* 0-1 */
};

//===============================================================================================================================
//	Define an soil 	default object.				
//===============================================================================================================================
struct	soil_default
{
	char    Soil_Name[50][50];           /*soil class name*/
	int		ID;
	int	    theta_psi_curve;		     /* unitless */
	double	albedo;						 /* 0 to 1   */
	double	Ksat_0;						 /* meters/day */
	double	m_z;						 /* m^-1	*/
	double	porosity_0;					 /* unitless */
	double	porosity_decay;				 /* m^-1 */
	double	p3;						     /* unitless */
	double	pore_size_index;			 /* unitless */
	double	psi_air_entry;				 /* m */
	double	psi_max;					 /* m */
	double	sat_to_gw_coeff;			 /* percent/day */
	double	soil_depth;					 /* m */
	double	effective_soil_depth;		 /* m */
	double	soil_water_cap;				 /* m of water */
	double	deltaz;						 /* m */
	double	min_heat_capacity;			 /* J/m3/K */
	double	max_heat_capacity;			 /* J/m3/K */
	double	detention_store_size;		 /* m water */
	double  active_zone_z;				 /* m */
	double  daily_fire_litter_turnover;	 /* (DIM) 0-1 */
	double  N_decay_rate;				 /* kg N /m */
	double  mobile_N_proportion;		 /* (DIM) 0-1 */
	double  denitrif_proportion;		 /* (DIM) 0-1 */
	double	DON_loss_rate;				 /* (DIM) 0-1 */
	double	gl_c;						 /* m/s */
	double	gsurf_slope;				 /* (DIM) */
	double  gsurf_intercept;			 /* m/s */
	struct  soil_class	soil_type;
};


//===============================================================================================================================
//	Define a neighbours object.					
//===============================================================================================================================
struct	neighbour_object
{
	double gamma;		/* m**2 / day */
	struct	patch_object *patch;
};

//===============================================================================================================================
//	Define litter  and soil cn flux objects			 
//===============================================================================================================================
struct	cdayflux_patch_struct
{
	/* dissolved organic fluxes */
	double do_litr1c_loss;	/* (kgC/m2/day) labile DOC loss */
	double do_litr2c_loss;	/* (kgC/m2/day) cellulose DOC loss */
	double do_litr3c_loss;	/* (kgC/m2/day) shielded cellulose DOC loss */
	double do_litr4c_loss;	/* (kgC/m2/day) lignan DOC loss */

	double do_soil1c_loss;	/* (kgC/m2/day) fast soil carbon DOC loss */
	double do_soil2c_loss;	/* (kgC/m2/day) medium soil DOC loss */
	double do_soil3c_loss;	/* (kgC/m2/day) slow DOC loss */
	double do_soil4c_loss;	/* (kgC/m2/day) recalcitrant DOC loss */

	double total_DOC_loss;	/* (kgN/m2/day)	*/

	/* potential decomp fluxes */
	double plitr1c_loss;	/* (kgC/m2/d) microbial assim. of labile lit */
	double plitr2c_loss;	/* (kgC/m2/d) microbial assim. of cellulose lit */
	double plitr4c_loss;	/* (kgC/m2/d) microbial assim. of lignin lit */
	double psoil1c_loss;	/* (kgC/m2/d) fast microbial recycling */
	double psoil2c_loss;	/* (kgC/m2/d) slow microbial recycling */
	double psoil3c_loss;	/* (kgC/m2/d) release of shielded cellulose */
	double psoil4c_loss;	/* (kgC/m2/d) recalcitrant SOM formation */
	double kl4;			    /* (1/day) rate constant for lignin litter decomp */

	/* daily turnover fluxes */
	double leafc_to_litr1c;  /* (kgC/m2/d) leaf litfall (labile) */
	double leafc_to_litr2c;  /* (kgC/m2/d) leaf litfall (cell., unshielded) */
	double leafc_to_litr3c;  /* (kgC/m2/d) leaf litfall (cell., shielded) */
	double leafc_to_litr4c;  /* (kgC/m2/d) leaf litfall (lignin) */
	double frootc_to_litr1c; /* (kgC/m2/d) froot litfall (labile) */
	double frootc_to_litr2c; /* (kgC/m2/d) froot litfall (cell., unshielded) */
	double frootc_to_litr3c; /* (kgC/m2/d) froot litfall (cell., shielded) */
	double frootc_to_litr4c; /* (kgC/m2/d) froot litfall (lignin) */
	double litr1c_to_soil1c; /* (kgC/m2/d) microbial assim. of labile lit */
	double litr2c_to_soil2c; /* (kgC/m2/d) microbial assim. of cellulose lit */
	double litr3c_to_litr2c; /* (kgC/m2/d) release of shielded cellulose */
	double litr4c_to_soil3c; /* (kgC/m2/d) microbial assim. of lignin lit */
	double soil1c_to_soil2c; /* (kgC/m2/d) fast microbial recycling */
	double soil2c_to_soil3c; /* (kgC/m2/d) slow microbial recycling */
	double soil3c_to_soil4c; /* (kgC/m2/d) recalcitrant SOM formation */
	double cwdc_to_litr2c;   /* (kgC/m2/d) CWD to unshielded cellulose litter */
	double cwdc_to_litr3c;   /* (kgC/m2/d) CWD to shielded cellulose litter */
	double cwdc_to_litr4c;   /* (kgC/m2/d) CWD to lignin litter */

	/* daily heterotroph respiration fluxes */
	double litr1c_hr;         /* (kgC/m2/d) labile litter respiration */
	double litr2c_hr;         /* (kgC/m2/d) unshielded cellulose litter resp */
	double litr4c_hr;         /* (kgC/m2/d) lignin litter respiration */
	double soil1c_hr;         /* (kgC/m2/d) fast microbial recycling resp */
	double soil2c_hr;         /* (kgC/m2/d) medium microbial recycling resp */
	double soil3c_hr;         /* (kgC/m2/d) slow microbial recycling resp */
	double soil4c_hr;         /* (kgC/m2/d) recalcitrant SOM respiration */

	/* daily MORTALITY fluxes */
	double m_leafc_to_litr1c;       /* (kgC/m2/d) leaf to labile */
	double m_leafc_to_litr2c;       /* (kgC/m2/d) leaf to unshielded cell. */
	double m_leafc_to_litr3c;       /* (kgC/m2/d) leaf to shielded cell. */
	double m_leafc_to_litr4c;       /* (kgC/m2/d) leaf to lignin */
	double m_frootc_to_litr1c;      /* (kgC/m2/d) froot to labile */
	double m_frootc_to_litr2c;      /* (kgC/m2/d) froot to un shielded cell. */
	double m_frootc_to_litr3c;      /* (kgC/m2/d) froot to shielded cell. */
	double m_frootc_to_litr4c;      /* (kgC/m2/d) froot to lignin */

	double m_leafc_store_to_litr1c;       /* (kgC/m2/d) leaf store to labile */
	double m_frootc_store_to_litr1c;      /* (kgC/m2/d) froot store to labile */
	double m_livestemc_store_to_litr1c;   /* (kgC/m2/d) live stem store to labile */
	double m_deadstemc_store_to_litr1c;   /* (kgC/m2/d) dead stem store to labile */
	double m_livecrootc_store_to_litr1c;  /* (kgC/m2/d) live coarse root store to labile */
	double m_deadcrootc_store_to_litr1c;  /* (kgC/m2/d) dead coarse root store to labile */

	double m_leafc_transfer_to_litr1c;      /* (kgC/m2/d) leaf transfer to labile */
	double m_frootc_transfer_to_litr1c;     /* (kgC/m2/d) froot transfer to labile */
	double m_livestemc_transfer_to_litr1c;  /* (kgC/m2/d) live stem transfer to labile */
	double m_deadstemc_transfer_to_litr1c;  /* (kgC/m2/d) dead stem transfer to labile */
	double m_livecrootc_transfer_to_litr1c; /* (kgC/m2/d) live coarse root transfer to labile */
	double m_deadcrootc_transfer_to_litr1c; /* (kgC/m2/d) dead coarse root transfer to labile */

	double m_gresp_store_to_litr1c;         /* (kgC/m2/d) */
	double m_gresp_transfer_to_litr1c;      /* (kgC/m2/d) */
};

struct	ndayflux_patch_struct
{
	/* dissolved organic fluxes */
	double do_litr1n_loss;	   /* (kgN/m2/day) labile DON loss */
	double do_litr2n_loss;	   /* (kgN/m2/day) cellulose DON loss */
	double do_litr3n_loss;	   /* (kgN/m2/day) shielded cellulose DON loss */
	double do_litr4n_loss;	   /* (kgN/m2/day) lignan DON loss */

	double do_soil1n_loss;	   /* (kgN/m2/day) fast soil DON loss */
	double do_soil2n_loss;	   /* (kgN/m2/day) medium soil DON loss */
	double do_soil3n_loss;	   /* (kgN/m2/day) slow DON loss */
	double do_soil4n_loss;	   /* (kgN/m2/day) recalcitrant DON loss */

	double total_DON_loss;	   /* (kgN/m2/day)	*/

	/* potential decomp fluxes */
	double mineralized;		   /* (kgN/m2/d) total mineralized N */
	double potential_immob;	   /* (kgN/m2/d) potential imobilized N */
	double net_mineralized;    /* (kgN/m2/d) net mineralization */

	/* potential mineralization */
	double pmnf_l1s1;		   /* (kgN/m2/d) microbial assim. of labile lit */
	double pmnf_l2s2;		   /* (kgN/m2/d) microbial assim. of cellulose lit */
	double pmnf_l4s3;		   /* (kgN/m2/d) microbial assim. of lignin lit */
	double pmnf_s1s2;		   /* (kgN/m2/d) fast microbial recycling */
	double pmnf_s2s3;		   /* (kgN/m2/d) slow microbial recycling */
	double pmnf_s3s4;		   /* (kgN/m2/d) recalcitrant SOM formation */
	double pmnf_s4;		       /* (kgN/m2/d) recalcitrant SOM decay */

	/* daily turnover fluxes */
	double leafn_to_litr1n;    /* (kgN/m2/d) leaf litfall labile */
	double leafn_to_litr2n;    /* (kgN/m2/d) leaf litfall unshielded cellulose */
	double leafn_to_litr3n;    /* (kgN/m2/d) leaf litfall shielded cellulose */
	double leafn_to_litr4n;    /* (kgN/m2/d) leaf litfall lignin */
	double frootn_to_litr1n;   /* (kgN/m2/d) froot litfall labile */
	double frootn_to_litr2n;   /* (kgN/m2/d) froot litfall unshielded cellulose */
	double frootn_to_litr3n;   /* (kgN/m2/d) froot litfall shielded cellulose */
	double frootn_to_litr4n;   /* (kgN/m2/d) froot litfall lignin */
	double litr1n_to_soil1n;   /* (kgN/m2/d) microbial assim. of labile N */
	double litr2n_to_soil2n;   /* (kgN/m2/d) microbial assim. of cellulose N */
	double litr4n_to_soil3n;   /* (kgN/m2/d) microbial assim. of lignin N */
	double litr3n_to_litr2n;   /* (kgN/m2/d) release of shielded cellulose N */
	double soil1n_to_soil2n;   /* (kgN/m2/d) fast microbial recycling */
	double soil2n_to_soil3n;   /* (kgN/m2/d) slow microbial recycling */
	double soil3n_to_soil4n;   /* (kgN/m2/d) formation of recalcitrant SOM N */
	double cwdn_to_litr2n;     /* (kgN/m2/d) CWD to unshielded cellulose N */
	double cwdn_to_litr3n;     /* (kgN/m2/d) CWD to shielded cellulose N */
	double cwdn_to_litr4n;     /* (kgN/m2/d) CWD to lignin N */

	/* daily N immobilization & mineralization fluxes */
	double plant_potential_ndemand;          /* (kgN/m2/d) potential N demand from plants	      */
	double plant_avail_uptake;               /* (kgN/m2/d) available N uptake from plants	      */
	double sminn_to_soil1n_l1;               /* (kgN/m2/d) N immob. between litr1 and soil1 */
	double sminn_to_soil2n_l2;               /* (kgN/m2/d) N immob. between litr2 and soil2 */
	double sminn_to_soil3n_l4;               /* (kgN/m2/d) N immob. between litr4 and soil3 */
	double sminn_to_soil2n_s1;               /* (kgN/m2/d) N immob. between soil1 and soil2 */
	double sminn_to_soil3n_s2;               /* (kgN/m2/d) N immob. between soil2 and soil3 */
	double sminn_to_soil4n_s3;               /* (kgN/m2/d) N immob. between soil3 and soil4 */
	double sminn_to_npool;                   /* (kgN/m2/d) N mineralized to plant pool uptake */
	double preday_sminn_to_npool;
	double soil4n_to_sminn;                  /* (kgN/m2/d) N from soil4 to mineral pool */
	double sminn_to_nitrate;                 /* (kgN/m2/d) nitrification  */

	/* daily N MORTALITY fluxes */
	double m_leafn_to_litr1n;                /* (kgN/m2/d) leaf to labile */
	double m_leafn_to_litr2n;                /* (kgN/m2/d) leaf to unshielded cell. */
	double m_leafn_to_litr3n;                /* (kgN/m2/d) leaf to shielded cell. */
	double m_leafn_to_litr4n;                /* (kgN/m2/d) leaf to lignin */
	double m_frootn_to_litr1n;               /* (kgN/m2/d) froot to labile */
	double m_frootn_to_litr2n;               /* (kgN/m2/d) froot to un shielded cell. */
	double m_frootn_to_litr3n;               /* (kgN/m2/d) froot to shielded cell. */
	double m_frootn_to_litr4n;               /* (kgN/m2/d) froot to lignin */

	double m_livestemn_to_litr1n;            /*(kgN/m2/d) live stem to labile */
	double m_livecrootn_to_litr1n;           /*(kgN/m2/d) live coarse root to labile */
	double m_leafn_store_to_litr1n;          /* (kgN/m2/d) leaf store to labile */
	double m_frootn_store_to_litr1n;         /* (kgN/m2/d) froot store to labile */
	double m_livestemn_store_to_litr1n;      /* (kgN/m2/d) live stem store to labile */
	double m_deadstemn_store_to_litr1n;      /* (kgN/m2/d) dead stem store to labile */
	double m_livecrootn_store_to_litr1n;     /* (kgN/m2/d) live noarse root store to labile */
	double m_deadcrootn_store_to_litr1n;     /* (kgN/m2/d) dead noarse root store to labile */

	double m_leafn_transfer_to_litr1n;       /* (kgN/m2/d) leaf transfer to labile */
	double m_frootn_transfer_to_litr1n;      /* (kgN/m2/d) froot transfer to labile */
	double m_livestemn_transfer_to_litr1n;   /* (kgN/m2/d) live stem transfer to labile */
	double m_deadstemn_transfer_to_litr1n;   /* (kgN/m2/d) dead stem transfer to labile */
	double m_livecrootn_transfer_to_litr1n;  /* (kgN/m2/d) live coarse root transfer to labile */

	double m_deadcrootn_transfer_to_litr1n;  /* (kgN/m2/d) dead coarse root transfer to labile */
	double m_retransn_to_litr1n;             /* (kgN/m2/d) */

	/* denitrification  (volatilization) */
	double sminn_to_nvol;		     /* (kgN/ms/d) */
	double denitrif;			     /* (kgN/m2/day) */

	/* daily N sources and sinks */
	double nfix_to_sminn;            /* (kgN/m2/d) bio-fixation to soil min N pool */
	double ndep_to_sminn;            /* (kgN/m2/d) deposition to soil min N pool */
	//double sminn_leached;            /* (kgN/m2/d) loss due to leaching  to satsubsurface throughflow */
	double N_to_gw;	                 /* (kgN/m2/day) loss due to leaching to gw */
	double plant_Nuptake;  //added by guoping
};

//===============================================================================================================================
//	Define a litter objects 		
//===============================================================================================================================
struct  litter_object
{
	double preday_rain_stored;
	double rain_stored;
	double gsurf;
	double proj_pai;
	double rain_capacity;
	double gl_c;						/* m/s */
	double gsurf_slope;				/* (DIM) */
	double gsurf_intercept;			/* m/s */
	double moist_coef;					/* m/kg */
};

struct	litter_c_object
{
	double litr1c;            /* (kgC/m2) litter labile C */
	double litr2c;            /* (kgC/m2) litter unshielded cellulose C */
	double litr3c;            /* (kgC/m2) litter shielded cellulose C */
	double litr4c;            /* (kgC/m2) litter lignin C */
	double totalc;            /* (kgC/m2) litter  C */
	double litr1c_hr_snk;     /* (kgC/m2) SUM of labile litr microbial resp */
	double litr2c_hr_snk;     /* (kgC/m2) SUM of cellulose litr microbial resp */
	double litr4c_hr_snk;     /* (kgC/m2) SUM of lignin litr microbial resp */
	double fire_snk;	      /* (kgC/m2) SUM of losses due to fire */
	double t_scalar;	      /* (DIM) tempertaure scalar for decomposition */
	double w_scalar;	      /* (DIM) tempertaure scalar for decomposition */
};

struct	litter_n_object
{
	double litr1n;            /* (kgN/m2) litter labile N */
	double litr2n;            /* (kgN/m2) litter unshielded cellulose N */
	double litr3n;            /* (kgN/m2) litter shielded cellulose N */
	double litr4n;            /* (kgN/m2) litter lignin N */
	double totaln;            /* (kgN/m2) litter N */
	double preday_totaln;
	double fire_snk;	      /* (kgC/m2) SUM of losses due to fire */
	double leaf_to_littern;
	double froot_to_littern;
	double cwdn_to_littern;
};

//===============================================================================================================================
//	Define a soil 	objects 			
//===============================================================================================================================
struct	soil_c_object
{
	double frootc;	          /* (kgC/m2) total soil fine root C   	*/
	double totalc;	          /* (kgC/m2) total soil  C   	*/
	double soil_cpool;	      /* (kgC/m2) temporary soil carbon pool	*/
	double soil1c;            /* (kgC/m2) microbial recycling pool C (fast) */
	double soil2c;            /* (kgC/m2) microbial recycling pool C (medium) */
	double soil3c;            /* (kgC/m2) microbial recycling pool C (slow) */
	double soil4c;            /* (kgC/m2) recalcitrant SOM C (humus, slowest) */
	double soil1c_hr_snk;     /* (kgC/m2) SUM of fast microbial respiration */
	double soil2c_hr_snk;     /* (kgC/m2) SUM of medium microbial respiration */
	double soil3c_hr_snk;     /* (kgC/m2) SUM of slow microbial respiration */
	double soil4c_hr_snk;     /* (kgC/m2) SUM of recalcitrant SOM respiration */
};

struct	soil_n_object
{
	int	nlimit;				                    /* DIM (0 or 1) */
	double fract_potential_immob;				/* DIM (0-1) */
	double fract_potential_uptake;				/* DIM (0-1) */
	double totaln;	                            /* (kgN/m2) total soil  N   	*/
	double preday_totaln;
	double soil_npool;	                        /* (kgN/m2) temporary soil nitrogen pool	*/
	double soil1n;                              /* (kgN/m2) microbial recycling pool N (fast) */
	double soil2n;                              /* (kgN/m2) microbial recycling pool N (medium) */
	double soil3n;                              /* (kgN/m2) microbial recycling pool N (slow) */
	double soil4n;                              /* (kgN/m2) recalcitrant SOM N (humus, slowest) */
	double sminn;                               /* (kgN/m2) soil mineral N */
	double nitrate;                             /* (kgN/m2) soil mineral N in nitrate form */
	double preday_sminn;
	double preday_nitrate;
	//double Qin;                                 /* (kgN/m2) soil mineral N output */
	// double Qout;                                /* (kgN/m2) soil mineral N input */
	double leach;                               /* (kgN/m2) soil mineral N input */
	double nfix_src;                            /* (kgN/m2) SUM of biological N fixation */
	double ndep_src;                            /* (kgN/m2) SUM of N deposition inputs */
	double nvolatilized_snk;                    /* (kgN/m2) SUM of N lost to volatilization */
};

//===============================================================================================================================
//	Define a snowpack object.					
//===============================================================================================================================
struct	snowpack_object
{
	double  APAR_direct;                   	/* umol/(m2*day)*/
	double  APAR_diffuse;                   /* umol(m2*day) */
	double	energy_deficit;			        /* degree days	*/
	double	height;				            /* meters   	*/
	double  Kstar_direct;                   /* Kj/(m2*day)  */
	double  Kstar_diffuse;                  /* Kj/(m2*day)  */
	double	K_reflectance;			        /* unitless 0-1	*/
	double	K_absorptance;			        /* unitless 0-1 */
	double	PAR_reflectance;		        /* unitless 0-1	*/
	double	PAR_absorptance;		        /* unitless 0-1 */
	double	surface_age;			        /* days		*/
	double	sublimation;			        /* m water	*/
	double  T; 		                        /*  degrees C  	*/
	double	water_depth;			        /* m water	*/
	double  water_equivalent_depth;	        /* m water   	*/
	double	overstory_fraction;		        /* percent	*/
	double	overstory_height;		        /* m		*/
};


//===============================================================================================================================
//Define an patch default object by combing basin, hillslop, zone and land-use default object Edited by guoping
//===============================================================================================================================
struct patch_default
{
	//int		n_routing_timesteps;	            /* number per day */ 
	double  latitude;                           //averaged latitude for the whole watershed
	double  mean_elev;                          //averaged elevation for the whole watershed
	double  std;
	double  cover_fraction;
	double  gap_fraction;
	double  root_depth;
	double  screen_height;                      //meters
	double	gw_loss_coeff;				        /* percent/day */
	double	atm_trans_lapse_rate;	            /* 1/m	*/
	double	dewpoint_lapse_rate;	            /* 	degrees C / m	 	*/
	double	lapse_rate;		                    /* Celcius degrees/m	*/
	double	wet_lapse_rate;		                /* Celcius degrees/m	*/
	double	lapse_rate_tmin;		            /* Celcius degrees/m	*/
	double	lapse_rate_tmax;		            /* Celcius degrees/m	*/
	double  lapse_rate_prec;                    /* mm/m */
	double	pptmin;			                    /*	m	*/
	double	sea_level_clear_sky_trans;	        /* 0-1	*/
	double	temcf;			                    /* 	DIM	*/
	double	trans_coeff1;		                /* 	DIM	*/
	double	trans_coeff2;		                /* 	DIM	*/
	double	wind;			                    /* m/s		*/
	double  max_snow_temp;                      /* degrees C */
	double  min_rain_temp;                      /* degrees C */
	double  ndep_NO3;		                    /* kgN/m2/day	*/
	double	maximum_snow_energy_deficit; /* degree days */
	double  snow_water_capacity;		 /* m */
	double  snow_light_ext_coef;		 /* (DIM) radiation extinction */
	double  snow_melt_Tcoef;			 /* unitless */
	struct  snowpack_object snowpack;
};

//===============================================================================================================================
//	Define an patch object	 									
//===============================================================================================================================
struct patch_object
{
	int		year_day;                          //from world object
	int		ID;
	int		num_innundation_depths;
	int		num_canopy_strata;
	int		num_layers;
	double	x;									/* meters	*/
	double	y;									/* meters	*/
	double	z;									/* meters	*/
	double  acc_area;                             /*accumulated area*/
	double  mean_z;
	double  critical_depth;		                /* m */
	double	gamma;
	int	    num_neighbours;
	double  assim_sunlight;
	double  assim_shade;
	//patch related input data
	float   slope;                              //added by guoping
	float   aspect;                             // added by guoping
	int     stream;
	int     vegtype;
	int     soiltype;
	int     road;

	//xu.
	int		streamorder;
	int		basins;

	int   gsi_based_leafon_startday;
	int   gsi_based_litterfall_stopday;
	//patch fields from world_object
	double	declin;			                   /* rads	*/
	double	cos_declin;		                   /* DIM	*/
	double	sin_declin;		                   /* DIM	*/
	double	Io;			                       /* Wm-2 	*/

	//patch fields from basin object
	double	cos_latitude;		               /* DIM	*/
	double	daylength;		                   /* s	*/
	double	latitude;		                   /* decimal degrees	*/
	double	theta_noon;		                   /* rads	*/
	double	sin_latitude;		               /* DIM	*/
	double	screen_height;			           /* meters	*/
	int 	daylength_flag;			           /* 0 or 1 */
	int	   	Kdown_diffuse_flag;                /* 0 or 1  */
	int   	Kdown_direct_flag;                 /* 0 or 1  */

	double  atm_trans;                    	   /* 0 - 1    */
	double  cloud;                    	       /* 0 - 1    	*/
	double  cloud_fraction;                    /* 0 - 1    	*/
	double  cloud_opacity;                     /* 0 - 1   */
	double	CO2;						       /* ppm	*/
	double	cos_aspect;				           /* DIM	*/
	double	cos_slope;				           /* DIM	*/
	double	daytime_rain_duration;			   /* hours/day	*/
	double  Delta_T;                           /* C degrees    */
	double	e_dewpoint;				           /* Pa		*/
	double	e_horizon; 	                       /* cos of angle to normal of flat  	*/
	double  Kdown_diffuse_adjustment;          /* 0-1 */
	double	Kdown_diffuse_flat;			       /* Kj/(m^2*day)	*/
	double  Kdown_direct_adjustment;           /* 0-1 */
	double	Kdown_direct_flat;			       /* Kj/(m^2*day)	*/
	double	LAI_temp_adjustment;			   /* 0 - 1	*/
	double  Ldown;                             /* W/m2 	*/
	double  ndep_NO3;				           /* kg/m2/day	*/
	double  ndep_NH4;				           /* kg/m2/day	*/
	double  preday_NO3;//for nitrogen balance
	double  preday_NH4; ///FOR nitrogen balance
	double	radrat;					           /* ratio	*/
	double	rain;				               /* m water 	*/
	double  relative_humidity;    	           /* 0 - 1   	*/
	double	sin_aspect;				           /* DIM	*/
	double	sin_slope;				           /* DIM	*/
	double	snow;				               /* m water	*/
	double	tdewpoint;				           /* degrees C	*/
	//	double	vpd_max;				           /* Pa		*/
	double	w_horizon;	                       /* cos of angle to normal of flat	*/

	/////////////////////////////////////////////////////////////////////////
	double	cap_rise;		            /* m water / day */
	//double	delta_rain_stored;	        /* m water	*/
	//double	delta_snow_stored;	        /* m water	*/
	double  detention_store;	        /* m water	*/
	double	evaporation;		        /* m  water*/
	double	evaporation_surf_litter;	        /* m  water*/
	double  evaporation_surf_soil;
	double  ga;			                /* m/s */
	double  ga_final;		            /* m/s */
	double  gw_drainage; 		        /* m/day */
	//double  interim_sat;		        /* m */
	//double  stream_gamma;		        /* meters**2/day	*/
	double	Kdown_direct;		        /* Kj/(m^2*day)	*/
	double	Kdown_diffuse;		        /* Kj/(m^2*day)	*/
	double	Kdown_direct_final;	        /* Kj/(m^2*day)	*/
	double	Kdown_diffuse_final;	    /* Kj/(m^2*day)	*/
	//double	Ksat_0;			            /* meteres/day  */
	double  Ksat_vertical;		        /* meters/day	*/
	//	double	lna;			            /* unitless	*/
	double  lai;			            /* unitless	*/
	double	Lup_canopy;		            /* Kj/(m^2*day)	*/
	double	Lup_canopy_day;		        /* Kj/(m^2*day)	*/
	double	Lup_canopy_night;	        /* Kj/(m^2*day)	*/
	//	double	Lup_surface;		        /* Kj/(m^2*day)	*/
	//	double	Lup_surface_day;	        /* Kj/(m^2*day)	*/
	//	double	Lup_surface_night;	        /* Kj/(m^2*day)	*/
	double	Lup_snow;		            /* Kj/(m^2*day)	*/
	double	Lup_soil;		            /* Kj/(m^2*day)	*/
	double	Lstar_canopy;		        /* Kj/(m^2*day)	*/
	//	double	Lstar_canopy_day;	        /* Kj/(m^2*day)	*/
	//	double	Lstar_canopy_night;	        /* Kj/(m^2*day)	*/
	//	double	Lstar_surface;		        /* Kj/(m^2*day)	*/
	//	double	Lstar_surface_day;	        /* Kj/(m^2*day)	*/
	//	double	Lstar_surface_night;	    /* Kj/(m^2*day)	*/
	double	Lstar_snow;		            /* Kj/(m^2*day)	*/
	double	Lstar_soil;		            /* Kj/(m^2*day)	*/
	double	PAR_direct;		            /* umol/(m^2*day)	*/
	double	PAR_diffuse;		        /* umol/(m^2*day)	*/
	double  PAR_diffuse_initial;
	double  PAR_direct_initial;
	double	PAR_direct_final;	        /* umol/(m^2*day)	*/
	double	PAR_diffuse_final;	        /* umol(m^2*day)	*/
	double	potential_cap_rise;	        /* m water/ day */
	double	potential_exfiltration;	    /* m water/ day */
	double	potential_evapotranspiration;	    /* m water/ day */

	double	psi;			            /* MPa		*/
	double  subsurface_Qin;	            /* m /day 	*/
	double  subsurface_Qout;	            /* m /day 	*/
	//	double  preday_Qout;	            

	double  subsurface_leach_NO3_out;  /* kg/m2/day 	*/
	double  surface_leach_NO3_out;     /* kg/m2/day 	*/
	double  subsurface_leach_NH4_out;   /* kg/m2/day 	*/
	double  surface_leach_NH4_out;      /* kg/m2/day 	*/

	double	road_cut_depth;		        /* m */
	double	rain_throughfall;	        /* m water	*/
	//double	return_flow;		        /* m water	*/
	double	snow_throughfall;	        /* m water	*/
	double	rain_throughfall_final;	    /* m water	*/
	double	rain_stored;		        /* m water	*/

	double	S;			                /* m/m		*/
	//	double	sat_zone_storage;	        /* m water 	*/
	double	snow_redist_scale;	        /* multiplier	*/
	double	std;			            /* m water	*/
	double  surface_NO3;		        /* kg/m2	*/
	double  surface_NH4;		        /* kg/m2	*/
	double  fertilizer_NO3;		        /* kg/m2	*/
	double  fertilizer_NH4;		        /* kg/m2	*/

	double  surface_Qin;		        /* m day	*/
	double  surface_Qout;		        /* m day	*/
	double  infiltration_excess;        /* m water      */
	double	snow_throughfall_final;	    /* m water	*/
	double	snow_melt;		            /* m water	*/
	double	snow_stored;		        /* m water	*/
	double	surface_heat_flux;	        /* kJ		*/
	double	transpiration_sat_zone;	    /* m water	*/
	double	transpiration_unsat_zone;	/* m water	*/
	double	exfiltration_sat_zone;	    /* m water	*/
	double	exfiltration_unsat_zone;    /* m water	*/
	double	unsat_drainage;		        /* m water	*/
	double	AET;		                /* m water	*/
	double	PET;		                 /* m water	*/ //only used once guoping
	double	rz_drainage;		        /* m water by Taehee Hwang */
	double  wind;			            /* m/s		*/
	double  wind_final;		            /* m/s		*/

	double  DON_loss;
	double  DOC_loss;
	double  NO3_leached;
	double  preday_NO3_leached;

	struct  gw_object gw;
	struct	soil_default		    *soil_defaults;
	struct	landuse_default		    *landuse_defaults;
	struct	canopy_strata_object    *canopy_strata;
	struct	patch_hourly_object	    *hourly;
	struct	layer_object		    *layers;
	struct	neighbour_object 	    *neighbours;
	struct	patch_object		    *next_stream;
	struct  rooting_zone_object	    rootzone;
	//xu.
	struct  channel_object  *channel;
	struct  channel_default  *channel_defaults;


	//===============================================================================================================================
	//	Surface Hydrology  stuff			
	//===============================================================================================================================
	int   	drainage_type;				/* unitless 1 stream, 0 land, 2, road */
	double	water_balance;				/* meters water		*/
	double	delta_snowpack;				/* meters		*/
	//	double	delta_canopy_storage;		/* meters water		*/
	//	double	deltaS;					    /* meters water		*/
	//	double	evap_potential;				/* Joules		*/
	double	field_capacity;				/* meters water 	*/
	double	preday_snow_stored;			/* meters water		*/
	double  preday_detention_store;		/* meters water		*/
	double	preday_rain_stored;			/* meters water		*/
	double	preday_snowpack;			/* meters water		*/  //only used once guoping ??
	double	preday_sat_deficit;			/* meters water		*/
	double	preday_sat_deficit_z;		/* meters		*/
	double  preday_subsurface_out;
	double	sat_deficit;				/* meters water		*/
	double	sat_deficit_z;				/* meters		*/
	struct	snowpack_object	snowpack;	/* meters		*/
	double	preday_unsat_storage;		/* meters water		*/
	double  preday_rz_storage;			/* meters water by Taehee Hwang */
	double	unsat_storage;				/* meters water		*/
	double  rz_storage;				    /* meters water by Taehee Hwang */
	double	unsat_zone_volume;			/* meters water		*/  //only used once guoping

	//===============================================================================================================================
	//	Forest floor stuff				    
	//===============================================================================================================================
	//  double  surface_Tday;			    /* deg C		*/
	//  double  surface_Tnight;			    /* deg C		*/
	//  double	soil_height;			    /* m relative to surface */
	double	gsurf;				        /* m/s		*/
	double	stability_correction; 		/* DIM */
	//double	Tsoil;				    /* degrees C	*/

	//===============================================================================================================================
	//	carbon and nitrogen objects for patch soil and litter 
	//===============================================================================================================================
	double  net_plant_psn;			    /* kgC/m2 net carbon flux into patch */
	double  preday_totalc;			    /* kgC/m2 previous day total carbon */
	double  totalc;				        /* kgC/m2 total carbon */
	double  carbon_balance;			    /* kgC/m2 */
	double  preday_soil_totalc;
	double  preday_litter_totalc;
	double  heterol_respiration;
	double  heteros_respiration;
	double  preday_totaln;			    /* kgC/m2 total nitrogen */
	double  totaln;				        /* kgC/m2 total nitrogen */
	double	satzone_nitrate;		    /* kgN/m2 saturated zone */

	struct	soil_c_object	soil_cs;
	struct	soil_n_object	soil_ns;
	struct	litter_object	litter;
	struct  litter_c_object	litter_cs;
	struct  litter_n_object	litter_ns;
	struct  cdayflux_patch_struct	cdf;
	struct  ndayflux_patch_struct	ndf;
	struct	metvar_struct		metv;              //from zone_object
	//	struct	dated_input_object	 *dated_input;     //from base_station object
	struct	daily_clim_object 	 *daily_clim;      //from base_station object
	struct  patch_default *patch_defaults;         //new definition by guoping
	//double  expand_flag;
	//double  litfall_flag;

	double detention_store_preday;
	double rz_storage_preday;
	double unsat_storage_preday;
	double sat_deficit_preday;

	double tmin21[21];
	double vpd21[21];
	double dayl21[21];

	int  leaf_on_flag;
	int  leaf_expand_flag;
	int  litfall_stop_flag;
	int  preyear_litfall_stopday;
	double proj_lai_sunlit_initial;
	double proj_lai_shade_initial;
	double leaf_day_mr_initial;
	double gpp_initial;
};

//xu.start
//===============================================================================================================================
//	Define an channel default	 									
//===============================================================================================================================
struct channel_default{
	int strahler_order;
	double channel_width;
	double hydraulic_roughness;
};
//===============================================================================================================================
//	Define an channel object	 									
//===============================================================================================================================
struct channel_object{
	
	double surface_storage;
	double subsurface_storage;
	
	double h;

	double surface_Q_in;
	double subsurface_Q_in;
	double surface_Q_out;
	double subsurface_Q_out;

	double k;
	double Rr;//hydrualic radius
	double channel_slope;
	double channel_length;
	double hydraulic_roughness;
	double channel_width;
	int strahler_order;
};
//xu. end


//===============================================================================================================================
//	Define the layer object structure.			
//===============================================================================================================================
struct	layer_object
{
	int	    count;
	long 	*strata;
	double	height;		/* m */
	double  base;		/* m */
	double  null_cover;
};


//===============================================================================================================================
//	Define the patch hourly parameter structure.			
//===============================================================================================================================
struct	patch_hourly_object
{
	int	i;
	double	rain_throughfall;
	double	infiltration;
	double	cos_declin_cos_hourangle;	/*	DIM	*/
	double 	cos_declin_sin_hourangle;	/*	DIM	*/
	double	cos_hour_angle;			    /*	DIM	*/
	double	cos_sza;			        /*	DIM	*/
	double	hour_angle;			        /* rads		*/
	double	sin_hour_angle;			    /*	DIM	*/
	double	optical_air_mass;		    /*	DIM	*/
	double	rain;					    /*	m	*/
	double	rain_duration;			    /* 	s	*/
	double	cos_aspect;				    /*	DIM	*/
	double	cos_beam_slope;			    /*	DIM	*/
	double	direct_attenuation;		    /*	0 - 1	*/
	double	Kdown_BOA;				    /*	W/m^2	*/
	double	Kdown_direct;			    /*	W/m^2	*/
	double	Kdown_direct_flat;		    /*	W/m^2	*/
	double	Kdown_diffuse;			    /*	W/m^2	*/
	double	Kdown_diffuse_flat;		    /*	W/m^2	*/
	double	sin_aspect;				    /*	DIM	*/
	double	sin_slope;				    /*	DIM	*/
};

//===============================================================================================================================
// 	Define a command_line_object object.				
//===============================================================================================================================
struct	command_line_object
{
	bool    b;
	bool    p;
	
	//xu. gg for gauges' outputs in the gauge patchID list
	bool	gg;

	int     spin_flag;
	int		grow_flag;
	int		routing_flag;
	double	sat_to_gw_coeff_mult;
	double	gw_loss_coeff_mult;
	int		road_flag;
	int		verbose_flag;
	int		tec_flag;
	int		gw_flag;
	int		vmort_flag;
};

//===============================================================================================================================
// 	Define carbon and nitrogen related struct.				
//===============================================================================================================================
struct phenology_struct
{
	double leaflitfallc;         /* (kgC/m2) current growth year leaflitter carbon */
	double leaflitfallc_wstress; /* (kgC/m2) accumulated leaf drop due to water stress for that year */
	double leaflitfalln;         /* (kgN/m2) current growth year leaflitter nitrogen */
	double leafretransn;         /* (kgN/m2) current growth year leaf N retranslocation */
	double frootlitfallc;        /* (kgC/m2) current growth year leaflitter carbon */
	double frootlitfalln;        /* (kgN/m2) current growth year leaflitter nitrogen */
	double daily_allocation;     /* (DIM) signal to allocate when set to 1 */
	double gsi;		             /* (0 to 1) growing season phenology index */
	int annual_allocation;       /* (DIM) signal to allocate when set to 1 */
	int expand_startday;         /* (yday) yearday of first leaf growth */
	int litfall_startday;        /* (yday) yearday of litterfall growth */
	int expand_stopday;          /* (yday) yearday of last leaf growth */
	int litfall_stopday;         /* (yday) yearday of last litterfall growth */
	int ngrowthdays;             /* (days) days between onday and next offday */
	int nretdays;                /* (days) days between allocations */
	int gwseasonday;             /* (day) day within the growing season */
	int lfseasonday;             /* (day) day within litter fall period */
};


// carbon state variables (including sums for sources and sinks) 
struct cstate_struct
{
	int	   age;                    /* (num years) */
	int    num_resprout;        /* (num years) running index of years of resprouting */
	double preday_totalc;       /* (kgC/m2) previous days plant carbon total */
	double totalc;  	        /* (kgC/m2) previous days plant carbon total */
	//double net_psn;	         /* (kgC/m2) net photosynthesis (psn-respiration) */
	double cpool;               /* (kgC/m2) temporary plant C pool */
	double availc;              /* (kgC/m2) plant C from photosynthesis available for growth*/

	double leafc;               /* (kgC/m2) leaf C */
	double dead_leafc;          /* (kgC/m2) standing dead leaf C for grasses */
	double live_stemc;          /* (kgC/m2) live stem C */
	double dead_stemc;          /* (kgC/m2) dead stem C */
	double live_crootc;         /* (kgC/m2) live coarse root C */
	double dead_crootc;         /* (kgC/m2) dead coarse root C */
	double frootc;              /* (kgC/m2) fine root C */

	double leafc_transfer; 	    /* (kgC/m2) leaf C to be allocated from last season */
	double livestemc_transfer;  /* (kgC/m2) live stemwood C to be allocated from last season */
	double deadstemc_transfer;  /* (kgC/m2) dead stemwood C to be allocated from last season */
	double livecrootc_transfer; /* (kgC/m2) live coarse root C to be allocated from last season */
	double deadcrootc_transfer; /* (kgC/m2) dead coarse root C to be allocated from last season */
	double frootc_transfer; 	/* (kgC/m2) fine root C to be allocated from last season */
	double gresp_transfer;      /* (kgC/m2) growth respiration C to be allocated from last season*/

	double leafc_store;         /* (kgC/m2) leaf C stored from this year's growth */
	double livestemc_store;     /* (kgC/m2) live stemwood C stored from this years growth */
	double deadstemc_store;     /* (kgC/m2) dead stemwood C  stored from this years growth*/
	double livecrootc_store;    /* (kgC/m2) live coarse root C  stored from this years growth*/
	double deadcrootc_store;    /* (kgC/m2) dead coarse root C  stored from this years growth*/
	double frootc_store;        /* (kgC/m2) fine root C  stored from this years growth */

	double gresp_store;         /* (kgC/m2) growth respiration C stored from this years growth */
	double cwdc;	            /* (kgC/m2) coarse woody debris C*/

	/* sink for respiration and fire losses */
	double gpsn_src;            /* (kgC/m2) SUM of gross PSN carbon inputs */
	double fire_snk;            /* (kgC/m2) SUM of carbon loss due to fire */
	double leaf_mr_snk;         /* (kgC/m2) SUM of leaf maint resp */
	double leaf_gr_snk;         /* (kgC/m2) SUM of leaf growth resp */
	double froot_mr_snk;        /* (kgC/m2) SUM of fine root maint resp */
	double froot_gr_snk;        /* (kgC/m2) SUM of fine root growth resp */
	double livestem_mr_snk;     /* (kgC/m2) SUM of live stem maint resp */
	double livestem_gr_snk;     /* (kgC/m2) SUM of live stem growth resp */
	double deadstem_gr_snk;     /* (kgC/m2) SUM of dead stem growth resp */
	double livecroot_mr_snk;    /* (kgC/m2) SUM of live coarse root maint resp */
	double livecroot_gr_snk;    /* (kgC/m2) SUM of live coarse root growth resp */
	double deadcroot_gr_snk;    /* (kgC/m2) SUM of dead coarse root growth resp */

	double DayMaxLeafc;
	double DayMaxFrootc;
	double PreDayMaxLeafc;
	double PreDayMaxFrootc;

};

// maintenance respiration constants 
struct	mrconst_struct
{
	double q10;            /* (DIM) Q10 for maintenance respiration */
	double per_N;          /* (kgC/kgN/day)  maintenance resp per kg Nitrogen ie. kgC/kgN/day */
	// double sapw_max;       /* (kgC/m2) maximum sapwood */
};


// special input and output structures for the photosynthesis routine 
struct	psnin_struct
{
	int	c3;		            /* C3/C4 plant flag	*/
	double pa;              /* (Pa) atmospheric pressure */
	double co2;             /* (ppm) atmospheric [CO2] */
	double t;               /* (deg C) temperature */
	double irad;            /* (umol/m2/s) shortwave irradiance */
	double g;               /* (m * 10**3/s) conductance to CO2 */
	double Rd;              /* (umol/m2/s) dark respiration rate */
	double lnc;             /* (kg Nleaf/m2) leaf nitrogen per unit area */
	double flnr;            /* (kg NRub/kg Nleaf) fract. of leaf N in Rubisco */
};

struct	psnout_struct
{
	double g;               /* (umol/m2/s/Pa) conductance to CO2 */
	double O2;              /* (Pa) atmospheric [O2] */
	double Ca;              /* (Pa) atmospheric [CO2] */
	double Ci;              /* (Pa) intercellular [CO2] */
	double gamma;           /* (Pa) CO2 compensation point, no Rd */
	double Kc;              /* (Pa) MM constant carboxylation */
	double Ko;              /* (Pa) MM constant oxygenation */
	double act;             /* (umol/kg/s) Rubisco activity */
	double Vmax;            /* (umol/m2/s) max rate carboxylation */
	double Jmax;            /* (umol/m2/s) max rate electron transport */
	double J;               /* (umol/m2/s) rate of RuBP regeneration */
	double Av;              /* (umol/m2/s) carboxylation limited assimilation */
	double Aj;              /* (umol/m2/s) RuBP regen limited assimilation */
	double A;               /* (umol/m2/s) final assimilation rate */
	double dC13;		    /* % discrimination of C13 */
};


/* ecophysiological variables */
struct epvar_struct
{
	double height;					/* meters	*/
	double all_pai;                 /* (DIM) all-sided plant area index */
	double proj_pai;                /* (DIM) projected plant area index */
	double all_lai;                 /* (DIM) live all-sided leaf area index */
	double proj_lai;                /* (DIM) live projected leaf area index */
	double proj_sla_sunlit;         /* (DIM) live projected leaf area index */  /* (m2/kgC) sunlit projected SLA */
	double proj_sla_shade;          /* (DIM) live projected leaf area index */  /* (m2/kgC) shaded projected SLA */
	double proj_lai_sunlit;         /* (DIM) live projected leaf area index */
	double proj_lai_shade;          /* (DIM) live projected leaf area index */
	double max_proj_lai;            /* (DIM) this year's max growing season LAI */
	double prev_leafcalloc;         /* (kgC/m2/y) last year's leaf C allocation */
	double canopy_w;                /* (kgH2O/m2) water store on canopy */
	double psi;                     /* (MPa) water potential of soil and leaves */
	int	   wstress_days;            /* # of days in which water stress occurs */
	double crit_vwc;	            /* (m/m) water content at stomatal closure */
	double min_vwc;	                /* (m/m) this growing season minimum water content */
	double max_fparabs;	            /* (DIM) maximum fraction of PAR absorbed */
	int    leafon;                  /* (flag) binary flag, leaves on=1, not on=0 */
	double day_livestem_turnover;   /* (kgC/m2/day) livestem turnover to dead wood */
	double day_livecroot_turnover;  /* (kgC/m2/day) livestem turnover to dead wood */
	double day_deadleaf_turnover;   /* (kgC/m2/day) dead grass turnover to soil */
};

struct cdayflux_struct
{
	/* gross PSN input */
	double psn_to_cpool;              /* (kgC/m2/d) gross photosynthesis */
	double net_psn;                   /* net photosynthesis psn_to_cpool minus respiration*/
	double potential_psn_to_cpool;    /* (kgC/m2/d) potential gross photosynthesis */

	/* daily phenology fluxes */
	double leafc_to_deadleafc;	   /* (kgC/m2/d) standing dead grass accumulation */
	double leafc_store_to_leafc;   /* (kgC/m2/d) leaf expansion */
	double frootc_store_to_frootc; /* (kgC/m2/d) froot expansion */

	/* phenology fluxes from transfer pool */
	double leafc_transfer_to_leafc;          /* (kgC/m2/d) */
	double frootc_transfer_to_frootc;        /* (kgC/m2/d) */
	double livestemc_transfer_to_livestemc;  /* (kgC/m2/d) */
	double deadstemc_transfer_to_deadstemc;  /* (kgC/m2/d) */
	double livecrootc_transfer_to_livecrootc;/* (kgC/m2/d) */
	double deadcrootc_transfer_to_deadcrootc;/* (kgC/m2/d) */

	/* daily growth fluxes */
	double cpool_to_leafc;              /* (kgC/m2/d) */
	double cpool_to_leafc_store;        /* (kgC/m2/d) */
	double cpool_to_frootc;             /* (kgC/m2/d) */
	double cpool_to_frootc_store;       /* (kgC/m2/d) */
	double cpool_to_livestemc;          /* (kgC/m2/d) */
	double cpool_to_livestemc_store;    /* (kgC/m2/d) */
	double cpool_to_deadstemc;          /* (kgC/m2/d) */
	double cpool_to_deadstemc_store;    /* (kgC/m2/d) */
	double cpool_to_livecrootc;         /* (kgC/m2/d) */
	double cpool_to_livecrootc_store;   /* (kgC/m2/d) */
	double cpool_to_deadcrootc;         /* (kgC/m2/d) */
	double cpool_to_deadcrootc_store;   /* (kgC/m2/d) */
	double cpool_to_gresp_store;        /* (kgC/m2/d) */

	/* annual turnover of storage to transfer pools */
	double leafc_store_to_leafc_transfer;            /* (kgC/m2/d) */
	double frootc_store_to_frootc_transfer;          /* (kgC/m2/d) */
	double livestemc_store_to_livestemc_transfer;    /* (kgC/m2/d) */
	double deadstemc_store_to_deadstemc_transfer;    /* (kgC/m2/d) */
	double livecrootc_store_to_livecrootc_transfer;  /* (kgC/m2/d) */
	double deadcrootc_store_to_deadcrootc_transfer;  /* (kgC/m2/d) */
	double gresp_store_to_gresp_transfer;            /* (kgC/m2/d) */

	/* turnover of live wood to dead wood */
	double livestemc_to_deadstemc;        /* (kgC/m2/d) */
	double livecrootc_to_deadcrootc;      /* (kgC/m2/d) */

	/* daily autotroph respiration fluxes */
	double leaf_day_mr;     /* (kgC/m2/d) day leaf maintenance respiration */
	double leaf_night_mr;   /* (kgC/m2/d) night leaf maintenance respiration */
	double froot_mr;        /* (kgC/m2/d) froot maintenance respiration */
	double livestem_mr;     /* (kgC/m2/d) live stem maintenance respiration */
	double livecroot_mr;    /* (kgC/m2/d) live croot maint. respiration */
	double total_mr;	    /* (kgC/m2/d) total maintenance respiration */

	/* daily growth respiration fluxes */
	double cpool_leaf_gr;                /* (kgC/m2/d) */
	double transfer_leaf_gr;             /* (kgC/m2/d) */
	double cpool_froot_gr;               /* (kgC/m2/d) */
	double transfer_froot_gr;            /* (kgC/m2/d) */
	double cpool_livestem_gr;            /* (kgC/m2/d) */
	double transfer_livestem_gr;         /* (kgC/m2/d) */
	double cpool_deadstem_gr;            /* (kgC/m2/d) */
	double transfer_deadstem_gr;         /* (kgC/m2/d) */
	double cpool_livecroot_gr;           /* (kgC/m2/d) */
	double transfer_livecroot_gr;        /* (kgC/m2/d) */
	double cpool_deadcroot_gr;           /* (kgC/m2/d) */
	double transfer_deadcroot_gr;        /* (kgC/m2/d) */
	double transfer_gr;                  /* (kgC/m2/d) */
	//following is added by guoping based biomeBGC4.2
	double cpool_to_leafc_store_gr;
	double cpool_to_frootc_store_gr;
	double cpool_livestem_storage_gr;
	double cpool_deadstem_storage_gr;
	double cpool_livecroot_storage_gr;
	double cpool_deadcroot_storage_gr;

	double total_gr;                     /* (kgC/m2/d) */
};


/* nitrogen state variables (including sums for sources and sinks) */
struct nstate_struct
{
	double preday_totaln;   /* (kgN/m2) previous days plant nitrogen total */
	double totaln;  	    /* (kgN/m2) previous days plant nitrogen total */
	double npool;           /* (kgN/m2) temporary plant N pool */
	double leafn;           /* (kgN/m2) leaf N */
	double dead_leafn;      /* (kgN/m2) standing dead leaf N for grasses */
	double live_stemn;      /* (kgN/m2) live stem N */
	double dead_stemn;      /* (kgN/m2) dead stem N */
	double live_crootn;     /* (kgN/m2) live coarse root N */
	double dead_crootn;     /* (kgN/m2) dead coarse root N */
	double frootn;          /* (kgN/m2) fine root N */
	double retransn;	    /* (kgN/m2) retranslocated N */

	double leafn_transfer;   	/* (kgN/m2) leaf N to be allocated from last season */
	double livestemn_transfer;  /* (kgN/m2) live stemwood N to be allocated from last season */
	double deadstemn_transfer;  /* (kgN/m2) dead stemwood N to be allocated from last season */
	double livecrootn_transfer; /* (kgN/m2) live coarse root N to be allocated from last season */
	double deadcrootn_transfer; /* (kgN/m2) dead coarse root N to be allocated from last season */
	double frootn_transfer; 	/* (kgN/m2) fine root N C to be allocated from last season */

	double leafn_store;         /* (kgN/m2) leaf N stored from year's growth */
	double livestemn_store;     /* (kgN/m2) live stemwood N stored from this years growth */
	double deadstemn_store;     /* (kgN/m2) dead stemwood N  stored from this years growth*/
	double livecrootn_store;    /* (kgN/m2) live coarse root N  stored from this years growth*/
	double deadcrootn_store;    /* (kgN/m2) dead coarse root N  stored from this years growth*/
	double frootn_store;        /* (kgN/m2) fine root N  stored from this years growth */

	double cwdn;	            /* (kgN/m2) coarse woody debris N*/
	double fire_snk;            /* (kgC/m2) sum of nitrogen loss due to fire */
};

/* daily nitrogen flux variables */
struct ndayflux_struct
{
	/* daily flux to plant pool */
	double sminn_to_npool;         /* (kgN/m2/d) plant N uptake */

	/* daily phenology fluxes */
	double leafn_to_deadleafn;	               /* (kgN/m2/d) standing dead grass accumulation */
	double leafn_store_to_leafn;               /* (kgN/m2/d) leaf expansion N */
	double frootn_store_to_frootn;             /* (kgN/m2/d) fine root expansion N */

	/* phenology fluxes from transfer pool */
	double leafn_transfer_to_leafn;             /* (kgN/m2/d) */
	double frootn_transfer_to_frootn;           /* (kgN/m2/d) */
	double livestemn_transfer_to_livestemn;     /* (kgN/m2/d) */
	double deadstemn_transfer_to_deadstemn;     /* (kgN/m2/d) */
	double livecrootn_transfer_to_livecrootn;   /* (kgN/m2/d) */
	double deadcrootn_transfer_to_deadcrootn;   /* (kgN/m2/d) */

	/* daily growth fluxes */
	double potential_N_uptake; /* (kgN/m2) potential uptake from soil */
	double retransn_to_npool;             /* (kgN/m2/d) */
	double npool_to_leafn;                /* (kgN/m2/d) */
	double npool_to_leafn_store;          /* (kgN/m2/d) */
	double npool_to_frootn;               /* (kgN/m2/d) */
	double npool_to_frootn_store;         /* (kgN/m2/d) */
	double npool_to_livestemn;            /* (kgN/m2/d) */
	double npool_to_livestemn_store;      /* (kgN/m2/d) */
	double npool_to_deadstemn;            /* (kgN/m2/d) */
	double npool_to_deadstemn_store;      /* (kgN/m2/d) */
	double npool_to_livecrootn;           /* (kgN/m2/d) */
	double npool_to_livecrootn_store;     /* (kgN/m2/d) */
	double npool_to_deadcrootn;           /* (kgN/m2/d) */
	double npool_to_deadcrootn_store;     /* (kgN/m2/d) */

	/* annual turnover of storage to transfer */
	double leafn_store_to_leafn_transfer;           /* (kgN/m2/d) */
	double frootn_store_to_frootn_transfer;         /* (kgN/m2/d) */
	double livestemn_store_to_livestemn_transfer;   /* (kgN/m2/d) */
	double deadstemn_store_to_deadstemn_transfer;   /* (kgN/m2/d) */
	double livecrootn_store_to_livecrootn_transfer; /* (kgN/m2/d) */
	double deadcrootn_store_to_deadcrootn_transfer; /* (kgN/m2/d) */

	/* turnover of live wood to dead wood, with retranslocation */
	double livestemn_to_deadstemn;        /* (kgN/m2/d) */
	double livestemn_to_retransn;         /* (kgN/m2/d) */
	double livecrootn_to_deadcrootn;      /* (kgN/m2/d) */
	double livecrootn_to_retransn;        /* (kgN/m2/d) */
};


// daily conductance multipliers
struct mult_conduct_struct
{
	//double  APAR;	/* 0-1 */
	//double  tavg;	/* 0-1 */
	//double  LWP;	/* 0-1 */
	//double	CO2;	/* 0-1 */
	//double	tmin;	/* 0-1 */
	//double  vpd;	/* 0-1 */
};

/* canopy ecophysiological constants */
/*--------------------------------------------------------------*/
/*	Notes:							*/
/*	lai_ratio currently IGNORED and the input projected	*/
/*		  and all sided lai and pai are used.		*/
/*	int_coef  is assumed to be kg/m2/PAI/d			*/
/*--------------------------------------------------------------*/
struct epconst_struct
{
	double max_lai;                   /* (m2/m2) maximum leaf area */
	double proj_sla;                  /* (m2/kgC) projected specific leaf area */
	double shade_sla_mult;            /* (DIM) ratio of shade sla to sunlit sla */
	double proj_swa;                  /* (m2/kgC) all-sided specific stem wood area */
	double lai_ratio;                 /* (DIM) all-sided LA / one-sided LA ratio */
	double int_coef;                  /* (kg/m2/LAI/d) canopy precip interception coef */
	double ext_coef;                  /* (DIM) canopy light extinction coefficient */
	double flnr;                      /* (kg NRub/kg Nleaf) leaf N in Rubisco */
	double ppfd_coef;                 /* (s/m2/umol) shape parameter for ppfd hyperbola */
	double topt;                      /* (deg C) optimum air temperature for gs */
	double tmax;                      /* (deg C) maximum air temperature for gs */
	double tcoef;                     /* (NONE) coefficient for tempcurve function */
	double psi_open;                  /* (MPa) psi at start of conductance reduction */
	double psi_close;                 /* (MPa) psi at complete conductance reduction */
	double vpd_open;                  /* (Pa) vpd at start of conductance reduction */
	double vpd_close;                 /* (Pa) vpd at complete conductance reduction */
	double gl_smax;                   /* (m/s) maximum leaf-scale stomatal conductance */
	double gl_c;                      /* (m/s) leaf-scale cuticular conductance */
	double gl_bl;                     /* (m/s) leaf-scale boundary layer conductance */
	double gs_tmin;		              /* (deg C) lower temperature theshold for leaf onset */
	double gs_tmax;                   /* (deg C) upper temperature theshold for leaf onset */
	double gs_trange;                 /* (deg C) temperature range for leaf onset */
	double gs_vpd_min;	              /* (Pa) lower vpd threshold for leaf onset */
	double gs_vpd_max;	              /* (Pa) upper vpd threshold for leaf onset */
	double gs_vpd_range;              /* (Pa)  vpd range for leaf onset */
	double gs_dayl_min;	              /* (s) lower day length threshold for leaf onset */
	double gs_dayl_max;	              /* (s) upper day length threshold for leaf onset */
	double gs_dayl_range;	          /* (s)  day length range for leaf onset */
	double coef_CO2;	              /* DIM 0-1  conductance sensitivity to CO2 */
	//int day_leafon;                   /* (DIM) yearday leaves on */
	//int day_leafoff;                  /* (DIM) yearday leaves off - set to 0 for no leaf drop cond.  */
	//int ndays_expand;                 /* (DIM) number of transition days leaf on to/from off  */
	//int ndays_litfall;                /* (DIM) number of transition days for full leaf drop  */
	//int phenology_flag;	              /* (DIM) set as 1 for dynamic phenology	*/
	int allocation_flag;	          /* (DIM) set as 1 for dynamic allocation */
	int veg_type;		              /* (DIM) set as 1 for tree; 0 for grass	*/
	int phenology_type;   	          /* (DIM) set as 1 for decid; 0 for evergreen	*/
	double gr_perc;	                  /* (DIM 0-1) percent of growth allocated to respiration */
	double leaf_turnover;             /* (1/yr) annual leaf turnover fraction */
	double livewood_turnover;         /* (1/yr) annual live wood turnover fraction */
	double deadleaf_turnover;         /* (1/yr) annual turnover of standing dead grass fraction */
	double froot_turnover;            /* (1/yr) annual fine root turnover fraction */
	double kfrag_base;	              /* (1/day) daily coarse woody debris fragmentation rate */
	double storage_transfer_prop;     /* (DIM) percent of storage allocated in a given year */
	double daily_mortality_turnover;  /* (1/day) daily mortality turnover */
	double daily_fire_turnover;       /* (1/day) daily fire loss */
	double froot_cn;                  /* (kgC/kgN) C:N for fine roots */
	double leaf_cn;                   /* (kgC/kgN) C:N for leaves */
	double livewood_cn;               /* (kgC/kgN) C:N for live wood */
	double leaflitr_cn;               /* (kgC/kgN) constant C:N for leaf litter */
	double leaflitr_flab;             /* (DIM) leaf litter labile fraction */
	double leaflitr_fucel;            /* (DIM) leaf litter unshielded cellulose fract. */
	double leaflitr_fscel;            /* (DIM) leaf litter shielded cellulose fract. */
	double leaflitr_flig;             /* (DIM) leaf litter lignin fraction */
	double litter_gsurf_intercept;    /* m/2 - intercept of conductance vs litter theta */
	double litter_gsurf_slope;        /* (DIM) - slope of conductance vs litter theta */
	double litter_moist_coef;         /* m/kg  water holding capacity of litter in m/kg/m2 */
	double frootlitr_flab;            /* (DIM) froot litter labile fraction */
	double frootlitr_fucel;           /* (DIM) froot litter unshielded cellulose fract */
	double frootlitr_fscel;           /* (DIM) froot litter shielded cellulose fract */
	double frootlitr_flig;            /* (DIM) froot litter lignin fraction */
	double deadwood_fucel;            /* (DIM) dead wood unshileded cellulose fraction */
	double deadwood_fscel;            /* (DIM) dead wood shielded cellulose fraction */
	double deadwood_flig;             /* (DIM) dead wood lignin fraction */
	double deadwood_cn;               /* (kgC/kgN) dead wood C:N (calc. internally) */
	double alloc_frootc_leafc;        /* (ratio) new fine root C to new leaf C */
	double alloc_crootc_stemc;        /* (ratio) new live croot C to new live stem C */
	double alloc_stemc_leafc;         /* (ratio) new live stem C to new leaf C */
	double alloc_livewoodc_woodc;     /* (ratio) new live wood to total wood C */
	double height_to_stem_coef;       /* (DIM) height vs stemC, parameter 1 (multiplier) */
	double height_to_stem_exp;        /* (DIM) height vs stemC, parameter 2 (exponent) */
	double phloemcsa_per_alllai;      /* phloem CSA per unit all-sided LAI */
	double alloc_cpool_fhold;         /* (DIM) fraction cpool held for next year */
	double alloc_npool_fhold;         /* (DIM) fraction npool held for next year */
	double alloc_maxlgf;              /* (DIM) maximum leaf growth fraction */
	double alloc_prop_day_growth;     /* (DIM) fraction of daily photosynthate used for daily growth */
	double min_leaf_carbon;           /* kgC minimum leaf carbon before death */
	double resprout_leaf_carbon;      /* kgC leaf carbon to assign for resprouting */
	double root_growth_direction;     /* (0-1) 1 is full vertical, 0 fully horizontal */
	double root_distrib_parm;         /*  (DIM) used with root biomass in kg/m2 */
	int	   max_years_resprout;        /* num years of resprouting before death */
};


//===============================================================================================================================
//	Define a stratum default object.				
//===============================================================================================================================
struct	stratum_default
{
	int		ID;
	double	gsurf_slope;			            /* DIM */
	double	gsurf_intercept;			        /* DIM */
	double	K_absorptance;			            /* DIM */
	double	K_reflectance; 			            /* DIM */
	double	K_transmittance; 		            /* DIM */
	double	lai_stomatal_fraction;		        /* DIM	*/
	double	min_heat_capacity;	                /* J/m3/K */
	double	max_heat_capacity;	                /* J/m3/K */
	double	PAR_absorptance;		            /* DIM */
	double	PAR_reflectance;		            /* DIM */
	double	PAR_transmittance; 		            /* DIM */
	double	specific_rain_capacity;			    /* m / LAI	*/
	double	specific_snow_capacity;			    /* m /  LAI	*/
	double  wind_attenuation_coeff;	    	    /* 1/meters     */
	double	ustar_overu;			            /* DIM	*/
	struct	epconst_struct	epc;
	struct	mrconst_struct	mrc;
};

//===============================================================================================================================
//	Define a canopy strata object.					
//===============================================================================================================================
struct	canopy_strata_object
{
	//	int		patch_ID;
	int		ID;
	//	int		num_base_stations;				
	double	APAR_direct;					/* (umol photon/m2*day) */
	double	APAR_diffuse;					/* */
	double	cover_fraction;
	double  dC13;		                    /* % discrimination of C13 */
	double	evaporation;					/* meters/day	*/
	double	ga;						        /* m/s		*/
	double	gap_fraction;					/* unitless 	*/
	double	gs;						        /* m/s		*/
	double	potential_gs_sunlit;			/* m/s		*/
	double	potential_gs_shade;				/* m/s		*/
	double	gs_sunlit;					    /* m/s		*/ /* (m/s) leaf-scale conductance to transpired water */
	double	gs_shade;					    /* m/s		*//* (m/s) leaf-scale conductance to transpired water */
	double	gsurf;						    /* m/s		*/
	double	Kstar_direct;					/* Kj/(m2*day)	*/
	double	Kstar_diffuse;					/* Kj/(m2*day)	*/
	double	Lstar;						    /* Kj/(m2*day)	*/
	double	PAR_after_reflection;			/* (umol photon/m2*day) */
	double  ppfd_sunlit;			        /* (umol/m2/s) PAR photon flux density */
	double  ppfd_shade;			            /*  (umol/m2/s) PAR photon flux density */
	double	potential_evaporation;			/* meters/day	*/
	double	rain_stored;					/* meters	*/
	double  rain_interception;
	double	snow_stored;					/* meters	*/
	double  snow_interception;
	double  preday_rain_stored;
	double  preday_snow_stored;
	double	sublimation;					/*	meters	*/
	double	surface_heat_flux;				/* kJ/day		*/
	double  PET;		                    /* m water /day */
	double  PE;		                        /* m water /day */
	double	transpiration_unsat_zone;	    /* m water / day */
	double	transpiration_sat_zone;		    /* m water / day */
	double  wind;						    /* 1/meters	*/
	struct  rooting_zone_object	rootzone;
	struct	cdayflux_struct cdf;
	struct	cstate_struct	cs;
	struct	epvar_struct epv;
	struct	nstate_struct	ns;
	struct	ndayflux_struct ndf;
	struct	phenology_struct phen;
	//	struct	base_station_object	**base_stations;
	struct	stratum_default	*defaults;
	struct	canopy_strata_hourly_object	*hourly;
	struct	mult_conduct_struct	mult_conductance;
};


struct out_basin_daily_hydrology
{
	double arain_throughfall;
	double asnow_throughfall;
	double asat_deficit_z;
	double asat_deficit;
	double arz_storage;
	double aunsat_storage;
	double arz_drainage;
	double aunsat_drainage;
	double acap_rise;
	double aevaporation;
	double asnowpack;
	double atranspiration;
	double abase_flow;
	double areturn_flow;
	double astreamflow;
	double apsn;
	double alai;
	double hgwQout;
	double hgwNout;
	double hgw;
	double hgwN;
	double adetention_store;
	double asat_area;
	double alitter_store;
	double acrain;
	double aperc_snow;
	double asublimation;
	double var_trans;
	double aacctrans;
	double var_acctrans;
	double apet;
	double adC13;
};


struct reservoir_object{
	int    ID;
	double DEM;
	double min_height;
	double min_volume;
	double max_height;
	double max_volume;
	double acc_area;
	double volume;
	double discharge;
	bool   start_discharge;
};