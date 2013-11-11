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
* Description:  Sub-Session class of LBT server
*
*/


#ifndef C_LBTSUBSESSION_H
#define C_LBTSUBSESSION_H

// INCLUDES
#include <e32base.h>

//FORWARD DECLARATIONS
class CLbtServerLogicContainer;

/**
 * Class definition for LBT sub-session
 * @since S60 v4.0
 */
class CLbtSubSession : public CObject
	{
	public:
		/**
		 * Enumeration to distiguish the 
		 * client for which the sub-session is 
		 * created for
		 */
		enum TSubSessionType
			{
			TLbtClientLibrary = 1, // Sub-session for location trigger client library
			TLbtManagementLibrary // Sub-session for location trigger management library
			};
		/**
	     * Instantiates a new object of 
	     * CLbtSubSession
	     *
	     * @param[in] aContainer reference to the 
	     * server logic container object
	     * @return a pointer to instance of CLbtSubSession
	     */
	    static CLbtSubSession* NewLC(CLbtServerLogicContainer& aContainer, TSubSessionType aType);
	    
	    /**
         * C++ destructor
         *
         */
        ~CLbtSubSession();
        
        /**
	     * Handles the servicing of a client request that 
	     * has been passed from the session
	     *
	     * @aMessage The message containing the details of 
	     * the client request
	     *
	     */	     
	    void ServiceL(const RMessage2 &aMessage);
	    
	private:
		
		/**
		 * C++ Default Constuctor
		 *
		 */
		CLbtSubSession(CLbtServerLogicContainer& aContainer);
		
		/**
	     * 2nd phase constuctor for instantiating 
	     * member variables
	     *
	     */
		void ConstructL(TSubSessionType aType);
			
	private: // Data
	
		// Reference to server logic container
		CLbtServerLogicContainer& iContainer;
		
		// The handle number for this session object
		TInt iHandle;
		
		// The client for which this sub-session is created
		TSubSessionType iType;
	};

#endif //C_LBTSUBSESSION_H

// end of file
