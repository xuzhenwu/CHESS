//------------------------------------------------------------------------------------------------------------------------------
// Identify the phenological type of vegetation
//------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>

#include <string.h>
#include "CHESS.h"
#include "Constants.h"

int	parse_phenology_type( char *input_string)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//	Local Variable Definition. 						
	//---------------------------------------------------------------------------------------------------------------------------
	int veg_type;

	if ((strncmp(input_string,"DECIDUOUS",5) == 0) ||  (strncmp(input_string,"deciduous",5) == 0) )
		veg_type = DECID;
	else if ((strncmp(input_string,"EVERGREEN",9) == 0) || 	(strncmp(input_string,"evergreen",9) == 0) )
		veg_type = EVERGREEN;
	else {
		fprintf(stderr,"\n FATAL ERROR - parse_phenology_type");
		fprintf(stderr,"\n Unknown phenology type %s in stratum default file", input_string);
		fprintf(stderr,"\n Must be either DECID or EVERGREEN");
		exit(0);
		}
	return(veg_type);
}
