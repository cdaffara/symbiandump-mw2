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
#

TEMPLATE = app
CONFIG += hb
TARGET = dialpadtest
DEPENDPATH += .

INCLUDEPATH += .

HEADERS += dialpadtestview.h

SOURCES += main.cpp \
           dialpadtestview.cpp

RESOURCES += resources/dialpadtest.qrc

symbian {    
TARGET.EPOCHEAPSIZE = 0x1000 0xA00000
TARGET.CAPABILITY = CAP_APPLICATION    
} else:win32 {
INCLUDEPATH += ../../../../phonesrv_plat/dialpad_api/inc \
               c:/hb/include/hbcore \
               c:/hb/include/hbwidgets
}

# Libraries
LIBS += -ldialpad

