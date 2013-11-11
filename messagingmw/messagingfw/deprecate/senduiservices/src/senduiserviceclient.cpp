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
* Description:   Communicates message data to SendUILauncher server application
*
*/



#include <aknappui.h>
#include <AknServerApp.h>
#include <s32mem.h>

#include <CMessageData.h>
#include <SendUiConsts.h>

#include "senduiserviceclient.h"
#include "senduiserviceslog.h"
#include "senduilauncher.h"
#include "senduiservicecommands.h"

typedef TBuf8<KTransferBufferSize> TTransferBuf;
// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Launch SendUi
// ---------------------------------------------------------------------------
//
void RSendUiServiceClient::LaunchSendUiL( 
    TUid& aServiceUid,
    TUid& aServiceProviderUid, 
    TBool& aLaunchEmbedded, 
    const CMessageData* aMessageData )
    {
   }

// ---------------------------------------------------------------------------
// Return service identifier
// ---------------------------------------------------------------------------
//
TUid RSendUiServiceClient::ServiceUid() const
    {
      }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
RSendUiServiceClient::~RSendUiServiceClient() 
    { 
    }
