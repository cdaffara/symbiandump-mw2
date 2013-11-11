/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Utility class for SendUI.
*
*/




// INCLUDE FILES
#include <s32mem.h>
#include <e32base.h>
#include <mtclbase.h>
#include <tmsvpackednotifierrequest.h>

#include "SendUiCapabilityCheck.h"

// CONSTANTS
const TUid KSendUiNotifierPluginUid = {0x10282895};
const TInt32 KSendUiServiceMaxNameLength    = 40; // original defined in SendUIPrivateCRKeys.h
const TInt32 KSendUiNotifierBufferSize = KSendUiServiceMaxNameLength + sizeof(TUint32);

typedef TBuf8<KSendUiNotifierBufferSize> TTransferBuf;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck:CSendUiCapabilityCheck
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUiCapabilityCheck::CSendUiCapabilityCheck()
    : CActive( EPriorityStandard )
    {
     }
    
// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendUiCapabilityCheck* CSendUiCapabilityCheck::NewLC()
    {

    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::ConstructL
// -----------------------------------------------------------------------------
//
void CSendUiCapabilityCheck::ConstructL()
    {

    }


// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::CheckCapabilitiesL
// -----------------------------------------------------------------------------
//
TBool CSendUiCapabilityCheck::CheckCapabilitiesL(
    const TSecurityInfo& aSecurityInfo,
    CMsvSession& aSession,
    const TUid aMtmType,
    const TDesC& aServiceName )
    {
            
    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::~CSendUiCapabilityCheck
// Destructor
// -----------------------------------------------------------------------------
//
CSendUiCapabilityCheck::~CSendUiCapabilityCheck()
    {

    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::RunL
// -----------------------------------------------------------------------------
//
void CSendUiCapabilityCheck::RunL()
    {

    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::DoCancel
// -----------------------------------------------------------------------------
//
void CSendUiCapabilityCheck::DoCancel()
    {

    }

    
//  End of File
