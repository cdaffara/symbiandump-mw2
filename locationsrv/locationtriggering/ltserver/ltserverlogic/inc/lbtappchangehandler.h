/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This header file describes the class reposnible for handling
*              : removal of applications
*
*/


#ifndef C_LBTAPPCHANGEHANDLER_H
#define C_LBTAPPCHANGEHANDLER_H

#include <e32base.h>
#include <apgnotif.h>
#include "lbttriggerchangeobserver.h"

// FORWARD DECLARATION
class TApaAppInfo;
class RApaLsSession;
class CApaAppListNotifier;
class CLbtContainer;
class CLbtContainerTriggerEntry;
class CLbtCleanupHandler;
class MLbtServerLogicTriggerChangeObserver;
struct TLbtAppInfo;

/**
 *
 */
class CLbtAppChangeHandler : public CActive,
							 public MApaAppListServObserver,
							 public MLbtContainerChangeEventObserver
							 
	{	
	public:
		/**
	     * Constructs a new instance of App Change handler.
	     *
	     * @return The new instance of App Change handler object.
	     */
		static CLbtAppChangeHandler* NewL(CLbtContainer& aContainer, CLbtCleanupHandler& iCleanupHandler,
		                                  MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver);		
		
		/**
	     * Destructor.
	     */
		~CLbtAppChangeHandler();
		
		/**
		 * Start listening to application un-installation events
		 *
		 */
		void StartListeningL();
		
		/**
		 * Stops listening to application un-installation events
		 *
		 */
		void StopListening();
		
	private: // New functions
		/**
	     * C++ constructor
	     */
		CLbtAppChangeHandler(CLbtContainer& aContainer, CLbtCleanupHandler& iCleanupHandler,
		                     MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver);
		
		/**
	     * Symbian 2nd phase of construction
	     */
		void ConstructL();
		
		/**
		 * Handles removal of an application that resided on the MMC. The function 
		 * checks if same MMC card is still inserted. If it still inserted it implies
		 * that the application has been un-installed and hence takes appropriate action.
		 * If the MMC is not present in its slot then it means that the MMC card is 
		 * removed for the time being and may be inserted again, based on which it takes
		 * appropriate action
		 *
		 * @param aAppInfo information of the application removed
		 */
		void HandleMMCApplicationRemovalL();
		
		/**
		 * Handles un-installation of an application.
		 *
		 * @param aAppInfo information of the application un-installed
		 */
		void HandleApplicationUninstallationL();
		
		
		/**
		 * Checks if MMC is present in its slot
		 * 
		 * @return ETrue if MMC is present EFlase otherwise
		 */
		TBool IsMemoryCardPresent();
		
		/**
         * Get Memory card character
         */
        void MemoryCardChar();        
		
		/**
		 * Finds the a particular element in the array. The search is based on the 
		 * UID in the application info. If the application is not present then the
		 * function return KErrNotFound
		 *
		 * @param aArray the array in which the element needs to be searched
		 * @param aElement the element which needs to be searched for
		 */ 
		TInt FindElementInArray(RArray<TLbtAppInfo>& aArray, TLbtAppInfo& aElement);
		
		/**
		 * Handles list notification event when notification is for addition of an
		 * application
		 */
		void HandleAppListAdditionEvent(RArray<TLbtAppInfo>& aNewAppList);
		
		/**
		 * Handles list notification event when notification is for removal of an
		 * application		 
		 */
		void HandleAppListRemovalEvent(RArray<TLbtAppInfo>& aNewAppList);
		
		
		/**
		 * Validates the triggers of the applications present in iAppList
		 */
		void SetValidTriggersOfApplicationsL();
		
		
		
	public: // from MLbtContainerChangeEventObserver
	    void TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                                  TLbtTriggerEventMask aEventMask,
                                  TLbtContainerChangedAreaType aAreaType,
                                  RArray<TUid>& aManagerUids );

	private: // From CActive
		
		void DoCancel();
		
		void RunL();
		
		TInt RunError(TInt aError);
		
	
	private: /// from MApaAppListServObserver
		/**
	     * Handles notification from app list observer when ever an application is 
	     * installed or uninstalled
	     *
	     */
		void HandleAppListEvent(TInt aEvent);
		
		/**
		 * Lists triggers for which apps iAppList in are the 
		 * startup process
		 */
		void ListTriggersOfStartupProcessL();
		
		/**
		 * Deletes triggers for which apps iAppList in are the 
		 * startup process
		 */
		void DeleteTriggersOfStartupProcessL();

	private: // data members
	
		
		// Reference to the container
		CLbtContainer& iContainer;
		
		// Reference to cleanup handler
        CLbtCleanupHandler& iCleanupHandler;
		        
		// Reference to MLbtServerLogicTriggerChangeObserver
		MLbtServerLogicTriggerChangeObserver& iTriggerChangeObserver;
		
		// Session with the application architecture framework
        RApaLsSession iAppArcSession;
        
        // Pointer to application scanner
        CApaAppListNotifier* iAppListNotifier;
		        
		// Array to hold the currently installed applications on the system
        RArray<TLbtAppInfo> iCurrentInstalledApps;
		        
		// The async operation in progress.
		TInt iOperation;
		
		// Array of pointers to container trigger entries
		RPointerArray<CLbtContainerTriggerEntry> iTriggerEntries;
		
		// Array to store the list of app removed
		RArray<TLbtAppInfo> iAppList;
	
		// Operation id for calls to container. Needed for any cancellation
		TInt iOpId;
		
		// External Memory card drive character
		TBuf<2> iMMCDriveChar; // drive and colon
	};

#endif // C_LBTAPPREMOVALHANDLER_H