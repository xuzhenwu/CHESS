/*--------------------------------------------------------------*/
/* 																*/
/*					construct_daily_clim	 					*/
/*																*/
/*	construct_daily_clim.c - makes a daily clim object			*/
/*																*/
/*	NAME														*/
/*	construct_daily_clim.c - makes a daily clim object			*/
/*																*/
/*	SYNOPSIS													*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	Allocates a climate object.		  							*/
/*	Computes the number of entries based on start and end dates.*/
/*	Allocates an array of critical clim data for n entries.		*/
/*																*/
/*  A clim object is simply a list of doubleing point values     */
/*  however, by checking the index in the file containgin the   */
/*  list, the code insures that the list matches the number of  */
/*  dayss in the simulation and extends from start to end 		*/
/*	julian day.													*/
/*                                                              */
/*  PROGRAMMER NOTES                                            */
/*                                                              */
/*  A clim sequence file should be named with a prefix          */
/*  matching that in its base station file.  It will be         */
/*  ignored if its suffix is not in the list of parameters      */
/*  required at that time step.                                 */
/*                                                              */
/*  The clim_sequence is constructed by calling construct_      */
/*  clim_sequence.  Refer to that code for documentation of     */
/*  what a valid clim sequence is.                              */
/*                                                              */
/*  Feb 28/95 - Richard Fernandes				*/
/*	Took out warnings that optional clim squences were not	*/
/*	found (this should error fatally instead).		*/
/*	It will now do so in the contruct_clim_sequence code	*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <string.h>
#include "CHESS.h"

void	rewind_daily_clim(struct input_Clim_Files inClimFiles)
{
	/*--------------------------------------------------------------*/
	/*	rewind file pointer to the beggning of input files             .					*/
	/*--------------------------------------------------------------*/
	rewind(inClimFiles.pPrec);    //rewind precipitaiton
	rewind(inClimFiles.pTmax);	  //rewind maximum temperature data
	rewind(inClimFiles.pTmin);    //rewind minimum temperature data
	rewind(inClimFiles.pCO2);     //rewind annual atmospheric CO2 data
	return;
} /*end construct_daily_clim*/
