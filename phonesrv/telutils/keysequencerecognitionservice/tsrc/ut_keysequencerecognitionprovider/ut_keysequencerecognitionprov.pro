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
TARGET = ut_keysequencerecognitionprovider
CONFIG += hb qtestlib
MOC_DIR = moc
DEFINES += QT_NO_DEBUG_STREAM

QT -= gui

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += . ../shared ../../inc
    
    HEADERS += ../../inc/keysequencerecognitionprovider.h
    SOURCES += ../../src/keysequencerecognitionprovider.cpp

    HEADERS += ut_keysequencerecognitionprovider.h
    SOURCES += ut_keysequencerecognitionprovider.cpp
    
    #MOCKED DEPENDENCIES
    HEADERS +=                                            \
        ../../inc/keysequencehandler.h                    \
        ../../inc/imeikeysequencehandler.h                \
        ../../inc/lifetimerkeysequencehandler.h           \
        ../../inc/manufacturerkeysequencehandler.h        \
        ../../inc/simcontrolkeysequencehandler.h          \
        ../../inc/bluetoothkeysequencehandler.h
    
    SOURCES +=                                            \
        ../shared/mock_qtranslator.cpp                    \
        ../shared/mock_keysequencehandler.cpp             \
        ../shared/mock_imeikeysequencehandler.cpp         \
        ../shared/mock_lifetimerkeysequencehandler.cpp    \
        ../shared/mock_manufacturerkeysequencehandler.cpp \
        ../shared/mock_simcontrolkeysequencehandler.cpp   \
        ../shared/mock_centralrepository.cpp              \
        ../shared/mock_tstasksettings.cpp                 \
        ../shared/mock_bluetoothkeysequencehandler.cpp

    LIBS += -lmocklib -lsymbianmock -lxqservice -lxqserviceutil
}
