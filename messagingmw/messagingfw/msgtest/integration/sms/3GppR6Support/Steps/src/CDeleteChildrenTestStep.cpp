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
  
#include "CDeleteChildrenTestStep.h"

#include <msvapi.h>
#include <msvuids.h>
#include <smut.h>

#include "CAsyncWaiter.h"
#include "CentralRepoUtils.h"
#include "MsgingUtils.h"

using namespace SharedData;

class CSessionObserver1 : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};

/**
 Constructor to set test step name
*/
CDeleteChildrenTestStep::CDeleteChildrenTestStep(MSharedData& aObj)
:CBasicTestStep(aObj)
	{
	SetTestStepName(KDeleteChildrenTestStep);
	}


CDeleteChildrenTestStep::~CDeleteChildrenTestStep()
	{
	}


TVerdict CDeleteChildrenTestStep::doTestStepL()
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

	if(parentTag.CompareF(_L("Root")) == 0) // This means We Want To Delete ServiceId 
		{
		CentralRepoUtils::DeleteServiceL(session);
		}
	else
		{
			// LookUp for the FolderId.
		TMsvId paramParentId =	MsgingUtils::GetLocalFolderId(parentTag);//KMsvGlobalOutBoxIndexEntryId;

		TMsvSelectionOrdering order(KMsvNoGrouping,EMsvSortByNone,ETrue);
		CMsvEntry* entry = CMsvEntry::NewL(*session,paramParentId,order);
		CleanupStack::PushL(entry);
		entry->SetEntryL(paramParentId);
		CMsvEntrySelection* selection  = entry->ChildrenL();
		
		CleanupStack::PushL(selection);
		
		TInt count = selection->Count();

		CAsyncWaiter* waiter = CAsyncWaiter::NewL();
		CleanupStack::PushL(waiter);
		if(count)
			{
			for(TInt i =0; i< count; i++)
				{
				CMsvOperation* opObj = entry->DeleteL(selection[i],waiter->iStatus);
				waiter->StartAndWait();
				delete opObj;
				User::LeaveIfError(waiter->Result());
				}
			}
		CleanupStack::PopAndDestroy(waiter);
		CleanupStack::PopAndDestroy(2,entry);
		}

	CleanupStack::PopAndDestroy(3, scheduler);

	return TestStepResult();
	}

