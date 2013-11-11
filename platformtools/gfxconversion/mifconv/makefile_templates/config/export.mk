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
# Description: makefile_templates, actual build configuration export makefile
# 
 
MAKEFILE =      /sf/mw/platformtools/gfxconversion/mifconv/makefile_templates/config/export.mk
$(call push,MAKEFILE_STACK,$(MAKEFILE)) 

 
TEMPLATEMAKEFILES =	/sf/mw/platformtools/gfxconversion/mifconv/makefile_templates/*.*			/epoc32/tools/makefile_templates/s60/

template_makefiles :: template_makefiles_config
         
$(call addfiles, $(TEMPLATEMAKEFILES), template_makefiles_config) 
         

$(call popout,MAKEFILE_STACK) 
