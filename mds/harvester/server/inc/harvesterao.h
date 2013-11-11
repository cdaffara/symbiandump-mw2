/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Thread which actually performs the harvesting of the files*
*/


#ifndef __CHARVESTERAO_H__
#define __CHARVESTERAO_H__

#include <e32cmn.h>
#include <e32base.h>
#include <badesca.h>

#include <rlocationobjectmanipulator.h>

#include "contextengine.h"
#include "mdeharvestersession.h"

#include "harvesterplugin.h"
#include "composerplugin.h"
#include "harvesterplugininfo.h"
#include "harvesterqueue.h"
#include "reharvesterao.h"
#include "harvesterpluginfactory.h"
#include "unmounthandlerao.h"
#include "harvesteroomao.h"
#include "backupsubscriber.h"
#include "harvestermediaidutil.h"
#include "harvesterdiskspaceobserver.h"


// forward declarations
class CHarvesterBlacklist;
class MProcessOriginMapperInterface;
class COnDemandAO;
class CMdeObjectHandler;
class CHarvesterData;
class CHarvesterEventManager;
class CRestoreWatcher;
class CHarvesterServerSession;
struct TScanItem;

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester AO.
 */
class CHarvesterAoPropertyDefs : public CBase
	{
	public:
		// Common property definitions
		CMdEPropertyDef* iCreationDatePropertyDef;
		CMdEPropertyDef* iLastModifiedDatePropertyDef;
		CMdEPropertyDef* iSizePropertyDef;
		CMdEPropertyDef* iOriginPropertyDef;
		CMdEPropertyDef* iItemTypePropertyDef;
		CMdEPropertyDef* iTitlePropertyDef;
		CMdEPropertyDef* iDefaultFolderPropertyDef;
		
		// Media property definitions
		CMdEPropertyDef* iPreinstalledPropertyDef;

	private:
		CHarvesterAoPropertyDefs();
	
		void ConstructL(CMdEObjectDef& aObjectDef);

	public:	
        static CHarvesterAoPropertyDefs* NewL();
        void SetByObjectDefL( CMdEObjectDef& aObjectDef );
	};

/**
 * MHarvesterStatusObserver
 */	

class MHarvesterStatusObserver
	{
	public:
		virtual void PauseReady( TInt aError ) = 0;
		virtual void ResumeReady( TInt aError ) = 0;
	};

/**
* Harvester server active object.
*
* @lib harvesterblacklist.lib
*/

class CHarvesterAO : public CActive,
                     public MHarvesterPluginObserver, 
                     public MContextInitializationObserver,
                     public MContextSnapshotObserver,
                     public MMdESessionObserver,
                     public MBackupRestoreObserver,
                     public MUnmountObserver,
                     public MHarvesterOomObserver,
                     public MMdSHarvesterDiskSpaceObserver
	{
    public:
    
        enum TRequest
        	{
        	ERequestIdle,
        	ERequestHarvest,
        	ERequestPause,
        	ERequestResume,
        	ERequestContainerPlaceholder
        	};
        	
        /**
         * Constructs a new harvesting thread
         *
         * @return  Harvester thread implementation
         */
        static CHarvesterAO* NewL();

        /**
         * Constructs a new harvesting thread and leaves the server implementation
         * in the cleanup stack.
         *
         * @return  Harvester thread implementation
         */
        static CHarvesterAO* NewLC();

        /**
         * C++ Destructor.
         */
        virtual ~CHarvesterAO();
        		
        /**
         * Derived from MHarvesterPluginObserver - used to indicate when harvesting of a file is done
         * @param aHD  harvester data type
         */
        void HarvestingCompleted( CHarvesterData* aHD );

        /**
         * Derived from MContextInitializationObserver - for checking Context Engine initialization
         * @param aErrorCode  Indicates whether Context Engine was successfully initialized
         */
        void ContextInitializationStatus( TInt aErrorCode );
        	
        /**
         * Derived from MMdESessionObserver - for checking MdE initialization status
         * @param aSession  MdE Session which was opened
         * @param aError  Error code from the opening
         */
        void HandleSessionOpened( CMdESession& aSession, TInt aError );

        /**
         * Derived from MMdESessionObserver - for checking MdE session errors
         * @param aSession  MdE Session which was opened
         * @param aError  Error which has occurred
         */
        void HandleSessionError( CMdESession& aSession, TInt aError );

        /**
         * Method used to pause monitor plugins
         */
        void PauseMonitoring();

        /**
         * Method used to resume monitor plugins
         */       
        void ResumeMonitoring();

        /**
         * Standard RunL-method.
         * From CActive.
         */       
        void RunL();

        /**
         * From CActive.
         */       
        void DoCancel();

        /**
         * From CActive.
         * @param aError  Leave error code.
         * @return Always KErrNone.
         */       
        TInt RunError( TInt aError );

        /**
         * Set next request (=state) of this active object.
         * @param aRequest  State enumeration.
         */       
        void SetNextRequest( TRequest aRequest );

        /**
         * Checks if harvester server is paused.
         * @return Truth value.
         */       
        TBool IsServerPaused();
        
        /**
         * Checks whether connection to MDE has been established
         * @return ETrue if connection to MDE has been made otherwise EFalse
         */
         TBool IsConnectedToMde();        
        
        
        /**
         * Starts a file harvest.
         * @param aMessage  A message containing file info (URI etc)
         */       
        void HarvestFile( const RMessage2& aMessage );
        
        /**
         * Starts a file harvest.
         * @param aMessage  A message containing file info (URI etc)
         */       
        void HarvestFileWithUID( const RMessage2& aMessage );

        /**
         * Register an origin value for a process.
         * @param aMessage  A message containing process id and origin.
         */       
        void RegisterProcessOrigin( const RMessage2& aMessage );

        /**
         * Unregister an origin value for a process.
         * @param aMessage  A message containing process id.
         */       
        void UnregisterProcessOrigin( const RMessage2& aMessage );

        /**
         * This method is called by Context framework after context snapshot is taken
         * @param aHD  Harvester data type (if ErrorCode() == KErrNone - snapshot taken ok)
         */
         void ContextSnapshotStatus( CHarvesterData* aHD );

		/**
         * Register a harvesting complete from server
         * @param aMessage  Message which was received from the client side.
         * @param aSession  harvester server session.
         */ 
        TInt RegisterHarvestComplete( const CHarvesterServerSession& aSession, const RMessage2& aMessage );
        
        /**
         * Unregister a harvesting complete from server
         * @param aMessage  Message which was received from the client side.
         */ 
        TInt UnregisterHarvestComplete( const CHarvesterServerSession& aSession );
        
        /**
         * Register a harvester event from server
         * @param aMessage  Message which was received from the client side.
         */ 
        void RegisterHarvesterEvent( const RMessage2& aMessage );
        
        /**
	     * Unregister a harvester event from server
	     * @param aMessage  Message which was received from the client side.
	     */ 
	    void UnregisterHarvesterEvent( const RMessage2& aMessage );
	    
        void GetLastObserverId( const RMessage2& aMessage );

	// from MBackupRestoreObserver

        /**
         * From MBackupRestoreObserver.
         * Called by CBlacklistBackupSubscriberAO when
         * Backup&Restore is backing up or restoring.
         */
        void BackupRestoreStart();

        /**
         * From MBackupRestoreObserver.
         * Called by CBlacklistBackupSubscriberAO when
         * Backup&Restore has finished backup or restore.
         */
        void BackupRestoreReady();

        // From MMdSHarvesterDiskSpaceObserver
        void HandleDiskSpaceNotificationL( TDiskSpaceDirection aCrossDirection );
        
        /** */  	
        void HandleUnmount( TUint32 aMediaId );
        
        void SetHarvesterStatusObserver( MHarvesterStatusObserver* aObserver );
        
        /**
         * From MHarvesterOomObserver
         */
        void MemoryLow();
        
        /**
         * From MHarvesterOomObserver
         */
        void MemoryGood();
        
        void RemoveBlacklistedFile( CHarvesterData* aItem );
        
        CHarvesterAO::TRequest GetHarvesterAORunState();
		
    private:
        
        TBool CheckForCameraItem( CHarvesterData* aHd, TDes& aObjectDef );
        
        void AddDefaultFolderDataL( CMdEObject* aObject );
        
	private:
		
	struct THarvestFileRequest
		{
		const CHarvesterServerSession& iSession;
		RMessage2 iMessage;
			
		THarvestFileRequest( const CHarvesterServerSession& aSession,
				const RMessage2& aMessage ) :
	       		iSession( aSession ), 
	       		iMessage( aMessage )
	       		{}
		};
	
	/**
         * C++ Constructor, called by NewLC()
         */
        CHarvesterAO();

        /**
         * 2nd-phase construction, called by NewLC()
         */
        void ConstructL(); 		

        /**
         * Method used to load monitor plugins
         */        
        void LoadMonitorPluginsL();

        /**
         * Method used to delete monitor plugins
         */           
        void DeleteMonitorPlugins();

        /**
         * Method used to start monitor plugins
         */  
        void StartMonitoring();

        /**
         * Method used to stop monitor plugins
         */          
        void StopMonitoring();

        /**
         * Method used to start composer plugins
         */  
        void StartComposersL();

        /**
         * Method used to stop composer plugins
         */  		
        void StopComposers();

        /**
         * Method used to delete composer plugins
         */  
        void DeleteComposers();

        /**
         * Method used to check whether composer plugins are active
         * @return  Whether all compsosers are ready
         */  				
        TBool IsComposingReady();

        /**
         * Method used to read one harvester item from queue
         */	    
        void ReadItemFromQueueL();
        
        /**
         * Method used to handle placeholder object to database
         */	    
        void HandlePlaceholdersL( TBool aCheck );

        /**
         * Method used to check file extension and harvest file
         * @param aFile  File to be harvester 
         * @param aEventType  Harveting type
         * @param aTakeSnapshot  Whether context snapshot should be added to metadata
         */		
        void CheckFileExtensionAndHarvestL( CHarvesterData* aHD );

        /**
         * Method used to pause the operation of the Harvesting thread
         * @return  Whether pausing of the harvesting thread succeeded 
         */	
        TInt PauseHarvester();

        /**
         * Method used to resume the operation of the Harvesting thread
         */			
        void ResumeHarvesterL();
        
        /**
         * Signals the client observer for file harvesting was completed
         * @param aClientId, RMessage2.Identity
         * @param aFile, Harvested file name
         * @param aErr, KErrNone, or system wide error code if exists
         */ 
        void HarvestCompleted( TUid aClientId, const TDesC& aUri, TInt aErr );
        
        /**
         * Scan ROM drive (Z) in first phone boot
         */
        void BootRomScanL();
        
        /**
         * Scan system drive (C) after partial restore
         */
        void BootPartialRestoreScanL();
        
        static TBool IsDescInArray(const TPtrC& aSearch, const RPointerArray<HBufC>& aArray);
        
        /**
         * Method used to scan paths for file to be harvest. This method 
         * removes paths from aPaths after scanning.
         * 
         * @param aScanPaths scan paths
         * @param aIgnorePaths ignored paths
         */
        void BootScanL( RPointerArray<TScanItem>& aScanItems,
        		        const RPointerArray<HBufC>& aIgnorePaths,
        		        TBool aCheckDrive );
        
        /**
         * Checks if there are any items unharvested in harvester plugins.
         * @return  Whether or not there are any files to be harvested in plugin queues.
         */
        TBool UnharvestedItemsLeftInPlugins();

        void PreallocateNamespaceL( CMdENamespaceDef& aNamespaceDef );
        
        void StartThumbAGDaemon();
        
 	private:
 	
        /**
         * Pointer to Context Engine implementation
         */	
        CContextEngine* iCtxEngine;

        /**
         * Pointer to Metadata Engine session
         */			
        CMdESession* iMdESession;
        
        /**
         * Pointer to Harvester Metadata Engine session
         */			
        CMdEHarvesterSession* iMdEHarvesterSession;

        /**
         * Harvester server internal event queue
         */	
        CHarvesterQueue* iQueue;

        /**
         * Blacklister of files that have failed harvesting.
         */
        CHarvesterBlacklist* iBlacklist;

        /**
         * Publish & Subscribe subscriber for Backup & Restore.
         * Notifies harvester server when something happens.
         */
        CBackupSubscriber* iBackupSubscriber;

        /**
         * This array contains pointers to monitor plugins
         */		
        RPointerArray<CMonitorPlugin> iMonitorPluginArray;
        
        /**
         * This array contains pointers to composer plugins
         */		
        RPointerArray<CComposerPlugin> iComposerPluginArray;

        /**
         * Indicates whether server is paused or not
         */			
        TBool iServerPaused;		

        /**
         * Indicator to show which task will be next to do
         */
        TRequest iNextRequest;
        
        /** */
		RArray<THarvestFileRequest> iHarvestFileMessages;
        /** Re-harvester */
        CReHarvesterAO* iReHarvester;
        
        /** Flag to signal if mde session is properly initialized. */
        TBool iMdeSessionInitialized;

        /** Flag to signal if context engine is properly initialized. */
        TBool iContextEngineInitialized;
        
        /** Connection to file server (CheckForMoveEventL) */
        RFs iFs;

        /** */
        CHarvesterPluginFactory* iHarvesterPluginFactory;

		/** */
        RPointerArray<CHarvesterData> iPHArray;
		
		/** */
        RPointerArray<CHarvesterData> iReadyPHArray;
        
        /** */
        RPointerArray<CHarvesterData> iContainerPHArray;
        
		/** */
        RPointerArray<CHarvesterData> iTempReadyPHArray;
                
        /**
         * Pointer to process origin mapper for registering mappings.
         * Implemented by file monitor plugin.
         */
        MProcessOriginMapperInterface* iProcessOriginMapper;
        
		/** */
        COnDemandAO* iOnDemandAO;
        
		/** */
        CMdeObjectHandler* iMdeObjectHandler;
		
        /** */
        CUnmountHandlerAO* iUnmountHandlerAO;
        
        /** */
        CHarvesterEventManager* iHarvesterEventManager;
                
        MHarvesterStatusObserver* iHarvesterStatusObserver;
        
        CRestoreWatcher* iRestoreWatcher;
        
        CHarvesterOomAO* iHarvesterOomAO;
        
        // not own
        CHarvesterMediaIdUtil* iMediaIdUtil;
        
        CHarvesterAoPropertyDefs* iPropDefs;
        
        // Own.
        CDesCArray* iCameraExtensionArray;

        /**
        * Notifier for situations where free disk space runs out. Own
        */
        CMdSHarvesterDiskspaceObserverAO* iDiskFullNotifier;
        
        TBool iRamFull;
        
        TBool iDiskFull;
        
        TBool iManualPauseEnabled;
        
        TBool iFastHarvestNeeded;
        
        TBool iHarvestingPlaceholders;
        
        HBufC* iPhoneImagesPath;
        HBufC* iMmcImagesPath;
        
        HBufC* iPhoneVideosPath;
        HBufC* iMmcVideosPath;
        
        HBufC* iPhoneSoundsPath;
        HBufC* iMmcSoundsPath;
        
        TBool iUnmountDetected;
        TBool iUnmountHandlingOngoing;
        TBool iPriorityInterruptDetected;
        
        RLocationObjectManipulator iLocManipulator;
        TBool iLocManipulatorConnected;
	};
	
#endif //__CHARVESTERAO_H__
