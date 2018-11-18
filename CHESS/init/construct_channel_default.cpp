//xu.
#include <stdio.h>

#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include<iostream>

using namespace std;

struct channel_default *construct_channel_defaults(
	int	nytpes,
	FILE	*default_file,
struct command_line_object *command_line,
struct channel_default * channel_default_object_list)
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


	for (int i = 0; i<nytpes; i++) {
		fscanf(default_file, "%d", &(channel_default_object_list[i].stream_order));
	} 
	read_record(default_file, record);

	for (int i = 0; i<nytpes; i++) {
		fscanf(default_file, "%lf", &(channel_default_object_list[i].channel_bottom_width));
	}
	read_record(default_file, record);

	for (int i = 0; i < nytpes; i++) {
		fscanf(default_file, "%lf", &(channel_default_object_list[i].inverse_side_slope));
	}
	read_record(default_file, record);

	for (int i = 0; i<nytpes; i++) {
		fscanf(default_file, "%lf", &(channel_default_object_list[i].hydraulic_roughness));
	}
	read_record(default_file, record);

	fclose(default_file);
	return(channel_default_object_list);
} /*end construct_zone_defaults*/
