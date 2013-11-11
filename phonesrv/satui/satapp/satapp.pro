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
#

TEMPLATE = app
TARGET = satapp
DEPENDPATH += .
HEADERS += $$PUBLIC_HEADERS
MOC_DIR = moc

CONFIG += hb

symbian: {
    TARGET.CAPABILITY = CAP_APPLICATION NetworkControl
    TARGET.UID3=0x101f4ce0
    SKINICON = qtg_large_sat
}

# enable this for tracing into a file (log/sat/satui.txt)
# DEFINES += SAT_DEBUG_TO_FILE
# LIBS += -lflogger

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
    $$MOC_DIR \
    ../../inc 

LIBS += -lSatClient \
        -lSatInternalClient \
        -legul \
        -lfbscli \
        -lcentralrepository \
        -lCenRepNotifHandler \
        -lCdlEngine \
        -lmediaclientaudio \
        -lprofileeng \
        -lbafl \
        -lafservice

HEADERS += inc/satappmainhandler.h \
           inc/satappserverdispatcher.h \
           inc/satappmenuprovider.h \
           inc/satappinputprovider.h \
           inc/satapptoneprovider.h \
           inc/satapppopupprovider.h\
           inc/satappconfirmprovider.h \
           inc/satappaction.h \
           inc/satappconstant.h

SOURCES += src/main.cpp \
           src/satappmainhandler.cpp \
           src/satappserverdispatcher.cpp \
           src/satappmenuprovider.cpp \
           src/satappinputprovider.cpp \
           src/satapptoneprovider.cpp \
           src/satapppopupprovider.cpp \
           src/satappconfirmprovider.cpp \
           src/satappaction.cpp

BLD_INF_RULES.prj_exports += "rom/satapp.iby CORE_MW_LAYER_IBY_EXPORT_PATH(satapp.iby)" \
                             "rom/satapp_stub.sis /epoc32/data/z/system/install/satapp_stub.sis" \
                             "resource/satapp.ts /epoc32/include/platform/qt/translations/satapp.ts"
                             

RESOURCES += resource/satapp.qrc

