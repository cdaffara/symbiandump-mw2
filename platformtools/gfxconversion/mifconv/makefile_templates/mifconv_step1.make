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


# Define location for the parameter file
PARAMFILE=$(VECTORINTERMEDIATEDIR)$/mifconv-$(notdir $(TARGETFILE))-params.txt

# Set file-extension to .mif
TARGETFILE:=$(addsuffix .mif,$(basename $(TARGETFILE)))

# Set default directory for TARGETFILE if no diretory set
ifeq ($(strip $(dir $(TARGETFILE))),./)
    TARGETFILE:=$(addprefix $(firstword $($(PLATFORM)_TARGETDIR) $(TARGETDIR))$/,$(TARGETFILE))
endif

# Set default directory for HEADERFILE if no directory set 
ifeq ($(strip $(dir $(HEADERFILE))),./)
    HEADERFILE:=$(addprefix $(HEADERDIR)$/,$(HEADERFILE))
endif