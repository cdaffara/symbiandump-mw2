# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description:
TEMPLATE = lib
TARGET = psuinotes
CONFIG += hb
MOC_DIR = moc
TRANSLATIONS = telephone_cp.ts
INCLUDEPATH += inc
SOURCEPATH += src

# Input
HEADERS += ../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuidivertnotehandler.h \
	../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuiwaitingnotehandler.h \
    ../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuinotes.h \
    ../../phonesrv_plat/phone_settings_ui_notes_api/inc/psuilocalisation.h \
    inc/psuiutils.h 
SOURCES += src/psuidivertnotehandler.cpp \
	src/psuiwaitingnotehandler.cpp \
    src/psuinotes.cpp \
    src/psuilocalisation.cpp \
    src/psuiutils.cpp 
DEFINES += BUILD_PSUINOTES
symbian: { 
    load(data_caging_paths)
    
     LIBS += -lxqsysinfo \
    -lxqsettingsmanager \
    
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0X200315B9
    VERSION = 10.0
    
    defFiles = \
    "$${LITERAL_HASH}ifdef WINS" \
        "DEFFILE bwins/psuinotes.def" \
    "$${LITERAL_HASH}else" \
        "DEFFILE eabi/psuinotes.def" \
    "$${LITERAL_HASH}endif"
    MMP_RULES += defFiles
     
    # For sis file
    dllfile.sources = $${TARGET}.dll
    dllfile.path = $$SHARED_LIB_DIR
    DEPLOYMENT += dllfile
    
    BLD_INF_RULES.prj_exports +=  \
    "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "./rom/psuinotes.iby    CORE_MW_LAYER_IBY_EXPORT_PATH(psuinotes.iby)"   
     BLD_INF_RULES.prj_exports += \
     "./rom/psuinotes_stub.sis /epoc32/data/z/system/install/psuinotes_stub.sis"
} else: { 
    INCLUDEPATH += ../../../../../mw/phonesrv/phonesrv_plat/phone_settings_api/inc
    INCLUDEPATH += ../../../../../mw/gsprofilesrv/controlpanel/controlpanel_plat/inc
    DESTDIR = c:\hb\lib
    DLLDESTDIR = c:\hb\bin
}
