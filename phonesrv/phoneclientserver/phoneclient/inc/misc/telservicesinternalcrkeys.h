/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Telephony Services internal Central Repository keys
*
*/


#ifndef TELSERVICESINTERNALCRKEYS_H
#define TELSERVICESINTERNALCRKEYS_H

#include <e32std.h>

/******************************************************************************
* Telephony Services Variation API
* Keys under this category are used in defining Telephony Services variation.
******************************************************************************/
const TUid KCRUidTelSrvVariation = { 0x102029AC };

/**
* Telephony Services Local Variation Flags. Values are defined in 
* TelephonyServicesVariant.hrh.
*/
const TUint32 KTelSrvVariationFlags             = 0x00000001;

#endif      // TELSERVICESINTERNALCRKEYS_H

// End of file