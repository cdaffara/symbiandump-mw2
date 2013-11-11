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
* Description:  Defines the notification map data structure used by Location 
*				 Triggering Server Logic to store notification request messages
*
*/


#ifndef LBTNOTIFICATIONMAP_H
#define LBTNOTIFICATIONMAP_H

//  INCLUDE FILES
#include <e32cmn.h>
#include "lbtserverlogicbase.h"

// FORWARD DECLARATIONS
class RMessage2;


// CLASS DECLARATION

/**
 * Defination of notification map data structure used 
 * by Location Triggering Server Logic to store IPC
 * requests by client for asynchronous notifications
 *  
 */
class CLbtNotificationMap : public CBase
	{
	public:
		/**
	     * Instantiates a new object of 
	     * CLbtNotificationMap
	     *	     
	     */
	    static CLbtNotificationMap* NewL();
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtNotificationMap();
	    
	    /**
	     * Inserts the IPC message into the notification map.
	     * The IPC messages is inserted such that the notification
	     * map is always is sorted in ascending order based on the 
	     * secure id of the client.
	  	 *
	  	 * @param aMessage[in] the IPC message to be inserted into 
	  	 * notification map
	     */
	    void InsertL(
	        const RMessage2& aMessage, 
	        CLbtServerLogicBase::TSubSessionType aType);

	    /**
	     * Retreives the IPC message which matches all the supplied data.
	     * The retreived IPC message is removed from the notification
	     * map
	     * 
	     * @param[out] aMessage the retreived IPC message is returned
	     * through this parameter
	     * @param[in] aSubSessionHandle the handle number of the sub-session
	     * @param[in] aSecureId the secure id of the client whose IPC message
	     * has to be retreived
	     * @param[in] aServiceId the service id of the IPC message which matches
	     * @param[in] aSession pointer to the session of the IPC message which
	     * should be retreived id.
	     */
	    TInt Retreive(RMessage2& aMessage, 
	    			  TUint32 aSubSessionHandle,
	    			  TSecureId aSecureId,
	    			  TInt aServiceId,
	    			  CSession2* aSession);
		
		/**
	     * Retreives the IPC message which matches all the supplied data.
	     * The retreived IPC message is removed from the notification
	     * map
	     *
	     * @param[out] aMessage the retreived IPC message is returned
	     * through this parameter
	     * @param[in] aSecureId the secure id of the client whose IPC message
	     * has to be retreived
	     * @param[in] aServiceId the service id of the IPC message which matches
	     */
		TInt Retreive(RMessage2& aMessage,	    			  
	    			  TSecureId aSecureId,
	    			  TInt aServiceId);
		
		/**
         * Retreives the IPC message which matches all the supplied data.
         * The retreived IPC message is removed from the notification
         * map
         *
         * @param[out] aMessage the retreived IPC message is returned
         * through this parameter
         * @param[in] aSecureId the secure id of the client whose IPC message
         * has to be retreived
         * @param[in] aServiceId the service id of the IPC message which matches
         */
		TInt Retreive(RMessage2& aMessage, 
		              TInt aSubSessionHandle,  
		              TSecureId aSecureId);    
		/**
	     * Retreives the IPC message which matches all the supplied data.
	     * The retreived IPC message is removed from the notification
	     * map
	     *
	     * @param[out] aMessage the retreived IPC message is returned
	     * through this parameter
	     * @param[in] aServiceId the service id of the IPC message which matches
	     * @param[in] aType the subsession type of the client whose IPC message
	     * has to be retreived
	     */
        TInt Retreive(RMessage2& aMessage, 
				      TInt aServiceId,
				      CLbtServerLogicBase::TSubSessionType aType);
		/**
		 * Completes and removes all the IPC messages in the notification map which 
		 * matches the supplied data of the parameters
		 *
		 * @param[in] aSession pointer to the session of the IPC message which
	     * should be completed
	     * @param[in] aSubSessionHandle the handle number of the sub-session
	     * @param[in] aErrorCode the error code with which the messages have
	     * to be completed
		 */
		void CompleteMessagesOfSubsession(const CSession2* aSession, const TInt aSubSessionHandle, TInt aErrorCode);
	    
	    /**
	     * Removes all the IPC messages registered by the particular client.
	     *
	     * @param[in] aSecureId the secure id of the client whose IPC message
	     * has to be retreived
	     */
	    void RemoveAllClientMessages(TSecureId aSecureId);
	    
	    /**
	     * Resets the notification map and removes all the IPC messages. All
	     * the IPC messages are completed with the supplied error code
	     *
	     * @param[in] aErrorCode the error code with which the IPC messages 
	     * needs to be completed
	     */
	    void Reset(TInt aErrorCode);	    	    
	    
    public:
        struct TLbtNotificationReq
            {
            RMessage2 iMsg;
            CLbtServerLogicBase::TSubSessionType iType; 
            };

	private:
	
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();		
    
	private: // Data Members		
		
		// Array to store all the IPC messages
		RArray<TLbtNotificationReq> iMessageArray;
	};


#endif // LBTNOTIFICATIONMAP_H

// end of file
