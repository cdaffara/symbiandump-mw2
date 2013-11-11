/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is Metadata engine server file*
*/

#ifndef __MDSSERVER_H__
#define __MDSSERVER_H__

// INCLUDE FILES
#include <e32base.h>
#include <e32cmn.h>

#include "mdccommon.pan"

#include "mdsdiskspacenotifier.h"
#include "mdebackuprestoreobserver.h"
#include "mdsshutdownobserver.h"

// FORWARD DECLARATIONS
class CMdSObjectLockList;
class CMdSNotifier;
class CMdSMaintenanceEngine;
class CMdSManipulationEngine;
class CMdSSqLiteConnection;
class CMdEBackupRestoreWatcherAO;
class CMdsSchema;
class CMdCSerializationBuffer;

// CLASS DECLARATION

/**
* CMdSServer.
*  An instance of class CMdSServer is the main server class
*/
class CMdSServer : public CPolicyServer, public MMdSDiskSpaceNotifierObserver, 
                   public MMdEBackupRestoreObserver,
                   public MMDSShutdownObserver
                   
    {
    public : // From MMdSDiskSpaceNotifierObserver
    	void HandleDiskSpaceNotificationL(TDiskSpaceDirection aCrossDirection);

		void HandleDiskSpaceError(TInt aError);
		
	public: // From MMdEBackupRestoreObserver
		// Called when backup & restore watcher notices starting back up.
		void HandleBackup();
		// Called when backup & restore watcher notices starting restore.
		void HandleRestore();
		// Called when backup or watcher is completed and normal operation continues.
		void ResumeOperation();	
		
	public: // MMDSShutdownObserver
	        
	        void ShutdownNotification();
	        
	        void RestartNotification();
    
    public : // Constructors and destructors

	    /**
	    * Constructs a new metadata server implementation.
	    *
	    * @return  metadata server implementation
	    */
        static CMdSServer* NewL();

	    /**
	    * Constructs a new metadata server implementation and leaves the server implementation
	    * in the cleanup stack.
	    *
	    * @return  metadata server implementation
	    */
        static CMdSServer* NewLC();

	    /**
	    * Destructor.
	    */
        virtual ~CMdSServer();

    public: // New functions

        /**
        * ThreadFunction.
        * Main function for the server thread.
        * @param aStarted A semaphore to be flagged when server
        *                 has been started.
        * @return Error code.
        */
        static TInt ThreadFunction( TAny* aStarted );

        /**
        * Reference to the schema
        */
        CMdsSchema& Schema();

        /**
        * Get lock list
        */
        CMdSObjectLockList& LockList();

        /**
        * Get notifier
        */
        CMdSNotifier& Notifier();

        /**
        * Get maintenance engine
        */
        
        CMdSMaintenanceEngine& Maintenance();
        
        /**
        * Get manipulator engine
        */
        
        CMdSManipulationEngine& Manipulate();

        /**
        * PanicClient.
        * Panics the client.
        * @param aMessage The message channel to the client.
        * @param aReason The reason code for the panic.
        */
        static void PanicClient( const RMessage2& aMessage, TInt aReason );
        
        TBool DiskFull() const;
        
        TBool BackupOrRestoreRunning() const;
        
        TBool ShutdownInProgress() const;

        TInt SetHarvestingPrioritizationChunkL( const RMessagePtr2 aMessage, TInt aParam );

        TInt AddHarvestingPrioritizationObserver( RMessagePtr2 aMessage );

        TInt CancelHarvestingPrioritizationObserver();

        TBool ReserveSpaceHarvestingPrioritizationUri( const TDesC16& aUri );

        TBool StartAddingHarvestingPrioritizationUrisL();
        
        TBool AddHarvestingPrioritizationUriL( TDesC16& aUri );

        void NotifyHarvestingPrioritizationObserver( TInt aStatus ) const;

        /** 
         * ResetDBL
         * resets DB, should be called only in DEBUG build
         */
        TInt ResetDBL();

    protected: // Functions from base classes

        /**
        * From CActive, RunError.
        * Processes any errors.
        * @param aError The leave code reported.
        * @result return KErrNone if leave is handled.
        */
        TInt RunError( TInt aError );
        
        /**
        * From CPolicyServer.
        * Handles capability checking for certain server requests.
        * @result return one of TCustomResult set {EPass, EFail, EAsync}.
        */
        CPolicyServer::TCustomResult CustomSecurityCheckL( 
                                                const RMessage2 &aMsg,
                                                TInt &aAction,
                                                TSecurityInfo &aMissing);

        /**
        * From CPolicyServer.
        * Handles xxxx.
        * @result return one of TCustomResult set {EPass, EFail, EAsync}.
        */
        CPolicyServer::TCustomResult CustomFailureActionL(
                                                const RMessage2 &aMsg,
                                                TInt aAction,
                                                const TSecurityInfo &aMissing);


    private: // Constructors and destructors

        /**
         * CMdEServer.
         * C++ default constructor.
         * @param aPriority priority for this thread.
         */
        CMdSServer( TInt aPriority );

        /**
         * ConstructL.
         * 2nd phase constructor.
         */
        void ConstructL();

        /**
         * InitializeL
         * Do part of internal resources initializations
         * used together with DeInitialize to ResetDB functionality
         */
        void InitializeL();

        /**
         * DeInitializeL
         * Do part of internal resources deinitializations
         * used together with Initialize to ResetDB functionality
         */
        void DeInitializeL();
        
        void CheckInitSriptL();
        
    private: // New methods

        /**
        * PanicServer.
        * Panics the server.
        * @param aPanic The panic code.
        */
        static void PanicServer( TMetadataPanic aPanic );

        /**
        * ThreadFunctionL.
        * Second stage startup for the server thread.
        */
        static void ThreadFunctionL();

        friend TInt E32Main();

    private: // Functions from base classes

        /**
        * From CServer, NewSessionL.
        * Creates a time server session.
        * @param aVersion The client version.
        * @return Pointer to new session.
        */
        CSession2* NewSessionL( const TVersion& aVersion, const RMessage2& aMessage ) const;

    private: // Data

        /**
        * iSessionCount, the number of session owned by this server.
        */
        TInt iSessionCount;
        
        /**
        * class for installing and maintaining the database
        */
        CMdSMaintenanceEngine* iMaintenance;

		/**
		* Default DB connection for default namespace.
		*/
		CMdSSqLiteConnection* iDefaultDBConnection;

        /**
        * The internal schema
        */
        CMdsSchema* iSchema;

        /**
        * Server status. If other than KErrNone, prevents client sessions.
        */
        TInt iServerErrorStatus;
        
        /**
        * Locked object list.
        */
       	CMdSObjectLockList* iLockList;
       
		/**
		* Notifier engine
		*/
		CMdSNotifier* iNotifier;

        /**
        * Object adding, removing and modification.
        * upper proxy layer, provides validation and such.
        */
        CMdSManipulationEngine* iManipulate;
        
        /**
        * Garbage collector notifier for situations where free disk space is getting low.
        */
        CMdSDiskSpaceNotifierAO* iDiskSpaceGarbageCollectorNotifier;
        
        /**
        * Notifier for situations where free disk space runs out.
        */
        CMdSDiskSpaceNotifierAO* iDiskFullNotifier;
        
        CMdEBackupRestoreWatcherAO* iBURWatcher;
        
        TBool iBackupOrRestoreRunning;
        
        RChunk iHarvestingPrioritizationChunk;
        CMdCSerializationBuffer* iHarvestingPrioritizationSerializationBuffer;
        TUint32 iHarvestingPrioritizationLimit;
        TInt32 iHarvestingPrioritizationUriCount;
        TBool iHarvestingPrioritizationLocked; // adding of new URIs allowed

        TInt32 iHarvestingPrioritizationBufferUriCount;
        TUint32 iHarvestingPrioritizationBufferUriOffset;

        RMessagePtr2 iHarvestingPrioritizationObserver;
        
        CMDSShutdownObserver* iShutdownObserver;
        
        TBool iShutdown;
        
        RThread iClientThread;
    };

#endif // __MDSSERVER_H__

// End of File
