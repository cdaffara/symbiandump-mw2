# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
# Initial Contributors:
# Nokia Corporation - initial contribution.
# Contributors:
# Description:  project file for positioning settings in control panel


TEMPLATE = subdirs
SYMBIAN_PLATFORMS = WINSCW \
    ARMV5

# Build.inf rules
BLD_INF_RULES.prj_mmpfiles += "posindicatorhelperserver/group/posindicatorhelperserver.mmp"
BLD_INF_RULES.prj_mmpfiles += "posreversegeocodeinterface/group/posreversegeocodeinterface.mmp"
BLD_INF_RULES.prj_mmpfiles += "posreversegeocodeplugin/group/posreversegeocodeplugin.mmp"

SUBDIRS += posindhelperclientlib/posindicatorhelperclientlib.pro
SUBDIRS += posindicatorplugin/posindicator.pro

DEFINES += BUILD_POSINDICATOR

BLD_INF_RULES.prj_exports += "rom/posindicator.iby CORE_MW_LAYER_IBY_EXPORT_PATH(posindicator.iby)" \
														 "rom/posindicatorstub.iby CORE_MW_LAYER_IBY_EXPORT_PATH(posindicatorstub.iby)" \														
														 "data/posindicator_stub.sis /epoc32/data/z/system/install/posindicator_stub.sis" \
                             "posindhelperclientlib/inc/posgeoaddress.h  ./inc/posgeoaddress.h" \
                             "posindhelperclientlib/inc/posindicatorglobal.h ./inc/posindicatorglobal.h" \
                             "posindhelperclientlib/inc/posindicatorinfo.h ./inc/posindicatorinfo.h" \
                             "posindicatorhelperserver/inc/posindicatorserverconst.h ./inc/posindicatorserverconst.h" \
                             "posreversegeocodeinterface/inc/posreversegeocodeinterface.h ./inc/posreversegeocodeinterface.h" \
                             "posreversegeocodeinterface/inc/posreversegeocodeinterface.hrh  ./inc/posreversegeocodeinterface.hrh" \
                              
# TODO:This is temporary solution. Once there is support to include bld.inf directly, this needs to be 
# removed
