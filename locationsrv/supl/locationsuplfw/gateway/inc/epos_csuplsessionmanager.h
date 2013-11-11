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
* Description:   Registry for subsessions
*
*/



#ifndef __CSuplSessionManager_H__
#define __CSuplSessionManager_H__


#include <e32base.h>
#include <lbspositioninfo.h>
#include <epos_suplterminal.h>
#include <epos_suplterminaltrigger.h>
#include <epos_suplgeocellinfo.h>
#include "epos_csuplecomeventwatcher.h"
#include "epos_csuplsessionretryq.h"
#include "epos_csuplcommunicationmanager.h"

// FORWARD DECLARATIONS
class CSUPLProtocolManagerBase;
class CSuplSessionBase;
class CSuplCommunicationManager;


// CLASS DECLARATION

/**
*  Class to handle SUPL sessions 
*/
class CSuplSessionManager : public CBase, MSuplConnectionMonitor
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor
        *
        * @return a new instance of this class
        */
		static CSuplSessionManager* NewL();

        /**
        * C++ destructor
        */
		~CSuplSessionManager();

	public:		// New functions
		void InitializeL(TUid uId,TRequestStatus& aStatus,MSuplEcomEventObserver& aSuplEcomEventObserver);
		CSuplSessionBase* CreateNewSessionL(CSUPLProtocolManagerBase::TSuplReqType aReqType, TInt aIpcSessionId,RSuplTerminalSubSession::TSuplServiceType aSuplService);
		void RunSuplSessionL(CSuplSessionBase* aSuplSessn,  TRequestStatus& aStatus,const TDesC& aHslpAddress, TBool aFallBack, TInt aSuplSETCaps, TInt aReqId, TBool aFirstReq);
		void RunSuplSessionL(CSuplSessionBase* aSuplSessn,  TRequestStatus& aStatus,const TDesC& aHslpAddress, TBool aFallBack, TInt aSuplSETCaps, TInt aReqId,TSuplTerminalQop& aQop, TBool aFirstReq);
		TInt DestroySession(CSuplSessionBase* aSuplSession);
		void CancelRunSession(CSuplSessionBase *aSessn);
		void ForwardMessageL(CSuplSessionBase* aSuplSession,TRequestStatus& iStatus,HBufC8* clientBuf);
		TInt GetConnectError();
		void CancelInitialize();
		void ProtocolHUnloaded(); 
        void DeInitialize(TRequestStatus& aStatus);
        void CancelDeInitialize();

		void QueueForReIssueRequestL(CSuplSessionRequest& aSessionRequest);
		void RemoveFromQueueForReIssueRequest(CSuplSessionRequest& aSessionRequest);
		void StartTriggerSessionL(
				CSuplSessionBase* aSuplSession,
				TRequestStatus& aStatus,
				TSuplTerminalPeriodicTrigger& aPTrigger,
				const TDesC& aHslpAddress, 
				TBool aFallBack,
				TInt aAllowedCapabilities,
				TInt aRequestID
   			);
   		
		void CancelTriggerringSession(CSuplSessionBase *aSessn);
		 
		void NotifyTriggerFired(
				CSuplSessionBase *aSessn,        	
				TRequestStatus& aStatus,  
        		TSuplTriggerFireInfo& aFireInfo
        	);
		
	   /**
        * Get SUPL message version.
        */
		TInt GetSUPLMessageVersionL(TInt& aMajorVersion, const TDesC8& aReceivedMessage);
		
		
	   /**
        * Makes location conversion request.
        */
		void MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,
		                                             TGeoCellInfo& aCellInfo,
		                                             TRequestStatus& aStatus
		                                           );	                                           
		
		/**
		 * Cancels outstanding conversion request.
		 */
		void CancelLocationConversionRequest(CSuplSessionBase *aSessn);
		
	
		// from MSuplConnectionMonitor
		void ConnectionOpened();
		void ConnectionClosed();
	
    private:

        /**
        * C++ default constructor.
        */
        CSuplSessionManager();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CSuplSessionManager( const CSuplSessionManager& );
        // Prohibit assigment operator
        CSuplSessionManager& operator= ( const CSuplSessionManager& );
		
	
    private:	// Data
        CSUPLProtocolManagerBase*   iProtocolMgr;
        CSuplCommunicationManager*  iCommMgr;
        TInt iConnectError;
        CSuplEcomEventWatcher*      iEcomWatcher;
		CSuplSessionRetryQ*			iSessionRetryQ;
    };


#endif // __CSuplSessionManager_H__

// End of File
