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
* Description:  ECOM Interface defination for LBT Server Logic. The server 
*				 logic has to implement this interface.
*
*/


#ifndef C_LBTSERVERLOGICBASE_H
#define C_LBTSERVERLOGICBASE_H

// INCLUDE FILES
#include <e32base.h>

/**
 * Class definition for LBT Server Logic ECOM Interface
 * @since S60 v4.0
 *
 */
class CLbtServerLogicBase : public CBase
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
	     * CLbtServerLogicBase
	     *
	     * @param aImplementaionUid uid of the required implementaion 
	     * @return pointer to an instance of CLbtServerLogicBase
	     */
	    IMPORT_C static CLbtServerLogicBase* NewL(TUid aImplementaionUid);
	    
	    /**
	     * Destructor
	     *
	     */
	    IMPORT_C virtual ~CLbtServerLogicBase();
	     
	    /**
	     * Sends the message to server logic. The server
	     * logic should service the client request.
	     *
	     * @param aMessage IPC message to be serviced
	     */
	    IMPORT_C virtual void ServiceL(const RMessage2& aMessage, TSubSessionType aType) = 0;
	    
	    /**
	     * Informs server logic implementation that a sub-session within 
	     * a particular session is being closed. When the sub-session is closed,
	     * all operations related to that sub-session should be cancelled.
	     * 
	     * @param aSubSessionId the id of the sub-session which is being closed
	     * @param aSession pointer to the session to which the sub-session belongs to
	     */
	    IMPORT_C virtual void HandleSubSessionClosure(const TInt aSubSessionId, const CSession2* aSession) = 0;
	    
	    /**
	     * Informs server logic implementation that all the sessions of
	     * client with a particular secure id has been closed. 
	     * 
	     * @param aSecureId the secure id of the client whose sessions
	     * are being closed
	     */
	    IMPORT_C virtual void HandleSessionClosureL(const TSecureId aSecureId) = 0;
	
	protected:
		/**
		 * Default C++ constructor
		 *
		 */
		IMPORT_C	CLbtServerLogicBase();

	private: // Data
		/**
	     * identifer returned by ECom to identify an instance to the framework.
     	 */
		TUid iDtor_ID_Key;		

	};

#endif // C_LBTSERVERLOGICBASE_H
