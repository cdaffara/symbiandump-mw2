// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// csendastestservermtm.cpp
//
#include "csendastestservermtm.h"

#include <mtclbase.h>
#include <msventry.h>
#include <msvids.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

const TInt KSendAsTestServerSendDelay	= 1000000;

EXPORT_C CSendAsTestServerMtm* CSendAsTestServerMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry)
	{
	CSendAsTestServerMtm* self = new (ELeave) CSendAsTestServerMtm(aRegisteredMtmDll, aServerEntry);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CSendAsTestServerMtm::~CSendAsTestServerMtm()
	{
	Cancel();
	
	iTimer.Close();
	delete iSelection;
	}

CSendAsTestServerMtm::CSendAsTestServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry)
: CBaseServerMtm(aRegisteredMtmDll, aServerEntry)
	{
	CActiveScheduler::Add(this);
	}

void CSendAsTestServerMtm::ConstructL()
	{
	User::LeaveIfError(iTimer.CreateLocal());
	}
	
void CSendAsTestServerMtm::SendMessagesL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
	{
	aStatus = KRequestPending;
	iRequestStatus = &aStatus;
	
	iProgress.iMessagesDone = 0;
	iProgress.iMessageCount = aSelection.Count();

	// Are there any messages to send?
	if( iProgress.iMessageCount > 0 )
		{
		// Copy the selection of messages...
		delete iSelection;
		iSelection = NULL;
		iSelection = aSelection.CopyL();
		
		iProgress.iState = TSendAsTestMtmProgress::ESendAsTestInitSend;
		CompleteSelf();		
		}
	else
		{
		// Complete the request...
		DoComplete(KErrNone);
		}
	}
	
void CSendAsTestServerMtm::DoInitialiseSendL()
	{
	// Set the message send state to waiting.
	TInt count = iSelection->Count();
	for( TInt i=0; i<count; ++i )
		{
		User::LeaveIfError(iServerEntry->SetEntry(iSelection->At(i)));
		iEntry = iServerEntry->Entry();
		iEntry.SetSendingState(KMsvSendStateWaiting);
		User::LeaveIfError(iServerEntry->ChangeEntry(iEntry));
		}	
	iServerEntry->SetEntry(KMsvNullIndexEntryId);	// ignore error...
	
	// Set the current message to first message in selection and move onto the
	// next one.
	iCurrentMessage = iSelection->At(0);
	++iProgress.iMessagesDone;
	
	iProgress.iState = TSendAsTestMtmProgress::ESendAsTestSendNext;
	CompleteSelf();	
	}
	
void CSendAsTestServerMtm::DoSendMessageL()
	{
	// Get the current message.
	User::LeaveIfError(iServerEntry->SetEntry(iCurrentMessage));
	iEntry = iServerEntry->Entry();

	// Change the message send state to sending.
	iEntry.SetSendingState(KMsvSendStateSending);
	User::LeaveIfError(iServerEntry->ChangeEntry(iEntry));
	
	// Pretend to send - just wait a bit...
	iProgress.iState = TSendAsTestMtmProgress::ESendAsTestSending;
	iTimer.After(iStatus, KSendAsTestServerSendDelay);
	SetActive();
	}
	
void CSendAsTestServerMtm::DoMoveToSentFolderL()
	{
	__ASSERT_DEBUG( iCurrentMessage == iEntry.Id(), User::Invariant() );
	__ASSERT_DEBUG( iEntry == iServerEntry->Entry(), User::Invariant() );
	
	// Update the sending state and move to the sent folder.
	iEntry.SetSendingState(KMsvSendStateSent);
	User::LeaveIfError(iServerEntry->ChangeEntry(iEntry));

	User::LeaveIfError(iServerEntry->SetEntry(iEntry.Parent()));
	User::LeaveIfError(iServerEntry->MoveEntryWithinService(iEntry.Id(),KMsvSentEntryId));
	User::LeaveIfError(iServerEntry->SetEntry(KMsvNullIndexEntryId));
	
	iProgress.iState = TSendAsTestMtmProgress::ESendAsTestMoving;
	CompleteSelf();
	}
	
void CSendAsTestServerMtm::HandleSendErrorL(TInt aSendingState)
	{
	// Update the current message to reflect the error.
	User::LeaveIfError(iServerEntry->SetEntry(iCurrentMessage));
	iEntry = iServerEntry->Entry();
	iEntry.iError = iProgress.iError;
	iEntry.SetSendingState(aSendingState);
	User::LeaveIfError(iServerEntry->ChangeEntry(iEntry));
	
	// Change the state of the remaining messages.
	for(TInt i=iProgress.iMessagesDone; i<iProgress.iMessageCount; ++i )
		{
		User::LeaveIfError(iServerEntry->SetEntry(iSelection->At(i)));
		iEntry = iServerEntry->Entry();
		iEntry.iError = iProgress.iError;
		iEntry.SetSendingState(aSendingState);
		User::LeaveIfError(iServerEntry->ChangeEntry(iEntry));		
		}
	iServerEntry->SetEntry(KMsvNullIndexEntryId);	// ignore error...
	}
	
void CSendAsTestServerMtm::CompleteSelf()
	{
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();		
	}
	
/*
 * Methods from CServerBaseMtm
 */

void CSendAsTestServerMtm::CopyToLocalL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::DeleteAllL(const CMsvEntrySelection& /*aSelection*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::CreateL(TMsvEntry /*aNewEntry*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::ChangeL(TMsvEntry /*aNewEntry*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& /*aParameter*/, TRequestStatus& aStatus)
	{
	__ASSERT_ALWAYS( iProgress.iState == TSendAsTestMtmProgress::ESendAsTestWaiting, User::Invariant() );

	switch( aCommand )
		{
	case KMTMStandardFunctionsSendMessage:
		{
		// Send request - do the 'send'...
		SendMessagesL(aSelection, aStatus);		
		} break;
	default:
		TRequestStatus* status=&aStatus;
		User::RequestComplete(status,KErrNotSupported);	
		break;
		};
	}

TBool CSendAsTestServerMtm::CommandExpected()
	{
	return EFalse;
	}

const TDesC8& CSendAsTestServerMtm::Progress()
	{
	iProgressBuf = TSendAsTestMtmProgressBuf(iProgress);
	return iProgressBuf;
	}

void CSendAsTestServerMtm::MoveToLocalL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);
	}

void CSendAsTestServerMtm::MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);	
	}

void CSendAsTestServerMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& aStatus)
	{
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status,KErrNotSupported);		
	}

void CSendAsTestServerMtm::DoRunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	switch( iProgress.iState )
		{
	case TSendAsTestMtmProgress::ESendAsTestInitSend:
		{
		DoInitialiseSendL();
		} break;
	case TSendAsTestMtmProgress::ESendAsTestSendNext:
		{
		DoSendMessageL();
		} break;
	case TSendAsTestMtmProgress::ESendAsTestSending:
		{
		// Check the error value of the message entry - this has been set to
		// indicate whether the send should have succeeded or not.
		User::LeaveIfError(iEntry.iError);
		
		// The message was sent ok - move message to the sent folder.
		DoMoveToSentFolderL();
		} break;
	case TSendAsTestMtmProgress::ESendAsTestMoving:
		{
		if( iProgress.iMessagesDone < iProgress.iMessageCount )
			{
			// There are still messages to send - move onto the next...
			iCurrentMessage = iSelection->At(iProgress.iMessagesDone);

			// Move onto the next message...
			++iProgress.iMessagesDone;
			
			iProgress.iState = TSendAsTestMtmProgress::ESendAsTestSending;
			CompleteSelf();
			}
		else
			{
			// All messages sent - complete request
			DoComplete(KErrNone);
			}
		} break;
	case TSendAsTestMtmProgress::ESendAsTestWaiting:
	default:
		User::Invariant();
		break;
		}	
	}
	
void CSendAsTestServerMtm::DoComplete(TInt aError)
	{
	iProgress.iError = aError;
	
	if( iProgress.iError != KErrNone )
		{
		TRAPD(err, HandleSendErrorL(KMsvSendStateFailed));
		}
	else
		{
		// Make sure that the server MTM is not locking any entries.
		iServerEntry->SetEntry(KMsvNullIndexEntryId);	// ignore error...	
		}
	
	// Complete the request with the KErrNone - the progress has the error code.
	User::RequestComplete(iRequestStatus, KErrNone);
	}
	
/*
 * Methods from CActive
 */

void CSendAsTestServerMtm::DoCancel()
	{
	switch( iProgress.iState )
		{
	case TSendAsTestMtmProgress::ESendAsTestSending:
		{
		iTimer.Cancel();
		iProgress.iState = TSendAsTestMtmProgress::ESendAsTestWaiting;	
		} break;
	case TSendAsTestMtmProgress::ESendAsTestWaiting:
	case TSendAsTestMtmProgress::ESendAsTestInitSend:
	case TSendAsTestMtmProgress::ESendAsTestSendNext:
	case TSendAsTestMtmProgress::ESendAsTestMoving:
	default:
		// Do nothing...
		break;
		}
	// Suspend the messages that have not been sent.
	iProgress.iError = KErrCancel;
	TRAPD(err, HandleSendErrorL(KMsvSendStateSuspended));
	
	// Complete the request with the KErrNone - the progress has the error code.
	User::RequestComplete(iRequestStatus, KErrNone);
	}
