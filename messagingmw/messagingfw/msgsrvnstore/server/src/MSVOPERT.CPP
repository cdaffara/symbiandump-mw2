// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include <e32std.h>

#include "MSVSTD.H"

#include "MTSR.H"
#include "MSVENTRY.H"
#include "MSVSERV.H"
#include "MSVROPS.H"
#include "MSVPANIC.H"


//**********************************
// CMsvServerOperation
//**********************************

CMsvServerOperation::CMsvServerOperation(const RMessage2& aMessage, TMsvOp aId, TUid aMtmUid, TMsvId aServiceId, TInt aSessionId)
: CActive(EPriorityStandard), iMessage(aMessage), iId(aId), iMtmUid(aMtmUid), iServiceId(aServiceId), iSessionId(aSessionId), iState(EMsvOperationNull)
//
//
//
	{
	__DECLARE_NAME(_S("CMsvServerOperation"));
	}


CMsvServerOperation::~CMsvServerOperation()
//
//
//
	{
	}

/** 
This method should be overridden by SystemProgress() in other server MTMs to populate the
TMsvSystemProgress structure.
@param aOutSysProg The TMsvSystemProgress structure to be populated by the server
@return KErrExtensionNotSupported
*/
TInt CMsvServerOperation::SystemProgress(TMsvSystemProgress& /*aSysProgress*/)
	{
	return KErrExtensionNotSupported;
	}

//**********************************
// CMsvMtmOperation
//**********************************

CMsvMtmOperation* CMsvMtmOperation::NewL(const RMessage2& aMessage, TMsvOp aId, TUid aMtmUid, TMsvId aServiceId, TInt aSessionId, MMsvOperationObserver& aOpObserver)
//
//
//
	{
	CMsvMtmOperation* self = new(ELeave) CMsvMtmOperation(aMessage, aId, aMtmUid, aServiceId, aSessionId, aOpObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


CMsvMtmOperation::CMsvMtmOperation(const RMessage2& aMessage, TMsvOp aId, TUid aMtmUid, TMsvId aServiceId, TInt aSessionId, MMsvOperationObserver& aOpObserver)
: CMsvServerOperation(aMessage, aId, aMtmUid, aServiceId, aSessionId), iOpType(EMtmOpNone), iOpObserver(aOpObserver)
//
//
//
	{
	__DECLARE_NAME(_S("CMsvMtmOperation"));
	}


void CMsvMtmOperation::ConstructL()
//
//
//
	{
	CActiveScheduler::Add(this);
	}


void CMsvMtmOperation::Start(CBaseServerMtm& aServerMtm, TBool aCapabilityCheck)
//
//
//
	{
	iServerMtm = &aServerMtm;
	TRAPD(leave, DoStartL(aCapabilityCheck));
	if (leave)
		{
		iStatus=KRequestPending;
		TRequestStatus* st = &iStatus;
		User::RequestComplete(st, leave);
		}
	SetActive();
	}


void CMsvMtmOperation::Failed(TInt aError)
//
//
//
	{
	iMessage.Complete(aError);
	SetState(EMsvOperationFailed);
	}


void CMsvMtmOperation::DoStartL(TBool aCapabilityCheck)
//
//
//
	{
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
	iServerMtm->ClientThreadInfo(iThreadId, aCapabilityCheck);
#else
	// For versions which don't support SYMBIAN_USER_PROMPT_SERVICE, 
	// this is needed to stop compiler warnings .
	aCapabilityCheck = aCapabilityCheck;	
#endif	
	switch (iOpType)
		{
		case EMtmOpCopyFromLocal:
			iServerMtm->CopyFromLocalL(*iSelection, iIntParam, iStatus);
			break;
		case EMtmOpCopyToLocal:
			iServerMtm->CopyToLocalL(*iSelection, iIntParam, iStatus);
			break;
		case EMtmOpCopyWithinService:
			iServerMtm->CopyWithinServiceL(*iSelection, iIntParam, iStatus);
			break;
		case EMtmOpMoveFromLocal:
			iServerMtm->MoveFromLocalL(*iSelection, iIntParam, iStatus);
			break;
		case EMtmOpMoveToLocal:
			iServerMtm->MoveToLocalL(*iSelection, iIntParam, iStatus);
			break;
		case EMtmOpMoveWithinService:
			iServerMtm->MoveWithinServiceL(*iSelection, iIntParam, iStatus);
			break;
		case EMtmOpDeleteAll:
			iServerMtm->DeleteAllL(*iSelection, iStatus);
			break;
		case EMtmOpCreate:
			iServerMtm->CreateL(*iEntry, iStatus);
			break;
		case EMtmOpChange:
			iServerMtm->ChangeL(*iEntry, iStatus);
			break;
		case EMtmOpCommand:
			iServerMtm->StartCommandL(*iSelection, iIntParam, *iDesParam, iStatus);
			break;
        case EMtmOpChangeEntries:
            iServerMtm->ChangeEntriesL(*iSelection,iIntParam,iStatus);
            break;
		default:
			PanicServer(EMsvUnknownMtmOpType);
			break;
		}
	}

CMsvMtmOperation::~CMsvMtmOperation()
//
//
//
	{
	Cancel();


	switch (State())
		{
		case EMsvOperationQueued:
			__ASSERT_DEBUG(iServerMtm==NULL, PanicServer(EMsvDeletingMtmOperation1));
			iMessage.Complete(KErrCancel);
			iOpObserver.OperationCancelled(*this);
			break;
		case EMsvOperationCompleted:
			__ASSERT_DEBUG(iServerMtm, PanicServer(EMsvDeletingMtmOperation2));
			iOpObserver.OperationCompleted(ServiceId(), Id()); 
			break;
		default:
			__ASSERT_DEBUG(State()!=EMsvOperationRunning, PanicServer(EMsvDeletingMtmOperation3));
			break;
		}

	delete iEntry;
	delete iSelection;
	delete iBuf1;
	delete iBuf2;
	delete iDesParam;
	}


void CMsvMtmOperation::DoCancel()
//
//
//
	{
	__ASSERT_DEBUG(State()==EMsvOperationRunning, PanicServer(EMsvCancellingNonRunningOp));
	iServerMtm->Cancel();
	// inform client of the cancellation (or completion)
	iMessage.Complete(iStatus.Int());
	SetState(EMsvOperationCompleted);
	}


void CMsvMtmOperation::RunL()
//
//
//
	{
	__ASSERT_DEBUG(State()==EMsvOperationRunning, PanicServer(EMsvCompletedNonRunningOp));
	// inform client of the completion
	iMessage.Complete(iStatus.Int());
	SetState(EMsvOperationCompleted);
	}


const TDesC8& CMsvMtmOperation::Progress()
//
//
//
	{
	__ASSERT_DEBUG(iServerMtm, PanicServer(EMsvProgressOnQueuedOp));
	return iServerMtm->Progress();
	}

/** 
Call one of the server MTMs to populate the TMsvSystemProgress structure.
@param aOutSysProg The TMsvSystemProgress structure to be populated by the server
@return error if SystemProgress method fails
*/
TInt CMsvMtmOperation::SystemProgress(TMsvSystemProgress& aSysProgress)
//
//
//
	{
	__ASSERT_DEBUG(iServerMtm, PanicServer(EMsvSystemProgressOnQueuedOp));
	return iServerMtm->SystemProgress(aSysProgress);
	}

void CMsvMtmOperation::StoreParametersL(TMtmOpType aOpType, const TMsvEntry& aNewEntry)
//
//
//
	{
	iOpType = aOpType;
	iEntry = new (ELeave) TMsvEntry(aNewEntry);
	iBuf1 = HBufC::NewL(iEntry->iDescription.Length());
	iBuf1->Des() = iEntry->iDescription;
	iEntry->iDescription.Set(iBuf1->Des());
	iBuf2 = HBufC::NewL(iEntry->iDetails.Length());
	iBuf2->Des() = iEntry->iDetails;
	iEntry->iDetails.Set(iBuf2->Des());
	}

void CMsvMtmOperation::StoreParameters(TMtmOpType aOpType, CMsvEntrySelection* aSelection, TInt aIntParam, HBufC8* aDesParam)
//
//
//
	{
	iOpType = aOpType;
	iSelection = aSelection;
	iIntParam = aIntParam;
	iDesParam = aDesParam;
	}

