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

SUBDIRS += locnotificationengine/locnotificationengine.pro
SUBDIRS += posverificationdialogplugin/posverificationdialogplugin.pro

# Build.inf rules
BLD_INF_RULES.prj_exports += "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "install/privacydialogs_stub.sis /epoc32/data/z/system/install/privacydialogs_stub.sis" \
    "rom/privacydialogs.iby CORE_MW_LAYER_IBY_EXPORT_PATH(privacydialogs.iby)"

symbian {
BLD_INF_RULES.prj_mmpfiles = "./locutils/group/locutils.mmp" \
                             "./locprivacyserver/group/locprivacyserver.mmp" \
                             "./contactresolversession/group/contactresolversession.mmp" \
                             "./locverifier/group/locverifierdlg.mmp"
}
