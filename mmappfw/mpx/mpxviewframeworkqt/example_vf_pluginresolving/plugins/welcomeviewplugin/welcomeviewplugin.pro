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

TEMPLATE        = lib

CONFIG         += qt hb ecomplugin
TARGET.UID3 = 0xE1253164
TARGET = 

SERVICE.INTERFACE_NAME = org.nokia.mmdt.QViewPlugin/1.0
SERVICE.CONFIGURATION = "<t>0x10000000</t><p>0x30000001</p><i>EMPXViewPluginPriorityNormal</i><f>0x00000003</f>"

INCLUDEPATH    += inc \
		  /epoc32/include/mw
                  
LIBS += -lmpxviewframeworkqt

HEADERS         = inc/welcomeview.h \
		  inc/welcomeviewplugin.h 


SOURCES         = src/welcomeviewplugin.cpp \
                  src/welcomeview.cpp

TARGET.CAPABILITY = All -Tcb
symbian:TARGET.EPOCALLOWDLLDATA = 1

