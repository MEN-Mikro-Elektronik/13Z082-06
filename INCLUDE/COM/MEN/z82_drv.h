/***********************  I n c l u d e  -  F i l e  ***********************/
/*!
 *        \file  z82_drv.h
 *
 *      \author  ts@men.de
 *        $Date: 2013/06/19 16:27:05 $
 *    $Revision: 3.2 $
 *
 *       \brief  Header file for Z82 driver containing
 *               Z82 specific status codes and Z82 function prototypes
 *
 *    \switches  _ONE_NAMESPACE_PER_DRIVER_
 *               _LL_DRV_
 */
 /*-------------------------------[ History ]--------------------------------
 *
 * $Log: z82_drv.h,v $
 * Revision 3.2  2013/06/19 16:27:05  MRoth
 * R: MDIS5 driver required
 * M: driver ported from MDIS4 to MDIS5 (according porting guide rev. 0.9)
 *
 * Revision 3.1  2006/08/01 17:12:24  ts
 * Initial Revision
 *
 *
 *---------------------------------------------------------------------------
 * (c) Copyright 2004 by MEN mikro elektronik GmbH, Nuernberg, Germany
 ****************************************************************************/

#ifndef _Z82_DRV_H
#define _Z82_DRV_H

#ifdef __cplusplus
      extern "C" {
#endif


/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/* none */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name Z82 specific Getstat/Setstat standard codes 
 *  \anchor getstat_setstat_codes
 */
/**@{*/
#define Z82_CLR_RESULT  M_DEV_OF+0x00   /**<   S: Set RESULT Register 0 	*/
#define Z82_GET_RESULT  M_DEV_OF+0x01   /**< G  : Get recent RESULT Val.	*/
#define Z82_GET_OVF     M_DEV_OF+0x02   /**< G  : Get OVERFLOW Flag			*/
#define Z82_DEBOUNCE    M_DEV_OF+0x05   /**< G,S: Get/set debouncing value  */
/**@}*/

#ifndef  Z82_VARIANT
# define Z82_VARIANT       Z82
#endif

# define _Z82_GLOBNAME(var,name) var##_##name
#ifndef _ONE_NAMESPACE_PER_DRIVER_
# define Z82_GLOBNAME(var,name) _Z82_GLOBNAME(var,name)
#else
# define Z82_GLOBNAME(var,name) _Z82_GLOBNAME(Z82,name)
#endif

#define __Z82_GetEntry          Z82_GLOBNAME(Z82_VARIANT, GetEntry)

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
#ifdef _LL_DRV_
#ifndef _ONE_NAMESPACE_PER_DRIVER_
	extern void __Z82_GetEntry(LL_ENTRY* drvP);
#endif
#endif /* _LL_DRV_ */

/*-----------------------------------------+
|  BACKWARD COMPATIBILITY TO MDIS4         |
+-----------------------------------------*/
#ifndef U_INT32_OR_64
 /* we have an MDIS4 men_types.h and mdis_api.h included */
 /* only 32bit compatibility needed!                     */
 #define INT32_OR_64  int32
 #define U_INT32_OR_64 u_int32
 typedef INT32_OR_64  MDIS_PATH;
#endif /* U_INT32_OR_64 */


#ifdef __cplusplus
      }
#endif

#endif /* _Z82_DRV_H */

