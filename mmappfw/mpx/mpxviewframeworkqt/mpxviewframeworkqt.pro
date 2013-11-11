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

TEMPLATE = lib

TARGET = mpxviewframeworkqt
CONFIG += qt hb

TARGET.UID3 = 0x2002B33F

# QT -= gui

DEPENDPATH += inc \
              include \
              src

INCLUDEPATH +=  . \
			    ./inc \
			    ./include \
			    ../viewframework/include \
			    /epoc32/include/mw/hb/hbcore \
			    /epoc32/include/mw/hb/hbinput \
			    /epoc32/include/mw/hb/hbwidgets \
			    /epoc32/include/mw/hb/hbfeedback \
			    /epoc32/include/mw/hb/tools 

LIBS+= -lhbcore -lhbwidgets -lxqplugins
    
DEFINES += BUILD_VIEWFRAMEWORK

    
HEADERS += inc/mpxplugindata.h \
		   inc/mpxplugindataparser.h \
		   inc/mpxviewpluginresolver_p.h \
		   inc/mpxviewpluginresolver.h \
                   ../../mmappfw_plat/mpx_view_utility_api/inc/mpxviewframeworkqt.h \
	           ../../mmappfw_plat/mpx_base_view_plugins_api/inc/mpxviewpluginqt.h    
	
SOURCES += \
	   src/mpxplugindata.cpp \
	   src/mpxplugindataparser.cpp \
	   src/mpxviewpluginresolver.cpp \
	   src/mpxviewpluginresolver_p.cpp \
	   src/mpxviewframework.cpp

symbian { 

TARGET.CAPABILITY = All -TCB
TARGET.EPOCALLOWDATA = 1
MMP_RULES += EPOCALLOWDLLDATA

include(rom/rom.pri)

myDefInclude = "NOSTRICTDEF" \
"$${LITERAL_HASH}if defined(WINS)"\
"DEFFILE  bwins/mpxviewframeworkqt.def "\
"$${LITERAL_HASH}else "\
"DEFFILE  eabi/mpxviewframeworkqt.def "\
"$${LITERAL_HASH}endif"

MMP_RULES += myDefInclude



}
