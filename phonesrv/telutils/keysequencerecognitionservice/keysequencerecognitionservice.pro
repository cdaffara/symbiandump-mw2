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
# 
# Description:
#
#

TEMPLATE = app
TARGET = keysequencerecognitionservice
CONFIG += hb service
TRANSLATIONS = telephone.ts
MOC_DIR = moc

symbian {
    TARGET.UID3 = 0x2002FF8A
    TARGET.VID = VID_DEFAULT
    TARGET.CAPABILITY = CAP_APPLICATION
    TARGET.EPOCALLOWDLLDATA = 1
    VERSION = 10.0
    
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += ../../inc 
    
    BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>"
    BLD_INF_RULES.prj_exports += "./rom/keysequencerecognitionservice.iby \
        CORE_MW_LAYER_IBY_EXPORT_PATH(keysequencerecognitionservice.iby)"
    BLD_INF_RULES.prj_exports += \
      "./rom/keysequencerecognitionservice_resources.iby \
      LANGUAGE_MW_LAYER_IBY_EXPORT_PATH(keysequencerecognitionservice_resources.iby)"
    
    RSS_RULES += "hidden = KAppIsHidden;"
    RSS_RULES += "launch = KAppLaunchInBackground;"
    
    include(./keysequencerecognitionservice.pri)
    
    # Service provider specific configuration.
    SERVICE.FILE = ./resources/keysequencerecognitionservice_conf.xml
    SERVICE.OPTIONS += embeddable
    SERVICE.OPTIONS += hidden
  
    LIBS += -lxqservice -lxqserviceutil -lxqutils \
            -letel -letelmm -lcustomapi \
            -lcentralrepository -lQtSystemInfo -lsecui \
            -ltstaskmonitorclient
    BLD_INF_RULES.prj_exports += \
    "./rom/keysequencerecognitionservice_stub.sis /epoc32/data/z/system/install/keysequencerecognitionservice_stub.sis"
            
}
