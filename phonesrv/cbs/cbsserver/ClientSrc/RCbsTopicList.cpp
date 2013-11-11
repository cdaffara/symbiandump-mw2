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
* Description:  This module contains the implementation of RCbsTopicList class 
                 member functions.
*
*/



// INCLUDE FILES

#include "CbsServerConstants.h"
#include "RCbs.h"
#include "RCbsTopicList.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RCbsTopicList::RCbsTopicList
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RCbsTopicList::RCbsTopicList()
    : iNotifyEventPtr( NULL, 0, 0 ), 
    iNotifyHandlePtr( NULL, 0, 0 )
    {
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::Open
// Creates a subsession to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::Open( 
    RCbs& aServer )
    {
    const TIpcArgs args( TIpcArgs::ENothing );
    return CreateSubSession( aServer, ECbsCreateTopicListSubsession, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::Close
// Closes the subsession and connection to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsTopicList::Close()
    {
    // Close the subsession.
    RSubSessionBase::CloseSubSession( ECbsCloseTopicListSubsession );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetTopicCount
// Returns the total amount of topics the topic list contains.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicList::GetTopicCount( 
    TInt& aCount )
    {
    // Send request to the server. Get the total amount of topics.
    aCount = 0;
    TPckgBuf< TInt > pckg( aCount );
    const TIpcArgs args( &pckg );

    TInt result = SendReceive( ECbsGetTopicCount, args );
    // In case everything went just fine update the parameter!
    if ( result == KErrNone )
        {
        aCount = pckg();
        }
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetTopic
// Returns information about a topic from the topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::GetTopic( 
    const TInt aIndex, 
    TCbsTopic& aTopic )
    {
    TPckgBuf< TCbsTopic > pckgTopic;
    const TIpcArgs args( aIndex, &pckgTopic );

    TInt result = SendReceive( ECbsGetTopic, args );

    // Update if everything went just fine
    if ( result == KErrNone )
        {
        aTopic = pckgTopic();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::FindTopicByNumber
// Finds the topic by given number.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsTopicList::FindTopicByNumber( 
    TCbsTopicNumber aNumber, 
    TCbsTopic& aTopic )
    {
    TPckgBuf< TCbsTopic > pckgTopic;
    const TIpcArgs args( aNumber, &pckgTopic );

    TInt result = SendReceive( ECbsFindTopicByNumber, args );

    // Update aTopic if everything went just fine.
    if ( result == KErrNone )
        {
        aTopic = pckgTopic();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::DeleteTopic
// Deletes an existing topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsTopicList::DeleteTopic( 
    TCbsTopicNumber aNumber )
    {
    // Send request to the server to delete a topic by handle    
    const TIpcArgs args( aNumber );
    return SendReceive( ECbsDeleteTopic, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::DeleteAllTopics
// Delete all topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsTopicList::DeleteAllTopics()
    {
    const TIpcArgs args( TIpcArgs::ENothing );
    return SendReceive( ECbsDeleteAll, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::AddTopic
// Adds a new topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::AddTopic( 
    TCbsTopic& aTopic )
    {
    // Send request to add a topic to the list.
    TPckgBuf< TCbsTopic > pckgTopic( aTopic );
    const TIpcArgs args( &pckgTopic );

    TInt result = SendReceive( ECbsAddTopic, args );

    if ( result == KErrNone )
        {
        // Update the topic information (only handle).
        aTopic = pckgTopic();    
        }

    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::ChangeTopicNameAndNumber
// Changes the name and number of the existing topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::ChangeTopicNameAndNumber( 
    TCbsTopicNumber aOldNumber, 
    TCbsTopicNumber aNewNumber, 
    const TCbsTopicName& aName )
    {
    // Send request to the server
    const TIpcArgs args( aOldNumber, aNewNumber, &aName );
    return SendReceive( ECbsChangeTopicNameAndNumber, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::ChangeTopicSubscriptionStatus
// Changes topic subscription status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::ChangeTopicSubscriptionStatus( 
     TCbsTopicNumber aNumber, 
     TBool aNewStatus )
    {
    // Send request to the server
    TPckgBuf< TBool > pckgStatus( aNewStatus );
    const TIpcArgs args( aNumber, &pckgStatus );
    return SendReceive( ECbsChangeTopicSubscriptionStatus, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::ChangeTopicHotmarkStatus
// Changes topic hotmark status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::ChangeTopicHotmarkStatus( 
     TCbsTopicNumber aNumber, 
     TBool aNewStatus )
    {
    // Send request to the server
    TPckgBuf< TBool > pckgStatus( aNewStatus );
    const TIpcArgs args( aNumber, &pckgStatus );
    return SendReceive( ECbsChangeTopicHotmarkStatus, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::NotifyOnEvent
// Requests the server to notify the client whenever an event occurs that 
// changes the information of the topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicList::NotifyOnEvent( 
    TRequestStatus& aStatus, 
    const TInt aRequested, 
    TCbsTopicListEvent& aEvent, 
    TCbsTopicNumber& aHandle )
    {
    // Send request for the notify
    iNotifyEventPtr.Set( reinterpret_cast< TText8* >( &aEvent ), 
                         sizeof( TCbsTopicListEvent ),
                         sizeof( TCbsTopicListEvent ) );
    iNotifyHandlePtr.Set( reinterpret_cast< TText8* >( &aHandle ), 
                         sizeof( TCbsTopicNumber ),
                         sizeof( TCbsTopicNumber ) );

    const TIpcArgs args( aRequested, &iNotifyEventPtr, &iNotifyHandlePtr );
    SendReceive( ECbsNotifyOnEvent, args, aStatus );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::NotifyOnEventCancel
// Cancels the pending notify request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicList::NotifyOnEventCancel()
    {
    const TIpcArgs args( TIpcArgs::ENothing );
    SendReceive( ECbsNotifyOnEventCancel, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetNewTopicsCount
// Returns the number of session-specific new topics and resets the counter.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::GetNewTopicsCount( 
    TInt& aCount )
    {
    // Send request to the server to get the total amount of new topics
    aCount = 0;
    TPckgBuf<TInt> pckg( aCount );
    const TIpcArgs args( &pckg );

    TInt result( SendReceive( ECbsGetNewTopicsCount, args ) );

    if ( result == KErrNone )
        {
        aCount = pckg();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetLatestTopicNumber
// Returns the latest topic's number.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicList::GetLatestTopicNumber( 
    TCbsTopicNumber& aNumber )
    {
    aNumber = 0;
    TPckgBuf<TCbsTopicNumber> pckg( aNumber );
    const TIpcArgs args( &pckg );

    TInt result( SendReceive( ECbsGetLatestTopicNumber, args ) );

    if ( result == KErrNone )
        {
        aNumber = pckg();
        }
    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetUnreadMessageCount
// Returns the total amount of unread messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicList::GetUnreadMessageCount( 
    TInt& aCount )
    {
    // Send request to the server to get the total amount of unread messages
    TPckgBuf<TInt> pckg( aCount );
    const TIpcArgs args( &pckg );

    TInt result = SendReceive( ECbsGetUnreadMessageCount, args );

    // In case everything went just fine update the parameter
    if ( result == KErrNone )
        {
        aCount = pckg();
        }
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetHotmarkedMessageHandle
// Returns the handle to the latest hotmarked message that has been
// received after the system has started up.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void RCbsTopicList::GetHotmarkedMessageHandle( 
    TCbsMessageHandle& aMessage )
    {
    // Send request to the server to get the handle to the hotmarked message
    TPckgBuf<TCbsMessageHandle> pckgMessageHandle( aMessage );
    const TIpcArgs args( &pckgMessageHandle );

    TInt result = SendReceive( ECbsGetHotmarkedMessageHandle, args );

    if ( result == KErrNone )
        {
        aMessage = pckgMessageHandle();
        }
    else
        {
        aMessage = 0;
        }
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::NumberOfUnreadHotmarkedMessages
// Returns the number of unread messages in hotmarked topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicList::NumberOfUnreadHotmarkedMessages() 
    {
    TInt hotMsgCount( 0 );
    TPckgBuf<TInt> pckgHotMsgCount;    
    const TIpcArgs args( &pckgHotMsgCount );

    if ( SendReceive( ECbsGetUnreadHotmarkedMessageCount, args ) == KErrNone )
        {
        hotMsgCount = pckgHotMsgCount();
        }

    return hotMsgCount;
    }

// -----------------------------------------------------------------------------
// RCbsTopicList::GetNextAndPrevTopicNumber
// Returns the numbers of topics that precede and succeed the given 
// topic in server-side topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicList::GetNextAndPrevTopicNumber( 
	const TCbsTopicNumber& aCurrentTopicNumber,
	TCbsTopicNumber& aPrevTopicNumber,
	TCbsTopicNumber& aNextTopicNumber,
	TInt& aPosition )
    {
    TPckgBuf<TCbsTopicNumber> pckgCurrentTopic( aCurrentTopicNumber );
    TPckgBuf<TCbsNextPrevTopicAndPosition> pckgResults;

    const TIpcArgs args( &pckgCurrentTopic, &pckgResults );

    TInt result( SendReceive( ECbsGetNextAndPrevTopicNumber, args ) );
    if ( result == KErrNone )
        {
        aPrevTopicNumber = pckgResults().iPrevTopic;
        aNextTopicNumber = pckgResults().iNextTopic;
        aPosition = pckgResults().iPosition;
        }
    return result;
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
