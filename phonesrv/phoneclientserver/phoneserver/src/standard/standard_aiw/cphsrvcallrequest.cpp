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
* Description:  Call Request.
*
*/


// INCLUDE FILES

#include "cphsrvcallrequest.h" 
#include "phsrvutils.h" 


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::CPhSrvCallRequest
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CPhSrvCallRequest::CPhSrvCallRequest( 
    const RMessage2& aMessage, 
    CPhCltExtPhoneDialData& aArguments,
    TInt aSubSessionHandle )
    : iPendingRequestPointer( aMessage ),      
    iSubSessionHandle( aSubSessionHandle )
    {
    iCallArguments = &aArguments;
    }

// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::CPhSrvCallRequest
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CPhSrvCallRequest::CPhSrvCallRequest()
    {
    }

// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::NewLC
// 
// Static Symbian OS two-phase constructor. Return an instance
// and leave it on the cleanup stack.
// -----------------------------------------------------------------------------
//
CPhSrvCallRequest* CPhSrvCallRequest::NewLC( 
    const RMessage2& aMessage, 
    CPhCltExtPhoneDialData& aArguments,
    TInt aSubSessionHandle )
    {
    CPhSrvCallRequest* self = new( ELeave ) CPhSrvCallRequest( 
        aMessage, 
        aArguments, 
        aSubSessionHandle );
        CleanupStack::PushL( self );
    
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::NewL
// 
// Static Symbian OS two-phase constructor.
// -----------------------------------------------------------------------------
//
CPhSrvCallRequest* CPhSrvCallRequest::NewL()
    {
    CPhSrvCallRequest* self = new( ELeave ) CPhSrvCallRequest();
    
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::InformOfCallAttemptResult
// 
// Complete a client request which will indicate the result
// of attempting to dial a number.
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequest::InformOfCallAttemptResult( 
    TPhCltPhoneResults aResultOfAttemptingCall )
    {
    // Complete the client's pending request, indicating 
    // the result of the attempt.
    iPendingRequestPointer.Complete( aResultOfAttemptingCall );
    }



// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::Cancel
// 
// Complete the client's request with KErrCancel
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequest::Cancel()
    {
    iPendingRequestPointer.Complete( KErrCancel );
    }


// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::SubSessionHandle
// 
// Return the handle of the subsession that initiated the
// original dial request.
// -----------------------------------------------------------------------------
//
TInt CPhSrvCallRequest::SubSessionHandle() const
    {
    return iSubSessionHandle;
    }


// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::CallArguments
// 
// Returns the call arguments
// -----------------------------------------------------------------------------
//
CPhCltExtPhoneDialData& CPhSrvCallRequest::CallArguments() const
    {
    return *iCallArguments;
    
    }

// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::UpdateEmercgencyCall
// 
// Set new parameter to emergency object.
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequest::UpdateEmercgencyCall(
    const RMessage2& aMessage, 
    CPhCltExtPhoneDialData& aArguments, 
    TInt aSubSessionHandle)
    {
  
    __ASSERT_ALWAYS(  aArguments.TelephoneNumber() == KPhSrvUsedEmergencyCallNumber, 
        PhoneServerUtils::Panic( 
            EPhoneServerPanicEmergencyCallRequestFailure ) );
            
    iPendingRequestPointer = aMessage;
    iCallArguments = &aArguments;
    iSubSessionHandle = aSubSessionHandle;
    }

// -----------------------------------------------------------------------------
// CPhSrvNotificationManager::ClearEmergencyCall
// 
// Emtpies Emergency call data
// -----------------------------------------------------------------------------
//
void CPhSrvCallRequest::ClearEmergencyCall()
    {
    if ( iSubSessionHandle == 0 )
        {
        return;
        }
    __ASSERT_ALWAYS(  
        iCallArguments->TelephoneNumber() == KPhSrvUsedEmergencyCallNumber, 
        PhoneServerUtils::Panic( 
            EPhoneServerPanicEmergencyCallRequestFailure ) );
    iSubSessionHandle = 0;
    }


// End of File
