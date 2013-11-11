#
# Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

BUILD_DIR=$(EPOCROOT)epoc32\build\generated\WapPush\Plugins\StringDictionaries
GENERATED_FILES= \
	$(BUILD_DIR)\DRMStringDict00TagTable.cpp
SOURCE_DIR=..\..\Plugins\StringDictionaries\DRM

$(BUILD_DIR)\DRMStringDict00TagTable.cpp : $(SOURCE_DIR)\DRMStringDict00TagTable.st
	perl -S emkdir.pl $(BUILD_DIR)
	perl -S ecopyfile.pl $(SOURCE_DIR)\DRMStringDict00TagTable.st $(BUILD_DIR)\DRMStringDict00TagTable.st
	perl -S stringtable.pl $(BUILD_DIR)\DRMStringDict00TagTable.st

do_nothing:
	@rem do nothing

#
# The targets invoked by bld...
#

MAKMAKE : $(GENERATED_FILES)

BLD : MAKMAKE

SAVESPACE : MAKMAKE

CLEAN : 
	perl -S ermdir.pl $(BUILD_DIR)

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing

FINAL : do_nothing

RELEASABLES : $(GENERATED_FILES)
