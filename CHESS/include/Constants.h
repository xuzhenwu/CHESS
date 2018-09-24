//===============================================================================================================================
// Physical and atmospheric constants
// Iribane, J.V. and W.L. Godson, 1981.  Atmospheric Thermodynamics. 2nd
//	Edition. D. Reidel Publishing Company, Dordrecht.  The Netherlands.
//	(pp. 10,167-168,245)
//===============================================================================================================================

//define threads and patch num
#define thread_num 11
#define patch_num  131240


#define	G_STD	9.80665		/* (m2/s) standard gravitational accel.		*/
#define	P_STD	101325.0	/* (Pa)	standard pressure at 0.0 m elevation	*/
#define T_STD	288.15		/* (K) 	standard temp at 0.0 m elevation	*/
#define	MA	28.9644e-3	    /* (kg/mol) molecular weight of air		*/
#define	MW	18.0148e-3	    /* (kg/mol) molecular weight of water		*/
#define	CP	1010.0		    /* (J/kg*K) specific heat of air		*/
#define	LR_STD	0.0065	    /* (-K/m) standard temperature lapse rate	*/
#define	R	8.3143		    /* (m3 PA / mol K) gas law constant		*/
#define	SBC	5.67e-8		    /* (W/m2*K4) Stefan Boltzmann Constant		*/
#define	EPS	0.6219		    /* (MW/MA) unitless ratio of molecular weights	*/
						    
#define HVAP 42.7           /* Heat of vaporization, kJ/mol */
#define KELVIN 273.16
#define PI	3.14159265359
#define seconds_per_day 86400

/*physiological constants	*/
#define	RAD2PAR	0.5		         /* (DIM) ratio PAR/ SWtotal			*/
#define	EPAR	4.55		     /* (umol/J) PAR photon energy ratio		*/
#define SECPERRAD 13750.9871     /* seconds per radian of hour angle */
#define LITTER_ALBEDO 0.02

#define LIVELAB_CN  50           /* C:N for labile fraction of live wood */
#define LIG_CN      500.0        /* C:N for all lignin components */
#define CEL_CN      250.0        /* C:N for all cellulose components */
#define SOIL1_CN    12.0         /* C:N for fast microbial recycling pool */
#define SOIL2_CN    12.0         /* C:N for slow microbial recycling pool */
#define SOIL3_CN    10.0         /* C:N for recalcitrant SOM pool (humus) */
#define SOIL4_CN    8.0          /* C:N for recalcitrant SOM pool (lignin) */

#define NONWOOD_GRPERC 1.2       /* (DIM) growth resp per unit of nonwood C grown */
#define WOOD_GRPERC    2.0       /* (DIM) growth resp per unit of woody C grown */


//===============================================================================================================================
//	Define macros.											
//===============================================================================================================================
#define MAXS   80		/* size of filename string    */
#define NULLVAL	-9999	
#define TRUE	1
#define FALSE	0

#define MAXSTR	1024
#define DtoR	  0.01745329  	
#define PI	3.14159265359

//#define seconds_per_day 86400
#define	ess_snow	0.85 //only used in compute_lstar
#define	ess_soil	0.95 //only used in compute_lstar
#define ess_veg   	0.96 //only used in compute_lstar

#define INTERVAL_SIZE 0.1 
#define STREAM 1
#define ROAD 2

#define NON_VEG 20
#define TREE 1
#define GRASS 2
#define C4GRASS 3
#define LITTER 3
#define DECID 1
#define EVERGREEN 0
#define STATIC 0
#define DYNAMIC 2
#define WARING 2
#define CONSTANT 1
#define DICKENSON 3
#define ZERO 0.000000001
//#define URBANID		100

//===============================================================================================================================
// Define min and max functions                            
//===============================================================================================================================
#define max(a,b)    ((a) > (b) ? (a) : (b))
#define min(a,b)    ((a) < (b) ? (a) : (b))

#define LEN_GRASSHEADER 12
#define LEN_ARCHEADER	12