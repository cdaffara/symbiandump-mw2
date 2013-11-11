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
* Description:  MCN server-side subsession implementation.
*
*                Implementation of MCN topic subscription commits changes (reception 
*                state, subscribed topics, etc) immediately after every request.
*
*/



// INCLUDE FILES

#include "CbsCommon.h"
#include "CbsServerPanic.h"
#include "CbsServerConstants.h"
#include "CCbsMcnSession.h"
#include "CCbsRecMessage.h"
#include "CCbsReceiverHelper.h"
#include "CCbsDbImpSettings.H"
#include "CCbsRecEtel.h"
#include "CCbsSession.h"
#include "CCbsServer.h"
#include "CbsLogger.h"

// CONSTANTS

/// Initial size of the array holding numbers of subscribed topics.
const TInt KInitialSpaceForSubscribedTopics = 1;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsMcnSession::CCbsMcnSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMcnSession::CCbsMcnSession( 
    CCbsSession& aSession, 
    CCbsRecEtel& aReceiver )
    : CCbsObject( aSession ), 
    iReceiver( aReceiver ),
    iMcnPckg( iMcnMessage )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::ConstructL()");

    iSubscribedTopics = new (ELeave) CArrayFixFlat<TUint16>
        ( KInitialSpaceForSubscribedTopics );
    iReceiver.AddSubscriptionProviderL( this );

	CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsMcnSession* CCbsMcnSession::NewL( 
    CCbsSession& aSession, 
    CCbsRecEtel& aReceiver )
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::NewL()");

    CCbsMcnSession* self = 
        new (ELeave) CCbsMcnSession( aSession, aReceiver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::NewL()");
    return self;
    }

// Destructor

CCbsMcnSession::~CCbsMcnSession()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::~CCbsMcnSession()");
    TRAP_IGNORE( iReceiver.RemoveSubscriptionProviderL( this ) );
    delete iSubscribedTopics;
    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::~CCbsMcnSession()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::HandleRequestsL
// Handles MCN client requests.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TBool CCbsMcnSession::HandleRequestsL( 
    const RMessage2& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::HandleRequestsL()");

    TBool requestHandled( ETrue );

    switch( aMessage.Function() )
        {
        case EMcnNotifyOnChange:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): EMcnNotifyOnChange");
            NotifyOnChange();
            break;

        case EMcnCloseSubsession:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): EMcnCloseSubsession");
            CloseMcnSession();
            aMessage.Complete( KErrNone );
            break;

        case EMcnNotifyOnChangeCancel:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): EMcnNotifyOnChangeCancel");
            NotifyOnChangeCancel();
            break;

        case EMcnSubscribeTopic:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): EMcnSubscribeTopic");
            SubscribeTopicL();
            break;

        case EMcnClearSubscriptions:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): EMcnClearSubscriptions");
            ClearSubscriptionsL();
            break;

        case EMcnNoMoreSubscriptions:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): EMcnNoMoreSubscriptions");
            ApplySubscriptionsL();
            break;

        default:
            CBSLOGSTRING("CBSSERVER: CCbsMcnSession::HandleRequestsL(): default");
            requestHandled = EFalse;
            break;
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::HandleRequestsL()");

    return requestHandled;
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::NumberOfSubscriptions
// Returns the number of topic subscriptions of this MCN client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TUint CCbsMcnSession::NumberOfSubscriptions() const
    {
    return iSubscribedTopics->Count();
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::RouteMessageL
// Routes messages to the subscribed Mcnclients.
// If the topic of the message is subscribed by the client,
// aMessage is copied to client-side. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::RouteMessageL( 
    const CCbsMessage& aMessage )
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::RouteMessageL()");

	TKeyArrayFix key(0, ECmpTUint16);
	TInt index;
    TUint16 topicNumber( aMessage.TopicNumber() );    
	TInt result( iSubscribedTopics->FindIsq( topicNumber, key, index ) );


    TBool isLC( aMessage.IsLivecastMessage() );

    if ( isLC )
        {
        iMcnMessage.iBuffer8 = aMessage.Contents8();
        iMcnMessage.iBuffer = KNullDesC;
        }
    else
        {
        iMcnMessage.iBuffer = aMessage.Contents();
        iMcnMessage.iBuffer8 = KNullDesC8;
        }
    
    iMcnMessage.iNetworkMode = aMessage.NetworkMode();
    iMcnMessage.iTopicNumber = aMessage.TopicNumber();

    CBSLOGSTRING2("CBSSERVER: CCbsMcnSession::RouteMessageL(): topicNumber: %d", iMcnMessage.iTopicNumber );
    CBSLOGSTRING2("CBSSERVER: CCbsMcnSession::RouteMessageL(): networkMode: %d (0=GSM, 1=WCDMA, 2=Not defined).", iMcnMessage.iNetworkMode );    
    
	if ( result == KErrNone )
		{        
        CBSLOGSTRING("CBSSERVER: CCbsMcnSession::RouteMessageL(): Notifying client...");
        NotifyClientL( iMcnPckg );
        CBSLOGSTRING("CBSSERVER: CCbsMcnSession::RouteMessageL(): Client notified.");
	    }
	CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::RouteMessageL()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::CloseMcnSession
// Close the subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::CloseMcnSession()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::CloseMcnSession()");

    // Removes the object.
    Session().Server().DeleteObjectByHandle( Message().Int3() );

    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::CloseMcnSession()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::NotifyOnChange
// Handles client request for message routing.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
void CCbsMcnSession::NotifyOnChange()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::NotifyOnChange()");

    if ( iIsMessage )
        {
        NotifyOnChangeCancel();
        }

    iMessage = Message();
    iIsMessage = ETrue;

    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::NotifyOnChange()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::NotifyOnChangeCancel
// Handles client request to cancel an outstanding routing request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::NotifyOnChangeCancel()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::NotifyOnChangeCancel()");

    if ( iIsMessage )
        {
        iMessage.Complete( KErrCancel );
        }

    iIsMessage = EFalse;
    Message().Complete( KErrNone );

    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::NotifyOnChangeCancel()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::SubscribeTopicL
// Handles client-side request to subscribe a single topic.
// An attempt to subscribe the same topic twice does not
// cause any error - the topic remains subscribed. 
// Note: ApplySubscriptionL has to be called in order to
// apply receiver changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::SubscribeTopicL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::SubscribeTopicL()");

    // Read topic number from client.
    TUint16 topicNumber( 0 );
    topicNumber = static_cast<TUint16> ( Message().Int0() );
   
    // Store the topic number and notify receiver.
	TKeyArrayFix key(0, ECmpTUint16);
	TRAPD( err, iSubscribedTopics->InsertIsqL( topicNumber, key ) );
	if( err != KErrAlreadyExists )
		{        
        CBSLOGSTRING2("CBSSERVER: CCbsMcnSession::SubscribeTopicL(), leaving if error != 0: %d", err);
		User::LeaveIfError( err );
		}

    // Complete the request.
    Message().Complete( KErrNone );

    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::SubscribeTopicL()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::NotifyClientL
// Called by RouteMessageL() to actually copy the message content
// to client side.
// If the client has a CB message routing request pending,
// the message and topicnumber of the message are given to client side.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::NotifyClientL( 
    const TPckg<TCbsMcnMessage>& aMcnPckg )
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::NotifyClientL()");

    if ( iIsMessage )
        {        
        CBSLOGSTRING("CBSSERVER: CCbsMcnSession::NotifyClientL(): Writing msg pckg to client...");
        // Write the message package to client
		iMessage.WriteL( 0, aMcnPckg );        
        CBSLOGSTRING("CBSSERVER: CCbsMcnSession::NotifyClientL(): Writing msg pckg to client OK.");

        // Complete the request
        iMessage.Complete( KErrNone );
        iIsMessage = EFalse;
        }
	CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::NotifyClientL()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::ClearSubscriptionsL
// Clears all topic subscriptions of this MCN session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::ClearSubscriptionsL() 
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::ClearSubscriptionsL()");

    iSubscribedTopics->Reset();
    iReceiver.ApplyStateChangesL();
    Message().Complete( KErrNone );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::ClearSubscriptionsL()");
    }

// -----------------------------------------------------------------------------
// CCbsMcnSession::ApplySubscriptionsL
// Forces receiver to reload CB topic subscriptions.
// This function has to be called after MCN client has subscribed
// topics with SubscribeTopicL().
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMcnSession::ApplySubscriptionsL() 
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsMcnSession::ApplySubscriptionsL()");

    iReceiver.ApplyStateChangesL();
    Message().Complete( KErrNone );

	CBSLOGSTRING("CBSSERVER: <<< CCbsMcnSession::ApplySubscriptionsL()");
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
