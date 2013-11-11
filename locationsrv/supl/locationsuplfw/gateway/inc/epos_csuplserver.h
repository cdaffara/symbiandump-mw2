/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Main server class
*
*/




#ifndef __CSuplServer_H__
#define __CSuplServer_H__

//  INCLUDES
#include <e32base.h>
#include <connect/sbdefs.h>

#include "epos_mnetinitiatedobserver.h"
#include "epos_mconnectobserver.h"
#include "epos_msuplecomeventobserver.h"

// FORWARD DECLARATIONS
class CSession2;
class CSuplServerDelayedShutdown;
class CSuplSessionManager;
class CSuplNetInitiatedSession;
class CSuplNetInitiatedRegistry;
class CSuplServerStartupRequest;
class CSuplServerDeInitRequest;

// CONSTANTS
const TUint KSuplServerShutdownTimer = 2000000;
const TUid KProtocolHandlerInterfaceUid = {0x102073CB}; // ECom interface UID
const TInt KMaxClients = 10;

// DATA TYPES

// CLASS DECLARATION

/**
 *  The main server side class.
 */
class CSuplServer :  public CPolicyServer,public MNetInitiatedObserver,public MConnectObserver,public MSuplEcomEventObserver
    {
    private:
        /**
         * C++ constructor.
         *
         * @param aPriority Priority at which to run the server active object.
         * @param aPolicy Reference to a policy object describing the security 
         *                checks required for each message type.
         * @param aServerType Sharable or non-sharable sessions.
         */
        CSuplServer(
        /* IN */    TInt aPriority,
        /* IN */    const TPolicy &aPolicy,
        /* IN */    TServerType aServerType = ESharableSessions
        );

        /**
         * EPOC default constructor.
         */
        void ConstructL();

        // By default, prohibit copy constructor
        CSuplServer( const CSuplServer& );
        // Prohibit assigment operator
        CSuplServer& operator= ( const CSuplServer& );

        /**
         * From CServer2
         */
        CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSuplServer* NewL();

        /**
        * Destructor.
        */
        ~CSuplServer();

    public: // New functions
        /**
         * This magic number has its origin in Symbian development.
         * It is they have some explaining to do, not us.
         */
        enum
            {
            EPriority=950
            };

    public: // Functions from base classes
        /**
         * From CActive
         *
         * @param aError the error code of the server leave
         * @return always KErrNone
         */
        TInt RunError(
        /* IN  */   TInt aError
        );

    public:  // New functions
        /**
         * Increment number of sessions
         */
        void IncrementSessions();

        /**
         * Decrement number of sessions
         */
        void DecrementSessions();

		void HandleNewNetSessionL(CSuplSessionManager& aSessionManager,const RMessage2& aMessage);
		void CompleteForwardMessageL(TInt aHandle);
		void CompleteConnect(TInt aHandle);
		TBool GetConnectFlag();
		/**
        * MSuplEcomEventObserver Observer method
        */
		void PlugInUninstalled(); 
        void DeInitialize();
    private:
        void SetErrorCode(TInt aErrCode);
        TInt GetErrorCode();
        void IncreamentIpcSessionCount();
				
    protected: 
        /**
         * From CPolicyServer
         */
        CPolicyServer::TCustomResult CustomSecurityCheckL(
            /* IN  */   const RMessage2& aMsg, 
            /* IN  */   TInt& aAction, 
            /* IN  */   TSecurityInfo& aMissing
            );

    private:     // Data
        CSuplServerDelayedShutdown* iShutdown;
        CSuplSessionManager*        iSessionManager;
        TInt                        iNumSessions;
		TTimeIntervalMicroSeconds32	iServerShutdownDelay;
		TInt iConnectFail;
		CSuplNetInitiatedRegistry* iSuplNetInitRegistry;
		TInt iNumNetMessages;
		
		CSuplServerStartupRequest*	iServerStartup;
        CSuplServerDeInitRequest*   iDeInitRequest;
		TBool iConnectFlag;
        TInt  iErrorCode;
        TInt  iIpcSessionCount;
		
		
	};

#endif      // __CSuplServer_H__

// End of File

