#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

TEMPLATE = app
TARGET = ut_vmbxengine
TARGET.EPOCALLOWDLLDATA = 1

CONFIG += hb
CONFIG += qtestlib

DEPENDPATH += .

INCLUDEPATH += .
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
HEADERS += \epoc32\include\platform\mw\cvoicemailbox.h
HEADERS += \epoc32\include\platform\mw\cvoicemailboxentry.h

INCLUDEPATH += ..\..\inc

HEADERS += $$PUBLIC_HEADERS

symbian: { 
    TARGET.CAPABILITY = CAP_GENERAL_DLL
}

# Code subject to test 
SOURCES += ..\..\src\vmbxuihandler.cpp
# It is mocked by qtuihandler_mock.cpp, remove the real one
#SOURCES += ..\..\src\vmbxqtuihandler.cpp
SOURCES += ..\..\src\voicemailboximpl.cpp
SOURCES += ..\..\src\vmbxpbkstore.cpp
SOURCES += ..\..\src\vmbxetelconnection.cpp
SOURCES += ..\..\src\vmbxsimhandler.cpp
SOURCES += ..\..\src\vmbxcenrephandler.cpp
SOURCES += ..\..\src\vmbxutilities.cpp
SOURCES += ..\..\src\vmbxenginefactory.cpp
SOURCES += ..\..\src\vmbxenginebase.cpp
SOURCES += ..\..\src\vmbxcsvoiceengine.cpp
SOURCES += ..\..\src\vmbxcsvideoengine.cpp
SOURCES += ..\..\src\vmbxvoipengine.cpp
SOURCES += ..\..\src\vmbxemergencycall.cpp  
SOURCES += ..\..\src\vmbxuiutilities.cpp
SOURCES += ..\..\src\vmbxcenrepobserver.cpp
SOURCES += ..\..\src\vmbxsimstoreobserver.cpp
SOURCES += ..\..\src\vmbxsatrefreshobserver.cpp
SOURCES += ..\..\src\vmbxobserver.cpp
SOURCES += ..\..\src\vmbshandler.cpp
SOURCES += ..\..\src\vmspshandler.cpp
SOURCES += ..\..\src\vmsettingsuiiconfileprovider.cpp
SOURCES += ..\..\src\cvoicemailbox.cpp
SOURCES += ..\..\src\cvoicemailboxentry.cpp

HEADERS += ..\..\inc\vmbxuihandler.h
HEADERS += ..\..\inc\vmbxqtuihandler.h
HEADERS += ..\..\inc\voicemailboximpl.h
HEADERS += ..\..\inc\vmbxpbkstore.h
HEADERS += ..\..\inc\vmbxetelconnection.h
HEADERS += ..\..\inc\vmbxsimhandler.h
HEADERS += ..\..\inc\vmbxcenrephandler.h
HEADERS += ..\..\inc\vmbxutilities.h
HEADERS += ..\..\inc\vmbxenginefactory.h
HEADERS += ..\..\inc\vmbxenginebase.h
HEADERS += ..\..\inc\vmbxcsvoiceengine.h
HEADERS += ..\..\inc\vmbxcsvideoengine.h
HEADERS += ..\..\inc\vmbxvoipengine.h
HEADERS += ..\..\inc\vmbxemergencycall.h  
HEADERS += ..\..\inc\vmbxuiutilities.h
HEADERS += ..\..\inc\vmbxcenrepobserver.h
HEADERS += ..\..\inc\vmbxsimstoreobserver.h
HEADERS += ..\..\inc\vmbxsatrefreshobserver.h
HEADERS += ..\..\inc\vmbxobserver.h
HEADERS += ..\..\inc\vmbshandler.h
HEADERS += ..\..\inc\vmspshandler.h
HEADERS += ..\..\inc\vmsettingsuiiconfileprovider.h

HEADERS += ..\..\inc\mvmbxcenrephandler.h 
HEADERS += ..\..\inc\mvmbxcenrepobserver.h 
HEADERS += ..\..\inc\mvmbxchangeobserver.h 
HEADERS += ..\..\inc\mvmbxresourceprovider.h
HEADERS += ..\..\inc\mvmbxsatrefreshobserver.h
HEADERS += ..\..\inc\mvmbxservicenotify.h
HEADERS += ..\..\inc\mvmbxsimstoreobserver.h
HEADERS += ..\..\inc\mvmbxuihandler.h
HEADERS += ..\..\inc\mvmbxuiutilities.h

# Test code
HEADERS += inc\ut_vmbxengine.h
HEADERS += inc\ut_pbkstore.h
HEADERS += inc\ut_simhandler.h

SOURCES += src\ut_vmbxengine.cpp
SOURCES += src\ut_pbkstore.cpp
SOURCES += src\ut_simhandler.cpp
SOURCES += src\main.cpp

# Mock class
SOURCES += src\etel_mock.cpp
SOURCES += src\mpbutil_mock.cpp
SOURCES += src\qtuihandler_mock.cpp
SOURCES += src\cenrep_mock.cpp

LIBS += -lflogger
LIBS += -lfeatmgr    
LIBS += -lcenrepnotifhandler
LIBS += -lphoneclient
LIBS += -lsssettings
LIBS += -lsatclient
LIBS += -lserviceprovidersettings
LIBS += -lefsrv
    
#End of file
