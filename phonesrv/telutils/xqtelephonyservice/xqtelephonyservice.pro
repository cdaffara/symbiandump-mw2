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
TARGET = xqtelephonyservice

symbian {
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x20029F04
    DEFINES += BUILD_XQCALLINFO
    VERSION = 10.0
    
    INCLUDEPATH += ../../inc
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    
    defFiles = \
    "$${LITERAL_HASH}ifdef WINSCW" \
    "DEFFILE bwins/xqtelephonyservice.def" \
    "$${LITERAL_HASH}else" \
    "DEFFILE eabi/xqtelephonyservice.def" \
    "$${LITERAL_HASH}endif"

    MMP_RULES += defFiles
    
    LIBS += -ltelephonyservice
            
    BLD_INF_RULES.prj_exports +=  \
     "$${LITERAL_HASH}include <platform_paths.hrh>" \
     "./rom/xqtelephonyservice.iby    CORE_MW_LAYER_IBY_EXPORT_PATH(xqtelephonyservice.iby)"
     
    # tracing macros
    headers.sources = inc/qtphonesrvlog.h
    headers.path = |../../inc
    for(header, headers.sources):BLD_INF_RULES.prj_exports += "$$header $$headers.path/$$basename(header)"       
    
    BLD_INF_RULES.prj_exports += \
    "./rom/xqtelephonyservice_stub.sis /epoc32/data/z/system/install/xqtelephonyservice_stub.sis
}

# Input
include(xqtelephonyservice.pri)
