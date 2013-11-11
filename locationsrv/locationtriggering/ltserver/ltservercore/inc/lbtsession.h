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
* Description:  Session class declaration for LBT server
*
*/


#ifndef C_LBTSERVERSESSION_H
#define C_LBTSERVERSESSION_H

#include <e32base.h>

//FORWARD DECLARATIONS
class CLbtSubSessionRegistry;
class CLbtServerLogicContainer;

/**
 * Class definition for LBT server session
 * @since S60 v4.0
 *
 */
class CLbtSession : public CSession2
	{
	public:
		/**
	     * Instantiates a new object of 
	     * CLbtSession
	     *
	     * @param[in] aContainer reference to server logic container
	     * which holds the server logic ecom interface
	     * @param[in] aSecureId the secure Id of the client application
	     * which has created this session object.
	     *
	     */
	    static CLbtSession* NewL(CLbtServerLogicContainer& aContainer, const TSecureId& aSecureId);
	    
	    /**
	     * Returns the secure Id of the client which has created 
	     * the session
	     *
	     * @return the secure id of the client which has created 
	     * the session object
	     */ 
	    TSecureId SecureId();
	    	    
    public: // 	    
	    /**
	     * Handles the servicing of a client request that 
	     * has been passed to the server via this session.
	     *
	     * @param[in] aMessage The message containing the details of 
	     * the client request
	     *
	     */	     
	    virtual void ServiceL(const RMessage2 &aMessage);

	    /**
	     * Handles error cases when the ServiceL method leaves.
	     *
	     * @param[in] aMessage The message containing the details of 
	     * the client request
	     * @param[in] aError The error code that the serviceL method 
	     * leaves with
	     */ 
	    virtual void ServiceError(const RMessage2& aMessage,TInt aError);

	    /**
	     * Handles session closure when the client panics.
	     *
	     * @param[in] aMessage The message containing the details of 
	     * the client request
	     * @param[in] aError The error code that the serviceL method 
	     * leaves with
	     */ 
	    virtual void Disconnect(const RMessage2& aMessage);

	private:
		/**
	     * Default C++ Constructor
	     *
	     * @param[in] aContainer reference to server logic container
	     * which holds the server logic ecom interface
	     *
	     */
	    CLbtSession(CLbtServerLogicContainer& aContainer, const TSecureId& aSecureId);
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtSession();
	    
		/**
	     * 2nd phase constuctor for instantiating 
	     * member variables
	     *
	     * @param[in] aSecureId the secure Id of the client application
	     * which has created this session object.
	     */
		void ConstructL();
		
		/**
		 * Create a sub-session object
		 *
		 */
		void CreateSubSessionL(const RMessage2 &aMessage);
		
		/**
		 * Forwards client request to sub-session
		 *
		 * @aMessage[in] The message containing the details of 
	     * the client request
	     *
		 */
		void ForwardToSubSessionL(const RMessage2& aMessage);
		
		/**
		 * Checks if the session is the only
		 * open session of the client
		 * 
		 * @return ETrue if the session object is the only open
		 * server session object 
		 */
		TBool CheckIfOnlySessionOfClient();		
			    
	    /**
	     * Completes the clien message with the specified
	     * complete code
	     *
	     * @param aMessage the message containing the details of 
	     * the client request
	     * @param aCompleteCode the error code of message completion
	     *
	     */
	    void RequestComplete(const RMessage2& aMessage, TInt aCompleteCode);
	
	private: //Data
		
		// Pointer to the sub-session registry
		CLbtSubSessionRegistry* iSubSessionRegistry;
		
		// Reference to the logic container
		CLbtServerLogicContainer& iContainer;
		
		// The secure id of the client which has opened this session
		TSecureId iSecureId;		
	};

#endif //C_LBTSERVERSESSION_H
