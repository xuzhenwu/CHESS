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
void	init_patch_object(struct patch_object *patch);



void    patch_daily_I(struct patch_object *, struct daily_clim_object, struct command_line_object *, struct	date);
void    patch_hourly(struct patch_object *, struct command_line_object *, struct	date);
void	patch_daily_F(struct patch_object *, struct command_line_object *, struct date);
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
void	parallel_daily(struct patch_object *patch, struct command_line_object *command_line, struct	date current_date, struct  daily_clim_object daily_clim,
	int patch_pch[][patch_num], int thread_patch_num[], int thread_inx);
void	parallel_basins_pches(struct 	patch_object *patch, int patch_pch[][patch_num], int thread_patch_num[]);

//CLIMATE
void    chess_climate_daily(struct input_Clim_Files, struct date, struct  daily_clim_object *daily_clim, int, int, int, int *); //windows
//struct  daily_clim_object construct_daily_clim(double, double, double, double); //unix

//LAND
void	chess_land_daily(struct 	patch_object *patch, struct 	command_line_object *command_line, struct date current_date,
	struct  daily_clim_object daily_clim, int num_patches, int patch_pch[][patch_num], int thread_patch_num[]);

//CHANNEL
void    chess_channel_daily(struct patch_object *, struct reservoir_object, struct command_line_object *, struct	date, int, double);
