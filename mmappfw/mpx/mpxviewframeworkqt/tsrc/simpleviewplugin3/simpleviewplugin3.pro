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

CONFIG += qt hb ecomplugin
TARGET.UID3 = 0xE1253179
TARGET = 

SERVICE.INTERFACE_NAME = org.nokia.mmdt.MpxViewPlugin/1.0
SERVICE.CONFIGURATION = "<t>0x10000123</t><p>0x30000001</p><i>EMPXViewPluginPriorityHighest</i><f>0x00000001</f>"

INCLUDEPATH    += inc \
		  /epoc32/include/mw

DEFINES += SIMPLE_VIEW_NO=3 SVIEW_PLUGIN_ID=simpleviewplugin3
                  
LIBS += -lmpxviewframeworkqt                  

HEADERS = ../simpleviewpluginsrc/simpleview.h \
		  ../simpleviewpluginsrc/simpleviewplugin.h 


SOURCES         = ../simpleviewpluginsrc/simpleviewplugin.cpp \
                  ../simpleviewpluginsrc/simpleview.cpp

symbian {
  TARGET.CAPABILITY = All -Tcb
  TARGET.EPOCALLOWDLLDATA = 1
  MMP_RULES += EXPORTUNFROZEN
}

