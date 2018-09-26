//=====================================================================================================================
//  assign_neighbours( struct patch_object *patch,int num_neighbours,FILE *routing_file)						
//                                                              
//  OPTIONS                                                     
//                                                              
//  DESCRIPTION                                                 
//	assigns pointers to neighbours of each patch				
//	as given in topology input file								
//=====================================================================================================================
#include <stdio.h>

#include "CHESS.h"
int assign_neighbours( struct neighbour_object *neighbours,
					   int num_neighbours,
					   struct patch_object *patch,
					   FILE *routing_file, int rows,int cols)
{
	/*--------------------------------------------------------------*/
	/*  Local function declaration                                  */
	/*--------------------------------------------------------------*/
	void *alloc (size_t, char *, char *);
	struct patch_object *find_patch( int,struct patch_object *, int, int);
	
	/*--------------------------------------------------------------*/
	/*  Local variable definition.                                  */
	/*--------------------------------------------------------------*/
	int i,inx, new_num_neighbours;
	int patch_ID;
	double gamma;
	double sum_gamma;
	//struct patch_object *neigh;

	/*--------------------------------------------------------------*/
	/*  find and assign each neighbour to array						*/
	/*--------------------------------------------------------------*/
	inx = 0;
	sum_gamma = 0.0;
	new_num_neighbours = num_neighbours;
	for (i=0; i< num_neighbours; i++) {
		fscanf(routing_file,"%d %lf",
			&patch_ID,
			&gamma);
		/*----------------------------------------------------------------------*/
		/*	only attach neighbours which have a gamma > 0 			*/
		/* patches should point only to other patches in the same basin		*/
		/*	- ie. gamma to adjacent areas outside the basin 		*/
		/* 	should be zero							*/
		/* 	excepth in the case of a stream patch 				*/
		/*	 - this however is not strictly enforced			*/
		/*----------------------------------------------------------------------*/
		if (gamma > 0.0) {
			//if  ( (patch_ID != 0) && (zone_ID != 0) && (hill_ID != 0) )
			//neigh = find_patch(patch_ID, patch,rows,cols);
			//else	neigh = basin->outside_region;
			//neighbours[inx].patch= neigh;
			neighbours[inx].patch= find_patch(patch_ID, patch,rows,cols);
			sum_gamma += gamma;
			neighbours[inx].gamma = gamma;

			//printf("neighbours[inx].patch->ID %d %d \n",inx,neighbours[inx].patch->ID);
			inx += 1;
		}
		else	new_num_neighbours -= 1;//xu. ??? gamma<0? which has been checked in Create flowtable.exe
	}

	return(new_num_neighbours);
}/*end assign_neighbours.c*/
