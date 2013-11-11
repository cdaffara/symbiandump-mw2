/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SSSettings private Central Repository keys.
*
*/


#ifndef SSSETTINGSPRIVATECRKEYS_H
#define SSSETTINGSPRIVATECRKEYS_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

/**
* Supplementary Service Settings API
* Supplementary and CPHS service settings.
*/
const TUid KCRUidSupplementaryServiceSettings = {0x101F87E3};

/**
* ALS line for outgoing calls, integer value
*
* This setting is only for SSSettings internal use.
*
* 0 = ALS not supported
* 1 = Primary line
* 2 = Secondary line
*/
const TUint32 KSettingsAlsLine = 0x00000001;

/**
* CLIR (Calling Line Idefitication Restriction) mode.
*
* 0 = Network default
* 1 = Explicit invoke
* 2 = Explicit suppress
*/
const TUint32 KSettingsCLIR = 0x00000002;

/**
* CUG mode.
*
*/
const TUint32 KSettingsCUG = 0x00000003;

/**
* Previous CUG index.
*/
const TUint32 KSettingsPreviousCUGIndex = 0x00000004;


#endif      // SSSETTINGSPRIVATECRKEYS_H

// End of file