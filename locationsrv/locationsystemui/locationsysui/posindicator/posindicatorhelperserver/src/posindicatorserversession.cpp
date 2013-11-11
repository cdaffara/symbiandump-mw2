/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of server session class.
*
*/

#include "posindicatorserversession.h"
#include "posindicatorservercore.h"
#include "posindicatorserversubsession.h"
#include "posindicatorserverconst.h"
#include "posindicatorsubsessionregistry.h"
#include "posindicatorlogger.h"

//---------------------------------------------------------------
// CPosIndicatorServerSession::NewL
//---------------------------------------------------------------
CPosIndicatorServerSession* CPosIndicatorServerSession::NewL(
                            const CPosIndicatorServerCore& aServerCore )
    {
    FUNC("CPosIndicatorServerSession::NewL");
    CPosIndicatorServerSession* self = new( ELeave )
                                  CPosIndicatorServerSession( aServerCore );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

//---------------------------------------------------------------
// CPosIndicatorServerSession::CPosIndicatorServerSession
//---------------------------------------------------------------
CPosIndicatorServerSession::CPosIndicatorServerSession(
                            const CPosIndicatorServerCore& aServerCore ):
                            iServerCore( aServerCore )
    {
    FUNC("CPosIndicatorServerSession::CPosIndicatorServerSession");
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSession::~CPosIndicatorServerSession
// Destructor
// ---------------------------------------------------------------------------
//
CPosIndicatorServerSession::~CPosIndicatorServerSession()
    {
    FUNC("CPosIndicatorServerSession::~CPosIndicatorServerSession");
    delete iSubSessionRegistry;
    iServerCore.HandleSessionClosure();
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSession::ConstructL()
// 2nd phase constructor for instantiating member variables
// ---------------------------------------------------------------------------
void CPosIndicatorServerSession::ConstructL()
    {
    FUNC("CPosIndicatorServerSession::ConstructL");
    iSubSessionRegistry = CPosIndicatorSubSessionRegistry::NewL();
    }

//---------------------------------------------------------------
// CPosIndicatorServerSession::SecureId
//---------------------------------------------------------------
void CPosIndicatorServerSession::ServiceL( const RMessage2 &aMessage )
    {
    FUNC("CPosIndicatorServerSession::ServiceL");
    switch( aMessage.Function() )
        {
        case ESubSessionOpen:
            {
            FUNC("ESubSessionOpen");
            CreateSubSessionL( aMessage );                
            break;
            }
        case ESubSessionClose:
            {
            FUNC("ESubSessionClose");
            // Close the sub-session and free the sub-session object
            TInt retVal = iSubSessionRegistry->CloseSubSession( aMessage.Int3() );
            aMessage.Complete( retVal );
            break;
            }
        default: // Forward other messages to the sub-session
            {
            ForwardToSubSessionL( aMessage );
            break;
            }
        }
    }

//---------------------------------------------------------------
// CPosIndicatorServerSession::SecureId
//---------------------------------------------------------------
void CPosIndicatorServerSession::ServiceError( const RMessage2& aMessage,
                                               TInt aError )
    {
    FUNC("CPosIndicatorServerSession::ServiceError");
    aMessage.Complete( aError );
    }

//---------------------------------------------------------------
// CPosIndicatorServerSession::Disconnect
//---------------------------------------------------------------
void CPosIndicatorServerSession::Disconnect( const RMessage2& aMessage )
    {
    FUNC("CPosIndicatorServerSession::Disconnect");
    CSession2::Disconnect( aMessage );
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSession::CreateSubSessionL
// ---------------------------------------------------------------------------
//
void CPosIndicatorServerSession::CreateSubSessionL( const RMessage2& aMessage )
    {
    FUNC("CPosIndicatorServerSession::CreateSubSessionL");
    CPosIndicatorServerSubsession* subSession = 
                        CPosIndicatorServerSubsession::NewL(
                                    *iServerCore.GetLocationRequestorHandle());
    CleanupStack::PushL( subSession );
    // If the NewL cannot allocate then it leaves with KErrMemory. In such a
    // scenario the ServiceError is invoked which responds to the IPC message
    // with the error code. Hence no special handling is required for
    // KErrNoMemory here.

    TInt subSessionHandle = iSubSessionRegistry->AddSubSessionL( subSession );
    CleanupStack::Pop(subSession); // subSession ownership taken by registry
   
    // Set the client subsession identifier.    
    TPckg<TInt> handlePackage( subSessionHandle );    
    TInt retVal = aMessage.Write( KParamSubsessionHandle,handlePackage );

    // The error code from Write has to be handled as below,
    // If the retVal is an error code then 
    //      close the open subsession first.
    //      If error is KErrBadDescriptor then panic the client
    //      else complete the message with the error code.
    // else complete the message with KErrNone.
    if ( retVal != KErrNone )
        {
        // close the open subsession first.
        iSubSessionRegistry->CloseSubSession( subSessionHandle );     
        }
    aMessage.Complete( retVal );
    }

// ---------------------------------------------------------------------------
// CPosIndicatorServerSession::ForwardToSubSessionL
// ---------------------------------------------------------------------------
void CPosIndicatorServerSession::ForwardToSubSessionL(const RMessage2& aMessage)
    {
    FUNC("CPosIndicatorServerSession::ForwardToSubSessionL");
    CPosIndicatorServerSubsession* subSession = 
            iSubSessionRegistry->SubSessionFromHandle(aMessage.Int3());
            
    if (!subSession)
        {
        // Complete client's request if the sub-session handle is not 
        // present in the registry
        aMessage.Complete( KErrNotFound );              
        }
    else
        {
        subSession->ServiceL(aMessage);
        }
    }
// End of file.
