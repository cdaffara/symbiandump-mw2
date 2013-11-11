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
INCLUDEPATH += . ../../../ ../common
DEFINES += BUILD_PSETWRAPPER

MOC_DIR = .

QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += /epoc32/include/mw/QtTest    \
                 /epoc32/include/platform/mw  \
                 /epoc32/include/mw/qt
  LIBS += -lmocklib -lsymbianmock -letelmm -letel -lbafl -lcustomapi
}

# test code
HEADERS += ut_psetwrapper.h
HEADERS += ../common/testutilities.h
SOURCES += ut_psetwrapper.cpp
SOURCES += ../common/testutilities.cpp

# code to be tested
HEADERS += ../../../../../../phonesrv_plat/phone_settings_api/inc/psetwrapper.h \
           ../../../../../../phonesrv_plat/phone_settings_api/inc/psetcalldivertingwrapper.h \
           ../../../../../../phonesrv_plat/phone_settings_api/inc/psetcallwaitingwrapper.h \
           ../../../../../../phonesrv_plat/phone_settings_api/inc/psetcliwrapper.h \
           ../../../../../../phonesrv_plat/phone_settings_api/inc/psetnetworkwrapper.h \
           ../../../../../../phonesrv_plat/phone_settings_api/inc/psetcallbarringwrapper.h
SOURCES += ../../src/psetwrapper.cpp

# mocks needed for testing
SOURCES += ../mocks/mock_psetcontainer.cpp \
           ../mocks/mock_psetnetworkwrapper.cpp \
           ../mocks/mock_psetcliwrapper.cpp \
           ../mocks/mock_psetcallwaitingwrapper.cpp \
           ../mocks/mock_psetcalldivertingwrapper.cpp \
           ../mocks/mock_psetcallbarringwrapper.cpp
