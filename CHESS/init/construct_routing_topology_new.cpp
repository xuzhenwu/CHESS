#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

int construct_routing_topology(struct patch_object	*patch,
	char *routing_filename, char *FlowTableName,
	int rows, int cols)

{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	struct patch_object *find_patch(int, struct patch_object *, int, int);

	int assign_neighbours(struct neighbour_object *,
		int,
		struct patch_object *,
		FILE *, int, int);

	void	*alloc(size_t size, const char *array_name, const char *calling_function);

	void match_patch(int, int, struct patch_object *, int, int);

	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	int		i, j;
	int		num_patches, num_neighbours;
	int		patch_ID;
	int		drainage_type;
	double	x, y, z, area, gamma, width;
	FILE	*routing_file;
	struct	patch_object	*stream;


	//=======================================================================================================================
	//xu. 1. IN THIS SECTION WE ARE UNKNOWN ABOUT ALL THE patche[i]'s i and their IDs.
	//=======================================================================================================================
	/*--------------------------------------------------------------*/
	/*  Try to open the routing file in read mode.                    */
	/*--------------------------------------------------------------*/
	strcat(routing_filename, FlowTableName);
	if ((routing_file = fopen(routing_filename, "r")) == NULL) {
		fprintf(stderr, "FATAL ERROR:  Cannot open routing file %s\n",
			routing_filename);
		exit(0);
	} /*end if*/
	fscanf(routing_file, "%d", &num_patches);

	/*--------------------------------------------------------------*/
	/*	Read in  each patch record and find it		.				*/
	/*	if it is a stream add it to the basin level routing list	*/
	/*	otherwise add it to the hillslope level routing list		*/
	/*--------------------------------------------------------------*/


	for (i = 0; i< num_patches; i++) {
		fscanf(routing_file, "%d %lf %lf %lf %lf %d %lf %d",
			&patch[i].ID,
			&patch[i].x, &patch[i].y, &patch[i].z,
			&patch[i].acc_area,
			&patch[i].drainage_type,
			&patch[i].gamma,
			&patch[i].num_neighbours);

		patch[i].num_innundation_depths = 1;
		patch[i].critical_depth = NULLVAL;

		//xu.we only check if runoff generation parameter gamma >0
		if (patch[i].gamma < ZERO) {
			patch[i].drainage_type = STREAM;
			printf("\nAN FATAL ERROR IN Patch %d  \n", patch[i].ID);
		}

		/*--------------------------------------------------------------*/
		/*  Allocate neighbour array									*/
		/*--------------------------------------------------------------*/

		//xu. just assign arrays. the two strings has no means
		//patch[i].neighbours = (struct neighbour_object *)alloc(patch[i].num_neighbours *sizeof(struct neighbour_object), "neighbours", "construct_routing_topology");
		patch[i].neighbours = new struct neighbour_object[(patch[i].num_neighbours)]{};

		//we are unknown about neigh's struct address now and it will be fixed in section 2
		for (int neigh = 0; neigh != patch[i].num_neighbours; neigh++) {
			fscanf(routing_file, "%d %lf",
				&patch[i].neighbours[neigh].patch,
				&patch[i].neighbours[neigh].gamma);
		}

		//patch[i].num_neighbours = assign_neighbours(patch[i].neighbours, patch[i].num_neighbours, patch, routing_file,rows,cols);

		/*
		if (patch[i].drainage_type == 2) {
		fscanf(routing_file,"%d %lf",
		&patch_ID,
		&width);
		patch[i].road_cut_depth = width * tan(patch[i].slope);
		stream = find_patch(patch_ID, patch, rows,cols);
		patch[i].next_stream = stream;
		}*/
	}

	//=======================================================================================================================
	//xu. 2. FIND THE ADDRESS OF EACH NEIGHBOURS
	//=======================================================================================================================



	for (i = 0; i != num_patches; i++) {

		for (int neigh = 0; neigh != patch[i].num_neighbours; neigh++) {
			
			//xu.search for neigh's order and find its address 
			patch_ID = (int)patch[i].neighbours[neigh].patch;
			//it should start form i since it's neighbors will have lower elevations
			for (int patch_inx = i; patch_inx != num_patches; patch_inx++) {
				if (patch[patch_inx].ID == patch_ID) {
					patch[i].neighbours[neigh].patch = &(patch[patch_inx]);//find it address and break
					break;
				}
			}


		}
	}
	//END OF IT

	printf("\nFinishing construct_routing_topology.cpp\n");
	return (num_patches);
}

