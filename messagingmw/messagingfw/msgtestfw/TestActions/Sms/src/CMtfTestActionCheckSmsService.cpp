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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CheckSmsService
// [Action Parameters]
// CMsvSession  Session		<input>: Reference to the session.
// [Action Description]
// Checks to see if an SMS service has been created.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCheckSmsService.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <smut.h>
#include <msvstd.h>
#include <miutset.h>
#include <msvapi.h>


_LIT(KDetails, "Short Message");
_LIT(KDescription, "");
const TInt KPriority = 1;
const TInt KReadOnlyFlag = EFalse;
const TInt KVisibleFlag = EFalse;


CMtfTestAction* CMtfTestActionCheckSmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckSmsService* self = new (ELeave) CMtfTestActionCheckSmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionCheckSmsService::CMtfTestActionCheckSmsService(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCheckSmsService::~CMtfTestActionCheckSmsService()
	{
	 Cancel();
	 delete iOperation;
	}

void CMtfTestActionCheckSmsService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckSmsService);
    TInt err;
    CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TRAP(err, TSmsUtilities::ServiceIdL(*paramSession,iSmsServiceId));
	if (err == KErrNone)
		{		
		CMsvEntry* service = paramSession->GetEntryL(iSmsServiceId);
		CleanupStack::PushL(service);

		if (service->Entry().Priority() != KPriority ||
			service->Entry().ReadOnly() != KReadOnlyFlag ||
			service->Entry().Visible() != KVisibleFlag ||
			(service->Entry().iDescription.CompareF(KDescription()) != 0 ) ||
			(service->Entry().iDetails.CompareF(KDetails()) != 0))
			{			
			// failed
			TestCase().ERR_PRINTF1(_L("SMS service fields don't match !"));
			TestCase().SetTestStepResult(EFail);
			}

	    CleanupStack::PopAndDestroy(service); 				
		}
	else if (err == KErrNotFound)
		{
		// failed
		TestCase().ERR_PRINTF1(_L("SMS service doeesn't exist !"));
		TestCase().SetTestStepResult(EFail);		
		}
	else	
		{  
		User::LeaveIfError(err);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckSmsService);
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionCheckSmsService::DoCancel()
	{
	iOperation->Cancel();
	}

 void CMtfTestActionCheckSmsService::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation, iStatus);
	User::LeaveIfError(err);

	TPckgBuf<TMsvLocalOperationProgress> progress;
	progress.Copy(iOperation->FinalProgress());
	
	TestCase().ActionCompletedL(*this);
	}

