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
# Description:  Location Based Services Plugin pro file
#

TEMPLATE = lib
TARGET = poslocationservices
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += ../possettingsengine/inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
CONFIG += hb \
    plugin
MOC_DIR = moc
OBJECTS_DIR = obj
RCC_DIR = rcc
RESOURCES = poslocationservices.qrc 

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5
LIBS += -lcpframework \
    -lpossettingsengine \
    -lepos_suplsettings \
    -lflogger

# Input
SOURCES += src/poslocationservices.cpp
SOURCES += src/poslocationservicesengine.cpp
SOURCES += src/poslocationservicesengineprivate.cpp
SOURCES += src/locationservicescustomviewitem.cpp
SOURCES += src/locationserviceswidget.cpp
SOURCES += src/possettingssignalmapper.cpp

HEADERS += inc/poslocationservices.h
HEADERS += inc/poslocationservicescommon.h 
HEADERS += inc/poslocationservicesengine.h
HEADERS += inc/poslocationservicesengineprivate.h
HEADERS += inc/locationservicescustomviewitem.h
HEADERS += inc/locationserviceswidget.h
HEADERS += inc/possettingssignalmapper.h

symbian: { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += PLUGINUID3=0x2002DCEB
    TARGET.UID3 = 0x2002DCEB
}
symbian { 
    deploy.path = C:
    qtplugins.path = /resource/qt/plugins/positioningsettings
    qtplugins.sources += qmakepluginstubs/poslocationservices.qtplugin
    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
}
TARGET.CAPABILITY = All \
    -TCB
plugin.sources = poslocationservices.dll
plugin.path = \resource\qt\plugins\positioningsettings
DEPLOYMENT += plugin
