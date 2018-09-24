#include <stdio.h>

#include "CHESS.h"
#include <iostream>
using namespace std;

void init_phenology_object(patch_object *patch, int num_patches){

	for (int i = 0; i < num_patches; i++){
		patch[i].leaf_expand_flag = -999;
		patch[i].litfall_stop_flag = -999;
		patch[i].preyear_litfall_stopday = patch[i].canopy_strata->phen.litfall_stopday;
	}

	return;
}