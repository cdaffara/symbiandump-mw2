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
# Build resource files
#

PROJECT=t_msgs_registry
SOURCEDIR=.
LANGUAGE=sc

#
# Target directory
#

# Build to winscw\c for all targets
#
TARGETDIR=$(EPOCROOT)Epoc32\winscw\c\msgtest\msgs


$(TARGETDIR) :
	@perl $(EPOCROOT)epoc32\tools\emkdir.pl $(TARGETDIR)

#
# Build the mtm resource files
#

SOURCERESOURCE=$(SOURCEDIR)\$(PROJECT).rss
TARGETRESOURCE=$(TARGETDIR)\$(PROJECT).r$(LANGUAGE)

$(TARGETRESOURCE) : $(TARGETDIR)
	@epocrc.bat -u -I. -I..\inc -I%EPOCROOT%epoc32\include $(SOURCERESOURCE) -o$(TARGETRESOURCE)

#$(TARGETRESOURCE) : $(TARGETDIR) $(SOURCERESOURCE)
#	@copy $(SOURCERESOURCE) $(TARGETDIR)\$(PROJECT).rss
#	@epocrc.bat -u -I. -I%EPOCROOT%epoc32\include $(TARGETDIR)\$(PROJECT).rss -o$(TARGETRESOURCE)
#	@del $(TARGETDIR)\$(PROJECT).rss

do_nothing:
	rem do nothing

#
# The targets invoked by abld 
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETRESOURCE)

SAVESPACE : BLD

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN : 
	@if exist $(TARGETRESOURCE) @erase $(TARGETRESOURCE)

RELEASABLES : 
	@echo $(TARGETRESOURCE)
