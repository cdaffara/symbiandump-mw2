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
#

TEMPLATE = app
TARGET = 
TARGET.UID3 = 0xE31F4ECE
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib

symbian {

    INCLUDEPATH += \epoc32\include\platform \
                   \epoc32\include\platform\mw \
                   \epoc32\include\mw \
                   ../../../../../inc

    LIBS += -lxqservice \
            -lxqserviceutil \
            -lxqtelephonyservice

    TARGET.CAPABILITY = ALL -TCB
    DEFINES += BUILD_XQCALLINFO
}

# Input
HEADERS += ../../../../../phonesrv_plat/call_information_api/inc/xqcallinfo.h \
           ../../../inc/xqcallinfoimpl.h \
           stubs/ccallinformation.h \
           stubs/ccallinformationimpl.h 
SOURCES += unit_tests.cpp
SOURCES += ../../../src/xqcallinfo.cpp \
           ../../../src/xqcallinfoimpl.cpp\
           stubs/ccallinformation.cpp \
           stubs/ccallinformationimpl.cpp \
           ../../../..\telephonyservice\src\ccallinfoiter.cpp
