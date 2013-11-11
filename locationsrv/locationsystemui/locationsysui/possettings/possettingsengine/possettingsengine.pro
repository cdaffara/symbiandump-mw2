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
# Description:  PosSettingsEngine pro file
#

TEMPLATE = lib
TARGET = possettingsengine
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
CONFIG += hb


       
MOC_DIR =     moc
OBJECTS_DIR = obj
RCC_DIR = rcc
RESOURCES = possettings.qrc 

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5
LIBS += -lcpframework \
    -llbt \
    -llbs \
    -llbtmanager \
    -leposmodset \
    -lepos_suplsettings \
    -lxqsettingsmanager

# Input
SOURCES += src/possettingspluginsloader.cpp \
    src/possettingspushbuttonitem.cpp \
    src/possettingsgroupboxitem.cpp \
    src/possettingsform.cpp \
    src/possettingsview.cpp \
    src/possettingsengineprivate.cpp \
    src/possettingsengine.cpp \
    src/possettingsadvview.cpp \
    src/possettingsadvop.cpp \
    src/checkcustomviewitem.cpp
HEADERS += inc/possettingspluginsloader.h \
    inc/possettingspushbuttonitem.h \
    inc/possettingsgroupboxitem.h \
    inc/possettingsform.h \
    inc/possettingsengineprivate.h \
    inc/possettingsengine.h \
    inc/possettingscommon.h \
    inc/possettingsadvview.h \    
    ../inc/possettingsadvop.h \
    inc/checkcustomviewitem.h 
symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += BUILD_POSSETTINGS
    DEFINES += PLUGINUID3=0x2002C319
    TARGET.UID3 = 0x2002C319
}

# Build.inf rules
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" 
     
TARGET.CAPABILITY = All -TCB

