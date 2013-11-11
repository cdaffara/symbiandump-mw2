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
# Description:  QTMS TestApp project description
#

TEMPLATE = app
TARGET 	 = qttmstestapp

QT = core gui
CONFIG += qt
#CONFIG += ordered

include (buildflags.pri)

symbian: {
    DEFINES += SYMBIAN
    TARGET.CAPABILITY = CAP_APPLICATION MultimediaDD
    TARGET.EPOCALLOWDLLDATA	= 1
    TARGET.VID = VID_DEFAULT
    TARGET.UID2 = 0x100039CE
    TARGET.UID3 = 0x10207CA6
    }

#RESOURCES += qtmstestapp.qrc

#DEPENDPATH += src

symbian:
{
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
               $$OS_LAYER_LIBC_SYSTEMINCLUDE \
               $$OS_LAYER_GLIB_SYSTEMINCLUDE
}

LIBS +=	-ltmsfactory \
				-ltmsapi \
				-lqtmsapi \
				-llibglib \
				-llibgobject \
				-llibgmodule \
				-llibstdcpp

INCLUDEPATH += . inc

DEPENDPATH  += $$INCLUDEPATH src

message(Qt version: $$[QT_VERSION])
message($$_PRO_FILE_PWD_)
MOC_DIR = $$_PRO_FILE_PWD_/tmp
# Input

HEADERS += qtmstestengine.h
HEADERS += ctmstestengine.h
HEADERS += mtmstestengine.h
HEADERS += qttmstestapp.h

SOURCES += main.cpp
SOURCES += qttmstestapp.cpp
SOURCES += qtmstestengine.cpp
SOURCES += ctmstestengine.cpp

SYMBIAN_PLATFORMS = WINSCW ARMV5