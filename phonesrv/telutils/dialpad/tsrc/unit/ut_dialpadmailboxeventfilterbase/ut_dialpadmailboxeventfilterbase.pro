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

DEFINES += BUILD_DIALPAD \
           XQ_BUILD_XQSERVICE_LIB

symbian {
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE

} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
    INCLUDEPATH += ../../../../xqtelephonyservice/inc
}

HEADERS += ../../../inc/dialpadmailboxeventfilterbase.h \ 
           ../../../../../phonesrv_plat/dialpad_api/inc/dialpad.h \
           ../../../inc/dialpadsymbianwrapper.h \
           /epoc32/include/mw/xqservicerequest.h

# test code
SOURCES += ut_dialpadmailboxeventfilterbase.cpp

# mock files
SOURCES += ../shared/mock_qtimer.cpp \
           ../shared/mock_dialpad.cpp \
           ../shared/mock_dialpadsymbianwrapper.cpp \
           ../shared/mock_hblineedit.cpp \
           ../shared/mock_xqservicerequest.cpp

# code to be tested
SOURCES += ../../../src/dialpadmailboxeventfilterbase.cpp

LIBS += -lmocklib \
        -lsymbianmock \
        -lxqserviceutil

