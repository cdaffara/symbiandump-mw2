#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: posindicator plugin project file
#


TEMPLATE = lib
TARGET = posindicator
CONFIG += hb \
          plugin
MOC_DIR =     moc
OBJECTS_DIR = obj

DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

#Uncomment following line to disable logging
#DEFINES += QT_NO_DEBUG_OUTPUT
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5

HEADERS += inc/posindicator.h \
           inc/apilogger.h
SOURCES += src/posindicator.cpp \
           src/apilogger.cpp

LIBS += -lposindicatorhelper
symbian { 
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0x2002E686

    hblib.sources = Hb.dll
    hblib.path = \sys\bin
    hblib.depends = "(0xEEF9EA38), 1, 0, 0, {\"Hb\"}"

    pluginstub.sources = posindicator.dll
    pluginstub.path = /resource/plugins/indicators
    DEPLOYMENT += pluginstub
    
}

