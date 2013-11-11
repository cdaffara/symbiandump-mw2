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
DEFINES += XQSETTINGSMANAGER_NO_LIBRARY

symbian {
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
    
    # mocks
    HEADERS += /epoc32/include/platform/mw/xqsettingskey.h \
               /epoc32/include/platform/mw/xqsettingsmanager.h
    SOURCES += ../shared/mock_xqsettingsmanager.cpp \
               ../shared/mock_xqsettingskey.cpp
    
    LIBS += -lsymbianmock -lmocklib
} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
    INCLUDEPATH += ../../../../xqtelephonyservice/inc
}

#
HEADERS += ../../../inc/dialpadvideomailboxeventfilter.h \
           ../../../inc/dialpadmailboxeventfilterbase.h \
           ../../../inc/dialpadsymbianwrapper.h \
           xqservicerequest.h \
           dialpadtestutil.h
SOURCES += ut_dialpadvideomailboxeventfilter.cpp \
           ../../../src/dialpadvideomailboxeventfilter.cpp \
           ../../../src/dialpadmailboxeventfilterbase.cpp \
           ../shared/dialpadtestutil.cpp

#
LIBS += -ldialpad
