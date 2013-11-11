/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for doing asynchronous service for POS
*
*/



#include <lbs/epos_comasuplpossessionbase.h>
#include <lbs/epos_comasuplinforequestlist.h>
#include <lbs/epos_comasuplpospayload.h>
#include <lbs/epos_comasuplvelocity.h>
#include <lbs/epos_comasuplsetcapabilities.h>

#include "epos_comasuplstate.h"
#include "epos_comasuplposrequestor.h"
#include "epos_comasuplstate.h"
#include "epos_comasupltrace.h"

_LIT(KTraceFileName,"SUPL_OMA_PH::EPos_COMASuplPOSRequestor.cpp");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::COMASuplAOOperation
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplPOSRequestor::COMASuplPOSRequestor(MOMASuplMsgStateObserver* aObserver, //MOMASuplPOSObserver* aObserver,
											COMASuplPosSessionBase* aPOSSession):
											CActive( EPriorityStandard),
											iObserver(aObserver),iPOSSession(aPOSSession)	
    {
    	
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::ConstructL()
    {
		iTrace = COMASuplTrace::NewL();	
		iTrace->Trace(_L("COMASuplPOSRequestor::ConstructL"), KTraceFileName, __LINE__); 
		CActiveScheduler::Add( this );
		iPOSSessionRequestType = EPOS_IDEAL;
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::CreateListL
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::CreateListL()
    {
    	DestroyList(); //Safe side
		iTrace->Trace(_L("COMASuplPOSRequestor::CreateListL"), KTraceFileName, __LINE__); 
		iOMASuplInfoRequestList = COMASuplInfoRequestList::NewL();
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::DestroyList
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::DestroyList()
    {
    if(iTrace)
        iTrace->Trace(_L("COMASuplPOSRequestor::DestroyList"), KTraceFileName, __LINE__); 
    
    if(iOMASuplInfoRequestList)
        {
        delete iOMASuplInfoRequestList;
        iOMASuplInfoRequestList = NULL;

        }
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplPOSRequestor* COMASuplPOSRequestor::NewL(MOMASuplMsgStateObserver* aObserver,COMASuplPosSessionBase* aPOSSession)
    {
	    COMASuplPOSRequestor* self =  new ( ELeave ) COMASuplPOSRequestor(aObserver,aPOSSession);
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::~COMASuplPOSRequestor
// -----------------------------------------------------------------------------
//
COMASuplPOSRequestor::~COMASuplPOSRequestor()
    {
    	Cancel();
		DestroyList();
		
		if(iTrace)
			iTrace->Trace(_L("COMASuplPOSRequestor::~COMASuplPOSRequestor"), KTraceFileName, __LINE__); 		
		
		delete iTrace;
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::RunL
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::RunL()
{
	if(!iPOSSession || !iObserver )    
		return;
	
	//if(iStatus == KErrNone)
	{
		switch(iPOSSessionRequestType)
		{
			case EPOS_GET_POSITION:
			{
				iPOSSessionRequestType = EPOS_IDEAL;
				iTrace->Trace(_L("Request Completed for EPOS_GET_POSITION..."), KTraceFileName, __LINE__); 		
				iObserver->GetPositionComplete(iStatus.Int()); 
				break;
			}
			case EPOS_GET_SUPL_INFO:
			case EPOS_SESSION_INITIALIZE:
			{
				iPOSSessionRequestType = EPOS_IDEAL;
				iTrace->Trace(_L("Request Completed for POS_GET_SUPL_INFO / POS_SESSION_INITIALIZE..."), KTraceFileName, __LINE__); 		
				iObserver->OperationCompleteL(iStatus.Int());
				break;
			}
			case EPOS_IDEAL:
			default:
			{
				break;
			}
		}	
	}
}
// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::SetObserver
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::SetObserver(MOMASuplMsgStateObserver* aObserver)
    {
		iObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::RunError
// -----------------------------------------------------------------------------
//
TInt COMASuplPOSRequestor::RunError(TInt /* aError */)
    {
     	return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::DoCancel
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::DoCancel()
    {    
		CancelRequest();
    }
    
// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::GetSuplInfoL
// -----------------------------------------------------------------------------
//
TInt COMASuplPOSRequestor::GetSuplInfoL()
	{
	if(iPOSSession)
		{
			iTrace->Trace(_L("COMASuplPOSRequestor::GetSuplInfoL"), KTraceFileName, __LINE__); 
			iPOSSessionRequestType = EPOS_GET_SUPL_INFO;
			TRAPD(err,iPOSSession->GetSuplInfoL(iOMASuplInfoRequestList,iStatus));	
			if(err != KErrNone)
				{
					iTrace->Trace(_L("COMASuplPOSRequestor::GetSuplInfoL Leaved "), KTraceFileName, __LINE__); 
					return KErrCompletion;
				}
				else
				{
				    iTrace->Trace(_L("COMASuplPOSRequestor::GetSuplInfoL returned successfully "), KTraceFileName, __LINE__);
					SetActive();					
					return KErrNone;
				}
		}
	else
		{
			return KErrNotSupported;	
		}
		
	}

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::GetSuplInfoL
// -----------------------------------------------------------------------------
//
TInt COMASuplPOSRequestor::GetPositionL(COMASuplPosition* aPosition)
	{
	
	if(iPOSSession)
		{
			iTrace->Trace(_L("COMASuplPOSRequestor::GetPositionL"), KTraceFileName, __LINE__); 
			iPOSSessionRequestType = EPOS_GET_POSITION;
			TRAPD(err,iPOSSession->GetPositionL(iStatus,aPosition));	
			if(err != KErrNone)
				{
					iTrace->Trace(_L("COMASuplPOSRequestor::GetPositionL Leaved "), KTraceFileName, __LINE__); 
					return KErrCompletion;
				}
				else
				{
					SetActive();					
					return KErrNone;
				}
		}
	else
		{
			return KErrNotSupported;	
		}
	}
	
// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::CancelRequest
// Cancels any outstanding request 
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::CancelRequest()
{

	if(!iPOSSession)    
		{
			return;
		}

	switch(iPOSSessionRequestType)
	{
		case EPOS_GET_POSITION:
		{
			iTrace->Trace(_L("COMASuplPOSRequestor::CancelRequest EPOS_GET_POSITION "), KTraceFileName, __LINE__); 
			iPOSSession->CancelGetPosition();	
			break;
		} 
		case EPOS_GET_SUPL_INFO:
		{
			iTrace->Trace(_L("COMASuplPOSRequestor::CancelRequest EPOS_GET_SUPL_INFO "), KTraceFileName, __LINE__); 
			iPOSSession->CancelSuplInfoRequest();
			break;
		}
		case EPOS_SESSION_INITIALIZE:
		{
			iTrace->Trace(_L("COMASuplPOSRequestor::CancelRequest EPOS_SESSION_INITIALIZE "), KTraceFileName, __LINE__); 
			iPOSSession->CancelInitialize();
			break;
		}
		case EPOS_IDEAL:
		default:
		{
			break;
		}
	}
}

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::GetOMASuplInfoRequestList
// returns SUPLInfoRequest List 
// -----------------------------------------------------------------------------
//
COMASuplInfoRequestList* COMASuplPOSRequestor::GetOMASuplInfoRequestList()
	{
		return iOMASuplInfoRequestList;
	}

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::AppendInfoRequest
// Append InfoRequest List in COMASuplInfoRequestList
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::AppendInfoRequest(COMASuplInfoRequest* aSuplInfoRequest)
	{
		iOMASuplInfoRequestList->Append(aSuplInfoRequest);
	}
	
// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::InitilizePOSSessionL
// 
// -----------------------------------------------------------------------------
//
TInt COMASuplPOSRequestor::InitilizePOSSessionL(TInt aRequestId)
	{
		Cancel();
		iPOSSessionRequestType = EPOS_SESSION_INITIALIZE;
		TRAPD(err,iPOSSession->InitializeL(aRequestId,iStatus));		
		if(err != KErrNone)
		{
			iTrace->Trace(_L("COMASuplPOSRequestor::InitilizePOSSessionL  "), KTraceFileName, __LINE__); 
			return err;
		}
		else
		{
			SetActive();					
			return KErrNone;
		}
	}

// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::SetPosMethodAndAllowedCapabilities
// 
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::SetPosMethodAndAllowedCapabilities(TOMASuplAllowedCapabilities& aPOSAllowedCapabilities,
													COMASuplPosSessionBase::TOMASuplPositioningMethod aPOSPositioningMethod)
	{
			iPOSSession->SetPosMethodAndAllowedCapabilities(aPOSAllowedCapabilities,aPOSPositioningMethod);
	}
// -----------------------------------------------------------------------------
// COMASuplPOSRequestor::SetSLPAddressUsed
// 
// -----------------------------------------------------------------------------
//
void COMASuplPOSRequestor::SetSLPAddressUsed(const TDesC& aServerAddress)
    {
    iPOSSession->SetSLPAddressUsed(aServerAddress);
    }
//  End of File


