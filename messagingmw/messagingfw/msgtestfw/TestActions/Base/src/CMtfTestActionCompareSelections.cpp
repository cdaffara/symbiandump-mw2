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
// CMtfTestActionCreateOrderedChildrenSelection.cpp
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CompareSelections
// [Action Parameters
// Session				<input>: Reference to the session.
// Entry selection 1	<input>: Selection of entries (MsvIds).
// Entry selection 2	<input>: Selection of entries (Msvids).
// [Action Description]
// Compares ordered entry selection 1 (MsvIds) with ordered entry selection 2 to determine if they match.
// The date/timestamp of each matching entry is logged.
// The test action passes if the selections match.
// [APIs Used]
// CMsvEntrySelection
// CMsvEntry
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionCompareSelections.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>

CMtfTestAction* CMtfTestActionCompareSelections::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareSelections* self = new (ELeave) CMtfTestActionCompareSelections(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionCompareSelections::CMtfTestActionCompareSelections(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCompareSelections::~CMtfTestActionCompareSelections()
	{
	}


void CMtfTestActionCompareSelections::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareSelections);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	CMsvEntrySelection* selection1 = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(1));
	CMsvEntrySelection* selection2 = ObtainParameterReferenceL<CMsvEntrySelection>(TestCase(),ActionParameters().Parameter(2));

	TInt count1 = selection1->Count();
	TInt count2 = selection2->Count();

	if(count1 != count2)
		{
		TestCase().INFO_PRINTF4(_L("Test Action %S failed. [count1=%d, count2=%d]"), &KTestActionCompareSelections, count1, count2);
		TestCase().SetTestStepResult(EFail);
		TestCase().ActionCompletedL(*this);
		return;
		}

	TBuf<45> dateTimeString; //Format: "dd/mm/yyyy hh:mm:ss am/pm".

	for(TInt ii=0; ii<count1; ++ii)
		{
		TMsvId id1 = selection1->At(ii);
		TMsvId id2 = selection2->At(ii);

		if(id1 != id2)
			{
			TestCase().ERR_PRINTF2(_L("Test Action %S failed. Ordered selections are different."), &KTestActionCompareSelections);
			TestCase().SetTestStepResult(EFail);
			TestCase().ActionCompletedL(*this);
			return;
			}
		else
			{
			TestCase().INFO_PRINTF2(_L("Index %d matched"), ii);
		
			CMsvEntry* entry=paramSession->GetEntryL((*selection1)[ii]);
			TMsvEntry entryDetails=entry->Entry();
			delete entry;

			//print the time of the entry to the log file		
			dateTimeString.Zero();
			_LIT(KFormat,"%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B");
			entryDetails.iDate.FormatL(dateTimeString,KFormat); 

			TestCase().INFO_PRINTF2(_L("%S"),&dateTimeString);
			}
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareSelections);
	TestCase().ActionCompletedL(*this);
	}

