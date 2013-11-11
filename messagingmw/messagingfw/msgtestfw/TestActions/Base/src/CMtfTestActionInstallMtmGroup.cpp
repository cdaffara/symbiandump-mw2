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
// CMtfTestActionInstallMtm.cpp
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// InstallMtmGroup
// [Action Parameters]
// Session     <input>: Reference to the session.
// FileName	   <input>: Data component file name
// [Action Description]
// Installs a new group of MTMs.
// [APIs Used]
// CMsvSession::InstallMtmGroup
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionInstallMtmGroup.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>

/**
  Function : NewL
  Description				:Creates a new CMtfTestActionInstallMtmGroup object
  @internalTechnology
  @param : aTestCase 		:Reference to the Test case
  @param : aActionParams 	:Test Action parameters 
  @return : CMtfTestAction* :a base class pointer to the newly created object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionInstallMtmGroup::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionInstallMtmGroup* self = new (ELeave) CMtfTestActionInstallMtmGroup(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionInstallMtmGroup
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @pre none
  @post none
*/
CMtfTestActionInstallMtmGroup::CMtfTestActionInstallMtmGroup(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionInstallMtmGroup
  Description : Destructor
  @internalTechnology
*/
CMtfTestActionInstallMtmGroup::~CMtfTestActionInstallMtmGroup()
	{
	}

/**
  Function : ExecuteActionL
  Description		: Installs a new group of MTMs
  @internalTechnology
  @param			: none
  @return			: void
  @pre		: 
  @post	: none
*/
void CMtfTestActionInstallMtmGroup::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionInstallMtmGroup);
	// Obtain input parameters
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),
												 ActionParameters().Parameter(0));

	HBufC* paramFileName = ObtainParameterReferenceL<HBufC>(TestCase(),
												ActionParameters().Parameter(1));
												
	// If valid parameters received, call install MTM group
	if( (paramFileName != NULL) && (paramSession != NULL) )
		{
		TInt result  = paramSession->InstallMtmGroup(*paramFileName); 
		
		// Print the result
		if(result == KErrAlreadyExists)
			{
			TestCase().INFO_PRINTF1(_L("MTM already installed"));
			}
		else if(result == KErrNone)
			{
			TestCase().INFO_PRINTF1(_L("MTM installed successfully"));
			}
		else
			{
			TestCase().ERR_PRINTF2(_L("MTM installation Failed with error %d"), result);
			User::Leave(result);
			}
		}
	else
		{
		// Invaid parameters received, leave the Test Action
		User::Leave(KErrArgument);
		}	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionInstallMtmGroup);
	TestCase().ActionCompletedL(*this);
	}
