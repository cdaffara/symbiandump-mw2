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
TARGET = ut_manufacturerkeysequencehandler
MOC_DIR = moc
CONFIG += hb qtestlib
DEFINES += QT_NO_DEBUG_STREAM
DEFINES += XQ_BUILD_XQSERVICE_LIB

QT -= gui

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += . ../shared ../../inc
    HEADERS += ../../inc/manufacturerkeysequencehandler.h
    SOURCES += ../../src/manufacturerkeysequencehandler.cpp

    HEADERS += ut_manufacturerkeysequencehandler.h
    SOURCES += ut_manufacturerkeysequencehandler.cpp
    
    #MOCKED DEPENDENCIES
    HEADERS +=                                  \ 
        ../../inc/keysequencehandler.h          \
        /epoc32/include/mw/xqappmgr.h           \
        /epoc32/include/mw/xqservicerequest.h   \
        /epoc32/include/mw/xqaiwrequest.h
  SOURCES +=                                    \ 
      ../shared/mock_keysequencehandler.cpp     \
      ../shared/mock_xqappmgr.cpp               \
      ../shared/mock_xqservicerequest.cpp       \
      ../shared/mock_xqaiwrequest.cpp
    
    LIBS += -lmocklib -lsymbianmock -lxqserviceutil
}
