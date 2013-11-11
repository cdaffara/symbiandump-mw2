/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Command Handler Request.
*
*/


// INCLUDE FILES

#include "cphsrvcomhandrequest.h" 
#include "phsrvutils.h" 




// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::CPhSrvComHandRequest
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequest::CPhSrvComHandRequest( 
    const RMessage2& aMessage, 
    const TPhCltComHandCommandParameters& aParams, 
    TInt aSubSessionHandle )
:   iPendingRequestPointer( aMessage ), 
    iComHandParams( aParams ), 
    iSubSessionHandle( aSubSessionHandle )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::CPhSrvComHandRequest
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequest::CPhSrvComHandRequest()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::NewLC
// 
// Static Symbian OS two-phase constructor. Return an instance
// and leave it on the cleanup stack.
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequest* CPhSrvComHandRequest::NewLC( 
    const RMessage2& aMessage, 
    const TPhCltComHandCommandParameters& aParams, 
    TInt aSubSessionHandle )
    {
    CPhSrvComHandRequest* self = new( ELeave ) CPhSrvComHandRequest( 
        aMessage, 
        aParams, 
        aSubSessionHandle );
    CleanupStack::PushL( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::NewL
// 
// Static Symbian OS two-phase constructor.
// -----------------------------------------------------------------------------
//
CPhSrvComHandRequest* CPhSrvComHandRequest::NewL()
    {
    CPhSrvComHandRequest* self = new( ELeave ) CPhSrvComHandRequest();
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::InformOfComHandRequestAttemptResult
// 
// Complete a client request which will indicate the result
// of attempting to perform a request.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequest::InformOfComHandRequestAttemptResult( 
    TPhCltPhoneResults aResultOfAttemptingRequest )
    {
    // Complete the client's pending request, indicating 
    // the result of the command handler request attempt.
    iPendingRequestPointer.Complete( aResultOfAttemptingRequest );
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::Cancel
// 
// Complete the client's request with KErrCancel.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequest::Cancel()
    {
    iPendingRequestPointer.Complete( KErrCancel );
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::SubSessionHandle
// 
// Return the handle of the subsession that initiated the
// original command handler request.
// -----------------------------------------------------------------------------
//
TInt CPhSrvComHandRequest::SubSessionHandle() const
    {
    return iSubSessionHandle;
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::ComHandParameters
// 
// Returns the command handler parameters.
// -----------------------------------------------------------------------------
//
const TPhCltComHandCommandParameters& 
    CPhSrvComHandRequest::ComHandParameters() const
    {
    return iComHandParams;
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::UpdateEmergencyRequest
// 
// Set new parameters to emergency request object.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequest::UpdateEmergencyRequest( 
    const RMessage2& aMessage, 
    const TPhCltComHandCommandParameters& aParams, 
    TInt aSubSessionHandle )
    {
    iPendingRequestPointer = aMessage;
    iComHandParams = aParams;
    iSubSessionHandle = aSubSessionHandle;
    }


// -----------------------------------------------------------------------------
// CPhSrvComHandRequest::ClearEmergencyRequest
// 
// Empties Emergency request data.
// -----------------------------------------------------------------------------
//
void CPhSrvComHandRequest::ClearEmergencyRequest()
    {
    iSubSessionHandle = 0;
    }


// End of File
