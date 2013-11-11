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

CONFIG += qtestlib
CONFIG += hb
TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/
INCLUDEPATH += . ../../inc/
DEFINES += BUILD_PSUINOTES 
    
QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
  INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
  LIBS += -lmocklib -lsymbianmock
}

# test code
HEADERS += ut_psuilocalisation.h
SOURCES += ut_psuilocalisation.cpp

# code to be tested
HEADERS += ../../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuilocalisation.h 
           
SOURCES += ../../src/psuilocalisation.cpp

# mocks needed for testing
SOURCES += ../mocks/mock_qtranslator.cpp 
