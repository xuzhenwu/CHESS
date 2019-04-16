#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"
#include <iostream>
using namespace std;

//struct output_hydro_plant construct_output_files(int start_year,int end_year,
void construct_patch_output_files(struct date current_date, struct out_date_range outdate,
	char *outPutPath,
	int patch_num,
	struct output_hydro_plant *DM_outfiles,
	struct OutArray_object *OutArray,
struct command_line_object *command_line
	)
{
	//define local variables
	const int hydro_num = 36;
	const int plant_num = 36;
	static int i = 0;
	char  buffer[100];
	static int out_patch_flag = 0;

	//define output file
	char  outPDHfile[120]; //Patch daily hydrology
	char  outPDGfile[120]; //Patch daily growth
	//char  outBDHfile[120]; //Basin daily hydrology
	//char  outBDGfile[120]; //Basin daily growth
	//char  outPMHfile[120]; //Patch monthly hydrology
	//char  outPMGfile[120]; //Patch monthly growth
	//char  outBMHfile[120]; //Basin monthly hydrology
	//char  outBMGfile[120]; //Basin monthly growth

	//xu. allocate the pmon array and it is used in out_patch_level_daily
	if(command_line->pmon== true){
		OutArray->HydroMon = new double[patch_num][HYDRO_NUM]{};
		OutArray->PlantMon = new double[patch_num][PLANT_NUM]{};
	}


	if (current_date.year == outdate.first_year && current_date.month == outdate.first_month)
		out_patch_flag = 1;

	if (out_patch_flag == 1 && command_line->p != NULL){
		strcpy(outPDHfile, outPutPath);
		strcpy(outPDGfile, outPutPath);
		//strcpy(outPMHfile,outPutPath);
		//strcpy(outPMGfile,outPutPath);
		_itoa(current_date.year, buffer, 10);

		strcat(outPDHfile, buffer);
		strcat(outPDGfile, buffer);
		//strcat(outPMHfile,buffer);
		//strcat(outPMGfile,buffer);

		strcat(outPDHfile, "_");
		strcat(outPDGfile, "_");
		//strcat(outPMHfile,"_");
		//strcat(outPMGfile,"_");

		_itoa(current_date.month, buffer, 10);

		strcat(outPDHfile, buffer);
		strcat(outPDGfile, buffer);
		//strcat(outPMHfile,buffer);
		//strcat(outPMGfile,buffer);

		strcat(outPDHfile, "_daily_hydro.dat");
		strcat(outPDGfile, "_daily_plant.dat");
		//strcat(outPMHfile,"_monthly_hydro.dat");
		//strcat(outPMGfile,"_monthly_Plant.dat");

		//first to close patch-related files if they already exist
		if (DM_outfiles->fPatchDailyHydro != NULL) fclose(DM_outfiles->fPatchDailyHydro);
		if (DM_outfiles->fPatchDailyPlant != NULL) fclose(DM_outfiles->fPatchDailyPlant);
		//if(DM_outfiles->fPatchMonthlyHydro!=NULL) fclose(DM_outfiles->fPatchMonthlyHydro);
		//if(DM_outfiles->fPatchMonthlyPlant!=NULL) fclose(DM_outfiles->fPatchMonthlyPlant);

		//if(command_line->daily_out_flag==0){ //daily output at the patch level

		DM_outfiles->fPatchDailyHydro = fopen(outPDHfile, "w");
		if (DM_outfiles->fPatchDailyHydro == NULL){
			printf("can not create patch daily hydrology output file! \n");
			exit(0);
		}

		//print header of patch_daily_hydrology
		fprintf(DM_outfiles->fPatchDailyHydro, "%4s", "Year");
		fprintf(DM_outfiles->fPatchDailyHydro, "%4s", "Mon");
		fprintf(DM_outfiles->fPatchDailyHydro, "%4s", "Day");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "ID");
		fprintf(DM_outfiles->fPatchDailyHydro, "%11s", "XX");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "YY");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "DayRain");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "DaySnow");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Rain_thr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Snow_thr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Soil_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Litter_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Canopy_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "CaIntTot");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "CrainInt");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "CsnowInt");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "StreamF");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Baseflow");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "ReturnF");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "PET");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "AET");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Trans");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Evapo");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "S_Evapo");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Can_Evap");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Sur_Evap");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Snowpack");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Sublimat");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "SnowMelt");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "WaterTD");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "SatDefi");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "RootZ_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Unsat_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "RootZ_Dr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Unsat_Dr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "Cap_rise");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "GwDrain");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "GwStorQ");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "GwStorN");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "GwQout");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "GwNout");
		fprintf(DM_outfiles->fPatchDailyHydro, "%12s", "RSMC");
		//fprintf(DM_outfiles->fPatchDailyHydro,"%12s","PercSnow");
		//fprintf(DM_outfiles->fPatchDailyHydro,"%12s","adC13");    //discrimmination of C13
		fprintf(DM_outfiles->fPatchDailyHydro, "\n");

		DM_outfiles->fPatchDailyPlant = fopen(outPDGfile, "w");
		if (DM_outfiles->fPatchDailyPlant == NULL){
			printf("can not create patch daily plant output file! \n");
			exit(0);
		}

		fprintf(DM_outfiles->fPatchDailyPlant, "%4s", "year");           // fprintf(outfile,"%4.0d",current_date.year);
		fprintf(DM_outfiles->fPatchDailyPlant, "%4s", "mon");			   // fprintf(outfile,"%4.0d",current_date.month);
		fprintf(DM_outfiles->fPatchDailyPlant, "%4s", "day");			   // fprintf(outfile,"%4.0d",current_date.day);
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "ID");
		fprintf(DM_outfiles->fPatchDailyPlant, "%11s", "XX");
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "YY");
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "LAI");			   // fprintf(outfile,"%9.3f",alai);         // average LAI
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "PAI");			   // fprintf(outfile,"%9.3f",apai);         // average PAI
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "GPP");			   // fprintf(outfile,"%9.4f",agpp*1000);    // GPP converted from kgC/m2 to gc/m2
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "NPP");			   // fprintf(outfile,"%9.4f",anpp*100);     // NPP converted from kgC/m2 to gc/m2
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "Aresp");		   // fprintf(outfile,"%9.4f",aresp);        // maintance respiration + growth res
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "Arespm");		   // fprintf(outfile,"%9.4f",arespm);       // maintance respiration
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "Arespg");		   // fprintf(outfile,"%9.4f",arespg);       // growth respiration
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "AsoilHr");		   // fprintf(outfile,"%9.4f",asoilhr);      // soil heterotrophic respiration
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "AsoilAr");		   // fprintf(outfile,"%9.4f",asoilar);      // soil autotrophic respiration
		//fprintf(DM_outfiles->fPatchDailyPlant,"%12s","VegC");		   // fprintf(outfile,"%9.4f",(aleafc + awoodc + afrootc)); //total vegetation car
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "LeafC");		   // fprintf(outfile,"%9.4f",aleafc);  //leaf carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "StemC");		   // fprintf(outfile,"%9.4f",awoodc);  //wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "DStemC");		   // fprintf(outfile,"%9.4f",awoodc);  //DEAD wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "RootC");		   // fprintf(outfile,"%9.4f",afrootc); //fine root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "CrootC");		   // fprintf(outfile,"%9.4f",afrootc); //coarse root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "DCrootC");		   // fprintf(outfile,"%9.4f",afrootc); //dead coarse root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "LitrC");		   // fprintf(outfile,"%9.4f",alitrc);  //liter carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "SoilC");		   // fprintf(outfile,"%9.4f",asoilc);  //soil carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "TotalC");		   // fprintf(outfile,"%9.4f",acpool);  //soil carbon
		//fprintf(DM_outfiles->fPatchDailyPlant,"%12s","VegN");		   // fprintf(outfile,"%9.4f",(aleafn + awoodn + afrootn)); //total vegtation nitr
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "LeafN");		   // fprintf(outfile,"%9.4f",aleafn);  //leaf carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "StemN");		   // fprintf(outfile,"%9.4f",awoodn);  //wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "DStemN");		   // fprintf(outfile,"%9.4f",awoodn);  //dead wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "RootN");		   // fprintf(outfile,"%9.4f",afrootn); //root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "CrootN");		   // fprintf(outfile,"%9.4f",afrootc); //coarse root Nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "DCrootN");		   // fprintf(outfile,"%9.4f",afrootc); //dead coarse root Nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "LitrN");		   // fprintf(outfile,"%9.4f",alitrn);  //liter nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "SoilN");	       // fprintf(outfile,"%9.4f",asoiln);  //soil nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "TotalN");		   // fprintf(outfile,"%9.4f",anpool);  //total nitrogen??
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "Anitrate");	   // fprintf(outfile,"%9.4f",anitrate);       //(kgN/m2) soil mineral N in nitrate form */
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "AsminN");         // fprintf(outfile,"%9.4f",asminn);         //soil mineral nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "AsurfN");		   // fprintf(outfile,"%9.4f",asurfaceN);      //surface no3 + surface nh4
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "AstreamN");	   // fprintf(outfile,"%9.4f",astreamflow_N);  //stream flow nitrate?????
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "Denitrif");	   // fprintf(outfile,"%9.4f",adenitrif);      //annual denitrification
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "Anitrif");	       // fprintf(outfile,"%9.4f",anitrif);        //annual nitrification
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "DOC");			   // fprintf(outfile,"%9.4f",aDOC);
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "DON");		       // fprintf(outfile,"%9.4f",aDON);
		fprintf(DM_outfiles->fPatchDailyPlant, "%12s", "R_Depth");        // fprintf(outfile,"%9.4f",arootdepth);     //annual root zone depth						            //dessolved organic carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "\n");
		//}
	}

	if (current_date.year == outdate.last_year && current_date.month == outdate.last_month)
		out_patch_flag = 0;
	// return (DM_outfiles);
}