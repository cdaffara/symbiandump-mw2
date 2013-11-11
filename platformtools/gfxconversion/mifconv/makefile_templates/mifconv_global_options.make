#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
# 
# Description:
# 

# Check if $CFG variable has been defined, if not, then it is always udeb
# This is needed because some build operations are not urel/udeb specific, but
# Mifconv must always have one specific, because in WINSCW we need to know the
# correct target directory
ifeq ($(strip $(CFG)),)
    CFG:=udeb
endif

# Parallel builds may cause 'overriding commands for target...' warnings.
# This is caused by multiple configurations having same targets.
# To avoid those warnings, let's take the initial configuration and build 
# dependencies only for it.
ifeq ($(strip $(INIT_CFG)),)
	INIT_CFG:=$(PLATFORM)$(CFG)
endif

# Icon BMP bitmap depth mask additional name parts:
MASK_OPTION_1:=_mask
MASK_OPTION_8:=_mask_soft
		
# Binary conversion tools:
SVGTBINENCODE:=$(EPOCROOT)epoc32$/tools$/svgtbinencode
MIFCONV:=$(EPOCROOT)epoc32$/tools$/mifconv

# SVG and BMP icon bitmap searching directories, depends on a platform:
VECTORSOURCEDIR:=$(EPOCROOT)epoc32$/s60$/icons
BITMAPSOURCEDIR:=$(EPOCROOT)epoc32$/s60$/bitmaps

# Optional SVG icon searching subdirectories:
VECTORSOURCESUBDIR_NSS:=$(VECTORSOURCEDIR)$/nss
VECTORSOURCESUBDIR_NOKIA:=$(VECTORSOURCEDIR)$/nokia
VECTORSOURCESUBDIR_OEM:=$(VECTORSOURCEDIR)$/oem

# Default target directories:
WINS_TARGETDIR:=$(EPOCROOT)epoc32$/release$/wins$/$(CFG)$/z$/resource$/apps
WINSCW_TARGETDIR:=$(EPOCROOT)epoc32$/release$/winscw$/$(CFG)$/z$/resource$/apps
TARGETDIR:=$(EPOCROOT)epoc32$/data$/z$/resource$/apps
HEADERDIR:=$(EPOCROOT)epoc32$/include
