/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsSession class 
*                member functions.
*    
*                This class represents a server-side session.
*
*                The instances of the class are responsible of handling 
*                the requests of client-side RCbs and RCbsMcnSession objects.
*
*                This class contains handlers for opening subsessions and
*                retrieving some general CBS information.
*
*                Client will be panicked, if a request is invalid. 
*
*/



// INCLUDE FILES

#include "CbsCommon.h"
#include "CbsServerConstants.h"
#include "CbsServerPanic.h"
#include "CCbsServer.h"
#include "CCbsSession.h"
#include "CCbsObject.h"
#include "CCbsRecEtel.h"
#include "CCbsSettings.h"
#include "CCbsTopicList.h"
#include "CCbsTopicCollection.h"
#include "CCbsTopicMessages.h"
#include "CCbsMcnSession.h"

#include "CCbsDbImp.H"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsSession::CCbsSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsSession::CCbsSession( 
    CCbsServer& aServer )
    : CSession2(), 
    iCbsServer( aServer )
    {
    }

// -----------------------------------------------------------------------------
// CCbsSession::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsSession::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsSession::ConstructL()");
    CBSLOGSTRING("CBSSERVER: <<< CCbsSession::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsSession::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSession2* CCbsSession::NewL(     
    CCbsServer& aServer )
    {
    CCbsSession* self = new ( ELeave ) CCbsSession( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

    
// Destructor
// Session destructor is called when the client-side session
// object makes a Close()-call. There's no need to send
// a "close session" request to the server.
CCbsSession::~CCbsSession()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsSession::~CCbsSession()");    
    CBSLOGSTRING("CBSSERVER: <<< CCbsSession::~CCbsSession()"); 
    }

// -----------------------------------------------------------------------------
// CCbsSession::DispatchMessageL
// Finds the appropriate message service method and calls it to 
// handle the request. 
// 
// Relays a message to a proper message handling method. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSession::DispatchMessageL( 
    const RMessage2& aMessage )
    {
    if ( !HandleSessionRequestsL( aMessage ) ) 
        {
        // The request was not for the session. Submit requests to 
        // the correct subsessions.
        CCbsObject* object = iCbsServer.ReturnObjectByHandle( aMessage.Int3() );

        if ( object )
            {
            // Let the subsession to handle the request
            if ( !object->HandleRequestsL( aMessage ) )
                {
                aMessage.Complete( KErrNotSupported );
                }
            }
        else
            {
            PanicClient( ECbsBadRequest );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCbsSession::HandleSessionRequestsL
// Handles the session-based requests.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsSession::HandleSessionRequestsL( 
    const RMessage2& aMessage )
    {
    TBool result( ETrue );
    TCbsServerRequest request( ( TCbsServerRequest ) aMessage.Function() );    

    // Handle requests for the session.
    switch ( request ) 
        {
        case EMcnCloseSession:
            {
            // Close the session
            aMessage.Complete( KErrNone );
            }            
            break;

        case ECbsCloseSession:
            {
            // Close the session
            aMessage.Complete( KErrNone );                
            }
            break;
                
        case EMcnGetCellInfo:
        case EMcnGetInfoMessage:
            {
            GetCurrentMcnInfoL();
            }            
            break;

        case ECbsCreateSettingsSubsession:
            {
            NewSettingsL();                
            }            
            break;

        case ECbsCreateTopicListSubsession:            
            {
            NewTopicListL();
            }
            
            break;

        case ECbsCreateTopicMessagesSubsession:            
            {
            NewTopicMessagesL();
            }
            break;

        case ECbsCreateTopicCollectionSubsession:            
            {
            NewTopicCollectionL();
            }                    
            break;

        case EMcnCreateSubsession:
            {
            NewMcnSubsessionL();
            }            
            break;
           
        case ECbsShutdown:        
            {
            iCbsServer.Shutdown();
            aMessage.Complete( KErrNone );
            }                                
            break;        

        default:
            // The request was not for the session.
            result = EFalse;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsSession::PanicClient
// Panics the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSession::PanicClient( 
    TCbsSessionPanic aPanic ) const
    {    
    // Panic
    iCurrentMessage.Panic( KCbsServerName, aPanic );
    }

// -----------------------------------------------------------------------------
// CCbsSession::TotalTopicsDetected
// Returns the number of new topics detected. 
// The counter will be reset after a call.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsSession::TotalTopicsDetected()
    {
    return iCbsServer.TotalTopicsDetected();
    }

// -----------------------------------------------------------------------------
// CCbsSession::Message
// Returns the current message in process 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RMessage2& CCbsSession::Message()
    {
    return iCurrentMessage;
    }

// -----------------------------------------------------------------------------
// CCbsSession::ServiceL
// Handles the received message.
// The method simply calls DispatchMessageL to process a
// message and informs the client if the handling failed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSession::ServiceL( 
    const RMessage2& aMessage )
    {
    iCurrentMessage = aMessage;

    TRAPD( result, DispatchMessageL( aMessage ) );

    if ( result != KErrNone )
        {
        // Inform the client
        aMessage.Complete( result );
        }
    }

// -----------------------------------------------------------------------------
// CCbsSession::NewSettingsL
// Create a new subsession, settings, for the session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CCbsSession::NewSettingsL()
    {
    // Create a new settings object
    CCbsSettings* object = 
        CCbsSettings::NewL( *this, iCbsServer.Database().SettingsL(), 
            iCbsServer.Receiver() );
    
    // Add the object to the object container to generate unique id
    iCbsServer.Container().AddL( object );

    // Add object to object index; this returns a unique handle so 
    // we can get it again
    TInt handle( iCbsServer.ObjectIx().AddL( object ) );

    // Write the handle to client
    TPckg< TInt > handlePckg( handle );
    TRAPD( result, iCurrentMessage.WriteL( 3, handlePckg ) );

    if ( result != KErrNone )
        {
        // Writing failed, the client called the server in a wrong way
        iCbsServer.ObjectIx().Remove( handle );
        PanicClient( ECbsBadDescriptor );
        }
    else
        {
        Message().Complete( KErrNone );
        }        
    }

// -----------------------------------------------------------------------------
// CCbsSession::NewTopicListL
// Create a new subsession, topic list, for the session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CCbsSession::NewTopicListL()
    {
    // Create a new topic list object
    CCbsTopicList* object = 
        CCbsTopicList::NewL( *this, iCbsServer.Database().TopicListL(),
            iCbsServer.Receiver() );

    // Add the object to the object container to generate unique id
    iCbsServer.Container().AddL( object );

    // Add object to object index; this returns a unique handle 
    // so we can get it again
    TInt handle( iCbsServer.ObjectIx().AddL( object ) );

    // Write the handle to client
    TPckg< TInt > handlePckg2( handle );
    TRAPD( result, iCurrentMessage.WriteL( 3, handlePckg2 ) );

    if ( result != KErrNone )
        {
        // Writing failed, the client called the server in a wrong way
        iCbsServer.ObjectIx().Remove( handle );
        PanicClient( ECbsBadDescriptor );
        }
    else
        {
        Message().Complete( KErrNone );
        }    
    }

// -----------------------------------------------------------------------------
// CCbsSession::NewTopicCollectionL
// Create a new subsession, topic collection, for the session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsSession::NewTopicCollectionL()
    {
    // Create a new topic list object
    CCbsTopicCollection* object = 
        CCbsTopicCollection::NewL( *this, 
            iCbsServer.Database().TopicCollectionL(),
            iCbsServer.Database().TopicListL() );

    // Add the object to the object container to generate unique id
    iCbsServer.Container().AddL( object );

    // Add object to object index; this returns a unique handle 
    // so we can get it again
    TInt handle( iCbsServer.ObjectIx().AddL( object ) );    

    // Write the handle to client
    TPckg< TInt > handlePckg2( handle );
    TRAPD( result, iCurrentMessage.WriteL( 3, handlePckg2 ) );

    if ( result != KErrNone )
        {
        // Writing failed, the client called the server in a wrong way
        iCbsServer.ObjectIx().Remove( handle );
        PanicClient( ECbsBadDescriptor );
        }
    else
        {
        Message().Complete( KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CCbsSession::NewTopicMessagesL
// Create a new subsession, topic messages, for the session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsSession::NewTopicMessagesL()
    {
    // Create a new topic list object
    CCbsTopicMessages* object = 
        CCbsTopicMessages::NewL( *this, 
            iCbsServer.Database().TopicMessagesL(),
            iCbsServer.Receiver() );

    // Add the object to the object container to generate unique id
    iCbsServer.Container().AddL( object );

    // Add object to object index; this returns a unique handle 
    // so we can get it again
    TInt handle( iCbsServer.ObjectIx().AddL( object ) );

    // Write the handle to client
    TPckg< TInt > handlePckg2( handle );
    TRAPD( result, iCurrentMessage.WriteL( 3, handlePckg2 ) );

    if ( result != KErrNone )
        {
        // Writing failed, the client called the server in a wrong way
        iCbsServer.ObjectIx().Remove( handle );
        PanicClient( ECbsBadDescriptor );
        }
    else
        {
        Message().Complete( KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CCbsSession::NewMcnSubsessionL
// Create a new MCN subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsSession::NewMcnSubsessionL()
    {
    // Create a new settings object
    CCbsMcnSession* object = 
        CCbsMcnSession::NewL( *this, iCbsServer.Receiver() );
    
    // Add the object to the object container to generate unique id
    iCbsServer.Container().AddL( object );

    // Add object to object index; this returns a unique handle so 
    // we can get it again
    TInt handle( iCbsServer.ObjectIx().AddL( object ) );

    // Write the handle to client
    TPckg< TInt > handlePckg( handle );
    TRAPD( result, iCurrentMessage.WriteL( 3, handlePckg ) );

    if ( result != KErrNone )
        {
        // Writing failed, the client called the server in a wrong way
        iCbsServer.ObjectIx().Remove( handle );
        PanicClient( ECbsBadDescriptor );
        }   
    else
        {
        Message().Complete( KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CCbsSession::GetCurrentMcnInfoL
// Return the current cell info message to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsSession::GetCurrentMcnInfoL()
    {
    TBuf<KCbsMcnMessageMaxLength> buf;    
    
    // First, read topic number from the client

    TInt topicNumber( 0 );
    topicNumber = iCurrentMessage.Int1();

    TInt infoReceived( KErrNone );
    if ( topicNumber == KCellInfoTopic )
        {
        infoReceived = iCbsServer.Receiver().GetCurrentMessage( buf, KCellInfoTopic );
        }
    else if ( topicNumber == KHomeZoneTopic )
        {
        infoReceived = iCbsServer.Receiver().GetCurrentMessage( buf, KHomeZoneTopic );
        }
    else
        {
        infoReceived = KErrNotSupported;
        }

    TRAPD( result, iCurrentMessage.WriteL( 0, buf ) );
    
    if ( result != KErrNone )
        {
        PanicClient( ECbsBadDescriptor );
        }
    else
        {
        Message().Complete( infoReceived );
        }    
    }

// -----------------------------------------------------------------------------
// CCbsSession::Server
// Return the server instance
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CCbsServer& CCbsSession::Server()
    {
    return iCbsServer;
    }

//  End of File  
