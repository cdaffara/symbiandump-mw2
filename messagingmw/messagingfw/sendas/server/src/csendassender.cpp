// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// csendassender.cpp
//

#include "csendassender.h"

#include <msvids.h>
#include <mtclbase.h>
#include <tmsvpackednotifierrequest.h>
#include <s32mem.h>

#include "msendassenderobserver.h"
#include "tsendasserverpanic.h"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

// Buffer for IPC messages to notifier
const TInt KNotifierBufferLength 	= 60;
const TInt KNotifierBufferIncrement = 20;

CSendAsSender* CSendAsSender::NewL(MSendAsSenderObserver& aObserver)
	{
	CSendAsSender* self = new (ELeave) CSendAsSender(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSendAsSender::~CSendAsSender()
	{
	// do not notify observer during deletion.
	iObserver = NULL;
	Cancel();

	iNotifier.Close();
	delete iClientMtm;
	delete iOperation;
	delete iBuffer;
	delete iSelection;
	// Close UPS session
#if (defined SYMBIAN_USER_PROMPT_SERVICE)	
    iUpsSubsession.Close();
	iUpsSession.Close();
#endif	
	}

CSendAsSender::CSendAsSender(MSendAsSenderObserver& aObserver)
: CActive(CActive::EPriorityStandard), iObserver(&aObserver)
	{
	CActiveScheduler::Add(this);
	}
	
void CSendAsSender::ConstructL()
	{
	iBuffer = HBufC8::NewL(KNotifierBufferLength);
	User::LeaveIfError(iNotifier.Connect());		
	iSelection = new (ELeave) CMsvEntrySelection();
	}
	
/**
Sets the client MTM to use for move and send operations.
 
@param	aClientMtm
The client MTM.
*/
void CSendAsSender::SetClientMtm(CBaseMtm& aClientMtm)
	{
	__ASSERT_ALWAYS( iState == ESenderIdle, User::Invariant() );
	__ASSERT_ALWAYS( iClientMtm == NULL, User::Invariant() );
	__ASSERT_ALWAYS( iSelection != NULL, User::Invariant() );
	
	iClientMtm = &aClientMtm;
	}

/**
Populates the entry selection from the client MTM's entry.
*/
void CSendAsSender::SetEntrySelectionL()
	{
	iSelection->Reset();
	iSelection->AppendL(iClientMtm->Entry().EntryId());
	}

/**
Initiates an confirmed / unconfirmed message send operation.
*/
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
void CSendAsSender::SendMessageL(const RMessage2 aMessage, TBool aHasCapability)
	{
	__ASSERT_ALWAYS( iState == ESenderIdle, User::Invariant() );
	__ASSERT_ALWAYS( iClientMtm != NULL, User::Invariant() );
	
	iState = ESenderAuthoriseSend;
	// Serialise the recipient list
	const CMsvRecipientList& rcptList = iClientMtm->AddresseeList();
	const MDesC16Array& recipient = rcptList.RecipientList();
	
	TInt rcptCount = recipient.MdcaCount();
	TInt size = 0;
	for(TInt i=0;i<rcptCount; ++i)
		{
		size += recipient.MdcaPoint(i).Length();
		}		
	
 	_LIT16(KComma, ",");
 		
 	RBuf16 buffer;
 	buffer.CreateL(size+ rcptCount);
 	CleanupClosePushL(buffer);
 	if(rcptCount > 0)
 		{
 		buffer.Append(recipient.MdcaPoint(0));	
 		}
 		
 	for(TInt i=1;i<rcptCount; ++i)
 		{
 		buffer.Append(KComma);
 		buffer.Append(recipient.MdcaPoint(i));
 		}
   	
   	RThread clientThread;
   	aMessage.Client(clientThread);
   	
   	// Connect to UPS service.....
   	User::LeaveIfError(iUpsSession.Connect());
 	
 	User::LeaveIfError(iUpsSubsession.Initialise(iUpsSession, clientThread));
   	
   	iDecision = EUpsDecNo;
 	iUpsSubsession.Authorise(aHasCapability, KUidSendAs2Service, buffer, iDecision, iStatus);
   	SetActive();
 	CleanupStack::PopAndDestroy(&buffer);
	}
#endif	

/**
Initiates an unconfirmed message send operation.
*/
void CSendAsSender::SendMessage()
	{
	__ASSERT_ALWAYS( iState == ESenderIdle, User::Invariant() );
	__ASSERT_ALWAYS( iClientMtm != NULL, User::Invariant() );
	
	// update state and auto-complete - send process started by RunL()
	iState = ESenderStartingUnconfirmedSend;
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}
	
/**
Initiates a confirmed message send operation.
 
@param	aSecurityInfo
The security info associated with the message.

@param  aNotifierUid
The UID of the notifier plugin.
*/
void CSendAsSender::SendMessage(const TSecurityInfo& aSecurityInfo, const TUid& aNotifierUid)
	{
	__ASSERT_ALWAYS( iState == ESenderIdle, User::Invariant() );
	__ASSERT_ALWAYS( iClientMtm != NULL, User::Invariant() );
	
	// confirmed send - use notifier to prompt the user for confirmation.
	iSecurityInfo = aSecurityInfo;
	iNotifierUid  = aNotifierUid;

	// update state and auto-complete
	iState = ESenderStartingConfirmedSend;
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}
	
/**
Provides progress information for the send operation.
 
@param	aProgress
The operation progerss object.
*/
void CSendAsSender::ProgressL(TMsvSendOperationProgress& aProgress)
	{
	switch( iState )
		{
	case ESenderStartingUnconfirmedSend:
	case ESenderStartingConfirmedSend:
	case ESenderIdle:
	case ESenderConfirming:
	case ESenderMoving:
	case ESenderAuthoriseSend:
		{
		aProgress().iState = CMsvSendOperation::ESendStateWaitingToSend;
		} break;
	case ESenderSending:
		{
		// when sending, operation returns a CMsvSendOperation which returns the
		// correct progress.
		const TDesC8& opProgress = iOperation->ProgressL();
		__ASSERT_ALWAYS(opProgress.Size() == sizeof(CMsvSendOperation::TSendOperationProgress), 
						TSendAsServerPanic::Panic(TSendAsServerPanic::ESendAsBadDescriptor));
		TMsvSendOperationProgress& progress = (TMsvSendOperationProgress&) opProgress;
		aProgress = progress;
		} break;
	case ESenderDone:
		{
		FinalProgress(aProgress);
		} break;
	default:
		User::Invariant();
		break;
		}
	}
	
void CSendAsSender::FinalProgress(TMsvSendOperationProgress& aProgress)
	{
	__ASSERT_ALWAYS( iState == ESenderDone, User::Invariant() );
	
	aProgress = iFinalProgress;
	}

/**
Deletes the current operation
*/
void CSendAsSender::DeleteOperation()
	{
	delete iOperation;
	iOperation = NULL;
	}

/**
Invokes an asynchronous move operation.
Moves the message from current location (drafts folder) to the outbox.
*/
void CSendAsSender::InvokeMoveOperationL()
	{
	iEntry = iClientMtm->Entry().Entry();
	iClientMtm->Entry().SetEntryL(iEntry.Parent());

	DeleteOperation();
	iOperation = iClientMtm->Entry().MoveL(iEntry.Id(), KMsvGlobalOutBoxIndexEntryId, iStatus);
	SetActive();

	iState = ESenderMoving;
	}

/**
Invokes an asychronous send operation.
*/
void CSendAsSender::InvokeSendOperationL()
	{
	// Buffer required for call to InvokeAsyncFunction, unused.
	TBuf8<1> buf;
	iClientMtm->Entry().SetEntryL(iEntry.Id());
	DeleteOperation();
	// Note: This operation must be a CMsvSendOperation as TMsvSendOperationProgress is needed
	iOperation = iClientMtm->InvokeAsyncFunctionL(KMTMStandardFunctionsSendMessage, *iSelection, buf, iStatus);
	SetActive();
	
	iState = ESenderSending;	
	}

/**
Invokes an asynchronous notification request.
*/
void CSendAsSender::InvokeConfirmOperationL()
	{
	// Use plugin notifier to get the send confirmation.
	TMsvPackedNotifierRequest packedInfo(iBuffer);
	while(packedInfo.Pack(*iSelection, iSecurityInfo) != KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().Zero(); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KNotifierBufferIncrement);
		}

	iNotifier.StartNotifierAndGetResponse(iStatus, iNotifierUid, *iBuffer, iNotifierResult);
	SetActive();
	
	iState = ESenderConfirming;
	}

/**
Notifies the observer that the operation has completed.
@param  aError 	The error code.
*/
void CSendAsSender::Complete(TInt aError)
	{
	iState = ESenderDone; // This must be set before notifying observer
	if( iObserver != NULL )
		{
		// notify observer that the send has completed.
		iObserver->SenderComplete(aError);
		}
	}
	
/**
Methods from CActive
Handles a request completion event.
Implements the CSendAsSender state machine.

@see CActive::RunL()
*/
void CSendAsSender::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	switch( iState )
		{
	case ESenderStartingConfirmedSend:
		{
		SetEntrySelectionL();
		InvokeConfirmOperationL();
		}break;
	case ESenderStartingUnconfirmedSend:
		{
		SetEntrySelectionL();
		} // fall through
	case ESenderConfirming:
		{
		InvokeMoveOperationL();			
		} break;
#if (defined SYMBIAN_USER_PROMPT_SERVICE)		
	case ESenderAuthoriseSend:
		{
		if(iDecision != EUpsDecYes)
			{
			Complete(KErrPermissionDenied);
			}
		else
			{
			SetEntrySelectionL();
			InvokeMoveOperationL();	
			}	
						
		}break;
#endif		
	case ESenderMoving:
		{
		InvokeSendOperationL();			
		} break;
	case ESenderSending:
		{
		TMsvSendOperationProgress& progress = (TMsvSendOperationProgress&)iOperation->FinalProgress();
		iFinalProgress = progress;
		Complete(KErrNone);
		} break;
	case ESenderIdle:
	case ESenderDone:
	default:
		User::Invariant();
		}
	}
	
/** 
Implements cancellation of an outstanding request.
@see CActive::DoCancel()
*/
void CSendAsSender::DoCancel()
	{
	switch( iState )
		{
		case ESenderConfirming:
			{
			iNotifier.NotifyCancel();
			}break;
		case ESenderMoving:
		case ESenderSending:
			{
			__ASSERT_ALWAYS( iOperation != NULL, User::Invariant() );
			iOperation->Cancel();
			} break;
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
		case ESenderAuthoriseSend:
			{
			iUpsSubsession.Close();
			}break;
#endif
		case ESenderStartingConfirmedSend:
		case ESenderStartingUnconfirmedSend:	
		case ESenderIdle:	
		case ESenderDone:
		default:
			// do nothing...
			break;
			}
	Complete(KErrCancel);
	}
	
/** 
Handles leaves from the RunL method.
@see CActive::RunL()
*/
TInt CSendAsSender::RunError(TInt aError)
	{
	Complete(aError);
	return KErrNone;
	}
