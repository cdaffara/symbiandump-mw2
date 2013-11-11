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
* Description:  Messenger Request Manager.
*                The stub version of this file can be found from file
*                CPhSrvMessengerRequestManagerStub.cpp
*
*/



// INCLUDES
#include "cphsrvmessengerrequestmanager.h" 
#include "cphsrvmessengerobject.h" 
#include "cphsrvsubsessionbase.h" 
#include "phsrvutils.h" 
#include "phsrvmessengertypes.h" 
#include "phsrvdebuginfo.h" 


// CONSTANTS

// The default size of the Messenger object storage.
const TInt KPhSrvMessengerObjectGranularity = 4;

// The size of the Sent message's storage.
const TInt KPhSrvMessengerPendingSendRequestGranularity 
    = KPhSrvMessengerObjectGranularity;




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::CPhSrvMessengerRequestManager
// 
// Constructor.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerRequestManager::CPhSrvMessengerRequestManager( 
    CPhSrvServer& aServer )
:   iServer( aServer ),
    iSentDataIdGenerator( 1 ), // Is never 0.
    iSendProcessOngoing( EFalse )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::~CPhSrvMessengerRequestManager
// 
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvMessengerRequestManager::~CPhSrvMessengerRequestManager()
    {
    delete iDefaultSizeBuffer;

    RMessengerArray* array = SendChain();
    if ( array )
        {
        // Objects not deleted, because they are not owned.
        array->Reset();
        array->Close();
        delete array;
        }

    array = MessengerArray();
    if ( array )
        {
        array->ResetAndDestroy();
        array->Close();

        delete array;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::ConstructL
// 
// Complete construction.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::ConstructL()
    {
    iDefaultSizeBuffer = HBufC8::NewMaxL( KPhCltDefMesSize );

    // Space for messenger objects (clients)
    iMessengerObjects = new ( ELeave ) RPointerArray< 
        CPhSrvMessengerObject >( KPhSrvMessengerObjectGranularity );

    // Space for the Send queue.
    iSendChain = new ( ELeave ) RPointerArray< CPhSrvMessengerObject >( 
        KPhSrvMessengerPendingSendRequestGranularity );
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequest
// 
// Initiate a messenger request - called by the external messenger interface.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequest( 
    CPhSrvSubSessionBase& aSubSession,
    const TPhCltMessengerParameters& aParameters,
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequest START" );   // debug print

    // Panic if this is not the only request from this subsession.
    if ( RequestAlreadyExists( 
             aSubSession, 
             aParameters,
             aMessage ) )
        {
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequest PANIC" );   // debug print

        // First delete all data related to this object.
        NegotiatorRequestDelete( aSubSession );

        PhoneServerUtils::PanicClient( 
            EPhoneServerPanicTooManyRequestsOngoing,
            aMessage );
        }
    else
        {
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequest HANDLE" );   // debug print

        // Request was set successfully, so handle it.
        TPhCltMessengerCommand command = aParameters.iMessengerCommand;

        // Update the messenger chain.
        UpdateMessengerChain( iRequestObject, command );

        // Clear the request object.
        iRequestObject = NULL;

        // Complete Skip request.
        if ( command == EPhCltMesCommandSkip )
            {
            aMessage.Complete( KErrNone );
            }
        }

    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequest END" );   // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequestCancel
// 
// Cancel a messenger request handling.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequestCancel( 
    CPhSrvSubSessionBase& aSubSession,
    const TPhCltMessengerCommand aRequestType )
    {
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestCancel START" );   // debug print

    RMessengerArray* messengerObjects = MessengerArray();

    if ( aRequestType == EPhCltMesCommandSend ||
         aRequestType == EPhCltMesCommandReceive )
        {
        // Obtain the subsession's handle
        const TInt handle = aSubSession.SubSessionUniqueHandle();

        // Now look for any object that match.
        const TInt count = messengerObjects->Count();
        for( TInt i = 0; i < count; i++ )
            {
            CPhSrvMessengerObject* entry = ( *messengerObjects )[i];
            if ( entry->SubSessionHandle() == handle )
                {
                // Found matching entry, so cancel it's request.
                entry->InformOfMessengerRequest( KErrCancel, aRequestType );


                TPhCltMessengerCommand request = aRequestType;
                if ( aRequestType == EPhCltMesCommandSend )
                    {
                    // Send request is canceled.
                    request = EPhCltMesCommandCancelSend;
                    }
                else
                    {
                    // Receive request is canceled.
                    request = EPhCltMesCommandCancelReceive;
                    }

                // Update the messenger chain.
                UpdateMessengerChain( entry, request );
                }
            }
        }
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestCancel STOP" );   // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequestConstructL
// 
// Reserve all space needed for the subsession given as parameter.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequestConstructL( 
    CPhSrvSubSessionBase& aSubSession,
    const TPhCltMessengerParameters& aParameters,
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestConstructL START" ); // debug print

    // Create pending request
    CPhSrvMessengerObject* object = CPhSrvMessengerObject::NewLC(
        aSubSession,
        aParameters );

    RMessengerArray* sendArray = SendChain();
    RMessengerArray* messengerObjects = MessengerArray();

    /////////////////////////////////////////////////////////////////
    // First try that there is enough space in send array.
    /////////////////////////////////////////////////////////////////
    const CPhSrvMessengerObject* entry = NULL;
    TInt err = KErrNone;
    // The number of existing messenger objects + the new one.
    const TInt count = ( messengerObjects->Count() + 1 );

    TInt i = 0;
    for ( i = 0; ( i < count ) && ( err == KErrNone ); i++ )
        {
        // If Append failed, there is not enough memory and err != KErrNone.
        err = sendArray->Append( entry );
        }

    // Remove now the added objects -> Buffer remains reserved for them.
    const TInt sendChainCount = ( sendArray->Count() - 1 );
    for( i = sendChainCount; i >= 0; i-- )
        {
        if( ( * sendArray )[i] == NULL )
            {
            // This object was added, so remove it.
            sendArray->Remove( i );
            }
        }

    // Leave if addition to send chain were not successful.
    User::LeaveIfError( err );

    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestConstructL MIDDLE" ); // debug print

    /////////////////////////////////////////////////////////////////
    // There was enough space in send array, so next try to append the 
    // object to the object array.
    /////////////////////////////////////////////////////////////////
    User::LeaveIfError( 
        messengerObjects->Append( object ) );
    CleanupStack::Pop( object );

    aMessage.Complete( KErrNone );

    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestConstructL STOP" );  // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::NegotiatorRequestDelete
// 
// Delete all space reserved for the subsession given as parameter.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::NegotiatorRequestDelete( 
    CPhSrvSubSessionBase& aSubSession )
    {
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestDelete START" );   // debug print

    // Obtain the subsessions handle
    const TInt handle = aSubSession.SubSessionUniqueHandle();

    RMessengerArray* messengerObjects = MessengerArray();

    // Now look for any object that match.
    TInt count = messengerObjects->Count() - 1;
    for( TInt i = count; i >= 0; i-- )
        {
        CPhSrvMessengerObject* entry = ( *messengerObjects )[i];
        if ( entry->SubSessionHandle() == handle )
            {
            // Found matching entry, so cancel it and clean up.

            // This object is not in handled objects anymore.
            // This means also that objects receive is also canceled.
            messengerObjects->Remove( i );

            // Update the messenger chain, cancel Send.
            UpdateMessengerChain( entry, EPhCltMesCommandCancelSend );

            // Send and receive were canceled, so this entry can be destroyed.
            delete entry;
            }
        }
    _DPRINT( 4, "PhSrv.MRM.NegotiatorRequestDelete STOP" );   // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::UpdateMessengerChain
// 
// Update the request objects.
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::UpdateMessengerChain(
    CPhSrvMessengerObject* aEntry,
    TPhCltMessengerCommand aCommand )
    {
    _DPRINT( 4, "PhSrv.MRM.Negotiator.Update START" );   // debug print

    TInt err = KErrNone;
    RMessengerArray* sendArray = SendChain();


    if ( aEntry )
        {
        switch ( aCommand )
            {
            case EPhCltMesCommandSend:
                {
                // If this is new request, append it to send queue.
                if( sendArray->Find( aEntry ) == KErrNotFound )
                    {
                    err = sendArray->Append( aEntry );
                    if ( err )
                        {
                        // Should never happen because of space pre-reservation
                        // in construction phase. However, recover.

                        // If Append did not succeed, inform Sender of failure.
                        aEntry->InformOfMessengerRequest( 
                            err,
                            EPhCltMesCommandSend );
                        }
                    }

                // If Append was not performed or if it was successful.
                if ( !err )
                    {
                    // Handle all Send requests that exist.
                    HandleAllSendRequests( aEntry, sendArray );
                    }
                break;
                }
            case EPhCltMesCommandReceive:
            case EPhCltMesCommandSkip:
            case EPhCltMesCommandCancelReceive:
                {
                // Handle all Send requests that exist.
                HandleAllSendRequests( NULL, sendArray );
                break;
                }

            case EPhCltMesCommandCancelSend:
                {
                // Remove this Send object from chain.
                // Pending Receive requests not completed.
                TInt location = sendArray->Find( aEntry );
                if ( location >= 0 )
                    {
                    // If this is the first object in Send chain, then
                    // Send request processing is canceled.
                    if( aEntry == ( *sendArray )[0] )
                        {
                        iSendProcessOngoing = EFalse;
                        }
                    sendArray->Remove( location );
                    }

                // Handle all other Send requests that exist.
                HandleAllSendRequests( NULL, sendArray );
                break;
                }

            default:
                // Should never happen!
                break;
            }
        }
    _DPRINT( 4, "PhSrv.MRM.Negotiator.Update STOP" );   // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::HandleAllSendRequests
// 
// 
// -----------------------------------------------------------------------------
//
void CPhSrvMessengerRequestManager::HandleAllSendRequests(
    CPhSrvMessengerObject* aEntry,
    RMessengerArray* aSendArray )
    {
    CPhSrvMessengerObject* entry = aEntry;

    // If aEntry is NULL, try to set it as valid object.
    if ( !entry )
        {
        if ( aSendArray->Count() )
            {
            entry = ( *aSendArray )[0]; // Take the first object in send chain.
            }
        }

    // Handle all Send requests that exists.
    while ( HandleSendRequest( entry, aSendArray ) )
        {
        entry = NULL;
        if ( aSendArray->Count() )
            {
            entry = ( *aSendArray )[0]; // Take the first object in send chain.
            }
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::HandleSendRequest
// 
// 
// -----------------------------------------------------------------------------
//
TBool CPhSrvMessengerRequestManager::HandleSendRequest( 
    CPhSrvMessengerObject* aEntry,
    RMessengerArray* aSendArray )
    {
    TBool ret = EFalse;
    RMessengerArray* messengerObjects = MessengerArray();


    // Handling is done only if there is Send request and 
    // aEntry is valid object.
    if( aSendArray->Count() && aEntry )
        {
        if ( aEntry == ( *aSendArray )[0] )
            {
            // This was the first Send operation so complete Receives.

            // Some pre-work needs to be done if this Send has not been 
            // handled before.
            if( !iSendProcessOngoing )
                {
                // Now this Send request process is ongoing.
                iSendProcessOngoing = ETrue;

                const TInt sentDataLength = aEntry->GetSendDataLength();

                if ( sentDataLength > 
                     iDefaultSizeBuffer->Des().MaxLength() )
                    {
                    // The sent data is longer than the default buffer.
                    // We need to try reserve bigger buffer. If this does not
                    // succeed, then only the current size buffer is used in 
                    // sending.

                    HBufC8* variableSizeBuffer = NULL;
                    variableSizeBuffer = HBufC8::New( sentDataLength );
                    if ( variableSizeBuffer != NULL )
                        {
                        // Reservation was successful, so use reserved buffer
                        // as a new default buffer. 
                        // But first delete the old buffer.
                        if( iDefaultSizeBuffer )
                            {
                            delete iDefaultSizeBuffer;
                            iDefaultSizeBuffer = NULL;
                            }
                        iDefaultSizeBuffer = variableSizeBuffer;
                        }
                    }
                // Read the sent message.
                TPtr8 ptr = iDefaultSizeBuffer->Des();
                aEntry->GetSendData( ptr );
                }


            const TDesC8& sendData = *iDefaultSizeBuffer;
            const TUid& senderUid = aEntry->GetUid();
            const TUint sentDataId = aEntry->SentDataId();

            TBool completeSend = ETrue;
            TInt interested = 0;

            TInt count = messengerObjects->Count();

            ////////////////////////////////////////////////////////
            // Check that all the objects are ready to receive.
            ////////////////////////////////////////////////////////
            for( TInt i = 0; i < count; i++ )
                {
                CPhSrvMessengerObject* entry = ( *messengerObjects )[i];

                // Check only other objects.
                if ( aEntry->SubSessionHandle() != entry->SubSessionHandle() )
                    {
                    // We need to care only of those objects that want to
                    // listen messages same type as the sent was.
                    if( senderUid == entry->GetUid() )
                        {
                        // This object wants to receive this sent message.
                        interested++;

                        if ( !entry->IsReadyToReceive( sendData, sentDataId ) )
                            {
                            // The object was not ready to receive, so Send can 
                            // not be completed yet.
                            completeSend = EFalse;
                            }
                        }
                    }
                }


            ////////////////////////////////////////////////////////
            // If all objects were ready to receive, complete them.
            // If there were no objects interested of the sent message,
            // do nothing.
            ////////////////////////////////////////////////////////
            if ( completeSend && interested )
                {
                for( TInt i = 0; i < count; i++ )
                    {
                    CPhSrvMessengerObject* entry = ( *messengerObjects )[i];

                    // Complete only other's Receive requests.
                    if ( aEntry->SubSessionHandle() != 
                         entry->SubSessionHandle() )
                        {
                        // We complete only those object's receives that have
                        // the same category UID as the sent was.
                        if( senderUid == entry->GetUid() )
                            {
                            TInt err = entry->CompleteReceive( 
                                sendData,
                                sentDataId );
                            if( err )
                                {
                                // This should never fail because pre-check was
                                // successful. However, you can try to recover.
                                }
                            }
                        }
                    }
                // All receives were compeled, so complete Send.
                aEntry->InformOfMessengerRequest( 
                    KErrNone,
                    EPhCltMesCommandSend );
                // Remove the object whose Send was handled from chain.
                aSendArray->Remove( 0 );

                // The send process was completed.
                iSendProcessOngoing = EFalse;

                // Send was fully handled.
                ret = ETrue;
                }
            }
        }
    return ret;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::RequestAlreadyExists
// 
// Check whether there already is request(s) for the subsession given as 
// parameter. If not, then sets it active.
// -----------------------------------------------------------------------------
//
TBool CPhSrvMessengerRequestManager::RequestAlreadyExists( 
    const CPhSrvSubSessionBase& aSubSession,
    const TPhCltMessengerParameters& aParameters,
    const RMessage2& aMessage )
    {
    TBool isThereRequest = EFalse;
    const TPhCltMessengerCommand command = aParameters.iMessengerCommand;

    // Check is only done for Send and Receive and Skip.
    if ( command == EPhCltMesCommandSend ||
         command == EPhCltMesCommandReceive ||
         command == EPhCltMesCommandSkip )
        {
        // Obtain the subsessions handle
        const TInt handle = aSubSession.SubSessionUniqueHandle();

        RMessengerArray* messengerObjects = MessengerArray();

        // Look for any pending requests that match the handle.
        TInt i = 0;
        for( i = 0; i < messengerObjects->Count(); i++ )
            {
            CPhSrvMessengerObject* entry = ( *messengerObjects )[i];
            if ( entry->SubSessionHandle() == handle )
                {
                isThereRequest = entry->IsRequestActive( command );
                if ( !isThereRequest )
                    {
                    TUint sentDataId = 0;
                    if( command == EPhCltMesCommandSend )
                        {
                        // This was sent command, so set ID for it.
                        sentDataId = iSentDataIdGenerator;
                        // New Id for next sent message, must never be 0!
                        iSentDataIdGenerator++;
                        if( !iSentDataIdGenerator )
                            {
                            // Value was 0, so set to 1.
                            iSentDataIdGenerator++;
                            }
                        }
                    // Request was not active, so set it active.
                    entry->SetActive( aParameters, aMessage, sentDataId );
                    }

                // Set the object who initiated this request.
                iRequestObject = entry;
                break;
                }
            }
        }
    return isThereRequest;
    }



// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::MessengerArray
// 
// 
// -----------------------------------------------------------------------------
//
RMessengerArray* CPhSrvMessengerRequestManager::MessengerArray()
    {
    return iMessengerObjects;
    }


// -----------------------------------------------------------------------------
// CPhSrvMessengerRequestManager::SendChain
// 
// 
// -----------------------------------------------------------------------------
//
RMessengerArray* CPhSrvMessengerRequestManager::SendChain()
    {
    return iSendChain;
    }


// End of File
