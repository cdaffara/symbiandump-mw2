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
* Description:  This module contains the implementation of RCbsTopicMessages class 
                 member functions.
*
*/



// INCLUDE FILES

#include "CbsServerConstants.h"
#include "RCbs.h"
#include "RCbsTopicMessages.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RCbsTopicMessages::RCbsTopicMessages
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
RCbsTopicMessages::RCbsTopicMessages()
    {
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::Open
// Creates a subsession to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsTopicMessages::Open( 
    RCbs& aServer )
    {
    const TIpcArgs args( TIpcArgs::ENothing );
    return CreateSubSession( aServer, ECbsCreateTopicMessagesSubsession, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::Close
// Closes the subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::Close()
    {
    // Release locked message, if any
    TCbsMessageHandle handle( 0 );
    LockMessage( handle );

    // Close the subsession.
    RSubSessionBase::CloseSubSession( ECbsCloseTopicMessagesSubsession );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::GetMessageCount
// Returns the total amount of messages the topic contains.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::GetMessageCount( 
    TCbsTopicNumber aNumber, 
    TInt& aCount )
    {
    // Send request to get the total amount of messages
    aCount = 0;
    TPckgBuf< TInt > pckgCount( aCount );    
    const TIpcArgs args( aNumber, &pckgCount );

    TInt result = SendReceive( ECbsGetMessageCount, args );
    
    // If everything went fine, give the result to the client.
    if ( result == KErrNone )
        {
        aCount = pckgCount();
        }
    return result;
    }
    
// -----------------------------------------------------------------------------
// RCbsTopicMessages::GetMessage
// Returns message information.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::GetMessage( 
    TCbsTopicNumber aNumber, 
    TInt aIndex, 
    TCbsMessage& aMessage )
    {
    // Send request to get the message from server
    TPckgBuf< TCbsMessage > pckgMessage;
    const TIpcArgs args( aNumber, aIndex, &pckgMessage );

    TInt result = SendReceive( ECbsGetMessage, args );

    if ( result == KErrNone )
        {
        aMessage = pckgMessage();
        }
    else
        {
        aMessage.iHandle = 0;
        }

    return result;

    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::FindMessageByHandle
// Finds a message by given handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::FindMessageByHandle( 
    const TCbsMessageHandle& aHandle, 
    TCbsMessage& aMessage )
    {
    // Send request to the server to find a message by handle
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    TPckgBuf< TCbsMessage > pckgMessage;
    const TIpcArgs args( &pckgHandle, &pckgMessage );

    TInt result = SendReceive( ECbsFindMessageByHandle, args );

    // If everything went fine, update the message
    if ( result == KErrNone )
        {
        aMessage = pckgMessage();
        }
    else
        {
        aMessage.iHandle = 0;
        }

    return result;

    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::GetMessageIndexByHandle
// Returns the index of a message with given handle in topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::GetMessageIndexByHandle( 
    const TCbsMessageHandle& aHandle, 
    TInt& aIndex )
    {
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    TPckgBuf< TInt > pckgIndex( 0 );
    const TIpcArgs args( &pckgHandle, &pckgIndex );

    TInt result( SendReceive( ECbsGetMessageIndexByHandle, args ) );

    if ( result == KErrNone )
        {
        aIndex = pckgIndex();
        }
    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::DeleteMessage
// Deletes an existing message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::DeleteMessage( 
    const TCbsMessageHandle& aHandle )
    {
    // Send request to delete a message by handle
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    const TIpcArgs args( &pckgHandle );
    return SendReceive( ECbsDeleteMessage, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::SaveMessage
// Saves a message (i.e., the saved message won't be deleted to make 
// room for new messages).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::SaveMessage( 
    const TCbsMessageHandle& aHandle )
    {
    // Send request to save a message by handle
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    const TIpcArgs args( &pckgHandle );
    return SendReceive( ECbsSaveMessage, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::LockMessage
// Locks the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::LockMessage( 
    const TCbsMessageHandle& aHandle )
    {
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    const TIpcArgs args( &pckgHandle );
    return SendReceive( ECbsLockMessage, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::ReadMessage
// Sets the message as read.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::ReadMessage( 
    const TCbsMessageHandle& aHandle )
    {
    // Send request to set message read by handle
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    const TIpcArgs args( &pckgHandle );
    return SendReceive( ECbsReadMessage, args );
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::GetMessageContents
// Returns the message contents.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::GetMessageContents( 
    const TCbsMessageHandle& aHandle, 
    TDes& aBuffer )
    {
    // Send request to get the message contents
    TPckgBuf< TCbsMessageHandle > pckgHandle( aHandle );
    TInt length = aBuffer.MaxLength();
    const TIpcArgs args( &pckgHandle, length, &aBuffer );

    TInt result( SendReceive( ECbsGetMessageContents, args ) );
    if ( result != KErrNone )
        {
        aBuffer.Zero();
        }
    return result;
    }

// -----------------------------------------------------------------------------
// RCbsTopicMessages::GetNextAndPrevMessageHandle
// Returns the handles of messages that precede and succeed the 
// given message in server-side list of topic messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbsTopicMessages::GetNextAndPrevMessageHandle(
	const TCbsMessageHandle& aCurrentMsgHandle,
	TCbsMessageHandle& aPrevMsgHandle,
	TCbsMessageHandle& aNextMsgHandle,
	TInt& aPosition )
    {
    TPckgBuf< TCbsMessageHandle > pckgCurrentMsg( aCurrentMsgHandle );
    TPckgBuf< TCbsNextPrevMsgAndPosition > pckgResults;
    const TIpcArgs args( &pckgCurrentMsg, &pckgResults );

    TInt result( SendReceive( ECbsGetNextAndPrevMsgHandle, args ) );
    if ( result == KErrNone )
        {
        aPrevMsgHandle = pckgResults().iPrevMsg;
        aNextMsgHandle = pckgResults().iNextMsg;
        aPosition = pckgResults().iPosition;
        }
    return result;
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
