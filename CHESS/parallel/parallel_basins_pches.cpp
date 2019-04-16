//------------------------------------------------------------------------------------------------------------------------------
//	A parallel way for computing daily F
//	Designed by Xu.
//------------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include <iostream>

using namespace std;


void		parallel_basins_pches(struct 	patch_object *patch,
	int **patch_pch,
	int patch_num,
	int thread_patch_num[]
)
{
	int thread_inx=0;

	for (int inx = 0; inx < patch_num; inx++) {
		
		//start from 0
		thread_inx = patch[inx].basins-1;

		patch_pch[thread_inx][(thread_patch_num[thread_inx])] = inx;
		
		thread_patch_num[thread_inx]++;
	
	
	}//end of all patches
	


}//end of thread_line