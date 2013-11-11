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
// DeInstallMtmGroup
// [Action Parameters]
// Session     <input>: Reference to the session.
// FileName	   <input>: Data component file name
// [Action Description]
// Deinstalls a group of MTMs.
// [APIs Used]
// CMsvSession::DeInstallMtmGroup
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionDeInstallMtmGroup.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>

/**
  Function : NewL
  Description				:Creates a new CMtfTestActionDeInstallMtmGroup object
  @internalTechnology
  @param : aTestCase 		:Reference to the Test case
  @param : aActionParams 	:Test Action parameters 
  @return : CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionDeInstallMtmGroup::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDeInstallMtmGroup* self = new (ELeave) CMtfTestActionDeInstallMtmGroup(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionDeInstallMtmGroup
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post none
*/
CMtfTestActionDeInstallMtmGroup::CMtfTestActionDeInstallMtmGroup(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionDeInstallMtmGroup
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionDeInstallMtmGroup::~CMtfTestActionDeInstallMtmGroup()
	{
	}

/**
  Function : ExecuteActionL
  Description		: Deinstalls a group of MTMs
  @internalTechnology
  @param			: none
  @return			: void
  @pre		: 
  @post	: none
*/
void CMtfTestActionDeInstallMtmGroup::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDeInstallMtmGroup);

	// Obtain input parameters
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),
												 ActionParameters().Parameter(0));

	HBufC* paramFileName = ObtainParameterReferenceL<HBufC>(TestCase(),
												ActionParameters().Parameter(1));
												
	// If valid parameters received, call deinstall MTM group
	if( (paramFileName != NULL) && (paramSession != NULL) )
		{
		TInt result  = paramSession->DeInstallMtmGroup(*paramFileName); 
		
		// Print the result
		if(result == KErrNone)
			{
			TestCase().INFO_PRINTF1(_L("MTM deinstalled successfully"));
			}
		else if(result == KErrNotFound)
			{
			TestCase().INFO_PRINTF1(_L("Cannot deinstalled MTM - not currently installed"));
			}
		else
			{
			TestCase().ERR_PRINTF2(_L("MTM deinstallation Failed with error %d"), result);
			User::Leave(result);
			}
		}
	else
		{
		// Invalid parameters received, leave the Test Action
		User::Leave(KErrArgument);
		}	
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDeInstallMtmGroup);
		
	TestCase().ActionCompletedL(*this);
	}
