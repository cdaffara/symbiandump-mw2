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

#include "CMtfTestBeginManager.h"
#include "CMtfTestServer.h"
#include "CMtfScriptedTestCase.h"
	 
CMtfTestBeginManager* CMtfTestBeginManager::NewL(CMtfTestServer& aTestServer,
		const TBool& aSynchronous)
{
	CMtfTestBeginManager* self=new(ELeave) CMtfTestBeginManager(aTestServer,aSynchronous);
	return self;
}
 
CMtfTestBeginManager::CMtfTestBeginManager(CMtfTestServer& aTestServer,
		const TBool& aSynchronous)
: CTestStep(), iTestServer(aTestServer), iSynchronous(aSynchronous)
{
}
		
CMtfTestBeginManager::~CMtfTestBeginManager()
{
}

TVerdict CMtfTestBeginManager::doTestStepPreambleL()  
{
	return TestStepResult();
}
	
/** Creates a new scripted test case and hands it over to the test server. */
TVerdict CMtfTestBeginManager::doTestStepL()
{
	iTestServer.SetCurrentTestCase(CMtfScriptedTestCase::NewL(iTestServer,iSynchronous));
	return TestStepResult();
}
		
TVerdict CMtfTestBeginManager::doTestStepPostambleL()
{
	return TestStepResult();
}
