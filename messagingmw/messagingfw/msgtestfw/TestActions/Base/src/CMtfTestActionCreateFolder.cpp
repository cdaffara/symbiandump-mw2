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
// CreateFolder
// [Action Parameters]
// Session        <input>: Reference to the session.
// ParentId       <input>: Value of the parent id.
// MtmUid         <input>: Value of the MTM uid.
// ServiceId      <input>: Value of the the service id.
// (Priority)     <input>: Value of the entry priority. Default is medium priority.
// (ReadOnlyFlag) <input>: Value of the read only flag. Default is false.
// (VisibleFlag)  <input>: Value of the visible flag. Default is true.
// (Description)  <input>: Name of the description.	Default is blank.
// (Details)      <input>: Name of the details. Default is blank.
// FolderId      <output>: Value of the created folder id.
// [Action Description]
// Creates a folder on the specified parent.
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


#include "CMtfTestActionCreateFolder.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionCreateFolder::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateFolder* self = new (ELeave) CMtfTestActionCreateFolder(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateFolder::CMtfTestActionCreateFolder(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionCreateFolder::~CMtfTestActionCreateFolder()
	{
	delete iBlank;
	}


void CMtfTestActionCreateFolder::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateFolder);
	iBlank = KNullDesC().AllocL();

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramParentId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TUid paramMtmUid = ObtainValueParameterL<TUid>(TestCase(),ActionParameters().Parameter(2));
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(3));
	TMsvPriority paramPriority = ObtainValueParameterL<TMsvPriority>(TestCase(),ActionParameters().Parameter(4),EMsvMediumPriority);
	TInt paramReadOnlyFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(5),EFalse);
	TInt paramVisibleFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(6),ETrue);
	HBufC* paramDescription = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(7),iBlank);
	HBufC* paramDetails = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(8),iBlank);

	iIndexEntry.iType = KUidMsvFolderEntry;
	iIndexEntry.iMtm = paramMtmUid;	
	iIndexEntry.iServiceId = paramServiceId;
	iIndexEntry.SetPriority(TMsvPriority(paramPriority));
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
		TMsvId paramFolderId;
		paramFolderId = iIndexEntry.Id();

		StoreParameterL<TMsvId>(TestCase(),paramFolderId,ActionParameters().Parameter(9));

		TestCase().ActionCompletedL(*this);
		}
	else
		{
		iOperation = entry->CreateL(iIndexEntry,iStatus);
		SetActive();
		}
	CleanupStack::PopAndDestroy(entry);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateFolder);
	}


void CMtfTestActionCreateFolder::DoCancel()
	{
	iOperation->Cancel();
	}


void CMtfTestActionCreateFolder::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	delete iOperation;
	User::LeaveIfError(err);

	TMsvId paramFolderId;
	paramFolderId = iIndexEntry.Id();
	StoreParameterL<TMsvId>(TestCase(),paramFolderId,ActionParameters().Parameter(9));

	TestCase().ActionCompletedL(*this);
	}




