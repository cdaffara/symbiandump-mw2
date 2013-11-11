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
* Description:  LBT Server Core implementation
*
*/


#include <e32base.h>

#ifndef C_LBTSERVERCORE_H
#define C_LBTSERVERCORE_H

// FORWARD DECLARATIONS
class CLbtSystemStateListener;
class CLbtServerLogicContainer;
class CLbtDiskSpaceListner;

// INLCUDES
#include "lbtstartupobserver.h"
#include "lbtdiskspaceobserver.h"

/**
 * Class definition for LBT Server Core implementation
 * @since S60 v4.0
 *
 */
class CLbtServerCore : public CServer2, 
					   public MLbtStartupObserver,
					   public MLbtDiskSpaceObserver
	{
	
	public:		
		/**
         * Destructor
         *
         */
        ~CLbtServerCore();
        
		/**
	     * Instantiates a new object of 
	     * CLbtServerCore
	     *
	     */
	    static CLbtServerCore* NewLC();

		/**	     
	     * Starts the LBT Server to listen
	     * to incomming requests
	     *
	     * @param[in] aName the name of the server which the 
	     * client will use to connect
	     */		
		void StartL(const TDesC &aName);
		
		/**
		 * Returns the number of open session of client whose
		 * secure id is provided
		 *
		 * @param[in] aSecureId the secure id of the client 
		 * whose number of open session has to be retreived
		 */ 
		TInt NumberOfClientSessions(TSecureId aSecureId);
		
	private:
		/**
         * C++ constructor.
         *         
         * @param aServerType Sharable or non-sharable sessions.
         *
         */
         CLbtServerCore( TServerType aServerType = ESharableSessions );
         
        /**
         * 2nd phase constuctor for instantiating member variables
         *
         */
        void ConstructL();
                
        /**
         * From CServer2. Creates a server-side session object.
         *
         * @param aVersion The version information supplied by the client. 
	     * @param aMessage Represents the details of the client request that 
	     * is requesting the creation of the session.
	     *
	     * @return A pointer to the newly created server-side session object
         */
        CSession2* NewSessionL(const TVersion& aVersion, 
        					   const RMessage2& aMessage) const;
	
	public:// from MLbtStartupObserver        	   
		void HandleStartupCompletionL( );
	
    
    public:// from MLbtDiskSpaceObserver
        void HandleDiskSpaceChange();
        	
	private: // Private Data
		
		// Instance to the system state listner		
		CLbtSystemStateListener*	iListener;
	    
	    // Object to load and hold server logic interface
	    CLbtServerLogicContainer*	iContainer;
	    
	    // Pointer to disk space listner
	    CLbtDiskSpaceListner* iDiskSpaceListner;
	    
	};

#endif // C_LBTSERVERCORE_H

