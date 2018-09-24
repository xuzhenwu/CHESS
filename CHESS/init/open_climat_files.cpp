//------------------------------------------------------------------------------------------------------------------------------
// open daily-step input files
// created by Guoping Tang
//------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "CHESS.h"

#include "Constants.h"

struct	input_Clim_Files open_Clim_Files(char *inClimFile,char *prefix)
{
	//------------------------------------------------------------------------------------------------------------------------------
	//	local variable declarations 							
	//------------------------------------------------------------------------------------------------------------------------------
	char  inPrec[MAXSTR],inTmin[MAXSTR],inTmax[MAXSTR],inCO2[MAXSTR];
    struct input_Clim_Files inClimFiles;
	
	//------------------------------------------------------------------------------------------------------------------------------
	//	Attempt to open the daily clim sequence file for each critical clim parameter and read them in.		
	//------------------------------------------------------------------------------------------------------------------------------
	//strcpy(file_name, file_prefix);
	strcat(inClimFile,prefix);
	strcpy(inPrec,inClimFile);
	strcpy(inTmin,inClimFile);
	strcpy(inTmax,inClimFile);
	strcpy(inCO2,inClimFile);
	strcat(inPrec,"_daily.rain");
	strcat(inTmin,"_daily.tmin");
	strcat(inTmax,"_daily.tmax");
	strcat(inCO2,"_annual.CO2");
	
 	if((inClimFiles.pPrec=fopen(inPrec,"r"))==NULL){
		printf("can not open input precipitation file \n");
		exit (0);
	}
	if((inClimFiles.pTmin=fopen(inTmin,"r"))==NULL){
		printf("can not open input minimum temperature file \n");
		exit (0);
	}

	if((inClimFiles.pTmax=fopen(inTmax,"r"))==NULL){
		printf("can not open input maximum temperature file \n");
		exit (0);
	}

	if((inClimFiles.pCO2=fopen(inCO2,"r"))==NULL){
		printf("can not open input annual CO2 concentration file \n");
		exit (0);
	}

	return(inClimFiles);
} 
