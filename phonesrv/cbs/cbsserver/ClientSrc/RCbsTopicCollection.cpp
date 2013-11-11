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
* Description:  This module contains the implementation of RCbsTopicCollection class 
                 member functions.
*
*/


// INCLUDE FILES

#include "CbsServerConstants.h"
#include "RCbs.h"
#include "RCbsTopicCollection.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RCbsTopicCollection::RCbsTopicCollection
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RCbsTopicCollection::RCbsTopicCollection() 
    : iIterator( 0 )
    {   
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::Open
// Creates a subsession to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicCollection::Open( 
    RCbs& aServer  )
    {
    // Open the subsession.
    const TIpcArgs args( TIpcArgs::ENothing );
    return CreateSubSession( aServer, ECbsCreateTopicCollectionSubsession, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::Close
// Closes a subsession to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicCollection::Close()
    {
    // Close the subsession
    RSubSessionBase::CloseSubSession( ECbsCloseTopicCollectionSubsession );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::GetTopicCount
// Returns the total amount of topic information the topic collection
// contains.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicCollection::GetTopicCount( 
    TInt& aCount )
    {
    // Get the topic count from the server
    TPckgBuf< TInt > pckgCount;
    const TIpcArgs args( &pckgCount );
    SendReceive( ECbsGetTopicInfoCount, args );
    aCount = pckgCount();
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::GetTopicInfo
// Returns a topic information structure.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicCollection::GetTopicInfo( 
    TInt aIndex, 
    TCbsTopicInfo& aInfo )
    {
    // Get topic information from the server
    TPckgBuf< TCbsTopicInfo > pckgInfo( aInfo );
    const TIpcArgs args( aIndex, &pckgInfo );
    TInt result( SendReceive( ECbsGetTopicInfo, args ) );

    if ( result == KErrNone )
        {
        aInfo = pckgInfo();
        }
    return result;    
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::NotifyOnChange
// Notifies the client next time when topic collection changes.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicCollection::NotifyOnChange( 
    TRequestStatus& aStatus )
    {
    // Send notification on change
    const TIpcArgs args( TIpcArgs::ENothing );
    SendReceive( ECbsNotifyOnChange, args, aStatus );
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::NotifyOnChangeCancel
// Cancels the pending request to notify.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicCollection::NotifyOnChangeCancel()
    {
    // Cancel pending notification request.
    const TIpcArgs args( TIpcArgs::ENothing );
    SendReceive( ECbsNotifyOnChangeCancel, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::Start
// Interface for topic collection browsing. Resets the iterator.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsTopicCollection::Start()
    {
    iIterator = 0;
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::HasNextTopic
// Returns ETrue, if the topic collection has topic identities 
// unaccessed with NextTopic().
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool RCbsTopicCollection::HasNextTopic()
    {
    TInt count( 0 );
    TInt result( KErrNone );
    TBool inList( EFalse );

    GetTopicCount( count );
    for ( ; iIterator < count; iIterator++ )
        {
        // Check if the current topic is in the topic list.
        result = TopicInList( iIterator, inList );
        if ( result != KErrNone )
            {
            return EFalse;
            }
        else if ( !inList )  
            {
            return ETrue;   // this topic is not in the list; 
            }
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::NextTopic
// Returns the next topic identity if one exists.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicCollection::NextTopic( 
    TCbsTopicInfo& aInfo )
    {
    // iIterator points to the next unlisted topic: return the topic 
    // and error code.
    TInt result( KErrNone );
    result = GetTopicInfo( iIterator, aInfo );
    if ( result == KErrNone )
        {
        iIterator++;    // advance to the next topic
        }
    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicCollection::TopicInList
// Returns information whether the topic was in topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt RCbsTopicCollection::TopicInList( 
    TInt aIndex, 
    TBool& aInList )
    {
    // value returned by the server gets stored here
    TPckgBuf< TBool > pckgInList;
    const TIpcArgs args( aIndex, &pckgInList );

    TInt result = SendReceive( ECbsTopicInList, args );
    if ( result == KErrNone )
        {
        aInList = pckgInList();
        }
    
    return result;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
