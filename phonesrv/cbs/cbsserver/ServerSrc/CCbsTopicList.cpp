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
* Description:  This module contains the implementation of CCbsTopicList class 
*                member functions. 
* 
*
*/



// INCLUDE FILES

#include "CbsCommon.h"
#include "CbsServerConstants.h"
#include "CCbsSession.h"
#include "CCbsTopicList.h"

#include "CCbsDbImpTopicList.h"
#include "CCbsRecEtel.h"
#include "CCbsServer.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsTopicList::CCbsTopicList
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsTopicList::CCbsTopicList( 
    CCbsSession& aSession, 
    CCbsDbImpTopicList& aTopicList, 
    CCbsRecEtel& aReceiver )
    : CCbsObject( aSession ), 
    iTopicList( aTopicList ), 
    iReceiver( aReceiver ), 
    iIsMessage( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsTopicList::ConstructL()
    {
    iTopicList.AddObserverL( this );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsTopicList* CCbsTopicList::NewL( 
    CCbsSession& aSession, 
    CCbsDbImpTopicList& aTopicList, 
    CCbsRecEtel& aReceiver )
    {
    CCbsTopicList* self = 
            new ( ELeave ) CCbsTopicList( aSession, aTopicList, aReceiver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
   
// Destructor
CCbsTopicList::~CCbsTopicList()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsTopicList::~CCbsTopicList()");
    iTopicList.RemoveObserver( this );
    CBSLOGSTRING("CBSSERVER: <<< CCbsTopicList::~CCbsTopicList()");
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::HandleRequestsL
// Handle the requests for the object. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TBool CCbsTopicList::HandleRequestsL( 
    const RMessage2& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsTopicList::HandleRequestsL()");
    
    TBool requestHandled( ETrue );
    
    // Handle all requests for the subsession
    switch ( aMessage.Function() )
        {
        case ECbsCloseTopicListSubsession:
            CloseTopicList();
            aMessage.Complete( KErrNone );
            break;
        case ECbsGetTopicCount:
            GetTopicCountL();
            break;
    
        case ECbsGetTopic:
            GetTopicL();
            break;
        
        case ECbsFindTopicByNumber:
            FindTopicByNumberL();
            break;

        case ECbsGetNextAndPrevTopicNumber:
            GetNextAndPrevTopicNumberL();
            break;

        case ECbsDeleteTopic:
            DeleteTopicL();
            break;

        case ECbsChangeTopicNameAndNumber:
            ChangeTopicNameAndNumberL();
            break;

        case ECbsChangeTopicSubscriptionStatus:
            ChangeTopicSubscriptionStatusL();
            break;

        case ECbsChangeTopicHotmarkStatus:
            ChangeTopicHotmarkStatusL();
            break;

        case ECbsAddTopic:
            AddTopicL();
            break;

        case ECbsNotifyOnEvent:
            NotifyOnEvent();
            break;

        case ECbsNotifyOnEventCancel:
            NotifyOnEventCancel();
            break;

        case ECbsGetNewTopicsCount:
            GetNewTopicsCountL();
            break;

        case ECbsGetLatestTopicNumber:
            GetLatestTopicNumberL();
            break;

        case ECbsGetUnreadMessageCount:
            GetUnreadMessageCountL();
            break;

        case ECbsGetHotmarkedMessageHandle:
            GetHotmarkedMessageHandleL();
            break;

        case ECbsDeleteAll:        
            DeleteAllL();
            break;

        case ECbsGetUnreadHotmarkedMessageCount:
            GetUnreadHotmarkedMessageCountL();
            break;
        
        default:
            requestHandled = EFalse;
            break;
        }    

    // Return truth value indicating whether it was possible 
    // to handle the request or not.    
    
    CBSLOGSTRING2("CBSSERVER: <<< CCbsTopicList::HandleRequestsL(), returning requestHandled: %d", requestHandled );
    
    return requestHandled;
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::TopicListInitializedIndL
// Called when the topic list is initialized.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::TopicListInitializedIndL()
    {
    // Notification.
    NotifyClientL( ECbsTopicListInitialized, 0 );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::TopicAddedIndL
// Called whenever a topic is created. 
// If necessary, the client will be notified.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::TopicAddedIndL( 
    const TCbsDbTopicNumber& aNumber )
    {
    // Notification.
    NotifyClientL( ECbsTopicAdded, aNumber );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::TopicDeletedIndL
// Called whenever a topic is deleted.  
// If necessary, the client will be notified.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::TopicDeletedIndL( 
    const TCbsDbTopicNumber& aNumber )
    {
    // Notification.
    NotifyClientL( ECbsTopicDeleted, aNumber );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::TopicModifiedIndL
// Called whenever a topic is modified.  
// If necessary, the client will be notified.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::TopicModifiedIndL( 
    const TCbsDbTopicNumber& aNumber )
    {
    // Notification.
    NotifyClientL( ECbsTopicModified, aNumber );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::TopicNewMessageReceivedIndL
// Called whenever a new message has been received.   
// If necessary, the client will be notified.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CCbsTopicList::TopicNewMessageReceivedIndL( 
    const TCbsDbMessageHandle& aHandle )
    {
    TCbsTopicNumber topic( iTopicList.ExtractTopicNumber( aHandle ) );
    NotifyClientL( ECbsTopicReceivedNewMessage, topic );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::CloseTopicList
// Closes and destroys the subsession. 
// Note that after the call has completed, the object is no longer
// valid. It is also left for the callee's task to complete 
// the request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::CloseTopicList()
    {
    // Removes the object.
    Session().Server().DeleteObjectByHandle( Message().Int3() );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetTopicCountL
// Returns the total number of topics to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::GetTopicCountL()
    {
    // Write the topic count to the client side.
    TInt count( 0 );
    iTopicList.GetTopicCount( count );
    TPckgBuf< TInt > pckg( count );

    Message().WriteL( 0, pckg );

    // Complete the request
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetTopicL
// Returns the requested topic to the client.
// Retrieves topic information with an index to topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::GetTopicL()
    {
    // First, read the index from the client side
    TInt topicIndex( 0 );
    topicIndex = Message().Int0();

    // Then read the topic from the topic list
    TPckgBuf< TCbsTopic > pckgTopic;
    TRAPD( errorCode, iTopicList.GetTopicL( topicIndex, pckgTopic() ) );

    if ( errorCode == KErrNone )
        {
        // Finally, write the topic information to the client side
        Message().WriteL( 1, pckgTopic );
        }

    // Complete the request.
    Message().Complete( errorCode );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::FindTopicByNumberL
// Finds a topic by number and returns it to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::FindTopicByNumberL()
    {
    // First, read the number from the client side
    TInt topicNumber( 0 );
    topicNumber = Message().Int0();    
    
    // Find topic by handle
    TPckgBuf< TCbsTopic > pckgTopic;
    iTopicList.FindTopicByNumberL( static_cast<TUint16> ( topicNumber ), 
        pckgTopic() );

    // Finally, write the topic information to the client side
    Message().WriteL( 1, pckgTopic );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::DeleteTopicL
// Deletes a topic by its number and returns it to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::DeleteTopicL()
    {
    // First, read the handle from the client side    
    TInt topicNumber( 0 );
    topicNumber = Message().Int0();

    // Delete the topic
    iTopicList.DeleteTopicL( static_cast<TUint16> ( topicNumber ) );

    iReceiver.DeleteSimTopicL( static_cast<TUint16> ( topicNumber ), EFalse );

    iReceiver.ApplyStateChangesL();

    // Complete the request
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::ChangeTopicNameAndNumberL
// Changes a topic name and number.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::ChangeTopicNameAndNumberL()
    {
    TCbsDbTopicName name;
    TInt oldNumber( 0 );
    TInt newNumber( 0 );

    oldNumber = Message().Int0();
    newNumber = Message().Int1();
    Message().ReadL( 2, name );

    // First find the topic.
    TCbsDbTopic topic;
    iTopicList.FindTopicByNumberL( static_cast<TUint16> ( oldNumber ), topic );

    // Then do the operations.
    iTopicList.UpdateTopicNameAndNumberL( static_cast<TUint16> ( oldNumber ), 
        static_cast<TUint16> ( newNumber ), name );

    // Now we may need to change the subscriptions
    if ( oldNumber != newNumber )
        {
        iReceiver.DeleteSimTopicL( static_cast<TUint16> ( oldNumber ), EFalse );
        }

    // Complete the request.
    Message().Complete( KErrNone );
    }
        
// -----------------------------------------------------------------------------
// CCbsTopicList::ChangeTopicSubscriptionStatusL
// Changes topic subscription status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::ChangeTopicSubscriptionStatusL()
    {
    TInt topicNumber( 0 );
    TPckgBuf< TBool > pckgStatus;
    
    topicNumber = Message().Int0();
    Message().ReadL( 1, pckgStatus );

    // First find the topic.
    TCbsDbTopic topic;
    iTopicList.FindTopicByNumberL( static_cast<TUint16> ( topicNumber ), topic );

    // Then update.
    iTopicList.UpdateTopicSubscriptionStatusL( static_cast<TUint16> ( topicNumber ), pckgStatus() );
    iReceiver.ApplyStateChangesL();

    // Complete the request.
    Message().Complete( KErrNone );
    }
     
// -----------------------------------------------------------------------------
// CCbsTopicList::ChangeTopicHotmarkStatusL
// Changes topic hotmark status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::ChangeTopicHotmarkStatusL()
    {
    TPckgBuf< TBool > pckgStatus;
    TInt topicNumber( 0 );
    topicNumber = Message().Int0();
    Message().ReadL( 1, pckgStatus );

    // Then update
    iTopicList.UpdateTopicHotmarkStatusL( static_cast<TUint16> ( topicNumber ), pckgStatus() );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::AddTopicL
// Adds a topic to the list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::AddTopicL()
    {
    // First, get the topic information from the client side
    TPckgBuf< TCbsTopic > pckgTopic;
    Message().ReadL( 0, pckgTopic );

    // It is not allowed to set protected.
    if ( pckgTopic().iProtected )
        {
        User::Leave( KErrArgument );
        }

    // Add the topic, not detected automatically
    iTopicList.AddTopicL( pckgTopic(), EFalse );

    // Load the default topic stream
    iTopicList.LoadDefaultTopicStreamL();
    
    // Write updated data to the client side (only handle is updated)
    Message().WriteL( 0, pckgTopic );
    
    // Complete the request
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::NotifyOnEvent
// Sets up a pending notification request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::NotifyOnEvent()
    {
    if ( iIsMessage )
        {
        NotifyOnEventCancel();
        }

    iMessage = Message();
    iIsMessage = ETrue;
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::NotifyOnEventCancel
// Cancels a pending notification request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::NotifyOnEventCancel()
    {
    // Cancel the pending notify request.
    if ( iIsMessage )
        {
        iMessage.Complete( KErrCancel );
        iIsMessage = EFalse;
        }

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetNewTopicsCountL
// Returns the number of "new" topics to the client.
// By new it is referred to topics that are created because of 
// topic detection since last power on.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::GetNewTopicsCountL()
    {
    TPckgBuf< TInt > pckg( Session().TotalTopicsDetected() );
    Message().WriteL( 0, pckg );

    // Complete the request
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetLatestTopicNumberL
// Retrieves the number of the topic that was added
// last to the topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::GetLatestTopicNumberL()
    {
    TCbsTopicNumber number( 0 );
    TInt result( iTopicList.GetLatestTopicNumber( number ) );
    if ( result == KErrNone )
        {
        TPckgBuf<TCbsTopicNumber> pckg( number );
        Message().WriteL( 0, pckg );
        Message().Complete( KErrNone );
        }
    else
        {
        User::Leave( result );
        }
    }
   
// -----------------------------------------------------------------------------
// CCbsTopicList::GetUnreadMessageCountL
// Returns the total amount of unread messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsTopicList::GetUnreadMessageCountL()
    {
    // Get unread message count from the database.
    TPckgBuf<TInt> pckg( 0 );
    iTopicList.GetUnreadMessageCount( pckg() );

    Message().WriteL( 0, pckg );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetHotmarkedMessageHandleL
// Returns a handle to the latest (that is not yet taken) 
// hotmarked message handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsTopicList::GetHotmarkedMessageHandleL()
    {
    // Get the hotmarked message handle.
    TCbsMessageHandle handle( 0 );
    iTopicList.GetHotmarkedMessageHandleL( handle );

    // And then write it to the client.
    TPckgBuf< TCbsMessageHandle > pckg( handle );
    Message().WriteL( 0, pckg );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetUnreadHotmarkedMessageCountL
// Returns the number of hotmarked, unread messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::GetUnreadHotmarkedMessageCountL()
    {
    TInt number( iTopicList.UnreadHotmarkedMessageCount() );
    TPckgBuf<TInt> pckgNumber( number );
    Message().WriteL( 0, pckgNumber );
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::DeleteAllL
// Deletes all topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::DeleteAllL()
    {
    // Delete all topic from the SIM card
    iReceiver.DeleteAllSimTopicsL();
    
    // Delete all topic from the server store
    iTopicList.InitializeListL( EFalse );
    iReceiver.ApplyStateChangesL();

    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::NotifyClientL
// If there is a pending notify request, notifies 
// the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::NotifyClientL( 
    TCbsTopicListEvent aEvent, 
    TCbsTopicNumber aNumber )
    {
    // If there is a pending request, then process it.
    if ( iIsMessage )
        {
        // First check if the client is interested of the topics.
        TInt requested( iMessage.Int0() );
        if ( ( requested & aEvent ) )
            {
            // The client is interested of the event, so notify the client.
            // Make up a pointer descriptors.
            TPtr8 eventPtr( reinterpret_cast<TText8*>(&aEvent),
                    sizeof( TCbsTopicListEvent ),
                    sizeof( TCbsTopicListEvent) );

            TPtr8 numberPtr( reinterpret_cast<TText8*>(&aNumber),
                    sizeof( TCbsTopicNumber ),
                    sizeof( TCbsTopicNumber) );
            // Write through the pointer descriptors.
            iMessage.WriteL( 1, eventPtr );
            iMessage.WriteL( 2, numberPtr );

            iMessage.Complete( KErrNone );
            iIsMessage = EFalse;
            }        
        }        
    }

// -----------------------------------------------------------------------------
// CCbsTopicList::GetNextAndPrevTopicNumberL
// Retrieves and returns the numbers of topics that
// precede and succeed the given topic in topic list.
// Also returns two flags indicating whether the
// given topic is the first and/or the last topic in list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicList::GetNextAndPrevTopicNumberL()
    {
    // Retrieve the requested topic number
    TPckgBuf<TCbsTopicNumber> pckgCurrentTopic( 0 );
    Message().ReadL( 0, pckgCurrentTopic );

    // Determine requested information
    TCbsTopicNumber nextTopic( 0 );
    TCbsTopicNumber prevTopic( 0 );
    TInt position( 0 );
    iTopicList.GetNextAndPrevTopicNumberL(
        pckgCurrentTopic(), nextTopic, prevTopic, position );

    // Write results back
    TPckgBuf<TCbsNextPrevTopicAndPosition> pckgResults;
    pckgResults().iNextTopic = nextTopic;
    pckgResults().iPrevTopic = prevTopic;
    pckgResults().iPosition = position;
    Message().WriteL( 1, pckgResults );

    // Complete request
    Message().Complete( KErrNone );
    }


// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
