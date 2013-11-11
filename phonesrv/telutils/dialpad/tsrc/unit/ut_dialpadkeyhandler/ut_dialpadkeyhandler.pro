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
           XQ_BUILD_XQSERVICEUTIL_LIB \
           BUILD_HB_WIDGETS

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
}

HEADERS += ../../../../../phonesrv_plat/dialpad_api/inc/dialpadkeyhandler.h
SOURCES += ../../../src/dialpadkeyhandler.cpp

HEADERS += ut_dialpadkeyhandler.h
SOURCES += ut_dialpadkeyhandler.cpp

HEADERS += ../../../../../phonesrv_plat/dialpad_api/inc/dialpad.h \
           ../../../inc/dialpadbluetootheventfilter.h             \
           ../../../inc/dialpademergencycalleventfilter.h         \
           ../../../inc/dialpadkeysequenceeventfilter.h           \
           ../../../inc/dialpadmailboxeventfilterbase.h           \
           ../../../inc/dialpadvideomailboxeventfilter.h          \
           ../../../inc/dialpadvoicemailboxeventfilter.h          \
           ../../../inc/dialpadhasheventfilter.h                  \
           ../../../inc/dialpadinternaleventfilter.h              \
           #/epoc32/include/mw/xqservicerequest.h                 \
           /epoc32/include/mw/xqappmgr.h                          \
           /epoc32/include/mw/xqaiwrequest.h                      \
           /epoc32/include/mw/xqrequestinfo.h

SOURCES += ../shared/mock_dialpad.cpp                             \
           ../shared/mock_dialpadbluetootheventfilter.cpp         \
           ../shared/mock_dialpademergencycalleventfilter.cpp     \
           ../shared/mock_dialpadkeysequenceeventfilter.cpp       \
           ../shared/mock_dialpadmailboxeventfilterbase.cpp       \
           ../shared/mock_dialpadvideomailboxeventfilter.cpp      \
           ../shared/mock_dialpadvoicemailboxeventfilter.cpp      \
           ../shared/mock_dialpadhasheventfilter.cpp              \
           ../../../src/dialpadinternaleventfilter.cpp            \
           ../shared/mock_featmgr.cpp                             \
           ../shared/mock_xqappmgr.cpp                            \
           ../shared/mock_xqaiwrequest.cpp                        \
           ../shared/mock_xqaiwinterfacedescriptor.cpp            \
           ../shared/mock_hbmainwindow.cpp                        \
           ../shared/mock_xqrequestinfo.cpp

LIBS += -lmocklib -lsymbianmock -lxqservice -lxqserviceutil -lcone -lapgrfx -lapparc

