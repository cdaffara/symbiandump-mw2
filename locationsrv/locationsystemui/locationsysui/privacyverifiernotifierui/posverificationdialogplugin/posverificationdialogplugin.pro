# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description: project file for positioing settings in control panel
TEMPLATE = lib
TARGET = posverificationdialogplugin
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
CONFIG += hb \
    plugin
MOC_DIR = moc
OBJECTS_DIR = obj
RCC_DIR = rcc
RESOURCES = resources/posverificationdialog.qrc

#Uncomment following line to disable logging
#DEFINES += QT_NO_DEBUG_OUTPUT

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5
symbian::TARGET.UID3 = 0x2002E718
SOURCES += src/apilogger.cpp \
		src/posverificationdialogplugin.cpp \
    src/customdocumentloader.cpp \
    src/posverificationcustomdialog.cpp
HEADERS += inc/apilogger.h \
    inc/posverificationdialogplugin.h \
    inc/customdocumentloader.h \
    inc/posverificationcustomdialog.h
symbian: { 
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1
    pluginstub.sources = posverificationdialogplugin.dll
    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
    pluginstub.path = /resource/plugins/devicedialogs
    DEPLOYMENT += pluginstub
}

