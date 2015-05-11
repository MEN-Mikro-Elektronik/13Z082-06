#***************************  M a k e f i l e  *******************************
#
#         Author: michael.roth@men.de
#          $Date: 2013/06/19 16:27:01 $
#      $Revision: 1.1 $
#
#    Description: Makefile definitions for the Z82 driver in IO mapped (x86)
#                 FPGAs, e.g. SC24
#
#---------------------------------[ History ]---------------------------------
#
#   $Log: driver_io.mak,v $
#   Revision 1.1  2013/06/19 16:27:01  MRoth
#   Initial Revision
#
#-----------------------------------------------------------------------------
#   (c) Copyright 2013 by MEN mikro elektronik GmbH, Nuernberg, Germany
#*****************************************************************************

MAK_NAME=z82_io

MAK_SWITCH=$(SW_PREFIX)MAC_IO_MAPPED

MAK_LIBS=$(LIB_PREFIX)$(MEN_LIB_DIR)/desc$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/oss$(LIB_SUFFIX)	\
         $(LIB_PREFIX)$(MEN_LIB_DIR)/dbg$(LIB_SUFFIX)	\


MAK_INCL=$(MEN_INC_DIR)/z82_drv.h	\
         $(MEN_INC_DIR)/men_typs.h	\
         $(MEN_INC_DIR)/oss.h		\
         $(MEN_INC_DIR)/mdis_err.h	\
         $(MEN_INC_DIR)/maccess.h	\
         $(MEN_INC_DIR)/desc.h		\
         $(MEN_INC_DIR)/mdis_api.h	\
         $(MEN_INC_DIR)/mdis_com.h	\
         $(MEN_INC_DIR)/modcom.h	\
         $(MEN_INC_DIR)/ll_defs.h	\
         $(MEN_INC_DIR)/ll_entry.h	\
         $(MEN_INC_DIR)/dbg.h		\

MAK_INP1=z82_drv$(INP_SUFFIX)

MAK_INP=$(MAK_INP1)

