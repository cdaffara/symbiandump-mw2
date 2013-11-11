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
# Description:  project file for Locsysui                                                    
#                                                                    


TEMPLATE = subdirs

SYMBIAN_PLATFORMS = WINSCW ARMV5

SUBDIRS += possettings/possettings.pro
SUBDIRS += privacyverifiernotifierui/privacyverifiernotifierui.pro
SUBDIRS += posindicator/posindicator.pro

BLD_INF_RULES.prj_exports += "conf/loclocalvariation.confml  MW_LAYER_CONFML(loclocalvariation.confml)" \
							 "conf/loclocalvariation_20022D83.crml  MW_LAYER_CRML(loclocalvariation_20022D83.crml)" 
   