#
# Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

# TEM specific block begins
include $(EPOCROOT)epoc32/tools/shell/$(notdir $(basename $(SHELL))).mk
# TEM specific block ends


# Check if $/ variable has been defined, if not, then it is always /
ifeq ($(strip $(/)),)
    /:=/
endif

# Replace \ with / in EPOCROOT
EPOCROOT:=$(subst \,/,$(EPOCROOT))
# Make sure that EPOCROOT ends in /
EPOCROOT:=$(patsubst %/,%,$(EPOCROOT))/
# Replace / with $/ in EPOCROOT
EPOCROOT:=$(subst /,$/,$(EPOCROOT))

# Platform specific helper tools
ifeq ($(TEMTARGET),)
    TOOLCAT:=type
    TOOLCOPY:=copy
    TOOLMKDIR:=md
else
    TOOLCAT:=cat
    TOOLCOPY:=cp
    TOOLMKDIR:=mkdir -p
endif

VECTORINTERMEDIATEDIR:=$(EPOCROOT)epoc32$/build$/s60$/icons$/$(notdir $(basename $(TARGETFILE)))
include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_global_options.make
include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_step1.make


# TEM specific block begins
MAKMAKE : 

WHATMAKEFILE \
WHATRESOURCE : 
	@echo $(TARGETFILE) 
	@echo $(HEADERFILE)



MAKMAKE \
WHATMAKEFILE \
BLD \
SAVESPACE \
LIB \
FREEZE \
RESOURCE \
FINAL \
RELEASABLES \
ROMFILE  :
# TEM specific block ends


include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_step2.make
include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_option_reader_recursive.make
include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_step3.make


# TEM specific block begins
MAKMAKE \
RESOURCE : $(BITMAPFILE) $(TARGETFILE) $(HEADERFILE)
# TEM specific block begins


include $(EPOCROOT)epoc32$/tools$/makefile_templates$/s60$/mifconv_step4.make


# TEM specific block begins
CLEAN CLEANMAKEFILE : $(addsuffix -erase,$(wildcard $(TARGETFILE) $(HEADERFILE) $(BITMAPFILE) $(PARAMFILE) $(patsubst %b,%,$(SVGBFILES)) $(SVGBFILES)))
# TEM specific block ends
