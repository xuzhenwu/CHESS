#include <stdio.h>

#include "CHESS.h"
#include <iostream>
using namespace std;

//struct output_hydro_plant construct_output_files(int start_year,int end_year,
void construct_patch_output_files(struct date current_date, struct out_date_range outdate,
	char *outPutPath, struct output_hydro_plant *DM_outfiles,
struct command_line_object *command_line
	)
{
	//define local variables
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
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "DayRain");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "DaySnow");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Rain_thr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Snow_thr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Soil_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Litter_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Canopy_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "CaIntTot");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "CrainInt");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "CsnowInt");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "StreamF");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Baseflow");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "ReturnF");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "PET");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "AET");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Trans");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Evapo");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "S_Evapo");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Can_Evap");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Sur_Evap");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Snowpack");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Sublimat");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "SnowMelt");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "WaterTD");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "SatDefi");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "RootZ_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Unsat_S");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "RootZ_Dr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Unsat_Dr");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "Cap_rise");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "GwDrain");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "GwStorQ");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "GwStorN");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "GwQout");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "GwNout");
		fprintf(DM_outfiles->fPatchDailyHydro, "%9s", "RSMC");
		//fprintf(DM_outfiles->fPatchDailyHydro,"%9s","PercSnow");        
		//fprintf(DM_outfiles->fPatchDailyHydro,"%9s","adC13");    //discrimmination of C13  
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
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "LAI");			   // fprintf(outfile,"%9.3f",alai);         // average LAI
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "PAI");			   // fprintf(outfile,"%9.3f",apai);         // average PAI
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "GPP");			   // fprintf(outfile,"%9.4f",agpp*1000);    // GPP converted from kgC/m2 to gc/m2
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "NPP");			   // fprintf(outfile,"%9.4f",anpp*100);     // NPP converted from kgC/m2 to gc/m2
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "Aresp");		   // fprintf(outfile,"%9.4f",aresp);        // maintance respiration + growth res
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "Arespm");		   // fprintf(outfile,"%9.4f",arespm);       // maintance respiration
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "Arespg");		   // fprintf(outfile,"%9.4f",arespg);       // growth respiration
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "AsoilHr");		   // fprintf(outfile,"%9.4f",asoilhr);      // soil heterotrophic respiration
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "AsoilAr");		   // fprintf(outfile,"%9.4f",asoilar);      // soil autotrophic respiration
		//fprintf(DM_outfiles->fPatchDailyPlant,"%9s","VegC");		   // fprintf(outfile,"%9.4f",(aleafc + awoodc + afrootc)); //total vegetation car
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "LeafC");		   // fprintf(outfile,"%9.4f",aleafc);  //leaf carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "StemC");		   // fprintf(outfile,"%9.4f",awoodc);  //wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "DStemC");		   // fprintf(outfile,"%9.4f",awoodc);  //DEAD wood carbon				
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "RootC");		   // fprintf(outfile,"%9.4f",afrootc); //fine root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "CrootC");		   // fprintf(outfile,"%9.4f",afrootc); //coarse root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "DCrootC");		   // fprintf(outfile,"%9.4f",afrootc); //dead coarse root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "LitrC");		   // fprintf(outfile,"%9.4f",alitrc);  //liter carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "SoilC");		   // fprintf(outfile,"%9.4f",asoilc);  //soil carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "TotalC");		   // fprintf(outfile,"%9.4f",acpool);  //soil carbon
		//fprintf(DM_outfiles->fPatchDailyPlant,"%9s","VegN");		   // fprintf(outfile,"%9.4f",(aleafn + awoodn + afrootn)); //total vegtation nitr
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "LeafN");		   // fprintf(outfile,"%9.4f",aleafn);  //leaf carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "StemN");		   // fprintf(outfile,"%9.4f",awoodn);  //wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "DStemN");		   // fprintf(outfile,"%9.4f",awoodn);  //dead wood carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "RootN");		   // fprintf(outfile,"%9.4f",afrootn); //root carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "CrootN");		   // fprintf(outfile,"%9.4f",afrootc); //coarse root Nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "DCrootN");		   // fprintf(outfile,"%9.4f",afrootc); //dead coarse root Nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "LitrN");		   // fprintf(outfile,"%9.4f",alitrn);  //liter nitrogen					 
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "SoilN");	       // fprintf(outfile,"%9.4f",asoiln);  //soil nitrogen							 
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "TotalN");		   // fprintf(outfile,"%9.4f",anpool);  //total nitrogen??						  
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "Anitrate");	   // fprintf(outfile,"%9.4f",anitrate);       //(kgN/m2) soil mineral N in nitrate form */								
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "AsminN");         // fprintf(outfile,"%9.4f",asminn);         //soil mineral nitrogen
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "AsurfN");		   // fprintf(outfile,"%9.4f",asurfaceN);      //surface no3 + surface nh4
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "AstreamN");	   // fprintf(outfile,"%9.4f",astreamflow_N);  //stream flow nitrate?????
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "Denitrif");	   // fprintf(outfile,"%9.4f",adenitrif);      //annual denitrification
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "Anitrif");	       // fprintf(outfile,"%9.4f",anitrif);        //annual nitrification
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "DOC");			   // fprintf(outfile,"%9.4f",aDOC);
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "DON");		       // fprintf(outfile,"%9.4f",aDON);
		fprintf(DM_outfiles->fPatchDailyPlant, "%9s", "R_Depth");        // fprintf(outfile,"%9.4f",arootdepth);     //annual root zone depth						            //dessolved organic carbon
		fprintf(DM_outfiles->fPatchDailyPlant, "\n");
		//}						 
	}

	if (current_date.year == outdate.last_year && current_date.month == outdate.last_month)
		out_patch_flag = 0;
	// return (DM_outfiles); 		 
}