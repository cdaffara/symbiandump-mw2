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
# Description: Project file for building phonesrv components 
#
#

TEMPLATE = subdirs
CONFIG += ordered

symbian: {
:BLD_INF_RULES.prj_mmpfiles += $$LITERAL_HASH"include \"group/bld.inf\""
SUBDIRS += telutils/xqtelephonyservice
SUBDIRS += telutils/dialpad
SUBDIRS += telutils/keysequencerecognitionservice
SUBDIRS += vmbx/vmbxcpplugin
SUBDIRS += vmbx/vmbxengine
SUBDIRS += cellular/sssettings/xqbindings/sssettingswrapper
SUBDIRS += cellular/telephonysettings/xqbindings/psetwrapper
SUBDIRS += satui/satapp
SUBDIRS += cellular/psuinotes
}
