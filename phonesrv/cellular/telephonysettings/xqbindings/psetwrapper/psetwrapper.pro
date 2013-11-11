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

TEMPLATE = lib
TARGET = psetwrapper
MOC_DIR = moc

INCLUDEPATH += ../../../../phonesrv_plat/phone_settings_api/inc
SOURCEPATH += src

# Input
include(psetwrapper.pri)
    
DEFINES += BUILD_PSETWRAPPER

symbian: {
    LIBS += -lsssettings \
        -lphonesettings \
        -lcustomapi \
        -letelmm \
        -lbafl \
        -lphoneclient \
        -lvmbxengine

    load(data_caging_paths)
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0X20029F20
    VERSION = 10.0
    
    defFiles = \
    "$${LITERAL_HASH}ifdef WINS" \
        "DEFFILE bwins/psetwrapper.def" \
    "$${LITERAL_HASH}else" \
        "DEFFILE eabi/psetwrapper.def" \
    "$${LITERAL_HASH}endif"
    MMP_RULES += defFiles
    
    # For sis file
    dllfile.sources = $${TARGET}.dll
    dllfile.path = $$SHARED_LIB_DIR
    DEPLOYMENT += dllfile

    BLD_INF_RULES.prj_exports +=  \
     "$${LITERAL_HASH}include <platform_paths.hrh>" \
     "./rom/psetwrapper.iby    CORE_MW_LAYER_IBY_EXPORT_PATH(psetwrapper.iby)"
    BLD_INF_RULES.prj_exports += \
    "./rom/psetwrapper_stub.sis /epoc32/data/z/system/install/psetwrapper_stub.sis"
} else: {
    DESTDIR = c:\hb\lib
    DLLDESTDIR = c:\hb\bin
}
