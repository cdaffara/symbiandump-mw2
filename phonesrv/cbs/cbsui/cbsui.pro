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
TARGET = cbsui
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += hb
MOC_DIR = moc

ICON = resources/icons/qgn_menu_cb.svg

symbian:
{
    TARGET.CAPABILITY = CAP_APPLICATION
    TARGET.UID3 = 0x101F4CD3
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
                   ../../inc \
                   $$MOC_DIR
	LIBS += -lcbsclient \
            -lflogger
            
    BLD_INF_RULES.prj_exports += "rom/cbsui.iby CORE_MW_LAYER_IBY_EXPORT_PATH(cbsui.iby)" \
            "rom/cbsuiresources.iby CORE_MW_LAYER_IBY_EXPORT_PATH(cbsuiresources.iby)" \
            "rom/cbsui_stub.sis /epoc32/data/z/system/install/cbsui_stub.sis"

}


# Input
HEADERS += inc/cbslog.h \
           inc/cbsuimainwindow.h \
           inc/cbsuitopiclistview.h \
           inc/cbsuitopicview.h \
           inc/cbsuiviewmanager.h \
           inc/ccbsuisettingsmonitor.h \
           inc/ccbsuitopiclistmonitor.h \
           inc/mcbs.h \
           inc/mcbsuisettingsobserver.h \
           inc/mcbsuitopicmonitorobserver.h \
           inc/msavemessagecontroller.h \
           inc/rcbsui.h 
SOURCES += src/cbsuimainwindow.cpp \
           src/cbsuitopiclistview.cpp \
           src/cbsuitopicview.cpp \
           src/cbsuiviewmanager.cpp \
           src/ccbsuitopiclistmonitor.cpp \
           src/main.cpp \
           src/rcbsui.cpp 
           
RESOURCES += resources/cbs_app.qrc

