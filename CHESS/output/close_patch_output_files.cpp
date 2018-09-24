#include <stdio.h>

#include "CHESS.h"
#include <iostream>
using namespace std;

//struct output_hydro_plant construct_output_files(int start_year,int end_year,
void close_patch_output_files(struct output_hydro_plant *DM_outfiles)
{
	if(DM_outfiles->fPatchDailyHydro  !=NULL) fclose(DM_outfiles->fPatchDailyHydro);
	if(DM_outfiles->fPatchDailyPlant  !=NULL) fclose(DM_outfiles->fPatchDailyPlant);
  // return (DM_outfiles);
}