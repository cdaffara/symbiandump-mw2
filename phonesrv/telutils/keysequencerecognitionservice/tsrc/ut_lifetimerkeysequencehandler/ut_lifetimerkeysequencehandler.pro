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
INCLUDEPATH += ../../../../inc

MOC_DIR = moc
CONFIG += hb qtestlib
DEFINES += QT_NO_DEBUG_STREAM

QT -= gui

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += . ../shared ../../inc

    HEADERS += ../../inc/lifetimerkeysequencehandler.h
    SOURCES += ../../src/lifetimerkeysequencehandler.cpp

    HEADERS += ut_lifetimerkeysequencehandler.h
    SOURCES += ut_lifetimerkeysequencehandler.cpp

    #MOCKED DEPENDENCIES
    HEADERS += ../../inc/keysequencehandler.h
    SOURCES +=                                    \
        ../shared/mock_keysequencehandler.cpp     \
        ../shared/mock_centralrepository.cpp      \
        ../shared/mock_hbdevicemessagebox.cpp     \
        ../shared/mock_etel.cpp                   \
        ../shared/mock_rmmcustomapi.cpp

    LIBS += -lmocklib -lsymbianmock -lxqserviceutil
    LIBS += -letel -letelmm -lcustomapi
}
