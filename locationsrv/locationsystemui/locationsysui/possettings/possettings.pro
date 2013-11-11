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
# Description:  project file for positioing settings in control panel                                                      
#                                                                    


TEMPLATE = subdirs

SYMBIAN_PLATFORMS = WINSCW ARMV5

SUBDIRS += possettingsengine/possettingsengine.pro
SUBDIRS += possettingsplugin/possettingsplugin.pro
SUBDIRS += posmethodsplugin/pospsysettings.pro
SUBDIRS += possuplsettingsplugin/possuplsettings.pro
SUBDIRS += poslocationservices/poslocationservices.pro                                                                                                                                                                                                                                                       

DEFINES += BUILD_POSSETTINGS

INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE

# Build.inf rules
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "possettings_stub.sis /epoc32/data/z/system/install/possettings_stub.sis" \
    "rom/possettings.iby CORE_MW_LAYER_IBY_EXPORT_PATH(possettings.iby)" \
    "conf/positioningsettings.confml MW_LAYER_CONFML(positioningsettings.confml)" \
    "conf/positioningsettings_2002E685.crml MW_LAYER_CRML(positioningsettings_2002E685.crml)" \
    "rom/possettings_resources.iby LANGUAGE_MW_LAYER_IBY_EXPORT_PATH(possettings_resources.iby)"
   