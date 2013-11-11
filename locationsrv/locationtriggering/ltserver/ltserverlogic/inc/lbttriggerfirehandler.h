/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation to fire triggers, both session and startup
*
*/


#ifndef LBTTRIGGERFIREHANDLER_H
#define LBTTRIGGERFIREHANDLER_H

// INCLUDE FILES
#include <e32base.h>
#include "lbtcommon.h"

// FORWARD DECLARATIONS
class CLbtContainer;
class CLbtContainerTriggerEntry;
class CLbtNotificationMap;

/**
 * Class declaration for  trigger fire handler.
 * Once provided with a trigger to be fired, this
 * class fire's the trigger asynchrnously
 *
 * @since S60 v4.0
 */
class CLbtTriggerFireHandler : public CActive
	{
	public:	
		/**
	     * Instantiates a new object of 
	     * CLbtTriggerFireHandler
	     *
	     * @param[in] aNotificationMap notification map in which all 
	     * the IPC requests are stored by server logic. This notification
	     * map is used to retreive the appropriate message for notification
	     * @param[in] aId the id of the trigger which has to be fired
	     * @param[in] aPosInfo the position information where the 
	     * trigger fired
	     * @param[in] aContainer reference to location triggering
	     * container
	     * @return a pointer to an instance of CLbtTriggerFireHandler
	     *
	     */
	    static CLbtTriggerFireHandler* NewL( CLbtNotificationMap& aNotificationMap,
	    									 CLbtContainer& aContainer );

		
		/**
	     * Destructor
	     *
	     */
	    ~CLbtTriggerFireHandler();		
		
	public: // new functions	
		/**
		 * Fires the trigger asynchronously.
		 * After firing the trigger, the object
		 * destroys iteself
		 * 
		 */
		void FireTriggerL(TLbtTriggerFireInfo aFireInfo);
		
	private: // Helper methods
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
		/**
	     * Default C++ Constructor
	     *
	     * @param[in] aMessageArray the message array in which all 
	     * the IPC requests are stored by server logic. This array
	     * will be iterated through to get the appropriate message
	     * @param[in] aId the id of the trigger which has to be fired
	     * @param[in] aPosInfo the position information where the 
	     * trigger fired
	     * @param[in] aContainer reference to location triggering
	     * container
	     * @return a pointer to an instance of CLbtTriggerFireHandler
	     */
		CLbtTriggerFireHandler(CLbtNotificationMap& aNotificationMap,
							   CLbtContainer& aContainer);
		
	    /**
	     * Fetches the information from Container for the next trigger
	     * in the trigger fired Queue.
	     */
	    void FireNextTrigger();

	    /**
	     * Notify the client, about trigger fire if there is any pending
	     * notification request.
	     */
        void NotifyTriggerFired();
	
	    /**
	     * Start the trigger handling process for the fired trigger.
	     */
	    void StartTriggerHandlingProcess(CLbtTriggerEntry* aTriggerEntry);

	protected: // From CActive
    	void RunL();
    	void DoCancel();
    	TInt RunError( TInt aError ); 
    
    private: // Data
        
    	// Reference to the notification map
    	CLbtNotificationMap& iNotificationMap;
    	
    	// Reference to the Location Triggering Container
    	CLbtContainer& iContainer;
    	
    	// Trigger array that will be populated by the container
    	RPointerArray<CLbtContainerTriggerEntry> iTriggerArray;
    	
    	// Array to hold the fire info of triggers
    	RArray<TLbtTriggerFireInfo> iFireInfoArray;
    	
    	// Variable to hold the internal state of the handler
    	TInt iState;
    	// Integer that identifies AO operation
		TInt iAOIdentificationNum;
	};

#endif //LBTTRIGGERFIREHANDLER_H

