// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "T_Performance427Client.h"

void TDummySessionObserver::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	}

CPerformance427Client::CPerformance427Client(CPerformanceStepBase& aStepBase) 
: CActive(EPriorityStandard), iStepBase(aStepBase) 
	{
	CActiveScheduler::Add(this);
	}

CPerformance427Client* CPerformance427Client::NewL(CPerformanceStepBase& aStepBase)
	{
	CPerformance427Client* self = NewLC(aStepBase);
	CleanupStack::Pop(self);
	return self;
	}

CPerformance427Client* CPerformance427Client::NewLC(CPerformanceStepBase& aStepBase)
	{
	CPerformance427Client* self = new(ELeave) CPerformance427Client(aStepBase);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
CPerformance427Client::~CPerformance427Client()
	{
	Cancel();
	delete iEntrySelectionToSend;
	delete iOperation;
	delete iInboxEntry;
	delete iChildren;
	delete iMsvOperation;	
	delete iSelection;
	delete iClientMtm;
	delete iClientRegistry;	
	delete iSession;
	delete iSessionObserver;
	}
	
void CPerformance427Client::ConstructL()
	{
	//open the imap account that the test step has created
	CEmailAccounts* emailAccounts;
	emailAccounts = CEmailAccounts::NewLC();	
	RArray<TImapAccount> imapAccounts;
	CleanupClosePushL(imapAccounts);
	emailAccounts->GetImapAccountsL(imapAccounts);
	iImapAccount=imapAccounts[0];
	
	//open the smtp account too
	RArray<TSmtpAccount> smtpAccounts;
	CleanupClosePushL(smtpAccounts);
	emailAccounts->GetSmtpAccountsL(smtpAccounts);
	iSmtpAccount=smtpAccounts[0];

	CleanupStack::PopAndDestroy(3,emailAccounts);
	
	// Session observer. Needed to create a session
	iSessionObserver = new (ELeave) TDummySessionObserver;
	// Session. Needed to create a client registry.
	iSession=CMsvSession::OpenSyncL(*iSessionObserver);
	// Client registry. Needed to get the MTM component
	iClientRegistry=CClientMtmRegistry::NewL(*iSession,KMsvDefaultTimeoutMicroSeconds32);
	//get the client mtm
	iClientMtm=iClientRegistry->NewMtmL(KUidMsgTypeIMAP4);	
	
	// Load the serial comms device drivers.  If this is not done,
	// connecting via NT-RAS returns KErrNotFound (-1).
	TInt driverErr;
	driverErr=User::LoadPhysicalDevice(PDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		{
		User::Leave(driverErr);	
		}
	driverErr=User::LoadLogicalDevice(LDD_NAME);
	if (driverErr!=KErrNone && driverErr!=KErrAlreadyExists)
		{
		User::Leave(driverErr);	
		}
		
	//initialise package object
	TImImap4GetMailInfo imap4GetMailInfo;
	imap4GetMailInfo.iMaxEmailSize = KMaxTInt;
	imap4GetMailInfo.iDestinationFolder = KMsvGlobalInBoxIndexEntryIdValue;
	imap4GetMailInfo.iGetMailBodyParts = EGetImap4EmailBodyTextAndAttachments;
	iPackage=imap4GetMailInfo;
	}
	

void CPerformance427Client::StartL()
	{
	iStepBase.INFO_PRINTF1(_L("Starting test..."));
	iState=EConnectAndSync;
	CompleteSelf();
	}
	
void CPerformance427Client::DoCancel()
	{	
	__ASSERT_DEBUG(iMsvOperation!=NULL, User::Invariant());
	iMsvOperation->Cancel();
	}

void CPerformance427Client::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CPerformance427Client::RunL()
	{
	
	User::LeaveIfError(iStatus.Int());
	
	TMsvSelectionOrdering ordering;
	TPckg<MMsvImapConnectionObserver*> param(this);
	TBuf8<4> buf;
	
	switch(iState)
		{
		case EConnectAndSync:
			//if we are to collect the whole email then the next operation will be to fetch it
			iSelection = new (ELeave) CMsvEntrySelection;
			//select the imap service entry
			iSelection->AppendL(iImapAccount.iImapService);
			//make the service entry the current context
			iClientMtm->SwitchCurrentEntryL(iImapAccount.iImapService);
			//sync the account
			iState=EWait;
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSyncCompleteAfterConnect,*iSelection,param,iStatus);			
			SetActive();
			break;
			
		case EWait:
			delete iMsvOperation;
			iMsvOperation=NULL;
			iStepBase.StartTimer();
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMWaitForBackground,*iSelection,buf,iStatus);	
			SetActive();
			iState=EDisconnect;
			break;
		
		case EDisconnect:
			iStepBase.StopTimer();	
			delete iMsvOperation;
			iMsvOperation=NULL;
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMDisconnect,*iSelection,buf,iStatus);	
			SetActive();
			iState=EComplete;
			break;
		
		case EComplete:
			iStepBase.INFO_PRINTF1(_L("Test complete."));
			//inform the owning object that the process is complete
			iStepBase.TestComplete(KErrNone);
			break;
			
		default:
			__ASSERT_DEBUG(0, User::Panic(_L("CPerformance427Client unknown state"), KErrUnknown));
			break;			
		}
	}
	
TInt CPerformance427Client::RunError(TInt aError)
	{
	iStepBase.TestComplete(aError);
	return KErrNone;
	}
	
void CPerformance427Client::HandleImapConnectionEvent(TImapConnectionEvent /*aConnectionState*/)
	{
	//this method does nothing
	}



