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
* Description:  This file contains the header file of the CCbsDmImpTopicList class
*    
*                This class represents the topic list stored in the database.
*
*/


#ifndef     CCBSDBIMPTOPICLIST_H
#define     CCBSDBIMPTOPICLIST_H

// INCLUDES
#include <f32file.h>

#ifndef __SECURE_BACKUP__
#include <babackup.h>
#endif

#include "CbsCommon.h"
#include "CCbsDbImp.H"

// FORWARD DECLARATIONS
//class CCbsDbImp;
class CCbsDbImpTopicMessages;
class MCbsDbTopicListObserver;
class CFileStore;
//  CLASS DECLARATION 

/**
*   CCbsDbImpTopicList is the database topic list manager.
*   
*/
class CCbsDbImpTopicList : public CBase
#ifndef __SECURE_BACKUP__
, public MBackupObserver
#endif
    {
    #ifndef __SECURE_BACKUP__
    public: // MBackupObserver
        /**
        *   Closes or reopens the settings file if requested by a backup.
        *
        *   @param aFileName                The name of the file to close.
        *   @param aFlags                   The action to be performed.
        *
        */
        void ChangeFileLockL( const TDesC& aFileName, TFileLockFlags aFlags );
    #else
    
    public:
        /**
        *   Closes or reopens the settings file if requested by a backup.
        *
        *   @param aRequest                 Current request        
        *
        */
        void ChangeFileLockL( const TCbsBackupRequest& aRequest );
        
    #endif

    private: 
        /**
        *   Check if the server has a file lock. Leaves with
        *   KErrLocked if not.
        */
        void CheckFileLockL() const;

    public:     // New functions
        
        /**
        *   Creates an instance of the class.
        *
        *   @param aFs                      Open file server session.
        *   @param aTopicsFile              Name of the file containing topics.
        *   @param aUnsavedMessagesFile     Name of the file containing unsaved
        *                                   messages.
        *   @param aDatabase                Reference to database object
        *   @return                         Pointer to the created instance.
        */
        static CCbsDbImpTopicList* NewL( RFs& aFs, 
            const TDesC& aTopicsFile, 
            const TDesC& aUnsavedMessagesFile,
            CCbsDbImp& aDatabase );

        /**
        *   Destructor.
        */
        ~CCbsDbImpTopicList();

    public:

        /**
        *   Creates a new topic list
		*
        *	@param	aTopicListName		Name of the new topic list
		*	@param	aTopicListIndex		Returns: Index number of the new list
        */
        void CreateNewTopicListL( const TDesC& aTopicListName );


		/**
        *	Deletes old read messages. The clean up interval is set
		*	by the user.
		*
        */
        void CleanupReadMessagesL();        

        /**
        *   Sets the topic messages manager.
        *
        *   Note that this function does not transfer ownership
        *   of aMessages.
        *
        *   @param aMessages            Pointer to the topic messages 
        *                               manager.
        */
        void SetTopicMessages( CCbsDbImpTopicMessages* aMessages );

        /**
        *   Returns pointer to the current store which contains topics
        *   of the server (i.e., Topics-file) and saved messages. 
        *   Leaves with KErrLocked if the store is locked by backup.
        *
        *   @return                     Pointer to topics store.
        */
        CFileStore* TopicStoreL() const;

        /**
        *   Returns a pointer to the name of the file, which contains unsaved
        *   messages of the server.
        *
        *   @return                     Reference to the file name.
        */
        const TDesC& TopicFilename() const;

        /**
        *   Returns a pointer to the store, which contains unsaved
        *   messages of the server.
        *
        *   @return                     Pointer to unsaved messages store.
        */
        CFileStore* UnsavedMessagesStore() const;

        /**
        *   Returns a pointer to the name of the file, which contains unsaved
        *   messages of the server (Unsaved Messages-file, "cbs4.dat").
        *
        *   @return                     Reference to the file name.
        */
        const TDesC& UnsavedMessagesFilename() const;

        /**
        *   Returns the topic messages stream id by topic handle.
        *
        *   @param  aNumber             Topic number
        *   @param  aId                 Returns: the stream id to the topic
        *                               messages stream.
        */
        void GetTopicMessagesIdL( TCbsDbTopicNumber aNumber,
            TStreamId& aId ) const;

        /**
        *   Updates the topic messages stream id by topic handle. The new 
        *   identifier is expected not to be a null id.
        *
        *   Note that the method will not commit changes to the store. 
        *   It also changes the internal state and thus if the method leaves, 
        *   it is good to reload the whole root stream.
        *
        *   The method will delete the old topic messages stream.
        *
        *   @param aNumber              Topic number
        *   @param aNewId               New stream id.
        */
        void UpdateTopicMessagesIdL( TCbsDbTopicNumber aNumber, 
            const TStreamId& aNewId );

        /**
        *   Generates a message handle.
        *
        *   Note that it must be checked that the message handle is unique 
        *   in that topic.
        *
        *   @param  aNumber             Topic number
        *   @param  aRandom             Number generated in some way.
        *   @return                     New message handle.
        */
        TCbsDbMessageHandle GenerateMessageHandle( 
            const TCbsDbTopicNumber aNumber, TUint16 aRandom ) const;

        /**
        *   Informs that a new message has been received in a topic.
        *
        *   @param  aMessageHandle      Handle of the received message.
        */
        void InformNewMessageReceivedL(
            const TCbsDbMessageHandle& aMessageHandle  );

        /**
        *   Informs that a message has been set as saved.
        *
        *   @param  aMessageHandle      Handle of the saved message.
        */
        void InformMessageSavedL( const TCbsDbMessageHandle& aMessageHandle );

        /**
        *   Informs that an unread message has been read.
        *
        *   @param  aMessageHandle      Handle of the read message.
        */
        void InformUnreadMessageReadL( 
            const TCbsDbMessageHandle& aMessageHandle );

        /**
        *   Informs that a message has been deleted.
        *
        *   @param  aMessageHandle      Handle of the deleted message.
        *   @param  aPermanent          ETrue, if the message was permanent.
        *   @param  aRead               ETrue, if the message was read.
        */
        void InformMessageDeletedL( 
            const TCbsDbMessageHandle& aMessageHandle, 
            TBool aPermanent, TBool aRead );

        /**
        *   Extracts topic handle from message handle.
        *
        *   Note that the method does not check that the message exists.
        *
        *   @param  aHandle             Handle to a message.
        *   @return                     Returns the number of the topic to 
        *                               which the message belongs.
        */
        TCbsDbTopicNumber ExtractTopicNumber( 
            const TCbsDbMessageHandle& aHandle ) const;
        
        /**
        *   Initialises the whole topic list. 
        */
        void InitializeListL( const TBool aFileOpenFailed );

        /**
        *   Returns the total amount of saved messages.
        *
        *   @return                     Number of saved messages.
        */
        TInt TotalSavedMessages() const;

        /**
        *   Returns the amount of topics the list contains.
        *
        *   @param  aCount              Returns: number of topics in topic list
        */
        void GetTopicCount( TInt& aCount ) const;
    
        /**
        *   Returns a topic by index.
        *
        *   Return values:
        *   KErrNone                    Parameter aTopic contains the requested
        *                               topic.
        *   KErrArgument                Index out of bounds.
        *
        *   Leaves if file read fails.
        *
        *   @param  aIndex              Index to the topic.
        *   @param  aTopic              Returns: topic information.
        */
        void GetTopicL( TInt aIndex, TCbsDbTopic& aTopic );

        /**
        *   Returns a topic by topic number.
        *
        *   Leaves with KErrNotFound, if a topic with the given number
        *   is not found.
        *
        *   @param    aNumber           Number of the topic.
        *   @param    aTopic            Returns: topic information.
        */
        void FindTopicByNumberL( TCbsDbTopicNumber aNumber, 
            TCbsDbTopic& aTopic );

        /**
        *   Reads the name of the index topic from resource.
        *
        *   @return     Name of the index topic
        */
        HBufC* ReadIndexTopicNameLC();

        /**
        *   Adds a new topic to the list.
        *
        *   @param    aTopic            Topic information.
        *   @param    aDetected         Was the topic detected automatically
        */
        void AddTopicL( TCbsDbTopic& aTopic, const TBool aDetected );

        /**
        *   Updates the topic name and number.
        *
        *   @param    aOldNumber        Old number of the topic.
        *   @param    aNewNumber        New number of the topic.
        *   @param    aName             New name of the topic.
        */
        void UpdateTopicNameAndNumberL( 
            TCbsDbTopicNumber aOldNumber, 
            TCbsDbTopicNumber aNewNumber, 
            const TCbsDbTopicName& aName );

        /**
        *   Updates the topic subscription status.
        *
        *   @param    aNumber           Topic number
        *   @param    aStatus           New subscription status.
        */
        void UpdateTopicSubscriptionStatusL( 
            TCbsDbTopicNumber aNumber, 
            TBool aStatus );

        /**
        *   Updates the topic hotmark status.
        *
        *   @param    aNumber           Topic number
        *   @param    aStatus           New hotmark status.
        */
        void UpdateTopicHotmarkStatusL( 
            TCbsDbTopicNumber aNumber, 
            TBool aStatus );

        /**
        *   Deletes an existing topic and all its messages.
        *
        *   @param    aNumber           Number of the topic to be deleted.
        */
        void DeleteTopicL( TCbsDbTopicNumber aNumber );

        /**
        *   Gets the total amount of unread messages.
        *
        *   @param  aCount              Returns: number of unread topics. 
        */
        void GetUnreadMessageCount( TInt& aCount ) const;

        /**
        *   Returns the handle to the latest hotmarked message. 
        *   
        *   The method leaves if there is no such message.
        *
        *   @param  aMessage            Returns: Handle to a message that has         
        *                               been  received under a hotmarked topic
        */
        void GetHotmarkedMessageHandleL( TCbsDbMessageHandle& aMessage );

        /**
        *   Adds a topic list observer. 
        *   
        *   After an observer is added to the topic list, it will be notified 
        *   whenever an event occurs.
        *
        *   @param  aObserver           New observer for the list.
        */
        void AddObserverL( MCbsDbTopicListObserver* aObserver );

        /**
        *   Removes a topic list observer. 
        *   
        *   If aObserver is not in the list, the method will panic.
        *
        *   @param  aObserver           Pointer to the observer to be removed.
        */
        void RemoveObserver( const MCbsDbTopicListObserver* aObserver );

        /**
        *   Adds numbers of subscribed topics to the given array.
        *
        *   @param aSubscriptions       Dynamic array into which the topic
        *                               numbers are inserted.
        */
        void AppendSubscribedTopicsL( CArrayFixFlat< TUint16 >& aSubscriptions )
            const;

        /**
        *   Reverts all changes made to three datafiles handled by this
        *   class.
        */
        void RevertFilesL();

        /**
        *   Commits all changes made to three datafiles handled by this
        *   class.
        */
        void CommitFilesL();

        /**
        *   Returns the number of the topic that was added last
        *   to the database by topic detection feature.
        *
        *   Result code KErrNotFound indicates that no topic has ever
        *   been added to the database.
        *
        *   @param aNumber              Returns: topic number
        *   @return                     Result code
        */
        TInt GetLatestTopicNumber( TCbsTopicNumber& aNumber ) const;

        /**
        *   Finds the index of the topic matching the given topic number
        *   in the topic list.
        *
        *   @param aNumber              Topic number
        *   @return                     Index to the position of topic in 
        *                               the topic list.
        */
        TInt TopicIndexInList( TCbsDbTopicNumber aNumber ) const;

        /**
        *   Returns the number of unread messages in hotmarked topics.
        */
        TInt UnreadHotmarkedMessageCount() const;

        /**
        *   Retrieves numbers of topics that precede and succeed the topic
        *   with number aCurrentTopic.
        *
        *   @param  aCurrentTopic   Topic number of current topic
        *   @param  aNextTopic      Topic number of next topic
        *   @param  aPrevTopic      Topic number of previous topic
        *   @param  aPosition       Position of topic in the topic list
        */
        void GetNextAndPrevTopicNumberL( const TCbsTopicNumber& aCurrentTopic,
            TCbsTopicNumber& aNextTopic, TCbsTopicNumber& aPrevTopic,
            TInt& aPosition );

        /**
        *   Notifies each observer that a new message has arrived to a topic.
        *
        *   @param    aHandle                    Handle to the topic.
        */
        void NotifyNewMessageArrivedL( const TCbsDbMessageHandle& aHandle );

        /**
        *   Notifies each observer that a topic has been modified.
        *
        *   @param    aNumber                    Topic number
        */
        void NotifyTopicModifiedL( TCbsDbTopicNumber aNumber );

        /**
        *   Loads the topic stream to the memory.
        *
        */
        void LoadDefaultTopicStreamL();

    private:                

        /**
        *   Constructor.
        *
        *   @param aFs                  Open file server session.
        *   @param aDatabase            Reference to database object
        */
        CCbsDbImpTopicList( RFs& aFs, CCbsDbImp& aDatabase );

        /**
        *   Returns the total amount of topics the list contains.
        *
        *   @return                     The total amount of topics in the list.
        */
        TInt TopicCount() const;

		/**
        *   Returns the total amount of topic lists.
        *
        *   @return                     The total amount of topic lists.
        */
        TInt TopicListCount() const;

        /**    
        *   Finalizes the construction.
        *
        *   @param aTopicFile           File name of the topic file
        *   @param aUnsavedMessagesFile File name of the unsaved messages file.
        */
        void ConstructL( const TDesC& aTopicsFile, 
            const TDesC& aUnsavedMessagesFile );

        /**
        *   Loads the root stream to the memory.
        *
        *   Root stream contains stream ids for each topic list and the total
        *   count of topic lists.        
        */
        void LoadRootStreamL();

        /**
        *   Reloads the root stream to the memory.
        */
        void ReloadRootStreamL();
       
		/**
        *   Reads all information on topic list found with stream ID.        
        *
        *   @param aId                  Stream id to the topic information
        *   @param aTopicList           Returns: topic list information
        */
		void ReadTopicListInformationL(const TStreamId& aId, 
			TCbsDbImpTopicList& aTopicList ) const;

        /**
        *   Reads all information on topic found in stream aId
        *   into aTopic.
        *
        *   This function is called when details of a topic are needed.
        *
        *   @param aId                  Stream id to the topic information
        *   @param aTopic               Returns: topic information.
        */
        void ReadTopicInformationL( const TStreamId& aId, TCbsDbImpTopic& aTopic ) const;

        /**
        *   Adds a topic to the database.
        *
        *   @param aTopic               Information on added topic.
        */
        void DoAddTopicL( const TCbsDbTopic& aTopic );

        /**
        *   Updates a topic information.
        *
        *   @param aTopic               Contains topic information.
        *   @param aNeedToChange        Indicates whether or not it is required
        *                               to change position in the array
        *   @param aOldPosition         Old position.
        *   @param aDeleting            Whether the call is due to a deletion.
        */
        void DoUpdateTopicL( const TCbsDbTopic& aTopic, 
            TBool aNeedToChange, TInt aOldPosition, TBool aDeleting );

        /**
        *   Deletes an existing topic.
        *
        *   @param aPosition            Position of the topic to be deleted.
        */
        void DoDeleteTopicL( TInt aPosition );

        /**
        *   Updates a topic list information.
        *
        *   @param aTopicList       Contains topic list information.
        *   @param aDeleting        Info if we are deleting an entry.
        */
        void UpdateTopicListStreamL( TCbsDbImpTopicList& aTopicList, 
            TBool aDeleting );

        /**
        *   Updates the root stream.
        *   If required, it will create a new stream.
        *
        *   @param aDeleting        Info if we are deleting an entry.
        */
        void UpdateRootStreamL( TBool aDeleting );

        /**
        *   Updates the topic information.
        *
        *   @param aTopic               Updated topic information.
        */
        void UpdateTopicL( TCbsDbTopicNumber aTopicNumber, 
            TCbsDbTopic& aTopic );

        /**
        *   Checks that the topic number is valid. 
        *
        *   @param  aNumber             The topic number.
        *   @return                     Boolean value indicating if number 
        *                               is ok.
        */
        TBool CheckTopicNumber( TCbsDbTopicNumber aNumber ) const;

        /**
        *   Write the root stream.
        *
        *   @param    aOut              The stream to which the data is 
        *                               written.
        */
        void WriteRootStreamL( RWriteStream& aOut ) const;

        /**
        *   Write the Topic list stream
        *
        *   @param    aOut              The stream to which the data is 
        *                               written.
        */
        void WriteTopicListStreamL( RWriteStream& aOut, TCbsDbImpTopicList& aTopicList ) const;

        /**
        *   Notifies each observer that the topic list has been initialized.
        */
        void NotifyTopicListInitializedL();

        /**
        *   Notifies each observer that a topic has been added.
        *
        *   @param    aNumber                    Topic number
        */
        void NotifyTopicAddedL( TCbsDbTopicNumber aNumber );

        /**
        *   Notifies each observer that a topic has been deleted.
        *
        *   @param    aNumber                    Topic number
        */
        void NotifyTopicDeletedL( TCbsDbTopicNumber aNumber );

        /**
        *   Sets the hotmarked message handle.
        *
        *   @param    aMessageHandle             Handle to the message.
        */
        void SetHotmarkedMessage( const TCbsDbMessageHandle& aMessageHandle );

        /**
        *   Creates a default root stream.
        *
        *   The method may leave, if the creation or writing fails.
        *
        *   @param    aStore            The stream to to which the root
        *                               stream is written.
        *   @return                     Returns the stream identifier to 
        *                               that created stream.
        */
        TStreamId CreateDefaultRootStreamL( CStreamStore& aStore ) const;

        /**
        *   Writes topic information to the stream.
        *
        *   @param    aOutstream        The stream to which the data is written.
        *   @param    aTopic            Topic information.
        *   @param    aTopicMessagesId  The stream identifier to the 
        *                               topic messages.
        */
        void WriteTopicInformationL( RWriteStream& aOutstream, 
            const TCbsDbTopic& aTopic, const TStreamId& aTopicMessagesId ) const;

        /**
        *   Extracts topic information (name, number, subscription and
        *   hotmark status) from a factory defaults topics line
        *   and adds that topic to the topic list.
        *
        *   @param aLine                Factory defaults line.
        */
        void AddDefaultTopicL( const TDesC& aLine );

        /**
        *   After a call to this function, the file stores can be assumed
        *   to be open and initialized.
        *
        *   @param aDeleteExistingFiles     A flag whether to recreate and 
        *                                   initialize the files or not
        *   @param  aCreateNewTopicList     ETrue, if the first topic list
        *                                   needs to be created
        */
        void OpenFilesL( TBool aDeleteExistingFiles, TBool aCreateNewTopicList );

        /**
        *   Creates CBS files, if appropriate.
        *
        *   @param  aUnsavedMsgFileExisted  Returns ETrue if unsaved messages
        *                                   file already existed.
        */
        void CreateFilesIfNecessaryL( TBool& aUnsavedMsgFileExisted );

        /**
        *   Tries to open topic and unsaved messages files.
        *
        *   @param  aDeleteUnsavedMsgStreamIds  ETrue, if unsaved msg file 
        *                                       has been deleted
        *   @param  aCreateNewTopicList         ETrue, if the first topic list
        *                                       needs to be created
        */
        void TryToOpenFilesL( TBool aDeleteUnsavedMsgStreamIds, TBool aCreateNewTopicList );

        /**
        *   Adds index topic to topic list.
        */
        void AddIndexTopicL();

        /**
        *   Creates an empty file store with the given filename.
        *
        *   @param aFilename            Filename to be created.
        */
        void DoCreateStoreL( const TDesC& aFilename );

        /**
        *   Deletes the unsaved messages' store and rebuilds it.
        */
        void RebuildUnsavedMessageStoreL();

        /**
        *   Deletes the topics' and unsaved messages' store and rebuilds them.
        */
        void RebuildTopicAndUnsavedStoresL();

        /**
        *  Called to repair the database when Unsaved messages -file
        *  has been deleted, but Topics-file still contains information
        *  on unsaved messages.
        * 
        *  Things that are done here: 
        *       - Reset unread message counters, because a saved message
        *         is always also read.
        *       - Remove stream ids to unsaved messages from 
        *         Topic messages -streams.
        */
        void HandleDeletionOfUnsavedMessagesFileL();

        /**
        *   Called by HandleDeletionOfUnsavedMessagesFileL().
        *   Executes the necessary file operations.
        */
        void DoHandleDeletionOfUnsavedMessagesFileL();

        /**
        *   Replaces the given Topic message -stream with a stream which
        *   contains all ids of saved messages but no ids of unsaved messages.
        *
        *   @param  aMsgStreamId        Topic messages -stream id
        */
        void DeleteUnsavedMessageStreamIdsL( const TStreamId& aMsgStreamId ) const;

        /**
        *   Resolves the topic position in topic list and uses this information
        *   to call DoUpdateTopicL. Changes to stores are NOT commited.
        *   Call to DoUpdateTopicL is not trapped.
        *
        *   @param  aTopic              Topic to be written into stream.
        *   @param  aDeleting           Whether the call is due to a deletion.
        */
        void UpdateTopicCountersL( const TCbsDbTopic& aTopic,
            const TBool aDeleting );

        /**
        *   Creates a standard topic list, which has it's topics and other settings
        *   stored in Shared Data.
        *
        */
        void CreateStandardTopicListL();

        /**
        *   Load the topic IDs belonging to a specified topic list
        *
        *   @param aTopicListStreamId   Stream ID of the topic list
        */
        void LoadTopicsIdsL( const TStreamId& aTopicListStreamId );

        /**
        *   Load the topics specified by the topic list stream ID
        *
        *   @param aTopicListStreamId   Stream ID of the topic list
        */
        void LoadTopicsL( const TStreamId& aTopicListStreamId );

        /**
        *   Updates the topic stream IDs in topic lists
        *   
        *   @param  aTopicNumber    The topic number
        */
        void UpdateTopicStreamIdsL( const TCbsDbTopicNumber aTopicNumber );

    private:    // Data

        // Ref: Database
        CCbsDbImp& iDatabase;

        // Ref: Topic messages manager.
        CCbsDbImpTopicMessages* iMessages;
        
        // Session to the file server.
        RFs& iFs;
        
        // Own: Store which contains topics.
        CFileStore* iTopicStore;

		// Own: Store which contains unsaved messages.
        CFileStore* iUnsavedMessageStore;

        // The last time iTopicStore and iUnsavedMessageStore were compacted
        TTime iPreviousCompact;

        // Current topic list in use
        TCbsDbImpTopicList iCurrentTopicList;

        // Own: Cached topic information.
        CArrayFixFlat< TCbsDbImpTopic >* iTopics;

        // Own: Registered topic list observers.
        CArrayFixFlat< MCbsDbTopicListObserver* >* iObservers;

        // Last topic added to topic list with topic detection.
        TCbsTopicNumber iLastTopicNumber;

        // Indicates whether there is a hotmarked message
        TBool iIsHotmarkedMessage;

        // Handle to the hotmarked message.
        TCbsDbMessageHandle iMessageHandle;

        // Own: Buffer containing the filename of topics file
        HBufC* iTopicsFilename;

        // Own: Buffer containing the filename of unsaved messages file
        HBufC* iUnsavedMessagesFilename;

        // Own: Cached topic list information.
        CArrayFixFlat< TCbsDbImpTopicList >* iTopicLists;

        // Own: Cached topic IDs
        CArrayFixFlat< TStreamId >* iTopicIds;               

        // Global topic count
        TInt iTopicCount;

        // Are we initializing Standard Topic list
        TBool iInitializing; 

        // Are we deleting all topics from current topic list
        TBool iDeleteAllTopics;
        
        // Local variation bits
        TInt iLVBits;

        __DECLARE_TEST;
    };

#endif      // CCBSDBIMPTOPICLIST_H
            
// End of File


