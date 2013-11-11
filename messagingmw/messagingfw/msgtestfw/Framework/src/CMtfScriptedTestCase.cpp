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

#include "CMtfScriptedTestCase.h"

void CMtfScriptedTestCase::SetupTestCaseL()
{
}

void CMtfScriptedTestCase::ExecuteTestCaseL()
{
}

void CMtfScriptedTestCase::CheckPostConditionsL()
{
}

CMtfScriptedTestCase* CMtfScriptedTestCase::NewL(const CMtfTestServer& aTestServer, const TBool& aSynchronous)
{
	CMtfScriptedTestCase* self = new (ELeave) CMtfScriptedTestCase(aTestServer, aSynchronous);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

CMtfScriptedTestCase::CMtfScriptedTestCase(const CMtfTestServer& aTestServer, const TBool& aSynchronous)
: 	CMtfTestCase(aTestServer, ETrue,aSynchronous)
{
}

CMtfScriptedTestCase::~CMtfScriptedTestCase()
{
}


