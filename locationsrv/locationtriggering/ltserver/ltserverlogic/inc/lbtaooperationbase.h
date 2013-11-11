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
* Description:  Abstract base class for active Object operation 
*				 for serving server logic requests
*
*/


#ifndef C_LBTAOOPEATIONBASE_H
#define C_LBTAOOPEATIONBASE_H

// INCLUDE FILES
#include <e32base.h>
#include "lbttriggerchangeobserver.h"

// FORWARD DECLARATION

class MLbtAOObserver;
class RMessage2;
class CLbtContainer;

/**
 * Abstract base class declaration of active object operation
 * to service client requests comming from server core.
 * The IPC messages are provided to this class to service the 
 * request asynchronously
 *
 * @since S60 v4.0
 */
class CLbtAOOperationBase : public CActive,
                            public MLbtContainerChangeEventObserver
	{
	public:
		/**
	     * Destructor
	     *
	     */
		virtual ~CLbtAOOperationBase();
		
		/**
	     * Starts the AO operation of serving the request.
	     * All derived class has to implement this object	     
	     *
	     */ 
		virtual void StartOperationL() = 0;
		
		/**
		 * Returns the secure ID of the client that has 
		 * requested the service
		 *
		 * @return the secure id of a client
		 */
		const TSecureId& GetSecureId();
		
		/**
		 * Returns the session pointer for which the AO
		 * is processing the request
		 *
		 * @return session pointer
		 */
		const CSession2* GetSession();
		
		/**
		 * Returns the service id which is being processed
		 * by this client 
		 *
		 * @return the service id
		 */
		TInt GetFunction();
		
		/**
		 * Returns the client type for which the AO
		 * operation object has been created.
		 *
		 * @return the client type
		 */
		TInt GetClientType();

		/**
		 * Returns the Handle to the SubSession that requested
		 * this service.
		 *
		 * @return the sub session handle.
		 */
		TInt GetSubSessionHandle();
		
		/**
		 * Get manager Uid array associated with the trigger
		 */
		const RArray<TUid>& GetManagerArray(); 
	
	public:// From MLbtContainerChangeEventObserver
	    void TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                                  TLbtTriggerEventMask aEventMask,
                                  TLbtContainerChangedAreaType aAreaType,
                                  RArray<TUid>& aManagerUids );
	
	protected: // From CActive
	    TInt RunError( TInt aError );
	    	
	protected:
		/**
	     * Default C++ Constructor
	     *
	     */
		CLbtAOOperationBase(MLbtAOObserver& aObserver, 
                            const RMessage2& aMessage,
                            CLbtContainer& aContainer,
                            TInt aClientType);
		
	protected: // Data
		
		// Reference to the event observer
		MLbtAOObserver&	iObserver;
		
		// Reference to the IPC message pack from the client
		 RMessage2 iMessage;
		
		// Reference to Location Triggering entry
		CLbtContainer& iContainer;
		
		// The client type for which this ao object was created to service
		TInt iClientType;
		
		// Secure id of the client for which the AO object has been created.
		// Extracted from the IPC message and saved, since it is required
		// even after the RMessage2 has been completed
		TSecureId iSecureId;
		
		// Session pointer to the client session
		CSession2* iSession;
		
		// Service id being served by this AO
		TInt iFunction;
		// Integer that identifies AO operation
		TInt iAOIdentificationNum;
		
		// Manager uid array associated with the operation
		RArray<TUid> iManagerUidArray;
	};


#endif // C_LBTAOOPEATIONBASE_H