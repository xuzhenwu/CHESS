//---------------------------------------------------------------------------------------------------------------------------
//xu. LAND.. PARALLEL COMPUTATION.. Initial and run parallel CHESS daily Ecohydrological process and Transport of water and nutrients
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <math.h>
#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include <iostream>
#include <thread>

using std::thread;

void  chess_patch_daily(struct patch_object *patch,
	struct 	command_line_object *command_line,
	struct	date 			current_date,
	struct  daily_clim_object *daily_clim,
	int num_patches,
	int **patch_pch,
	int thread_num,
	int thread_patch_num[]
)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. The patch_daily process are processed in separated sub-basins by ASP(Automatic Sub-basin Partition)
	//---------------------------------------------------------------------------------------------------------------------------
	
	//creat thread
	std::thread *thd=new std::thread [thread_num];
	//init and run all thread
	for (int thread_inx = 0; thread_inx != thread_num; thread_inx++) {

		//parallel patch daily is served to be a elementary function of patch daily process
		thd[thread_inx] = thread(parallel_patch_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx);
	}
	//wait till all threads terminate
	for (int thread_inx = 0; thread_inx != thread_num; thread_inx++) {
		thd[thread_inx].join();
	}

	delete[] thd;

	return;
}//__END OF DAILY PATCH