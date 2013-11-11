#
# Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:  scalable icons makefile for project EPos_LandmarksGlobalCategories.mmp
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\z
else
ZDIR=\epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\eposlmglcategories.mif
HEADERFILENAME=$(HEADERDIR)\eposlmglcategories.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) \
	    /c8,1 qgn_prop_lm_hotel.svg \
	    /c8,1 qgn_prop_lm_business.svg \
	    /c8,1 qgn_prop_lm_communication.svg \
	    /c8,1 qgn_prop_lm_education.svg \
	    /c8,1 qgn_prop_lm_fun.svg \
	    /c8,1 qgn_prop_lm_restaurant.svg \
	    /c8,1 qgn_prop_lm_area.svg \
	    /c8,1 qgn_prop_lm_outdoor.svg \
	    /c8,1 qgn_prop_lm_people.svg \
	    /c8,1 qgn_prop_lm_public.svg \
	    /c8,1 qgn_prop_lm_church.svg \
	    /c8,1 qgn_prop_lm_shopping.svg \
	    /c8,1 qgn_prop_lm_sightseeing.svg \
	    /c8,1 qgn_prop_lm_sport.svg \
	    /c8,1 qgn_prop_lm_transport.svg

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
