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
* Description:   Main class for server side sessions
*
*/



#ifndef __CSuplIPCSession_H__
#define __CSuplIPCSession_H__

//  INCLUDES
#include <e32base.h>
#include <lbspositioninfo.h>
#include <epos_suplterminal.h>

#include "epos_mconnecttimerobserver.h"

const TSecureId KSuplWapSecureId = 0x1020d684;
// FORWARD DECLARATIONS
class CSuplServer;
class CSuplSubsessionRegistry;
class CSuplConnectTimer;


// CLASS DECLARATION

/**
*  The SUPL IPC session class.
*
*  This inherits from CSession2
*/
class CSuplIPCSession : public CSession2,public MConnectTimerObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ destructor
        */
        ~CSuplIPCSession();

        /**
        * Two-phased constructor.
        *
        * @param aServer Pointer to the server Active Object
        * @return Pointer to a new instance of CSuplIPCSession
        */
        static CSuplIPCSession* NewL(
			/* IN  */   CSuplServer& aServer,
			/* IN  */   CSuplSessionManager& aSessnMgr,
            /* IN  */   TInt aSessionCount
            );

    public:     // New functions

        /**
        * Called when the server class is shutting down.
        */
        void NotifyServerShutdown();

        // Functions from base classes

        /**
        * From CSession2
        *
        * This function services all requests from clients.
        *
        * @param aMessage the message that should be serviced
        */
        void ServiceL(
            /* IN  */   const RMessage2& aMessage
            );
            
        /*
        Function: CLoseSubsessionL()
        Used for closing a particular subsession.
        Param: aMessage
        Return: None
        */
        void CloseSubsessionL(const RMessage2& aMessage);
		void ForwardMessageL(const RMessage2& aMessage);
		void CompleteConnect(TInt aHandle);
		void CompleteConnectTimerL();
		void ProtocolHUnloaded(); 

	private:

        CSuplIPCSession(
                CSuplServer& aServer,
                CSuplSessionManager& aSessnMgr
            );

        void ConstructL(TInt aIpcSessionId);

        // By default, prohibit copy constructor
        CSuplIPCSession( const CSuplIPCSession& );
        // Prohibit assigment operator
        CSuplIPCSession& operator= ( const CSuplIPCSession& );

        void ForwardToSubSessionL(const RMessage2& aMessage);

        void CreateSubSessionL(
                const RMessage2& aMessage,
                CSUPLProtocolManagerBase::TSuplReqType aReqType,
                RSuplTerminalSubSession::TSuplServiceType aSuplService
            );


        void RequestComplete(
                const RMessage2& aMessage,
                TInt aCompleteCode
             );

    private:    // Data
    		enum TConnectRequestType
    		{
                ETerminalConnectOpen,
    		    ENetworkConnectForwardMessage
    		};
        CSuplServer&                iSuplServer;
        CSuplSubsessionRegistry*    iSubSessionRegistry;
        CSuplSessionManager&        iSessionManager;
        TBool                       iDecrementSessions;
        TBool                       iServerShutdown;
        TInt iMessageId;
        CSuplConnectTimer* iConnectTimer; 
        TTimeIntervalMicroSeconds32	iConnectDelay;
        TConnectRequestType iReqType;
        RMessage2 iMessage;
        TInt iConnectCount;
        TInt iIpcSessionId;
        RSuplTerminalSubSession::TSuplServiceType iSuplService;
    };

#endif      // __CSuplIPCSession_H__

// End of File
