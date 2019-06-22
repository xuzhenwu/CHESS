#include <stdio.h>

#include "CHESS.h"
#include "Functions.h"
#include "Constants.h"
#include<iostream>
#include<fstream>

using namespace std;

void construct_resvoir_correponding_data(char *filename,int patchID, struct patch_object *patch) {

	//local var
	int yeartmp;
	int year_inx;

	//1 alpha beta gamma

	//init array
	int time_length = patch->channel->reservoir->EndYear - patch->channel->reservoir->StartYear + 1;
	patch->channel->reservoir->alpha = new double[time_length];
	patch->channel->reservoir->beta = new double[time_length];
	patch->channel->reservoir->gamma = new double[time_length];

	//file name 
	char inabgname[120]{};
	char idchar[20]{};
	strcpy(inabgname, filename);
	strcat(inabgname, "re//");
	_itoa(patchID, idchar, 10);
	strcat(inabgname, idchar);
	strcat(inabgname, "_alpha_beta_gamma.txt");

	ifstream ABG(inabgname);
	while (ABG) {

		for(int inx=1;inx<=time_length;inx++){
			ABG >> yeartmp;
			year_inx = yeartmp - patch->channel->reservoir->StartYear;//start from zero
			ABG >> patch->channel->reservoir->alpha[year_inx] >> patch->channel->reservoir->beta[year_inx]
				>> patch->channel->reservoir->gamma[year_inx];
		}

	}
	ABG.close();


	//2 i30

	//file name 
	char ini30name[120]{};
	strcpy(ini30name, filename);
	strcat(ini30name, "re//");
	strcat(ini30name, idchar);
	strcat(ini30name, "_long_term_i30.txt");

	ifstream I30(ini30name);
	while (I30) {
		for (int inx = 0; inx < 366; inx++) {
			I30 >> yeartmp;//skip first 
			I30 >> patch->channel->reservoir->longterm_i30[inx];
		}
	}
	I30.close();

	//3 qout

	//file name 
	char inqoutname[120]{};
	strcpy(inqoutname, filename);
	strcat(inqoutname, "re//");
	strcat(inqoutname, idchar);
	strcat(inqoutname, "_long_term_qout.txt");

	ifstream QOUT(inqoutname);
	while (QOUT) {
		for (int inx = 0; inx < 366; inx++) {
			QOUT >> yeartmp;//skip first 
			QOUT >> patch->channel->reservoir->longterm_qout[inx];
		}
	}
	QOUT.close();


}//END OF FUNCTION