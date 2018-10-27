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

using namespace std;

void  chess_land_daily(struct patch_object *patch,
	struct 	command_line_object *command_line,
	struct	date 			current_date,
	struct  daily_clim_object *daily_clim,
	int num_patches,
	int patch_pch[][PATCH_NUM],
	int thread_patch_num[]
)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. Initial and Run Multithreads for CHESS LAND daily processes
	//Where parallel_daily is a key function involves
	//Ecohydrological processes within a patch & Surface and subsurface transport of water and other nutrients
	//---------------------------------------------------------------------------------------------------------------------------

	int thread_inx = 0;
	thread pd1(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd2(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd3(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd4(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd5(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd6(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd7(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd8(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd9(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd10(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd11(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd12(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd13(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd14(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd15(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd16(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd17(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd18(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd19(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd20(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd21(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd22(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd23(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;
	thread pd24(parallel_land_daily, patch, command_line, current_date, daily_clim, patch_pch, thread_patch_num, thread_inx); thread_inx++;


	//Join all threads
	pd1.join();	pd2.join();	pd3.join(); pd4.join();
	pd5.join();	pd6.join();	pd7.join();	pd8.join();
	pd9.join();	pd10.join();pd11.join();pd12.join();
	pd13.join();	pd14.join();	pd15.join();	pd16.join();
	pd17.join();	pd18.join();	pd19.join();	pd20.join();
	pd21.join();	pd22.join();	pd23.join();	pd24.join();

	//---------------------------------------------------------------------------------------------------------------------------
	//END OF LAND COMPUTATION
	//---------------------------------------------------------------------------------------------------------------------------

	return;
}//__END OF THIS FUNCTION