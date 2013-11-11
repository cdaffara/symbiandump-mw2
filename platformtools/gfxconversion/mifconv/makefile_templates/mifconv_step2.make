##
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
#


# Handle issues related to SOURCEDIR
ifneq ($(strip $(SOURCEDIR)),)
    # Replace \ with / in SOURCEDIR
    SOURCEDIR:=$(subst \,/,$(SOURCEDIR))
    
    # Replace / with $/ in SOURCEDIR
    SOURCEDIR:=$(subst /,$/,$(SOURCEDIR))

    # Insert EXTENSION_ROOT
    SOURCEDIR:=$(EXTENSION_ROOT)$/$(SOURCEDIR)

endif


# Debug prints
#$(warning PARAMFILE $(PARAMFILE))
#$(warning VECTORSOURCEDIR $(VECTORSOURCEDIR))
#$(warning VECTORINTERMEDIATEDIR $(VECTORINTERMEDIATEDIR))
#$(warning BITMAPSOURCEDIR $(BITMAPSOURCEDIR))
#$(warning WINS_TARGETDIR $(WINS_TARGETDIR))
#$(warning WINSCW_TARGETDIR $(WINSCW_TARGETDIR))
#$(warning TARGETDIR $(TARGETDIR))
#$(warning HEADERDIR $(HEADERDIR))
#$(warning SOURCEDIR $(SOURCEDIR))
#$(warning TARGETFILE $(TARGETFILE))
#$(warning HEADERFILE $(HEADERFILE))
#$(warning SOURCES $(SOURCES))
#$(warning SOURCEFILE $(SOURCEFILE))
#$(warning SVGENCODINGVERSION $(SVGENCODINGVERSION))



# Check that SOURCES and SOURCESFILE is not defined at the same time and at least one of them has been defined
$(if $(SOURCES),$(if $(SOURCEFILE),$(error Must not define both SOURCES- and SOURCEFILE-parameters!),),$(if $(SOURCEFILE),,$(error Must define either SOURCES- or SOURCEFILE-parameter!)))

# If SOURCES-parameter is not empty, then it is the OPTIONS
ifneq ($(strip $(SOURCES)),)
    OPTIONS:=$(SOURCES)
endif

# If SOURCEFILE-parameter is not empty 
ifneq ($(strip $(SOURCEFILE)),)
    # Replace \ with / in SOURCEFILE
    SOURCEFILE:=$(subst \,/,$(SOURCEFILE))
    
    # Replace / with $/ in SOURCEFILE
    SOURCEFILE:=$(subst /,$/,$(SOURCEFILE))

    # Add each of the lines in the file to the OPTIONS
    OPTIONS:=$(foreach LINE, \
                $(shell $(TOOLCAT) $(EXTENSION_ROOT)$/$(SOURCEFILE)), \
                $(LINE))
endif

# Change any leading '/' into '-' due to problems in make
OPTIONS := $(patsubst /%,-%,$(OPTIONS))
