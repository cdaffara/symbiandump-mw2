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
* Description:  Implementation of the class CPosApiAsynchRequestor
*
*/


   

#include "posapiasynchrequestor.h"
#include <epos_comasuplpossessionbase.h>   // interface
#include <epos_comasuplpospayload.h>
#include <epos_comasuplposition.h>

#include <ecom/ecom.h> 
 
//Resets & destroys array if leave occurs
void ResetAndDestroy(TAny* aArray)
{
	((RImplInfoPtrArray*)aArray)->ResetAndDestroy();
}

CPosApiAsynchRequestor::CPosApiAsynchRequestor(CStifLogger* aLog,MRequestCompleteObserver* aObserver) :
    CActive(EPriorityStandard),iLog(aLog),iObserver(aObserver)
    {
	    CActiveScheduler::Add(this);
	}

CPosApiAsynchRequestor* CPosApiAsynchRequestor::NewL(CStifLogger* aLog,MRequestCompleteObserver* aObserver)
	{
	CPosApiAsynchRequestor* self = new (ELeave) CPosApiAsynchRequestor(aLog,aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); 
    return self; 
	}
	
CPosApiAsynchRequestor::~CPosApiAsynchRequestor()
	{
		Cancel();
		delete iPosition;
		delete iPOSSession;
		delete iPosHandler;
		delete iOMASuplInfoRequestList;
	}
 
void CPosApiAsynchRequestor::ConstructL()
	{
	
	iLastResult = KErrGeneral;	
	RImplInfoPtrArray pluginArray;

	REComSession& EComServerSession = REComSession::OpenL();
	
	CleanupStack::PushL(TCleanupItem(*ResetAndDestroy, &pluginArray));
	REComSession::ListImplementationsL(KOMAPOSHandlerInterfaceUid, pluginArray);
	
	if(pluginArray.Count() > 0)
		{
			iLog->Log(_L("Plugins Found for POS...")); 
			const CImplementationInformation* pluginInformation = pluginArray[0];
			implementationUid = pluginInformation->ImplementationUid();
			TRAPD(err,iPosHandler = COMASuplPosHandlerBase::NewL(implementationUid)); 
			TBuf<32> buf(_L("Plugins loaded with error ..."));				
			buf.AppendNum(err);
			iLog->Log(buf); 		
			User::LeaveIfError(err);
		}
	else
		{
			iLog->Log(_L("No Plugins Found for POS...")); 
		}	
		
	CleanupStack::PopAndDestroy(1);
	EComServerSession.Close();
		
	}
void CPosApiAsynchRequestor::RunL()
	{
			iLastResult = iStatus.Int();
			CActiveScheduler::Stop();	
 	}

void CPosApiAsynchRequestor::DoCancel()
	{
			
	}
	
TInt CPosApiAsynchRequestor::RunError(TInt aError)
	{
		iLog->Log(_L("Completed with Error %d"),aError);
		return aError;
	}

void CPosApiAsynchRequestor::COMASuplPosHandlerBase_InitializeL()
	{

		if(iPosHandler)
			{
				iPosHandler->InitializeL(iStatus);
				SetActive();
			}
		else
			{
				User::Leave(-1);	
			}
	}

void CPosApiAsynchRequestor::COMASuplPosHandlerBase_CancelInitializeL()
	{
		iPosHandler->CancelInitialize();
	}

TInt CPosApiAsynchRequestor::COMASuplPosHandlerBase_CreateNewSessionL()
	{
			if(iPosHandler)
			{
				iLog->Log(_L("Creating POSSession...")); 
				iPOSSession = iPosHandler->CreateNewSessionL(this);
				if(iPOSSession)
					{
						iLastResult = KErrNone;	
						return KErrNone;
					}
				else
					{
						iLastResult = KErrGeneral;	
						return KErrNotFound;				
					}	
				
			}
		else
			{
				iLog->Log(_L("No POS plugin exists...")); 				
				return KErrNotFound;				
			}
	}


TInt CPosApiAsynchRequestor::COMASuplPosHandlerBase_VersionL()
	{
		TInt ver = iPosHandler->VersionL();
		if(ver == 1)
			{
				return KErrNone;
			}
		else
			{
				return KErrGeneral;
			}
	}


TInt CPosApiAsynchRequestor::COMASuplPosHandlerBase_ImplementationUID()
	{
		TUid implUid = iPosHandler->ImplementationUID();
		if(implementationUid == implUid)
			{
				return KErrNone;
			}
		else
			{
				return KErrGeneral;
			}

	}


void CPosApiAsynchRequestor::COMASuplPosSessionBase_InitializeL()
	{

		TInt requestid = 100;
		if(iPOSSession)
			{
				iPOSSession->InitializeL(requestid,iStatus);
				SetActive();
			}
		else
			{
				iLastResult = KErrNotFound;	
				User::Leave(-1);	
			}
	}
	
void CPosApiAsynchRequestor::COMASuplPosSessionBase_CancelInitializeL()
	{
		iPOSSession->CancelInitialize();
	}

void CPosApiAsynchRequestor::COMASuplPosSessionBase_HandleSuplPosMessageL()
	{
		_LIT8(KPayLoad,"This is POS Pay Load Data");
		TBufC8<200> bufPayLoadData(KPayLoad);
		COMASuplPosPayload::TOMASuplPosPayloadType posPayloadType = COMASuplPosPayload::ETIA801;
		
		COMASuplPosPayload* posPayload = COMASuplPosPayload::NewL();
		HBufC8* ptrPayLoadData = bufPayLoadData.AllocL();
		posPayload->SetPosPayload(ptrPayLoadData,posPayloadType);

		iPOSSession->HandleSuplPosMessageL(posPayload);
		delete posPayload;
		iLastResult = KErrNone;	
	}

void CPosApiAsynchRequestor::COMASuplPosSessionBase_GetSuplInfoL()
	{
		iOMASuplInfoRequestList = COMASuplInfoRequestList::NewL();
		iPOSSession->GetSuplInfoL(iOMASuplInfoRequestList,iStatus);
		SetActive();
	}

void CPosApiAsynchRequestor::COMASuplPosSessionBase_CancelSuplInfoRequest()
	{
		iPOSSession->CancelSuplInfoRequest();
	}

void CPosApiAsynchRequestor::COMASuplPosSessionBase_GetPositionL()
	{
		iPosition = COMASuplPosition::NewL();
		iPOSSession->GetPositionL(iStatus,iPosition);
		SetActive();
	}

void CPosApiAsynchRequestor::COMASuplPosSessionBase_CancelGetPosition()
	{
		iPOSSession->CancelGetPosition();
	}

void CPosApiAsynchRequestor::COMASuplPosSessionBase_SessionEndL()
	{
		iPOSSession->SessionEnd();
		iLastResult = KErrNone;
	}
	
void CPosApiAsynchRequestor::COMASuplPosSessionBase_SuplStateL()
	{
		COMASuplPosSessionBase::TOMASuplMsg SuplMsg = COMASuplPosSessionBase::EOMASuplStart;
		COMASuplPosSessionBase::TOMASuplMsgState SuplMsgState = COMASuplPosSessionBase::EOMASuplCreating;
		
		COMASuplPosSessionBase::TOMASuplMsg recSuplMsg;
		COMASuplPosSessionBase::TOMASuplMsgState recSuplMsgState;

		iPOSSession->SetSuplState(SuplMsg,SuplMsgState);
		iPOSSession->GetSuplState(recSuplMsg,recSuplMsgState);
		if(recSuplMsg == SuplMsg && SuplMsgState == recSuplMsgState)
			iLastResult = KErrNone;
		else
			iLastResult = KErrArgument;
	}

TInt CPosApiAsynchRequestor::COMASuplPosHandlerBase_CreateNewSession_SuplObserverL()
	{
			if(iPosHandler)
			{
				iLog->Log(_L("Creating POSSession...")); 
				iPOSSession = iPosHandler->CreateNewSessionL(this);
				if(iPOSSession)
					{
						MOMASuplObserver* observer = iPOSSession->SuplObserver();
						if(observer)
						{
							iLastResult = KErrNone;	
							return KErrNone;
	
						}
						else
						{
							iLastResult = KErrGeneral;
							return KErrGeneral;	
						}
					}
				else
					{
						iLastResult = KErrGeneral;	
						return KErrNotFound;				
					}	
				
			}
		else
			{
				iLog->Log(_L("No POS plugin exists...")); 				
				return KErrNotFound;				
			}
	}
