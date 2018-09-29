#include "Constants.h"


struct  command_line_object	*construct_command_line(int main_argc, char **, struct command_line_object *);
void    read_R_RHESSys_File(char *, struct Input_Grid_Data *, struct In_File_Path  *, struct Input_Data_Range  *, struct out_date_range  *, struct Spin_Up *);
//void    read_R_RHESSys_File(char *);

void	read_images(struct patch_object *, int, int, double, double, double, char *, char *, int, int, int );
void    construct_patch(struct patch_object *, struct command_line_object *, int, int, char *, char *, int);
int     construct_routing_topology(struct patch_object *, char *, char *, int, int);
struct  input_Clim_Files open_Clim_Files(char *, char *);
void    construct_output_files(int, int, struct date, char *, struct output_hydro_plant *, struct command_line_object *);

void    construct_basin_output_files(char *, struct output_hydro_plant *, struct command_line_object *);
void    construct_patch_output_files(struct date, struct out_date_range, char *, struct output_hydro_plant *, struct command_line_object *);
void    close_patch_output_files(struct output_hydro_plant *);

int     end_day_norm_leap(int, int);
void    init_out_monthly(struct patch_object *, struct accumulate_patch_object *, int, int, struct command_line_object *);
void	zero_patch_object(struct patch_object *patch);



void    patch_daily_initial(struct patch_object *, struct daily_clim_object, struct command_line_object *, struct	date);
void    patch_radiation_daily(struct patch_object *, struct command_line_object *, struct	date);
void	patch_daily_final(struct patch_object *, struct command_line_object *, struct date);


void    compute_subsurface_routing_new(patch_object *, struct command_line_object *, int, struct date);

void    compute_subsurface_routing(patch_object *, struct command_line_object *, int, struct date);
void    compute_subsurface_no_routing(patch_object *, struct command_line_object *, int, struct date);

void    out_daily(int, struct patch_object *, struct date, struct out_date_range, struct output_hydro_plant *, struct command_line_object *);
void    out_basin_level_daily(int, struct patch_object *, struct date, struct out_date_range, struct output_hydro_plant *, struct command_line_object *);
void    out_patch_level_daily(int, struct patch_object *, struct date, struct out_date_range, struct output_hydro_plant *, struct command_line_object *);

void    *alloc(size_t, char *, char *);
void    rewind_daily_clim(struct input_Clim_Files);
void    read_daily_climate(struct input_Clim_Files, struct Input_Data_Range, double[][12][31], double[][12][31], double[][12][31], double[]);


int read_record(FILE *, char *);
void init_phenology_object(patch_object *, int);

//xu. parallel
void	parallel_land_daily(struct patch_object *patch, struct command_line_object *command_line, struct	date current_date, struct  daily_clim_object *daily_clim,
	int patch_pch[][patch_num], int thread_patch_num[], int thread_inx);
void	parallel_basins_pches(struct 	patch_object *patch, int patch_pch[][patch_num], int thread_patch_num[]);

//CLIMATE
void    chess_climate_daily(struct input_Clim_Files, struct date, struct  daily_clim_object *daily_clim, int, int, int, int *); //windows
//struct  daily_clim_object construct_daily_clim(double, double, double, double); //unix

//LAND
void	chess_land_daily(struct 	patch_object *patch, struct 	command_line_object *command_line, struct date current_date,
	struct  daily_clim_object *daily_clim, int num_patches, int patch_pch[][patch_num], int thread_patch_num[]);

//CHANNEL
void    chess_channel_daily(struct patch_object *, struct reservoir_object, struct command_line_object *, struct	date, int, double);


//LOCAL FUNCTION IN PATCH DAILY I

void   canopy_stratum_daily_I(struct	patch_object *,struct canopy_strata_object *,struct command_line_object *,struct date);

double	compute_layer_field_capacity(int,int,double,double,double,double,double,double,double,double);

double	compute_delta_water(int,double,double,double,double,double);

double	compute_z_final(int,double,double,double,double,double);

//int	update_rootzone_moist(
//	struct patch_object	*,
//	struct	rooting_zone_object	*,
//	struct command_line_object *);

double	compute_capillary_rise(int, double, double, double, double, double);

double  compute_potential_exfiltration(int,double,double,double,double,double,double,double,double);

double  compute_soil_water_potential(int,int,double,double,double,double,double,double,double,double);

int compute_potential_decomp(double,double,double,double,double,struct  soil_c_object   *,struct  soil_n_object   *,struct  litter_c_object *,
	struct  litter_n_object *,struct  cdayflux_patch_struct *,struct  ndayflux_patch_struct *);

void sort_patch_layers(struct patch_object *);

void zero_patch_daily_flux(struct	patch_object *,struct  cdayflux_patch_struct *,struct  ndayflux_patch_struct *);

void update_litter_interception_capacity(double,struct  litter_c_object *,struct  litter_object *);

void zone_daily_initial(struct 	patch_object 	*patch,struct    daily_clim_object daily_clim,struct 	command_line_object *command_line,struct 	date 	current_date);






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

double  compute_z_final(int,double,double,double,double,double);

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
