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
    CActiveScheduler::Add( this );
    iNotifierUid = KSendUiNotifierPluginUid;
    }
    
// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendUiCapabilityCheck* CSendUiCapabilityCheck::NewLC()
    {
    CSendUiCapabilityCheck* self = new (ELeave) CSendUiCapabilityCheck();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::ConstructL
// -----------------------------------------------------------------------------
//
void CSendUiCapabilityCheck::ConstructL()
    {
    User::LeaveIfError( iNotifier.Connect() );
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
    TBool capabilitiesOK = EFalse;
    // get required capabilities
    TCapabilitySet caps;
    aSession.GetMtmRequiredCapabilitiesL( aMtmType, caps );

    // check client capabilities with required capabilities
    if( aSecurityInfo.iCaps.HasCapabilities( caps ) )
        {
        capabilitiesOK = ETrue;
        }
    else
        {
        // compile a message buffer containing service name and service ID
        TInt nameLength = aServiceName.Length();
        CBufFlat* buffer = CBufFlat::NewL( nameLength + sizeof(TUint32) );
	    CleanupStack::PushL( buffer );
	    
	    RBufWriteStream bufStream;
	    bufStream.Open( *buffer );
	    bufStream.WriteUint32L( nameLength );
	    
	    if ( nameLength )
		    {
	        bufStream << aServiceName;
		    }
	    bufStream.WriteUint32L( aSecurityInfo.iSecureId );
	    bufStream.CommitL();
	    bufStream.Close();
	 
    	TTransferBuf transferBuffer;
        buffer->Read( 0, transferBuffer, buffer->Size() );
    	CleanupStack::PopAndDestroy( buffer );

  		iNotifier.StartNotifierAndGetResponse( iStatus, iNotifierUid, transferBuffer, iNotifierResult );
        SetActive();
        iWait.Start();
         
        if ( iStatus == KErrNone )
        	{
        	capabilitiesOK = ETrue;
        	}
        }
        
    return capabilitiesOK;            
    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::~CSendUiCapabilityCheck
// Destructor
// -----------------------------------------------------------------------------
//
CSendUiCapabilityCheck::~CSendUiCapabilityCheck()
    {
    iNotifier.Close();
    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::RunL
// -----------------------------------------------------------------------------
//
void CSendUiCapabilityCheck::RunL()
    {
   	iWait.AsyncStop();
    }

// -----------------------------------------------------------------------------
// CSendUiCapabilityCheck::DoCancel
// -----------------------------------------------------------------------------
//
void CSendUiCapabilityCheck::DoCancel()
    {
    iNotifier.NotifyCancel();
    }

    
//  End of File
