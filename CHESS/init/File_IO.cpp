/*-------------------------------------------------------------------------------------------------------------------------------
NAME:                    FILE_IO.CPP

contains all of the functions for handling file input and output.
------------------------------------------------------------------------------------------------------------------------------*/

#include <malloc.h>
#include <stdio.h> 

#include <stdlib.h> 
#include <string.h>
#include "CHESS.h"
#include "Functions.h"
#include "constants.h"
#include <iostream>

using namespace std;

//===============================================================================================================================
//creating x and y coordinate 
void create_x_coordinates(double *indata, int mr, int mc, double cellsize, double xll) {
	int i, j, r;
	for (i = 0; i<mr; i++) {
		for (j = 0; j<mc; j++) {
			r = i * mc + j;
			indata[r] = xll + (double)j*cellsize;
			//printf("\n longitude %f \n",indata[r]);

		}
	}
	//getchar();
	return;
}

void create_y_coordinates(double *indata, int mr, int mc, double cellsize, double yll) {
	int i, j, r;
	for (i = 0; i<mr; i++) {
		for (j = 0; j<mc; j++) {
			r = i * mc + j;
			indata[r] = yll + (double)(mr - i - 1)*cellsize;
		}
	}
	return;
}


//===============================================================================================================================
//        input_ascii_int() - input an ascii image into an interger array using the (row, col) coordinates maxr and maxc.
//===============================================================================================================================
void input_ascii_int(int *indata, char *filename, int mc, int mr, int arc_flag) {
	FILE *in1;
	int  r;
	int max;
	void    pause();

	max = 0;
	if ((in1 = fopen(filename, "r")) == NULL) {
		printf("cannot open file %s\n", filename);
		pause();
	}
	else {
		/* skip header */
		if (arc_flag == 0)
			for (r = 0; r < LEN_GRASSHEADER; r++)
				fscanf(in1, "%*s");
		else
			for (r = 0; r < LEN_ARCHEADER; r++)
				fscanf(in1, "%*s");

		for (r = 0; r < mr*mc; r++) {
			fscanf(in1, "%d", &indata[r]);
			//printf("\n patchID is:: %d \n",indata[r]);
			if (indata[r] > max)
				max = indata[r];
		}

		printf("\n Max for %s is %d", filename, max);
		fclose(in1);
	}
	return;
}


//===============================================================================================================================
//        input_ascii_float() - input an ascii image into an float array using the (row, col) coordinates maxr and maxc.
//===============================================================================================================================
void input_ascii_float(float *indata, char *filename, int mc, int mr, int arc_flag)
{
	FILE *in1;
	int  r;
	float max;
	void    pause();

	max = 0;
	if ((in1 = fopen(filename, "r")) == NULL) {
		printf("cannot open file %s\n", filename);
		pause();
	}
	else {
		/* skip header */
		if (arc_flag == 0)
			for (r = 0; r < LEN_GRASSHEADER; r++)
				fscanf(in1, "%*s");
		else
			for (r = 0; r < LEN_ARCHEADER; r++)
				fscanf(in1, "%*s");

		for (r = 0; r < mr*mc; r++) {
			fscanf(in1, "%f", &indata[r]);

			indata[r] = (float)(indata[r]);
			//printf("dem is %f \n",indata[r]);
			//getchar();
			if (indata[r] > max)
				max = indata[r];
		}

		printf("\n Max for %s is %f", filename, max);
		fclose(in1);
	}
	return;
}

//===============================================================================================================================
//        input_header() - input information (row, col) from [root].header
//===============================================================================================================================
void input_header(int maxr, int maxc, char *fndem, int arc_flag) {
	FILE  *in1;
	char tmp[100];
	int i;
	void    header_help(int, int, char*);

	if ((in1 = fopen(fndem, "r")) == NULL) {
		fprintf(stderr, "cannot open file %s\n\n", fndem);
		header_help(maxr, maxc, fndem);
	}
	else {
		if (arc_flag == 0) {
			for (i = 0; i< 9; i++)
				fscanf(in1, "%s", tmp);
			fscanf(in1, "%d %*s %d", &maxr, &maxc);
		}
		else
			fscanf(in1, "%*s %d %*s %d", &maxc, &maxr);
		fclose(in1);
	}
	return;
}


//===============================================================================================================================
//        header_help() - inform user they need a [root].header file
//===============================================================================================================================

void header_help(int maxr, int maxc, char *fnhdr) {
	FILE  *in1;

	fprintf(stderr, "\n\n I cannot open the .hdr file to retreive the row and\n");
	fprintf(stderr, " column coordinates. Please enter them now, they will\n");
	fprintf(stderr, " be stored in a new header file called %s.\n\n", fnhdr);
	fprintf(stderr, " Coordinates are (row col) : ");
	scanf("%d %d", &maxr, &maxc);

	if ((in1 = fopen(fnhdr, "w")) == NULL) {
		fprintf(stderr, "I still cannot open file %s, exiting program...\n", fnhdr);
		exit(1);
	}
	else
	{
		fprintf(in1, "%6d %6d", maxr, maxc);
		fclose(in1);
	}

	return;
}


//===============================================================================================================================
//        input_prompt() - input root filename, create full filenames
//===============================================================================================================================
void	read_geo_images(struct patch_object *patch, struct command_line_object *command_line, int rows, int cols, double cellsize, double xll, double yll,
	char *filename, char *prefix, int f_flag, int arc_flag, int num_patches, int*gauge_list,int thread_num) {

	// filenames for each image and file
	char  fnpatch[MAXS], fndem[MAXS], fnslope[MAXS], fnaspect[MAXS], fneast_horizon[MAXS], fnwest_horizon[MAXS];
	char  fnsoil[MAXS], fnveg[MAXS],  fnroads[MAXS], fnstreamorder[MAXS], fnbasins[MAXS],fngauges[MAXS];
	char  fnclimate[MAXS], fnlatitude[MAXS], fnreservoir[MAXS];


	//File pointer
	int          *ppatch;
	float        *pdem;
	float        *pslope;
	float        *paspect;
	float        *peast_horizon;
	float        *pwest_horizon;
	int          *psoil;
	int          *pveg;
	//xu.
	int			*pstreamorder;
	int			*pbasins;
	int			*pgauges;
	int			*pclimate;
	int			*preservoir;

	int          *proads;
	double       *plon;
	double       *plat;//y in cordinate system 
	
	//xu. 
	float		*platitude;//geography latitude


	int i;
	//local functions
	void    header_help(int, int, char*);
	void	input_ascii_int(int *, char *, int, int, int);
	void	input_ascii_float(float *, char *, int, int, int);
	void    create_x_coordinates(double *, int, int, double, double);
	void    create_y_coordinates(double *, int, int, double, double);
	void    pause();

	// copy the root filename into the specific filenames
	strcat(filename, prefix);
	strcpy(fnpatch, filename);
	strcpy(fndem, filename);
	strcpy(fnslope, filename);
	strcpy(fnaspect, filename);
	strcpy(fneast_horizon, filename);
	strcpy(fnwest_horizon, filename);
	strcpy(fnsoil, filename);
	strcpy(fnveg, filename);
	strcpy(fnroads, filename);
	
	//xu.
	strcpy(fnstreamorder, filename);
	strcpy(fnbasins, filename);
	strcpy(fngauges, filename);
	strcpy(fnclimate, filename);
	strcpy(fnlatitude, filename);
	strcpy(fnreservoir, filename);

	// append '.' extensions to each filename (these should be generalized) */
	strcat(fnpatch, ".patch");
	strcat(fndem, ".dem");
	strcat(fnslope, ".slope");
	strcat(fnaspect, ".aspect");
	strcat(fneast_horizon, ".east");
	strcat(fnwest_horizon, ".west");
	strcat(fnsoil, ".soil");
	strcat(fnveg, ".veg");
	strcat(fnroads, ".road");
	
	
	//xu.
	strcat(fnstreamorder, ".streamorder");
	strcat(fnbasins, ".basin");
	
	char char_thread[10];
	_itoa(thread_num, char_thread, 10);
	strcat(fnbasins, char_thread);
	cout << fnbasins << endl;

	strcat(fngauges, ".gauge");
	strcat(fnclimate, ".climate");
	strcat(fnlatitude, ".latitude");
	strcat(fnreservoir, ".reservoir");

	input_header(rows, cols, fndem, arc_flag);

	// allocate and read input map images
	ppatch = (int *)malloc(rows*cols * sizeof(int));
	input_ascii_int(ppatch, fnpatch, rows, cols, arc_flag);

	plon = (double *)malloc(rows*cols * sizeof(double));
	create_x_coordinates(plon, rows, cols, cellsize, xll);

	plat = (double *)malloc(rows*cols * sizeof(double));
	create_y_coordinates(plat, rows, cols, cellsize, yll);

	pdem = (float *)malloc(rows*cols * sizeof(float));
	input_ascii_float(pdem, fndem, rows, cols, arc_flag);

	pslope = (float *)malloc(rows*cols * sizeof(float));
	input_ascii_float(pslope, fnslope, rows, cols, arc_flag);

	paspect = (float *)malloc(rows*cols * sizeof(float));
	input_ascii_float(paspect, fnaspect, rows, cols, arc_flag);

	peast_horizon = (float *)malloc(rows*cols * sizeof(float));
	input_ascii_float(peast_horizon, fneast_horizon, rows, cols, arc_flag);

	pwest_horizon = (float *)malloc(rows*cols * sizeof(float));
	input_ascii_float(pwest_horizon, fnwest_horizon, rows, cols, arc_flag);

	psoil = (int *)malloc(rows*cols * sizeof(int));
	input_ascii_int(psoil, fnsoil, rows, cols, arc_flag);

	pveg = new int[rows*cols]{};
	input_ascii_int(pveg, fnveg, rows, cols, arc_flag);

	proads = new int[rows*cols]{};
	input_ascii_int(proads, fnroads, rows, cols, arc_flag);

	platitude = new float[rows*cols]{};
	input_ascii_float(platitude, fnlatitude, rows, cols, arc_flag);


	//xu.
	//controlled by command_line

	pstreamorder = new int[rows*cols]{};
	if(command_line->cf == TRUE){
		input_ascii_int(pstreamorder, fnstreamorder, rows, cols, arc_flag);
	}
	
	pbasins = new int[rows*cols]{};
	input_ascii_int(pbasins, fnbasins, rows, cols, arc_flag);

	
	pgauges = new int[rows*cols]{};
	if (command_line->gg == TRUE) {
		input_ascii_int(pgauges, fngauges, GAUGE_NUM, 1, arc_flag);
	}

	pclimate = new int[rows*cols]{};
	input_ascii_int(pclimate, fnclimate, rows, cols,  arc_flag);

	preservoir = new int[rows*cols]{};
	if (command_line->re == TRUE) {
		input_ascii_int(preservoir, fnreservoir, rows, cols, arc_flag);
	}




	printf("\n");
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. 1.MATCH PATHES WITH THEIR GEO IMAGIES
	//---------------------------------------------------------------------------------------------------------------------------
	for (int patch_inx = 0; patch_inx < num_patches; patch_inx++) {
		
		for (int imag_inx = 0; imag_inx < cols*rows; imag_inx++) {

			if (patch[patch_inx].ID == ppatch[imag_inx]) {
				patch[patch_inx].ID = ppatch[imag_inx];
				patch[patch_inx].x = plon[imag_inx];
				patch[patch_inx].y = plat[imag_inx];
				patch[patch_inx].z = pdem[imag_inx];
				patch[patch_inx].slope = pslope[imag_inx];
				patch[patch_inx].aspect = paspect[imag_inx];
				patch[patch_inx].e_horizon = peast_horizon[imag_inx];
				patch[patch_inx].w_horizon = pwest_horizon[imag_inx];

				//STREAM ORDER (accociated with _channel.def)
				patch[patch_inx].streamorder = pstreamorder[imag_inx];

				//BASIN(THREAD)
				patch[patch_inx].basins = pbasins[imag_inx];

				//CLIMATE ZONE(accociated with _climate.def)
				patch[patch_inx].climatetype = pclimate[imag_inx];

				//LATITUDE
				patch[patch_inx].latitude = platitude[imag_inx];

				//DOWNSTREAM RESERVOIR ID (accociated with _reservoir.def)
				patch[patch_inx].downslope_reservoir_ID = preservoir[imag_inx];

				patch[patch_inx].road = proads[imag_inx];
				patch[patch_inx].vegtype = pveg[imag_inx];
				patch[patch_inx].soiltype = psoil[imag_inx];
				break;
			}
		}
	}
	

	//---------------------------------------------------------------------------------------------------------------------------
	//xu. 2.MATCH PATHES WITH GAUGE_LISTS
	//---------------------------------------------------------------------------------------------------------------------------
	if (command_line->gg == TRUE) {
		for (int gauge_inx = 0; gauge_inx != GAUGE_NUM; gauge_inx++) {

			gauge_list[gauge_inx] = pgauges[gauge_inx];

			for (int patch_inx = 0; patch_inx != num_patches; patch_inx++) {

				if (patch[patch_inx].ID == gauge_list[gauge_inx]) {	//where the list was replaced with patch_inx
					gauge_list[gauge_inx] = patch_inx;
					break;
				}
			}
		}
	}

	free(ppatch);
	free(pdem);
	free(pslope);
	free(paspect);
	free(peast_horizon);
	free(pwest_horizon);
	free(psoil);
	free(pveg);
	
	
	//xu.
	free(pstreamorder);
	free(pbasins);
	free(pgauges);
	free(pclimate);
	free(platitude);

	free(proads);
	free(plon);
	free(plat);

	printf("\nFinishing reading_geo_images.cpp\n");
	return;
}


//===============================================================================================================================
//        pause() - stop program until user presses enter (return)
//===============================================================================================================================
void pause()
{
	/* clear buffer first */

	while (getchar() != '\n')
		;
	printf("\nPress enter to continue...");
	getchar();
}


