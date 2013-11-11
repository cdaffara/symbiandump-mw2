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
# Description:  PosPsySetting pro file
#

TEMPLATE = lib
TARGET = pospsysettings
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
CONFIG += hb \
					plugin

#Uncomment following line to disable logging
#DEFINES += QT_NO_DEBUG_OUTPUT
MOC_DIR = moc
OBJECTS_DIR = obj
RCC_DIR = rcc

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5
LIBS += -lcpframework \
    -llbs \
    -leposmodset \
    -lpossettingsengine

# Input
SOURCES += src/possettingssignalmapper.cpp \
    src/pospsysettingsplugin.cpp \
    src/pospsysettingsengine.cpp \
    src/pospsysettingsengineprivate.cpp
HEADERS += inc/possettingssignalmapper.h \
    inc/pospsysettingsplugin.h \
    inc/pospsysettingsengine.h \
    inc/pospsysettingsengineprivate.h \
    inc/pospsysettingscommon.h
symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += PLUGINUID3=0x2002C31A
    TARGET.UID3 = 0x2002C31A
}
symbian { 
    deploy.path = C:
    qtplugins.path = /resource/qt/plugins/positioningsettings
    qtplugins.sources += qmakepluginstubs/pospsysettings.qtplugin
    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
}
TARGET.CAPABILITY = All -TCB
plugin.sources = pospsysettings.dll
plugin.path = \resource\qt\plugins\positioningsettings
DEPLOYMENT += plugin
