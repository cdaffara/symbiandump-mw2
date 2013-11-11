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
TARGET = 
MOC_DIR = moc

INCLUDEPATH += ../../../../phonesrv_plat/ss_settings_api/inc
SOURCEPATH += src

# Input
include(sssettingswrapper.pri)

DEFINES += BUILD_SSSETTINGSWRAPPER

symbian: {
    LIBS += -lsssettings
    load(data_caging_paths)
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

    TARGET.EPOCALLOWDLLDATA = 1 
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3 = 0X20029F22
    VERSION = 10.0
    
    defFiles = \
    "$${LITERAL_HASH}ifdef WINS" \
        "DEFFILE bwins/sssettingswrapper.def" \
    "$${LITERAL_HASH}else" \
        "DEFFILE eabi/sssettingswrapper.def" \
    "$${LITERAL_HASH}endif"
    MMP_RULES += defFiles
    
    # For sis file
    dllfile.sources = $${TARGET}.dll
    dllfile.path = $$SHARED_LIB_DIR
    DEPLOYMENT += dllfile

    BLD_INF_RULES.prj_exports +=  \
     "$${LITERAL_HASH}include <platform_paths.hrh>" \
     "./rom/sssettingswrapper.iby    CORE_MW_LAYER_IBY_EXPORT_PATH(sssettingswrapper.iby)"
    BLD_INF_RULES.prj_exports += \
    "./rom/sssettingswrapper_stub.sis /epoc32/data/z/system/install/sssettingswrapper_stub.sis"
} else: {
    DESTDIR = c:\hb\lib
    DLLDESTDIR = c:\hb\bin
}
