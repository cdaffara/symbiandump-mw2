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
// CreateMmsService
// [Action Parameters]
// Session        <input>: Reference to the session.
// (Priority)     <input>: Value of the entry priority. Default is medium priority.
// (ReadOnlyFlag) <input>: Value of the read only flag. Default is false.
// (VisibleFlag)  <input>: Value of the visible flag. Default is true.
// (Description)  <input>: Name of the description.	Default is blank.
// (Details)      <input>: Name of the details. Default is blank.
// ServiceId     <output>: Value of the created service id.
// [Action Description]
// Creates a SMTP service.
// [APIs Used]
// TMsvEntry::iType
// TMsvEntry::SetPriority	
// TMsvEntry::iMtm	
// TMsvEntry::SetReadOnly	
// TMsvEntry::SetVisible	
// TMsvEntry::iDescription
// TMsvEntry::iDetails
// TMsvEntry::iDate
// TMsvEntry::Id
// CMsvEntry::SetEntryL
// CMsvEntry::CreateL
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCreateMmsService.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>
#include <miutset.h>
#include <mmsutils.h>


CMtfTestAction* CMtfTestActionCreateMmsService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateMmsService* self = new (ELeave) CMtfTestActionCreateMmsService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateMmsService::CMtfTestActionCreateMmsService(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCreateMmsService::~CMtfTestActionCreateMmsService()
	{
	delete iBlank;
	}


void CMtfTestActionCreateMmsService::ExecuteActionL()
	{
	iBlank = KNullDesC().AllocL();

	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvPriority paramPriority = ObtainValueParameterL<TMsvPriority>(TestCase(),ActionParameters().Parameter(1),EMsvMediumPriority);
	TInt paramReadOnlyFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2),EFalse);
	TInt paramVisibleFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3),ETrue);
	HBufC* paramDescription = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(4),iBlank);
	HBufC* paramDetails = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(5),iBlank);

	TMsvEntry indexEntry;
	indexEntry.iType = KUidMsvServiceEntry;
	indexEntry.iMtm = KUidMsgTypeMMS;
	indexEntry.SetPriority(paramPriority);
	indexEntry.SetReadOnly(paramReadOnlyFlag);	
	indexEntry.SetVisible(paramVisibleFlag);	
	indexEntry.iDescription.Set(paramDescription->Des());
	indexEntry.iDetails.Set(paramDetails->Des());
	indexEntry.iDate.HomeTime();
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession,KMsvRootIndexEntryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(KMsvRootIndexEntryId);
	entry->CreateL(indexEntry);
	CleanupStack::PopAndDestroy(entry);
	TMsvId paramServiceId;
	paramServiceId = indexEntry.Id();

	StoreParameterL<TMsvId>(TestCase(),paramServiceId,ActionParameters().Parameter(6));

	TestCase().ActionCompletedL(*this);
	}

