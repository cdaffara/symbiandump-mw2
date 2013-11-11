/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definitions
*
*/
/*
* ==============================================================================
*  Name        : ccptestplugindefs.h
*
*  Description : Definitions
*
* =============================================================================*/

#ifndef CCPTESTPLUGINDEFS_H
#define CCPTESTPLUGINDEFS_H

/* Implementation UID
*  - for PS plugin use 0x10282589 (271066505)
*  - for VCC plugin use 0x2000CFAA (536924074)
*  - for CS plugin use 0x102828E5 (271067365)
*/

const TInt KPluginPsImplementationUid = 0x10282589;
const TInt KPluginVccImplementationUid = 0x2000CFAA;
const TInt KPluginCsImplementationUid = 0x102828E5;

// a UID for identifying P&S events, DLL UID is used
const TInt KPSUidEventPsRequestValue = 0x01282F0D;
const TInt KPSUidEventCsRequestValue = 0x01282E3F;
const TUint32 KPropertyKeyEventIdentifier = 0x00000001;

const TInt KMaxKeywordLength = 50;

const TInt KMaxAddressName = 256;

/* If this flag is defined, plugin subscibes
*  to property changes that can be used to
*  control plugin
*/
#define __ENABLE_PLUGIN_COMMANDS

/* If this flag is defined, plugin executes
*  actions with a delay
*/
//#define __PLUGIN_ASYNCHRONOUS_ACTION

#endif //CCPTESTPLUGINDEFS_H