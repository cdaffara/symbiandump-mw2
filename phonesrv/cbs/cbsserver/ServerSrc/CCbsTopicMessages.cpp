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
* Description:  This module contains the implementation of CCbsTopicMessages class 
*                member functions. 
* 
*
*/


// INCLUDE FILES

#include <e32svr.h>
#include "CbsCommon.h"
#include "CbsServerPanic.h"
#include "CCbsTopicMessages.h"
#include "CCbsSession.h"
#include "CbsServerConstants.h"
#include "CCbsDbImpTopicMessages.h"
#include "CCbsServer.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsTopicMessages::CCbsTopicMessages
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsTopicMessages::CCbsTopicMessages( 
    CCbsSession& aSession, 
    CCbsDbImpTopicMessages& aMessages,
    CCbsRecEtel& aReceiver )
    : CCbsObject( aSession ), 
    iMessages( aMessages ),     
    iLocked( 0 ),
    iReceiver( aReceiver )
    {
    }


// -----------------------------------------------------------------------------
// CCbsTopicMessages::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsTopicMessages* CCbsTopicMessages::NewL( 
    CCbsSession& aSession, 
    CCbsDbImpTopicMessages& aMessages,
    CCbsRecEtel& aReceiver )
    {
    // Normal two phase construction
    CCbsTopicMessages* self = 
            new ( ELeave ) CCbsTopicMessages( aSession, aMessages, aReceiver );
    return self;
    }

    
// Destructor
CCbsTopicMessages::~CCbsTopicMessages()    
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsTopicMessages::~CCbsTopicMessages()");
    CBSLOGSTRING("CBSSERVER: <<< CCbsTopicMessages::~CCbsTopicMessages()");
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::HandleRequestsL
// Handle the requests for the object.
// Passes requests to proper functions
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsTopicMessages::HandleRequestsL( 
    const RMessage2& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsTopicMessages::HandleRequestsL()");
    TBool requestHandled ( ETrue );
    // Handle the requests that are for the subsession.
    switch ( aMessage.Function() )
        {
        case ECbsCloseTopicMessagesSubsession:                
            CloseTopicMessages();
            aMessage.Complete( KErrNone );
            break;
    
        case ECbsGetMessageCount:                
            GetMessageCountL();            
            break;

        case ECbsGetMessage:
            GetMessageL();                
            break;

        case ECbsFindMessageByHandle:                               
            FindMessageByHandleL();            
            break;

        case ECbsGetMessageIndexByHandle:                
            GetMessageIndexByHandleL();            
            break;

        case ECbsGetNextAndPrevMsgHandle:                
            GetNextAndPrevMsgHandleL();            
            break;
   
        case ECbsDeleteMessage:                
            DeleteMessageL( iReceiver );            
            break;

        case ECbsSaveMessage:                
            SaveMessageL( iReceiver );            
            break;

        case ECbsLockMessage:            
            LockMessageL();            
            break;

        case ECbsReadMessage:                
            ReadMessageL( iReceiver );            
            break;

        case ECbsGetMessageContents:                
            GetMessageContentsL();            
            break;

        default:
            requestHandled = EFalse;
            break;
        }    
    CBSLOGSTRING2("CBSSERVER: <<< CCbsTopicMessages::HandleRequestsL(), returning requestHandled: %d", requestHandled );   
    return requestHandled;
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::CloseTopicMessages
// Closes the subsession.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::CloseTopicMessages()
    {
    // Removes the object.
    Session().Server().DeleteObjectByHandle( Message().Int3() );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::GetMessageCountL
// Returns the total amount of messages to the client.   
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::GetMessageCountL()
    {
    // First, get the handle from the client    
    TInt topicNumber( 0 );
    topicNumber = Message().Int0();

    // Then, get the message count from the database.
    TInt count( 0 );
    iMessages.GetMessageCountL( static_cast<TUint16> ( topicNumber ), count );
    
    // Write the message count to the client side.
    TPckgBuf< TInt > pckgCount( count );
    Message().WriteL( 1, pckgCount );

    // Complete the request.
    Message().Complete( KErrNone );

    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::GetMessageL
// Returns the requested message to the client. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::GetMessageL()
    {  
    // Read the parameters from the client side
    TInt topicNumber( 0 );
    TInt index( 0 );
    topicNumber = Message().Int0();
    index = Message().Int1();
    
    // Then get the message from database.
    TPckgBuf< TCbsDbMessage > pckgMessage;
    iMessages.GetMessageL( static_cast<TUint16> ( topicNumber ), index, pckgMessage() );

    // And finally write the message information to the client side
    Message().WriteL( 2, pckgMessage );

    // And complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::FindMessageByHandleL
// Finds a message by its handle. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::FindMessageByHandleL()
    {
    // Read the parameters from the client side.
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    // Find the message.
    TPckgBuf< TCbsMessage > pckgMessage;
    iMessages.FindMessageByHandleL( pckgHandle(), pckgMessage() );

    // Write the result to the client side.
    Message().WriteL( 1, pckgMessage );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::GetMessageIndexByHandleL
// Returns message index in topic by message handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::GetMessageIndexByHandleL()
    {
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    TInt index( iMessages.FindMessagePositionByHandleL( pckgHandle() ) );

    TPckgBuf< TInt > pckgIndex( index );
    Message().WriteL( 1, pckgIndex );

    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::DeleteMessageL
// Deletes a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::DeleteMessageL( const CCbsRecEtel& aReceiver )
    {
    // Read the handle from the client side.
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    // And then try to delete the message.
    iMessages.DeleteMessageL( pckgHandle(), aReceiver );

    // Finally, complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::SaveMessageL
// Saves a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
void CCbsTopicMessages::SaveMessageL( const CCbsRecEtel& aReceiver )
    {
    // Read the parameter from the client side.
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    // Save the message.
    iMessages.SaveMessageL( pckgHandle(), aReceiver );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::LockMessageL
// Locks a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::LockMessageL()
    {
    // Read the parameter from the client side.
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    TCbsDbMessageHandle handle( pckgHandle() );

    if ( handle != iLocked )
        {
        iMessages.LockMessageL( iLocked, handle );
        iLocked = handle;
        }

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::ReadMessageL
// Reads a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::ReadMessageL( const CCbsRecEtel& aReceiver )
    {
    // Read parameter from the client side.
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    // Perform the operation.
    iMessages.ReadMessageL( pckgHandle(), aReceiver );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::GetMessageContentsL
// Returns the content of the message to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::GetMessageContentsL()
    {
    // Read the parameters from the client side.
    TPckgBuf< TCbsMessageHandle > pckgHandle( 0 );
    Message().ReadL( 0, pckgHandle );

    TInt bufSize( 0 );
    bufSize = Message().Int1();

    TCbsDbMessage message;
    iMessages.FindMessageByHandleL( pckgHandle(), message );
    
    if ( message.iLength != 0 )
        {
        // Message is not empty. 
        // First allocate memory and then get the contents.
        // Finally write data to client side.
        TInt size ( ( message.iLength < bufSize ) ? 
            message.iLength : bufSize );

        // guaranteed to allocate n bytes, n >= size
        HBufC* buffer = HBufC::NewLC( size );    // on CS

        TPtr16 pointer( buffer->Des() );
        pointer.Zero();

        // Note: parameter 'size' required, since MaxLength 
        // may be > bufSize
        iMessages.GetMessageContentsL( pckgHandle(), pointer, size );

#ifndef _DEBUG
        Message().WriteL( 2, pointer );
#else
        TRAPD( result, Message().WriteL( 2, pointer ) );
        if ( result != KErrNone )
            {
            RDebug::Print(_L("Server buffer length: %d, max: %d"), 
                pointer.Length(), pointer.MaxLength());
            __DEBUGGER();
            User::Leave( result );
            }
        
#endif
        CleanupStack::PopAndDestroy(); // buffer
        }

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsTopicMessages::GetNextAndPrevMsgHandleL
// Retrieves and returns the handles of messages that
// precede and succeed the given message in topic.
// Also returns two flags indicating whether the
// given message is the first and/or the last message in topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsTopicMessages::GetNextAndPrevMsgHandleL()
    {
    // Retrieve the requested topic number
    TPckgBuf< TCbsMessageHandle > pckgCurrentMsg( 0 );
    Message().ReadL( 0, pckgCurrentMsg );

    // Determine requested information
    TCbsMessageHandle nextMsg(0);
    TCbsMessageHandle prevMsg(0);
    TInt position(0);
    iMessages.GetNextAndPrevMsgHandleL(
        pckgCurrentMsg(), nextMsg, prevMsg, position);

    // Write results back
    TPckgBuf< TCbsNextPrevMsgAndPosition > pckgResults;
    pckgResults().iPrevMsg = prevMsg;
    pckgResults().iNextMsg = nextMsg;
    pckgResults().iPosition = position;

    Message().WriteL( 1, pckgResults );

    // Complete request
    Message().Complete( KErrNone );

    }
        
// ========================== OTHER EXPORTED FUNCTIONS =========================
//  End of File  
