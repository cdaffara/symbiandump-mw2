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
# Description:
#
#

TEMPLATE = app
TARGET =
INCLUDEPATH += . ../../../inc ../../../../../inc  ../shared

CONFIG += hb qtestlib

symbian {
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    LIBS += -lphoneclient
} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
    INCLUDEPATH += ../../../../xqtelephonyservice/inc
}

#ß
HEADERS += ../../../inc/dialpademergencycalleventfilter.h \
           ../shared/dialpadtestutil.h
SOURCES += ut_dialpademergencycalleventfilter.cpp \
           ../../../src/dialpademergencycalleventfilter.cpp \
           ../shared/dialpadtestutil.cpp \
           ../shared/mock_cphcltemergencycall.cpp

# 
LIBS += -ldialpad -lmocklib -lsymbianmock
