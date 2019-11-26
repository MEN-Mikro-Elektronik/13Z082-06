/*!
 *         \file z82_simp.c
 *       \author thomas.schnuerer@men.de
 *
 *       \brief  Simple example program for the Z82 Impulse driver
 *				 needs a Impulse Signal fed in and derives a speed from it
 *
 *     Required: libraries: mdis_api, usr_oss
 *     \switches (none)
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright 2003-2019, MEN Mikro Elektronik GmbH
 ****************************************************************************/
/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/z82_drv.h>

/*--------------------------------------+
|   DEFINES                             |
+--------------------------------------*/
/* none */

/*--------------------------------------+
|   TYPDEFS                             |
+--------------------------------------*/
/* none */

/*--------------------------------------+
|   EXTERNALS                           |
+--------------------------------------*/
/* none */

/*--------------------------------------+
|   GLOBALS                             |
+--------------------------------------*/

/*--------------------------------------+
|   PROTOTYPES                          |
+--------------------------------------*/
static void PrintError(char *info);

/********************************* main ************************************/
/** Program main function
 *
 *  \param argc       \IN  argument counter
 *  \param argv       \IN  argument vector
 *
 *  \return	          success (0) or error (1)
 */
int main(int argc, char *argv[])
{
	MDIS_PATH	path;
    u_int32 out=0;
	int32 inp=0;
	char	*device=NULL;

	if (argc < 2 || strcmp(argv[1],"-?")==0) {
		printf("Syntax: z82_simp <device>\n");
		printf("Function: Example program using the Z82 Impulse driver\n");
		printf("Options:\n");
		printf("    device       device name\n");
		printf("\n");
		return(1);
	}

	device = argv[1];

	/*--------------------+
    |  open path          |
    +--------------------*/
	if ((path = M_open(device)) < 0) {
		PrintError("open");
		return(1);
	}

	/*--------------------+
    |  config             |
    +--------------------*/

	/* noting to do in that unit */

	/*--------------------+
    |  output values      |
    +--------------------*/

	printf( "Reading impulse counter values:\n");

    for( out=0; out<32; ++out ) {

        M_read( path, &inp );
        printf( "%d: RESULT = %ld\n", out, inp );
        UOS_Delay( 1000 );
    }


	/*--------------------+
    |  cleanup            |
    +--------------------*/
	if (M_close(path) < 0)
		PrintError("close");

	return(0);
}

/********************************* PrintError ******************************/
/** Print MDIS error message
 *
 *  \param info       \IN  info string
*/
static void PrintError(char *info)
{
	printf("*** can't %s: %s\n", info, M_errstring(UOS_ErrnoGet()));
}

