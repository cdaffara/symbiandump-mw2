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
//

/**
 @file
*/

#include "CMtfTestActionManager.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestServer.h"
#include "CMtfTestAction.h"
#include "CMtfTestActionWait.h"

_LIT(KActionParam,"actionParam");
_LIT(KInvalidActionParameters,"Invalid Action Parameters");

// ISSUE: Add NewLC function.		

CMtfTestActionManager* CMtfTestActionManager::NewL(CMtfTestServer& aTestServer,
			const TDesC& aActionName)
{
	CMtfTestActionManager* self=new(ELeave) CMtfTestActionManager(aTestServer);
	CleanupStack::PushL(self);
	self->ConstructL(aActionName);
	CleanupStack::Pop();
	return self;
}

void CMtfTestActionManager::ConstructL(const TDesC& aActionName)
{
	iActionName = aActionName.AllocL();
}

CMtfTestActionManager* CMtfTestActionManager::NewL(CMtfTestServer& aTestServer)
{
	CMtfTestActionManager* self=new(ELeave) CMtfTestActionManager(aTestServer);
	return self;
}
 
CMtfTestActionManager::CMtfTestActionManager(CMtfTestServer& aTestServer)
	: iTestServer(aTestServer)
{
}
		
CMtfTestActionManager::~CMtfTestActionManager() 
{
	delete iActionName;
}

TVerdict CMtfTestActionManager::doTestStepPreambleL()  
{
	return TestStepResult();
}
	
/** Reads the .ini file and attempts to obtain parameters for the test action that 
this instance is managing. This functionality should become superfluous once TestExecute scripts
are changed to allow parameters directly in the script itself. */ 
TVerdict CMtfTestActionManager::doTestStepL()
{
	if (iActionName == NULL)
	{
		// no test action to manage
		return TestStepResult();
	}
	
	// Obtain parameters for the action and save them.

	TPtrC parameter;
	HBufC* actionId = NULL;
	CMtfTestActionParameters* actionParams = CMtfTestActionParameters::NewL();
	CleanupStack::PushL(actionParams);
	
	// if this check is false then we assume no parameters
	if (GetStringFromConfig(ConfigSection(),KActionParam,parameter))
	{
		TLex parser(parameter);
		
		parser.SkipSpace();
		
		// if this check is false then we assume no parameters
		if (!parser.Eos())
		{
			// there are parameters present
			// parameters are separated by white space
			// there is an optional action id parameter, in which case all other parameters
			// must be enclosed in [], e.g., [ p1 p2 ... pn] action_id
			
			TBool actionIdPresent = EFalse; // "first [ encountered" flag
			TBool paramBlockClosed = EFalse; // "second ] encountered" flag
					
			if (parser.Peek() == '[')
			{
				parser.Get();
				actionIdPresent = ETrue;
				parser.SkipSpace();
				
				// if we are at the end of the string then the parameters are just
				// consisting of a single [
			
				if (parser.Eos())
				{
					User::Leave(KErrGeneral);
				}
			}
			
			// the loop will be entered
			
			while(!parser.Eos())
			{
				if (actionIdPresent && (parser.Peek() == ']'))
				{
					parser.Get();
					paramBlockClosed = ETrue;
					parser.SkipSpace();
					
					if (parser.Eos())
					{
						// no action id,  [....] 
						// this is accepted
						break;
					}
					
					//actionId exists
					actionId = parser.NextToken().AllocLC();
					
					parser.SkipSpace();
					
					// check there is nothing after the action id
					__ASSERT_ALWAYS(parser.Eos(),User::Panic(KInvalidActionParameters,0));
					
					// we are at the end of the parameters, since the previous line 
					// was passed, the loop will be exited at the next iteration
				}
			
				actionParams->AddParameterL(parser.NextToken());
				parser.SkipSpace();
			}		
			
			if (actionIdPresent && !paramBlockClosed)
			{
				// [....] id was not closed with ]
				User::Leave(KErrGeneral);
			}
		}
	}

	if (actionId)
	{
		// actionId is on the cleanup stack last
		// CreateTestActionL takes ownership of actionParams at the END of the function
		iTestServer.CreateTestActionL(*iActionName,actionParams,*actionId);
		// test server has taken ownership of actionParams, we must not leave before
		// popping them off, but first we have to pop off actionId
		CleanupStack::PopAndDestroy(actionId);
	}		
	else
	{
		iTestServer.CreateTestActionL(*iActionName,actionParams);
	}
	
	CleanupStack::Pop(actionParams);
	return TestStepResult();
}
		
TVerdict CMtfTestActionManager::doTestStepPostambleL()
{
	return TestStepResult();
}
