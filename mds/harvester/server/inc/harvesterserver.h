/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester server*
*/

#ifndef __CHARVESTERSERVER__
#define __CHARVESTERSERVER__

#include <e32cmn.h>
#include <apmrec.h>

#include "mdeharvestersession.h"
#include "contextengine.h"

#include "harvesterplugin.h"
#include "composerplugin.h"
#include "harvesterplugininfo.h"
#include "harvesterqueue.h"
#include "harvesterao.h"
#include "pauseobserverao.h"
#include "harvestershutdownobserver.h"

_LIT( KDefaultPanicName, "Harvester Server Panic" );
_LIT( KHarvesterServerName, "HarvesterServer" );
_LIT( KHarvesterServerNameIAD, "__HarvesterServer" );

class CHarvesterServer : public CPolicyServer,
						 public MHarvesterStatusObserver,
						 public MHarvesterShutdownObserver
	{

    public:

	    /**
	    * Constructs a new harvester server implementation.
	    *
	    * @return  Harvester server implementation
	    */
        static CHarvesterServer* NewL();

	    /**
	    * Constructs a new harvester server implementation and leaves the server implementation
	    * in the cleanup stack.
	    *
	    * @return  Harvester server implementation.
	    */
        static CHarvesterServer* NewLC();
        
        /**
        * C++ Constructor.
        * @param aPriority  Server process priority.
        * @param aPolicy  Server policies.
        * @param aType  Indicates what session type server supports .
        */
        CHarvesterServer( TInt aPriority, const TPolicy& aPolicy, TServerType aType );
 		
 		/**
 		* 2nd-phase construction, called by NewLC()
 		*/
 		void ConstructL();
 		
	    /**
	    * Destructor.
	    */
        virtual ~CHarvesterServer();
        
        /**
        * Public method to start the server.
        * @param aNone  Not used.
        * @return Error code.
        */
        static TInt ThreadFunction( TAny* aNone );
        
        /**
        * Method used to panic the client.
        * @param aMessage  Message that is received from client.
        * @param aPanic  Panic code which to raise on client side.
        * @param aPanicDescription  Description of the panic.
        */
        static void PanicClient( const RMessage2& aMessage, TInt aPanic,
        		const TDesC& aPanicDescription = KDefaultPanicName );

        /**
        * Method used to panic the server.
        * @param aPanic  Panic code which to raise.
        * @param aPanicDescription  Description of the panic.
        */
        static void PanicServer( TInt aPanic, const TDesC& aPanicDescription = KDefaultPanicName );

        /**
        * Method used to pause the server.
        * @param aMessage  Message which was received from the client side.
        */
		void Pause( const RMessage2& aMessage );

        /**
        * Method used to resume the operation of the server.
        * @param aMessage  Message which was received from the client side.
        */				
		void Resume( const RMessage2& aMessage );
		
        /**
         * Method used to pause the server.
         */
 		void Pause();

         /**
         * Method used to resume the operation of the server.
         */				
 		void Resume();
 		
        /**
        * Method used to initiate harvesting of a file.
        * @param aMessage  Message which was received from the client side.
        */             
		void HarvestFile( const RMessage2& aMessage );

        /**
        * Method used to initiate harvesting of a file.
        * @param aMessage  Message which was received from the client side.
        */             
        void HarvestFileWithUID( const RMessage2& aMessage );
		
        /**
        * Register a processes's origin value.
        * @param aMessage  Message which was received from the client side.
        */             
        void RegisterProcessOrigin( const RMessage2& aMessage );

        /**
        * Unregister a process origin mapping.
        * @param aMessage  Message which was received from the client side.
        */             
        void UnregisterProcessOrigin( const RMessage2& aMessage );

        /**
         * Register a harvesting complete from server
         * @param aMessage  Message which was received from the client side.
         */ 
        TInt RegisterHarvestComplete( const CHarvesterServerSession& aSession, const RMessage2& aMessage );
        
        /**
         * Unregister a harvesting complete from server
         * @param aSession harvester server session.
         */ 
        TInt UnregisterHarvestComplete( const CHarvesterServerSession& aSession);
        
        /**
         * Register a harvester event from server
         * @param aMessage  Message which was received from the client side.
         */ 
        void RegisterHarvesterEvent( const RMessage2& aMessage );
        
        /**
         * Unregister a harvester event from server
         * @param aMessage  Message which was received from the client side.
         */ 
        void UnregisterHarvesterEvent( const RMessage2& aMessage );

    public: // MHarvesterStatusObserver
		virtual void PauseReady( TInt aError );
		virtual void ResumeReady( TInt aError );
        
        /**
         * Returns the ID of the latest registered event observer.
         * @param aMessage  Message which was received from the client side.
         */
        void GetLastObserverId( const RMessage2& aMessage );
        
    public: // MHarvesterShutdownObserver
        
        void ShutdownNotification();
        
        void RestartNotification();
        
    protected: // Functions from base classes

        /**
        * From CPolicyServer.
        * Handles capability checking for certain server requests.
        * @param aMsg  Message which was received from the client side
        * @param aAction  
        * @param aMissing
        * @result return one of TCustomResult set {EPass, EFail, EAsync}.
        */
        CPolicyServer::TCustomResult CustomSecurityCheckL( 
            const RMessage2 &aMsg, TInt &aAction, TSecurityInfo &aMissing );

        /**
        * From CPolicyServer.
        * Handles failure
        * @param aMsg  Message which was received from the client side
        * @param aAction  
        * @param aMissing
        * @result return one of TCustomResult set {EPass, EFail, EAsync}.
        */
        CPolicyServer::TCustomResult CustomFailureActionL(
            const RMessage2 &aMsg, TInt aAction, const TSecurityInfo &aMissing );

	private:
	   
	    /**
         * Method that initializes the server.
         */
		static void ThreadFunctionL();

	    /**
         * RunError.
         * @param aError  Error which has occurred
         * @return  Error code - how operation should continue
         */
		TInt RunError( TInt aError );
	    
	    /**
         * Method used to create new server session.
         * @param  Mandatory Client/Server framework parameter - not used
         * @param  Mandatory Client/Server framework parameter - not used
         */		
		CSession2* NewSessionL( const TVersion&, const RMessage2& ) const;
		
	private:

	    /**
         * Active Object which runs the harvesting operation
         */		
		CHarvesterAO* iHarvesterAO;
		
		const RMessage2* iMessage;
		
		/**
		 * Pause P&S observer
		 */
		CPauseObserverAO* iPauseObserverAO;
			    
		CHarvesterShutdownObserver* iShutdownObserver;	    	  

    };

#endif
