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
// RemoveDefaultImapService
// [Action Parameters]
// imapClientMtm		<input>:	CImap4ClientMtm
// [Action Description]
// RemoveDefaultImapService Test Action is intended to remove the default IMAP4
// service
// [APIs Used]	
// CImap4ClientMtm::RemoveDefaultServiceL
// __ACTION_INFO_END__
// 
//

// system #includes
#include <impcmtm.h>

// user #includes
#include "CMtfTestActionRemoveDefaultImapService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionRemoveDefaultImapService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionRemoveDefaultImapService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveDefaultImapService* self = new (ELeave) CMtfTestActionRemoveDefaultImapService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionRemoveDefaultImapService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionRemoveDefaultImapService::CMtfTestActionRemoveDefaultImapService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionRemoveDefaultImapService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionRemoveDefaultImapService::~CMtfTestActionRemoveDefaultImapService()
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
void CMtfTestActionRemoveDefaultImapService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveDefaultImapService);
	CImap4ClientMtm* imapClientMtm = 
		ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(), ActionParameters().Parameter(0));	
		
	TRAPD( error, imapClientMtm->RemoveDefaultServiceL() );
		
	if (error != KErrNone)
		{
		TestCase().INFO_PRINTF2(_L("Failed to remove default IMAP4 service error(%d)"), error );
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveDefaultImapService);
		}
	
	TestCase().ActionCompletedL(*this);	
	}


