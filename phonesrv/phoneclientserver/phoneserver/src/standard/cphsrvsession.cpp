/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Session.
*
*/



// INCLUDE FILES
#include "cphsrvserver.h" 
#include "cphsrvsession.h" 
#include "cphsrvsubsessionbase.h" 
#include "phsrvsubsessionfactory.h" 
#include "phcltclientserver.h" 

// CONSTANTS



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhSrvSession* CPhSrvSession::NewL( CPhSrvServer& aServer )   
    {
    CPhSrvSession* self = new ( ELeave ) CPhSrvSession();
    CleanupStack::PushL( self );
    self->ConstructL( aServer );
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::CPhSrvSession
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CPhSrvSession::CPhSrvSession() 
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::~CPhSrvSession
// Destructor.
// -----------------------------------------------------------------------------
//
CPhSrvSession::~CPhSrvSession()
    {
    if ( iServer )
        {
        iServer->CancelCreateAll( *this );
        }

    delete iObjectIx;
   
   if ( iContainer )
        {
        iServer->RemoveContainer( iContainer );
        iContainer = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::ServiceL
// 
// Calls DispatchMessageL under trap harness.
// -----------------------------------------------------------------------------
//
void CPhSrvSession::ServiceL( const RMessage2& aMessage )  
    {
    TRAPD( err, ProcessRequestL( aMessage ) );
    if ( err != KErrNone )
        {
        aMessage.Complete( err );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::ProcessRequestL
// 
// Checks the function specified by the client message
// and depending on the type, performs an appropriate action.
//
// If the function is one of the "special" factory sub-session
// creation op-codes, then this session will process it.
//
// If the function is a generic subsession function, then the
// subsession which can handle this function is identified by
// it's unique handle, and it is asked to process the request.
//
// -----------------------------------------------------------------------------
//
void CPhSrvSession::ProcessRequestL( const RMessage2& aMessage )
    {
    if ( HandleCommandL( aMessage ) )
        {
        return;
        }

    const TInt function = aMessage.Function();
    const TUint32 sid = aMessage.SecureId().iId;
    
    // Is this a message that results in a new subsession?
    if  ( PhSrvSubSessionFactory::PhSrvSubSessionFactoryIsCreationFunction( 
          function ) )
        {
        if ( !PhSrvSubSessionFactory::PhSrvSubSessionFactoryIsCreationAllowed(
            function,
            sid ) )
            {
            aMessage.Complete( KErrPermissionDenied );
            }
        else
            {
            // Need to create a new subsession
            CPhSrvSubSessionBase* subSession = 
                PhSrvSubSessionFactory::PhSrvSubSessionFactoryCreateLC( 
                    function, 
                    *this );

            // Add to container (takes ownership)
            iContainer->AddL( subSession );
            if (function != EPhoneServerImageHandlerSubSessionOpen )
                {
                CleanupStack::Pop( subSession );    
                }
            // Get a handle for the object
            const TInt handle = iObjectIx->AddL( subSession );

            // Inform client of the handle its been allocated
            TPckg<TInt> handlePckg( handle );
            Write( 
                aMessage,
                3,
                handlePckg );

            if ( !aMessage.IsNull() )
                {
                // Complete the message
                aMessage.Complete( KErrNone );
                }
             }    
         }
    else
        {
        // Find an appropriate object and pass the message to it
        // for processing...
        const TInt handle = aMessage.Int3();

        // Fetch the sub-session by its handle
        CObject* object = iObjectIx->At( handle );
        if ( !object )
            {
            PanicClient(
                aMessage,
                EPhCltServerInitiatedPanicInvalidHandle );
            }

        // We can cast the object to a subsession instance, since that's
        // all we store in the object container
        CPhSrvSubSessionBase* subSession = 
            static_cast< CPhSrvSubSessionBase* >( object );
        
        if ( subSession )
            {
            // Check that the subsession can handle this request
            if ( !subSession->PhSrvMessageDecoderCanProcessMessage( function ) )
                {
                PanicClient(
                    aMessage,
                    EPhCltServerInitiatedPanicInvalidHandle );
                }
            else
                {
                // Get it to process this request
                subSession->PhSrvMessageProcessorHandleMessageL( aMessage );
                }
            }            
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::SubSessionCount
// 
// Return the number of subsessions
// -----------------------------------------------------------------------------
//
TInt CPhSrvSession::SubSessionCount() const
    {
    return iContainer->Count();
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::SubSessionA
// 
// Return a subsession from an index
// -----------------------------------------------------------------------------
//
CPhSrvSubSessionBase& CPhSrvSession::SubSessionAt( TInt aIndex ) const
    {
    return *static_cast< CPhSrvSubSessionBase* >( ( *iContainer )[ aIndex ] );
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::CloseSubsession
// 
// Remove object from object index
// -----------------------------------------------------------------------------
//
void CPhSrvSession::CloseSubSession( const RMessage2& aMessage )
    {
    TInt handle = aMessage.Int3();

    CObject* obj = iObjectIx->At( handle );  
    __ASSERT_ALWAYS( obj, 
        PanicClient(
            aMessage,
            EPhCltServerInitiatedPanicInvalidHandle ) );
    iObjectIx->Remove( handle );

    if ( !aMessage.IsNull() )
        {
        aMessage.Complete( KErrNone );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::PanicClient
// 
// Panic the client's thread
// -----------------------------------------------------------------------------
//
void CPhSrvSession::PanicClient(
    const RMessage2& aMessage,
    TPhCltServerInitiatedPanic aPanic ) const    
    {
    if ( !aMessage.IsNull() )
        {
        _LIT( KPhServerPanicCategory, "PhSrvServer" );
        aMessage.Panic( KPhServerPanicCategory, aPanic );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CPhSrvSession::ConstructL( CPhSrvServer& aServer )
    {
    iContainer = aServer.NewContainerL();
    iObjectIx = CObjectIx::NewL();   
    
    iServer = &aServer;
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::SubSessionUniqueHandle
// 
// Return the unique handle for the specified subsession
// -----------------------------------------------------------------------------
//
TInt CPhSrvSession::SubSessionUniqueHandle( 
    const CPhSrvSubSessionBase& aSubSession ) const
    {
    // Ensure that the handle really is unique.
    // return iObjectIx->At( &aSubSession ); is unique only in same session.
    return reinterpret_cast<TInt>( &aSubSession );
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::PhoneServer
// 
// Returns the Phone Server
// -----------------------------------------------------------------------------
//
CPhSrvServer& CPhSrvSession::PhoneServer() const
    {
    return *static_cast< CPhSrvServer* >( const_cast< CServer2* >( Server() ) );
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::Write
// 
// Write to the client address space. Panic client upon error
// -----------------------------------------------------------------------------
//
void CPhSrvSession::Write( 
    const RMessage2& aMessage,
    TInt aLocation,
    const TDesC8& aDes,
    TInt aOffset )
    {
    TInt ret = aMessage.Write( aLocation, aDes, aOffset );
    if ( ret != KErrNone )
        {
        PanicClient( 
            aMessage,
            EPhCltServerInitiatedPanicBadDescriptor );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::Read
// 
// Read from the client address space. Panic client upon error
// -----------------------------------------------------------------------------
//
void CPhSrvSession::Read(
    const RMessage2& aMessage,
    TInt aLocation,
    TDes8& aDes,
    TInt aOffset )
    {
    TInt ret = aMessage.Read( aLocation, aDes, aOffset );
    if ( ret != KErrNone )
        {
        PanicClient( 
            aMessage,
            EPhCltServerInitiatedPanicBadDescriptor );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::Read
// 
// Read from the client address space. Panic client upon error
// -----------------------------------------------------------------------------
//
void CPhSrvSession::Read(
    const RMessage2& aMessage,
    TInt aLocation,
    TDes& aDes,
    TInt aOffset )
    {
    TInt ret = aMessage.Read( aLocation, aDes, aOffset );
    if ( ret != KErrNone )
        {
        PanicClient( 
            aMessage,
            EPhCltServerInitiatedPanicBadDescriptor );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::CompleteCreateAll
// -----------------------------------------------------------------------------
//
void CPhSrvSession::CompleteCreateAll( TInt aError )
    {
    if ( iCreateAll )
        {
        iCreateAll = EFalse;
        iCreateAllMsg.Complete( aError );
        }
    }


// -----------------------------------------------------------------------------
// CPhSrvSession::HandleCommandL
// -----------------------------------------------------------------------------
//
TBool CPhSrvSession::HandleCommandL( const RMessage2& aMessage )
    {
    TBool result = EFalse;

    switch ( aMessage.Function() )
        {
        case EPhoneCreateAll:
            result = ETrue;
            
            iServer->CancelCreateAll( *this );
            iServer->CreateAllL( *this );
            
            iCreateAll = ETrue;
            iCreateAllMsg = aMessage;
            break;
        
        default:
            break;
        }

    return result;
    }


//  End of File
