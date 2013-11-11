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

/**
 @file
*/
  
#include "CSendSmsTestStep.h"

#include <smut.h>
#include <msvapi.h>
#include <msvuids.h>

#include "CAsyncWaiter.h"
#include "MsgingUtils.h"
#include "MSharedData.h"

using namespace SharedData;

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

/**
 Constructor to set test step name
*/
CSendSmsTestStep::CSendSmsTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KSendSmsTestStep);
	}


CSendSmsTestStep::~CSendSmsTestStep()
	{
	}


TVerdict CSendSmsTestStep::doTestStepL()
	{

	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CActiveScheduler::Install(scheduler);
	CleanupStack::PushL(scheduler);
	
	CSessionObserver1* sessionObserver1 = new(ELeave) CSessionObserver1;
	CleanupStack::PushL(sessionObserver1);
	CMsvSession* session = CMsvSession::OpenSyncL(*sessionObserver1);
	CleanupStack::PushL(session);
	
	_LIT(KParent,"Parent");
	TPtrC parentTag;
	if ( !GetStringFromConfig(ConfigSection(),KParent,parentTag))
		{
		ERR_PRINTF1(_L("No Input for Outbox"));
		User::Leave(KErrNotReady);
		}
	// LookUp for the FolderId.
	TMsvId paramParentId =	MsgingUtils::GetLocalFolderId(parentTag);//KMsvGlobalOutBoxIndexEntryId expected

	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);

	TMsvSelectionOrdering order(KMsvNoGrouping,EMsvSortByNone,ETrue);
	CMsvEntry* entry = CMsvEntry::NewL(*session,paramParentId,order);
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramParentId);

	CMsvEntrySelection* selection = entry->ChildrenL();
	CleanupStack::PushL(selection);
	TMsvId serviceId =0;

	TSmsUtilities::ServiceIdL(*session,serviceId);
	
	SetTestStepResult(EPass);
	
	if(serviceId)
		{
		CMsvOperation* operation = entry->CopyL(*selection, serviceId,waiter->iStatus);
		waiter->StartAndWait();
		User::LeaveIfError(waiter->Result());
		TInt err =	FinalProgressStatus(*operation,waiter->iStatus);
		INFO_PRINTF2(_L("Err of Operation is %d"),err);
		delete operation;
		}
	else
		{
		ERR_PRINTF1(_L("No Service Id Set"));
		SetTestStepResult(EFail);
		}
	CleanupStack::PopAndDestroy(3,waiter); //selection, entry, waiter
	
	CleanupStack::PopAndDestroy(3,scheduler);
	
	return TestStepResult();
	}
	
TInt CSendSmsTestStep::FinalProgressStatus(CMsvOperation& aOperation,const TRequestStatus& aStatus)
	{
	TInt err;

	if (aOperation.Mtm() == KUidMsvLocalServiceMtm)
		{
		TPckgBuf<TMsvLocalOperationProgress> progress;
		progress.Copy(aOperation.FinalProgress());
		err = progress().iError;
		}
	else
		err = aStatus.Int();

	return (err);
	}

