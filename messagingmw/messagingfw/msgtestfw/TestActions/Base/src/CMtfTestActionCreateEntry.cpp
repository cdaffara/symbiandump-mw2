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
// CreateEntry
// [Action Parameters]
// Session        <input>: Reference to the session.
// TypeUid	 	  <input>: Value of the entry type uid;
// ParentId       <input>: Value of the parent id.
// MtmUid         <input>: Value of the MTM uid.
// ServiceId      <input>: Value of the the service id.
// (Priority)     <input>: Value of the entry priority. Default is medium priority.
// (ReadOnlyFlag) <input>: Value of the read only flag. Default is false.
// (VisibleFlag)  <input>: Value of the visible flag. Default is true.
// (Description)  <input>: Name of the description.	Default is blank.
// (Details)      <input>: Name of the details. Default is blank.
// EntryId       <output>: Value of the created entry id.
// [Action Description]
// Creates an entry on the specified parent.
// [APIs Used]
// TMsvEntry::iType
// TMsvEntry::SetPriority	
// TMsvEntry::iMtm	
// TMsvEntry::iServiceId
// TMsvEntry::SetReadOnly	
// TMsvEntry::SetVisible	
// TMsvEntry::iDescription
// TMsvEntry::iDetails
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// CMsvEntry::OwningService
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateEntry.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionCreateEntry::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateEntry* self = new (ELeave) CMtfTestActionCreateEntry(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateEntry::CMtfTestActionCreateEntry(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCreateEntry::~CMtfTestActionCreateEntry()
	{
	delete iBlank;
	}


void CMtfTestActionCreateEntry::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateEntry);
	CActiveScheduler::Add(this);
	
	iBlank = KNullDesC().AllocL();

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TUid paramEntryTypeUid = ObtainValueParameterL<TUid>(TestCase(),ActionParameters().Parameter(1));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));
	TUid paramMtmUid = ObtainValueParameterL<TUid>(TestCase(),ActionParameters().Parameter(3), KUidMsvLocalServiceMtm);
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(4), KMsvLocalServiceIndexEntryId);
	TMsvPriority paramPriority = ObtainValueParameterL<TMsvPriority>(TestCase(),ActionParameters().Parameter(5),EMsvMediumPriority);
	TInt paramReadOnlyFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(6),EFalse);
	TInt paramVisibleFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(7),ETrue);
	HBufC* paramDescription = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(8),iBlank);
	HBufC* paramDetails = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(9),iBlank);

	iIndexEntry.iType = paramEntryTypeUid;
	iIndexEntry.iMtm = paramMtmUid;	
	iIndexEntry.iServiceId = paramServiceId;
	iIndexEntry.SetPriority(paramPriority);	
	iIndexEntry.SetReadOnly(paramReadOnlyFlag);	
	iIndexEntry.SetVisible(paramVisibleFlag);	
	iIndexEntry.iDescription.Set(paramDescription->Des());
	iIndexEntry.iDetails.Set(paramDetails->Des());
	iIndexEntry.iDate.HomeTime();

	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,paramParentId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(paramParentId);
	if (entry->OwningService() == KMsvLocalServiceIndexEntryId)
		{
		entry->CreateL(iIndexEntry);
		CleanupStack::PopAndDestroy(entry);
		TMsvId paramEntryId;
		paramEntryId = iIndexEntry.Id();

		StoreParameterL<TMsvId>(TestCase(),paramEntryId,ActionParameters().Parameter(10));

		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iOperation = entry->CreateL(iIndexEntry,iStatus);
		SetActive();
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateEntry);
	}


void CMtfTestActionCreateEntry::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionCreateEntry::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	delete iOperation;
	User::LeaveIfError(err);

	TMsvId paramEntryId;
	paramEntryId = iIndexEntry.Id();
	StoreParameterL<TMsvId>(TestCase(),paramEntryId,ActionParameters().Parameter(10));

	TestCase().ActionCompletedL(*this);
	}


