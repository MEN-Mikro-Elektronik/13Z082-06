/*********************  P r o g r a m  -  M o d u l e ***********************/
/*!
 *        \file  z82_drv.c
 *
 *      \author  thomas.schnuerer@men.de
 *        $Date: 2013/06/28 17:21:46 $
 *    $Revision: 1.6 $
 *
 *      \brief   Low-level driver for Z82 Impulse controller
 *
 *     Required: OSS, DESC, DBG libraries
 *
 *     \switches _ONE_NAMESPACE_PER_DRIVER_
 */
 /*
 *---------------------------------------------------------------------------
 * Copyright (c) 2006-2019, MEN Mikro Elektronik GmbH
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

#define _NO_LL_HANDLE		/* ll_defs.h: don't define LL_HANDLE struct */

#include <MEN/men_typs.h>   /* system dependent definitions   */
#include <MEN/maccess.h>    /* hw access macros and types     */
#include <MEN/dbg.h>        /* debug functions                */
#include <MEN/oss.h>        /* oss functions                  */
#include <MEN/desc.h>       /* descriptor functions           */
#include <MEN/modcom.h>     /* ID PROM functions              */
#include <MEN/mdis_api.h>   /* MDIS global defs               */
#include <MEN/mdis_com.h>   /* MDIS common defs               */
#include <MEN/mdis_err.h>   /* MDIS error codes               */
#include <MEN/ll_defs.h>    /* low-level driver definitions   */

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/* general defines */
#define CH_NUMBER			1			/**< number of device channels 		*/
#define USE_IRQ				FALSE		/**< interrupt required  			*/
#define ADDRSPACE_COUNT		1			/**< nbr of required address spaces */
#define ADDRSPACE_SIZE		0x1c		/**< addr space size: standard		*/

/* debug defines */
#define DBG_MYLEVEL			llHdl->dbgLevel   /**< debug level */
#define DBH					llHdl->dbgHdl     /**< debug handle */
#define OSH                 llHdl->osHdl

/* Registers */
#define Z82_REG_RESULT			0x00     /**< count Register 				*/
#define Z82_REG_STATUS			0x04     /**< Status (Overflow) Register 	*/
#define Z82_REG_DEBOUNCER		0x08     /**< Debouncer Register 			*/
#define Z82_DEBOUNCE_MASK		0x7ff    /**< Mask bits 10..0 from Debounce */
#define Z82_DEBOUNCE_DEFAULT 	32 	 	/**< Debounce default: 32			*/

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/** low-level handle */
typedef struct {
	/* general */
    int32           memAlloc;		/**< size allocated for the handle */
    OSS_HANDLE      *osHdl;         /**< oss handle */
    OSS_IRQ_HANDLE  *irqHdl;        /**< irq handle */
    DESC_HANDLE     *descHdl;       /**< desc handle */
    MACCESS         ma;             /**< hw access handle */
	MDIS_IDENT_FUNCT_TBL idFuncTbl;	/**< id function table */
	/* debug */
    u_int32         dbgLevel;		/**< debug level */
	DBG_HANDLE      *dbgHdl;        /**< debug handle */
	/* misc */
	u_int32			nDebounce;		/**< debouncer value */
} LL_HANDLE;

/* include files which need LL_HANDLE */
#include <MEN/ll_entry.h>   /* low-level driver jump table  */
#include <MEN/z82_drv.h>    /* Z82 driver header file */

/*-----------------------------------------+
|  PROTOTYPES                              |
+-----------------------------------------*/
static int32 Z82_Init(DESC_SPEC *descSpec, OSS_HANDLE *osHdl,
					   MACCESS *ma, OSS_SEM_HANDLE *devSemHdl,
					   OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP);
static int32 Z82_Exit(LL_HANDLE **llHdlP );
static int32 Z82_Read(LL_HANDLE *llHdl, int32 ch, int32 *value);
static int32 Z82_Write(LL_HANDLE *llHdl, int32 ch, int32 value);
static int32 Z82_SetStat(LL_HANDLE *llHdl,int32 ch, int32 code, INT32_OR_64 value32_or_64);
static int32 Z82_GetStat(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P);
static int32 Z82_BlockRead(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							int32 *nbrRdBytesP);
static int32 Z82_BlockWrite(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size,
							 int32 *nbrWrBytesP);
static int32 Z82_Irq(LL_HANDLE *llHdl );
static int32 Z82_Info(int32 infoType, ... );

static char* Ident( void );
static int32 Cleanup(LL_HANDLE *llHdl, int32 retCode);

/****************************** Z82_GetEntry ********************************/
/** Initialize driver's jump table
 *
 *  \param drvP     \OUT pointer to the initialized jump table structure
 */
#ifdef _ONE_NAMESPACE_PER_DRIVER_
    extern void LL_GetEntry( LL_ENTRY* drvP )
#else
    extern void __Z82_GetEntry( LL_ENTRY* drvP )
#endif
{
    drvP->init        = Z82_Init;
    drvP->exit        = Z82_Exit;
    drvP->read        = Z82_Read;
    drvP->write       = Z82_Write;
    drvP->blockRead   = Z82_BlockRead;
    drvP->blockWrite  = Z82_BlockWrite;
    drvP->setStat     = Z82_SetStat;
    drvP->getStat     = Z82_GetStat;
    drvP->irq         = Z82_Irq;
    drvP->info        = Z82_Info;
}

/******************************** Z82_Init **********************************/
/** Allocate and return low-level handle, initialize hardware
 *
 * The function initializes all channels with the definitions made
 * in the descriptor. The interrupt is disabled.
 *
 * The following descriptor keys are used:
 *
 * \code
 * Descriptor key        Default          Range
 * --------------------  ---------------  -------------
 * DEBUG_LEVEL_DESC      OSS_DBG_DEFAULT  see dbg.h
 * DEBUG_LEVEL           OSS_DBG_DEFAULT  see dbg.h
 * ID_CHECK              1                0..1
 * \endcode
 *
 *  \param descP      \IN  pointer to descriptor data
 *  \param osHdl      \IN  oss handle
 *  \param ma         \IN  hw access handle
 *  \param devSemHdl  \IN  device semaphore handle
 *  \param irqHdl     \IN  irq handle
 *  \param llHdlP     \OUT pointer to low-level driver handle
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z82_Init(
    DESC_SPEC       *descP,
    OSS_HANDLE      *osHdl,
    MACCESS         *ma,
    OSS_SEM_HANDLE  *devSemHdl,
    OSS_IRQ_HANDLE  *irqHdl,
    LL_HANDLE       **llHdlP
)
{
    LL_HANDLE *llHdl = NULL;
    u_int32 gotsize;
    int32 error;
    u_int32 value;

    /*------------------------------+
    |  prepare the handle           |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */

	/* alloc */
    if ((llHdl = (LL_HANDLE*)OSS_MemGet(
    				osHdl, sizeof(LL_HANDLE), &gotsize)) == NULL)
       return(ERR_OSS_MEM_ALLOC);

	/* clear */
    OSS_MemFill(osHdl, gotsize, (char*)llHdl, 0x00);

	/* init */
    llHdl->memAlloc   = gotsize;
    llHdl->osHdl      = osHdl;
    llHdl->irqHdl     = irqHdl;
    llHdl->ma		  = *ma;

    /*------------------------------+
    |  init id function table       |
    +------------------------------*/
	/* driver's ident function */
	llHdl->idFuncTbl.idCall[0].identCall = Ident;
	/* library's ident functions */
	llHdl->idFuncTbl.idCall[1].identCall = DESC_Ident;
	llHdl->idFuncTbl.idCall[2].identCall = OSS_Ident;
	/* terminator */
	llHdl->idFuncTbl.idCall[3].identCall = NULL;

    /*------------------------------+
    |  prepare debugging            |
    +------------------------------*/
	DBG_MYLEVEL = OSS_DBG_DEFAULT;	/* set OS specific debug level */
	DBGINIT((NULL,&DBH));

    /*------------------------------+
    |  scan descriptor              |
    +------------------------------*/
	/* prepare access */
    if ((error = DESC_Init(descP, osHdl, &llHdl->descHdl)))
		return( Cleanup(llHdl,error) );

    /* DEBUG_LEVEL_DESC */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&value, "DEBUG_LEVEL_DESC")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

	DESC_DbgLevelSet(llHdl->descHdl, value);	/* set level */

    /* DEBUG_LEVEL */
    if ((error = DESC_GetUInt32(llHdl->descHdl, OSS_DBG_DEFAULT,
								&llHdl->dbgLevel, "DEBUG_LEVEL")) &&
		error != ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl,error) );

    DBGWRT_1((DBH, "LL - Z82_Init: base address = %08p\n", (void*)llHdl->ma));

	/* 	Z082_DEBOUNCE_VALUE */
    if ((error = DESC_GetUInt32(llHdl->descHdl, 
								Z82_DEBOUNCE_DEFAULT,
								&llHdl->nDebounce, 
								"Z082_DEBOUNCE_VALUE")) && error != \
		ERR_DESC_KEY_NOTFOUND)
		return( Cleanup(llHdl, error));

    /*------------------------------+
    |  init hardware                |
    +------------------------------*/
    
	/* set Debouncer Value */
 	MWRITE_D32( llHdl->ma, Z82_REG_DEBOUNCER, 
				(llHdl->nDebounce) &Z82_DEBOUNCE_MASK);

	/* Set Counter Register to zero */
	MWRITE_D32( llHdl->ma, Z82_REG_RESULT, 0x0 );

	*llHdlP = llHdl;	/* set low-level driver handle */

	return(ERR_SUCCESS);
}

/****************************** Z82_Exit ************************************/
/** De-initialize hardware and clean up memory
 *
 *  The function deinitializes all channels by setting them as inputs.
 *  The interrupt is disabled.
 *
 *  \param llHdlP      \IN  pointer to low-level driver handle
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z82_Exit(
   LL_HANDLE    **llHdlP
)
{
    LL_HANDLE *llHdl = *llHdlP;
	int32 error = 0;

    DBGWRT_1((DBH, "LL - Z82_Exit\n"));

    /*------------------------------+
    |  de-init hardware             |
    +------------------------------*/
	
    /*------------------------------+
    |  clean up memory               |
    +------------------------------*/
	*llHdlP = NULL;		/* set low-level driver handle to NULL */
	error = Cleanup(llHdl,error);

	return(error);
}

/****************************** Z82_Read ************************************/
/** Read a value from the device
 *
 *  The function reads the current value in the RESULT Register
 *
 *  \param llHdl      \IN  low-level handle
 *  \param ch         \IN  current channel
 *  \param valueP     \OUT read value
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z82_Read(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 *valueP
)
{
    DBGWRT_1((DBH, "LL - Z82_Read: ch=%d\n",ch));

    *valueP = MREAD_D32( llHdl->ma, Z82_REG_RESULT );

	return(ERR_SUCCESS);
}

/****************************** Z82_Write ***********************************/
/** Description:  Write a value to the device
 *
 *  The function writes a value to RESULT, which causes it to reset to 0.
 *
 *  \param llHdl      \IN  low-level handle
 *  \param ch         \IN  current channel
 *  \param value      \IN  value to write - unused (write causes Reset)
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z82_Write(
    LL_HANDLE *llHdl,
    int32 ch,
    int32 value
)
{
    DBGWRT_1((DBH, "LL - Z82_Write: ch=%d  val=0x%x\n",ch, value));

    MWRITE_D32( llHdl->ma, Z82_REG_RESULT, value );

	return(ERR_SUCCESS);
}

/****************************** Z82_SetStat *********************************/
/** Set the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl  			\IN  low-level handle
 *  \param code				\IN  \ref getstat_setstat_codes "status code"
 *  \param ch				\IN  current channel
 *  \param value32_or_64    \IN  data or pointer to block data structure
 *                               (M_SG_BLOCK) for block status codes
 *  \return					\c 0 on success or error code
 */
static int32 Z82_SetStat(
    LL_HANDLE *llHdl,
    int32  code,
    int32  ch,
    INT32_OR_64 value32_or_64
)
{
    MACCESS		ma = llHdl->ma;
	int32		error = ERR_SUCCESS;
	int32		value = (int32)value32_or_64;	/* 32bit value     */

    DBGWRT_1((DBH, "LL - Z82_SetStat: ch=%d code=0x%04x value=0x%x\n",
			  ch,code,value));

    switch(code) {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            llHdl->dbgLevel = value;
            break;
        /*--------------------------+
        |  enable interrupts        |
        +--------------------------*/
        case M_MK_IRQ_ENABLE:
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
            if( value != M_CH_INOUT ) {
                error = ERR_LL_ILL_DIR;
			}

            break;

        /*--------------------------+
        |  clear RESULT	            |
        +--------------------------*/
        case Z82_CLR_RESULT:
            MWRITE_D32( ma, Z82_REG_RESULT, 0 );
            break;

        /*--------------------------+
        |  set debouncer            |
        +--------------------------*/
        case Z82_DEBOUNCE:
			/* use only bits [10..0] */
            MWRITE_D32( ma, Z82_DEBOUNCE, value & Z82_DEBOUNCE_MASK );
            break;

        /*--------------------------+
        |  (unknown)                |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/****************************** Z82_GetStat *********************************/
/** Get the driver status
 *
 *  The driver supports \ref getstat_setstat_codes "these status codes"
 *  in addition to the standard codes (see mdis_api.h).
 *
 *  \param llHdl			\IN  low-level handle
 *  \param code				\IN  \ref getstat_setstat_codes "status code"
 *  \param ch				\IN  current channel
 *  \param value32_or_64P	\IN  pointer to block data structure (M_SG_BLOCK) for
 *                               block status codes
 *  \param value32_or_64P	\OUT data pointer or pointer to block data structure
 *                               (M_SG_BLOCK) for block status codes
 *
 *  \return           \c 0 on success or error code
 */
static int32 Z82_GetStat(
    LL_HANDLE	*llHdl,
    int32		code,
    int32		ch,
    INT32_OR_64 *value32_or_64P
)
{
    MACCESS		ma = llHdl->ma;
	int32		error = ERR_SUCCESS;
	int32		*valueP = (int32*)value32_or_64P;	/* pointer to 32bit value  */
	INT32_OR_64	*value64P = value32_or_64P;			/* stores 32/64bit pointer  */

    DBGWRT_1((DBH, "LL - Z82_GetStat: ch=%d code=0x%04x\n",
			  ch,code));

    switch(code)
    {
        /*--------------------------+
        |  debug level              |
        +--------------------------*/
        case M_LL_DEBUG_LEVEL:
            *valueP = llHdl->dbgLevel;
            break;
        /*--------------------------+
        |  number of channels       |
        +--------------------------*/
        case M_LL_CH_NUMBER:
            *valueP = CH_NUMBER;
            break;
        /*--------------------------+
        |  channel direction        |
        +--------------------------*/
        case M_LL_CH_DIR:
            *valueP = M_CH_INOUT;
            break;
        /*--------------------------+
        |  channel length [bits]    |
        +--------------------------*/
        case M_LL_CH_LEN:
            *valueP = 8;
            break;
        /*--------------------------+
        |  channel type info        |
        +--------------------------*/
        case M_LL_CH_TYP:
            *valueP = M_CH_BINARY;
            break;
        /*--------------------------+
        |  ID PROM check enabled    |
        +--------------------------*/
        case M_LL_ID_CHECK:
            *valueP = 0;
            break;
        /*--------------------------+
        |   ident table pointer     |
        |   (treat as non-block!)   |
        +--------------------------*/
        case M_MK_BLK_REV_ID:
           *value64P = (INT32_OR_64)&llHdl->idFuncTbl;
           break;

        /*--------------------------+
        |  get Result Register      |
        +--------------------------*/
        case Z82_GET_RESULT:
            *valueP = MREAD_D32( ma, Z82_REG_RESULT );
            break;

        /*--------------------------+
        |  get overflow bit         |
        +--------------------------*/
        case Z82_GET_OVF:
            *valueP = MREAD_D32( ma, Z82_REG_STATUS ) & 1;
            break;

        /*--------------------------+
        |  debouncer                |
        +--------------------------*/
        case Z82_DEBOUNCE:
            *valueP = MREAD_D32( ma, Z82_REG_DEBOUNCER) & Z82_DEBOUNCE_MASK;
            break;

        /*--------------------------+
        |  (unknown)                |
        +--------------------------*/
        default:
            error = ERR_LL_UNK_CODE;
    }

	return(error);
}

/******************************* Z82_BlockRead ******************************/
/** Read a data block from the device
 *
 *  \param llHdl       \IN  low-level handle
 *  \param ch          \IN  current channel
 *  \param buf         \IN  data buffer
 *  \param size        \IN  data buffer size
 *  \param nbrRdBytesP \OUT number of read bytes
 *
 *  \return            \c 0 on success or error code
 */
static int32 Z82_BlockRead(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrRdBytesP
)
{
    DBGWRT_1((DBH, "LL - Z82_BlockRead: ch=%d, size=%d\n",ch,size));

	/* return number of read bytes */
	*nbrRdBytesP = 0;

	return( ERR_LL_ILL_FUNC );
}

/****************************** Z82_BlockWrite *****************************/
/** Write a data block from the device
 *
 *  \param llHdl  	   \IN  low-level handle
 *  \param ch          \IN  current channel
 *  \param buf         \IN  data buffer
 *  \param size        \IN  data buffer size
 *  \param nbrWrBytesP \OUT number of written bytes
 *
 *  \return            \c 0 on success or error code
 */
static int32 Z82_BlockWrite(
     LL_HANDLE *llHdl,
     int32     ch,
     void      *buf,
     int32     size,
     int32     *nbrWrBytesP
)
{
    DBGWRT_1((DBH, "LL - Z82_BlockWrite: ch=%d, size=%d\n",ch,size));

	/* return number of written bytes */
	*nbrWrBytesP = 0;

	return( ERR_LL_ILL_FUNC );
}


/****************************** Z82_Irq ************************************/
/** Interrupt service routine
 *
 *  Not used in Z82 Unit.
 *
 *  \param llHdl  	   \IN  low-level handle
 *  \return LL_IRQ_DEV_NOT  always
 *
 */
static int32 Z82_Irq(
   LL_HANDLE *llHdl
)
{
	return( LL_IRQ_DEV_NOT );
}

/****************************** Z82_Info ***********************************/
/** Get information about hardware and driver requirements
 *
 *  The following info codes are supported:
 *
 * \code
 *  Code                      Description
 *  ------------------------  -----------------------------
 *  LL_INFO_HW_CHARACTER      hardware characteristics
 *  LL_INFO_ADDRSPACE_COUNT   nr of required address spaces
 *  LL_INFO_ADDRSPACE         address space information
 *  LL_INFO_IRQ               interrupt required
 *  LL_INFO_LOCKMODE          process lock mode required
 * \endcode
 *
 *  The LL_INFO_HW_CHARACTER code returns all address and
 *  data modes (ORed) which are supported by the hardware
 *  (MDIS_MAxx, MDIS_MDxx).
 *
 *  The LL_INFO_ADDRSPACE_COUNT code returns the number
 *  of address spaces used by the driver.
 *
 *  The LL_INFO_ADDRSPACE code returns information about one
 *  specific address space (MDIS_MAxx, MDIS_MDxx). The returned
 *  data mode represents the widest hardware access used by
 *  the driver.
 *
 *  The LL_INFO_IRQ code returns whether the driver supports an
 *  interrupt routine (TRUE or FALSE).
 *
 *  The LL_INFO_LOCKMODE code returns which process locking
 *  mode the driver needs (LL_LOCK_xxx).
 *
 *  \param infoType	   \IN  info code
 *  \param ...         \IN  argument(s)
 *
 *  \return            \c 0 on success or error code
 */
static int32 Z82_Info(
   int32  infoType,
   ...
)
{
    int32   error = ERR_SUCCESS;
    va_list argptr;

    va_start(argptr, infoType );

    switch(infoType) {
		/*-------------------------------+
        |  hardware characteristics      |
        |  (all addr/data modes ORed)   |
        +-------------------------------*/
        case LL_INFO_HW_CHARACTER:
		{
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);

			*addrModeP = MDIS_MA08;
			*dataModeP = MDIS_MD08 | MDIS_MD16;
			break;
	    }
		/*-------------------------------+
        |  nr of required address spaces |
        |  (total spaces used)           |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE_COUNT:
		{
			u_int32 *nbrOfAddrSpaceP = va_arg(argptr, u_int32*);
			*nbrOfAddrSpaceP = ADDRSPACE_COUNT;
			break;
	    }
		/*-------------------------------+
        |  address space type            |
        |  (widest used data mode)       |
        +-------------------------------*/
        case LL_INFO_ADDRSPACE:
		{
			u_int32 addrSpaceIndex = va_arg(argptr, u_int32);
			u_int32 *addrModeP = va_arg(argptr, u_int32*);
			u_int32 *dataModeP = va_arg(argptr, u_int32*);
			u_int32 *addrSizeP = va_arg(argptr, u_int32*);

			if (addrSpaceIndex >= ADDRSPACE_COUNT)
				error = ERR_LL_ILL_PARAM;
			else {
				*addrModeP = MDIS_MA08;
				*dataModeP = MDIS_MD16;
				*addrSizeP = ADDRSPACE_SIZE;
			}

			break;
	    }
		/*-------------------------------+
        |   interrupt required           |
        +-------------------------------*/
        case LL_INFO_IRQ:
		{
			u_int32 *useIrqP = va_arg(argptr, u_int32*);

			*useIrqP = USE_IRQ;
			break;
	    }
		/*-------------------------------+
        |   process lock mode            |
        +-------------------------------*/
        case LL_INFO_LOCKMODE:
		{
			u_int32 *lockModeP = va_arg(argptr, u_int32*);
			*lockModeP = LL_LOCK_CALL;
			break;
	    }
		/*-------------------------------+
        |   (unknown)                    |
        +-------------------------------*/
        default:
          error = ERR_LL_ILL_PARAM;
    }

    va_end(argptr);
    return(error);
}

/*******************************  Ident  ***********************************/
/** Return ident string
 *
 *  \return            pointer to ident string
 */
static char* Ident( void )
{
    return( "Z82 low level driver: $Id: z82_drv.c,v 1.6 2013/06/28 17:21:46 MRoth Exp $" );
}

/********************************* Cleanup *********************************/
/** Close all handles, free memory and return error code
 *
 *	\warning The low-level handle is invalid after this function is called.
 *
 *  \param llHdl      \IN  low-level handle
 *  \param retCode    \IN  return value
 *
 *  \return           \IN   retCode
 */
static int32 Cleanup(
   LL_HANDLE    *llHdl,
   int32        retCode
)
{
    /*------------------------------+
    |  close handles                |
    +------------------------------*/
	/* clean up desc */
	if (llHdl->descHdl)
		DESC_Exit(&llHdl->descHdl);

	/* clean up debug */
	DBGEXIT((&DBH));

    /*------------------------------+
    |  free memory                  |
    +------------------------------*/
    /* free my handle */
    OSS_MemFree(llHdl->osHdl, (int8*)llHdl, llHdl->memAlloc);

    /*------------------------------+
    |  return error code            |
    +------------------------------*/
	return(retCode);
}


