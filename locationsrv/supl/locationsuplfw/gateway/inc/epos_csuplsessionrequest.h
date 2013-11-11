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
* Description:  
*    Active object and base class for subclasses implementing a SUPL session 
*    request cycle.
*
*/



#ifndef __CSuplSessionRequest_H__
#define __CSuplSessionRequest_H__

// INCLUDE FILES
#include <e32base.h>
#include <lbspositioninfo.h>
#include <epos_suplterminalqop.h>
#include <epos_suplterminaltrigger.h>
#include "epos_suplgeocellinfo.h"
// CONSTANT DECLARATIONS

// FORWARD DECLARATIONS
class MSuplSessionObserver;
class MNetInitiatedObserver;
class CSuplSessionBase;
class CSuplSessionManager;
// CLASS DECLARATION

/**
*    Active object implementing a position request cycle. Handles
*    activities such as controlling logging and verifying privacy.
*
*/
class CSuplSessionRequest : public CActive
    {
    public:  // Constructors and destructor
   
        /**
        * Two-phased constructor.
        */
        static CSuplSessionRequest* NewL(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MSuplSessionObserver* aObserver);
        static CSuplSessionRequest* NewL(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MNetInitiatedObserver* aObserver);

        /**
        * Destructor.
        */
        virtual ~CSuplSessionRequest();

    public:  // New functions

        /**
        * Starts a SUPL session request cycle. 
        * @param aMessage the request message from the client
        */
        void MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn, TInt aSetCaps, TInt aReqId, TBool aFirstReq);
        void MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn, TInt aSetCaps, TInt aReqId, TSuplTerminalQop& aQop, TBool aFirstReq);
        void MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn, const TDesC& aHslpAddress, TBool aFallBack, TInt aSetCaps, TInt aReqId, TBool aFirstReq);
        void MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn, const TDesC& aHslpAddress, TBool aFallBack, TInt aSetCaps, TInt aReqId, TSuplTerminalQop& aQop, TBool aFirstReq);

		void MakeSuplSessionTriggerringRequestL(
				CSuplSessionBase* aSuplSessn, 
				TSuplTerminalPeriodicTrigger& aPTrigger,
				TInt aSetCaps, 
				TInt aReqId 
			);
		
		void MakeSuplSessionTriggerringRequestL(
				CSuplSessionBase* aSuplSessn, 
				TSuplTerminalPeriodicTrigger& aPTrigger,
				const TDesC& aHslpAddress, 
				TBool aFallBack,
				TInt aSetCaps, 
				TInt aReqId
			);
		
		void NotifyTriggerFiredRequestL(
				CSuplSessionBase* aSuplSessn, 
				TSuplTriggerFireInfo& aFireInfo
			);
		
        /**
        * Called when the server class is shutting down.
        */
        void NotifyServerShutdown();

		void ForwardMessageRequestL(CSuplSessionBase* aSuplSessn,TInt aHandle,HBufC8* clientBuf);
		
		void CancelRunSessionRequest();
		
		void CancelTriggerringRequest();
		
		void ProtocolHUnloaded();
		void MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,TGeoCellInfo& aCellInfo );
		        
		void CancelLocationConversionRequest();
		
		void ReIssueRequestL();
    protected:  // From CActive

        void RunL();
        TInt RunError(TInt aError);
        void DoCancel();

    private:

        CSuplSessionRequest(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MSuplSessionObserver* aObserver);

		CSuplSessionRequest(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MNetInitiatedObserver* aObserver);
        void ConstructL();

        // By default, prohibit copy constructor
        CSuplSessionRequest( const CSuplSessionRequest& );
        // Prohibit assigment operator
        CSuplSessionRequest& operator= ( const CSuplSessionRequest& );
 		void CompleteSelf(TInt aReason);
	
    private:  // Data

        enum TSuplRequestStage
            {
            ESuplReqInactive = 0,
            ESuplSessionRequest,
            ESuplCancelRunSessionRequest,
            ESuplForwardMessage,
            ESuplStartTriggerRequest,            
            ESuplStopTriggerRequest,
            ESuplTriggerFiredNotifyRequest,
            ESuplLocationConversionRequest,
            ESuplCancelLocationConversionRequest,
			ESuplWaitingToRetrySession,
			ESuplRetryingSession
		    };
     
	 TSuplRequestStage           iRequestPhase;
     HBufC8*                     iPositionBuffer;
     MSuplSessionObserver* 	     iObserver;
     MNetInitiatedObserver*		 iNetObserver;
  	 CSuplSessionManager& 		 iSessnMgr;
	 CSuplSessionBase*			 iSuplSessn;
	 TInt iHandle;

	 CSuplSessionBase* iParamSuplSessn;
	 TInt iParamSetCaps;
	 TInt iParamReqId;
	 TBool iParamFallback;
	 TBool iParamFirstReq;
	 TBool iParamExtendedQopUsed;
	 TSuplTerminalQop iParamQop;
	 HBufC* iParamExtendedFallback;
    };  

#endif  // __CSuplSessionRequest_H__

// End of File
