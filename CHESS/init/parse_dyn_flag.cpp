//------------------------------------------------------------------------------------------------------------------------------
// identify the 
//------------------------------------------------------------------------------------------------------------------------------

#include <string.h>
#include <stdio.h>

#include "CHESS.h"
#include "Constants.h"

int	parse_dyn_flag( char *input_string)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//	Local Variable Definition. 							
	//---------------------------------------------------------------------------------------------------------------------------
	int dyn_flag;
	if ((strncmp(input_string,"CONSTANT",8) == 0) || (strncmp(input_string, "constant",8) == 0) )
		dyn_flag = CONSTANT;
	else if ((strncmp(input_string,"WARING",6) == 0) ||  (strncmp(input_string, "waring",6) == 0) )
		dyn_flag = WARING;
	else if ((strncmp(input_string,"DICKENSON",9) == 0) || (strncmp(input_string, "dickenson",9) == 0) )
		dyn_flag = DICKENSON;
	else {
		fprintf(stderr,"\n FATAL ERROR - parse_dyn_flag");
		fprintf(stderr,"\n Flag must be stati or dynamic and is %s" , input_string);
		exit(0);
	}
	
	return(dyn_flag);
}
