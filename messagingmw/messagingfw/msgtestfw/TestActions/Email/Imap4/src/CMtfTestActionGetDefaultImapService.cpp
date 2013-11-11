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
// GetDefaultImapService
// [Action Parameters]
// imapClientMtm	<input>:	CImap4ClientMtm
// serviceId		<output>:	TMsvId
// [Action Description]
// GetDefaultImapService Test Action is intended to get the default IMAP4 service ID.
// [APIs Used]	
// CImap4ClientMtm::DefaultServiceL
// __ACTION_INFO_END__
// 
//


#include <impcmtm.h>

#include "CMtfTestCase.h"

#include "CMtfTestActionGetDefaultImapService.h"
#include "TMtfTestParameterType.h"
#include "CMtfTestActionParameters.h"

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionGetDefaultImapService object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionGetDefaultImapService::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetDefaultImapService* self = new (ELeave) CMtfTestActionGetDefaultImapService(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionGetDefaultImapService
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionGetDefaultImapService::CMtfTestActionGetDefaultImapService(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionGetDefaultImapService
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionGetDefaultImapService::~CMtfTestActionGetDefaultImapService()
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
void CMtfTestActionGetDefaultImapService::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetDefaultImapService);
	CImap4ClientMtm* imapClientMtm = ObtainParameterReferenceL<CImap4ClientMtm>(TestCase(), ActionParameters().Parameter(0));
	TInt shouldFail = ObtainValueParameterL<TInt>( TestCase(),ActionParameters().Parameter(1), 0 );
	TMsvId serviceId = 0;
	TRAPD( error, serviceId = imapClientMtm->DefaultServiceL() );
	
	if( shouldFail )
		{
		if ( error == KErrNone ) // pass check
			{
			TestCase().SetTestStepResult(EFail);
			}
		}
	else 
		{		
		if( error != KErrNone || serviceId < 0 ) // failure check 
			{
			TestCase().SetTestStepResult(EFail);
			}
		}
	StoreParameterL<TMsvId>(TestCase(), serviceId, ActionParameters().Parameter(2));
	TestCase().INFO_PRINTF3( _L("GetDefaultImapService Completed service id = %d  shouldfail = %d") , serviceId, shouldFail  );	
	
	TestCase().ActionCompletedL(*this);	
	}


