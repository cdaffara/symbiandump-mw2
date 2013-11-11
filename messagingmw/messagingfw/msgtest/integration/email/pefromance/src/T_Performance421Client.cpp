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

#include "T_Performance421Client.h"

void TDummySessionObserver::HandleSessionEventL(TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	}

CPerformance421Client::CPerformance421Client(CPerformanceStepBase& aStepBase) 
: CActive(EPriorityStandard), iStepBase(aStepBase)
	{
	CActiveScheduler::Add(this);
	}

CPerformance421Client* CPerformance421Client::NewL(CPerformanceStepBase& aStepBase)
	{
	CPerformance421Client* self = NewLC(aStepBase);
	CleanupStack::Pop(self);
	return self;
	}

CPerformance421Client* CPerformance421Client::NewLC(CPerformanceStepBase& aStepBase)
	{
	CPerformance421Client* self = new(ELeave) CPerformance421Client(aStepBase);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
CPerformance421Client::~CPerformance421Client()
	{
	iConnection.Stop();
	iConnection.Close();
	
	Cancel();
	delete iMsvOperation;	
	delete iSelection;
	delete iClientMtm;
	delete iClientRegistry;	
	delete iSession;
	delete iSessionObserver;
	}
	
void CPerformance421Client::ConstructL()
	{
	//open the imap account that the test step has created
	CEmailAccounts* emailAccounts;
	emailAccounts = CEmailAccounts::NewLC();	
	RArray<TImapAccount> imapAccounts;
	CleanupClosePushL(imapAccounts);
	emailAccounts->GetImapAccountsL(imapAccounts);
	iImapAccount=imapAccounts[0];
	CleanupStack::PopAndDestroy(2,emailAccounts);
	
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
	
	iSocketServ.Connect();
	User::LeaveIfError(iConnection.Open(iSocketServ));	
	
	}

void CPerformance421Client::StartL()
	{
	iStepBase.INFO_PRINTF1(_L("Starting test..."));
	//if we are to collect the whole email then the next operation will be to fetch it
	iSelection = new (ELeave) CMsvEntrySelection;
	
	//select the imap service entry
	iSelection->AppendL(iImapAccount.iImapService);
	//make the service entry the current context
	iClientMtm->SwitchCurrentEntryL(iImapAccount.iImapService);
	
	//start the connection
	iState=EStart;
	iConnection.Start(iStatus);
	SetActive();
	}
	
void CPerformance421Client::DoCancel()
	{	
	__ASSERT_DEBUG(iMsvOperation!=NULL, User::Invariant());
	iMsvOperation->Cancel();
	}

void CPerformance421Client::CompleteSelf()
	{
	TRequestStatus* pStat = &iStatus;
	User::RequestComplete(pStat, KErrNone);
	SetActive();
	}

void CPerformance421Client::RunL()
	{
	
	User::LeaveIfError(iStatus.Int());
	TBuf8<4> buf;	
	TPckg<MMsvImapConnectionObserver*> param(this);
		
	switch(iState)
		{
		case EStart:
		
			delete iMsvOperation;
			iMsvOperation=NULL;
			iStepBase.StartTimer();
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMConnectAndSyncCompleteAfterConnect,*iSelection,param,iStatus);
			SetActive();
			iState=EWait;
			break;	
			
		case EWait:
			iStepBase.StopTimer();
			delete iMsvOperation;
			iMsvOperation=NULL;
			iMsvOperation = iClientMtm->InvokeAsyncFunctionL(KIMAP4MTMWaitForBackground,*iSelection,buf,iStatus);	
			SetActive();
			iState=EDisconnect;
			break;	
			
		case EDisconnect:
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
			__ASSERT_DEBUG(0, User::Panic(_L("CPerformance421Client unknown state"), KErrUnknown));
			break;			
		}
	
	}
	
TInt CPerformance421Client::RunError(TInt aError)
	{
	iStepBase.TestComplete(aError);
	return KErrNone;
	}
	
void CPerformance421Client::HandleImapConnectionEvent(TImapConnectionEvent /*aConnectionState*/)
	{
	//this method does nothing
	}



