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
HEADERS += ut_psetnetworkwrapper.h
HEADERS += ../common/testutilities.h
SOURCES += ut_psetnetworkwrapper.cpp
SOURCES += ../common/testutilities.cpp

# code to be tested
HEADERS += ../../../../../../phonesrv_plat/phone_settings_api/inc/psetnetworkwrapper.h
SOURCES += ../../src/psetnetworkwrapper.cpp    \
           ../../src/psetnetworkwrapper_p.cpp  \
           ../../src/psetnetworkinfoconverter.cpp

# mocks needed for testing
SOURCES += ../mocks/mock_psetcontainer.cpp \
           ../mocks/mock_psetnetwork.cpp \
           ../mocks/mock_psetcsp.cpp \
           ../mocks/mock_cpsetrefreshhandler.cpp
HEADERS += ../mocks/mock_cpsetrefreshhandler.h
