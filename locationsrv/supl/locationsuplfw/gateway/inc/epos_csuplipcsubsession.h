/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Base class for SUPL subsessions
*
*/




#ifndef __C_SUPLIPCSUBSESSION_H__
#define __C_SUPLIPCSUBSESSION_H__

// INCLUDES
#include <e32base.h>
#include <lbspositioninfo.h>
#include <epos_suplterminal.h>

#include "epos_csuplsessionbase.h"

#include "epos_csuplsessionrequest.h"
#include "epos_msuplsessionobserver.h"
#include "epos_csuplsettings.h"
#include "epos_csuplsettingsinternal.h"
#include "epos_csuplsettingparams.h"


// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* This class provides functionality common to all SUPL subsessions.
*/
class CSuplIPCSubSession : public CObject,public MSuplSessionObserver
    {

	public:  // Constructor & Destructor

        /**
        * Two-phased constructor.
        */
        static CSuplIPCSubSession* NewLC(CSuplSessionManager& aSessnMgr, 
                                         CSUPLProtocolManagerBase::TSuplReqType aReqType,
                                         TInt aIpcSessionId,
                                         RSuplTerminalSubSession::TSuplServiceType aSuplService);

        /**
        * Destructor.
        */
		virtual ~CSuplIPCSubSession();

	public:		// New functions
        /**
        * This function is responsible for handling the
        * servicing of client requests to the server.
        * It uses the message passed as an argument to
        * obtain the request opcode and to access client
        * addresses for reading and writing.
        *
        * @param aMessage The current message
        */
        void ServiceL(
            /* IN  */         const RMessage2& aMessage
         );
        /**
        * Called when the server class is shutting down.
        */
        void NotifyServerShutdown();
        void DestroySession(CSuplSessionManager& aSessnMgr);
        
        
		void ProtocolHUnloaded(); 
		
		void CompleteGetServerAddress(const RMessage2& aMessage, TInt aReason);
		TInt PackServerAddressData(const RMessage2& aMessage);
		TInt PackTriggerData(const RMessage2& aMessage);


	public: //  from MSuplSessionObserver
	
		void CompleteRunSession(TInt aReason);
		void CompleteTriggerRunSession(TInt aReason);		
		void CompleteGetPositionRequest(const RMessage2& aMessage, TInt aReason);
		void CompleteTriggerFiredNotifyRequest(TInt aReason);
		TInt PackPositionData(const RMessage2& aMessage);
   
    private:

        CSuplIPCSubSession();

        void ConstructL(CSuplSessionManager& aSessnMgr, 
                        CSUPLProtocolManagerBase::TSuplReqType aReqType,
                        TInt aIpcSessionId,
                        RSuplTerminalSubSession::TSuplServiceType aSuplService);

        // By default, prohibit copy constructor
        CSuplIPCSubSession( const CSuplIPCSubSession& );
        // Prohibit assigment operator
        CSuplIPCSubSession& operator= ( const CSuplIPCSubSession& );

        void HandleRunSessionL(
        	/* IN */			const RMessage2& aMessage);
        
        void HandleGetPositionL(const RMessage2& aMessage);
        void HandleGenericGetPositionL(const RMessage2& aMessage);
		void HandleRunSessionCancelRequestL(const RMessage2& aMessage);
        void HandleGetServerAddressL(const RMessage2& aMessage);

        /*
        * For Periodic Trigger Seesion
        */
		void HandlePeriodicTriggerSessionL(const RMessage2& aMessage);
		void HandlePeriodicTriggerSessionWithServernameL(const RMessage2& aMessage);		
		void HandleStopTriggerringSessionL(const RMessage2& aMessage);		
		void HandleNotifyTriggerFiredL(const RMessage2& aMessage);
		/*
		         * For Periodic Trigger Seesion
		         */
		        void HandleLocationConversionL(const RMessage2& aMessage);
		        void HandleCancelLocationConversionL(const RMessage2& aMessage);

    private:    // Data
    	// Supl session handle.Ownership is with this object
    	CSuplSessionBase*		iSuplSession;
		// Supl session request object.Ownership is with this object
		CSuplSessionRequest*	iSuplSessnReq;
		RMessage2               iMessage;
		// Position buffer.Ownership is with this object
		HBufC8*                 iPositionBuffer;
		TInt 					iReqType;
		RSuplTerminalSubSession::TSuplServiceType iSuplService;
    };


#endif  // __CSuplIPCSubSession_H__

// End of File
