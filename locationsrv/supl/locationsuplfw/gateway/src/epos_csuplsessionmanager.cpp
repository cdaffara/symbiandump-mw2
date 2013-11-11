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
* Description:   Session manager related routines 
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <ecom/ecom.h>
#include <flogger.h>
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_msuplpacketnotifier.h"
#include "epos_csuplcommunicationmanager.h"
#include "epos_suplterminalqop.h"

#include "epos_csuplsessionbase.h"
#include "epos_sessionmanagerlogging.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csuplglobal.h"

//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplsessionmanager.cpp");
//#endif
//#define DEBUG

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSessionManager::CSuplSessionManager()
    {
    }

// EPOC default constructor
void CSuplSessionManager::ConstructL()
    {
    DEBUG_TRACE("CSuplSessionManager::ConstructL", __LINE__)            
	iSessionRetryQ = CSuplSessionRetryQ::NewL();
    iCommMgr = CSuplCommunicationManager::NewL(*this);
    iCommMgr->Initialize();
    iConnectError=KErrNone;
    }

// Two-phased constructor
CSuplSessionManager* CSuplSessionManager::NewL()
    {
    DEBUG_TRACE("CSuplSessionManager::NewL", __LINE__)            
    CSuplSessionManager* self = new (ELeave) CSuplSessionManager;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CSuplSessionManager::~CSuplSessionManager()
    {
    DEBUG_TRACE("CSuplSessionManager::~CSuplSessionManager", __LINE__)            
    if(iProtocolMgr)          
    delete iProtocolMgr;
    delete iCommMgr;
	delete iSessionRetryQ;
    delete iEcomWatcher;
    REComSession::FinalClose();
    }
 
// ------------------------------------------------------------------
// CSuplSessionManager::DestroySession
// DestroySession calls the Destroy Session Method in Protocol Manager    
// (other items were commented in a header).
// ------------------------------------------------------------------
//	 
TInt CSuplSessionManager::DestroySession(CSuplSessionBase* aSuplSession)
	{
	// Need to change this to RemoveSuplSessionL(CSuplSessionBase* aSuplSession,TSuplReqType aReqType) once this is  
	// changed in protocol manager
	DEBUG_TRACE("CSuplSessionManager::DestroySession", __LINE__)            	
	if(iProtocolMgr)	
		return iProtocolMgr->DestroySession(aSuplSession);
	else
		return KErrGeneral;
	}	
	
// ---------------------------------------------------------
// CSuplSessionManager::CreateNewSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	     
CSuplSessionBase* CSuplSessionManager::CreateNewSessionL(CSUPLProtocolManagerBase::TSuplReqType aReqType, TInt aIpcSessionId, RSuplTerminalSubSession::TSuplServiceType aSuplService)
	{
	DEBUG_TRACE("CSuplSessionManager::CreateNewSessionL", __LINE__)
	if(iProtocolMgr)
		{
		CSuplSessionBase* suplSessn=NULL;
		TInt err = KErrNone;
		TRAP(err, suplSessn=iProtocolMgr->CreateNewSessionL(aReqType, aIpcSessionId, aSuplService));
		if (err != KErrNone)
			return NULL;
		else 
			return suplSessn;
		}
	else
		return NULL;
	}

// ---------------------------------------------------------
// CSuplSessionManager::RunSuplSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	 
void CSuplSessionManager::RunSuplSessionL(
            CSuplSessionBase* aSuplSessn,  TRequestStatus& aStatus, 
            const TDesC& aHslpAddress, TBool aFallBack, TInt aSuplSETCaps, TInt aReqId, TBool aFirstReq
            )
	{
	// Need to remove this once changes are made in protocol manager
	// TSuplSETCapabilities setCaps;
#ifdef DEBUG
	    TBuf<255> tempBuf;
	    tempBuf.Append(_L("Server name in gateway is : "));
	    tempBuf.Append(aHslpAddress);
	    TRACETEXT(tempBuf);
	    if (aFallBack)
	        {
            tempBuf.Copy(_L("Fallback is set as TRUE."));
	        }
	    else
	        {
            tempBuf.Copy(_L("Fallback is set as False"));
	        }
	    
	    TRACETEXT(tempBuf);
#endif
	
	DEBUG_TRACE("CSuplSessionManager::RunSuplSessionL", __LINE__)
	TInt err = KErrNone;
	if(iProtocolMgr && aSuplSessn)
		{
		TRAP(err,iProtocolMgr->RunSessionL(aSuplSessn, aStatus, aHslpAddress, aFallBack, aFirstReq,  aSuplSETCaps, aReqId);)	
		if(err!=KErrNone)
			{
			TRequestStatus *status = &aStatus;
			User::RequestComplete(status,KErrNotReady);	
			}
		}
	else
		{
		TRequestStatus *status = &aStatus;
		User::RequestComplete(status,KErrNotReady);
		}
	}
	
// ---------------------------------------------------------
// CSuplSessionManager::RunSuplSessionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	 	
void CSuplSessionManager::RunSuplSessionL(
            CSuplSessionBase* aSuplSessn,  TRequestStatus& aStatus, 
            const TDesC& aHslpAddress, TBool aFallBack, TInt aSuplSETCaps, TInt aReqId,TSuplTerminalQop& aQop, 
            TBool aFirstReq
            )
	{
	DEBUG_TRACE("CSuplSessionManager::RunSuplSessionL with Qop", __LINE__)
#ifdef DEBUG
	TBuf<255> tempBuf;
    tempBuf.Append(_L("Server name in gateway is : "));
    tempBuf.Append(aHslpAddress);
    TRACETEXT(tempBuf);
    if (aFallBack)
        {
        tempBuf.Copy(_L("Fallback is set as TRUE."));
        }
    else
        {
        tempBuf.Copy(_L("Fallback is set as False"));
        }
    
    TRACETEXT(tempBuf);
#endif
	TInt err = KErrNone;
	if(iProtocolMgr && aSuplSessn)
		{
		TRAP(err,iProtocolMgr->RunSessionL(aSuplSessn, aStatus, aHslpAddress, aFallBack, aFirstReq, aSuplSETCaps,aQop, aReqId);)	
		if(err!=KErrNone)
			{
			TRequestStatus *status = &aStatus;
			User::RequestComplete(status,KErrNotReady);	
			}
		}
	else
		{
		TRequestStatus *status = &aStatus;
		User::RequestComplete(status,KErrNotReady);
		}
	}

// ---------------------------------------------------------
// CSuplSessionManager::ForwardMessageL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//	 	
void CSuplSessionManager::ForwardMessageL(CSuplSessionBase* aSuplSessn,TRequestStatus& iStatus,HBufC8* clientBuf)
	{
	DEBUG_TRACE("CSuplSessionManager::ForwardMessageL", __LINE__)
	TRequestStatus *status = &iStatus;
	
    if (aSuplSessn == NULL || clientBuf == NULL)
	    User::RequestComplete(status,KErrGeneral);
    else
    if(iProtocolMgr)
		iProtocolMgr->HandleSuplMessageL(aSuplSessn, iStatus,clientBuf);
	}

// ---------------------------------------------------------
// CSuplSessionManager::CancelRunSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::CancelRunSession(CSuplSessionBase *aSessn)
	{
	DEBUG_TRACE("CSuplSessionManager::CancelRunSession", __LINE__)
	if(iProtocolMgr && aSessn)
	iProtocolMgr->CancelRunSession(aSessn);
	}

// ---------------------------------------------------------
// CSuplSessionManager::InitializeL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::InitializeL(
				TUid uId,TRequestStatus& aStatus,
				MSuplEcomEventObserver& aSuplEcomEventObserver
		)
	{
    DEBUG_TRACE("CSuplSessionManager::InitializeL", __LINE__)
    
    TRAPD(err,iProtocolMgr = CSUPLProtocolManagerBase::NewL(uId));
    if(err!=KErrNone)
        { 
    		iConnectError=err;
				TRequestStatus *status = &aStatus;
				User::RequestComplete(status,err);	
        }
    else
	    {
		iCommMgr->SetPacketReceiver(iProtocolMgr);	
		TRAPD(err1,iProtocolMgr->InitializeL(iCommMgr,aStatus));
		if(err1!=KErrNone)
	 		iConnectError=err1;
	    }
  	iEcomWatcher = CSuplEcomEventWatcher::NewL(
  						aSuplEcomEventObserver,
  						KProtocolManagerInterfaceDefinitionUid,
  						uId);  
  						
  	iEcomWatcher->NotifyOnPlugInUnInstallation();
	}

// ---------------------------------------------------------
// CSuplSessionManager::GetConnectError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSessionManager::GetConnectError()
	{
	DEBUG_TRACE("CSuplSessionManager::GetConnectError", __LINE__)
	return iConnectError;
	}	

// ---------------------------------------------------------
// CSuplSessionManager::CancelInitialize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::CancelInitialize()
	{
	if(iProtocolMgr)
		{
		iProtocolMgr->CancelInitialize();		
		}	
	}

// ---------------------------------------------------------
// CSuplSessionManager::ProtocolHUnloaded
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::ProtocolHUnloaded()  
	{
    DEBUG_TRACE("CSuplSessionManager::ProtocolHUnloaded", __LINE__)	
	delete iProtocolMgr;
	iProtocolMgr = NULL;
	}

// ---------------------------------------------------------
// CSuplSessionManager::DeInitialize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::DeInitialize(TRequestStatus& aStatus)
    {
    DEBUG_TRACE("CSuplSessionManager::DeInitialize", __LINE__)	
	if(iProtocolMgr)
		{
        iProtocolMgr->DeInitialize(aStatus);
		}	
    }        

// ---------------------------------------------------------
// CSuplSessionManager::CancelDeInitialize
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::CancelDeInitialize()
    {
    DEBUG_TRACE("CSuplSessionManager::CancelDeInitialize", __LINE__)	
	if(iProtocolMgr)
		{
        iProtocolMgr->CancelDeInitialize();
		}	
    }       
     
// ---------------------------------------------------------
// CSuplSessionManager::GetSUPLMessageVersionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSessionManager::GetSUPLMessageVersionL(TInt& aMajorVersion, const TDesC8& aReceivedMessage)
    {
    return iProtocolMgr->GetSUPLMessageVersionL(aMajorVersion,aReceivedMessage);    
    }

// ------------------------------------------------------------------
// CSuplSessionManager::StartTriggerSessionL
//
// (other items were commented in a header).
// ------------------------------------------------------------------
//	 
void CSuplSessionManager::StartTriggerSessionL(
			CSuplSessionBase* aSuplSession,
			TRequestStatus& aStatus,
			TSuplTerminalPeriodicTrigger& aPTrigger,
			const TDesC& aHslpAddress, 
			TBool aFallBack,
			TInt aAllowedCapabilities,
			TInt aRequestID
           	)
	{
	DEBUG_TRACE("CSuplSessionManager::StartTriggerSessionL with Periodic Trigger Request", __LINE__)

#ifdef DEBUG
	    TBuf<255> tempBuf;
	    tempBuf.Append(_L("Server name in gateway is : "));
	    tempBuf.Append(aHslpAddress);
	    TRACETEXT(tempBuf);
	    if (aFallBack)
	        {
            tempBuf.Copy(_L("Fallback is set as TRUE."));
	        }
	    else
	        {
            tempBuf.Copy(_L("Fallback is set as False"));
	        }
	    
	    TRACETEXT(tempBuf);
#endif
	
	TInt err = KErrNone;
	if(iProtocolMgr && aSuplSession)
		{
		TRAP(err,iProtocolMgr->RunSuplTriggerSessionL(
					aSuplSession, 
					aStatus, 
					aPTrigger,
					aHslpAddress,
					aFallBack,
					aAllowedCapabilities, 
					aRequestID );	
		)	
					
		if(err!=KErrNone)
			{
			TRequestStatus *status = &aStatus;
			User::RequestComplete(status,KErrNotReady);	
			}
		}
	else
		{
		TRequestStatus *status = &aStatus;
		User::RequestComplete(status,KErrNotReady);
		}	
	}

// ---------------------------------------------------------
// CSuplSessionManager::CancelTriggerringSession
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::CancelTriggerringSession(CSuplSessionBase *aSessn)
	{
	DEBUG_TRACE("CSuplSessionManager::CancelTriggerringSession", __LINE__)
	if(iProtocolMgr && aSessn)
	iProtocolMgr->CancelSuplTriggerSession(aSessn);
	}

// ---------------------------------------------------------
// CSuplSessionManager::NotifyTriggerFired
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::NotifyTriggerFired(
				CSuplSessionBase *aSessn,        	
				TRequestStatus& aStatus,  
        		TSuplTriggerFireInfo& aFireInfo 
        	)
	{
	DEBUG_TRACE("CSuplSessionManager::StartTriggerSessionL with Periodic Trigger Request", __LINE__)
	
	TInt err = KErrNone;
	if(iProtocolMgr && aSessn)
		{		
		TRAP(err,iProtocolMgr->NotifyTriggerFiredL(
			aSessn, 
			aStatus, 
			aFireInfo);	
		)	
				
		if(err!=KErrNone)
			{
			TRequestStatus *status = &aStatus;
			User::RequestComplete(status,err);	
			}
		}
	else
		{
		TRequestStatus *status = &aStatus;
		User::RequestComplete(status,KErrNotReady);
		}	
	}
// ---------------------------------------------------------
// CSuplSessionManager::MakeLocationConversionRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//      
void CSuplSessionManager::MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,
                                                          TGeoCellInfo& aCellInfo,
                                                          TRequestStatus& aStatus
                                                        )
    {
    DEBUG_TRACE("CSuplSessionManager::MakeLocationConversionRequestL", __LINE__)
    TInt err = KErrNone;
    if(iProtocolMgr && aSuplSessn)
        {
        DEBUG_TRACE("CSuplSessionManager::MakeLocationConversionRequestL if condition true", __LINE__)
        TRAP(err,iProtocolMgr->MakeLocationConversionRequestL(aSuplSessn,aCellInfo,aStatus));   
        DEBUG_TRACE("CSuplSessionManager::MakeLocationConversionRequestL err", __LINE__)
        if(err!=KErrNone)
            {
            DEBUG_TRACE("CSuplSessionManager::MakeLocationConversionRequestL err not KErrNone", __LINE__)
            TRequestStatus *status = &aStatus;
            User::RequestComplete(status,KErrNotReady); 
            }
        }
    else
        {
        TRequestStatus *status = &aStatus;
        User::RequestComplete(status,KErrNotReady);
        }
    }

// ---------------------------------------------------------
// CSuplSessionManager::CancelLocationConversionRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionManager::CancelLocationConversionRequest(CSuplSessionBase *aSessn)
    {
    DEBUG_TRACE("CSuplSessionManager::CancelLocationConversionRequest", __LINE__)
    if(iProtocolMgr && aSessn)
     iProtocolMgr->CancelLocationConversionRequest(aSessn);
    }
    
void CSuplSessionManager::ConnectionOpened()
	{
	iSessionRetryQ->SessionStarted();	
	}

void CSuplSessionManager::ConnectionClosed()
	{
	iSessionRetryQ->SessionEnded();	
	}

void CSuplSessionManager::QueueForReIssueRequestL(CSuplSessionRequest& aSessionRequest)
	{
	iSessionRetryQ->AddToQueueL(aSessionRequest);
	}

void CSuplSessionManager::RemoveFromQueueForReIssueRequest(CSuplSessionRequest& aSessionRequest)
	{
	iSessionRetryQ->RemoveFromQueueL(aSessionRequest);
	}

// End of File
