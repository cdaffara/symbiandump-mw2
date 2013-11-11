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
* Description:  This module contains the implementation of CCbsTopicCollection class 
*                member functions. 
* 
*
*/


// INCLUDE FILES

#include "CbsCommon.h"
#include "CCbsSession.h"
#include "CCbsTopicCollection.h"
#include "CCbsDbImpTopicCollection.h"
#include "CCbsDbImpTopicList.h"
#include "CbsServerConstants.h"
#include "CCbsServer.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsTopicCollection::CCbsTopicCollection
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsTopicCollection::CCbsTopicCollection( 
    CCbsSession& aSession, 
    CCbsDbImpTopicCollection& aCollection, 
    CCbsDbImpTopicList& aList )    
    : CCbsObject( aSession ), 
    iCollection( aCollection ), 
    iTopicList ( aList )
    {
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::ConstructL()
    {
    iCollection.AddObserverL( this );
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsTopicCollection* CCbsTopicCollection::NewL( 
    CCbsSession& aSession, 
    CCbsDbImpTopicCollection& aCollection,
    CCbsDbImpTopicList& aList )
    {
    // Normal two phase construction.
    CCbsTopicCollection* self = 
        new ( ELeave ) CCbsTopicCollection( aSession, aCollection, aList );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsTopicCollection::~CCbsTopicCollection()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsTopicCollection::~CCbsTopicCollection()"); 
    // Remove from the observers.
    iCollection.RemoveObserver( this );
    CBSLOGSTRING("CBSSERVER: <<< CCbsTopicCollection::~CCbsTopicCollection()");
    }
    
// -----------------------------------------------------------------------------
// CCbsTopicCollection::HandleRequestsL
// Passes the requests to proper functions.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
TBool CCbsTopicCollection::HandleRequestsL( 
    const RMessage2& aMessage )
    {    
    CBSLOGSTRING("CBSSERVER: >>> CCbsTopicCollection::HandleRequestsL()");
    
    TBool requestHandled( ETrue );

    // Handle the requests.
    switch ( aMessage.Function() )
        {
        case ECbsCloseTopicCollectionSubsession:
            CloseCollection();
            aMessage.Complete( KErrNone );
            break;

        case ECbsGetTopicInfoCount:                 
            GetTopicCountL();
            break;

        case ECbsGetTopicInfo:                      
            GetTopicInfoL();
            break;

        case ECbsTopicInList:                       
            TopicInListL();
            break;

        case ECbsNotifyOnChange:
            NotifyOnChange();
            break;
    
        case ECbsNotifyOnChangeCancel:
            NotifyOnChangeCancel();
            break;

        default:
            requestHandled = EFalse;
            break;
        }
              
    CBSLOGSTRING2("CBSSERVER: <<< CCbsTopicCollection::HandleRequestsL(), returning requestHandled: %d", requestHandled );    
    return requestHandled;
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::TopicCollectionContentsChangedInd
// Called whenever topic collection contents are changed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::TopicCollectionContentsChangedInd()
    {
    // Notify if necessary.
    Notify();
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::CloseCollection
// Closes the subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::CloseCollection()
    {
    // Removes the object.
    Session().Server().DeleteObjectByHandle( Message().Int3() );
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::GetTopicCountL
// Writes the number of topics in the topic collection to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::GetTopicCountL()
    {
    TInt count;
    iCollection.GetTopicIdentityCount( count );
    // Write the count to the client side.
    TPckgBuf< TInt > pckg( count );
    Message().WriteL( 0, pckg );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::GetTopicInfoL
// Writes the topic identity requested by the client to the client side. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::GetTopicInfoL()
    {
    // Get the index from the client
    TInt index( 0 );
    index = Message().Int0();

    TCbsTopicInfo info;
    iCollection.GetTopicIdentityL( index, info );

    // Write the count to the client side.
    TPckgBuf< TCbsTopicInfo > pckg( info );
    Message().WriteL( 1, pckg );

    // Complete the request.
    Message().Complete( KErrNone );
    
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::TopicInListL
// Writes a truth value to the client determining the existence 
// of a topic in the current topic list. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::TopicInListL()
    {
    // Get the topic index from the client
    TInt index( 0 );
    index = Message().Int0();
      
    // Check if this topic exists in the topic list.
    TCbsTopicInfo topicInfo;
    iCollection.GetTopicIdentityL( index, topicInfo );

    TBool inList( iTopicList.TopicIndexInList( topicInfo.iNumber ) >= 0 );

    // Write the result back to the client.
    TPckgBuf< TBool > pckg( inList );
    Message().WriteL( 1, pckg );

    // Complete the request.
    Message().Complete( KErrNone );

    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::NotifyOnChange
// Sets up a notification so that the client will be notified when the 
// collecton is changed. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::NotifyOnChange()
    {
    // Just create a pending request.
    if ( iIsMessage )
        {
        NotifyOnChangeCancel();
        }

    iIsMessage = ETrue;
    iMessage = Message();
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::NotifyOnChangeCancel
// Cancels the pending request. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::NotifyOnChangeCancel()
    {
    // Cancel the notification.
    if ( iIsMessage )
        {
        iMessage.Complete( KErrCancel );
        }

    iIsMessage = EFalse;
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicCollection::Notify
// Notifies the client. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicCollection::Notify()
    {
    // Notify, if necessary
    if ( iIsMessage ) 
        {
        iMessage.Complete( KErrNone );
        }    
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
