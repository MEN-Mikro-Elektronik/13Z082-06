/*!
 *         \file z82_simp.c
 *       \author thomas.schnuerer@men.de
 *        $Date: 2013/06/28 17:21:50 $
 *    $Revision: 1.4 $
 *
 *       \brief  Simple example program for the Z82 Impulse driver
 *				 needs a Impulse Signal fed in and derives a speed from it
 *
 *     Required: libraries: mdis_api, usr_oss
 *     \switches (none)
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z82_simp.c,v $
 * Revision 1.4  2013/06/28 17:21:50  MRoth
 * R: cosmetics
 * M: changed output from Tacho to Impulse
 *
 * Revision 1.3  2013/06/19 16:27:03  MRoth
 * R: MDIS5 driver required
 * M: driver ported from MDIS4 to MDIS5 (according porting guide rev. 0.9)
 *
 * R: MDIS5 driver required
 * M: driver ported from MDIS4 to MDIS5 (according porting guide rev. 0.9)
 * Revision 1.2  2006/08/01 17:37:33  ts
 * Cosmetics, corrected printf formatting
 *
 * Revision 1.1  2006/08/01 17:20:30  ts
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2003 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

static const char RCSid[]="$Id: z82_simp.c,v 1.4 2013/06/28 17:21:50 MRoth Exp $";

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
static int G_sigCount = 0;

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
    u_int32 inp, out;
	char	*device;

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

