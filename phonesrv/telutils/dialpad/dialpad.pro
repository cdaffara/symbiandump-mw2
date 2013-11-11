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
TARGET = dialpad
CONFIG += hb
HB += hbinput

INCLUDEPATH += . inc ../../inc

HEADERS += ../../phonesrv_plat/dialpad_api/inc/dialpad.h \
           ../../phonesrv_plat/dialpad_api/inc/dialpadkeyhandler.h \
           ../../phonesrv_plat/dialpad_api/inc/dialpadvtkeyhandler.h \
           inc/dialpadbutton.h \
           inc/dialpadinputfield.h \
           inc/dialpadkeypad.h \
           inc/dialpadmultitaphandler.h\
           inc/dialpadsymbianwrapper.h \
           inc/dialpadbackground.h \
           inc/dialpadvoicemailboxeventfilter.h \
           inc/dialpadbluetootheventfilter.h \
           inc/dialpadkeysequenceeventfilter.h \
           inc/dialpadvideomailboxeventfilter.h \
           inc/dialpadmailboxeventfilterbase.h \
           inc/dialpademergencycalleventfilter.h \
           inc/dialpadhasheventfilter.h \
           inc/dialpadnumericbutton.h \
           inc/dialpadnumericbuttongrid.h

SOURCES += src/dialpad.cpp \
           src/dialpadbutton.cpp \
           src/dialpadinputfield.cpp \
           src/dialpadkeypad.cpp \
           src/dialpadmultitaphandler.cpp\
           src/dialpadkeyhandler.cpp \
           src/dialpadsymbianwrapper.cpp \
           src/dialpadbackground.cpp \
           src/dialpadvoicemailboxeventfilter.cpp \
           src/dialpadbluetootheventfilter.cpp \
           src/dialpadkeysequenceeventfilter.cpp \
           src/dialpadvideomailboxeventfilter.cpp \
           src/dialpadmailboxeventfilterbase.cpp \
           src/dialpademergencycalleventfilter.cpp \
           src/dialpadvtkeyhandler.cpp \
           src/dialpadhasheventfilter.cpp \
           src/dialpadnumericbutton.cpp \
           src/dialpadnumericbuttongrid.cpp

RESOURCES += dialpad.qrc

symbian {    
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x102078ED
    VERSION = 10.0
    
    defFiles = \
        "$${LITERAL_HASH}ifdef WINS" \
            "DEFFILE bwins/dialpad.def" \
        "$${LITERAL_HASH}else" \
            "DEFFILE eabi/dialpad.def" \
        "$${LITERAL_HASH}endif"
    
    MMP_RULES += defFiles
    
    HEADERS += inc/dialpadsymbianwrapper_p.h
    SOURCES += src/dialpadsymbianwrapper_p.cpp
    
    BLD_INF_RULES.prj_exports += \
    "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "./rom/dialpad.iby    CORE_MW_LAYER_IBY_EXPORT_PATH(dialpad.iby)" 
    
    LIBS += -lxqservice \
            -lxqserviceutil \
            -lvmbxengine \
            -lfeatmgr \
            -lphoneclient \
            -lcentralrepository \
            -lxqsettingsmanager 

     contains(SYMBIAN_PLATFORMS, "WINSCW") {
        HEADERS += inc/dialpadinternaleventfilter.h
        SOURCES += src/dialpadinternaleventfilter.cpp
        LIBS += -lcone \
                -lapgrfx \
                -lapparc
    
    BLD_INF_RULES.prj_exports += \
    "./rom/dialpad_stub.sis /epoc32/data/z/system/install/dialpad_stub.sis"
    }
}
else:win32 { 
    HEADERS += inc/dialpadsymbianwrapper_p_stub.h
    SOURCES += src/dialpadsymbianwrapper_p_stub.cpp
    
    # Put it to Hb/lib because it is in path
    DESTDIR = c:/hb/lib
    DLLDESTDIR = c:/hb/bin
    INCLUDEPATH += ../../phonesrv_plat/dialpad_api/inc \
        c:/hb/include/hbcore \
        c:/hb/include/hbwidgets \
        c:/hb/include/hbtools \
        c:/hb/include/hbinput
    INCLUDEPATH += ../xqtelephonyservice/inc
}

DEFINES += BUILD_DIALPAD \
           BUILD_DIALPADKEYHANDLER
