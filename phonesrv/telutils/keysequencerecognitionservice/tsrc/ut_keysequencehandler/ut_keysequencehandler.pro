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
MOC_DIR = moc
CONFIG += hb qtestlib
DEFINES += QT_NO_DEBUG_STREAM

QT -= gui

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += . ../shared ../../inc
    
    HEADERS += ../../inc/keysequencehandler.h
    SOURCES += ../../src/keysequencehandler.cpp

    HEADERS += ut_keysequencehandler.h
    SOURCES += ut_keysequencehandler.cpp
    
    LIBS += -lmocklib -lsymbianmock -lxqserviceutil
}
