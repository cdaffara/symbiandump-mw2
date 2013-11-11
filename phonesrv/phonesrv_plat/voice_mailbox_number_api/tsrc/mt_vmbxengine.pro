#
# Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

TEMPLATE = app
TARGET = mt_vmbxengine
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib

symbian: { 
    TARGET.CAPABILITY = CAP_GENERAL_DLL 
}

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
                   /epoc32/include/mw/QtTest \
                  ../../inc

    LIBS += -lvmbxengine.dll
}

# Input
HEADERS += mt_vmbxengine.h
SOURCES += mt_vmbxengine.cpp

