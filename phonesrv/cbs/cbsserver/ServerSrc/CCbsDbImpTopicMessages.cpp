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
* Description:  This module contains the implementation of CCbsDbImpTopicMessages class 
*                member functions. 
*
*/



// INCLUDE FILES

#include <e32svr.h>

#include <shareddataclient.h>

#include "CbsServerPanic.h"
#include "CbsStreamHelper.h"
#include "CbsUtils.h"
#include "CbsDbConstants.h"
#include "CCbsDbImpTopicList.h"
#include "CCbsDbImpTopicMessages.h"
#include "CCbsRecEtel.h"
#include "CCbsReceiverHelper.h"

#include "CbsLogger.h"

// CONSTANTS

// Expected maximum number of locked messages at once. 
const TInt KDefaultSpaceForLockedMessages = 1;

// CB message header size in bytes.
const TInt KCbMessageHeaderSize = 16;

// The space used for the header of messages stream
const TInt KMessageRootStreamSize = 6;

/// The space used by one message in the messages stream
const TInt KMessageEntrySize = 3;

// Default space reserved for topic messages in message stream
const TInt KDefaultSpaceForMessages = 6;

// Space for reading messages
const TInt KReadMessageSize = 92;


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::CCbsDbImpTopicMessages
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicMessages::CCbsDbImpTopicMessages( 
    CCbsDbImpTopicList& aTopicList, 
    RFs& aFs )
    :iTopicList( aTopicList ), 
    iCacheValid( EFalse ), 
    iCachedTopicNumber( 0 ), 
    iFs( aFs )
    {
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::ConstructL()
    {
    // Allocate the default size for the message list.
    iMessageList = 
        new ( ELeave ) CArrayFixFlat< TCbsDbImpTopicMessagesCacheItem > 
            ( KCbsDbMaxReceivedMessages + KCbsDbMaxSavedMessages );

    // Allocate the array for locked messages.
    iLockedMessages = new ( ELeave ) CArrayFixFlat< TCbsDbMessageHandle >
        ( KDefaultSpaceForLockedMessages );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicMessages* CCbsDbImpTopicMessages::NewL( 
    CCbsDbImpTopicList& aTopicList, RFs& aFs )
    {
    CCbsDbImpTopicMessages* self = 
        new ( ELeave ) CCbsDbImpTopicMessages( aTopicList, aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

    
// Destructor
CCbsDbImpTopicMessages::~CCbsDbImpTopicMessages()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::~CCbsDbImpTopicMessages()");
    // Invalidate cache.
    InvalidateCache();

    // And then delete the message list.
    delete iMessageList;
    
    // Delete locked messages.
    delete iLockedMessages; 
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::~CCbsDbImpTopicMessages()");   
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::InvalidateCache
// Resets the cache to default values.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::InvalidateCache()
    {    
    // Initialize members back to default.
    iCacheValid = EFalse;
    iCachedTopicNumber = 0;
        
    // Resize the message list. After the call is made, the
    // array is empty.
    if ( iMessageList )
        {
        iMessageList->Reset();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::InvalidateCacheIfTopic
// Resets the cache to default values.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::InvalidateCacheIfTopic( 
    const TCbsDbTopicNumber& aNumber )
    {
    // Check the handle and then invalidate
    if ( iCachedTopicNumber == aNumber )
        {
        InvalidateCache();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::CreateDefaultTopicMessagesStreamL
// Creates a default topic messages stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TStreamId CCbsDbImpTopicMessages::CreateDefaultTopicMessagesStreamL(
    CStreamStore& aStore )
    {
    // Create the stream.
    RStoreWriteStream outstream;
    TStreamId id = outstream.CreateLC( aStore ); // on CS

    // Write total amount of messages.
    outstream.WriteInt16L( 0 );

    // Write total amount of space for messages.
    outstream.WriteInt16L( KDefaultSpaceForMessages );

    for ( TInt index( 0 ); index < KDefaultSpaceForMessages; index++ )
        {
        // "Saved"-flag
        CbsStreamHelper::WriteBoolL( outstream, EFalse );
        // Write message stream id.
        outstream << TStreamId( 0 );
        }

    outstream.CommitL();
    CleanupStack::PopAndDestroy();  // outstream

    return id;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::CreateDefaultTopicMessagesStreamL
// Delete all topics messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DeleteAllTopicMessagesL( 
    CStreamStore& aSavedStore,
    CStreamStore& aUnsavedStore,
    RReadStream& aIn )
    {
    // Read total amount of messages.
    TInt total( aIn.ReadInt16L() );
    
    // Skip space for messages.
    aIn.ReadInt16L();

    // Now, delete all streams..
    for ( TInt index( 0 ); index < total; index++ )
        {

        TBool isSaved( CbsStreamHelper::ReadBoolL( aIn ) );
        TStreamId id;

        // Read stream id.
        aIn >> id;
        CStreamStore& store = isSaved ? aSavedStore : aUnsavedStore;
        store.DeleteL( id );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::IsLockedMessagesInTopic
// Determines whether or not there are any locked messages in 
// the topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsDbImpTopicMessages::IsLockedMessagesInTopic( 
    const TCbsDbTopicNumber& aNumber ) const
    {
    TBool result( EFalse );

    // Checks whether or not there is a locked message in the topic.
    TInt count( iLockedMessages->Count() );
    for ( TInt index( 0 ); index < count; index++ )
        {
        if ( iTopicList.ExtractTopicNumber( 
            iLockedMessages->At( index ) ) == aNumber )
            {
            result = ETrue;
            }
        }    
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::IsLockedMessages
// Determines whether there is at least one locked message.
// Returns ETrue, if there are any locked messages in the 
// database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsDbImpTopicMessages::IsLockedMessages() const
    {
    return ( iLockedMessages->Count() > 0 );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::GetMessageCountL
// Returns the total amount of messages the topic contains.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::GetMessageCountL( 
    const TCbsDbTopicNumber& aNumber, 
    TInt& aCount )
    {
    // Load cache.
    LoadCacheL( aNumber );

    // and then get the message count.
    aCount = iMessageList->Count();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::GetMessageL
// Returns message information.
// Returns the message matching the given
// index value. Leaves if the index is not valid.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::GetMessageL( 
    const TCbsDbTopicNumber& aNumber, 
    TInt aIndex, 
    TCbsDbMessage& aMessage )
    {
    // Load the cache.
    LoadCacheL( aNumber );

    // Check if the index is not valid.
    if ( ( aIndex < 0 ) || ( aIndex >= iMessageList->Count() ) )
        {
        User::Leave( KErrNotFound );
        }

    // Load the message.
    LoadMessageL( aIndex, aMessage );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::FindMessageByHandleL
// Finds the message for given handle and returns it.
// Returns the message matching the given
// index value.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::FindMessageByHandleL( 
    const TCbsDbMessageHandle& aHandle, 
    TCbsDbMessage& aMessage )
    {
    // Load cache
    LoadCacheL( iTopicList.ExtractTopicNumber( aHandle ) );

    // Load message
    LoadMessageL( FindMessagePositionByHandleL( aHandle ), aMessage );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::GetMessageContentsL
// Returns the contents of a message.
// Stores the contents of the message to the buffer aContents. 
// If aContents is too small to contain the whole message
// body, the message body is truncated.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::GetMessageContentsL( 
    const TCbsDbMessageHandle& aHandle, 
    TPtr& aContents,
    TUint aSize )
    {
    LoadCacheL( iTopicList.ExtractTopicNumber( aHandle ) );

    TInt position( FindMessagePositionByHandleL( aHandle ) );

    TCbsDbMessage message;
    LoadMessageL( position, message );

    LoadMessageContentsL( iMessageList->At( position ).iContentsId, 
                          aContents, message.iPermanent, aSize );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DeleteMessageL
// Deletes a message. Leaves if the message is protected
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DeleteMessageL( 
    const TCbsDbMessageHandle& aHandle,
    const CCbsRecEtel& aReceiver )
    {
    // Get the topic handle.
    LoadCacheL( iTopicList.ExtractTopicNumber( aHandle ) );

    // Check if the message is locked.
    if ( IsMessageLocked( aHandle ) )
        {
        // Yes, leave.
        User::Leave( KErrAccessDenied );
        }
    else
        {
        // Otherwise, delete.
        TRAPD( error, DoDeleteMessageL( 
            FindMessagePositionByHandleL( aHandle ) ) );

        if ( error != KErrNone )
            {
            // Failed.
            RevertFileOperationL( error );
            }
        else
            {
            // Update the soft notification dialog, if topic is hotmarked
            UpdateSoftNotificationL( aHandle, aReceiver );
            }
        }

    iTopicList.NotifyTopicModifiedL( 
        iTopicList.ExtractTopicNumber( aHandle ) );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::AddMessageL
// Creates a new message to the topic.
// Stores a handle to the message in aHandle.
// FFS critical level check is made prior to operation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::AddMessageL( 
    const TCbsDbTopicNumber& aNumber, 
    TCbsDbMessage& aMessage, 
    const TPtrC& aContents )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::AddMessageL()");
    
    LoadCacheL( aNumber );

    aMessage.iLength = aContents.Length();
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::AddMessageL(): Content length: %d.", aMessage.iLength );

    // Generate a new message handle.
    aMessage.iHandle = GenerateNewMessageHandle( aNumber );    
    
    // Make sure that the message is not inserted as permanent
    aMessage.iPermanent = EFalse;

    TRAPD( error, DoAddMessageL( aMessage, aContents ) );
    if ( error != KErrNone )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::AddMessageL(): DoAddMessageL() failed, error: %d.", error );
        
        // Failed.
        RevertFileOperationL( error );
        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::AddMessageL(): RevertFileOperationL() finished OK." );
        }
        
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::AddMessageL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::SaveMessageL
// Saves a message. 
// This operation is called "keeping" in the UI. 
// The CBS server has a global limit for saved 
// messages. The function leaves, if this is exceeded. 
// A saved message has it's permanent-flag raised.
// FFS critical level check is made prior to the operation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::SaveMessageL( 
    const TCbsDbMessageHandle& aHandle,
    const CCbsRecEtel& aReceiver )
    {
    // Load cache.
    TCbsTopicNumber topicNumber( iTopicList.ExtractTopicNumber( aHandle ) );
    LoadCacheL( topicNumber );

    // Now find message by handle.
    TInt position( FindMessagePositionByHandleL( aHandle ) );

    TCbsDbMessage message;
    LoadMessageL( position, message );
    TStreamId contentsId( iMessageList->At( position ).iContentsId );

    // This function should not be called if the message is already permanent.
    if ( message.iPermanent )
        {
        User::Leave( KErrAlreadyExists );
        }

    // Check that the limit for maximum saved messages/topic is not exceeded.
    TCbsDbTopic topic;
    iTopicList.FindTopicByNumberL( topicNumber, topic );
    if ( iTopicList.TotalSavedMessages() >= KCbsDbMaxSavedMessages )
        {
        User::Leave( KErrOverflow );
        }

    // FFS critical level check (header is 16 bytes, message coded in Unicode)
    CbsUtils::FFSCriticalLevelCheckL( KCbMessageHeaderSize + 
        2 * message.iLength, iFs );

    // Create a temporary buffer for message body and load the contents into
    // the buffer.
    HBufC* buffer = HBufC::NewLC( message.iLength ); // on CS
    TPtr contentPtr( buffer->Des() );
    LoadMessageContentsL( contentsId, contentPtr, EFalse, message.iLength );

    // Delete message's previous header and body streams.
    TCbsDbImpTopicMessagesCacheItem item( iMessageList->At( position ) );    

    TRAPD( result, DoSaveMessageL( message, item, contentPtr, position ) );
    CleanupStack::PopAndDestroy( buffer );
    if ( result != KErrNone )
        {
        RevertFileOperationL( result );
        }
    else
        {
        iTopicList.NotifyTopicModifiedL( 
            iTopicList.ExtractTopicNumber( aHandle ) );
            
        // Update the soft notification dialog, if topic is hotmarked
        UpdateSoftNotificationL( aHandle, aReceiver );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DoSaveMessageL
// Saves a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DoSaveMessageL( 
    TCbsDbMessage& aMessage,
    const TCbsDbImpTopicMessagesCacheItem& aItem,
    const TDesC& aContentPtr,
    TInt aPosition )
    {
    CFileStore* store = iTopicList.UnsavedMessagesStore();
    store->Delete( aItem.iId );
    store->Delete( aItem.iContentsId );

    TBool needToUpdateReadCounter( EFalse );
    aMessage.iPermanent = ETrue;
    if ( !aMessage.iRead ) 
        {
        needToUpdateReadCounter = ETrue;
        aMessage.iRead = ETrue;
        }

    // Create new streams for message header and body.
    TStreamId contentsStream( CreateMessageContentsStreamL( 
        *iTopicList.TopicStoreL(), aContentPtr ) );
    TStreamId messageStream( CreateMessageStreamL( 
        *iTopicList.TopicStoreL(), aMessage, contentsStream ) );

    // Update message's cache item information.
	TCbsDbImpTopicMessagesCacheItem& item = iMessageList->At( aPosition );
    item.iContentsId = contentsStream;
    item.iId = messageStream;
    item.iIsSaved = ETrue;
    item.iMessage = aMessage;

    // Update topic messages stream; the cache item is saved into the stream.
    UpdateTopicMessagesStreamL( EFalse );

    iTopicList.InformMessageSavedL( aMessage.iHandle );
    if ( needToUpdateReadCounter )
        {
        iTopicList.InformUnreadMessageReadL( aMessage.iHandle );
        }

    // Commit changes to both files.
    iTopicList.CommitFilesL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::ReadMessageL
// Marks the message read
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::ReadMessageL(
    const TCbsDbMessageHandle& aHandle,
    const CCbsRecEtel& aReceiver )
    {
    //Checked that there is space for writing data on FFS.
    CbsUtils::FFSCriticalLevelCheckL( KReadMessageSize, iFs );

    // Load cache.
    TCbsDbTopicNumber topicNum( iTopicList.ExtractTopicNumber( aHandle ) );
    LoadCacheL( topicNum );

    // Now find message by handle.
    TInt position( FindMessagePositionByHandleL( aHandle ) );

    TCbsDbMessage message;
    LoadMessageL( position, message );
    TStreamId contentsId( iMessageList->At( position ).iContentsId );
    TStreamId id( iMessageList->At( position ).iId );

    // Check if the message is already read, then there is nothing more to do.
    if ( !message.iRead )
        {
        message.iRead = ETrue;

        TRAPD( error, DoReadMessageL( id, message, contentsId ) );
        if ( error == KErrNone )
            {
            iMessageList->At( position ).iMessage = message;
            
            iTopicList.NotifyTopicModifiedL( 
                iTopicList.ExtractTopicNumber( aHandle ) );
        
            // Update the soft notification dialog, if topic is hotmarked
            UpdateSoftNotificationL( aHandle, aReceiver );
            }
        else
            {
            RevertFileOperationL( error );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::UpdateHandlesOfTopicMessagesL
// Updates handles of messages in the topic with the number given in
// aOldTopicNumber to match new topic number. 
// Traverses through the given topic processing
// each message stream encountered. Since the topic number is
// the primary key and previous message handles can be assumed
// to be unique, updating the high word of the handle to
// match the new topic number is adequate.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::UpdateHandlesOfTopicMessagesL(
    const TCbsDbTopicNumber& aOldTopicNumber, 
    const TCbsDbTopicNumber& aNewTopicNumber )
    {
    LoadCacheL( aOldTopicNumber );

    TInt msgs( iMessageList->Count() );

    for ( TInt i( 0 ); i < msgs; i++ )
        {
        // Fetch message at this position
        TCbsDbMessage message;
        LoadMessageL( i, message );
        TStreamId contentsStreamId( iMessageList->At( i ).iContentsId );
        TStreamId messageStreamId( iMessageList->At( i ).iId );

        // Insert new topic number as the high word of the handle
        message.iHandle = ( aNewTopicNumber << 16 ) |
            ( message.iHandle & 0xFFFF );
        DoUpdateMessageL( messageStreamId, message, contentsStreamId );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::LockMessageL
// Unlocks one message and then locks another message.
// Message that is locked can not be removed. Topic that 
// contains a locked message can not be removed.
// Note that locking status is not persistent in the sense that 
// when the power is switched off and turned on, no messages 
// are automatically locked.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::LockMessageL( 
    const TCbsDbMessageHandle& aUnlock, 
    const TCbsDbMessageHandle& aLock )
    {
    // Check if the message exists: leave if not found, ignore return value
    if ( aLock != 0 )
        {
        TCbsDbMessage message;
        TInt position( FindMessagePositionByHandleL( aLock ) );
        LoadMessageL( position, message );
        }

    TBool unlockNull( ETrue );

    if ( aUnlock != 0 )
        {
        DeleteFromLocked( aUnlock );
        unlockNull = EFalse;
        }

    if ( aLock != 0 )
        {
        InsertToLockedL( aLock ) ;
        }

    if ( unlockNull == EFalse )
        {
        LoadCacheL( iTopicList.ExtractTopicNumber( aUnlock ) );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::GetNextAndPrevMsgHandleL
// Returns the handles of the next and previous message
// when given a current message. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::GetNextAndPrevMsgHandleL(
    const TCbsDbMessageHandle& aCurrentMsg,
    TCbsDbMessageHandle& aNextMsg, 
    TCbsDbMessageHandle& aPrevMsg, 
    TInt& aPosition)
    {
    LoadCacheL( iTopicList.ExtractTopicNumber( aCurrentMsg ) );

    // Load the current message
    TInt index( FindMessagePositionByHandleL( aCurrentMsg ) );

    // Reset position indications
    aPosition = 0;

    // Retrieve position indications and message handles
    if ( index == 0 )
        {
        aPosition |= ECbsHead;
        }
    else
        {
        TCbsDbMessage prevMessage;
        LoadMessageL( index - 1, prevMessage );
        aPrevMsg = prevMessage.iHandle;
        }

    // If index points to the last element of iMessageList,
    // raise flag ECbsTail.
    if ( index == iMessageList->Count()-1 )
        {
        aPosition |= ECbsTail;
        }
    else
        {
        TCbsDbMessage nextMessage;
        LoadMessageL( index+1, nextMessage );
        aNextMsg = nextMessage.iHandle;
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DoAddMessageL
// Inserts a message into the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DoAddMessageL(
    TCbsDbMessage& aMessage, 
    const TDesC& aContents )
    {
    // Cache is assumed to be loaded. Message is assumed not to
    // already exist in the DB.
    CFileStore* store = aMessage.iPermanent ? 
        iTopicList.TopicStoreL() : iTopicList.UnsavedMessagesStore();

    if ( aMessage.iPermanent )
        {
        // About to write to FFS: make critical level check.
        // Message is in Unicode characters, so its size
        // is double the length.
        CbsUtils::FFSCriticalLevelCheckL( KCbMessageHeaderSize + 
            2 * aMessage.iLength, iFs );
        }
    else
        {
        CbsUtils::VolumeCriticalLevelCheckL(
            iTopicList.UnsavedMessagesFilename(),
            KCbMessageHeaderSize + 2 * aMessage.iLength, iFs );
        }

    // Create stream for contents.
    TStreamId contentsStream( CreateMessageContentsStreamL( 
        *store, aContents ) );

    // Now we have to find the position to which the message should be
    // inserted.         
    TInt positionToBeInserted( -1 );

    TTime messageTime( aMessage.iDateTime );
    TInt index( 0 );

    // Go through all messages in topic and find the position.
    TInt count( iMessageList->Count() );
    for ( ; index < count && positionToBeInserted == -1; index++ )
        {
        TCbsDbMessage message;
        LoadMessageL( index, message );

        TTime tmp( message.iDateTime );

        if ( tmp <= messageTime )
            {
            positionToBeInserted = index;
            }
        }

    // If we looped through, append
    if ( positionToBeInserted == -1 )
        {
        positionToBeInserted = index;
        }

    // Create message stream.
    TStreamId messageStream( CreateMessageStreamL( *store, 
        aMessage, contentsStream ) );

    // Add to internal cache.
    TCbsDbImpTopicMessagesCacheItem item;
    
    item.iId = messageStream;
    item.iIsMessage = EFalse;
    item.iIsSaved = aMessage.iPermanent;

    iMessageList->InsertL( positionToBeInserted, item );

    // Check if message should be deleted.
    DeleteReceivedIfNecessaryL();

    // Update topic messages stream.
    UpdateTopicMessagesStreamL( EFalse );

    // Update counters.
    iTopicList.InformNewMessageReceivedL( aMessage.iHandle );

    // Commit changes to both stores.
    iTopicList.CommitFilesL();

    // Inform observers. Done after commit to make sure there's
    // a message to inform of.
    iTopicList.NotifyNewMessageArrivedL( aMessage.iHandle );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DoDeleteMessageL
// Deletes a message from the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DoDeleteMessageL( 
    TInt aPosition )
    {    
    TCbsDbMessage message;
    LoadMessageL( aPosition, message );

    // Delete the message, if it is possible.
    DeleteMessageByPositionL( aPosition );
    DeleteReceivedIfNecessaryL();
    UpdateTopicMessagesStreamL( ETrue );

    // Inform the topic list about deletion.
    iTopicList.InformMessageDeletedL( message.iHandle, 
        message.iPermanent, message.iRead );

    iTopicList.CommitFilesL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DoUpdateMessageL
// Updates the information for a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DoUpdateMessageL( 
    const TStreamId& aId, 
    const TCbsDbMessage& aMessage, 
    const TStreamId& aContentsId ) const
    {
    // Update the stream contents.
    CFileStore* store = aMessage.iPermanent ? 
        iTopicList.TopicStoreL() : iTopicList.UnsavedMessagesStore();

    RStoreWriteStream outstream;
    outstream.ReplaceLC( *store, aId ); // on CS

    WriteMessageInformationL( outstream, aMessage, aContentsId );

    outstream.CommitL();
    CleanupStack::PopAndDestroy(); // outstream
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DoReadMessageL
// Marks a message read by updating it in the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DoReadMessageL( 
    const TStreamId& aId, 
    const TCbsDbMessage& aMessage, 
    const TStreamId& aContentsId )
    {
    DoUpdateMessageL( aId, aMessage, aContentsId );
    iTopicList.InformUnreadMessageReadL( aMessage.iHandle );
    iTopicList.CommitFilesL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DoDeleteL
// Deletes messages that are not supposed to be in the topic.
// Because of message locking it is possible that there are more 
// received messages than there should be. This method tries to 
// delete this kind of message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DoDeleteL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::DoDeleteL()");

    // Delete temporary messages and update topic messages stream.
    if ( DeleteReceivedIfNecessaryL() )
        {
        UpdateTopicMessagesStreamL( ETrue );    
        }
    iTopicList.CommitFilesL();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::DoDeleteL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::FindMessageByKeyL
// Returns a handle to a message with the given key.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicMessages::FindMessageByKeyL( 
    TCbsDbTopicNumber aNumber, 
    TCbsDbMessageKey aKey,
    TCbsDbMessage& aMessage )
    {
    TCbsDbMessage message;
    TInt count;
    TBool result( KErrNotFound );

    // Find a message even if the update numbers don't match
    aKey &= 0xfff0;

    // Try to find a message with the same key in the topic.
    LoadCacheL( aNumber );
    GetMessageCountL( aNumber, count );

    for ( TInt index( 0 ); ( index < count ) && ( result != KErrNone ); index++ )
        {
        LoadMessageL( index, message );

        if ( (message.iKey & 0xfff0) == aKey )
            {
            aMessage = message;
            result = KErrNone; // message was found
            }
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::FindMessagePositionByHandleL
// Returns the position of a message with the given handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicMessages::FindMessagePositionByHandleL( 
    const TCbsDbMessageHandle& aHandle )
    {
    // Load the cache.
    LoadCacheL( iTopicList.ExtractTopicNumber( aHandle ) );

    // Now, find the message
    TInt count( iMessageList->Count() );

    TCbsDbMessage message;
    TBool found( EFalse );

    // Go through messages.
    TInt index( 0 );
    for ( ; ( index < count ) && !found; )
        {
        // Load the message.
        LoadMessageL( index, message );
        
        // Check if it is this one.
        if ( message.iHandle == aHandle )
            {
            found = ETrue;
            }
        else
            {
            index++;
            }        
        }

    if ( !found )
        {
        User::Leave( KErrNotFound );
        }
     
    return index;        
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::LoadCacheL
// Loads the cache, if it is not already loaded with this topic information.
// The method does not load any message information to the cache 
// - only what is really necessary.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::LoadCacheL( 
    const TCbsDbTopicNumber& aNumber )
    {
    CBSLOGSTRING2("CBSSERVER: >>> CCbsDbImpTopicMessages::LoadCacheL(), topic number: %d.", aNumber );
    
    // Check if the cache is valid. If not, load it.
    if ( !( iCacheValid && ( iCachedTopicNumber == aNumber ) ) ) 
        {        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::LoadCacheL(): Calling InvalidateCache()...");        
        // Invalidate the old cache, if it is necessary.
        InvalidateCache();        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::LoadCacheL(): InvalidateCache() called OK.");

        // Load message queue ids.
        TStreamId id;
        iTopicList.GetTopicMessagesIdL( aNumber, id );

        ReadTopicMessagesCacheItemsL( *iTopicList.TopicStoreL(), 
            id, *iMessageList, iSpaceForMessages );
    
        // Cache is valid.
        iCachedTopicNumber = aNumber;
        iCacheValid = ETrue;

        // Check if it there are more topics than there should be.
        TCbsDbTopic topic;
        iTopicList.FindTopicByNumberL( aNumber, topic );
        if ( ( iMessageList->Count() - topic.iSavedMessages ) > 
             KCbsDbMaxReceivedMessages ) 
            {
            TRAPD( error, DoDeleteL() );
            CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::LoadCacheL(): DoDeleteL() finished with %d.", error );        
            
            if ( error != KErrNone )
                {
                RevertFileOperationL( error );
                }
            }
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::LoadCacheL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::LoadMessageL
// Loads the message header to the cache.
// Note that it is assumed that the index is in proper range and 
// the cache is already loaded.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::LoadMessageL( 
    TInt aIndex, 
    TCbsDbMessage& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::LoadMessageL()");

    // Check that the aIndex is in proper range and.
    // that the cache is valid (just for sure).
    if ( aIndex < 0 || aIndex >= iMessageList->Count() ||
         !iCacheValid )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::LoadMessageL(): Leaving with KErrNotFound...");
        User::Leave( KErrNotFound );
        }

    // Check if the message information is already loaded.
    if ( iMessageList->At( aIndex ).iIsMessage )
        {       
        // Yes, just make a fast copy.
        aMessage = iMessageList->At( aIndex ).iMessage;
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::LoadMessageL(): Msg already loaded, fast copy made.");
        }
    else
        {
        TStreamId id( TStreamId( 0 ) );
        // Choose the file store: saved messages are stored elsewhere
        // than other messages.
        CFileStore* store = iMessageList->At( aIndex ).iIsSaved ?
            iTopicList.TopicStoreL() : 
            iTopicList.UnsavedMessagesStore();
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::LoadMessageL(): Store created.");

        // Then read the message. If the message stream cannot be
        // found, delete the reference to the message
        TRAPD( error, ReadMessageInformationL( *store, 
            iMessageList->At( aIndex ).iId, aMessage, id ) );
            
        CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::LoadMessageL(): ReadMessageInformationL() finished with %d.", error );
        
        if( error == KErrNotFound )
            {
            DeleteMessageByPositionL( aIndex );
            UpdateTopicMessagesStreamL( ETrue );
            iTopicList.CommitFilesL();    
            }
        
        CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::LoadMessageL(): Leaving if error code < 0: error %d...", error );
        User::LeaveIfError( error );

        // And copy to the internal cache.
        iMessageList->At( aIndex ).iIsMessage = ETrue;
        iMessageList->At( aIndex ).iContentsId = id;
        iMessageList->At( aIndex ).iMessage = aMessage;
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::LoadMessageL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::LoadMessageContentsL
// Loads message contents (as much as it fits).
// Restores the contents of a message
// by reading a stream specified in parameter aContentsId.
// Save status of message is required because saved and unsaved
// messages are stored on separate files.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::LoadMessageContentsL( 
    const TStreamId& aContentsId, 
    TPtr& aContents,
    TBool aIsSaved,
    TUint aSize ) const
    {
    CFileStore* store = aIsSaved ? 
        iTopicList.TopicStoreL() :
        iTopicList.UnsavedMessagesStore();

    // Read data to the buffer.
    RStoreReadStream instream;
    instream.OpenLC( *store, aContentsId ); // on CS

    // Read the data into a temporary buffer first.
	// Then copy to the aContents descriptor only
	// as much as it can take.
    TUint length( instream.ReadInt16L() );
    if ( length > KCbsMaxCharsInMessage )
        {
        User::Leave( KErrCorrupt );
        }

	HBufC* buf = HBufC::NewL( instream, length );    
	if ( length > aSize )
		{
		length = aSize;
		}

	aContents.Copy( buf->Ptr(), length );
	delete buf;

    CleanupStack::PopAndDestroy(); // instream
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::GenerateNewMessageHandle
// Generates a new, unique handle for a message.
// The generation uses topic number as 
// basis and then increments the handle value until an unique
// handle is found.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbMessageHandle CCbsDbImpTopicMessages::GenerateNewMessageHandle( 
    const TCbsDbTopicNumber& aNumber )
    {
    // Start with a reasonable seed. We'll try the number
    // of messages in topic, plus one.
    TUint16 handleLow( TUint16( iMessageList->Count() + 1 ) );
    TCbsDbMessageHandle handle( 1 );
    TInt error( KErrNone );

    while ( error == KErrNone )
        {
        handleLow++;
        if ( handleLow == 0 ) 
            {
            handleLow++;
            }

        // Generate a new message handle.
        handle = iTopicList.GenerateMessageHandle( aNumber, 
            handleLow );

        TRAP( error, ( FindMessagePositionByHandleL( handle ) ) );
        }
    return handle;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DeleteReceivedIfNecessaryL
// Checks that if there are too many received messages in the internal cache, 
// then they will be deleted if they are not locked.  
// Note: Count() is changed in for-loop.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsDbImpTopicMessages::DeleteReceivedIfNecessaryL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::DeleteReceivedIfNecessaryL()");
    
    // So check if there are too many received messages in the topic. 
    // Thus delete if message is not locked.

    __ASSERT_DEBUG( iCacheValid, CbsServerPanic( ECbsCacheNotValid ) );

    TInt totalReceived( 0 );
    TCbsDbMessage message;
    TBool result( EFalse );
    TInt index( 0 );

    for ( index = 0; index < iMessageList->Count(); index++ ) 
        {
        LoadMessageL( index, message );

        // Count the amount of received messages
        if ( !message.iPermanent )
            {
            totalReceived++;
            
            // If there are too many, then check that it can be deleted.
            if ( totalReceived > KCbsDbMaxReceivedMessages && 
                !IsMessageLocked( message.iHandle ) )
                {
                // Delete the message.
                DeleteMessageByPositionL( index );
                iTopicList.InformMessageDeletedL
                                    ( message.iHandle, 
                                      message.iPermanent, 
                                      message.iRead );
                result = ETrue;
                
                // They are always deleted from internal cache
                index--;
                totalReceived--;
                }
            }
        }

    CBSLOGSTRING2("CBSSERVER: <<< CCbsDbImpTopicMessages::DeleteReceivedIfNecessaryL(), returning %d.", result );
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DeleteMessageByPositionL
// Deletes message by its position in the internal cache.
// Note that after the call the internal cache won't contain 
// the item.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DeleteMessageByPositionL( 
    TInt aIndex )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::DeleteMessageByPositionL()");
    
    // Delete from cache and from file.
    __ASSERT_DEBUG( iCacheValid, CbsServerPanic( ECbsCacheNotValid ) );

    TCbsDbImpTopicMessagesCacheItem item( iMessageList->At( aIndex ) );

    // Determine the file where the message is stored.
    CFileStore* store = item.iIsSaved ? iTopicList.TopicStoreL() :
        iTopicList.UnsavedMessagesStore();

    iMessageList->Delete( aIndex );

    store->Delete( item.iId );
    store->Delete( item.iContentsId );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::DeleteMessageByPositionL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::IsMessageLocked
// Determines whether or not the message is locked.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsDbImpTopicMessages::IsMessageLocked( 
    const TCbsDbMessageHandle& aHandle ) const
    {
    // Find out if the message is locked.
    TBool found( EFalse );
    TInt count( iLockedMessages->Count() );
    for ( TInt index( 0 ); ( index < count ) && !found; index++ )
        {
        if ( iLockedMessages->At( index ) == aHandle )
            {
            found = ETrue;            
            }
        }

    return found;    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::DeleteFromLocked
// Deletes from the locked messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::DeleteFromLocked( 
    const TCbsDbMessageHandle& aHandle )
    {
    TBool deleted( EFalse );
    TInt count( iLockedMessages->Count() );

    for ( TInt index( 0 ); ( index < count ) && !deleted; index++ )
        {
        if ( iLockedMessages->At( index ) == aHandle )
            {
            iLockedMessages->Delete( index );
            deleted = ETrue;            
            }
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::InsertToLockedL
// Adds a handle to the locked messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::InsertToLockedL( 
    const TCbsDbMessageHandle& aHandle )
    {
    // Just append to the end. 
    iLockedMessages->AppendL( aHandle );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL
// Updates topic messages stream from the internal cache.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(
    TBool aDeleting )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL()");
    
    __ASSERT_DEBUG( iCacheValid, CbsServerPanic( ECbsCacheNotValid ) );

    TInt neededSpace( KMessageRootStreamSize + 
        iMessageList->Count() * KMessageEntrySize );

    RSharedDataClient sharedData;
    // About to write to FFS: make critical level check
    if ( aDeleting )
        {
        User::LeaveIfError( sharedData.Connect() );
        sharedData.RequestFreeDiskSpaceLC( neededSpace ); // on CS
        }
    else
        {
        CbsUtils::FFSCriticalLevelCheckL( neededSpace, iFs );
        }

    // Now, first check if the information fits well to the current stream.
    if ( iSpaceForMessages >= iMessageList->Count() )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (3)");
        
        // Write data to the old stream.
        TStreamId id;
        iTopicList.GetTopicMessagesIdL( iCachedTopicNumber, id );
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (4)");
        
        // Replace the existing stream.
        RStoreWriteStream outstream;
        outstream.ReplaceLC( *iTopicList.TopicStoreL(), id ); // on CS
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (5)");
    
        WriteTopicMessagesStreamL( outstream );
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (6)");

        outstream.CommitL();
        CleanupStack::PopAndDestroy(); // outstream
        }
    else
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (7)");
        // Calculate the amount of space for messages.
        if ( iSpaceForMessages == 0 )
            {
            // Zero is not very good, lets put it greater.
            iSpaceForMessages = KDefaultSpaceForMessages;
            }

        // But no need for extra messages.
        iSpaceForMessages = 2 * iSpaceForMessages;
        if ( iSpaceForMessages > 
             ( KCbsDbMaxReceivedMessages + KCbsDbMaxSavedMessages ) )
            {
            iSpaceForMessages = 
                KCbsDbMaxReceivedMessages + KCbsDbMaxSavedMessages;
            }
        if ( iMessageList->Count() > iSpaceForMessages )
            {
            iSpaceForMessages = iMessageList->Count();
            }

        // And finally write the data to stream.
        RStoreWriteStream outstream;
        TStreamId id = outstream.CreateLC( *iTopicList.TopicStoreL() );// on CS
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (8)");

        WriteTopicMessagesStreamL( outstream );
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (9)");

        outstream.CommitL();
        CleanupStack::PopAndDestroy(); // outstream

        // And then update topic messages stream. It will also delete 
        // the old stream.
        iTopicList.UpdateTopicMessagesIdL( iCachedTopicNumber, id );
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (10)");
        }

    // Free the reserved space
    if ( aDeleting )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL(): (11)");
        CleanupStack::PopAndDestroy(); // disk space
        sharedData.Close();
        }

    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::UpdateTopicMessagesStreamL()"); 
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::WriteTopicMessagesStreamL
// Writes topic messages stream from the internal cache.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::WriteTopicMessagesStreamL( 
    RWriteStream& aOut )
    {
    // Write total amount of messages.
    TInt count( iMessageList->Count() );
    aOut.WriteInt16L( count );

    // Write total amount of space for messages.
    aOut.WriteInt16L( iSpaceForMessages );

    TInt index( 0 );

    for ( ; index < count; index++ )
        {
        CbsStreamHelper::WriteBoolL( aOut,
            iMessageList->At( index ).iIsSaved );

        // Write message stream id.
        aOut << iMessageList->At( index ).iId;
        }

    for ( ; index < iSpaceForMessages; index++ )
        {
        CbsStreamHelper::WriteBoolL( aOut, EFalse );

        // Write null message stream id.
        aOut << TStreamId( 0 );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::CreateMessageContentsStreamL
// Creates a new message content stream and writes 
// the given contents into this stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TStreamId CCbsDbImpTopicMessages::CreateMessageContentsStreamL(
    CStreamStore&  aStore, 
    const TDesC& aContents )
    {
    // Create stream and write data to it.
    RStoreWriteStream outstream;
    TStreamId id( outstream.CreateLC( aStore ) ); // on CS

    // Just write the contents..
    TInt count( aContents.Length() );
    outstream.WriteInt16L( count );
	outstream << aContents;

    outstream.CommitL();
    CleanupStack::PopAndDestroy(); // outstream

    return id;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::CreateMessageStreamL
// Creates a message stream and
// writes the message into that stream. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TStreamId CCbsDbImpTopicMessages::CreateMessageStreamL( 
    CStreamStore& aStore, 
    TCbsDbMessage& aMessage, 
    const TStreamId& aContents )
    {
    // Create stream and write data to it.
    RStoreWriteStream outstream;
    TStreamId id = outstream.CreateLC( aStore ); // on CS

    // Write the handle.
    outstream.WriteInt32L( aMessage.iHandle );

    // Write the message key.
    outstream.WriteInt16L( aMessage.iKey );
    
    // Write the language.
    outstream.WriteInt16L( aMessage.iLanguage );

    // Write the date and time.
    outstream << aMessage.iDateTime;
    
    // Write the permanent status.
    CbsStreamHelper::WriteBoolL( outstream, aMessage.iPermanent );
    
    // Write the read status.
    CbsStreamHelper::WriteBoolL( outstream, aMessage.iRead );
    
    // Write the stream identifier to the contents.
    outstream << aContents;

    outstream.CommitL();
    CleanupStack::PopAndDestroy(); // outstream

    return id;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL
// Reads all cache items for a topic.
// Restores topic message cache from a stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL( 
    const CStreamStore& aStore, 
    const TStreamId& aId, 
    TCbsDbImpTopicMessagesCache& aCache, 
    TInt& aSpace )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL()");
    
    RStoreReadStream instream;
    instream.OpenLC( aStore, aId ); // on CS
    CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL(): instream.OpenLC() OK.");

    // Read total amount of messages.
    TInt totalMessages( instream.ReadInt16L() );
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL(): totalMessages: %d.", totalMessages );

    // Read total amount of space for messages.
    aSpace = instream.ReadInt16L();
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL(): aSpace: %d.", aSpace );

    // Sanity check: if the number of messages exceeds
    // the worst case, leave. If the space for messages
    // is smaller than number of messages, leave.
    if ( totalMessages > KCbsDbMaxSavedMessages + KCbsDbMaxReceivedMessages 
        || aSpace < totalMessages )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL(): Stream corrupt, leaving with KErrCorrupt...");
        User::Leave( KErrCorrupt );
        }

    TCbsDbImpTopicMessagesCacheItem item;

    aCache.Reset();

    for ( TInt index( 0 ); index < totalMessages; index++ )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL(): Looping msgs, i: %d.", index );
        item.iIsSaved = CbsStreamHelper::ReadBoolL( instream );

        // Read topic message stream id.
        instream >> item.iId;

        // Initialize other fields.
        item.iIsMessage = EFalse;

        // And finally, append the item to the array
        aCache.AppendL( item );
        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL(): Msg appended to array.");
        }

    CleanupStack::PopAndDestroy(); // instream
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::ReadTopicMessagesCacheItemsL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::ReadMessageInformationL
// Restore a message from a stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::ReadMessageInformationL( 
    const CStreamStore& aStore, 
    const TStreamId& aId, 
    TCbsDbMessage& aMessage, 
    TStreamId& aContentsId )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicMessages::ReadMessageInformationL()");
    
    // Open the message information stream.
    RStoreReadStream instream;
    instream.OpenLC( aStore, aId ); // on CS

    // Read the handle.
    aMessage.iHandle = instream.ReadInt32L();

    // Read the message key.
    aMessage.iKey = instream.ReadInt16L();
    
    // Read the language.
    aMessage.iLanguage = instream.ReadInt16L();    
    
    // Read the date and time.
    TInt64 time;
    instream >> time;
    aMessage.iDateTime = time;
    
    // Read the permanent status.
    aMessage.iPermanent = CbsStreamHelper::ReadBoolL( instream );

    // Read the read status.
    aMessage.iRead = CbsStreamHelper::ReadBoolL( instream );
    
    // Read the stream identifier to the contents.
    instream >> aContentsId;

    CleanupStack::PopAndDestroy(); // instream

    if ( aContentsId == TStreamId( 0 ) )
        {
        aMessage.iLength = 0;
        }
    else
        {
        // Open message contents stream.
        RStoreReadStream instreamContents;

        instreamContents.OpenLC( aStore, aContentsId ); // on CS

        // Read the length of the message.
        aMessage.iLength = instreamContents.ReadInt16L();

        CleanupStack::PopAndDestroy(); // instreamContents
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicMessages::ReadMessageInformationL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::WriteMessageInformationL
// Writes message information into a stream. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::WriteMessageInformationL( 
    RWriteStream& aOut, 
    const TCbsDbMessage& aMessage, 
    const TStreamId& aContentsId )
    {    
    // Write the handle.
    aOut.WriteInt32L( aMessage.iHandle );
    
    // Write the message key.
    aOut.WriteInt16L( aMessage.iKey );
    
    // Write the language.
    aOut.WriteInt16L( aMessage.iLanguage );
        
    // Write the date and time.
    aOut << aMessage.iDateTime;
    
    // Write the permanent status.
    CbsStreamHelper::WriteBoolL( aOut, aMessage.iPermanent );
    
    // Write the read status.
    CbsStreamHelper::WriteBoolL( aOut, aMessage.iRead );
    
    // Write the stream identifier to the contents.
    aOut << aContentsId;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::UpdateSoftNotificationL
// Updates the soft notification (dialog) when unread message count changes.           
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::UpdateSoftNotificationL( 
    const TCbsDbMessageHandle& aHandle,
    const CCbsRecEtel& aReceiver )
    {
    TCbsDbTopic topic;
    TCbsDbTopicNumber topicNum( iTopicList.ExtractTopicNumber( aHandle ) );
    iTopicList.FindTopicByNumberL( topicNum, topic ); 
    
    // If topic is hotmarked, update the soft notification (dialog) ,
    // since number of read messages was changed
    if ( topic.iHotmarked )
        {            
        aReceiver.Interface().LaunchMessageSoftNotificationL( EFalse );
        } 
    }
    
// -----------------------------------------------------------------------------
// CCbsDbImpTopicMessages::RevertFileOperationL
// Reverts any not commited file operation in data files,
// reloads cache and then leaves with aReason.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicMessages::RevertFileOperationL( 
    TInt aReason )
    {
    iTopicList.RevertFilesL();
    InvalidateCache();
    User::Leave( aReason );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
