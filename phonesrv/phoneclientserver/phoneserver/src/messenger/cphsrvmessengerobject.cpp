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
* Description:  Messenger Object.
*
*/


// INCLUDE FILES

#include "cphsrvmessengerobject.h" 
#include "cphsrvsubsessionbase.h" 
#include "cphsrvsession.h" // Phone server session. 
#include "phsrvdebuginfo.h" 


// CONSTANTS

// Null ID.
const TUint KPhSrvNullId = 0;

// The minimum Default message size.
const TInt KPhSrvMinDefSize = 1;


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::CPhSrvMessengerObject
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerObject::CPhSrvMessengerObject( 
    CPhSrvSubSessionBase& aSubSession )
:   iSubSession( aSubSession )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::~CPhSrvMessengerObject
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerObject::~CPhSrvMessengerObject()
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::ConstructL
// 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerObject::ConstructL(
    const TPhCltMessengerParameters& aParameters )
    {
    TInt size = aParameters.iDefaultMessageSize;
    if( size < KPhSrvMinDefSize )
        {
        // Default message size must be at least KPhSrvMinDefSize.
        User::Leave( KErrArgument );
        }

    iParameters.iCategoryUid = aParameters.iCategoryUid;
    iParameters.iDefaultMessageSize = size;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::NewLC
// 
// Static Symbian OS two-phase constructor. Return an instance
// and leave it on the cleanup stack.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerObject* CPhSrvMessengerObject::NewLC(  
    CPhSrvSubSessionBase& aSubSession,
    const TPhCltMessengerParameters& aParameters )
    {
    CPhSrvMessengerObject* self = new( ELeave ) CPhSrvMessengerObject( 
        aSubSession );
    
    CleanupStack::PushL( self );
    self->ConstructL( aParameters );

    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::InformOfMessengerRequest
// 
// Complete a client request which will indicate the result
// of attempting to perform a request.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerObject::InformOfMessengerRequest( 
    const TPhCltPhoneResults aResultOfAttemptingRequest,
    const TPhCltMessengerCommand aRequest )
    {
    // Complete the client's pending request, indicating 
    // the result of the messenger request attempt.
    if ( aRequest == EPhCltMesCommandSend )
        {
        // Send command.
        if ( iParameters.iSendDataValid )
            {
            iParameters.iSendStatus.Complete( aResultOfAttemptingRequest );
            iParameters.iSendDataValid = EFalse;
            }
        }
    else if ( aRequest == EPhCltMesCommandReceive )
        {
        // Receive command.
        if ( iParameters.iReceiveDataValid )
            {
            iParameters.iReceiveMessage.Complete( aResultOfAttemptingRequest );
            iParameters.iReceiveDataValid = EFalse;
            }
        }
    else
        {
        // Should never happen!
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::SubSessionHandle
// 
// Return the handle of the subsession that initiated the
// original messenger request.
// -----------------------------------------------------------------------------
//
TInt CPhSrvMessengerObject::SubSessionHandle() const
    {
    return iSubSession.SubSessionUniqueHandle();
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::SetActive
// 
// Set the request active.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerObject::SetActive( 
    const TPhCltMessengerParameters& aParameters,
    const RMessage2& aMessage,
    const TUint aSentDataId )
    {
    TPhCltMessengerCommand request = aParameters.iMessengerCommand;

    switch( request )
        {
        case EPhCltMesCommandSend:
            {
            // Set the Send data parameters.
            iParameters.iSendStatus = aMessage;
            iParameters.iSendDataValid = ETrue;
            iParameters.iSentDataId = aSentDataId;
            iParameters.iSendPayloadLength = aParameters.iSendPayloadLength;
            break;
            }
        case EPhCltMesCommandReceive:
            {
            // Set the Receive parameters.
            iParameters.iReceiveMessage = aMessage;

            iParameters.iReceiveBufferMaxSize = 
                aParameters.iReceiveBufferMaxSize;

            iParameters.iReceiveDataValid = ETrue;

            // Receive called, so Skip can not be active.
            iParameters.iSkipNextMessage = EFalse;
            iParameters.iSkippedSentDataId = KPhSrvNullId;
            break;
            }
        case EPhCltMesCommandSkip:
            {
            iParameters.iSkipNextMessage = ETrue;
            break;
            }

        default:
            // Should never happen!
            break;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::IsRequestActive
// 
// Check if there is already active request.
// -----------------------------------------------------------------------------
//
TBool CPhSrvMessengerObject::IsRequestActive( 
    const TPhCltMessengerCommand aRequest ) const
    {
    TBool isRequestActive = EFalse;

    switch( aRequest )
        {
        case EPhCltMesCommandSend:
            {
            isRequestActive = iParameters.iSendDataValid;
            break;
            }
        case EPhCltMesCommandReceive:
            {
            isRequestActive = iParameters.iReceiveDataValid;
            break;
            }
        case EPhCltMesCommandSkip:
            {
            isRequestActive = iParameters.iSkipNextMessage;
            break;
            }

        default:
            // Should never happen!
            break;
        }
    return isRequestActive;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::IsReadyToReceive
// 
// Check whether this object is ready to receive or not.
// If Receive is active but the length of the buffer 
// -----------------------------------------------------------------------------
//
TBool CPhSrvMessengerObject::IsReadyToReceive( 
    const TDesC8& aSentMessage,
    const TUint aSentDataId )
    {
    TBool ret = EFalse;
    const TInt sendDataLength = aSentMessage.Length();

    ret = IsMessageSkipped( aSentDataId );

      // If the message is not skipped, handle it here.
    if ( !ret )
        {
        // If receive data is valid, then we can be ready to receive.
        if( iParameters.iReceiveDataValid )
            {
            // If the sent message does not fit to receive buffer, then complete
            // Receive to indicate that bigger buffer is needed.
            if ( sendDataLength > 
                 iParameters.iReceiveBufferMaxSize )
                {
                CompleteReceive( aSentMessage, aSentDataId );
                }
            else
                {
                // Object can receive the sent message.
                ret = ETrue;
                }
            }
        }
    return ret;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::CompleteReceive
// 
// Complete the receive request.
// -----------------------------------------------------------------------------
//
TInt CPhSrvMessengerObject::CompleteReceive(
    const TDesC8& aSentMessage,
    const TUint aSentDataId )
    {
    _DPRINT( 4, "PhSrv.MesObj.CompleteReceive START" );   // debug print

    TBool ret = IsMessageSkipped( aSentDataId );
    TInt err = KErrNotReady;

    // If message is skipped, then we do not complete the request.
    if( ret )
        {
        err = KErrNone;
        }

    // If err is not KErrNone, then receive is handled.
    if ( err != KErrNone )
        {
        // If request is not active, then it can not be completed, and 
        // something has gone wrong. However, recover (do nothing).
        if ( iParameters.iReceiveDataValid )
            {
            // Receive request active, it is handled.

    _DPRINT( 4, "PhSrv.MesObj.CompleteReceive WRITE1" );   // debug print

            const TInt recBufMaxSize = 
                iParameters.iReceiveBufferMaxSize;

            // Write the whole sent message or the beginnig of it to client data
            // area, i.e ensure that receive message buffer is not overflowed.
            iSubSession.Write(
                iParameters.iReceiveMessage,
                1, 
                aSentMessage.Left( recBufMaxSize ) );

            TInt length = aSentMessage.Length();
            TPckgC < TInt > intPckg( length );

    _DPRINT( 4, "PhSrv.MesObj.CompleteReceive WRITE2" );   // debug print

            // Write the length information to user side.
            iSubSession.Write(
                iParameters.iReceiveMessage,
                2, 
                intPckg );

            // If whole sent message was written to receive buffer, then
            // completion was successful.
            if ( length <= recBufMaxSize )
                {
                err = KErrNone;
                }
            else
                {
                // The receive was not completed fully.
                iParameters.iSkippedSentDataId = aSentDataId;
                }

    _DPRINT( 4, "PhSrv.MesObj.CompleteReceive COMPLETE" );   // debug print

            // Complete the receive request.
            if ( !iParameters.iReceiveMessage.IsNull() )
                {
                iParameters.iReceiveMessage.Complete( KErrNone );
                }
            iParameters.iReceiveDataValid = EFalse;
            }
        }

    _DPRINT( 4, "PhSrv.MesObj.CompleteReceive END" );   // debug print

    return err;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::GetSendDataLength
// 
// Get the Send data.
// -----------------------------------------------------------------------------
//
TInt CPhSrvMessengerObject::GetSendDataLength()
    {
    return iParameters.iSendPayloadLength;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::GetSendData
// 
// Get the Send data.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerObject::GetSendData( TDes8& aDes ) const
    {
    // Read the Send data information from user side.
    // Do not leave, but will panic if pointer not valid descriptor.
    iSubSession.Read(
        iParameters.iSendStatus,
        1,
        aDes );
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::GetUid
// 
// Get the Uid.
// -----------------------------------------------------------------------------
//
const TUid& CPhSrvMessengerObject::GetUid() const
    {
    return iParameters.iCategoryUid;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::SentDataId
// 
// Get the sent data ID.
// -----------------------------------------------------------------------------
//
TUint CPhSrvMessengerObject::SentDataId()
    {
    return iParameters.iSentDataId;
    }



// -----------------------------------------------------------------------------
// CPhSrvMessengerObject::IsMessageSkipped
// 
// Return information whether the message is skipped or not.
// -----------------------------------------------------------------------------
//
TBool CPhSrvMessengerObject::IsMessageSkipped(
    const TUint aSentDataId )
    {
    TBool ret = EFalse; // By default message is not skipped.

    // If Skip is active, then everything is OK.
    if ( iParameters.iSkipNextMessage )
        {
        // If the sent data is the same as that it was earlier, then it is 
        // skipped. If the sent message is not the same, then it is not skipped.
        if ( iParameters.iSkippedSentDataId == aSentDataId )
            {
            // This message is skipped.
            ret = ETrue;
            }
        else
            {
            // This message is not skipped.
            iParameters.iSkipNextMessage = EFalse;
            iParameters.iSkippedSentDataId = KPhSrvNullId; // Set to not valid.
            }
        }
    return ret;
    }


// End of File
