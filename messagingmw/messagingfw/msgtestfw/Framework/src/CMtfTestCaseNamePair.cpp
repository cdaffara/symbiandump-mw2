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
	
#include "CMtfTestCaseNamePair.h"

CMtfTestCaseNamePair* CMtfTestCaseNamePair::NewL(const TDesC& aName, 
		TestCaseNewL aFunctionPointer)
{
	CMtfTestCaseNamePair* self =  new (ELeave) CMtfTestCaseNamePair();
	CleanupStack::PushL(self);
	self->ConstructL(aName,aFunctionPointer);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestCaseNamePair::~CMtfTestCaseNamePair()
{
	delete iName;
}

CMtfTestCase* CMtfTestCaseNamePair::CreateTestCaseL(const CMtfTestServer& aTestServer) const
{
	return (*iFunctionPointer)(aTestServer);
}
		
void CMtfTestCaseNamePair::ConstructL(const TDesC& aName,
		TestCaseNewL aFunctionPointer)
{
	iName = aName.AllocL();
	iFunctionPointer = aFunctionPointer;
}

const TDesC& CMtfTestCaseNamePair::Name() const
{
	// iName is guaranteed not to be NULL
	return *iName;
}
