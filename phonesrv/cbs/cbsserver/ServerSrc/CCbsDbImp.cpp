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
* Description:  This contains basis for the implementation of the database.
*    
*                Constructs instances of database subclasses CCbsDbImpSettings,
*                CCbsDbImpTopicCollection, CCbsDbTopicList and CCbsDbTopicMessages.
*                On initialization determines whether datafiles already exist.
*                If all of them are present and valid, no special action is taken.
*
*                If some or all datafiles are missing or corrupt, all datafiles
*                are deleted and new files created.
*                Factory default settings, if available, are then loaded from
*                Shared Data and stored in server datafiles. This is done by
*                database subinstances.
*
*/


//  INCLUDE FILES

#include <barsc.h>
#include <e32svr.h>
#include <bautils.h>

#include <cbsserver.rsg>

#include "CbsUtils.h"
#include "CbsServerPanic.h"
#include "CCbsDbImp.H"
#include "CbsDbConstants.h"
#include "CCbsDbImpSettings.H"
#include "CCbsDbImpTopicMessages.h"
#include "CCbsDbImpTopicList.h"
#include "CCbsDbImpTopicCollection.h"
#include "CCbsBackupMonitor.h"
#include "CCbsDiskSpaceMonitor.h"

#include "CbsLogger.h"

// Data file names
_LIT( KCbsTopicsAndMsgsFileName, "cbtopicsmsgs.dat" );
_LIT( KCbsUnsavedMsgsFileName, "cbunsaved.dat" );

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsDbImp::CCbsDbImp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsDbImp::CCbsDbImp()
    {
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsDbImp::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );

    // Create the disk space monitor    
    iDiskSpaceMonitor = CCbsDiskSpaceMonitor::NewL( *this );

    TInt errorCode( KErrNone );
    
#ifdef __SECURE_BACKUP__
    // Use PubSub for monitoring
    iBackupMonitor = CCbsBackupMonitor::NewL( *this );
#else
    // Create a backup observer wrapper. If an error is returned,
    // ignore it and leave iBackupWrapper to NULL.
    TRAP( errorCode, ( iBackupWrapper = CBaBackupSessionWrapper::NewL() ) );
#endif        

    // Try to create the database components. If we couldn't because
    // the disk is almost full, that's alright. We'll try again
    // later.
    TRAP( errorCode, CreateComponentsL() );
    if ( errorCode == KErrDiskFull )
        {
        // Install an observer for the critical level
        WaitForCriticalLevel();
        }
    else
        {
        User::LeaveIfError( errorCode );
        }
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsDbImp* CCbsDbImp::NewL()
    {
    CCbsDbImp* self = new ( ELeave ) CCbsDbImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsDbImp::~CCbsDbImp()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImp::~CCbsDbImp()");
    if ( iDiskSpaceMonitor )
        {
        delete iDiskSpaceMonitor;
        }    
    
    DeleteComponents();

    iFs.Close();
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImp::~CCbsDbImp()");
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::TopicListL
// Returns a reference to the current active topic list. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicList& CCbsDbImp::TopicListL()
    {
    // Check if we have initialized the database succesfully
    if ( !iTopicList )
        {
        User::Leave( KErrDiskFull );
        }

    return *iTopicList;    
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::TopicMessagesL
// Returns a reference to the current topic messages DB object. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicMessages& CCbsDbImp::TopicMessagesL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImp::TopicMessagesL()");
    
    // Check if we have initialized the database succesfully
    if ( !iTopicMessages )
        {
        CBSLOGSTRING("CBSSERVER: CCbsDbImp::TopicMessagesL(): No iTopicMessages instance, leaving with KErrDiskFull...");
        User::Leave( KErrDiskFull );
        }

    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImp::TopicMessagesL(), returning *iTopicMessages.");
    return *iTopicMessages;
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::SettingsL
// Returns a reference to the settings DB object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsDbImpSettings& CCbsDbImp::SettingsL()
    {
    // Check if we have initialized the database succesfully
    if ( !iSettings )
        {
        User::Leave( KErrDiskFull );
        }

    return *iSettings;
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::TopicCollectionL
// Returns a reference to the topic collection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsDbImpTopicCollection& CCbsDbImp::TopicCollectionL()
    {
    // Check if we have initialized the database succesfully
    if ( !iTopicCollection )
        {
        User::Leave( KErrDiskFull );
        }

    return *iTopicCollection;
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::BackupWrapperL
// Returns a reference to the backup wrapper. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CBaBackupSessionWrapper& CCbsDbImp::BackupWrapperL()
    {
    if ( !iBackupWrapper )
        {
        User::Leave( KErrGeneral );
        }

    return *iBackupWrapper;
    }

// ---------------------------------------------------------
// IsInitialized()    
//
// ---------------------------------------------------------
TBool CCbsDbImp::IsInitialized() const
    {
    return iSettings != NULL;
    }

// -----------------------------------------------------------------------------
// CCbsDbImp::CreateComponentsL
// Creates topic list, settings topic messages and topic
// collection instances and loads factory settings if necessary. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsDbImp::CreateComponentsL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImp::CreateComponentsL()");
    
    // Return if we have already been initialized
    if ( IsInitialized() )
        {
        return;
        }    
    
    TBuf<16> topicsFile( KCbsTopicsAndMsgsFileName );
    TBuf<16> messagesFile( KCbsUnsavedMsgsFileName );  
    
    // Delete the old file for unsaved messages 
    CbsUtils::DeleteFileL( iFs, messagesFile );
    CBSLOGSTRING("CBSSERVER: CCbsDbImp::CreateComponentsL(): messagesFile deleted.");

    // Make sure that data file paths exist.
    iFs.MkDirAll( topicsFile ); 
    iFs.MkDirAll( messagesFile ); 
    
    // Create the individual components. If creating any of the components
    // fails, this method leaves and created components are deleted.
    CCbsDbImpSettings* settings = CreateSettingsL();
    CleanupStack::PushL( settings );

    CCbsDbImpTopicList* topicList = CreateTopicListL( topicsFile, messagesFile );
    CleanupStack::PushL( topicList );

    CCbsDbImpTopicMessages* topicMessages = CreateTopicMessagesL( *topicList );
    CleanupStack::PushL( topicMessages );

    CCbsDbImpTopicCollection* topicCollection = CreateTopicCollectionL();
    CleanupStack::PushL( topicCollection );
    

#ifndef __SECURE_BACKUP__

    CBSLOGSTRING("CBSSERVER: CCbsDbImp::CreateComponentsL(): __SECURE_BACKUP__ not defined.");
    
    // Register the file to backup server
    if ( iBackupWrapper )
        {
        iBackupWrapper->RegisterFileL( *topicsFile, *topicList );
        }
#endif

    // We have succesfully created all components. Set them to member
    // variables.
    iTopicList = topicList;
    iSettings = settings;
    iTopicMessages = topicMessages;
    iTopicCollection = topicCollection;

    // Does not transfer ownership.
    iTopicList->SetTopicMessages( iTopicMessages );    

    // If we could create the database, cancel the disk space notify request
    WaitForCriticalLevelCancel();

    // The database components are preserved, but the resource reader and
    // related buffers are deleted.
    CleanupStack::Pop( 4 ); // topicMessages, settings, topicList, topicCollection
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImp::CreateComponentsL()");
    }

// ---------------------------------------------------------
// DeleteComponents()     
//
// ---------------------------------------------------------
void CCbsDbImp::DeleteComponents()
    {
    delete iTopicMessages;
    iTopicMessages = NULL;

    delete iTopicCollection;
    iTopicCollection = NULL;

#ifdef __SECURE_BACKUP__

#else
    // Delete owned objects and deregister files from
    // backup server.
    if ( iBackupWrapper && iSettings )
        {
        //iBackupWrapper->DeregisterFile( iSettings->FileName() );
        }
        
    if ( iBackupWrapper && iTopicList )
        {
        //iBackupWrapper->DeregisterFile( iTopicList->TopicFilename() );
        }
#endif        
        
    delete iSettings;
    iSettings = NULL;
    
    delete iTopicList;
    iTopicList = NULL;

#ifdef __SECURE_BACKUP__
    delete iBackupMonitor;
    iBackupMonitor = NULL;
#else
    delete iBackupWrapper;
    iBackupWrapper = NULL;
#endif    
    }

// ---------------------------------------------------------
// CreateSettingsL()     
//
// ---------------------------------------------------------
CCbsDbImpSettings* CCbsDbImp::CreateSettingsL()
    {
    CCbsDbImpSettings* settings = NULL;
    TRAPD( err, ( settings = CCbsDbImpSettings::NewL( *this ) ) );
    User::LeaveIfError( err );
    return settings;
    }

// ---------------------------------------------------------
// CreateTopicListL()     
//
// ---------------------------------------------------------
CCbsDbImpTopicList* CCbsDbImp::CreateTopicListL( 
    const TDesC& aTopicsFile,
    const TDesC& aMessagesFile )
    {
    // Try to create the topic list.
    CCbsDbImpTopicList* topicList = NULL;
    TRAPD( err, ( topicList = CCbsDbImpTopicList::NewL(
        iFs, aTopicsFile, aMessagesFile, *this ) ) );

    // Handle data file corruption here.
    switch ( err )
        {
        case KErrNone:
            break;

        case KErrCorrupt:
        case KErrEof:
            // The data file was corrupt. Delete it and try again.
            CbsUtils::DeleteFileL( iFs, aTopicsFile );
            CbsUtils::DeleteFileL( iFs, aMessagesFile );
            topicList = CCbsDbImpTopicList::NewL(
                iFs, aTopicsFile, aMessagesFile, *this );
            break;

        default:
            User::Leave( err );
            break;
        }

    return topicList;
    }

// ---------------------------------------------------------
// CreateTopicMessagesL()     
//
// ---------------------------------------------------------
CCbsDbImpTopicMessages* CCbsDbImp::CreateTopicMessagesL( 
    CCbsDbImpTopicList& aTopicList )
    {
    // This cannot leave because of data file corruption.
    return CCbsDbImpTopicMessages::NewL( aTopicList, iFs );
    }


// ---------------------------------------------------------
// CreateTopicCollectionL()     
//
// ---------------------------------------------------------
CCbsDbImpTopicCollection* CCbsDbImp::CreateTopicCollectionL()
    {
    // This does not create a data file.
    return CCbsDbImpTopicCollection::NewL();
    }

// ---------------------------------------------------------
// WaitForCriticalLevelL()     
//
// ---------------------------------------------------------
void CCbsDbImp::WaitForCriticalLevel()
    {
    // We are running low on disk space. Ask for a notification
    // when there is space again.
    iDiskSpaceMonitor->IssueRequest();  
    }


// ---------------------------------------------------------
// WaitForCriticalLevelCancel()     
//
// ---------------------------------------------------------
void CCbsDbImp::WaitForCriticalLevelCancel()
    {
    // Cancel the notification for critical level    
    iDiskSpaceMonitor->Cancel();    
    }

// ---------------------------------------------------------
// ChangeFileLocksL()
// 
// Releases all files for backup/restore, or takes the
// files into use again.
// ---------------------------------------------------------
    
void CCbsDbImp::ChangeFileLocksL( const TCbsBackupRequest& aRequest )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsDbImp::ChangeFileLocksL()");
    
    // Check/change the topiclist/topics/saved messages file
    iTopicList->ChangeFileLockL( aRequest );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsDbImp::ChangeFileLocksL()");
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
