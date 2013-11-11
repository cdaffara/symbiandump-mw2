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
INCLUDEPATH += . ../../../inc ../../../../../inc ../shared

CONFIG += hb qtestlib

QT -= gui

DEFINES += BUILD_DIALPAD \
           BUILD_DIALPADKEYHANDLER \
           XQ_BUILD_XQSERVICE_LIB \
           BUILD_HB_WIDGETS

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
}

HEADERS += ../../../inc/dialpadkeysequenceeventfilter.h
SOURCES += ../../../src/dialpadkeysequenceeventfilter.cpp

HEADERS += ut_dialpadkeysequenceeventfilter.h
SOURCES += ut_dialpadkeysequenceeventfilter.cpp

HEADERS += ../../../../../phonesrv_plat/dialpad_api/inc/dialpad.h
HEADERS += /epoc32/include/mw/xqservicerequest.h \
           /epoc32/include/mw/xqappmgr.h         \
           /epoc32/include/mw/xqaiwrequest.h     \
           hblineedit.h

SOURCES += ../shared/mock_dialpad.cpp          \
           ../shared/mock_xqservicerequest.cpp \
           ../shared/mock_hblineedit.cpp       \
           ../shared/mock_xqappmgr.cpp         \
           ../shared/mock_xqaiwrequest.cpp     \
           ../shared/mock_xqaiwinterfacedescriptor.cpp

LIBS += -lmocklib -lsymbianmock -lxqserviceutil
