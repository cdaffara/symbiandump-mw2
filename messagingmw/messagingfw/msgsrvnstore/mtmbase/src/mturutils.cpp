// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <eikenv.h>
#include <msvapi.h>
#include <mtclreg.h>
#include <mtuireg.h>
#include <mtclbase.h>
#include <eikappui.h>
#include <mturutils.h>
#include "MTMUIBAS.H"
#include "MTUDPAN.H"

_LIT(KThreadName, "LaunchEditor");
const TInt KLaunchMinHeapSize=0x1000;
const TInt KLaunchMaxHeapSize=0x80000;

//**********************************
// CMturEnv
//**********************************

class CMturEnv : public CEikonEnv
	{
public:
	void DestroyEnvironment();
	void ConstructL();
	};

void CMturEnv::DestroyEnvironment()
	{
	CEikonEnv::DestroyEnvironment();
	}

void CMturEnv::ConstructL()
	{
	CEikonEnv::ConstructL();
	SetAutoForwarding(ETrue);
	}

//**********************************
// CMturAppUi
//**********************************

class CMturAppUi : public CEikAppUi
	{
public:
	~CMturAppUi();
	void ConstructL();
	};

CMturAppUi::~CMturAppUi()
	{
	}

void CMturAppUi::ConstructL()
	{
	CEikAppUi::BaseConstructL(ENoAppResourceFile);
	}

//**********************************
// CDummyObserver
//**********************************

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};

//**********************************
// Global
//**********************************

static void DoLaunchEditorL(TMsvId aId)
	{
	// Create an observer
	CDummyObserver* ob = new(ELeave)CDummyObserver;
	CleanupStack::PushL(ob);

	// Create a message server session
	CMsvSession* session = CMsvSession::OpenSyncL(*ob);
	CleanupStack::PushL(session);

	// Create a client registry
	CClientMtmRegistry* clientReg = CClientMtmRegistry::NewL(*session);
	CleanupStack::PushL(clientReg);

	// Create a client entry sitting on the message
	CMsvEntry* cEntry = CMsvEntry::NewL(*session, aId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// Create the required client Mtm
	CBaseMtm* client = clientReg->NewMtmL(cEntry->Entry().iMtm);
	CleanupStack::PushL(client);

	// Create a UI registry
	CMtmUiRegistry* uiReg = CMtmUiRegistry::NewL(*session);
	CleanupStack::PushL(uiReg);

	// Create the required UI
	CBaseMtmUi* ui = uiReg->NewMtmUiL(*client);
	CleanupStack::PushL(ui);

	// Give the client entry to the UI
	CleanupStack::Pop(); // cEntry
	client->SetCurrentEntryL(cEntry);

	// Start the waiter
	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();

	// Launch the editor
	CMsvOperation* op = NULL;
	TRAPD(error, op = ui->EditL(wait->iStatus));
	if (error)
		{
		TRequestStatus* status = &(wait->iStatus);
		User::RequestComplete(status, KErrNone);
		}

	// Compete the operation
	CActiveScheduler::Start();
	delete op;

	// Tidy up
	CleanupStack::Pop(7); // wait, ui, uiReg, client, clientReg, session, ob
	
	// Order of deletion is significant
	delete wait;
	delete ui;
	delete client;
	delete uiReg;
	delete clientReg;
	delete session;
	delete ob;
	}

static TInt LaunchEditor(TMsvId aId)
	{
	TRAPD(error, DoLaunchEditorL(aId));
	return error;
	}

static TInt DoThreadStart(TAny* aParam)
	{
	CMturEnv* env = new CMturEnv;
	__ASSERT_ALWAYS(env, Panic(EMturThreadStartPanic1));
	CMturAppUi* ui = new CMturAppUi;
	__ASSERT_ALWAYS(ui, Panic(EMturThreadStartPanic2));

	TRAPD(error, env->ConstructL(); ui->ConstructL());
	__ASSERT_ALWAYS(!error, Panic(EMturThreadStartPanic3));
	env->DisableExitChecks(ETrue);

	LaunchEditor((TMsvId)aParam);

	ui->PrepareToExit();
	env->DestroyEnvironment();
	return KErrNone;
	}	

//**********************************
// MturUtils
//**********************************

EXPORT_C void MturUtils::LaunchEditorL(TMsvId aId)
	{
	RThread thread;
	User::LeaveIfError(thread.Create(KThreadName, DoThreadStart, KDefaultStackSize, KLaunchMinHeapSize, KLaunchMaxHeapSize, (TAny*)aId, EOwnerThread));
	thread.Resume();
	thread.Close();
	}

EXPORT_C void MturUtils::LaunchEditorAndWaitL(TMsvId aId)
	{
	TRequestStatus aStatus;
	RThread thread;
	User::LeaveIfError(thread.Create(KThreadName, DoThreadStart, KDefaultStackSize, KLaunchMinHeapSize, KLaunchMaxHeapSize, (TAny*)aId, EOwnerThread));
	thread.Logon(aStatus);
	thread.Resume();
	User::WaitForRequest(aStatus);
	thread.Close();
	}

EXPORT_C void MturUtils::LaunchEditorL(TMsvId aId, RThread& aThread, TRequestStatus& aStatus)
	{
	User::LeaveIfError(aThread.Create(KThreadName, DoThreadStart, KDefaultStackSize, KLaunchMinHeapSize, KLaunchMaxHeapSize, (TAny*)aId, EOwnerThread));
	aThread.Logon(aStatus);
	aThread.Resume();
	}
