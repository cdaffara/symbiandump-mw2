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
* Description:  Class definition of CLbtSystemStateListener
*
*/


#ifndef C_LBTSYSTEMSTATELISETER_H
#define C_LBTSYSTEMSTATELISETER_H

//FORWARD DECLARATION
class MLbtStartupObserver;

#include <e32base.h>
#include <Symbian_OS.hrh>

#ifndef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    #include <e32property.h>       
#else
    #include <ssm/ssmstateawaresession.h>
#endif
/**
 * Class definition for LBT System state listner
 * @since S60 v4.0
 */
class CLbtSystemStateListener : public CActive
	{
	
	public:	
		/**
	     * Instantiates a new object of 
	     * CLbtSystemStateListener
	     *
	     * @return a pointer to an instance of
	     * CLbtSystemStateListener
	     */
	    static CLbtSystemStateListener* NewL(MLbtStartupObserver& aObserver);
	    
	    /**
	     * Starts listening to system state
	     * events
	     *
	     */
	    void StartToListenL();
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtSystemStateListener();
	    	    
	private:
		
		/**
		 * Enum to determine which event's 
		 * notification is outstanding
		 */
		enum 
			{
			ENoneOutstanding, // no notification outstanding
			ESSEventOutstanding, // system state event outstanding
			ETimerEventOutstanding // timer event outstanding
			};
	
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
		/**
	     * Default C++ Constructor
	     *
	     */
	    CLbtSystemStateListener(MLbtStartupObserver& aObserver);
	    
	private: // From CActive
	
		/**
	     * Request completion event handler
	     *
	     */	
	    void RunL();
	    
	    /**
	     * Invoked by the active object FW when 
	     * any outstanding request is cancelled
	     *
	     */	
	    void DoCancel();

	    /**
	     * Handles errors if RunL leaves
	     * This override is necessary because the base implementation
	     * panics the thread with E32User-CBase 47.
	     *
	     */	
	    TInt RunError(TInt aError);
	    
	private: // Data Members
	
		
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	    /**
	     * 
	     */
	    RSsmStateAwareSession iStateAwareSession;
#else
	    /**
         * Handle to the startup property object
         *
         */ 
        RProperty       iStartupObserver;
        
#endif
	    
	    /**
	     * Pointer to the system state observer
	     *
	     */
	    MLbtStartupObserver&	iObserver;
	};

#endif // C_LBTSYSTEMSTATELISETER_H

