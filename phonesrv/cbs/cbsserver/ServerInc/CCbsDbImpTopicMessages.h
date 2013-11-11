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
* Description:  This file contains the header file of the CCbsDbImpTopicMessages class.
*    
*                This class represents the topic messages stored in the database.
*
*/



#ifndef CCBSDBIMPTOPICMESSAGES_H
#define CCBSDBIMPTOPICMESSAGES_H

// INCLUDES
#include <e32std.h>
#include <s32file.h>
#include "CbsCommon.h"

// FORWARD DECLARATIONS
class CCbsDbImp;
class CCbsDbImpTopicList;
class CCbsRecEtel;

//  CLASS DECLARATION 

/**
*   CCbsDbImpTopicMessages represents CB messages stored in persistent memory.
*   CBS server has two files to accommodate messages: one for unsaved 
*   and one for saved messages. "Topic messages"-stream contains information
*   on whether a message is saved or not, and what is message's stream id.
*   CCbsDbImpTopicMessages determines the correct data file depending on
*   message's save status.
*
*   Changes are written to stores immediately after operations. Message cache
*   iMessageList contains header information of all messages in a single topic
*   at once. A call to LoadCacheL() is thus made prior to any operation to
*   make sure that the appropriate topic is being used.
*/
class CCbsDbImpTopicMessages : public CBase
    {
    public:        // New functions
        /**
        *   Creates an instance of the class.
        *
        *   @param  aTopicList          Topic list database manager
        *   @param  aFs                 File server session
        *   @return                     Pointer to the created instance
        */
        static CCbsDbImpTopicMessages* NewL( CCbsDbImpTopicList& aTopicList,
            RFs& aFs );

        /**
        *   Destructor.
        */
        ~CCbsDbImpTopicMessages();

    public:
        /**
        *   Invalidates the cache.
        */
        void InvalidateCache();

        /**
        *   Invalidates the cache if the cache contains messages of the topic.
        *
        *   @param aNumber              Number of the topic that is 
        *                               invalidated.
        */
        void InvalidateCacheIfTopic( const TCbsDbTopicNumber& aNumber );

        /**
        *   Creates a default topic messages stream.
        *
        *   @param  aStore              Store to which the stream is created.
        *   @return                     Stream identifier to the created stream
        */
        static TStreamId CreateDefaultTopicMessagesStreamL( 
            CStreamStore& aStore );

        /**
        *   Delete all topics messages.
        *
        *   @param  aSavedStore         Store which contains saved messages.
        *   @param  aUnsavedStore       Store which contains unsaved messages.
        *   @param  aIn                 Stream for topic messages.
        */
        static void DeleteAllTopicMessagesL( CStreamStore& aSavedStore, 
            CStreamStore& aUnsavedStore, RReadStream& aIn );

        /**
        *   Determines whether or not there are any locked messages in 
        *   the topic.
        *
        *   @param  aNumber             Number of the topic.
        *   @return                     Boolean value indicating if there are 
        *                               locked messages in topic.
        */
        TBool IsLockedMessagesInTopic( const TCbsDbTopicNumber& aNumber ) 
            const;

        /**
        *   Determines whether there is at least one locked message.
        *
        *   @return                     ETrue, if there's at least one locked
        *                               message.
        */
        TBool IsLockedMessages() const;

        /**
        *   Returns the total amount of messages the topic contains.
        *
        *   @param aNumber              Number of the topic.
        *   @param aCount               Returns: number of messages in given
        *                               topic
        */
        void GetMessageCountL( const TCbsDbTopicNumber& aNumber, 
            TInt& aCount );

        /**
        *   Returns message information.
        *
        *   @param aNumber              Number of the topic.
        *   @param aIndex               Index to the message in topic.
        *   @param aMessage             Returns: message information.
        */
        void GetMessageL( const TCbsDbTopicNumber& aNumber, 
            TInt aIndex, TCbsDbMessage& aMessage );

        /**
        *   Finds the message for given handle and returns it.
        *
        *   @param aHandle              Handle to the message.
        *   @param aMessage             Returns: message information.
        */
        void FindMessageByHandleL( const TCbsDbMessageHandle& aHandle, 
            TCbsDbMessage& aMessage );

        /**
        *   Finds the message by message key.
        *
        *   @param  aNumber             Number of the topic.
        *   @param  aKey                Message key to be looked for.
        *   @return                     Handle to the message, or KErrNotFound if no 
        *                               such message was found.
        */
        TInt FindMessageByKeyL( TCbsDbTopicNumber aNumber, 
            TCbsDbMessageKey aKey,
            TCbsDbMessage& aMessage );

        /**
        *   Stores the contents of the message to the buffer aContents. 
        *   If aContents is too small to contain the whole message
        *   body, the message body is truncated.
        *
        *   @param aHandle              Handle to the message.
        *   @param aContents            Returns: message body
        *   @param aSize                Number of bytes to read
        */
        void GetMessageContentsL( const TCbsDbMessageHandle& aHandle, 
            TPtr& aContents, TUint aSize );

        /**
        *   Deletes a message from the topic.
        *
        *   @param aHandle      Handle to the message to be deleted.
        *   @param  aReceiver   Receiver.
        */
        void DeleteMessageL( const TCbsDbMessageHandle& aHandle,
            const CCbsRecEtel& aReceiver );
        
        /**
        *   Creates a new message to the topic.
        *
        *   FFS critical level check is made prior to operation.
        *
        *   @param aNumber              Number of the topic to which 
        *                               the message is to be added.
        *   @param aMessage             Contains the message information 
        *                               (i.e., header, not body).
        *   @param aContents            The contents of the message.
        */
        void AddMessageL( const TCbsDbTopicNumber& aNumber, 
            TCbsDbMessage& aMessage, const TPtrC& aContents );

        /**
        *   Saves the message.
        *
        *   FFS critical level check is made prior to the operation.
        *
        *   @param aHandle      Handle to the message to be saved.
        *   @param  aReceiver   Receiver.
        */
        void SaveMessageL( const TCbsDbMessageHandle& aHandle,
            const CCbsRecEtel& aReceiver );

        /**
        *   Sets the message as read.
        *
        *   @param aHandle      Handle to the message to be set read.
        *   @param  aReceiver   Receiver.
        */
        void ReadMessageL( const TCbsDbMessageHandle& aHandle,
            const CCbsRecEtel& aReceiver );

        /**
        *   Updates handles of messages in the topic with the number given in
        *   aOldTopicNumber to match new topic number.
        *
        *   @param  aOldTopicNumber     Number of topic to be changed
        *   @param  aNewTopicNumber     Change handles to match this number
        */
        void UpdateHandlesOfTopicMessagesL(
            const TCbsDbTopicNumber& aOldTopicNumber, 
            const TCbsDbTopicNumber& aNewTopicNumber );

        /**
        *   Unlocks one message and then locks another message.
        *
        *   Message that is locked can not be removed. Topic that 
        *   contains a locked message can not be removed.
        *
        *   Note that locking status is not persistent in the sense that 
        *   when the power is switched off and turned on, no messages 
        *   are automatically locked.
        *
        *   @param aUnlock       Handle to the message to be unlocked.
        *   @param aLock         Handle to the message to be locked.
        */
        void LockMessageL( const TCbsDbMessageHandle& aUnlock, 
            const TCbsDbMessageHandle& aLock );

        /**
        *   Retrieves handles of messages that precede and succeed the message
        *   with handle aCurrentMsg.
        */
        void GetNextAndPrevMsgHandleL(
            const TCbsDbMessageHandle& aCurrentMsg,
            TCbsDbMessageHandle& aNextMsg, 
            TCbsDbMessageHandle& aPrevMsg, 
            TInt& aPosition);
    
        /**
        *   Finds message position by message handle.
        *
        *   The method leaves in case the message is not found.
        *
        *   @param  aHandle             Handle to the message to be looked for.
        *   @return                     The position of the message.
        */
        TInt FindMessagePositionByHandleL( 
            const TCbsDbMessageHandle& aMessage );        

    private:

        // The structure that represents the topic messages cache item.
        struct TCbsDbImpTopicMessagesCacheItem
            {   
            // The stream identifier for the message information.
            TStreamId iId;

            // Whether this message is saved or not
            TBool iIsSaved;

            // Indicates whether iMessage and iContentsId are valid.
            TBool iIsMessage;

            // Message header - valid only if iIsMessage is true.
            TCbsDbMessage iMessage;

            // Stream identifier for message body-valid if iIsMessage is ETrue
            TStreamId iContentsId;
            };

        typedef CArrayFixFlat< TCbsDbImpTopicMessagesCacheItem > 
            TCbsDbImpTopicMessagesCache;

        /**
        *   Constructor.
        *
        *   @param aTopicList           Topic list database object
        *   @param aFs                  File server session
        */
        CCbsDbImpTopicMessages( CCbsDbImpTopicList& aTopicList, RFs& aFs );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

        /**
        *   Adds a message.
        *
        *   @param aMessage             Contains the message information.
        *   @param aContents            Contains the message contents.
        */
        void DoAddMessageL( TCbsDbMessage& aMessage, const TDesC& aContents );

        /**
        *   Deletes a message.
        *
        *   @param aPosition            Position in the internal array where 
        *                               the message is.
        */
        void DoDeleteMessageL( TInt aPosition );

        /**
        *   Updates message information.
        *
        *   @param aId                  Stream id to which the information is
        *                               written.
        *   @param aMessage             Contains the message information.
        *   @param aContentsId          Contains the contents id.
        */
        void DoUpdateMessageL( const TStreamId& aId, 
            const TCbsDbMessage& aMessage, const TStreamId& aContentsId ) const;

        /**
        *   Sets the message read.
        *
        *   @param  aId                 Stream id to which the information is 
        *                               written.
        *   @param  aMessage            Contains the message information.
        *   @param  aContentsId         Stream idenfitier to the contents.
        */
        void DoReadMessageL( const TStreamId& aId, const TCbsDbMessage& aMessage, 
            const TStreamId& aContentsId );

        /**
        *   Deletes messages that are not supposed to be in the topic.
        *
        *   Because of message locking it is possible that there are more 
        *   received messages than there should be. This method tries to 
        *   delete this kind of message.
        */
        void DoDeleteL();

        /**
        *   Loads the cache, if it is not already loaded with this topic 
        *   information.
        *
        *   The method does not load any message information to the cache 
        *   - only what is really necessary.
        *
        *   @param aNumber              Number of the topic to be loaded 
        *                               to the cache.
        */
        void LoadCacheL( const TCbsDbTopicNumber& aNumber );

        /**
        *   Loads the message header to the cache.
        *
        *   Note that it is assumed that the index is in proper range! 
        *   And the cache is already loaded!
        *
        *   @param aIndex               The index to the message in cache.
        *   @param aMessage             Returns: message information.
        */
        void LoadMessageL( TInt aIndex, TCbsDbMessage& aMessage );

        /**
        *   Loads message contents (as much as it fits).
        *
        *   Save status of message is required because saved and unsaved
        *   messages are stored on separate files.
        *
        *   @param aContentsId          Stream identifier to the content.
        *   @param aContents            Returns: message body.
        *   @param aIsSaved             ETrue, if the message is saved.
		*	@param aSize				Size of the message.
        */
        void LoadMessageContentsL( const TStreamId& aContentsId, TPtr& aContents, 
            TBool aIsSaved, TUint aSize ) const;

        /**
        *   Generates a new message handle.
        *
        *   @param  aNumber             Number of the topic which will contain 
        *                               the message.
        *   @return                     Returns the new message handle.
        */
        TCbsDbMessageHandle GenerateNewMessageHandle( 
            const TCbsDbTopicNumber& aNumber );

        /**
        *   Checks that if there are too many received messages in 
        *   the internal cache, then they will be deleted if they
        *   are not locked.
        *
        *   @return                     Boolean value indicating if some 
        *                               changes was made.
        */
        TBool DeleteReceivedIfNecessaryL();

        /**
        *   Deletes message by its position in the internal cache.
        *
        *   Note that after the call the internal cache won't contain 
        *   the item.
        *
        *   @param aPosition            The position to the message.
        */
        void DeleteMessageByPositionL( TInt index );

        /**
        *   Determines whether or not the message is locked.
        *
        *   @param  aHandle             The handle to the message.
        *   @return                     Boolean value indicating if 
        *                               the message is locked.
        */
        TBool IsMessageLocked( const TCbsDbMessageHandle& aHandle ) const;

        /**
        *   Deletes from the locked messages.
        *
        *   @param aHandle              Message to be unlocked.
        */
        void DeleteFromLocked( const TCbsDbMessageHandle& aHandle );

        /**
        *   Adds a handle to the locked messages.
        *
        *   @param aHandle              Message to be locked.
        */
        void InsertToLockedL( const TCbsDbMessageHandle& aHandle );

        /**
        *   Updates topic messages stream from the internal cache.
        *
        *   @param aDeleting            Whether the call is due to a deletion.
        */
        void UpdateTopicMessagesStreamL( TBool aDeleting );

        /**
        *   Writes topic messages stream from the internal cache.
        *
        *   @param aOut                 Stream to which data is written.
        */
        void WriteTopicMessagesStreamL( RWriteStream& aOut );

        /**
        *   Saves the message.
        *
        *   @param aMessage             The message to be saved.
        *   @param aItem                The cache item to the previous instance of the
        *                               message.
        *   @param aContentPtr          The contents of the message.
        *   @param aPosition            The position of the message in the cache.
        */
        void DoSaveMessageL( 
            TCbsDbMessage& aMessage,
            const TCbsDbImpTopicMessagesCacheItem& aItem,
            const TDesC& aContentPtr,
            TInt aPosition );

        /**
        *   Reverts any not commited file operation in data files,
        *   reloads cache and then leaves with aReason.
        *
        *   @param  aReason             Leave code
        */
        void RevertFileOperationL( TInt aReason );

        /**
        *   Creates a new message content stream.
        *
        *   @param aStore               Store to which the stream is created.
        *   @param aContents            Message content.
        */
        static TStreamId CreateMessageContentsStreamL( CStreamStore& aStore,
            const TDesC& aContents );
        
        /**
        *   Creates a new message stream.
        *
        *   @param aStore               Store to which the stream is created.
        *   @param aMessage             Message information.
        *   @param aContents            Stream identifier to the contents.
        */
        static TStreamId CreateMessageStreamL( CStreamStore& aStore, 
            TCbsDbMessage& aMessage, const TStreamId& aContents );

        /**
        *   Reads all cache items for a topic.
        *
        *   @param aStore               Reference to the store.
        *   @param aId                  Stream id to the topic messages stream.
        *   @param aCache               Empty cache where it is supposed that 
        *                               these messages are put.
        *   @param aSpace               Contain the amount of space for 
        *                               messages.
        */
        static void ReadTopicMessagesCacheItemsL( const CStreamStore& aStore, 
            const TStreamId& aId, 
            TCbsDbImpTopicMessagesCache& aCache, 
            TInt& aSpace );

        /**
        *   Reads necessary message information from the store.
        *
        *   @param aStore               Store that contains the information.
        *   @param aId                  Stream identifier to the message 
        *                               stream.
        *   @param aMessage             Contain message information.
        *   @param aContentsId          Contain the stream identifier to 
        *                               the contents.
        */
        static void ReadMessageInformationL( 
            const CStreamStore& aStore, 
            const TStreamId& aId, 
            TCbsDbMessage& aMessage, 
            TStreamId& aContentsId );

        /**
        *   Writes message information to the store.
        *
        *   @param aOut                 Stream to which data is written.
        *   @param aMessage             Contains the message information to be
        *                               written.
        *   @param aContentsId          Stream identifier to the contents.
        */
        static void WriteMessageInformationL( RWriteStream& aOut, 
            const TCbsDbMessage& aMessage, const TStreamId& aContentsId );

        /**
        *   Updates the soft notification (dialog) when unread message
        *   count changes.
        *
        *   @param aHandle      Handle to the message to be set as read
        *   @param  aReceiver   Receiver.
        */            
        void UpdateSoftNotificationL( const TCbsDbMessageHandle& aHandle,
           const CCbsRecEtel& aReceiver );

    private:

        // Topic list manager.
        CCbsDbImpTopicList& iTopicList;

        // Indicates whether the cache is valid or not.
        TBool iCacheValid;

        // If cache is valid, then it contains the topic handle of the 
        // cached topic.        
        TCbsDbTopicNumber iCachedTopicNumber;

        // If cache is valid, then it tells how many messages the stream 
        // can hold.        
        TInt iSpaceForMessages;
       
        // Own: If cache is valid, it contains some information of the 
        // topic messages for fast access.        
        TCbsDbImpTopicMessagesCache* iMessageList;

        // Own: handles of the locked messages.
        CArrayFixFlat< TCbsDbMessageHandle >* iLockedMessages;

        // Reference to a file server session of CCbsDbImp
        RFs& iFs;

    };

#endif      //  CCBSDBIMPTOPICMESSAGES_H   
            
// End of File
