/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Parent class of classes testing GetSuplInfoL
*
*/

#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplreqasstdata.h>
#include <epos_comasuplsetcapabilities.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplvelocity.h>
#include <epos_eomasuplposerrors.h>

#include "epos_comasuplgetsuplinfomanager.h"
#include "epos_comasuplpostesterlogger.h"


_LIT8(KValueofSetCapCount, "Info: Resetting number of SetCapability objects\
 in request list to ");

_LIT8(KValueOfReqAsstDataCount, "Info: Resetting number of Request \
Assistance Data objects in request list to ");

_LIT8(KValueOfPositionCount, "Info: Resetting number of Position objects \
in request list to ");

_LIT8(KValueOfVelocityCount, "Info: Resetting number of Velocity objects \
in request list to ");

_LIT8(KValueOfPayloadCount, "Info: Resetting number of Payload objects in \
request list to ");



// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplGetSuplInfoManager::~COMASuplGetSuplInfoManager()
	{
	delete iSessionBase;
	if(iCreatedTypeSeq)
		{
		for(TInt counter = 0; counter < iCreatedTypeSeq->Count(); counter++)
			{
			iCreatedTypeSeq->Delete(counter);
			}
		}
	delete iCreatedTypeSeq;
	delete iRequestList;
	}
// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
COMASuplGetSuplInfoManager::COMASuplGetSuplInfoManager(
									COMASuplPosHandlerBase& aPosHandler
									, COMASuplPosTesterCategory* aCallBack )
									:COMASuplPosTesterCategory( aCallBack )
									, iPosHandler(aPosHandler)
									, iRequestID(-1)
									
	{
	}

// -----------------------------------------------------------------------------
// COMASuplGetSuplInfoManager::CreateRequestListL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplInfoRequestList& COMASuplGetSuplInfoManager::CreateRequestListL(
										 TInt aSetCapReqCount 
										, TInt aReqAsstDataCount
										, TInt aPositionCount
										, TInt aVelocityCount
										, TInt aPayLoadCount)
	{
	TInt counter = 0;
	ValidateAndSetArguments(aSetCapReqCount, aReqAsstDataCount
						, aPositionCount, aVelocityCount, aPayLoadCount);

	TInt arraySize = aSetCapReqCount + aReqAsstDataCount + aPositionCount
					+ aVelocityCount + aPayLoadCount;
	if(arraySize <= 0)
		{
		User::Leave(KErrUnderflow);
		}
	ResetLists();
	
	iCreatedTypeSeq = new(ELeave)CArrayFixFlat< TInt >(arraySize);
	iRequestList = COMASuplInfoRequestList::NewL();
	iCreatedTypeSeq->SetReserveL(arraySize);
	for( ; counter < aSetCapReqCount; counter++ )
		{
		COMASuplInfoRequest* request = COMASuplSETCapabilities::NewL();
		iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplSETCapabilities);
		iRequestList->Append( request );
		}

	for( counter = 0; counter < aReqAsstDataCount; counter++ )
		{
		COMASuplInfoRequest* request = COMASuplReqAsstData::NewL();
		iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplReqAsstData);
		iRequestList->Append( request );
		}

	for( counter = 0; counter < aPositionCount; counter++ )
		{
		COMASuplInfoRequest* request = COMASuplPosition::NewL();
		iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplPosition);
		iRequestList->Append( request );
		}

	for( counter = 0; counter < aVelocityCount; counter++ )
		{
		COMASuplInfoRequest* request = COMASuplVelocity::NewL();
		iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplVelocity);
		iRequestList->Append( request );
		}

	for( counter = 0; counter < aPayLoadCount; counter++ )
		{
		COMASuplInfoRequest* request = COMASuplPosPayload::NewL();
		iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplPosPayload);
		iRequestList->Append( request );
		}

	return *iRequestList;
	}

// -----------------------------------------------------------------------------
// COMASuplGetSuplInfoManager::CreateRequestListL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplInfoRequestList& COMASuplGetSuplInfoManager::CreateRequestListL()
	{
	return CreateRequestListL(iSetCapReqCount, iReqAsstDataCount
						, iPositionCount, iVelocityCount, iPayLoadCount);
	}

// -----------------------------------------------------------------------------
// COMASuplGetSuplInfoManager::ValidateAndSetArguments
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplGetSuplInfoManager::ValidateAndSetArguments(
											TInt& aSetCapReqCount 
										, TInt& aReqAsstDataCount
										, TInt& aPositionCount
										, TInt& aVelocityCount
										, TInt& aPayLoadCount )
	{
	//validate aSetCapReqCount
	ValidateAndSetArguments(aSetCapReqCount, KValueofSetCapCount
							, iSetCapReqCount);
	
	//validate aReqAsstDataCount
	ValidateAndSetArguments(aReqAsstDataCount, KValueOfReqAsstDataCount
							, iReqAsstDataCount);
	
	//validate aPositionCount
	ValidateAndSetArguments(aPositionCount,KValueOfPositionCount
							, iPositionCount);
	
	//validate aVelocityCount
	ValidateAndSetArguments(aVelocityCount, KValueOfVelocityCount
							, iVelocityCount);

	//validate aPayLoadCount
	ValidateAndSetArguments(aPayLoadCount, KValueOfPayloadCount
							, iPayLoadCount);
	}

// -----------------------------------------------------------------------------
// COMASuplGetSuplInfoManager::ValidateAndSetArguments
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplGetSuplInfoManager::ValidateAndSetArguments(
											TInt& aObjCount
											, const TDesC8& aMsg
											, TInt& aForNextReq)
	{
	if(aObjCount > 1)
		{
		iInfo++;
		aObjCount = 1;
		iLogger->WriteStatus(aMsg, aObjCount, iTestNo, &KNULL());
		}
	else if(aObjCount < 0)
		{
		aObjCount = 0;
		iInfo++;
		iLogger->WriteStatus(aMsg, aObjCount, iTestNo, &KNULL());
		}
		else
			{
			//Do nothing
			}
	aForNextReq = aObjCount;
	}

// -----------------------------------------------------------------------------
// COMASuplGetSuplInfoManager::ResetLists
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplGetSuplInfoManager::ResetLists()
	{
	if(iRequestList)
		{
		delete iRequestList;
		}
	iRequestList = NULL;
	if(iCreatedTypeSeq)
		{
		delete iCreatedTypeSeq;
		}
	iCreatedTypeSeq = NULL;
	}

// -----------------------------------------------------------------------------
// COMASuplGetSuplInfoManager::MatchLists
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplGetSuplInfoManager::MatchLists(TInt& aDiffIndex)
	{
	if(iCreatedTypeSeq->Count() != iRequestList->GetCount())
		{
		aDiffIndex = 0;
		return EFalse;
		}

	for(TInt count = 0; count < iCreatedTypeSeq->Count(); count++)
		{
		TInt typeSeq = iCreatedTypeSeq->At(count);
		TInt reqEltType = iRequestList->GetElement(count)->Type();
		if(typeSeq != reqEltType)
			{
			aDiffIndex = count;
			return EFalse;
			}
		}

	return ETrue;
	}
