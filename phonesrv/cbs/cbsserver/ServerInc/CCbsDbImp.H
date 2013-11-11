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
* Description:  This file contains the header file of the CCbsDbImp class
*
*/




#ifndef     CCBSDBIMP_H
#define     CCBSDBIMP_H

// INCLUDES
#include <f32file.h>

// CONSTANTS
enum TCbsBackupRequest
    {
    ECbsNoBackup,
    ECbsBackup,
    ECbsRestore,
    ECbsBackupNotDefined
    };

// FORWARD DECLARATIONS
class CCbsDbImpSettings;
class CCbsDbImpTopicList;
class CCbsDbImpTopicCollection;
class CCbsDbImpTopicMessages;
class CBaBackupSessionWrapper;
class CCbsBackupMonitor;
class CCbsDiskSpaceMonitor;

//  CLASS DECLARATION 

/**
*   Basis for the implementation of the database.
*
*   Constructs instances of database subclasses CCbsDbImpSettings,
*   CCbsDbImpTopicCollection, CCbsDbTopicList and CCbsDbTopicMessages.
*   On initialization determines whether datafiles already exist.
*   If all of them are present and valid, no special action is taken.
*
*   If some or all datafiles are missing or corrupt, all datafiles
*   are deleted and new files created.
*   Factory default settings, if available, are then loaded from
*   Shared Data and stored in server datafiles. This is done by
*   database subinstances.
*/
class CCbsDbImp : public CBase
    {
    public:        // New functions

        /**
        *   Creates an instance of the class.
        */
        static CCbsDbImp* NewL();

        /**
         *   The destructor.
         */
        ~CCbsDbImp();
    
    public:

        /**
        *   Returns a reference to the current active topic list. 
        *   
        *   Using that instance the clients of the database can manage the 
        *   topic list such as add and delete topics.
        *
        *   @return reference to the topic list.
        */
        CCbsDbImpTopicList& TopicListL();
    
        /**
        *   Returns a reference to the current topic messages DB object.
        *
        *   @return reference to the topic messages.
        */
        CCbsDbImpTopicMessages& TopicMessagesL();

        /**
        *   Returns a reference to the settings DB object.
        *    
        *   @return reference to the settings.
        */
        CCbsDbImpSettings& SettingsL();

        /**
        *   Returns a reference to the topic collection. 
        *   
        *   @return reference to the topic collection.
        */
        CCbsDbImpTopicCollection& TopicCollectionL();

        /**
        *   Returns a reference to the backup wrapper. 
        *   
        *   @return reference to the backup wrapper.
        */
        CBaBackupSessionWrapper& BackupWrapperL();

        /**
        *   Checks if the database is initialized.
        *
        *   @return     ETrue if database has been initialized.
        */
        TBool IsInitialized() const;

        /**
        *   Registers for a notification for critical level.
        */
        void WaitForCriticalLevel();

        /**
        *   Deregisters from critical level notification.
        */
        void WaitForCriticalLevelCancel();
        
        /**
        *   Changes file lock states as requested.
        */
        void ChangeFileLocksL( const TCbsBackupRequest& aRequest );
        
        /**
        *   Constructs DB components.
        *
        */
        void CreateComponentsL();

    private:

        /**
        *   Constructor.
        */
        CCbsDbImp();

        /**
        *   The method creates the member variables and so finalizes 
        *   the construction. It will leave if the construction fails.
        */
        void ConstructL();

        /**
        *   Constructs CCbsDbImpSettings.
        *
        *   @param  aFileName               Name of the settings file.
        *   @return                         A ready-made CCbsDbImpSettings object.
        */
        CCbsDbImpSettings* CreateSettingsL();

        /**
        *   Constructs CCbsDbImpTopicList.
        *
        *   @param  aTopicsFile             Name of the topics file.
        *   @param  aMessagesFile           Name of the messages file.
        *   @return                         A ready-made CCbsDbImpTopicList object.
        */
        CCbsDbImpTopicList* CreateTopicListL( const TDesC& aTopicsFile,
                                              const TDesC& aMessagesFile );

        /**
        *   Constructs CCbsDbImpTopicMessages.
        *
        *   @param  aTopicList              The CCbsDbImpTopicList to link to.
        *   @return                         A ready-made CCbsDbImpTopicMessages object.
        */
        CCbsDbImpTopicMessages* CreateTopicMessagesL( CCbsDbImpTopicList& aTopicList );

        /**
        *   Constructs CCbsDbImpTopicCollection.
        *
        *   @param  aTopicList              The CCbsDbImpTopicList to link to.
        *   @return                         A ready-made CCbsDbImpTopicCollection object.
        */
        CCbsDbImpTopicCollection* CreateTopicCollectionL();

        /**
        *   Free DB components.
        */
        void DeleteComponents();        
        
    private:
        // Owns: DB settings object.
        CCbsDbImpSettings* iSettings;

        // Owns: DB topic list object.
        CCbsDbImpTopicList* iTopicList;

        // Owns: DB topic collection object. 
        CCbsDbImpTopicCollection* iTopicCollection;

        // Owns: DB topic messages object.
        CCbsDbImpTopicMessages* iTopicMessages;

        // File server session shared by all DB objects. 
        RFs iFs;

        // Owns: The backup wrapper
        CBaBackupSessionWrapper* iBackupWrapper;
        
        // Owns: The backup monitor, uses PubSub
        CCbsBackupMonitor* iBackupMonitor;
        
        // Owns: Disk space monitor, uses PubSub
        CCbsDiskSpaceMonitor* iDiskSpaceMonitor;

    };

#endif      //  CCBSDBIMP_H   
            
// End of File
