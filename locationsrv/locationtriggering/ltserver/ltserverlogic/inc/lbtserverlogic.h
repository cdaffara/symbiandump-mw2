/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  LBT Server Logic implementation
*
*/


#ifndef C_LBTSERVERLOGIC_H
#define C_LBTSERVERLOGIC_H

// INCLUDE FILES
#include "lbtserverlogicbase.h" // interface 
#include "lbtaoobserver.h"
#include "lbttriggeringsupervisionobserver.h" // stratergy observer
#include "lbtbackuprestoreobserver.h" // For Backup and Restore observer
#include "lbtstrategytriggeringsystemstatus.h"
#include "lbtdeletesessiontriggers.h"
#include "lbtdeletesessiontriggerobserver.h"
#include "lbttriggerchangeobserver.h"
#include <lbttriggeringsystemmanagementsettings.h>
#include <lbttriggerchangeevent.h>
#include "lbtserverlogictriggerchangeobserver.h"

// FORWARD DECLARATIONS
class CLbtAOOperationBase;
class CLbtStratergyContainer;
class CLbtBackupRestoreListener;
class CLbtContainer;
class CLbtSettingsManager;
class CLbtNotificationMap;
class CLbtTriggerFireHandler;
class CLbtAppChangeHandler;
class CLbtCleanupHandler;
class CLbtSimChangeHandler;

/**
 * Class declaration for server logic implementation
 * 
 * @since S60 v4.0
 */
class CLbtServerLogic : public CLbtServerLogicBase, 
						public MLbtAOObserver,	// AOO operation observer
						public MLbtTriggeringSupervisionObserver,	// Stratergy observer
						public MLbtBackupRestoreObserver, // Backup and Restore observer
						public MLbtDeleteSessionTriggerObserver, // Delete session trigger observer
						public MLbtServerLogicTriggerChangeObserver
	{
	public:	
		/**
	     * Instantiates a new object of 
	     * CLbtServerLogic
	     *
	     * @return a pointer to an instance of 
	     * CLbtServerLogic
	     */
	    static CLbtServerLogic* NewL();
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtServerLogic();
	
	public: // new functions
		
		/**
	     * Handles the servicing of a client request that 
	     * has been passed from server core session
	     *
	     * @aMessage The message containing the details of 
	     * the client request. The resposibility of completing
	     * the message is upto this object
	     *
	     */	     
	    void ServiceL(const RMessage2 &aMessage, TSubSessionType aType);
	    
	    /**
	     * Loads stratergy plugin if not already loaded
	     * After loading it starts the supervision process.
	     * In case strategy is already loaded and there are no
	     * valid and enabled triggers in the system then strategy
	     * plugin is un-loaded.	     
	     */
	    void LoadOrUnloadStrategyPluginL();
	    
	    /**
	     * Method is invoked when a session with a particular session
	     * id is being closed. One closure of this session all the 
	     * data pertaining to that session like session triggers,
	     * IPC messages has to be deleted
	     *
	     * @param aSessionId the id of the session which is being closed
	     */
	    void HandleSubSessionClosure(const TInt aSessionId, const CSession2* aSession);
	    
	    /**
	     * Informs server logic implementation that all the sessions of
	     * client with a particular secure id has been closed. 
	     * 
	     * @param aSecureId the secure id of the client whose sessions
	     * are being closed
	     */
	    void HandleSessionClosureL(const TSecureId aSecureId);
	
	public: // from MLbtAOObserver
	    void HandleDeleteSessionTriggersClosureL(CLbtDeleteSessionTriggers*
	    												 aDeleteSessionTriggers);
	
	public: // from MLbtAOObserver
		void HandleOperationClosureL(CLbtAOOperationBase* aOperation, TInt aStatus);
	
	public: // from MLbtTriggeringSupervisionObserver
		void TriggerFiredL( CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, 
        					const TPositionInfo& aPosInfo );
        					
		void SetTriggeringSupervisionDynamicInfo( 
   							const TLbtStrategySupervisionDynamicInfo& aStatus );
   			
   		void GetTriggeringSupervisionSettings( 
 							TLbtStrategyTriggeringSupervisionSettings& aSettings );
	
	public: // from MLbtBackupRestoreObserver
		void HandleBackupRestoreOperationL(TBRState aState);

	public: // from	MLbtServerLogicTriggerChangeObserver
	    void HandleTriggersChange( RArray<TLbtTriggerId>& aTriggerIds,
                                   RArray<TUid>& aManagerUids,
                                   TLbtTriggerEventMask aEventMask  );
	private:
	
		/**
	     * Default C++ Constructor
	     *
	     */
	    CLbtServerLogic();
	    
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
	    /**
	     * Cancels all outstanding operation objects and destroys it
	     *
	     */
	    void CancelAllOperations();
	    
	    /**
	     * Removes the meesage which matches the notification cancellation
	     * message received. The message in the notification map which matches 
	     * the session id and the service id of the received message is removed.
	     *
	     * @param aMessage the message to be removed from the message notification map
	     * @param aServiceId the service id for the cancellation of which this
	     *        message has been received
	     */
	    TInt RemoveFromNotificationMap( const RMessage2& aMessage, TInt aServiceId );
	    
	    /**
	     * Checks if the received message is not a duplicate. If it is a duplicate
	     * request then the client message is completed with ELbtDuplicateRequest
	     *
	     * @param aMessage message to be inserted into the message notification map
	     * 
	     */
	    void InsertIntoNotificationMapL(const RMessage2& aMessage, TSubSessionType aType);
	    
	    /**
	     * Makes capability checks and other Notify change related
	     * sanity checks on the message. Returns appropriate error
	     * code.
	     *
	     * @return KErrNone if trigger can be created and any one of the error 
	     * codes otherwise
	     */
	     TInt ValidateChangeNotification(const RMessage2& aMessage);
	     
	     /**
	     * Makes capability checks for general LBT operation
	     * on the message. Returns appropriate error
	     * code.
	     *
	     * @return KErrNone if trigger can be created and any one of the error 
	     * codes otherwise
	     */
	     TInt ValidateGeneralOperation(const RMessage2& aMessage);
	     
	    /**
	     * Makes capability checks and other create trigger related
	     * sanity checks on the message. Returns appropriate error
	     * code.
	     *
	     * @return KErrNone if trigger can be created and any one of the error 
	     * codes otherwise
	     */
		TInt ValidateCreateTriggerMessage(const RMessage2& aMessage);
		/**
	     * Makes capability checks and other get triggering setting related
	     * sanity checks on the message. Returns appropriate error
	     * code.
	     *
	     * @return KErrNone if trigger can be created and any one of the error 
	     * codes otherwise
	     */
		TInt ValidateGetTriggeringSysSetting(const RMessage2& aMessage);
		
		/**
	     * Makes capability checks and other set triggering setting related
	     * sanity checks on the message. Returns appropriate error
	     * code.
	     *
	     * @return KErrNone if trigger can be created and any one of the error 
	     * codes otherwise
	     */
		TInt ValidateSetTriggeringSetting(const RMessage2& aMessage);
		
		/**
	     * Makes capability checks and other Get triggering status related
	     * sanity checks on the message. Returns appropriate error
	     * code.
	     *
	     * @return KErrNone if trigger can be created and any one of the error 
	     * codes otherwise
	     */
		TInt ValidateGetTriggeringSysStatus(const RMessage2& aMessage);
		
		/**
		 * Checks if the provided service id is for a asynchronous
		 * notification request
		 * 
		 * @param[in] aServiceId the provided service id 
		 * @return ETrue if the provided service id is for a 
		 * asynchronous service request. EFalse otherwise		 
		 */
		TBool IsNotificationRequest(TInt aServiceId);
		
		/**
		 * Handles all asynchronous notifications except system settings 
		 * change notification. System settings change notification is valid
		 * only when management library changes the settings. Hence that 
		 * notification will be taken care by the active object handling 
		 * all management library requests
		 *
		 * @param aMessage the message to be removed from the message notification map
		 */
		void HandleNotificationOperationsL(CLbtAOOperationBase* aOperation, TInt aServiceId);
	    
		/**
		 * Change the Settings used by the triggering engine.
		 * These settings values tweak the behaviour of triggering engine
		 * Only when management library changes the settings. 
		 *
		 * @param aSettings the new settings values for the triggering engine.
		 */
        void SetTriggeringSystemSettings( 
            TLbtTriggeringSystemManagementSettings& aSettings,
            TLbtManagementSettingsMask aSettingsMask );

        TInt GetTriggeringSystemSettings( const RMessage2& aMessage, TSubSessionType aClientType );
        
        void GetTriggeringSysStatus( TLbtTriggeringSystemStatus& aSystemStatus );
        
        void HandleSingleNotificationOperationsL(
            CLbtAOOperationBase* aOperation, 
            TInt aServiceId);

        void HandleMultipleNotificationOperationsL(
            CLbtAOOperationBase* aOperation, 
            TInt aServiceId);

        void SetNotificationEventType( 
            CLbtAOOperationBase* aOperation, 
            TLbtTriggerChangeEvent& aEvent );
       void NotifyTriggeringSystemSettingChange(TLbtManagementSettingsMask aSettingsMask );    
       
       void NotifyTriggeringSystemStatusChange();
	private: // Data
	    // Handle to RProperty server
	    RProperty    iProperty;
	
		// Array to maintain the list of ao operation running in server logic
		RPointerArray<CLbtAOOperationBase> iAOOArray;
		
		// Pointer to stratergy container
		CLbtStratergyContainer* iStratergyContainer;
		
		// Pointer to backup and restore event listener
		CLbtBackupRestoreListener* iBackupRestoreListener;
		
		// Pointer to location triggering container
		CLbtContainer* iContainer;
		
		// Pointer to the triggering settings manager
		CLbtSettingsManager* iSettingsManager;
		
		// Notification map to store all notification messages
		CLbtNotificationMap* iNotificationMap;
		
		// Trigger fire handler
		CLbtTriggerFireHandler* iFireHandler;
		
		// Array that maintains delete session triggers AO.
		RPointerArray<CLbtDeleteSessionTriggers> iDeleteSessionTriggersArray;
		// Strategy dynamic info structure.
		TLbtStrategySupervisionDynamicInfo iStrategyDynamicInfo;
		
		// Pointer to application change handler
		CLbtAppChangeHandler* iAppChangeHandler;
		// Pointer to sim change handler
		CLbtSimChangeHandler* iSimChangeHandler;
		
		// Pointer to cleanup handler
		CLbtCleanupHandler* iCleanupHandler;
	};

#endif // C_LBTSERVERLOGIC_H