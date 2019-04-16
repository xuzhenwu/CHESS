#include "Constants.h"
//---------------------------------------------------------------------------------------------------------------------------
// CHESS INITIALIZATION .function	                                                
//---------------------------------------------------------------------------------------------------------------------------

struct  command_line_object	*construct_command_line(int main_argc, char **, struct command_line_object *);
int     construct_routing_topology(struct patch_object *, char *, char *, int, int);
void	read_geo_images(struct patch_object *, struct command_line_object *, int, int, double, double, double, char *, char *, int, int, int ,int *,int);
void    construct_patch(struct patch_object *, struct command_line_object *, int, int, char *, char *, int);
struct  input_Clim_Files open_Clim_Files(char *, char *);

//---------------------------------------------------------------------------------------------------------------------------
// CHESS SIMULATION .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
void    chess_climate_daily(struct input_Clim_Files,  struct date, struct  daily_clim_object *daily_clim, int climate_num, int, int, int, int *);
void	chess_patch_daily(struct 	patch_object *patch, struct 	command_line_object *command_line, struct date current_date,
	struct  daily_clim_object *daily_clim, int num_patches, int **patch_pch,int thread_num, int thread_patch_num[]);
void    chess_channel_daily(struct patch_object *, struct reservoir_object, struct command_line_object *, struct	date, int, double);



//---------------------------------------------------------------------------------------------------------------------------
// CHESS OUTPUT .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
void    construct_basin_output_files(char *, struct output_hydro_plant *, struct command_line_object *);
void    construct_patch_output_files(struct date, struct out_date_range, char *, int patch_num, struct output_hydro_plant *, struct OutArray_object *OutArray, struct command_line_object *);
void    close_patch_output_files(struct output_hydro_plant *, struct OutArray_object *OutArray);

//xu.
void	construct_gauge_output_files(struct patch_object *patch, char *outPutPath, struct output_hydro_plant *DM_outfiles, struct command_line_object *command_line,
		int *gauge_list);


//---------------------------------------------------------------------------------------------------------------------------
// CHESS OUTPUT .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
int     end_day_norm_leap(int, int);
void    init_out_monthly(struct patch_object *, struct accumulate_patch_object *, int, int, struct command_line_object *);








//---------------------------------------------------------------------------------------------------------------------------
// Patch daily .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
void	zero_patch_object(struct patch_object *patch);
void    patch_climate_initial(struct patch_object *, struct daily_clim_object, struct command_line_object *, struct	date);
void    patch_land_initial(struct patch_object *,  struct command_line_object *, struct date);


void    patch_radiation_daily(struct patch_object *, struct command_line_object *, struct	date);
void	patch_daily_final(struct patch_object *, struct command_line_object *, struct date);

//patch lateral flow
void    patch_lateral_flow(patch_object *, struct command_line_object *,  struct date);
void	update_drainage_stream(struct patch_object *, int, double, int, int);
void	update_drainage_land(struct patch_object *, int, double, int, int);



void    out_daily(int, struct patch_object *, struct date, struct out_date_range, struct output_hydro_plant *, struct command_line_object *);
void    out_basin_level_daily(int, struct patch_object *, struct date, struct out_date_range, struct output_hydro_plant *, struct command_line_object *);
void    out_patch_level_daily(int, struct patch_object *, struct date, struct out_date_range, struct output_hydro_plant *, struct OutArray_object *OutArray, struct command_line_object *);
void    output_patch_daily_hydrology(struct patch_object *patch, int pch,double(*HydroMon)[HYDRO_NUM],struct	date current_date,FILE *outfile,
		struct command_line_object *command_line);
void	output_patch_daily_growth(struct patch_object *patch, int pch,double(*PlantMon)[PLANT_NUM],struct	date current_date,FILE *outfile,
		struct command_line_object *command_line);
//xu.
void	out_gauge_level_daily(int num_patches, struct patch_object *patch, struct date current_date, struct out_date_range outdate,
		struct output_hydro_plant *DM_outfiles, struct command_line_object *command_line,int *gauge_list, int cellsize);
void	output_gauge_daily_hydrology(struct	patch_object *patch, struct	date	current_date, FILE *outfile, struct command_line_object *command_line,int cellsize);
void	output_gauge_daily_growth(struct patch_object *patch, struct	date current_date, FILE *outfile, struct command_line_object *command_line, int cellsize);

void    *alloc(size_t, char *, char *);
void    rewind_daily_clim(struct input_Clim_Files);
void    read_daily_climate(struct input_Clim_Files, struct Input_Data_Range, double[][12][31], double[][12][31], double[][12][31], double[]);


int read_record(FILE *, char *);
void init_phenology_object(patch_object *, int);

//---------------------------------------------------------------------------------------------------------------------------
// Parallel .function	                                                
//---------------------------------------------------------------------------------------------------------------------------
void	parallel_patch_daily(struct patch_object *patch, struct command_line_object *command_line, struct	date current_date, struct  daily_clim_object *daily_clim,
	int **patch_pch, int thread_patch_num[], int thread_inx);
void	parallel_basins_pches(struct 	patch_object *patch, int **patch_pch, int patch_num, int thread_patch_num[]);





//LOCAL FUNCTION IN PATCH DAILY I

void   canopy_stratum_daily_I(struct	patch_object *,struct canopy_strata_object *,struct command_line_object *,struct date);

double	compute_layer_field_capacity(int,int,double,double,double,double,double,double,double,double);

double	compute_delta_water(int,double,double,double,double,double);

double	compute_z_final(int,double,double,double,double,double);

//---------------------------------------------------------------------------------------------------------------------------
//  HYDRO .function	                                                
//---------------------------------------------------------------------------------------------------------------------------

double	compute_capillary_rise(int, double, double, double, double, double);

double  compute_potential_exfiltration(int,double,double,double,double,double,double,double,double);

double  compute_soil_water_potential(int,int,double,double,double,double,double,double,double,double);

int compute_potential_decomp(double,double,double,double,double,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,
	struct  litter_n_object *,struct  cdayflux_patch_struct *,struct  ndayflux_patch_struct *);

void sort_patch_layers(struct patch_object *);

void update_litter_interception_capacity(double,struct  litter_c_object *,struct  litter_object *);


//LOCAL FUNCTION IN PATCH DAILY F
void zone_daily_final(struct patch_object *, struct command_line_object *, struct date);
void compute_Lstar(struct patch_object *);
double compute_delta_water(int, double, double, double, double, double);

double compute_layer_field_capacity(int,int,double,double,double,double,double,double,double,double);

void canopy_stratum_daily_F(struct patch_object *,struct layer_object *,struct canopy_strata_object *,struct command_line_object *,
	struct date);

void   surface_daily_F(struct patch_object *,struct command_line_object *,struct date);

double	snowpack_daily_F(struct date,int,struct	snowpack_object	*,double,double,double,double,double,double,double,double *,double *,
	double *,double *,double,double,double,double,double);


double	compute_infiltration(int,double,double,double,double,double,double,double,
	double,double,double);

double  compute_surface_heat_flux(int,double,double,double,double,double,double,double,double,double);

double	compute_unsat_zone_drainage(int,int,double,double,double,double,double,double);

double  compute_radiative_fluxes(int,double *,double,double,double);

//double  compute_stability_correction(int ,double,double,double,double,double,double);
int 	compute_year_day(struct date);

void 	check_zero_stores(struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,struct  litter_n_object *);

void	update_decomp(struct	date,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,struct  litter_n_object *,
	struct cdayflux_patch_struct *,struct ndayflux_patch_struct *,struct patch_object *);

void	update_dissolved_organic_losses(struct	date,double,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,
	struct  litter_n_object *,struct cdayflux_patch_struct *,struct ndayflux_patch_struct *);

void	update_septic(struct	date,struct  patch_object   *);

void	update_nitrif(struct  soil_c_object   *,struct  soil_n_object   *,struct cdayflux_patch_struct *,struct ndayflux_patch_struct *,
	struct soil_class,double,double,double,double,double,double);

void update_denitrif(struct  soil_c_object   *,struct  soil_n_object   *,struct cdayflux_patch_struct *,struct ndayflux_patch_struct *,
	struct soil_class,double,double);

int	resolve_sminn_competition(struct  soil_n_object   *,double, double,struct ndayflux_patch_struct *);

void canopy_stratum_growth(struct patch_object *,struct canopy_strata_object *,struct command_line_object *,struct date);

void update_gw_drainage( struct patch_object *,struct command_line_object *,struct date);

long julday(struct date);

void compute_ground_water_loss(struct	patch_object *, struct 	command_line_object *, struct 	date);



//---------------------------------------------------------------------------------------------------------------------------
// Update_Drainage_Stream.cpp		                                                
//---------------------------------------------------------------------------------------------------------------------------
double  compute_delta_water(int, double, double, double, double, double);
double  compute_N_leached(int, double, double, double, double, double, double, double,
						double, double, double, double, double);
double  compute_transmissivity_curve(double, double, double, double);
double  compute_varbased_returnflow(double, double, double, struct litter_object *);
double  compute_varbased_flow(double, double, double, double, double, double);
double  recompute_gamma(struct patch_object *, double);
double  compute_infiltration(int, double, double, double, double, double, double, double, 
						double, double, double);

//---------------------------------------------------------------------------------------------------------------------------
// CN .function	                                                
//---------------------------------------------------------------------------------------------------------------------------