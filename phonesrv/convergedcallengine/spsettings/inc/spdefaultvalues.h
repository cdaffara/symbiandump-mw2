/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Service provider settings default values
*
*/


#ifndef SPDEFAULTVALUES_H
#define SPDEFAULTVALUES_H

#include "spdefinitions.h"

// Publish & Subscribe key
const TUint KSPNotifyChangeKey = 0x2000B151;
const TUint KSPSupportInternetCallKey = 0x2000B15F;
const TUint KSPSupportCallOutKey = 0x2000B160;
const TUint KSPSupportVoIPSSKey = 0x2000B161;

/** Initial default value of feature check P&S key */
const TInt KSPInitFeatureValue = -1;

/** default VoIP service Id */
const TInt KSPDefaultVoIPServiceId = 2;

///**  List of default values of predefined service settings properties */
//const TInt KPropertyServiceAttributeMask             = KSPValueNotSet;
//const TInt KPropertyCTIPluginId                      = KSPValueNotSet;
//const TInt KPropertyCLIPluginId                      = KSPValueNotSet;
//const TInt KPropertyCallLoggingPluginId              = KSPValueNotSet;
//const TInt KPropertyCallStateIndicatorPluginId       = KSPValueNotSet;
//const TInt KPropertyCallMenuHandlerPluginId          = KSPValueNotSet;
//const TInt KPropertyCallProviderPluginId             = KSPValueNotSet;
//const TInt KPropertyPCSPluginId                      = KSPValueNotSet;
//const TInt KPropertyVoIPSubServicePluginId           = KSPValueNotSet;
//const TInt KPropertyPresenceSubServicePluginId       = KSPValueNotSet;
//const TInt KPropertyIMSubServicePluginId             = KSPValueNotSet;
//const TInt KPropertyVMBXSubServicePluginId           = KSPValueNotSet;
//const TInt KPropertyConfigurationPluginId            = KSPValueNotSet;
//const TInt KPropertyProfileId                        = KSPValueNotSet;
//const TInt KPropertyServiceSetupPluginId             = KSPValueNotSet;
//const TInt KPropertyServiceConnectivityPluginId      = KSPValueNotSet;
//
//
///**  List of default values of predefined sub service settings properties */
//
//const TOnOff KSubPropertyVmbxConnPreference                 = EOONotSet;
//const TInt KSubPropertySNAPNetworkId                      = KSPValueNotSet;
//const TInt KSubPropertyMWISubscribeInterval               = KSPValueNotSet;
//const TOnOff KSubPropertyVMBXAddressEditLock                = EOONotSet;



#endif      // SPDEFAULTVALUES_H


