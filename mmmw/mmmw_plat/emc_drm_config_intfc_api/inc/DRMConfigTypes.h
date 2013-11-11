/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header of DRMConfigTypes object.
*
*/

#ifndef DRMCONFIGTYPES_H
#define DRMCONFIGTYPES_H

enum TDRMType {
    ENone,
    EOMA1,
    EOMA2,
    EWMDRM
};

enum TDRMAllowedOutputDevice {
    EAudioAllowAll,
    EAudioAllowAnalog,
    EAudioAllowFMTx,
    EAudioAllowBTA2DP,
    EAudioAllowBTHFPHSP,
    EAudioAllowUplink,
    EAudioAllowUSB,
    EAudioAllowRecording,
    EAudioAllowVisualization,
    EVideoAllowAnalog,
    EVideoAllowMacroVision,
    /**
	* RIM CR 417-7642: HDMI with HDCP to Resctricted Audio Output API
	* Due to addition of new ENUMs to CRestrictedAudioOutput::TAllowedOutputPreference for HDMI and HDCP
	* EAllowAudioHDMI and EAllowAudioHdmiHdcpRequired,the same is matched by adding
	* EAudioAllowHDMI and EAudioAllowHdmiHdcpRequired
	*/
    EAudioAllowHDMI,
    EAudioAllowHdmiHdcpRequired
};

#endif // DRMCONFIGTYPES_H

// End of file
