#include <stdio.h>

#include "CHESS.h"

void match_patch(int p,int patch_ID,struct patch_object *patch,int rows,int cols){
	struct patch_object temp_patch;
	int i,fnd;

	i = 0;
	fnd = 0;

	while ( (fnd == 0) && (i >= 0) && (i <cols*rows)) {
		if (patch[i].ID == patch_ID) {
			temp_patch = patch[i];
			patch[i]=patch[p];
			patch[p]=temp_patch;
			fnd = 1;
			break;
		}
		else {
			i += 1;
		}
	}
	if (fnd == 0) {
		fprintf(stderr,
			"FATAL ERROR: Could not find patch %d\n",
			patch_ID);
		exit(1);
	}
	return;
}