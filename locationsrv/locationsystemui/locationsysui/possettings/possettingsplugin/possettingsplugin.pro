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
# Description:  project file for positioing settings plugin                                                     
#  

TEMPLATE = lib
TARGET = possettingsplugin
DEPENDPATH += .
INCLUDEPATH += ../inc
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

CONFIG += hb\
           plugin
  
#Uncomment following line to disable logging
#DEFINES += QT_NO_DEBUG_OUTPUT
         
MOC_DIR =     moc
OBJECTS_DIR = obj
RCC_DIR =     rcc

# Platforms
SYMBIAN_PLATFORMS = WINSCW \
                    ARMV5
    
LIBS += -lcpframework \
        -lpossettingsengine \
        -leposmodset \
        -lfeatmgr

# Input
SOURCES += 	src/possettingsplugin.cpp \
						src/possettingspluginhelper.cpp	\
						src/possettingspluginhelper_p.cpp	 \
    				src/apilogger.cpp					
			
HEADERS += 	inc/possettingsplugin.h \
						inc/possettingspluginhelper.h \
						inc/possettingspluginhelper_p.h \
    				inc/apilogger.h


TRANSLATIONS=lilocationmw.ts

symbian { 
    TARGET.EPOCALLOWDLLDATA = 1
    DEFINES += PLUGINUID3=0x2002C318
    TARGET.UID3 = 0x2002C318 

    deploy.path = C:
    qtplugins.path = /resource/qt/plugins/controlpanel
    qtplugins.sources += qmakepluginstubs/possettingsplugin.qtplugin    
    # This is for new exporting system coming in garden
    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin $$deploy.path$$qtplugins.path/$$basename(qtplugin)"
}
     
TARGET.CAPABILITY = All -TCB
plugin.sources = possettingsplugin.dll
plugin.path = \resource\qt\plugins\controlpanel
DEPLOYMENT += plugin	



