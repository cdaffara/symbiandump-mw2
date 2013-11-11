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
# Description: unit test pro file for satapp.
#

TEMPLATE = app
TARGET = ut_satapp
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib
HEADERS += $$PUBLIC_HEADERS
MOC_DIR = moc

symbian {
    TARGET.CAPABILITY = CAP_APPLICATION
}

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               $$MOC_DIR \
               ../../../../inc \
               ../../inc

LIBS += -legul \
        -lfbscli \
        -lcentralrepository \
        -lCenRepNotifHandler \
        -lCdlEngine \
        -lbafl \
        -lmediaclientaudio \
        -lprofileeng \
        -lFlogger

HEADERS += ../../inc \
           ../../inc/satappmainhandler.h \
           ../../inc/satappserverdispatcher.h \
           ../../inc/satappmenuprovider.h \
           ../../inc/satappinputprovider.h \
           ../../inc/satapptoneprovider.h \
           ../../inc/satapppopupprovider.h \
           ../../inc/satappconfirmprovider.h \
           ../../inc/satappaction.h \
           ../../inc/satappconstant.h \
           inc/ut_satappmainhandler.h \
           inc/ut_satappserverdispatcher.h \
           inc/ut_satappmenuprovider.h \
           inc/ut_satappinputprovider.h \
           inc/ut_satapptoneprovider.h \
           inc/ut_satapppopupprovider.h \
           inc/ut_satappconfirmprovider.h \
           inc/ut_satappaction.h


SOURCES += ../../src/satappmainhandler.cpp \
           ../../src/satappserverdispatcher.cpp \
           ../../src/satappmenuprovider.cpp \
           ../../src/satappinputprovider.cpp \
           ../../src/satapptoneprovider.cpp \
           ../../src/satapppopupprovider.cpp \
           ../../src/satappconfirmprovider.cpp \
           ../../src/satappaction.cpp \
           src/main.cpp \
           src/ut_satappmainhandler.cpp \
           src/ut_satappserverdispatcher.cpp \
           src/ut_satappmenuprovider.cpp \
           src/ut_satappinputprovider.cpp \
           src/ut_satapptoneprovider.cpp \
           src/ut_satapppopupprovider.cpp \
           src/ut_satappconfirmprovider.cpp \
           src/ut_satappaction.cpp \
           src/rsat_stub.cpp

RESOURCES += ../../resource/satapp.qrc

