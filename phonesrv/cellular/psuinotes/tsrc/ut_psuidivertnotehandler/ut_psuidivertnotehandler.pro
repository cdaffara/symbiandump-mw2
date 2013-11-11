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


CONFIG += hb qtestlib
TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/
INCLUDEPATH += . ../../inc/
DEFINES += BUILD_PSUINOTES BUILD_PSETWRAPPER

QT -= gui
QT += testlib

symbian: {
  INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
  LIBS += -lmocklib -lsymbianmock
}

# test code
HEADERS += ut_psuidivertnotehandler.h
SOURCES += ut_psuidivertnotehandler.cpp

# code to be tested
HEADERS += ../../../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuidivertnotehandler.h	 
                   
SOURCES += ../../src/psuidivertnotehandler.cpp 

# mocks needed for testing
HEADERS += ../../../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuinotes.h \ 
	../../../../phonesrv_plat/phone_settings_api/inc/psetcalldivertingwrapper.h	

SOURCES += ../mocks/mock_psuinotes.cpp \
		../mocks/mock_psetcalldivertingwrapper.cpp \
		../mocks/mock_qobject.cpp
