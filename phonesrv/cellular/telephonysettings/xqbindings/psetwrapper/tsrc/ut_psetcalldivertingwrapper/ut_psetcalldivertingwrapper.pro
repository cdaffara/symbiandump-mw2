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
# Description: Project file for building unit test component
#
#

CONFIG += qtestlib release
TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/
INCLUDEPATH += . ../common
DEFINES += BUILD_PSETWRAPPER
DEFINES += QT_NO_DEBUG_STREAM

QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += /epoc32/include/mw/QtTest    \
                 /epoc32/include/platform/mw  \
                 /epoc32/include/mw/qt
  LIBS += -lmocklib -lsymbianmock -letelmm -letel -lbafl -lvmbxengine
}

# test code
HEADERS += ut_psetcalldivertingwrapper.h
HEADERS += ../common/testutilities.h
SOURCES += ut_psetcalldivertingwrapper.cpp
SOURCES += ../common/testutilities.cpp

# code to be tested
HEADERS += ../../../../../../phonesrv_plat/phone_settings_api/inc/psetcalldivertingwrapper.h
SOURCES += ../../src/psetcalldivertingwrapper.cpp    \
           ../../src/psetcalldivertingwrapper_p.cpp

# mocks needed for testing
SOURCES += ../mocks/mock_psetcontainer.cpp       \
           ../mocks/mock_cphcltemergencycall.cpp \
           ../mocks/mock_psetcalldiverting.cpp \
           ../mocks/mock_cvoicemailbox.cpp \
           ../mocks/mock_cvoicemailboxentry.cpp
