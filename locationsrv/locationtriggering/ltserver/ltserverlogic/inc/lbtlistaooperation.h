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
* Description:  Active Object operation for serving server logic requests
*				 for listing of triggers.
*
*/


#ifndef C_LBTLISTAOOPERATION_H
#define C_LBTLISTAOOPERATION_H

// INCLUDE FILES
#include <e32base.h>
#include "lbtaooperationbase.h"

// FORWARD DECLARATIONS
class MLbtAOObserver;
class CLbtContainer;
class CLbtContainerTriggerEntry;
class CLbtTriggerFilterByAttribute;

/**
 * Class declaration of active object operation
 * to service client requests. This object services
 * requests which require listing of triggers.
 * The IPC messages are provided to
 * this class to service the request asynchronously
 *
 * @since S60 v4.0
 */
class CLbtListAOOperation : public CLbtAOOperationBase
	{
	enum TState
	    {
	    EStateGetFiredTriggers,	    
	    EStateUpdateFiredState,
	    EStateServiceMessageReceived,
	    EStateUpdateCompleted,
	    EStateNone
	    };
	public:
	
		/**
	     * Instantiates a new object of 
	     * CLbtListAOOperation
	     *
	     * @param[in] aObserver reference to the active
	     * object observer
	     * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request
	     * @param aContainer reference to the container which holds
	     * the reference to the logic processor
	     */
	    static CLbtListAOOperation* NewL( MLbtAOObserver& aObserver,
		    							  const RMessage2& aMessage,
		    							  CLbtContainer& aContainer,
		    							  TInt aClientType );
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtListAOOperation();
	    
	private:
	
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
		/**
	     * Default C++ Constructor
	     *
	     * @param[in] aObserver reference to the active object observer
	     * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request
	     * @param aContainer reference to the container which holds
	     * the reference to the logic processor
	     */
	    CLbtListAOOperation( MLbtAOObserver& aObserver, 
		    				 const RMessage2& aMessage,
		    				 CLbtContainer& aContainer,
		    				 TInt aClientType );
	
	public: // From CLbtAOOperationBase
	
	    /**
	     * Starts the AO operation of serving the request	     
	     *
	     */ 
		void StartOperationL();
		
		/**
		 * Services the second part of the operation. This is a
		 * specialized service for this AO Object since servicing
		 * list operations requires two IPC messages. On the first
		 * IPC the AO object gets the required data from the 
		 * container and sends the size of the required buffer 
		 * from the client side to return back the data. It also 
		 * holds the data (like number of triggers). On receiving the
		 * second IPC message the data saved by the AO object is
		 * populated into the client buffer.
		 * 
		 * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request	     
		 */ 
		void ServiceMessageL(const RMessage2 aMessage);
		
		/**
	     * Cancels the list operation.	     
	     *
	     */ 
	     void CancelListOperation();
		
	private: // new functions
		void ServiceGetFiredTriggersL();
		    
    protected: // From CActive
    	void RunL();
    	TInt RunError(TInt aError);
    	void DoCancel();

	private: // Data
		
		// The pointer array which will be populated by the container
		// based on the search
		RPointerArray<CLbtContainerTriggerEntry> iTriggerArray;
		
		// Trigger Buffer
		CBufFlat* iTriggerBuffer;
		
		TState iState;
	};

#endif // C_LBTLISTAOOPERATION_H