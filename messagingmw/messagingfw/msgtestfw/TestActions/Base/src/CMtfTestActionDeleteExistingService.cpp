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
// CMtfTestActionDeleteExistingService.h
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// DeleteExistingService
// [Action Parameters]
// Session		<input>: Reference to session.
// TUid			<input>: MTM UID
// [Action Description]
// Deletes the exisitng service(s) of the speficied MTM type
// [APIs Used]
// none
// __ACTION_INFO_END__
// 
//

//System Include
#include <msvapi.h>

//User Include
#include "CMtfTestActionDeleteExistingService.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase 		:Reference to the Test case
  @param : aActionParams 	:Test Action parameters 
  @return : CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionDeleteExistingService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeleteExistingService* self = new (ELeave) CMtfTestActionDeleteExistingService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  Function : CMtfTestActionLaunchSchSendExe
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post none
*/
CMtfTestActionDeleteExistingService::CMtfTestActionDeleteExistingService(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionLaunchSchSendExe
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionDeleteExistingService::~CMtfTestActionDeleteExistingService()
	{
	}

/**
  Function : ExecuteActionL
  Description : Deletes all Existing Parent Services.
  @internalTechnology
  @param  : none
  @return : void
  @pre  : 
  @post none
*/
void CMtfTestActionDeleteExistingService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeleteExistingService);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TUid paramMtmUid = ObtainValueParameterL<TUid>(TestCase(),ActionParameters().Parameter(1));

	CMsvEntry* entry1 = CMsvEntry::NewL(*paramSession,KMsvRootIndexEntryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry1);
	CMsvEntrySelection* sel = entry1->ChildrenWithMtmL(paramMtmUid);
	CleanupStack::PushL(sel);

	if(TestCase().TestStepResult() == EPass)
		{
		TInt count = sel->Count();

		while (count--)
			{
			TMsvId del = sel->At(count);

			entry1->SetEntryL(del);
			entry1->SetEntryL(entry1->Entry().Parent());
			TInt err = KErrNone;
			if( err == KErrNone )
				{
				TRAP(err, entry1->DeleteL(del));
				}
			}
		}
	CleanupStack::PopAndDestroy(sel);
	CleanupStack::PopAndDestroy(entry1);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeleteExistingService);
	TestCase().ActionCompletedL(*this);
	}
