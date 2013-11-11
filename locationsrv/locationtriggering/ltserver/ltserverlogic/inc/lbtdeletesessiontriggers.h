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
* Description:  Active Object operation for deletiong session triggers.
*
*/


#ifndef C_LBTDELETESESSIONTRIGGERS_H
#define C_LBTDELETESESSIONTRIGGERS_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CLbtContainer;
class CLbtNotificationMap;
class CLbtContainerUpdateFilter;
class MLbtDeleteSessionTriggerObserver;


class CLbtDeleteSessionTriggers:public CActive
	{
	public:
	    
	    /**
	     * Instantiates a new object of
	     * CLbtDeleteSessionTriggers
	     *
         * @param[in] aContainer reference to the container which holds
	     * the reference to the logic processor
	     * @param[in] aNotificationMap notification map in which all 
	     * the IPC requests are stored by server logic. This notification
	     * map is used to retreive the appropriate message for notification
         * @return a pointer to an instance of CLbtDeleteSessionTriggers
	     *
	     */
         static CLbtDeleteSessionTriggers* NewL(MLbtDeleteSessionTriggerObserver& aObserver,
         										CLbtContainer& aContainer,CLbtNotificationMap& aNotificationMap);
	   
        /**
	     * Destructor
	     *
	     */
	     ~CLbtDeleteSessionTriggers();
            
        /**
         * Deletes session triggers asynchronously.
         * @param[in] aContainerFilter This parameter specifies the triggers which need to
         * be deleted.
         */
         void DeleteSessionTriggers(CLbtContainerUpdateFilter* aContainerFilter);
		

	protected: // From CActive
    	 void RunL();
    	 void DoCancel();
	
	private:
		
	    /**
	     * Default C++ Constructor
	     *
	     * @param[in] aContainer reference to the container which holds
	     * the reference to the logic processor
	     * @param[in] aNotificationMap notification map in which all 
	     * the IPC requests are stored by server logic. This notification
	     * map is used to retreive the appropriate message for notification
	     */	
	     CLbtDeleteSessionTriggers(MLbtDeleteSessionTriggerObserver& aObserver,
	     							CLbtContainer& aContainer,CLbtNotificationMap& aNotificationMap);
		
	    /**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
	     void ConstructL();
		
	
	private:
		// Reference to event observer
		MLbtDeleteSessionTriggerObserver& iObserver;
		//Reference to location triggering container
		CLbtContainer& iContainer;
		//Reference to Notification map
		CLbtNotificationMap& iNotificationMap;
		// Integer that identifies AO operation
		TInt iAOIdentificationNum;
		
	};

#endif // C_LBTDELETESESSIONTRIGGERS_H

