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
TARGET = 
TARGET.UID3 = 
CONFIG += hb qt

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += /epoc32/include/domain \
               /epoc32/include/domain/middleware \
               /epoc32/include/domain/applications \
               /orbit/include \
	           /epoc32/include/mw
	           
TARGET.CAPABILITY = ALL -TCB
TARGET.EPOCHEAPSIZE = 0x20000 0x1600000

LIBS += -lxqplugins -lmpxviewframeworkqt 

SOURCES += \
           src/main.cpp \
           src/qtracker.cpp \ 
           src/welcomeview.cpp
          
HEADERS += \
           inc/qtracker.h \
           inc/welcomeview.h
           

