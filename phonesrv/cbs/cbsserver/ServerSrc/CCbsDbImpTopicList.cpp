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
* Description:  This file contains the implementation of the CCbsDbImpTopicList class
*            member functions.
*    
*                This class represents the topic list contained in the database.
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <e32svr.h>
#include <f32file.h>

#include <shareddataclient.h>   // RequestFreeDiskSpaceLC

#include <barsc.h>              // Resource access
#include <barsread.h>           // Resource access

#include <cbsserver.rsg>

#include "CbsServerPanic.h"
#include "CbsStreamHelper.h"
#include "CbsDbConstants.h"
#include "CbsUtils.h"
#include "CCbsDbImpTopicList.h"
#include "MCbsDbTopicListObserver.H"
#include "CCbsDbImpTopicMessages.h"

#include "CbsLogger.h"

#include <centralrepository.h>  // for local variation
#include "cbsinternalcrkeys.h"  // for local variation
#include "cbsvariant.hrh"       // for local variation
 
// CONSTANTS

// Initial size for topic cache array
const TInt KDefaultTopicListSize = 10;

// Size of the topic stream, used in checking against FFS free space limit
const TInt KTopicStreamSize = 92;

// Size of the topic messages stream, FFS critical level check
const TInt KEmptyTopicMessagesStreamSize = 4;

// Size of topic list stream, FFS critical level check
const TInt KTopicListStreamSize = 85;

// Size of topic list stream (root), FFS critical level check
const TInt KTopicListRootStreamSize = 2;

// Time in microseconds to wait after a critical store exception 
// before a recovery attempt is made.
const TInt KWaitAfterCriticalStoreException = 2000000;

// Used when unsaved message stream ids are deleted from topic messages 
// stream
const TInt KTypicalNumberOfTopicMessages = 6;

// Minimum interval between compacting the stores in minutes
const TInt KMinimumCompactInterval = 30;

// Number of the index topic
const TInt KIndexTopicNumber = 0;

// Space for reading messages
const TInt KReadMessageSize = 92;

const TInt KTopicsGranularity = 1;

const TInt KTopicListsGranularity = 1;

const TInt KTopicIdsGranularity = 5;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CCbsDbImpTopicList
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicList::CCbsDbImpTopicList( 
    RFs& aFs, 
    CCbsDbImp& aDatabase )
    : iDatabase( aDatabase ), 
    iFs( aFs ),
    iTopicCount( 0 ),
    iInitializing( EFalse ),
    iDeleteAllTopics( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::ConstructL( 
    const TDesC& aTopicsFile,
    const TDesC& aUnsavedMessagesFile )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::ConstructL()");
    
    // Create observer array
    iObservers = new ( ELeave ) CArrayFixFlat< MCbsDbTopicListObserver* >( 
        KCbsDbObserverArraySize );

    // Create topic array, additional memory size in new alloc, 1*192 B = 192 B
    iTopics = new ( ELeave ) CArrayFixFlat< TCbsDbImpTopic >( KTopicsGranularity );

    // Create the root item table, additional memory size in new alloc, 1*184 B = 184 B
    iTopicLists = new ( ELeave ) CArrayFixFlat< TCbsDbImpTopicList >( KTopicListsGranularity ); 

    // Create topic ID array, additional memory size in new alloc, 5*4 B = 20 B
    iTopicIds = new ( ELeave ) CArrayFixFlat< TStreamId >( KTopicIdsGranularity ); 

    // Initialize iPreviousCompact to now
    iPreviousCompact.UniversalTime();

    // Copy datafile names from parameters
    iTopicsFilename = aTopicsFile.AllocL();
    iUnsavedMessagesFilename = aUnsavedMessagesFile.AllocL();
    
    // Fetch local variation bits from CenRep    
    CRepository* repository = CRepository::NewL( KCRUidCbsVariation );    
    TInt err = repository->Get( KCbsVariationFlags, iLVBits );
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::ConstructL(): CenRep error: %d", err );    
    if ( err )
        {
        iLVBits = 0;
        }    
    delete repository;

    // If aLoadFactorySettings, then the files are recreated and initialized.
    // If not, then the nonexisting files are created, files opened and
    // data internalized.

    TBool unsavedMsgFileExists( CbsUtils::ExistsL( iFs, *iUnsavedMessagesFilename ) );

    TBool loadFactorySettings( !CbsUtils::ExistsL( iFs, aTopicsFile ) );
    TRAPD( error, OpenFilesL( loadFactorySettings, EFalse ) );
    
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::ConstructL(): OpenFilesL() error: %d", error );
    
    if ( error == KErrDiskFull )
        {
        // Delete the store and open is again
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ConstructL(): About to delete iTopicStore..." );
        
        delete iTopicStore;
        iTopicStore = NULL;
        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ConstructL(): iTopicStore deletion finished." );

        // Try to open the store again   
        TRAPD( error, iTopicStore = CFileStore::OpenL( iFs, *iTopicsFilename, EFileRead | EFileWrite ) );
        CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::ConstructL(); iTopicStore OpenL() error: %d", error );
        if ( error )
            {
            InitializeListL( ETrue );
            }        
        }
    else
        {
        if ( loadFactorySettings )
            {
            // Create a topic list for standard topics
            if ( iTopicLists->Count() == 0 )
                {
                CreateStandardTopicListL();
                }                
            }

        // Only load the topics, if not already loaded when opening the files
        if ( loadFactorySettings || unsavedMsgFileExists )
            {
            // Load the topics
            LoadDefaultTopicStreamL();
            }
        
        // Compact the topic store
        TopicStoreL()->CompactL();
        TopicStoreL()->CommitL();
        }            
    
    // Reset the unread message count to be sure that UI client gets the correct information,
    // at this point count is always zero.
    for ( TInt i( 0 ); i < iTopics->Count(); ++i )
        {
        iTopics->At( i ).iTopicData.iUnreadMessages = 0;
        }
        
    __TEST_INVARIANT;
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicList* CCbsDbImpTopicList::NewL( 
    RFs& aFs, 
    const TDesC& aTopicsFile, 
    const TDesC& aUnsavedMessagesFile,
    CCbsDbImp& aDatabase )
    {
    CCbsDbImpTopicList* self = 
        new ( ELeave ) CCbsDbImpTopicList( aFs, aDatabase );
    CleanupStack::PushL( self );
    self->ConstructL( aTopicsFile, aUnsavedMessagesFile );
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsDbImpTopicList::~CCbsDbImpTopicList()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::~CCbsDbImpTopicList()");
    delete iTopics;
    delete iTopicStore;
    delete iTopicLists;
    delete iTopicIds;
    delete iUnsavedMessageStore;
    delete iObservers;
    delete iUnsavedMessagesFilename;
    if ( iTopicsFilename )
        {
        delete iTopicsFilename;
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::~CCbsDbImpTopicList()");
    }

#ifndef __SECURE_BACKUP__
// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::ChangeFileLockL
// Closes or reopens the settings file if requested by a backup.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::ChangeFileLockL( 
    const TDesC& aFileName,
    TFileLockFlags aFlags )
    {
    CBSLOGSTRING2("CBSSERVER: >>> CCbsDbImpTopicList::ChangeFileLockL() (1): flag: %d", aFlags );
    
    if ( aFlags == ETakeLock && iTopicStore == NULL )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (1): Try to open store...");
        
        // Try to open the store.
        iTopicStore = CPermanentFileStore::OpenL( iFs, 
            aFileName, EFileRead | EFileWrite );
            
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (1): Store opened.");
        }
    else if ( aFlags != ETakeLock )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (1): Deleting iTopicStore...");
        
        delete iTopicStore; 
        iTopicStore = NULL;
        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (1): iTopicStore deleted.");
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::ChangeFileLockL() (1)");
    }
#else

// -----------------------------------------------------------------------------
// CCbsDbImpSettings::ChangeFileLockL
// Closes or reopens the settings file if requested by backup.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CCbsDbImpTopicList::ChangeFileLockL( const TCbsBackupRequest& aRequest )
    {
    CBSLOGSTRING2("CBSSERVER: >>> CCbsDbImpTopicList::ChangeFileLockL() (2): aRequest: %d", aRequest );
    
    // If backing up or restoring, release locks
    if ( ( aRequest == ECbsBackup || 
        aRequest == ECbsRestore ) )
        {
        delete iTopicStore; 
        iTopicStore = NULL;
        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (2): iTopicStore deleted.");
        }
    // Else take files into use again
    else if ( ( aRequest == ECbsNoBackup || 
        aRequest == ECbsBackupNotDefined ) && 
        iTopicStore == NULL)
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (2): Calling CPermanentFileStore::OpenL()...");
        // Try to open the store.
        iTopicStore = CPermanentFileStore::OpenL( iFs, 
            iTopicsFilename->Des(), EFileRead | EFileWrite );
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::ChangeFileLockL() (2): CPermanentFileStore::OpenL() finished.");
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::ChangeFileLockL() (2)");
    }

#endif

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CheckFileLockL
// Check if the server has a file lock.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::CheckFileLockL() const
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::CheckFileLockL()");
    if ( iTopicStore == NULL )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::CheckFileLockL(): iTopicStore == NULL, leaving with KErrLocked...");
        User::Leave( KErrLocked );
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::CheckFileLockL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CreateNewTopicListL
// Creates a new topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::CreateNewTopicListL( const TDesC& aTopicListName )
    {
    // FFS critical level check
    CbsUtils::FFSCriticalLevelCheckL( KTopicListStreamSize + 
        KCbsDbTopicArraySize, iFs );

    TInt topicListCount = iTopicLists->Count();

    // Check if there already is 10 topic lists
    if ( topicListCount < KCbsRootItemsSize )
        {                  
        // Create the stream for this topic list entry
        RStoreWriteStream outstream;
        TStreamId topicListStreamId( 
            outstream.CreateLC( *TopicStoreL() ) ); // on CS        

        // Set the values for this new topic list and set it as "current".
        // Reset the topic count of this topic list.
        iCurrentTopicList.iTopicCount = 0;

        // Topic list stream id for this list
        iCurrentTopicList.iTopicListId = topicListStreamId;
    
        // List name
        iCurrentTopicList.iTopicListName = aTopicListName; 

        // Is this the default list
        iCurrentTopicList.iIsDefaultTopicList = ETrue;

        // Number of this list, which is a int value between 0...9
        iCurrentTopicList.iNumber = iTopicLists->Count();

        // Add this list to the list array        
        TKeyArrayFix key( _FOFF( TCbsDbImpTopicList, iNumber ), ECmpTUint16 );
        iTopicLists->InsertIsqL( iCurrentTopicList, key );
    
        // Write the values to this topic list's stream
        outstream << iCurrentTopicList.iTopicListName;
        CbsStreamHelper::WriteBoolL( outstream, iCurrentTopicList.iIsDefaultTopicList );
        outstream.WriteInt16L( iCurrentTopicList.iNumber );
        outstream.WriteInt16L( iCurrentTopicList.iTopicCount );

        // Write space for topic stream IDs        
        for ( TInt i( 0 ); i < KCbsDbTopicArraySize; i++ )
            {
            outstream << TStreamId( 0 );
            }        
           
        outstream.CommitL();
        CleanupStack::PopAndDestroy(); // outstream    
        
        // Reset the topic array and add an index topic to this
        // topic list
        iTopics->Reset();
        AddIndexTopicL();
            
        // Update the root stream
        UpdateRootStreamL( EFalse );
    
        // Commit file changes
        CommitFilesL();

        // Load the topics (count = 1, Index topic only), 
        // so that the array is up to date
        LoadTopicsL( iCurrentTopicList.iTopicListId );            
        } 
    else
        {
        User::Leave( KErrGeneral );
        }   
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::SetTopicMessages
// Sets the topic messages db object for this topic list.
// Note that this function does not transfer ownership
// of aMessages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::SetTopicMessages( 
    CCbsDbImpTopicMessages* aMessages )
    {
    __ASSERT_ALWAYS( aMessages != NULL,
        CbsServerPanic( ECbsTopicMessagesNull ) );
    iMessages = aMessages;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::TopicCount
// Returns the total amount of topics the list contains.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicList::TopicCount() const
    {
    return iTopics->Count();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::TopicListCount
// Get topic list count.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicList::TopicListCount() const
    {
    return iTopicLists->Count();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::TopicStoreL
// Returns pointer to the current store which contains topics
// of the server (i.e., Topics-file) and saved messages. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFileStore* CCbsDbImpTopicList::TopicStoreL() const
    {
    CheckFileLockL();
    return iTopicStore;
    }

// ---------------------------------------------------------
// TopicFilename() 
//
// ---------------------------------------------------------
const TDesC& CCbsDbImpTopicList::TopicFilename() const
    {
    return *iTopicsFilename;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UnsavedMessagesStore
// Returns a pointer to the store, which contains unsaved
// messages of the server (Unsaved Messages-file, "cbs4.dat").
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CFileStore* CCbsDbImpTopicList::UnsavedMessagesStore() const
    {
    return iUnsavedMessageStore;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UnsavedMessagesFilename
// Returns a reference to the name of the file, which contains unsaved
// messages of the server (Unsaved Messages-file, "cbs4.dat").
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CCbsDbImpTopicList::UnsavedMessagesFilename() const
    {
    return *iUnsavedMessagesFilename;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::ExtractTopicNumber
// Extracts topic handle from message handle.
// Note that the method does not check that the message exists.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbTopicNumber CCbsDbImpTopicList::ExtractTopicNumber( 
    const TCbsDbMessageHandle& aHandle ) const
    {    
    return TCbsDbTopicNumber( aHandle >> 16 );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetTopicMessagesIdL
// Returns the topic messages stream id by topic handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::GetTopicMessagesIdL( 
    TCbsDbTopicNumber aNumber, TStreamId& aId ) const
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::GetTopicMessagesIdL()");

    // Find the topic.
    TInt index( TopicIndexInList( aNumber ) );
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::GetTopicMessagesIdL(): Leaving if index != 0. Index: %d.", index );
    User::LeaveIfError( index );

    // Return the topic message stream id.
    aId = iTopics->At( index ).iTopicMessagesId;
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::GetTopicMessagesIdL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicMessagesIdL
// Updates the topic messages stream id by topic handle. 
// The new identifier is expected not to be a null id.
// Note that the method will not commit changes to the store. 
// It also changes the internal state and thus if the method leaves, 
// it is good to reload the whole root stream.
// The method will delete the old topic messages stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicMessagesIdL( 
    TCbsDbTopicNumber aNumber, 
    const TStreamId& aNewId )
    {
    __TEST_INVARIANT;
    // Find position.
    TInt index( TopicIndexInList( aNumber ) );
    User::LeaveIfError( index );

    // Get the old id.
    TStreamId oldId( iTopics->At( index ).iTopicMessagesId );

    // Get the topic information.
    TCbsDbTopic topic;
    GetTopicL( index, topic );

    // Write information to the stream.
    RStoreWriteStream outstream;
    outstream.OpenLC( *TopicStoreL(), iTopics->At( index ).iTopicId ); // on CS
    WriteTopicInformationL( outstream, topic, aNewId );
    outstream.CommitL();
    CleanupStack::PopAndDestroy();

    // Delete the old stream.
    TopicStoreL()->DeleteL( oldId );

    // Update the topic messages id.
    iTopics->At( index ).iTopicMessagesId = aNewId;
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GenerateMessageHandle
// Generates a new message handle using the topic 
// handle of the message and a given random value.
// Note that it must be checked that the message handle is unique 
// in that topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbMessageHandle CCbsDbImpTopicList::GenerateMessageHandle( 
    const TCbsDbTopicNumber aNumber, 
    TUint16 aRandom ) const
    {
    return ( aNumber << 16 ) + aRandom;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::ReloadRootStreamL
// Reloads the root stream to the memory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::ReloadRootStreamL()
    {
    LoadRootStreamL();
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::InformNewMessageReceivedL
// Informs that a new message has been received in a topic.
// This method is called by CCbsDbImpTopicMessages. After internal 
// records are changed, the observers are informed of this event.
// Note: leaves changes in stores uncommited.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::InformNewMessageReceivedL( 
    const TCbsDbMessageHandle& aMessageHandle )
    {
    __TEST_INVARIANT;
    // Find topic by handle.
    TCbsDbTopicNumber number( ExtractTopicNumber( aMessageHandle ) );

    TCbsDbTopic topic;
    FindTopicByNumberL( number, topic );

    topic.iUnreadMessages++;

    // Increase the counter in cache
    TInt position = TopicIndexInList( topic.iNumber );
    iTopics->At( position ).iTopicData.iUnreadMessages++;

    // Write topic information to topic stream but leave changes uncommited.
    UpdateTopicCountersL( topic, EFalse );    

    if ( topic.iHotmarked )
        {
        SetHotmarkedMessage( aMessageHandle );
        }

    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::InformMessageSavedL
// Informs that a message has been set as saved.
// Updates the saved messages counters
// for the topic of the message and the whole system.
// Called by CCbsDbTopicMessages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::InformMessageSavedL( 
    const TCbsDbMessageHandle& aMessageHandle )
    {
    __TEST_INVARIANT;
    // Find topic by handle.
    TCbsDbTopicNumber number( ExtractTopicNumber( aMessageHandle ) );

    TCbsDbTopic topic;
    FindTopicByNumberL( number, topic );

    topic.iSavedMessages++;

    // Increase the counter in cache
    TInt position = TopicIndexInList( topic.iNumber );
    iTopics->At( position ).iTopicData.iSavedMessages++;

    // Write topic information to topic stream but leave changes uncommited.
    UpdateTopicCountersL( topic, EFalse );

    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::InformUnreadMessageReadL
// Informs that an unread message has been read.
// Updates the counters when a message is read by the client. 
// Note: leaves changes in stores uncommited.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::InformUnreadMessageReadL( 
    const TCbsDbMessageHandle& aMessageHandle )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::InformUnreadMessageReadL()");
    
    __TEST_INVARIANT;

    // Check for file lock
    CheckFileLockL();

    // Check disk space
    TRAPD( error, CbsUtils::FFSCriticalLevelCheckL( KReadMessageSize, iFs ) );
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::InformUnreadMessageReadL(): FFSCriticalLevelCheckL returned: %d", error );
    if ( error == KErrDiskFull )
        {
        return;
        }

    // Find topic by number.
    TCbsDbTopicNumber number( ExtractTopicNumber( aMessageHandle ) );

    TCbsDbTopic topic;
    FindTopicByNumberL( number, topic );

    // Decrease the counter
    topic.iUnreadMessages--;

    // Decrease the counter in cache
    TInt position = TopicIndexInList( topic.iNumber );
    iTopics->At( position ).iTopicData.iUnreadMessages--;

    // Write topic information to topic stream but leave changes uncommited.
    UpdateTopicCountersL( topic, EFalse );

    __TEST_INVARIANT;
    
   CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::InformUnreadMessageReadL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::InformMessageDeletedL
// Informs that an unread message has been deleted.
// Updates the counters when a message is deleted. 
// Note: leaves changes in stores uncommited.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::InformMessageDeletedL( 
    const TCbsDbMessageHandle& aMessageHandle,
    TBool aPermanent, 
    TBool aRead )
    {
    // Find topic by handle.
    TCbsDbTopicNumber number( ExtractTopicNumber( aMessageHandle ) );

    TCbsDbTopic topic;
    FindTopicByNumberL( number, topic );

    TInt position = TopicIndexInList( topic.iNumber );    

    if ( aPermanent )
        {
        topic.iSavedMessages--;
        iTopics->At( position ).iTopicData.iSavedMessages--;
        }
    
    if ( !aRead )
        {        
        topic.iUnreadMessages--;
        iTopics->At( position ).iTopicData.iUnreadMessages--;
        }

    // Write topic information to topic stream but leave changes uncommited.
    UpdateTopicCountersL( topic, ETrue );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::TotalSavedMessages
// Returns the number of saved messages in the current topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicList::TotalSavedMessages() const
    {
    // Return the total amount of saved messages.
    TInt count( 0 );
    for ( TInt i( 0 ); i < iTopics->Count(); i++ )
        {
        count += iTopics->At( i ).iTopicData.iSavedMessages;
        }

    return count;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetTopicCount
// Returns the number of topic stored in this topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::GetTopicCount( 
    TInt& aCount ) const
    {
    // Return the total amount of topics.
    aCount = TopicCount();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::InitializeListL
// Initializes the whole topic list. 
// Creates and opens the topic list file, 
// invalidates the cache and informs the observers.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::InitializeListL( const TBool aFileOpenFailed )
    {
    __TEST_INVARIANT;

    if ( !aFileOpenFailed )
        {
        // Check for file lock
        CheckFileLockL();
        }    

    // About to write to FFS: make critical level check
    CbsUtils::FFSCriticalLevelCheckL( 0, iFs );

    if ( iMessages != NULL && iMessages->IsLockedMessages() )
        {
        User::Leave( KErrAccessDenied );
        }    

    // If only one topic list exists, just delete and recreate the whole file
    if ( iTopicLists->Count() == 1 || aFileOpenFailed == 1 )
        {
        delete iTopicStore; 
        iTopicStore = NULL;
        delete iUnsavedMessageStore; 
        iUnsavedMessageStore = NULL;
        CbsUtils::DeleteFileL( iFs, *iTopicsFilename  );
        CbsUtils::DeleteFileL( iFs, *iUnsavedMessagesFilename );

        iTopicLists->Reset();
       
        // Create new files.
        OpenFilesL( EFalse, ETrue );
        }
    
    iIsHotmarkedMessage = EFalse;
    iLastTopicNumber = 0;
    iMessageHandle = 0;

    // Inform the message manager.
    if ( iMessages )
        {
        iMessages->InvalidateCache();
        }

    // Notify everyone.
    NotifyTopicListInitializedL();

    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetTopicL
// Returns a topic matching the given index.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::GetTopicL( 
    TInt aIndex, 
    TCbsDbTopic& aTopic )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::GetTopicL()");

    __TEST_INVARIANT;
    
    // Check that aIndex is in proper range.
    if ( ( aIndex < 0 ) || ( aIndex >= iTopics->Count() ) )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::GetTopicL(): Leaving with KErrArgument...");
        User::Leave( KErrArgument );
        }

    // And then get the information from the array.
    aTopic = iTopics->At( aIndex ).iTopicData;

    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::GetTopicL()");
    
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::FindTopicByNumberL
// Returns a topic matching the given topic number 
// (in GSM Specs this is called the Message Identifier)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::FindTopicByNumberL( 
    TCbsDbTopicNumber aNumber, 
    TCbsDbTopic& aTopic )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::FindTopicByNumberL()");
    
    __TEST_INVARIANT;

    TInt topicIndex( TopicIndexInList( aNumber ) );
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::FindTopicByNumberL(): Leaving if topicIndex < 0: topicIndex: %d.", topicIndex );
    User::LeaveIfError( topicIndex );

    GetTopicL( topicIndex, aTopic );

    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::FindTopicByNumberL()");
    
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::AddTopicL
// Adds a new topic to the list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::AddTopicL( 
    TCbsDbTopic& aTopic, const TBool aDetected )
    {
    __TEST_INVARIANT;

    // Check for file lock
    CheckFileLockL();
    
    // Check that topic number is in proper range
    if ( !CheckTopicNumber( aTopic.iNumber ) )
        {
        User::Leave( KErrArgument );
        }    

    // Check if there is a topic with the 
    // same topic number in current topic list.
    if ( TopicIndexInList( aTopic.iNumber ) != KErrNotFound )
        {
        User::Leave( KErrAlreadyExists );
        } 

    // There should not be any saved or unread messages
    aTopic.iSavedMessages = 0;
    aTopic.iUnreadMessages = 0;

    // Variated feature. Also, only topics that were not detected automatically
    // are subscribed by default.
    if ( iLVBits & KCbsLVFlagTopicSubscription && !aDetected )
        {
        aTopic.iSubscribed = ETrue;
        }
    else
        {
        aTopic.iSubscribed = EFalse;        
        }
        
    // Now we have the handle, so let's add the topic.
    TRAPD( error, DoAddTopicL( aTopic ) );
    if ( error != KErrNone )
        {
        RevertFilesL();
        __TEST_INVARIANT;
        User::Leave( error );
        }
    else
        {        
        iLastTopicNumber = aTopic.iNumber;
        NotifyTopicAddedL( aTopic.iNumber );        
        }        
    
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicNameAndNumberL
// Updates the name and the topic number of a topic 
// matching the given handle to the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicNameAndNumberL( 
    TCbsDbTopicNumber aOldNumber,
    TCbsDbTopicNumber aNewNumber, 
    const TCbsDbTopicName& aName )
    {
    // Check for file lock
    CheckFileLockL();

    // First, check that the new number is ok.
    if ( !CheckTopicNumber( aNewNumber ) )
        {
        User::Leave( KErrArgument );
        }

    // First find and then update.
    TCbsDbTopic topic;
    FindTopicByNumberL( aOldNumber, topic );

    // If no changes to topic, no need to update
    if ( !( aOldNumber == aNewNumber && topic.iName == aName ) )
        {        
        if ( topic.iProtected )
            {
            User::Leave( KErrAccessDenied );
            }

        topic.iName = aName;
        topic.iNumber = aNewNumber;

        UpdateTopicL( aOldNumber, topic );        
        }    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicSubscriptionStatusL
// Updates the new topic subscription status to the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicSubscriptionStatusL( 
    TCbsDbTopicNumber aNumber, 
    TBool aStatus )
    {
    __TEST_INVARIANT;

    // Update topic subsciption status.
    TCbsDbTopic topic;
    FindTopicByNumberL( aNumber, topic );

    topic.iSubscribed = aStatus;
    UpdateTopicL( aNumber, topic );
    __TEST_INVARIANT;

    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicHotmarkStatusL
// Updates the new topic hotmarking status to the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicHotmarkStatusL( 
    TCbsDbTopicNumber aNumber, 
    TBool aStatus )
    {
    __TEST_INVARIANT;

    // Check for file lock
    CheckFileLockL();

    // Update topic hotmark status.
    TCbsDbTopic topic;
    FindTopicByNumberL( aNumber, topic );

    topic.iHotmarked = aStatus;
    UpdateTopicL( aNumber, topic );
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DeleteTopicL
// Deletes an existing topic and all its messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DeleteTopicL( 
    TCbsDbTopicNumber aNumber )
    {
    __TEST_INVARIANT;

    // Try to find the position. If it is not found, leave.
    TInt position( TopicIndexInList( aNumber ) );
    User::LeaveIfError( position );

    // Check that there are no locked messages in the topic.
    if ( iMessages->IsLockedMessagesInTopic( aNumber ) )
        {
        User::Leave( KErrAccessDenied );
        }

    // Topic only in one topic list, not an index topic, so delete the topic.
    if ( aNumber != 0 )
        {
        // Just try to delete
        TRAPD( error, DoDeleteTopicL( position ) );
        if ( error != KErrNone )
            {
            // It failed, so we must revert.
            RevertFilesL();

            // Inform the topic messages.
            iMessages->InvalidateCacheIfTopic( aNumber );

            __TEST_INVARIANT;
            User::Leave( error );
            }
        else
            {               
            // Notify the observers
            NotifyTopicDeletedL( aNumber );
            }
        }
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetUnreadMessageCount
// Gets the total amount of unread messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::GetUnreadMessageCount( 
    TInt& aCount ) const
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::GetUnreadMessageCount()");
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::GetUnreadMessageCount(): Topic count: %d", iTopics->Count() );
    
    // Return the total amount of unread messages.
    TInt count( 0 );
    for ( TInt i( 0 ); i < iTopics->Count(); i++ )
        {
        count += iTopics->At( i ).iTopicData.iUnreadMessages;
        }

    aCount = count;
    
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::GetUnreadMessageCount(): Unread msgs found: %d", count );
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::GetUnreadMessageCount()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetHotmarkedMessageHandleL
// Returns the handle to the latest hotmarked message.      
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::GetHotmarkedMessageHandleL( 
    TCbsDbMessageHandle& aMessage )
    {
    __TEST_INVARIANT;
    
    // Check if there is a hotmarked message.
    if ( iIsHotmarkedMessage )
        {
        // If there is, then return it.
        aMessage = iMessageHandle;
        iIsHotmarkedMessage = EFalse;
        }
    else
        {
        // Otherwise leave.
        __TEST_INVARIANT;
        User::Leave( KErrNotFound );
        }
    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UnreadHotmarkedMessageCount
// Returns the handle to the latest hotmarked message.      
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicList::UnreadHotmarkedMessageCount() const
    {
    // Return the total amount of unread messages in hotmarked topics.
    TInt count( 0 );
    for ( TInt i( 0 ); i < iTopics->Count(); i++ )
        {
        TCbsDbTopic& topic = iTopics->At( i ).iTopicData;
        if ( topic.iHotmarked )
            {
            count += topic.iUnreadMessages;
            }
        }
    return count;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::AddObserverL
// Adds a topic list observer. 
// After an observer is added to the topic list, 
// it will be notified whenever an event occurs.      
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::AddObserverL(
    MCbsDbTopicListObserver* aObserver )
    {
    __ASSERT_DEBUG( aObserver != 0, CbsServerPanic( ECbsObserverNull ) );
    iObservers->AppendL( aObserver );
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::RemoveObserver
// Removes a topic list observer. 
// If aObserver is not in the list, the method will panic.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::RemoveObserver( 
    const MCbsDbTopicListObserver* aObserver )
    {
    __TEST_INVARIANT;
    __ASSERT_DEBUG( aObserver != 0, CbsServerPanic( ECbsObserverNull ) );
    
    TBool observerFound( EFalse );
    TInt amountOfObservers( iObservers->Count() );

    // Check if the observer exists and 
    // find its possible location index in the array.
    for ( TInt index( 0 ); ( index < amountOfObservers ) && !observerFound; index++ )
        {
        if ( aObserver == iObservers->At( index ) )
            {
            iObservers->Delete( index );
            observerFound = ETrue;
            }
        }

    __TEST_INVARIANT;

#ifdef _DEBUG
    if ( !observerFound )
        {
        CbsServerPanic( ECbsObserverNotFound );
        }
#endif

    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::TopicIndexInList
// Finds the index of the topic matching the given topic number
// in the topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicList::TopicIndexInList( 
    TCbsDbTopicNumber aNumber ) const
    {
    // Create a TCbsDbImpTopic to compare against and use a binary search.
    TKeyArrayFix key( _FOFF( TCbsDbImpTopic, iTopicData.iNumber ), ECmpTUint16 );
    TCbsDbImpTopic dummy;
    TInt position;

    dummy.iTopicData.iNumber = aNumber;
    TInt result( iTopics->FindIsq( dummy, key, position ) );
    
    if ( result != KErrNone )
        {
        position = KErrNotFound;
        }

    return position;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetNextAndPrevTopicNumberL
// Retrieves numbers of topics that precede and succeed the topic
// with number aCurrentTopic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::GetNextAndPrevTopicNumberL( 
    const TCbsTopicNumber& aCurrentTopic,
    TCbsTopicNumber& aNextTopic, 
    TCbsTopicNumber& aPrevTopic,
    TInt& aPosition )
    {
    // Determine position of requested topic in topic list.
    TInt index( TopicIndexInList( aCurrentTopic ) );
    User::LeaveIfError( index );    // if KErrNotFound
    
    // Determine position indications
    aPosition = 0;
    if ( index == 0 )
        {
        aPosition |= ECbsHead;
        }
    else
        {
        aPrevTopic = iTopics->At( index-1 ).iTopicData.iNumber;
        }

    if ( index == iTopics->Count()-1 )
        {
        aPosition |= ECbsTail;
        }
    else
        {
        aNextTopic = iTopics->At( index+1 ).iTopicData.iNumber;
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::NotifyNewMessageArrivedL
// Notifies each observer that a new message has arrived to a topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::NotifyNewMessageArrivedL(
    const TCbsDbMessageHandle& aHandle )
    {
    // Notify each observer.
    TInt count( iObservers->Count() );
    for ( TInt index( 0 ); index < count; index++ )
        {
        iObservers->At( index )->TopicNewMessageReceivedIndL( aHandle );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::AppendSubscribedTopicsL
// Adds numbers of subscribed topics to the given array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::AppendSubscribedTopicsL( 
    CArrayFixFlat<TUint16>& aSubscriptions ) const
    {
    TInt count( iTopics->Count() );
    for ( TInt i( 0 ); i < count; i++ ) 
        {
        TCbsDbImpTopic& topic = iTopics->At( i );
        if ( topic.iTopicData.iSubscribed )
            {
            aSubscriptions.AppendL( topic.iTopicData.iNumber );
            }
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::LoadRootStreamL
// Loads the root stream to the memory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::LoadRootStreamL()
    {
    __ASSERT_DEBUG( iTopicLists != NULL, 
        CbsServerPanic( ECbsTopicListArrayNull ) );

    // Get the root stream and open it.
    TStreamId id( TopicStoreL()->Root() );
    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), id ); // on CS

    // Load the topic list count
    TInt topicListCount( instream.ReadInt16L() );

    // Sanity check
    if ( topicListCount < 0 )
        {
        User::Leave( KErrCorrupt );
        }

    // Reset the topic list array
    iTopicLists->Reset();    

    // Load the topic list information
    TCbsDbImpTopicList item; 
    instream >> item.iTopicListId;
    ReadTopicListInformationL( item.iTopicListId, item );
    iTopicLists->AppendL( item );
        
    // Destroy the stream.
    CleanupStack::PopAndDestroy(); // instream
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::LoadDefaultTopicStreamL
// Loads the default topic list to the memory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::LoadDefaultTopicStreamL()
    {
    __ASSERT_DEBUG( iTopics != NULL, 
        CbsServerPanic( ECbsTopicListArrayNull ) );

    // Read root item count
    TInt topicListCount = iTopicLists->Count();

    // If there isn't any, create a topic list for standard topics here
    if ( topicListCount == 0 )
        {
        CreateStandardTopicListL();
        topicListCount = iTopicLists->Count();
        }

    TStreamId defaultTopicListId( 0 );
    TBool quitSearch( EFalse );

    TInt i;
    // Find the default topic list      
    for ( i = 0; ( i < topicListCount ) && !quitSearch; ++i )
        {
        if ( iTopicLists->At( i ).iIsDefaultTopicList )
            {            
            defaultTopicListId = iTopicLists->At( i ).iTopicListId;
            quitSearch = ETrue;
            }        
        }
        
    CArrayFixFlat< TStreamId >* topicIds = 
        new ( ELeave ) CArrayFixFlat< TStreamId >( KTopicIdsGranularity );

    // Open the default topic list stream
    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), defaultTopicListId ); // on CS

    // Set the ID
    iCurrentTopicList.iTopicListId = defaultTopicListId;
    
    // Set the name
    HBufC* topicListName = HBufC::NewL( instream, KCbsDbTopicNameLength );
    iCurrentTopicList.iTopicListName.Copy( topicListName->Des() );
    delete topicListName;
    topicListName = NULL;

    // Skip default list status, since it is always true in default topic list
    CbsStreamHelper::ReadBoolL( instream );
    iCurrentTopicList.iIsDefaultTopicList = ETrue;

    // Set the topic list number
    iCurrentTopicList.iNumber = instream.ReadInt16L();
    
    // Read the amount of topics
    TInt topicCount = instream.ReadInt16L();
    iCurrentTopicList.iTopicCount = topicCount;

    // Clear the topic array.
    iTopics->ResizeL( 0 );

    TStreamId id( 0 );

    // Load the topic IDs       
    for ( i = 0; i < topicCount; i++ )
        {
        instream >> id;
        topicIds->AppendL( id );
        }   
    
    // Destroy the stream.
    CleanupStack::PopAndDestroy();
       
    // Load necessary information for each topic
    TInt count = topicIds->Count();
    TCbsDbImpTopic topic;
    for ( i = 0; i < count; i++ )
        {
        ReadTopicInformationL( topicIds->At( i ), topic );

        if ( topic.iTopicData.iNumber == KIndexTopicNumber )
            {
            HBufC* indexName = ReadIndexTopicNameLC(); // on CS
            topic.iTopicData.iName.Copy( *indexName );
            CleanupStack::PopAndDestroy(); // indexName
            }

        iTopics->AppendL( topic );
        }
    delete topicIds;
    topicIds = NULL;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::ReadTopicListInformationL
// Reads topic list information from stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::ReadTopicListInformationL( 
    const TStreamId& aId, 
    TCbsDbImpTopicList& aTopicList ) const
    {
    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), aId ); // on CS

    // Topic List name
    HBufC* topicListName = HBufC::NewL( instream, KCbsDbTopicNameLength );
    aTopicList.iTopicListName.Copy( topicListName->Des() );
    delete topicListName;
    topicListName = NULL;

    // Default list status
    aTopicList.iIsDefaultTopicList = CbsStreamHelper::ReadBoolL( instream );

    // Topic List number
    aTopicList.iNumber = instream.ReadInt16L();

    // Topic count of this topic list
    aTopicList.iTopicCount = instream.ReadInt16L();   

    CleanupStack::PopAndDestroy(); // instream
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::ReadTopicInformationL
// Reads all information on topic found in stream aId
// into aTopic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::ReadTopicInformationL( 
    const TStreamId& aId, 
    TCbsDbImpTopic& aTopic ) const
    {
    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), aId ); // on CS

    // Topic ID
    aTopic.iTopicId = aId;

    // Read saved messages.
    aTopic.iTopicData.iSavedMessages = instream.ReadInt16L();

    // Read name.
    instream >> aTopic.iTopicData.iName;
    
    // Read topic number (message identifier)
    aTopic.iTopicData.iNumber = instream.ReadInt16L();

    // Read statuses
    aTopic.iTopicData.iProtected = CbsStreamHelper::ReadBoolL( instream ); // Protected
    aTopic.iTopicData.iSubscribed = CbsStreamHelper::ReadBoolL( instream );// Subscribed
    aTopic.iTopicData.iHotmarked = CbsStreamHelper::ReadBoolL( instream ); // Hotmarked
    
    // Read unread messages count
    aTopic.iTopicData.iUnreadMessages = instream.ReadInt16L();

    // Topic messages' stream ID
    instream >> aTopic.iTopicMessagesId;

    // Sanity check
    if ( aTopic.iTopicData.iSavedMessages > KCbsDbMaxSavedMessages
        || aTopic.iTopicData.iNumber > KCbsMaxValidTopicNumber
        || aTopic.iTopicData.iUnreadMessages > KCbsDbMaxReceivedMessages )
        {
        User::Leave( KErrCorrupt );
        }

    CleanupStack::PopAndDestroy(); // instream
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DoAddTopicL
// Adds a topic into the database.
// Assumes aTopic is not a duplicate.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DoAddTopicL( 
    const TCbsDbTopic& aTopic )
    {
    // About to write to FFS: make critical level check
    CbsUtils::FFSCriticalLevelCheckL( KTopicStreamSize + 
        KEmptyTopicMessagesStreamSize, iFs );  

    // Generate general information about topic.
    TCbsDbImpTopic topic;
    topic.iTopicData = aTopic;
    
    // Write stream for messages.
    topic.iTopicMessagesId = 
        CCbsDbImpTopicMessages::CreateDefaultTopicMessagesStreamL( 
            *TopicStoreL() );

    // Create stream for topic information.
    RStoreWriteStream outstream;
    topic.iTopicId = outstream.CreateLC( *TopicStoreL() ); // on CS
    
    WriteTopicInformationL( outstream, aTopic, topic.iTopicMessagesId );

    outstream.CommitL();
    CleanupStack::PopAndDestroy(); // outstream

    // Now, insert the new topic information to the array
    TKeyArrayFix key( _FOFF( TCbsDbImpTopic, iTopicData.iNumber ), ECmpTUint16 );
    iTopics->InsertIsqL( topic, key );    

    // Update the topic list stream
    UpdateTopicListStreamL( iCurrentTopicList, EFalse );
  
    // We have modified only iTopicStore, so if CommitFilesL() leaves,
    // we either get all committed or nothing committed.
    CommitFilesL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DoUpdateTopicL
// Updates the data for a topic into the database.
// The position of the topic in the list is
// changed if necessary. 
// If the position has to be changed, the handles of messages
// contained in the topic have to be changed as well because
// the high word of the handle identifies the topic by it's
// number.
// Please note that standard EPOC DoSomethingL protocol
// is not followed here as this function does NOT commit
// changes made to the store. Instead, the caller must ensure
// use of proper commit/revert operations.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DoUpdateTopicL( 
    const TCbsDbTopic& aTopic, 
    TBool aNeedToChange, 
    TInt aOldPosition,
    TBool aDeleting )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::DoUpdateTopicL()");
    
    RSharedDataClient sharedData;
    // About to write to FFS: make critical level check
    if ( aDeleting )
        {
        User::LeaveIfError( sharedData.Connect() );
        sharedData.RequestFreeDiskSpaceLC( KTopicStreamSize ); // on CS
        }
    else
        {
        CbsUtils::FFSCriticalLevelCheckL( KTopicStreamSize, iFs );
        }

    TInt newPosition( aOldPosition );

    // If there is need to change position, change it.
    if ( aNeedToChange )
        {
        TCbsDbImpTopic topic( iTopics->At( aOldPosition ) );

        // Adjust handles of topic messages to match the changed topic number.
        iMessages->UpdateHandlesOfTopicMessagesL( 
            topic.iTopicData.iNumber, aTopic.iNumber );
            
        // Delete topic from the array.
        iTopics->Delete( aOldPosition );        

        // Set the number and reinsert into array
        topic.iTopicData.iNumber = aTopic.iNumber;
        TKeyArrayFix key( _FOFF( TCbsDbImpTopic, iTopicData.iNumber ), ECmpTUint16 );
        newPosition = iTopics->InsertIsqL( topic, key );
        }

    iTopics->At( newPosition ).iTopicData.iSubscribed = aTopic.iSubscribed;
    iTopics->At( newPosition ).iTopicData.iHotmarked = aTopic.iHotmarked;
    iTopics->At( newPosition ).iTopicData.iName = aTopic.iName;
    iTopics->At( newPosition ).iTopicData.iSavedMessages = aTopic.iSavedMessages;
    iTopics->At( newPosition ).iTopicData.iUnreadMessages = aTopic.iUnreadMessages;
    
    // Replace existing stream.
    RStoreWriteStream outstream;
    outstream.ReplaceLC( *TopicStoreL(), 
        iTopics->At( newPosition ).iTopicId ); // on CS
    WriteTopicInformationL( outstream, aTopic, 
        iTopics->At( newPosition ).iTopicMessagesId );

    outstream.CommitL();
    CleanupStack::PopAndDestroy(); // outstream

    // Free the reserved space
    if ( aDeleting )
        {
        CleanupStack::PopAndDestroy(); // disk space
        sharedData.Close();
        }
    
    // Update topic list stream, if necessary.
    if ( aNeedToChange )
        {
        UpdateTopicListStreamL( iCurrentTopicList, EFalse );
        }
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::DoUpdateTopicL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DoDeleteTopicL
// Deletes a topic from the given position in the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DoDeleteTopicL( 
    TInt aPosition )
    {
    // Check that the topic is not protected.
    TCbsDbTopic topic;
    GetTopicL( aPosition, topic );

    // It is not allowed to delete a topic that is protected. It must
    // first be updated to be not protected.
    if ( topic.iProtected )
        {
        User::Leave( KErrAccessDenied );
        }

    // First delete all messages the topic contains.
    RStoreReadStream instream;    
    instream.OpenLC( *TopicStoreL(), 
        iTopics->At( aPosition ).iTopicMessagesId ); // on CS
    CCbsDbImpTopicMessages::DeleteAllTopicMessagesL( *TopicStoreL(), 
        *iUnsavedMessageStore, instream );
    iMessages->InvalidateCache();
    CleanupStack::PopAndDestroy(); // instream

    // Delete topic and topic messages streams.
    TopicStoreL()->DeleteL( iTopics->At( aPosition ).iTopicMessagesId );
    TopicStoreL()->DeleteL( iTopics->At( aPosition ).iTopicId );    

    // Remove from the internal cache.
    iTopics->Delete( aPosition );

    iCurrentTopicList.iTopicCount--;                
    UpdateTopicListStreamL( iCurrentTopicList, ETrue );
    CommitFilesL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateRootStreamL
// Updates the root stream. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateRootStreamL(
    TBool aDeleting )
    {      
    // Check the free space
    TInt neededSpace( KTopicListRootStreamSize + iTopicLists->Count() * 2 );        

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

    // Now there is room for all topics. So we can just replace.
    RStoreWriteStream outstream;
    outstream.ReplaceLC( *TopicStoreL(), TopicStoreL()->Root() ); // on CS        

    // Write root stream
    WriteRootStreamL( outstream );
    CleanupStack::PopAndDestroy(); // outstream

    // Free the reserved space
    if ( aDeleting )
        {
        CleanupStack::PopAndDestroy(); // disk space
        sharedData.Close();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicListStreamL
// Updates topic list stream. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicListStreamL( 
    TCbsDbImpTopicList& aTopicList,
    TBool aDeleting )
    {        
    TInt neededSpace( KTopicListStreamSize + iTopics->Count() * 2 );
    
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
    
    // Replace the stream
    RStoreWriteStream outstream;
    outstream.ReplaceLC( *TopicStoreL(), aTopicList.iTopicListId ); // on CS
    
    // Write root stream
    WriteTopicListStreamL( outstream, aTopicList );
    CleanupStack::PopAndDestroy(); // outstream
    
    // Free the reserved space
    if ( aDeleting )
        {
        CleanupStack::PopAndDestroy(); // disk space
        sharedData.Close();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicL
// Updates the information for a topic already existing 
// in the database.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicL( 
    TCbsDbTopicNumber aTopicNumber, 
    TCbsDbTopic& aTopic )
    {
    // Check that the new topic number is unique.
    TBool needToChangePosition( ETrue );

    // Try to find the topic and leave if it was not found.
    TInt oldPosition( TopicIndexInList( aTopicNumber ) );
    User::LeaveIfError( oldPosition );

    // Check if we have to change the position of the topic in the 
    // internal array
    if ( aTopic.iNumber == aTopicNumber )
        {
        needToChangePosition = EFalse;
        }
    else
        {
        TInt topicWithTheNewNumber( TopicIndexInList( aTopic.iNumber ) );
        if ( topicWithTheNewNumber != KErrNotFound && 
             topicWithTheNewNumber != oldPosition )
            {
            User::Leave( KErrAlreadyExists );
            }
        }

    // Write data to store.
    // Deviation from EPOC standards: DoUpdateTopicL does NOT commit 
    // the store.
    TRAPD( result, DoUpdateTopicL( aTopic, needToChangePosition, 
        oldPosition, EFalse ) );

    // Commit both topic and unsaved msgs store
    if ( result == KErrNone )
        {
        TRAP( result, CommitFilesL() );
        }

    // If either DoUpdateTopicL or CommitFilesL fails => revert.
    if ( result != KErrNone )
        {
        TopicStoreL()->Revert();
        ReloadRootStreamL();
        User::Leave( result );
        }    
    else
        {
        // Notify the observers.
        NotifyTopicModifiedL( aTopicNumber );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CheckTopicNumber
// Checks if a topic number is valid.
// The valid topic number range in this implementation is 
// 000..999, with 000 indicating an index message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsDbImpTopicList::CheckTopicNumber( 
    TCbsDbTopicNumber aNumber ) const
    {
    // Check that the number is in proper range
    return aNumber <= KCbsMaxValidTopicNumber;        
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::WriteRootStreamL
// Write the root stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::WriteRootStreamL( 
    RWriteStream& aOut ) const
    {
    // Write the total amount of topic lists 
    TInt topicListCount( iTopicLists->Count() );
    aOut.WriteInt16L( topicListCount );
    
    // Write space for topic list stream ids
    TInt index;
    for ( index = 0; index < topicListCount; index++ )
        {
        aOut << iTopicLists->At( index ).iTopicListId;
        }
    
    // Write null stream ids
    for ( ; index < KCbsRootItemsSize; index++ )
        {
        aOut << TStreamId( 0 );
        }
    
    aOut.CommitL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::WriteTopicListStreamL
// Writes topic list information to specified stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::WriteTopicListStreamL( 
    RWriteStream& aOut, 
    TCbsDbImpTopicList& aTopicList ) const
    {
    // Write the values to this topic list's stream

    // Topic List name
    aOut << aTopicList.iTopicListName;

    // Is this the default list
    CbsStreamHelper::WriteBoolL( aOut, aTopicList.iIsDefaultTopicList );

    // Topic List number
    aOut.WriteInt16L( aTopicList.iNumber );

    // NUmber of topics in this list
    aOut.WriteInt16L( iTopics->Count() );

    // Write the stream IDs of the topics belonging to this list    
    TInt i;
    for ( i = 0; i < iTopics->Count(); i++ )
        {
        aOut << iTopics->At( i ).iTopicId;
        }

    // Write space for the rest topic stream IDs    
    for ( ; i < KCbsDbTopicArraySize; i++ )
        {
        aOut << TStreamId( 0 );
        }        
           
    aOut.CommitL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::NotifyTopicListInitializedL
// Notifies each observer that the topic list has been initialized.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::NotifyTopicListInitializedL()
    {
    // Notify each observer.
    TInt count( iObservers->Count() );
    for( TInt index( 0 ); index < count; index++ )
        {
        iObservers->At( index )->TopicListInitializedIndL();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::NotifyTopicAddedL
// Notifies each observer that a topic has been added.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::NotifyTopicAddedL( 
    TCbsDbTopicNumber aNumber )
    {
    // Notify each observer.
    TInt count( iObservers->Count() );
    for ( TInt index( 0 ); index < count; index++ )
        {
        iObservers->At( index )->TopicAddedIndL( aNumber );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::NotifyTopicModifiedL
// Notifies each observer that a topic has been modified.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::NotifyTopicModifiedL( 
    TCbsDbTopicNumber aNumber )
    {
    // Notify each observer.
    TInt count( iObservers->Count() );
    for( TInt index( 0 ); index < count; index++ )
        {
        iObservers->At( index )->TopicModifiedIndL( aNumber );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::NotifyTopicDeletedL
// Notifies each observer that a topic has been deleted.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::NotifyTopicDeletedL( 
    TCbsDbTopicNumber aNumber )
    {
    // Notify each observer.
    TInt count( iObservers->Count() );
    for( TInt index( 0 ); index < count; index++ )
        {
        iObservers->At( index )->TopicDeletedIndL( aNumber );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::SetHotmarkedMessage
// Sets the hotmarked message handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::SetHotmarkedMessage( 
    const TCbsDbMessageHandle& aMessageHandle )
    {
    // Set the hotmarked message.
    iIsHotmarkedMessage = ETrue;
    iMessageHandle = aMessageHandle;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CreateDefaultRootStreamL
// Creates a root stream and writes default values into it. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TStreamId CCbsDbImpTopicList::CreateDefaultRootStreamL( 
    CStreamStore& aStore ) const
    {
    // Create the stream
    RStoreWriteStream outstream;
    TStreamId id( outstream.CreateLC( aStore ) ); // on CS

    // Write the amount of topic lists
    outstream.WriteInt16L( 0 );

    // Write space for topic list stream ids
    for ( TInt index( 0 ); index < KCbsRootItemsSize; index++ )
        {
        outstream << TStreamId( 0 );
        } 

    outstream.CommitL();
    CleanupStack::PopAndDestroy(); // aStore

    // Return the stream id.
    return id;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::WriteTopicInformationL
// Writes topic data into a stream.
// This includes number of saved messages, 
// number of unread messages, topic handle, 
// topic name, topic number, protection status, 
// subscription status and hotmark status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::WriteTopicInformationL( 
    RWriteStream& aOut, 
    const TCbsDbTopic& aTopic, 
    const TStreamId& aTopicMessagesId ) const
    {
    // Write saved messages.
    aOut.WriteInt16L( aTopic.iSavedMessages );

    // Write name.
    aOut << aTopic.iName;
    
    // Write topic number (message identifier)
    aOut.WriteInt16L( aTopic.iNumber );

    // Write statuses
    CbsStreamHelper::WriteBoolL( aOut, aTopic.iProtected ); // Protected
    CbsStreamHelper::WriteBoolL( aOut, aTopic.iSubscribed ); // Subscribed
    CbsStreamHelper::WriteBoolL( aOut, aTopic.iHotmarked ); // Hotmarked
    
    // Write unread messages count
    aOut.WriteInt16L( aTopic.iUnreadMessages );

    // And then write the topic messages stream id.
    aOut << aTopicMessagesId;
    }


// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::RevertFilesL
// Reverts all changes made to three datafiles handled by this class.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::RevertFilesL()
    {

    // Check for file lock
    CheckFileLockL();

    iTopicStore->Revert();   
    iUnsavedMessageStore->Revert();
    ReloadRootStreamL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CommitFilesL
// Commits all changes made to two datafiles handled by this class.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::CommitFilesL()
    {
    TopicStoreL()->CommitL();
    TInt errorCode( iUnsavedMessageStore->Commit() );

    // If committing of the unsaved msg store fails, remove all
    // messages in the store and rebuild the topic list
    if ( errorCode != KErrNone )
        {
        TRAPD( errorCode2, RebuildUnsavedMessageStoreL() );
        if ( errorCode2 != KErrNone )
            {
            CActiveScheduler::Stop();
            User::Leave( KErrServerTerminated );
            }

        // Tell the caller that something went wrong
        User::Leave( errorCode );
        }

    // Check if we should compact
    TTime now;
    TTimeIntervalMinutes interval;
    now.UniversalTime();
    now.MinutesFrom( iPreviousCompact, interval );
    if ( interval.Int() >= KMinimumCompactInterval )
        {
        TopicStoreL()->CompactL();
        iUnsavedMessageStore->CompactL();
        iPreviousCompact = now;
        }

    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::RebuildUnsavedMessageStoreL
// Deletes the unsaved messages' store and rebuilds it.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::RebuildUnsavedMessageStoreL() 
    {
    // Close the stores and delete the unsaved store
    delete iUnsavedMessageStore; iUnsavedMessageStore = NULL;
    delete iTopicStore; iTopicStore = NULL;
    CbsUtils::DeleteFileL( iFs, *iUnsavedMessagesFilename );

    // Re-create the store
    DoCreateStoreL( *iUnsavedMessagesFilename );
    TryToOpenFilesL( ETrue, EFalse );

    // Remove the stream ids to unsaved messages, because
    // they were all just deleted.
    HandleDeletionOfUnsavedMessagesFileL();

    // Compact the topic list
    TopicStoreL()->CommitL();
    TopicStoreL()->CompactL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::RebuildTopicAndUnsavedStoresL
// Deletes and rebuilds topic/topic list and unsaved message stores.
// Loads Standard Topic List into memory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::RebuildTopicAndUnsavedStoresL()
    {
    __TEST_INVARIANT;

    // Check for file lock
    CheckFileLockL();

    // About to write to FFS: make critical level check
    CbsUtils::FFSCriticalLevelCheckL( KTopicListRootStreamSize + 
        KDefaultTopicListSize * 2, iFs );
    
    if ( iMessages != NULL && iMessages->IsLockedMessages() )
        {
        User::Leave( KErrAccessDenied );
        }

    delete iTopicStore; 
    iTopicStore = NULL;
    delete iUnsavedMessageStore; 
    iUnsavedMessageStore = NULL;
    CbsUtils::DeleteFileL( iFs, *iTopicsFilename  );
    CbsUtils::DeleteFileL( iFs, *iUnsavedMessagesFilename );

    // Create new files.
    OpenFilesL( ETrue, EFalse );

    // Add standard index topic.
    AddIndexTopicL();
    
    // Load the Standard Topic list
    LoadDefaultTopicStreamL();

    iIsHotmarkedMessage = EFalse;
    iLastTopicNumber = 0;
    iMessageHandle = 0;

    // Inform the message manager.
    if ( iMessages )
        {
        iMessages->InvalidateCache();
        }

    // Notify everyone.
    NotifyTopicListInitializedL();

    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::GetLatestTopicNumber
// Returns the number of the topic that was added last
// to the database by topic detection feature.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsDbImpTopicList::GetLatestTopicNumber( 
    TCbsTopicNumber& aNumber ) const
    {
    TInt result( KErrNone );
    if ( iLastTopicNumber == 0 )
        {
        result = KErrNotFound;
        }
    aNumber = iLastTopicNumber;
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::OpenFilesL
// After a call to this function, the file stores can be assumed
// to be open and initialized.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::OpenFilesL( 
    TBool aDeleteExistingFiles,
    TBool aCreateNewTopicList )
    {
    __ASSERT_DEBUG( iTopicsFilename->Length() > 0, 
        CbsServerPanic( ECbsInvalidFilenameDescriptor ) );
    __ASSERT_DEBUG( iUnsavedMessagesFilename->Length() > 0, 
        CbsServerPanic( ECbsInvalidFilenameDescriptor ) );

    // if LFS, delete files, create files, open files, write root stream
    // if not LFS, create file if it doesn't exist, open files

    // Close file stores.
    delete iTopicStore; 
    iTopicStore = NULL;
    delete iUnsavedMessageStore; 
    iUnsavedMessageStore = NULL;

    // If any of files doesn't exist, create the file. Also writes the root 
    // stream of the topic file, if necessary.
    // It is possible that this operation fails because FFS is full.
    // In this case the server will take a break and retry. If the second
    // attempt fails, the server is shut down.
    TBool unsavedMsgFileExists( EFalse );
    TRAPD( result, CreateFilesIfNecessaryL( unsavedMsgFileExists ) );
    if ( result != KErrNone )
        {
        // Critical exception: wait for a while and retry.
        User::After( KWaitAfterCriticalStoreException );
        TBool ignoreThis( EFalse ); // value of unsavedMsgFileExists preserved
        TRAP( result, CreateFilesIfNecessaryL( ignoreThis ) );
        if ( result != KErrNone )
            {
            __DEBUGGER();
            // Recovery is not possible: shut the server down.
            CActiveScheduler::Stop();
            User::Leave( KErrServerTerminated );
            }
        }

    // Open the files for use. Also reads the topic file root stream.
    TryToOpenFilesL( unsavedMsgFileExists == EFalse && 
                     aDeleteExistingFiles == EFalse, 
                     aCreateNewTopicList );

    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CreateFilesIfNecessaryL
// Creates CBS files, if appropriate.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::CreateFilesIfNecessaryL( 
    TBool& aUnsavedMsgFileExisted )
    {    
    if ( CbsUtils::ExistsL( iFs, *iTopicsFilename ) == EFalse )
        {
        CbsUtils::FFSCriticalLevelCheckL( KTopicListRootStreamSize + 
            KDefaultTopicListSize * 2, iFs );
   
        CPermanentFileStore* store = CPermanentFileStore::CreateLC( iFs,
            *iTopicsFilename, EFileWrite ); // on CS
    
        store->SetTypeL( store->Layout() );
        TStreamId id( CreateDefaultRootStreamL( *store ) );
        store->SetRootL( id );
        store->CommitL();
        CleanupStack::PopAndDestroy(); // store
        }    

    aUnsavedMsgFileExisted = CbsUtils::ExistsL( iFs, 
        *iUnsavedMessagesFilename );
    if ( aUnsavedMsgFileExisted == EFalse )
        {
        // No critical level check, because unsaved msgs reside on ramdisk
        DoCreateStoreL( *iUnsavedMessagesFilename );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DoCreateStoreL
// Creates an empty file store with the given filename.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DoCreateStoreL( 
    const TDesC& aFilename )
    {
    CFileStore* store = CPermanentFileStore::CreateLC( iFs, 
        aFilename, EFileWrite ); // on CS
    store->SetTypeL( store->Layout() );
    store->CommitL();
    CleanupStack::PopAndDestroy();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::TryToOpenFilesL
// Tries to open topic and unsaved messages files.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::TryToOpenFilesL( 
    TBool aDeleteUnsavedMsgStreamIds,
    TBool aCreateNewTopicList )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::TryToOpenFilesL()" );
    
    if ( !iTopicStore )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::TryToOpenFilesL(): *** NO iTopicStore (1) ***" );    
        }
    
    // Try to open the store    
    TRAPD( error, iTopicStore = CFileStore::OpenL( iFs, *iTopicsFilename, EFileRead | EFileWrite ) );
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::TryToOpenFilesL(); iTopicStore OpenL() error: %d", error );
    
    TRAPD( error2, iUnsavedMessageStore = CFileStore::OpenL( iFs, *iUnsavedMessagesFilename, EFileRead | EFileWrite ) );
    CBSLOGSTRING2("CBSSERVER: CCbsDbImpTopicList::TryToOpenFilesL(); iUnsavedMessageStore OpenL() error: %d", error2 );
       
    if ( error || error2 )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::TryToOpenFilesL(): Calling InitializeListL( ETrue )..." );
        
        InitializeListL( ETrue );
        
        CBSLOGSTRING("CBSSERVER: CCbsDbImpTopicList::TryToOpenFilesL(): Calling InitializeListL( ETrue ) finished." );
        }
    else
        {
        if ( iTopicLists->Count() == 0 && 
            aDeleteUnsavedMsgStreamIds && 
            aCreateNewTopicList )
            {
            // Create first topic list, since it was deleted with the file
            CreateStandardTopicListL();
            }
                                               
        // Load the root stream for topic store.
        LoadRootStreamL();

        if ( aDeleteUnsavedMsgStreamIds )
            {
            // Load the topics and repair the topic file 
            // since unsaved msg file has been deleted
            LoadDefaultTopicStreamL();
            HandleDeletionOfUnsavedMessagesFileL();
            }
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::TryToOpenFilesL()" );
    }

// ---------------------------------------------------------
// CCbsDbImpTopicList::ReadIndexTopicNameLC()
// Reads the localized index topic name
// (other items were commented in a header).
// ---------------------------------------------------------
HBufC* CCbsDbImpTopicList::ReadIndexTopicNameLC()
    {
    // Open localized resource file.
    RResourceFile resourceFile;

    CbsUtils::FindAndOpenDefaultResourceFileLC( iFs, resourceFile ); // on CS
    // Read "Index"-string.
    TResourceReader reader;
    reader.SetBuffer( resourceFile.AllocReadLC( R_TEXT_INDEX_TOPIC ) );//on CS
    HBufC* text = reader.ReadHBufCL();
    CleanupStack::PopAndDestroy(2); // readerBuf, resourceFile
    CleanupStack::PushL( text );

    return text;
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::AddIndexTopicL
// Adds index topic to topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::AddIndexTopicL()
    {
    // Open localized resource file.
    HBufC* text = ReadIndexTopicNameLC();

    TCbsDbTopic topic;
    topic.iName.Copy( *text );
    topic.iNumber = 0;
    topic.iHotmarked = EFalse;
    topic.iProtected = ETrue;
    topic.iSubscribed = ETrue;
    topic.iSavedMessages = 0;
    topic.iUnreadMessages = 0;

    // Add the topic without notifying anybody.
    TRAPD( error, DoAddTopicL( topic ) );
    if ( error != KErrNone )
        {
        RevertFilesL();
        __TEST_INVARIANT;
        User::Leave( error );
        }

    CleanupStack::PopAndDestroy();  // text
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::HandleDeletionOfUnsavedMessagesFileL
// Called to repair the database when Unsaved messages -file
// has been deleted, but Topics-file still contains information
// on unsaved messages.
//  
// Things to do here: 
//      - Reset unread message counters, because a saved message
//        is always also read.
//      - Remove stream ids to unsaved messages from 
//        Topic messages -streams.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::HandleDeletionOfUnsavedMessagesFileL()
    {
    TRAPD( result, DoHandleDeletionOfUnsavedMessagesFileL() );
    if ( result != KErrNone )
        {
        // Recovery impossible -> reset the database.
        // Note that this function is not called when the database
        // is initialized.
        RebuildTopicAndUnsavedStoresL();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DoHandleDeletionOfUnsavedMessagesFileL
// Does the work for HandleDeletionOfUnsavedMessagesFileL().
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DoHandleDeletionOfUnsavedMessagesFileL()
    {
    // Reset the unread counter of each topic and delete stream ids of 
    // unsaved messages.
    TInt numberOfTopics( iTopics->Count() );
    for ( TInt i( 0 ); i < numberOfTopics; i++ )
        {
        TCbsDbTopic topic;
        FindTopicByNumberL( iTopics->At( i ).iTopicData.iNumber, topic );
        topic.iUnreadMessages = 0;
        DoUpdateTopicL( topic, EFalse, i, EFalse );
        DeleteUnsavedMessageStreamIdsL( iTopics->At( i ).iTopicMessagesId );
        }

    UpdateTopicListStreamL( iCurrentTopicList, ETrue );
    TopicStoreL()->CommitL();
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::DeleteUnsavedMessageStreamIdsL
// This function is called when the unsaved messages file has
// been deleted but the Topic messages -stream still contains
// stream ids to deleted message streams.
// All stream ids found in the given topic messages -stream 
// pointing to the unsaved messages file are removed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::DeleteUnsavedMessageStreamIdsL( 
    const TStreamId& aMsgStreamId ) const
    {
    CArrayFixFlat< TStreamId >* streamIds = 
        new ( ELeave ) CArrayFixFlat< TStreamId >( KTypicalNumberOfTopicMessages );
    CleanupStack::PushL( streamIds );

    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), aMsgStreamId ); // on CS

    // Read msg count
    TInt numberOfMsgs( instream.ReadInt16L() );

    // Read max msg count
    TInt maxNumberOfMsgs( instream.ReadInt16L() );

    TInt index( 0 );

    for ( ; index < numberOfMsgs; index++ )
        {
        TBool saved( CbsStreamHelper::ReadBoolL( instream ) );
        TStreamId id( 0 );
        instream >> id;
        if ( saved )
            {
            streamIds->AppendL( id );
            }
        }

    CleanupStack::PopAndDestroy();  // instream

    // Write stream ids of saved messages into the new topic message stream
    // which replaces the stream identified with aMsgStreamId.
    RStoreWriteStream outstream;    
    outstream.ReplaceLC( *TopicStoreL(), aMsgStreamId ); // on CS

    TInt numberOfSavedmessages( streamIds->Count() ); 

    // Number of messages = number of saved messages
    outstream.WriteInt16L( numberOfSavedmessages );

    // Number of saved messages <= number of messages <= maxNumberOfMsgs
    outstream.WriteInt16L( maxNumberOfMsgs );

    for ( index = 0; index < numberOfSavedmessages; index++ )
        {
        // All messages are saved (i.e., permanent)
        CbsStreamHelper::WriteBoolL( outstream, ETrue );
        outstream << streamIds->At( index );
        }

    for ( ; index < maxNumberOfMsgs; index ++ )
        {
        CbsStreamHelper::WriteBoolL( outstream, EFalse );
        outstream << TStreamId( 0 );
        }

    outstream.CommitL();
    CleanupStack::PopAndDestroy(2);  // outstream, streamIds
    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicCountersL
// Resolves the topic position in topic list and uses this information
// to call DoUpdateTopicL.       
// Changes to stores are NOT commited.
// Call to DoUpdateTopicL is not trapped.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicCountersL( 
    const TCbsDbTopic& aTopic,
    const TBool aDeleting )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImpTopicList::UpdateTopicCountersL()");
    
    // Find out the position of the topic in topic list.    
    TInt index( TopicIndexInList( aTopic.iNumber ) );
    if ( index == KErrNotFound )
        {
        User::Leave( KErrNotFound );
        }   

    // DoUpdateTopicL leaves changes uncommited. EFalse for not having to
    // change topic position in topic list.
    DoUpdateTopicL( aTopic, EFalse, index, aDeleting );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImpTopicList::UpdateTopicCountersL()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::CreateStandardTopicListL
// Creates the Standard topic list (topic list no. 0)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::CreateStandardTopicListL()
    {
    // Read standard topic list name from the resource file
    RResourceFile resourceFile;
    CbsUtils::FindAndOpenDefaultResourceFileLC( iFs, resourceFile ); // on CS

    TResourceReader reader;
    reader.SetBuffer( resourceFile.AllocReadLC( R_TEXT_STANDARD_TOPIC_LIST ) ); // on CS
    HBufC* standardTopicListName = reader.ReadHBufCL();
    CleanupStack::PushL( standardTopicListName );

    // Create a new topic list
    CreateNewTopicListL( standardTopicListName->Des() );

    CleanupStack::PopAndDestroy( 3 ); // resourceFile, reader, standardTopicListName
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::LoadTopicsIdsL
// Reads topic IDs of a topic list from the stream.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::LoadTopicsIdsL( 
    const TStreamId& aTopicListStreamId )
    {
    // Open the topic list stream
    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), aTopicListStreamId ); // on CS

    // Skip topic list name
    delete HBufC::NewL( instream, KCbsDbTopicNameLength );

    // Skip default list status
    CbsStreamHelper::ReadBoolL( instream );

    // Skip Topic List number
    instream.ReadInt16L();
    
    // Skip the amount of topics
    TInt topicCount = instream.ReadInt16L();

    // Clear the array
    iTopicIds->ResizeL( 0 );

    TStreamId id( 0 );

    // Load the topic IDs       
    for ( TInt i = 0; i < topicCount; i++ )
        {
        instream >> id;
        iTopicIds->AppendL( id );
        }   
    
    CleanupStack::PopAndDestroy(); // instream
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::LoadTopicsL
// Loads topics of a specified topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::LoadTopicsL( 
    const TStreamId& aTopicListStreamId )
    {
    // Open the topic list stream
    RStoreReadStream instream;
    instream.OpenLC( *TopicStoreL(), aTopicListStreamId ); // on CS

    // Skip topic list name
    delete HBufC::NewL( instream, KCbsDbTopicNameLength );

    // Skip default list status
    CbsStreamHelper::ReadBoolL( instream );

    // Skip Topic List number
    instream.ReadInt16L();
    
    // Skip the amount of topics
    TInt topicCount = instream.ReadInt16L();

    // Clear the arrays
    iTopics->ResizeL( 0 );
    iTopicIds->ResizeL( 0 );

    TStreamId id( 0 );

    // Load the topic IDs 
    TInt i;
    for ( i = 0; i < topicCount; i++ )
        {
        instream >> id;
        iTopicIds->AppendL( id );
        }   
    
    CleanupStack::PopAndDestroy(); // instream
   
    // Load necessary information for each topic
    TInt count = iTopicIds->Count();
    TCbsDbImpTopic topic;
    for ( i = 0; i < count; i++ )
        {
        ReadTopicInformationL( iTopicIds->At( i ), topic );

        if ( topic.iTopicData.iNumber == KIndexTopicNumber )
            {
            HBufC* indexName = ReadIndexTopicNameLC(); // on CS
            topic.iTopicData.iName.Copy( *indexName );
            CleanupStack::PopAndDestroy(); // indexName
            }
        iTopics->AppendL( topic );
        }    
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::UpdateTopicStreamIdsL
// Updates the topic stream IDs.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::UpdateTopicStreamIdsL( 
    const TCbsDbTopicNumber aTopicNumber )
    {
    // Try to find the topic from current topic list
    TInt position( TopicIndexInList( aTopicNumber ) );
    if ( position >= 0 )
        {
        // Remove from the internal cache of this topic list
        iTopics->Delete( position );

        iCurrentTopicList.iTopicCount--;

        // Update the stream
        UpdateTopicListStreamL( iCurrentTopicList, EFalse );
        CommitFilesL();
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImpTopicList::__DbgTestInvariant
// Checks that the object is in a valid state, and panics if it is not.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImpTopicList::__DbgTestInvariant() const
    {
    /*
#if defined(_DEBUG)
#endif
    */
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
