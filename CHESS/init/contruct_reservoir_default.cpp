//xu.
#include <stdio.h>

#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include<iostream>

using namespace std;

struct reservoir_default *construct_reservoir_defaults(
	int	nytpes,
	FILE	*default_file,
	struct command_line_object *command_line,
	struct reservoir_default * reservoir_default_object_list)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	void	*alloc(size_t, char *, char *);

	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	char	record[MAXSTR];

	read_record(default_file, record);


	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%d", &(reservoir_default_object_list[i].ID));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(reservoir_default_object_list[i].Vpr));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(reservoir_default_object_list[i].Vem));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%d", &(reservoir_default_object_list[i].Fld_beg));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%d", &(reservoir_default_object_list[i].Fld_end));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(reservoir_default_object_list[i].Qout_max));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(reservoir_default_object_list[i].Qout_min));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(reservoir_default_object_list[i].NDtarget));
	}
	read_record(default_file, record);



	fclose(default_file);
	return(reservoir_default_object_list);
} /*end construct_zone_defaults*/
