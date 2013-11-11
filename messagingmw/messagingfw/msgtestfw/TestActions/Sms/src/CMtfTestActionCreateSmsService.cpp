// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// CreateSmsService
// [Action Parameters]
// CMsvSession  Session		<input>: Reference to the session.
// TInt		   Priority     <input-optional>: Value of the service priority.
// TInt		   ReadOnlyFlag <input-optional>: Value of the read only flag.
// TInt        VisibleFlag  <input-optional>: Value of the visible flag.
// HBufC       Description  <input-optional>: Name of the description.
// HBufC       Details      <input-optional>: Name of the details.
// TMsvId      ServiceId 	<output-completion>: Value  of an SMS service id.
// [Action Description]
// Checks for an existing SMS service and returns it.
// If the service does not exist, creates a new SMS service with optional
// parameters Priority,ReadOnlyFlag,VisibleFlag,Description and Details.
// [APIs Used]
// TMsvEntry::iType
// TMsvEntry::SetPriority	
// TMsvEntry::iMtm	
// TMsvEntry::SetReadOnly	
// TMsvEntry::SetVisible	
// TMsvEntry::iDescription
// TMsvEntry::iDetails
// TMsvEntry::iDate
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateSmsService.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <smut.h>
#include <msvstd.h>
#include <miutset.h>
#include <msvapi.h>


CMtfTestAction* CMtfTestActionCreateSmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmsService* self = new (ELeave) CMtfTestActionCreateSmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}

CMtfTestActionCreateSmsService::CMtfTestActionCreateSmsService(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCreateSmsService::~CMtfTestActionCreateSmsService()
	{
	 Cancel();
	 delete iOperation;
	}

void CMtfTestActionCreateSmsService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSmsService);
    TInt err;
    CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TRAP(err, TSmsUtilities::ServiceIdL(*paramSession,iSmsServiceId));
	if (err==KErrNotFound)
		{
		TPtrC defaultDescription(_L("SMSService"));
		HBufC* defDescriptionBuf = defaultDescription.AllocLC();
		TPtrC defaultDetails(_L("SMS"));
		HBufC* defDetailsBuf = defaultDetails.AllocLC();
		
		TInt paramPriority = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1), EMsvMediumPriority);
		TInt paramReadOnlyFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), EFalse);
		TInt paramVisibleFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3), ETrue);
		HBufC* paramDescription = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(4), defDescriptionBuf);
		HBufC* paramDetails = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(5), defDetailsBuf);
		TMsvEntry indexEntry;
		indexEntry.iType = KUidMsvServiceEntry;
		indexEntry.iMtm = KUidMsgTypeSMS;
		indexEntry.SetReadOnly(paramReadOnlyFlag);	
		indexEntry.SetVisible(paramVisibleFlag);	
		indexEntry.SetPriority(TMsvPriority(paramPriority));
		indexEntry.iDescription.Set(paramDescription->Des());
		indexEntry.iDetails.Set(paramDetails->Des());
		indexEntry.iDate.HomeTime();
		
		CMsvEntry* entry = CMsvEntry::NewL(*paramSession,KMsvRootIndexEntryId,TMsvSelectionOrdering());
		CleanupStack::PushL(entry);
		iOperation = entry->CreateL(indexEntry,iStatus);
	    CleanupStack::PopAndDestroy(entry);
	
		CActiveScheduler::Add(this);
		SetActive();
		StoreParameterL<TMsvId>(TestCase(),iSmsServiceId,ActionParameters().Parameter(6));
		CleanupStack::PopAndDestroy(2, defDescriptionBuf); // defDetailsBuf, defDescriptionBuf
		}
	else if (err == KErrNone)
		{
		StoreParameterL<TMsvId>(TestCase(),iSmsServiceId,ActionParameters().Parameter(6));
		TestCase().ActionCompletedL(*this);
		}
	else
		{  
		User::LeaveIfError(err);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSmsService);
	}

void CMtfTestActionCreateSmsService::DoCancel()
	{
	iOperation->Cancel();
	}

 void CMtfTestActionCreateSmsService::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation, iStatus);
	User::LeaveIfError(err);

	TPckgBuf<TMsvLocalOperationProgress> progress;
	progress.Copy(iOperation->FinalProgress());
	
	iSmsServiceId = progress().iId;
	StoreParameterL<TMsvId>(TestCase(),iSmsServiceId,ActionParameters().Parameter(6));

	TestCase().ActionCompletedL(*this);
	}

