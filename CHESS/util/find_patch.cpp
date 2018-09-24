/*--------------------------------------------------------------*/
/*                                                              */
/*		find_patch					*/
/*                                                              */
/*  NAME                                                        */
/*		find_patch					*/
/*                                                              */
/*                                                              */
/*  SYNOPSIS                                                    */
/*  find_patch( struct basin_object *basin)			*/
/*                                                              */
/*  OPTIONS                                                     */
/*                                                              */
/*  DESCRIPTION                                                 */
/*                                                              */
/*	sorts hillslopes, zones and patches within a basin by	*/
/*		elevation					*/
/*                                                              */
/*                                                              */
/*  PROGRAMMER NOTES                                            */
/*                                                              */
/*                                                              */
/*                                                              */
/*--------------------------------------------------------------*/

#include <stdio.h>

#include "CHESS.h"

struct patch_object *find_patch( int patch_ID,
								struct patch_object *patch,
								int rows, int cols)
{
	/*------------------------------------------------------*/
	/*	Local Function Definition. 							*/
	/*------------------------------------------------------*/
	
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	int i;
	int fnd;
	//struct zone_object *zone;
	//struct hillslope_object *hillslope;
	struct patch_object *temp_patch = { nullptr };
	/*--------------------------------------------------------------*/
	/*	find hillslopes												*/
	/*--------------------------------------------------------------*/
	//i = 0;
	//fnd = 0;
	//while ( (fnd == 0) && (i >= 0) && (i < basin->num_hillslopes)) {
	//	if (basin->hillslopes[i]->ID == hill_ID) {
	//		hillslope = basin->hillslopes[i];
	//		fnd = 1;
	//	}
	//	else {
	//		i += 1;
	//	}
	//}
	//if (fnd == 0){
	//	fprintf(stderr,
	//		"FATAL ERROR: Could not find hillslope %d in find_patch\n",hill_ID);
	//	exit(1);
	//}
	/*--------------------------------------------------------------*/
	/*	find zones						*/
	/*--------------------------------------------------------------*/
	//i = 0;
	//fnd = 0;
	//while ( (fnd == 0) && (i >= 0) && (i < hillslope->num_zones)) {
	//	if (hillslope->zones[i]->ID == zone_ID) {
	//		zone = hillslope->zones[i];
	//		fnd = 1;
	//	}
	//	else {
	//		i += 1;
	//	}
	//}
	//if (fnd == 0) {
	//	fprintf(stderr,
	//		"FATAL ERROR: Could not find zone %d in find_patch\n",zone_ID);
	//	exit(1);
	//}
	/*--------------------------------------------------------------*/
	/*	find patches						*/
	/*--------------------------------------------------------------*/
	i = 0;
	fnd = 0;
	while ( (fnd == 0) && (i >= 0) && (i <cols*rows)) {
		if (patch[i].ID == patch_ID) {
			temp_patch = &patch[i];
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
		exit (1);
	}
	return(temp_patch);
}/*end find.patch.c*/
