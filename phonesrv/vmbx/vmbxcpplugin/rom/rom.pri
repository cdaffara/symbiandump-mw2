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

symbian: {
  BLD_INF_RULES.prj_exports += \
  "$${LITERAL_HASH}include<platform_paths.hrh>" \
  "rom/vmbxcpplugin.iby CORE_MW_LAYER_IBY_EXPORT_PATH(vmbxcpplugin.iby)" \
   "rom/vmbxcpplugin_stub.sis /epoc32/data/z/system/install/vmbxcpplugin_stub.sis"
}