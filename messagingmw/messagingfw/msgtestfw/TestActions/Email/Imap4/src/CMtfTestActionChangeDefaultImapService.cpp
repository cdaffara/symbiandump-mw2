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
// ChangeDefaultImapService
// [Action Parameters]
// serviceId		<input>:	TMsvId. Service ID to set as default.
// imapClientMtm	<input>:	CImap4ClientMtm	
// [Action Description]
// ChangeDefaultImapService Test Action is intended to change the default IMAP4
// service to serviceId.
// [APIs Used]	
// CImap4ClientMtm::ChangeDefaultServiceL
// __ACTION_INFO_END__
// 
//

#include <impcmtm.h>

#include "CMtfTestActionChangeDefaultImapService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionChangeDefaultImapService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionChangeDefaultImapService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionChangeDefaultImapService* self = new (ELeave) CMtfTestActionChangeDefaultImapService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionChangeDefaultImapService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionChangeDefaultImapService::CMtfTestActionChangeDefaultImapService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionChangeDefaultImapService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionChangeDefaultImapService::~CMtfTestActionChangeDefaultImapService()
	{
	}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionChangeDefaultImapService::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionChangeDefaultImapService);
	TMsvId serviceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(0));
	CImap4ClientMtm* imapClientMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(), ActionParameters().Parameter(1));
	
	TRAPD( error, imapClientMtm->ChangeDefaultServiceL(serviceId) );
		
	if (error != KErrNone)
		{
		// Failed KErrNotFound=(-1)		
		TestCase().ERR_PRINTF2(_L("Failed to change IMAP4 default service error(%d)"), error);
		TestCase().SetTestStepResult(EFail);
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionChangeDefaultImapService);
	TestCase().ActionCompletedL(*this);			

}


