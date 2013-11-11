# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description: LocNotificationEngine pro file
TEMPLATE = lib
TARGET = locnotificationengine
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
CONFIG += hb
CONFIG += mobility
MOBILITY = contacts

# Uncomment following line to disable logging
# DEFINES += QT_NO_DEBUG_OUTPUT
MOC_DIR = moc
OBJECTS_DIR = obj

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5
LIBS += -leposprvtyp

# Input
SOURCES += src/locrequestorutilsresolver.cpp \
    src/qlocnotificationengine.cpp \
    src/apilogger.cpp
HEADERS += inc/locrequestorutilsresolver.h \
    inc/qlocnotificationengine.h \
    inc/apilogger.h
symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += BUILD_POSSETTINGS
    DEFINES += PLUGINUID3=0x2002E705
    TARGET.UID3 = 0x2002E705
}

TARGET.CAPABILITY = All \
    -TCB
