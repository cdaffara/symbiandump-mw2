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

#
HEADERS += ../../../inc/dialpadhasheventfilter.h \
           ut_dialpadhasheventfilter.h \
           ../../../inc/dialpadmailboxeventfilterbase.h \
           ../../../inc/dialpadsymbianwrapper.h \
           ../../../inc/dialpadsymbianwrapper_p.h \
           /epoc32/include/mw/xqservicerequest.h \
           /epoc32/include/mw/xqappmgr.h \
           /epoc32/include/mw/xqaiwrequest.h \
           ../../../../../phonesrv_plat/dialpad_api/inc/dialpad.h \
           /epoc32/include/mw/hb/hbwidgets/hbnotificationdialog.h \
           hblineedit.h

SOURCES += ut_dialpadhasheventfilter.cpp \
           ../../../src/dialpadhasheventfilter.cpp \
           ../../../src/dialpadmailboxeventfilterbase.cpp \
           ../../../src/dialpadsymbianwrapper.cpp \
           ../../../src/dialpadsymbianwrapper_p.cpp \
           ../shared/mock_dialpad.cpp \
           ../shared/mock_xqservicerequest.cpp \
           ../shared/mock_hblineedit.cpp       \
           ../shared/mock_xqappmgr.cpp         \
           ../shared/mock_xqaiwrequest.cpp     \
           ../shared/mock_xqaiwinterfacedescriptor.cpp \
           ../shared/mock_cenrep.cpp \
           ../shared/mock_cvoicemailbox.cpp \
           ../shared/mock_hbnotificationdialog.cpp
	   

# 
LIBS += -lmocklib -lsymbianmock -lxqserviceutil
