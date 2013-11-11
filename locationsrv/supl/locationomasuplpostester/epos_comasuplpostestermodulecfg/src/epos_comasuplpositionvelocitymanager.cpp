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
* Description:  Handles test execution and monitors user input.
*
*/

#include <epos_comasuplposhandlerbase.h>
#include <epos_comasuplpossessionbase.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplinforequestlist.h>
#include <epos_comasuplposition.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplvelocity.h>
#include <epos_eomasuplposerrors.h>
#include <stiflogger.h>


#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplpostesterlogger.h"
#include "epos_comasuplmessagefilereader.h"
#include "epos_comasuplpositionvelocitymanager.h"
#include "epos_comasuplmessageinfo.h"
#include "epos_comasuplpositionvelocitymanager.h"
#include "epos_tomasuplmatchdatatypes.h"

/** Constant used for logging.*/
_LIT8(KPositionCreationFailed, "Error: Could Not create COMASuplPosition object.");

/** Constant used for logging.*/
_LIT8(KMsgSendingFailed, "Error: Could not send message through HandleSuplPosMessageL to\
 POS message plug-in");
 
 /** Constant used for logging.*/
 _LIT8(KWaitNotAllowed, "Error: \"wait\" instruction should not follow a\
 \"wait\" for which POS Message Plug-in has reported KErrOMASuplLastPosMsg.");
 
 /** Constant used for logging.*/
 _LIT8(KSkipInstruction, "Warning: Skipping contiguous wait instructions.");
 
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
COMASuplPositionVelocityManager::~COMASuplPositionVelocityManager()
	{
	delete iMsgFileReader;

	delete iRequestList;
	if(iCreatedTypeSeq)
		{
		delete iCreatedTypeSeq;
		}
	delete iSessionBase;

	delete iCurrentPosition;
	delete iPreviousPosition;
	
	if(iMsgInfoList)
		{
		iMsgInfoList->ResetAndDestroy();
		delete iMsgInfoList;
		}
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
COMASuplPositionVelocityManager
::COMASuplPositionVelocityManager(COMASuplPosTesterCategory* aCallBack
									, TFileName& aMsgFileName)
								:COMASuplPosTesterCategory(aCallBack)
								, iMsgFileName(aMsgFileName)
	{
	iExistingSessionTestStarted = EFalse;
	iMsgExecuted = 0;
	}

// ---------------------------------------------------------------------------
// Initializes COMASuplPosTesterCategory. Must be called before
// starting test.
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::InitializeL(COMASuplTestHandler* aTestHandler
								, COMASuplPosTesterLogger& aLogger
								, TInt aTestNo)
	{
	iTestHandler = aTestHandler;
	iLogger = &aLogger;
	iTestNo = aTestNo;
	iMsgFileReader = COMASuplMessageFileReader::NewL(iMsgFileName, iLogger);
	iInfo++; //info about message file name
	}

// ---------------------------------------------------------------------------
// Creates request list for GetSuplInfoL()
// ---------------------------------------------------------------------------
//
COMASuplInfoRequestList& COMASuplPositionVelocityManager::CreateRequestListL()
	{
	TInt arraySize = 3;
	iCreatedTypeSeq = new(ELeave)CArrayFixFlat< TInt >(arraySize);
	iCreatedTypeSeq->SetReserveL(arraySize);
	iRequestList = COMASuplInfoRequestList::NewL();

	COMASuplInfoRequest* requestVel = COMASuplVelocity::NewL();
	CleanupStack::PushL(requestVel);
	iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplVelocity);
	iRequestList->Append(requestVel);
	CleanupStack::Pop(requestVel);

	COMASuplInfoRequest* requestPosition = COMASuplPosition::NewL();
	CleanupStack::PushL(requestPosition);
	iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplPosition);
	iRequestList->Append(requestPosition);
	CleanupStack::Pop(requestPosition);

	COMASuplInfoRequest* requestPayload = COMASuplPosPayload::NewL();
	CleanupStack::PushL(requestPayload);
	iCreatedTypeSeq->AppendL(COMASuplInfoRequest::EOMASuplPosPayload);
	iRequestList->Append(requestPayload);
	CleanupStack::Pop(requestPayload);

	return *iRequestList;
	}

// ---------------------------------------------------------------------------
// Invokes GetSuplInfoL()
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::RequestForInfoL()
	{
	//Delete Previous lists
	ResetLists();
	//Create Req list
	TRAPD(err, CreateRequestListL());//One object each(default value)
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteStatus(KLeaveRequestListCreation(), err, iTestNo);
		TestingAborted(KNULL);
		return;
		}
	__ASSERT_ALWAYS( !(IsActive()),User::Panic( KPanicSetActive, 1 ) );
	iInfo++;
	iLogger->WriteLine(_L8("Info: Requesting for GetSuplInfoL"), iTestNo);
	iSessionBase->GetSuplInfoL(iRequestList, iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Handles completion of GetSuplInfoL()
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::ProcessRequestCompleteL(TRequestStatus&
														 aStatus)
	{
	TInt diffIndex = -1;
	TBool match = MatchLists(diffIndex);
	if(!match)
		{
		iError++;
		iLogger->WriteLine(_L8("Error: Types in Sent list and received list Mismatch."), iTestNo);
		TestingAborted(KNULL);
		return;
		}

	if(KErrNone == iStatus.Int())
		{
		iInfo++;
		iLogger->WriteStatus(_L8("Info: GetSuplInfoL completed with ")
						, iStatus.Int(), iTestNo);	
		}
	else
		{
		iWarning++;
		iLogger->WriteStatus(_L8("Warning: GetSuplInfoL completed with ")
						, iStatus.Int(), iTestNo);
		}
	
	__ASSERT_ALWAYS(!(IsActive()),User::Panic(KPanicSetActive, 1));
		//LogList
	iLogger->LogRequestList(*iRequestList);
	//If we receive Last message from POS Msg plugin then request for position
	//else if there are more instructions in message file process instruction
			//else abort testing.
	if(!LastMessage())
		{
		//if we hv more messages in Msgfile
		if(MoreMsgInMsgFileL())
			{
			ProcessNextInstructionL();
			return;
			}
		else
			{
			//POS API has not sent its last message, 
			//but we dont have any any instruction to process.
			iError++;
			iLogger->WriteLine(KMsgFileMsgExhausted, iTestNo);
			TestingAborted(KNULL);
			return;
			}
		}
	else//we got last message form POS message plugin.
		{
		//if we have more messages in Msgfile
		if(MoreMsgInMsgFileL())
			{
			//If next instruction is not a "wait" instruction
			//log an error, Move to next send message
			//Send messages
			if(IsNextInstructionWait())
				{
				iError++;
				iLogger->WriteLine(KWaitNotAllowed, iTestNo);
				iWarning++;
				iLogger->WriteLine(KSkipInstruction, iTestNo);
				
				if(MoveToNextSendInstruction())
					{
					ProcessNextInstructionL();
					}
				else
					{
					ProcessNextInstructionL();
					}
				}
			else
				{
				ProcessNextInstructionL();
				}
			return;
			}
		else
			{
			//if we dont have any message in msg file
			//Request for GetPositionL
			if(iMaxGetPositionReqCount == iGetPositionRequested)
				{
				TestingComplete(KNULL);
				return;
				}
			else
				{
				iNextInSeq = ERequestForPosition;
				DummyRequest(aStatus);
				SetActive();
				}
			return;
			}
		
		}
		
	}

// ---------------------------------------------------------------------------
// process completion of GetPositionL()
// ---------------------------------------------------------------------------
//
TInt COMASuplPositionVelocityManager::ProcessGetPositionCompleteL(TRequestStatus&
														 aStatus)
	{
	if(KErrNone == iStatus.Int())
		{
		iInfo++;
		iLogger->WriteStatus(_L8("Info: Position Request completes with ")
						, aStatus.Int(), iTestNo);	
		}
	else
		{
		iWarning++;
		iLogger->WriteStatus(_L8("Warning: Position Request completes with ")
						, aStatus.Int(), iTestNo);
		}
	
	iLogger->LogSuplPosition(iCurrentPosition);
	TInt errCode = KErrGeneral;
	TBool positionMatch = ETrue;
	//Match previous Position with new position
	if(iPreviousPosition)
		{
		positionMatch = TOMASuplMatchDataTypes::PositionsMatch(*iPreviousPosition, *iCurrentPosition);
		}
	if(positionMatch)
		{
		//Both are same 
		//If count allows 
		if(iMaxGetPositionReqCount > iGetPositionRequested)
			{
			delete iPreviousPosition;
			iPreviousPosition = NULL;
			RequestForPositionL(aStatus);
			errCode = KErrNone;
			}
		else
			{
			iInfo++;
			iLogger->WriteLine(_L8("Info: Ending Session."), iTestNo);
			iSessionBase->SessionEnd();
			if(!iExistingSessionTestStarted)
				{
				InitializeForExistingSessionTest();
				StartExistingSessionTest();
				}
			else
				{
				TestingComplete(KNULL);
				}
			}
		}
	else
		{
		//Log that retreived pos is different.
		iError++;
		iLogger->WriteLine(KPosDifferent, iTestNo);
		TestingAborted(KNULL);
		}

	return errCode;
	//Create Req list
	}

// ---------------------------------------------------------------------------
// Invokes GetPositionL()
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::RequestForPositionL(
												TRequestStatus& aStatus)
	{
	//Store Previous Position
	iPreviousPosition = iCurrentPosition;
	iGetPositionRequested++;
	//Create new position
	TRAPD(err, iCurrentPosition = COMASuplPosition::NewL());
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteLine(KPositionCreationFailed, iTestNo);
		TestingAborted(KNULL);
		return;
		}
	__ASSERT_ALWAYS(!IsActive(),User::Panic(KPanicSetActive, KErrAlreadyExists));
	iNextInSeq = ERequestForPositionComplete;
	//GetPosition
	iInfo++;
	iLogger->WriteLine(_L8("Info: Requesting for Position."), iTestNo);
	iSessionBase->GetPositionL(aStatus, iCurrentPosition);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Resets request list and iCreatedTypeSeq
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::ResetLists()
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

// ---------------------------------------------------------------------------
// Returns ETrue if there are unprocessed instructions in message file.
// Else it returns EFalse
// ---------------------------------------------------------------------------
//
TBool COMASuplPositionVelocityManager::MoreMsgInMsgFileL()
	{
	if(iMsgExecuted == iMsgInfoList->Count())
		{
		return EFalse;
		}
	else if(iMsgExecuted < iMsgInfoList->Count())
		{
		return ETrue;
		}
		else
			{
			User::Leave(KErrOverflow);
			}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// Checks if GetSuplInfoL() received last POS message from POS message plug-in
// ---------------------------------------------------------------------------
//

TBool COMASuplPositionVelocityManager::LastMessage()
	{
	if(KErrOMASuplLastPosMsg == 
		(iRequestList->GetElement(KPayLoadIndex))->Status())
		{
		return ETrue;
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// Prepares to execute tests on existing session
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::InitializeForExistingSessionTest()
	{
	iMsgExecuted = 0;
	iGetPositionRequested = 0;
	ResetLists();
	iExistingSessionTestStarted = ETrue;
	delete iPreviousPosition;
	iPreviousPosition = NULL;
	delete iCurrentPosition;
	iCurrentPosition = NULL;
	iNextInSeq = ERequestToInitializeSession;
	}

// ---------------------------------------------------------------------------
// Prepares to execute tests on existing session
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::StartExistingSessionTest()
	{
	DummyRequest(iStatus);
	SetActive();
	}

// ---------------------------------------------------------------------------
// Matches type and sequence of items in the list sent with the list received 
// from POS message plug-in
// ---------------------------------------------------------------------------
//
TBool COMASuplPositionVelocityManager::MatchLists(TInt& aDiffIndex)
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

// ---------------------------------------------------------------------------
// Sends a POS Payload to POS message plug-in
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::SendPosMessageL(COMASuplPosPayload& aPayload)
	{
	iInfo++;
	iLogger->WriteLine(KSendingPosMsg, iTestNo);
	iLogger->LogPayloadSent(&aPayload);
	iSessionBase->HandleSuplPosMessageL(&aPayload);
	}

// ---------------------------------------------------------------------------
// Reads next message from message file.
// ---------------------------------------------------------------------------
//
COMASuplMessageInfo* COMASuplPositionVelocityManager::ReadNextMessageL()
	{
	if(iMsgExecuted < iMsgInfoList->Count())
		{
		return (*iMsgInfoList)[iMsgExecuted++];
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	return NULL;
	}

// ---------------------------------------------------------------------------
// Executes message instruction.
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::ProcessMsgInstructionL(
											COMASuplMessageInfo& aMsgInfo)
	{
	switch(aMsgInfo.iOpType)
		{
			case COMASuplMessageInfo::ESend:
				{
				ProcessSendL(*(aMsgInfo.iPayload));
				break;
				}
			case COMASuplMessageInfo::EWait:
				{
				ProcessWaitL(ERequestForInfoComplete);
				break;
				}
			default:
				{
				User::Leave(KErrNotFound);
				}
		}
	}

// ---------------------------------------------------------------------------
// Executes "send" instruction
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::ProcessSendL(COMASuplPosPayload& aPayload)
	{
	TRAPD(err, SendPosMessageL(aPayload));
	if(KErrNone != err)
		{
		iError++;
		iLogger->WriteLine(KMsgSendingFailed, iTestNo);
		User::Leave(err);
		}
	ProcessNextInstructionL();
	}

// ---------------------------------------------------------------------------
// Executes "wait" instruction
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::ProcessWaitL(TExecutionSequence aNextInSeq)
	{
	iNextInSeq = aNextInSeq;
	RequestForInfoL();
	}

// ---------------------------------------------------------------------------
// Executes next instruction
// ---------------------------------------------------------------------------
//
void COMASuplPositionVelocityManager::ProcessNextInstructionL()
	{
	if(!MoreMsgInMsgFileL())
		{
		RequestForPositionL(iStatus);
		}
	else
		{
		COMASuplMessageInfo* info = ReadNextMessageL();
		ProcessMsgInstructionL(*info);
		}
	}

// ---------------------------------------------------------------------------
// returnd ETrue if next instruction is "wait"
// ---------------------------------------------------------------------------
//

TBool COMASuplPositionVelocityManager::IsNextInstructionWait()
	{
	if(iMsgInfoList->Count() > iMsgExecuted)
		{
		if(COMASuplMessageInfo::EWait == 
							(*iMsgInfoList)[iMsgExecuted]->iOpType)
			{
			return ETrue;
			}
		else
			{
			return EFalse;
			}
		}
	else
		{
		return EFalse;
		}
	
	}

// ---------------------------------------------------------------------------
// Returns ETrue and the index of next send instruction in aIndex. 
// If there is no send instruction, returns EFalse
// ---------------------------------------------------------------------------
//
TBool COMASuplPositionVelocityManager::MoveToNextSendInstruction()
	{
	TInt listLength = iMsgInfoList->Count();
	while(iMsgExecuted < listLength)
		{	
		COMASuplMessageInfo* msgInfo = (*iMsgInfoList)[iMsgExecuted];
		if(COMASuplMessageInfo::ESend == msgInfo->iOpType)
			{
			return ETrue;
			}
		iMsgExecuted++;
		}
	return EFalse;
	}
