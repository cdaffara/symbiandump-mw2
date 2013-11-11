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
* Description:  SIMPLE Engine CenRep keys
*
*/



#ifndef SIMPLEENGINEPRIVATECRKEYS_H
#define SIMPLEENGINEPRIVATECRKEYS_H

const TUid KCRUIDSimpleVariation = {0x10281EEF};

// Suggested publish refresh time (sec)
const TUint32 KSimplePublishRefresh  = 0x00000001;
// Suggested subscribe refresh time (sec)
const TUint32 KSimpleSubscribeRefresh  = 0x00000002;
// Suggested subscribe WINFO refresh time
const TUint32 KSimpleSubscribeWinfoRefresh  = 0x00000003;
// Expiry time of API requests (sec)
const TUint32 KSimpleExpiryTime  = 0x00000004;

#endif      // SIMPLEENGINEPRIVATECRKEYS_H

