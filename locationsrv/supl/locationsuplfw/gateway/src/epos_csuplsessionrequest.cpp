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
* Description:   Asynchronous SUPL session request
*
*/



// INCLUDE FILES
#include <e32base.h>

#include "epos_suplterminalerrors.h"
#include "epos_csuplprotocolmanagerbase.h"
#include "epos_csuplsessionbase.h"
#include "epos_csuplglobal.h"
#include "epos_csuplsessionmanager.h"
#include "epos_csuplserverpanic.h"
#include "epos_msuplsessionobserver.h"
#include "epos_mnetinitiatedobserver.h"
#include "epos_csuplsessionrequest.h"
#include "epos_suplterminalconstants.h"


// CONSTANTS
//#ifdef _DEBUG
_LIT(KTraceFileName, "SUPL_GW::epos_csuplsessionrequest.cpp");
//#endif

// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplSessionRequest::CSuplSessionRequest(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MSuplSessionObserver* aObserver):
    CActive(EPriorityStandard),
    iRequestPhase(ESuplReqInactive),
    iObserver(aObserver),
    iSessnMgr(aSessnMgr),
    iSuplSessn(aSuplSessn),
    iParamExtendedFallback(NULL)
    {
    DEBUG_TRACE("CSuplSessionRequest::CSuplSessionRequest", __LINE__)
    CActiveScheduler::Add(this);
    
    }
CSuplSessionRequest::CSuplSessionRequest(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MNetInitiatedObserver* aObserver):
    CActive(EPriorityStandard),
    iRequestPhase(ESuplReqInactive),
    iNetObserver(aObserver),
    iSessnMgr(aSessnMgr),
    iSuplSessn(aSuplSessn),
    iParamExtendedFallback(NULL)    
    {
    DEBUG_TRACE("CSuplSessionRequest::CSuplSessionRequest", __LINE__)
    CActiveScheduler::Add(this);
    
    }

// EPOC default constructor can leave.
void CSuplSessionRequest::ConstructL()
    {
    }

// Two-phased constructor.
CSuplSessionRequest* CSuplSessionRequest::NewL(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MSuplSessionObserver* aObserver)
    {
    DEBUG_TRACE("CSuplSessionRequest::NewL", __LINE__)
    CSuplSessionRequest* self = new (ELeave) CSuplSessionRequest(aSessnMgr, aSuplSessn, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
CSuplSessionRequest* CSuplSessionRequest::NewL(CSuplSessionManager& aSessnMgr, CSuplSessionBase* aSuplSessn, MNetInitiatedObserver* aObserver)
    {
    DEBUG_TRACE("CSuplSessionRequest::NewL", __LINE__)
    CSuplSessionRequest* self = new (ELeave) CSuplSessionRequest(aSessnMgr, aSuplSessn, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// Destructor
CSuplSessionRequest::~CSuplSessionRequest()
    {
    DEBUG_TRACE("CSuplSessionRequest::~CSuplSessionRequest", __LINE__)
    iSessnMgr.RemoveFromQueueForReIssueRequest(*this);
    
    if (iParamExtendedFallback)
		{
        delete iParamExtendedFallback;
		}
    
	if (IsActive())      
      	Cancel();
    }

// ---------------------------------------------------------
// CSuplSessionRequest::MakeSuplSessionRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn, TInt aSetCaps, TInt aReqId, TBool aFirstReq)
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeSuplSessionRequestL", __LINE__)
    iRequestPhase = ESuplSessionRequest;
	iStatus = KRequestPending;
	TBool fallBack = ETrue;
	TBuf<KMaxHslpAddressLength> aHslpAddress (KNullDesC);
	
	iSessnMgr.RunSuplSessionL(aSuplSessn, iStatus, aHslpAddress, fallBack, aSetCaps, aReqId, aFirstReq);
	SetActive();
	iParamFallback = ETrue;
	iParamSuplSessn = aSuplSessn;
	iParamSetCaps = aSetCaps;
	iParamReqId = aReqId;
	iParamFirstReq = aFirstReq;
    if (iParamExtendedFallback)
        {
        delete iParamExtendedFallback;
        iParamExtendedFallback = NULL;
        }
	iParamExtendedQopUsed = EFalse;
    }
// ---------------------------------------------------------
// CSuplSessionRequest::MakeSuplSessionRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn, TInt aSetCaps, TInt aReqId, TSuplTerminalQop& aQop, TBool aFirstReq)
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeSuplSessionRequestL", __LINE__)
    iRequestPhase = ESuplSessionRequest;
	iStatus = KRequestPending;
	TBool fallBack = ETrue;
	TBuf<KMaxHslpAddressLength> aHslpAddress ( KNullDesC);
	
	iSessnMgr.RunSuplSessionL(aSuplSessn, iStatus, aHslpAddress, fallBack, aSetCaps, aReqId, aQop, aFirstReq);
	SetActive();
	
    iParamFallback = ETrue;
	iParamSuplSessn = aSuplSessn;
	iParamSetCaps = aSetCaps;
	iParamReqId = aReqId;
	iParamFirstReq = aFirstReq;
    if (iParamExtendedFallback)
        {
        delete iParamExtendedFallback;
        iParamExtendedFallback = NULL;
        }
	iParamQop = aQop;	
	iParamExtendedQopUsed = ETrue;	
    }

void CSuplSessionRequest::MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn,const TDesC& aHslpAddress, TBool aFallBack, TInt aSetCaps, TInt aReqId, TBool aFirstReq)
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeSuplSessionRequestL", __LINE__)
    iRequestPhase = ESuplSessionRequest;
    iStatus = KRequestPending;
    iSessnMgr.RunSuplSessionL(aSuplSessn, iStatus, aHslpAddress, aFallBack, aSetCaps, aReqId, aFirstReq);
	SetActive();
	
    iParamFallback = aFallBack;
	iParamSuplSessn = aSuplSessn;
	iParamSetCaps = aSetCaps;
	iParamReqId = aReqId;
	iParamFirstReq = aFirstReq;
    if (iParamExtendedFallback)
        {
        delete iParamExtendedFallback;
        iParamExtendedFallback = NULL;
        }
	iParamExtendedFallback = HBufC::NewL(aHslpAddress.Length());
	*iParamExtendedFallback = aHslpAddress;
	iParamExtendedQopUsed = EFalse;	
    }

void CSuplSessionRequest::MakeSuplSessionRequestL(CSuplSessionBase* aSuplSessn,const TDesC& aHslpAddress, TBool aFallBack, TInt aSetCaps, TInt aReqId, TSuplTerminalQop& aQop, TBool aFirstReq)
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeSuplSessionRequestL", __LINE__)
    iRequestPhase = ESuplSessionRequest;
    iStatus = KRequestPending;
    iSessnMgr.RunSuplSessionL(aSuplSessn, iStatus, aHslpAddress, aFallBack, aSetCaps, aReqId, aQop, aFirstReq);
    SetActive();
	
    iParamFallback = aFallBack;
	iParamSuplSessn = aSuplSessn;
	iParamSetCaps = aSetCaps;
	iParamReqId = aReqId;
	iParamFirstReq = aFirstReq;
    iParamQop = aQop;   
    iParamExtendedQopUsed = ETrue;
    if (iParamExtendedFallback)
        {
        delete iParamExtendedFallback;
        iParamExtendedFallback = NULL;
        }
    iParamExtendedFallback = HBufC::NewL(aHslpAddress.Length());
    *iParamExtendedFallback = aHslpAddress;
    }  
    
// ---------------------------------------------------------
// CSuplSessionRequest::ForwardMessageRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
void CSuplSessionRequest::ForwardMessageRequestL(
			CSuplSessionBase* aSuplSessn,
			TInt aHandle,
			HBufC8* clientBuf
		)
	{
	DEBUG_TRACE("CSuplSessionRequest::ForwardMessageRequestL", __LINE__)
	iHandle=aHandle;
	iRequestPhase=ESuplForwardMessage;
	iStatus=KRequestPending;
	iSessnMgr.ForwardMessageL(aSuplSessn,iStatus, clientBuf);
	SetActive();	
	}

// ---------------------------------------------------------
// CSuplSessionRequest::CancelRunSessionRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::CancelRunSessionRequest()
    {
    DEBUG_TRACE("CSuplSessionRequest::CancelRunSessionRequest", __LINE__)
    iRequestPhase = ESuplCancelRunSessionRequest;
	iSessnMgr.CancelRunSession(iSuplSessn);
	//SetActive();
    }

// ---------------------------------------------------------
// CSuplSessionRequest::NotifyServerShutdown
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::NotifyServerShutdown()
    {
    DEBUG_TRACE("CSuplSessionRequest::NotifyServerShutdown", __LINE__)
    if (IsActive())
        {
		iObserver->CompleteRunSession(KErrServerTerminated);
        Cancel();
        }
    }

// ---------------------------------------------------------
// CSuplSessionRequest::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::RunL()
    {
    DEBUG_TRACE("CSuplSessionRequest::RunL", __LINE__)
    TInt err = iStatus.Int();
	
	if(err != KErrNone && iRequestPhase == ESuplSessionRequest)
		{
		DEBUG_TRACE("Retrying session", __LINE__)
		iRequestPhase = ESuplWaitingToRetrySession;
		iSessnMgr.QueueForReIssueRequestL(*this);
		}
	else
		{	
	    switch (iRequestPhase)
	        {
	        case ESuplStartTriggerRequest:
	        case ESuplStopTriggerRequest:
	        	{
				iRequestPhase = ESuplReqInactive;
				iObserver->CompleteTriggerRunSession(err);			
	        	break;
	        	}
	        case ESuplTriggerFiredNotifyRequest:
	        	{
				iRequestPhase = ESuplReqInactive;
				iObserver->CompleteTriggerFiredNotifyRequest(err);			
	        	break;
	        	}
	        case ESuplSessionRequest:
			case ESuplRetryingSession:
				{
				iRequestPhase = ESuplReqInactive;
				iObserver->CompleteRunSession(err);
				break;
				}				
	       case ESuplCancelRunSessionRequest:
	            {
				iRequestPhase = ESuplReqInactive;
				iObserver->CompleteRunSession(err);
	            break;
	            }
	        case ESuplForwardMessage:
	        	{	
	        	iRequestPhase = ESuplReqInactive;
	        	iNetObserver->CompleteForwardMessageL(iHandle);
	        	break;
	        	}
	        case ESuplLocationConversionRequest:
	        case ESuplCancelLocationConversionRequest:
	            {
	            iRequestPhase = ESuplReqInactive;
	            iObserver->CompleteRunSession(err);
	            break;
	            }
	        default :
	            DebugPanic(EPosSuplServerPanicRequestInconsistency);
	        }
		}
    }

void CSuplSessionRequest::ReIssueRequestL()
	{
	DEBUG_TRACE("CSuplSessionRequest::ReIssueRequestL", __LINE__)
	if(!iParamExtendedQopUsed)
		{
        if (!iParamExtendedFallback)
            {
            MakeSuplSessionRequestL(iParamSuplSessn, iParamSetCaps, iParamReqId, iParamFirstReq);
            }
        else
            {
            MakeSuplSessionRequestL(iParamSuplSessn, *iParamExtendedFallback, iParamFallback, iParamSetCaps, iParamReqId, iParamFirstReq);
            }
		}
	else
		{
        if (!iParamExtendedFallback)
            {
            MakeSuplSessionRequestL(iParamSuplSessn, iParamSetCaps, iParamReqId, iParamQop, iParamFirstReq);
            }
        else
            {
            MakeSuplSessionRequestL(iParamSuplSessn, *iParamExtendedFallback, iParamFallback, iParamSetCaps, iParamReqId, iParamQop, iParamFirstReq);
            }
		}
	DEBUG_TRACE("Retry request succesfull", __LINE__)
	iRequestPhase = ESuplRetryingSession;
	}

// ---------------------------------------------------------
// CSuplSessionRequest::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CSuplSessionRequest::RunError(TInt /*aError*/)
    {
    DEBUG_TRACE("CSuplSessionRequest::RunError", __LINE__)
    return KErrNone;
    }

// ---------------------------------------------------------
// CSuplSessionRequest::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::DoCancel()
    {
    }

// ---------------------------------------------------------
// CSuplSessionRequest::CompleteSelf
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::CompleteSelf(TInt aReason)
    {
    DEBUG_TRACE("CSuplSessionRequest::CompleteSelf", __LINE__)
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, aReason);
    }
    
// ---------------------------------------------------------
// CSuplSessionRequest::ProtocolHUnloaded
//
// (other items were commented in a header).
// ---------------------------------------------------------
//     
void CSuplSessionRequest::ProtocolHUnloaded() 
	{	
	DEBUG_TRACE("CSuplSessionRequest::ProtocolHUnloaded Start", __LINE__)
	//Cancel();		
	iSessnMgr.DestroySession(iSuplSessn);
	iSuplSessn = NULL;
	DEBUG_TRACE("CSuplSessionRequest::ProtocolHUnloaded End", __LINE__)	
	}  
    
// ---------------------------------------------------------
// CSuplSessionRequest::MakeSuplSessionTriggerringRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::MakeSuplSessionTriggerringRequestL(
				CSuplSessionBase* aSuplSessn, 
				TSuplTerminalPeriodicTrigger& aPTrigger,
				TInt aSetCaps, 
				TInt aReqId)
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeSuplSessionTriggerringRequestL", __LINE__)
    iRequestPhase = ESuplStartTriggerRequest;	
	iStatus = KRequestPending;
	TBuf<KMaxHslpAddressLength> hslpAddress ( KNullDesC );
	TBool fallBack = ETrue;
	iSessnMgr.StartTriggerSessionL(
		aSuplSessn, 
		iStatus, 
		aPTrigger,
		hslpAddress,
		fallBack,
		aSetCaps, 
		aReqId );	
	SetActive();
    }
    
// ---------------------------------------------------------
// CSuplSessionRequest::MakeSuplStartTriggerRequestRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::MakeSuplSessionTriggerringRequestL(
				CSuplSessionBase* aSuplSessn, 
				TSuplTerminalPeriodicTrigger& aPTrigger,
				const TDesC& aHslpAddress, 
				TBool aFallBack,
				TInt aSetCaps, 
				TInt aReqId)
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeSuplSessionTriggerringRequestL", __LINE__)    
    iRequestPhase = ESuplStartTriggerRequest;	
	iStatus = KRequestPending;	
	iSessnMgr.StartTriggerSessionL(
		aSuplSessn, 
		iStatus, 
		aPTrigger,
		aHslpAddress,
		aFallBack,
		aSetCaps, 
		aReqId );		
	SetActive();
    }
    
// ---------------------------------------------------------
// CSuplSessionRequest::NotifyTriggerFiredRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::NotifyTriggerFiredRequestL(
			CSuplSessionBase* aSuplSessn, 
			TSuplTriggerFireInfo& aFireInfo
		)
	{	
    iRequestPhase = ESuplTriggerFiredNotifyRequest;	
	iStatus = KRequestPending;	
	iSessnMgr.NotifyTriggerFired(
		aSuplSessn, 
		iStatus, 
		aFireInfo);		
	SetActive();		
	}
		
// ---------------------------------------------------------
// CSuplSessionRequest::CancelTriggerringRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::CancelTriggerringRequest()
    {
    DEBUG_TRACE("CSuplSessionRequest::CancelTriggerringRequest", __LINE__)
    iRequestPhase = ESuplStopTriggerRequest;
    iSessnMgr.CancelTriggerringSession(iSuplSessn);	
	//SetActive();
    }
// ---------------------------------------------------------
// CSuplSessionRequest::MakeLocationConversionRequestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::MakeLocationConversionRequestL( CSuplSessionBase* aSuplSessn,
                                                          TGeoCellInfo& aCellInfo )
    {
    DEBUG_TRACE("CSuplSessionRequest::MakeLocationConversionRequestL", __LINE__)
    iRequestPhase = ESuplLocationConversionRequest;
    iStatus = KRequestPending;
    iSessnMgr.MakeLocationConversionRequestL(aSuplSessn,aCellInfo,iStatus );
    SetActive();       
    }

        
// ---------------------------------------------------------
// CSuplSessionRequest::CancelLocationConversionRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSuplSessionRequest::CancelLocationConversionRequest()
    {
    DEBUG_TRACE("CSuplSessionRequest::CancelLocationConversionRequest", __LINE__)
    iRequestPhase = ESuplCancelLocationConversionRequest;
    iSessnMgr.CancelLocationConversionRequest(iSuplSessn); 
    }  
//  End of File
