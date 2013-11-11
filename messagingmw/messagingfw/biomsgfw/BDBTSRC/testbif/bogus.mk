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
# Build BIF resource & MBM files
#

PROJECT=bogus
BITMAPS=$(SOURCEDIR)\bogus.bmp

SOURCEDIR=..\..\bdbtsrc\testbif
LANGUAGE=sc

#
# Tools
#

BMCONV=$(EPOCROOT)epoc32\tools\bmconv.exe

#
# Target directory
#


!if "$(PLATFORM)"=="WINSCW"
TARGETDIR=$(EPOCROOT)Epoc32\winscw\c\test\bio\bif
!else 
TARGETDIR=$(EPOCROOT)Epoc32\wins\c\test\bio\bif
!endif



$(TARGETDIR) :
	@perl $(EPOCROOT)epoc32\tools\emkdir.pl $(TARGETDIR)

#
# Build the bitmaps
#

MBMFILE=$(TARGETDIR)\$(PROJECT).mbm
$(MBMFILE) : $(TARGETDIR) $(BMCONV) $(BITMAPS)
	$(BMCONV) $(MBMFILE) $(BITMAPS)

#
# Build the BIF resource file
#

SOURCERESOURCE=$(SOURCEDIR)\$(PROJECT).rss
TARGETRESOURCE=$(TARGETDIR)\$(PROJECT).r$(LANGUAGE)
$(TARGETRESOURCE) : $(TARGETDIR) $(SOURCERESOURCE)
	@copy $(SOURCERESOURCE) $(TARGETDIR)\$(PROJECT).rss
	@epocrc.bat -u -I. -I%EPOCROOT%epoc32\include $(TARGETDIR)\$(PROJECT).rss -o$(TARGETRESOURCE)
	@del $(TARGETDIR)\$(PROJECT).rss

do_nothing:
	rem do nothing

#
# The targets invoked by abld 
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETRESOURCE) $(MBMFILE)

SAVESPACE : BLD

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN : 
	@erase $(TARGETRESOURCE) $(MBMFILE)

RELEASABLES : 
	@echo $(TARGETRESOURCE) $(MBMFILE)
