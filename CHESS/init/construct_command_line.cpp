/*--------------------------------------------------------------*/
/* 																*/
/*					construct_command_line						*/
/*																*/
/*	construct_command_line->c - creates command line object		*/
/*																*/
/*	NAME														*/
/*	construct_command_line->c - creates command line object		*/
/*																*/
/*	SYNOPSIS													*/
/*	struct	command_line_object	*construct_command_line(		*/
/*								argc, argv, command_line)		*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	Creates a command_line object which consists of flags		*/
/*	entered on the command line during execution of rhessys.	*/
/*	Some error checking is performed but error checking must	*/
/*	wait until the world object has been specified.				*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	Original code, January 15, 1996.							*/
/*	valid_option to be written still - determines if the next	*/
/*			arguement is a valid option.						*/
/*	added routing option - May 7, 1997, C.Tague					*/
/*																*/
/*																*/
/*	Sep 1997	RAF												*/
/*	Removed extended output option flag as all output is		*/
/*	now of a single format specified by output routines.		*/
/*								*/
/*	Sept 1998	C.Tague					*/
/* 	added comma separated output option			*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <string.h>
#include "Functions.h"
#include "CHESS.h"

struct command_line_object	*construct_command_line(int main_argc,	char **main_argv,command_line_object *command_line)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	int	    valid_option( char * );
	void	*alloc( size_t, char *, char * );
	
	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	int		i;
	
	/*--------------------------------------------------------------*/
	/*	Initialize the options as null				*/
	/*--------------------------------------------------------------*/
	command_line->grow_flag = 0;
	command_line->road_flag = 1;
	command_line->verbose_flag = 0;
	command_line->routing_flag = 1;
	command_line->tec_flag = 0;
	command_line->vmort_flag = 0;
	command_line->gw_flag = 0;
	command_line->b = NULL;
	command_line->p = NULL;
	command_line->gg = NULL;
	command_line->cf = NULL;
	command_line->parallel = NULL;

	
	/*-------------------------------------------------*/
	/*	Loop through each arguement in the command line.*/
	/*-------------------------------------------------*/
	i = 1;
	while  ( i < main_argc){
		/*------------------------------------------*/
		/*		Check if the verbose flag is next.    */
		/*------------------------------------------*/
		if ( i< main_argc ){
			if ( strcmp(main_argv[i],"-v") == 0 ){
				/*-----------------------------------------------*/
				/*			Check if "-v" was the last agruement.   */
				/*-----------------------------------------------*/
				i++;
				if ( i == main_argc ){
					/*------------------------------------------*/
					/* assume we want verbose level 1			  */
					/*------------------------------------------*/
					command_line->verbose_flag= 1;
				}
				else if ( valid_option(main_argv[i]) == 1 ){
					/*----------------------------------------------*/
					/*	check if the next arguement is an option.		*/
					/*----------------------------------------------*/
					command_line->verbose_flag= 1;
				}
				else{
					/*-------------------------------------------------*/
					/*	read in the value of the verbose level.			*/
					/*-------------------------------------------------*/
					command_line->verbose_flag = (int)atoi(main_argv[i]);
					i++;
				}/*end if*/
			}/*end if*/

			/*------------------------------------------*/
			/*Check if the variable mortality flag is next.           */
			/*------------------------------------------*/
			else if ( strcmp(main_argv[i],"-vmort") == 0 ){
				command_line->vmort_flag = 1;
				i++;
			}
			/*------------------------------------------*/
			/*Check if the grow flag is next.           */
			/*------------------------------------------*/
			else if ( strcmp(main_argv[i],"-g") == 0 ){
				command_line->grow_flag = 1;
				i++;
			}
		    /*-------------------------------------------------*/
			/*	groundwater flag and coeffcients	  */
			/*-------------------------------------------------*/
			else if ( strcmp(main_argv[i],"-gw") == 0 ){
				i++;
				command_line->gw_flag = 1;
				if ((i == main_argc-1) || (valid_option(main_argv[i])==1)){
					fprintf(stderr,"FATAL ERROR: Values for gw coefficients not specified\n");
					exit(0);
				} /*end if*/
				/*-------------------------------*/
				/*Read in the loss to gw rate multiplier values		*/
				/*-------------------------------*/
				command_line->sat_to_gw_coeff_mult = (double)atof(main_argv[i]);
				i++;
				command_line->gw_loss_coeff_mult = (double)atof(main_argv[i]);
				i++;
			}/* end if */


			/*--------------------------------------------------------------*/
			/*		Check if the routing option file is next.				*/
			/*--------------------------------------------------------------*/
			else if ( strcmp(main_argv[i],"-r") == 0 ){
				command_line->routing_flag = 1;

				i++;
			} /*end if*/


			/*--------------------------------------------------------------*/
			/*		Check if the tec file is next.							*/
			/*--------------------------------------------------------------*/
			else if( strcmp(main_argv[i],"-t") == 0 ){
				/*--------------------------------------------------------------*/
				/*			Check that the next arguement exists.				*/
				/*--------------------------------------------------------------*/
				command_line->tec_flag = 1;
				i++;
				if ((i == main_argc) || (valid_option(main_argv[i])==1) ){
					fprintf(stderr,"FATAL ERROR: TEC file name not specified\n");
					exit(0);
				} /*end if*/

			} /*end if*/
			/*--------------------------------------------------------------*/
			/*		Check if the basin output flag is next.    				*/
			/*--------------------------------------------------------------*/
			else if( strcmp(main_argv[i],"-b") == 0 ){

				command_line->b = true;
				i++;

			} /*end if*/

			/*--------------------------------------------------------------*/
			/*		Check if the patch output flag is next.  			*/
			/*--------------------------------------------------------------*/
			else if( strcmp(main_argv[i],"-p") == 0 ){
				/*--------------------------------------------------------------*/
				/*			Allocate the patch output specifier.				*/
				/*--------------------------------------------------------------*/
				command_line->p = true;
				i++;
			} /*end if*/
			
			  
			/*--------------------------------------------------------------*/
			/*	xu.	Check if the gauge output flag is next.  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-gg") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			command_line->gg = true;
			i++;
			} /*end if*/
			/*--------------------------------------------------------------*/
			/*	Channel flow genration flag  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-cf") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			command_line->cf = true;
			i++;
			} /*end if*/
			/*--------------------------------------------------------------*/
			/*	Channel flow genration flag  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-parallel") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			command_line->parallel = true;
			i++;
			} /*end if*/
			else{
				fprintf(stderr,
					"FATAL ERROR: in construct_command_line option #%d is invalid.\n",i);
				fprintf(stderr,"for argument %s\n", main_argv[i]);
				exit(0);
			} /*end if*/
		} /*end if*/
	} /*end while*/

	return(command_line);
} /*end construct_command_line*/
