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
} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
}

#
HEADERS += ../../../inc/dialpadbluetootheventfilter.h \
           xqappmgr.h \
           dialpadtestutil.h
SOURCES += ut_dialpadbluetootheventfilter.cpp \
           ../../../src/dialpadbluetootheventfilter.cpp \
           ../shared/dialpadtestutil.cpp

# 
LIBS += -ldialpad
