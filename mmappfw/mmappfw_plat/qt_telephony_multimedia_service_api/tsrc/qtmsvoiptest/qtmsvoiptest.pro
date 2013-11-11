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
# Project specification file for tsrc.
#

TEMPLATE = app
TARGET = 
QT += testlib


TARGET.CAPABILITY = CAP_APPLICATION MultimediaDD
TARGET.EPOCALLOWDLLDATA	= 1


LIBS +=	-lqtmsapi \
				-llibglib \
				-llibgobject \
				-llibgmodule \
				-llibstdcpp

DEPENDPATH += .
INCLUDEPATH += .

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
							 $$OS_LAYER_LIBC_SYSTEMINCLUDE \
							 $$OS_LAYER_GLIB_SYSTEMINCLUDE \
							 ..\..\..\qt_telephony_multimedia_service_api\inc
# Input
HEADERS += qtestmains60.h 
SOURCES += qtmsvoiptest.cpp
