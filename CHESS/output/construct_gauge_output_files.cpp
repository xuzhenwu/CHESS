#include <stdio.h>

#include "CHESS.h"
#include <iostream>
using namespace std;

//struct output_hydro_plant construct_output_files(int start_year,int end_year,
void construct_gauge_output_files(char *outPutPath, struct output_hydro_plant *DM_outfiles,
	struct command_line_object *command_line,
	int *gauge_list
)
{
	//define local variables
	static int i = 0;

	//define output file
	//char  outPDHfile[120]; //Patch daily hydrology
	//char  outPDGfile[120]; //Patch daily growth
	char  outGDHfile[gauge_num][120]; //Basin daily hydrology
	char  outGDGfile[gauge_num][120]; //Basin daily growth    
	char  gauge_name[100];
	//char  outPMHfile[120]; //Patch monthly hydrology
	//char  outPMGfile[120]; //Patch monthly growth
	//char  outBMHfile[120]; //Basin monthly hydrology
	//char  outBMGfile[120]; //Basin monthly growth 

	for (int gauge_inx = 0; gauge_inx != gauge_num; gauge_inx++) {
		if (command_line->b != NULL) { //basin level output
			strcpy(outGDHfile[gauge_inx], outPutPath);
			strcpy(outGDGfile[gauge_inx], outPutPath);
			//strcpy(outBMHfile,outPutPath);
			//strcpy(outBMGfile,outPutPath);
			strcat(outGDHfile[gauge_inx], "Gauge_daily_hydro_");
			strcat(outGDGfile[gauge_inx], "Gauge_daily_plant_");

			_itoa(gauge_list[gauge_inx], gauge_name, 10);

			strcat(outGDHfile[gauge_inx], gauge_name);
			strcat(outGDGfile[gauge_inx], gauge_name);

			strcat(outGDHfile[gauge_inx], ".dat");
			strcat(outGDGfile[gauge_inx], ".dat");



			//if(command_line->daily_out_flag==0){ //for daily output question: need resolve commandline here
			DM_outfiles->fGaugeDailyHydro[gauge_inx] = fopen(outGDHfile[gauge_inx], "w");
			if (DM_outfiles->fGaugeDailyHydro[gauge_inx] == NULL) {
				printf("can not create basin daily hydrology output file! \n");
				exit(0);
			}

			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%4s", "Year");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%4s", "Mon");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%4s", "Day");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "DayRain");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "DaySnow");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Rain_thr");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Snow_thr");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Soil_S");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Litter_S");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Canopy_S");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "CaIntTot");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "CrainInt");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "CsnowInt");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "StreamF");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Baseflow");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "ReturnF");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "PET");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "AET");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Trans");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Evapo");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "S_Evapo");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Can_Evap");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Sur_Evap");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Snowpack");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Sublimat");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "SnowMelt");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "WaterTD");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "SatDefi");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "RootZ_S");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Unsat_S");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "RootZ_Dr");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Unsat_Dr");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "Cap_rise");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "GwDrain");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "GwStorQ");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "GwStorN");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "GwQout");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "GwNout");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "%9s", "RSMC");
			fprintf(DM_outfiles->fGaugeDailyHydro[gauge_inx], "\n");

			// for plant growth output
			DM_outfiles->fGaugeDailyPlant[gauge_inx] = fopen(outGDGfile[gauge_inx], "w");
			if (DM_outfiles->fGaugeDailyPlant[gauge_inx] == NULL) {
				printf("can not create basin daily plant output file! \n");
				exit(0);
			}

			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%4s", "year");           // fprintf(outfile,"%4.0d",current_date.year);		
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%4s", "mon");			   // fprintf(outfile,"%4.0d",current_date.month);
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%4s", "day");			   // fprintf(outfile,"%4.0d",current_date.day);
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "LAI");			   // fprintf(outfile,"%9.3f",alai);         // average LAI
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "PAI");			   // fprintf(outfile,"%9.3f",apai);         // average PAI
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "GPP");			   // fprintf(outfile,"%9.4f",agpp*1000);    // GPP converted from kgC/m2 to gc/m2
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "NPP");			   // fprintf(outfile,"%9.4f",anpp*100);     // NPP converted from kgC/m2 to gc/m2
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "Aresp");		   // fprintf(outfile,"%9.4f",aresp);        // maintance respiration + growth res
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "Arespm");		   // fprintf(outfile,"%9.4f",arespm);       // maintance respiration
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "Arespg");		   // fprintf(outfile,"%9.4f",arespg);       // growth respiration
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "AsoilHr");		   // fprintf(outfile,"%9.4f",asoilhr);      // soil heterotrophic respiration
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "AsoilAr");		   // fprintf(outfile,"%9.4f",asoilar);      // soil autotrophic respiration
			//fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx],"%9s","VegC");		   // fprintf(outfile,"%9.4f",(aleafc + awoodc + afrootc)); //total vegetation car
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "LeafC");		   // fprintf(outfile,"%9.4f",aleafc);  //leaf carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "StemC");		   // fprintf(outfile,"%9.4f",awoodc);  //wood carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "DStemC");		   // fprintf(outfile,"%9.4f",awoodc);  //DEAD wood carbon				
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "FrootC");		   // fprintf(outfile,"%9.4f",afrootc); //fine root carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "CrootC");		   // fprintf(outfile,"%9.4f",afrootc); //coarse root carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "DCrootC");		   // fprintf(outfile,"%9.4f",afrootc); //dead coarse root carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "LitrC");		   // fprintf(outfile,"%9.4f",alitrc);  //liter carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "SoilC");		   // fprintf(outfile,"%9.4f",asoilc);  //soil carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "TotalC");		   // fprintf(outfile,"%9.4f",acpool);  //soil carbon
			//fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx],"%9s","VegN");		   // fprintf(outfile,"%9.4f",(aleafn + awoodn + afrootn)); //total vegtation nitr
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "LeafN");		   // fprintf(outfile,"%9.4f",aleafn);  //leaf Nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "StemN");		   // fprintf(outfile,"%9.4f",awoodn);  //wood Nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "DStemN");		   // fprintf(outfile,"%9.4f",awoodn);  //dead wood Nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "FrootN");		   // fprintf(outfile,"%9.4f",afrootn); //Fine root Nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "CrootN");		   // fprintf(outfile,"%9.4f",afrootc); //coarse root Nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "DCrootN");		   // fprintf(outfile,"%9.4f",afrootc); //dead coarse root Nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "LitrN");		   // fprintf(outfile,"%9.4f",alitrn);  //liter nitrogen					 
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "SoilN");	       // fprintf(outfile,"%9.4f",asoiln);  //soil nitrogen							 
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "TotalN");		   // fprintf(outfile,"%9.4f",anpool);  //total nitrogen??						  
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "Anitrate");	   // fprintf(outfile,"%9.4f",anitrate);       //(kgN/m2) soil mineral N in nitrate form */								
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "AsminN");         // fprintf(outfile,"%9.4f",asminn);         //soil mineral nitrogen
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "AsurfN");		   // fprintf(outfile,"%9.4f",asurfaceN);      //surface no3 + surface nh4
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "AstreamN");	   // fprintf(outfile,"%9.4f",astreamflow_N);  //stream flow nitrate?????
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "Denitrif");	   // fprintf(outfile,"%9.4f",adenitrif);      //annual denitrification
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "Anitrif");	       // fprintf(outfile,"%9.4f",anitrif);        //annual nitrification
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "DOC");			   // fprintf(outfile,"%9.4f",aDOC);
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "DON");		       // fprintf(outfile,"%9.4f",aDON);
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "%9s", "R_Depth");        // fprintf(outfile,"%9.4f",arootdepth);     //annual root zone depth						            //dessolved organic carbon
			fprintf(DM_outfiles->fGaugeDailyPlant[gauge_inx], "\n");
		}
	}
	//}
}