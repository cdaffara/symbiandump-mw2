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
* Description:  Service provider settings central repository keys
*
*/


#ifndef SPSETTINGSCRKEYS_H
#define SPSETTINGSCRKEYS_H

/** Central Repisitory UID of service provider settings*/
const TUid KCRUidSPSettings = { 0x10282E7F };


/** Service ID counter. Contains the next available service ID */
const TUint32 KServiceIdCounter = 0x00000000;

const TUint32 KSPStartKey = 0x00001000;
const TUint32 KSPColIncrement = 0x00001000; 
const TUint32 KSPColMask = 0xFFFFF000;
const TInt KSPColCount = 81; 

#endif      // SPSETTINGSCRKEYS_H


