/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  z82_doc.c
 *
 *      \author  thomas.schnuerer@men.de
 *        $Date: 2013/06/28 17:21:48 $
 *    $Revision: 1.3 $
 *
 *      \brief   User documentation for the MDIS5 Z82 Impulse driver
 *
 *     Required: -
 *
 *     \switches -
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z82_doc.c,v $
 * Revision 1.3  2013/06/28 17:21:48  MRoth
 * R: cosmetics
 * M: fixed user documentation
 *
 * Revision 1.2  2013/05/22 16:37:35  ts
 * R: documentation was not adapted to Z82
 * M: updated documentation to current core
 *
 * Revision 1.1  2006/08/01 17:20:19  ts
 * Initial Revision
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2013 by MEN Mikro Elektronik GmbH, Nuremberg, Germany
 ****************************************************************************/

/*! \mainpage
    This is the documentation of the MDIS low-level driver for the 16Z082
    impulse counter IP core.

    The 16Z082_IMPULSE module can be used to count the impulses from e.g. wheel rotation sensors. 
    To be able to support a wide range of wheel speeds the debouncer is programmable.

    \n
    \section Variants Variants
    (no variants)

    \n \section FuncDesc Functional Description

    \n \subsection General General

	The IP core acts as an impulse counter with programmable debounce capability to
    adapt rotation sensors etc. to their signal source. The possible debounce values
    are described below.

    \n \subsection readwrite Reading and writing, Get- and SetStats

	\n Z82_CLR_RESULT    SetStat, clears the RESULT register to 0
	\n                   This should be done as initial application step.
 
	\n Z82_GET_RESULT    GetStat, gets the current value of RESULT register.
	\n                   A count value represents one pulse of the input.

	\n Z82_GET_OVF       GetStat, gets the RESULT registers overflow frag.
	\n                   The overflow flag should be read after the RESULT register
	\n                   or if the RESULT value increases near the unsigned 32-bit
    \n                   value limit (2^32 or 4294967296 impulses).

	\n Z82_DEBOUNCE      Get/SetStat, gets/sets the debounce register value. The
    \n                   debounce time is a value of 0-1023.

    \n \subsection interrupts Interrupt handling and signals
    \n The driver supports no interrupts as the IP core is a counter only
*/

/** \example z82_simp.c
Simple example for driver usage
*/

/*! \page dummy
  \menimages
*/
