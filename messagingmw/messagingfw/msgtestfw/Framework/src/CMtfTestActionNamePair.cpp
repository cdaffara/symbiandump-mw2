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
	
#include "CMtfTestActionNamePair.h"

CMtfTestActionNamePair* CMtfTestActionNamePair::NewL(const TDesC& aName, 
		ActionNewL aFunctionPointer)
{
	CMtfTestActionNamePair* self =  new (ELeave) CMtfTestActionNamePair();
	CleanupStack::PushL(self);
	self->ConstructL(aName,aFunctionPointer);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionNamePair::~CMtfTestActionNamePair()
{
	delete iName;
}

/** Creates a new test action object. The specific derived test action class instance is
created. Ownership of aActionParameters is taken at the end. */
CMtfTestAction* CMtfTestActionNamePair::CreateTestActionL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters) const
{
	return (*iFunctionPointer)(aTestCase,aActionParameters);
}
		
void CMtfTestActionNamePair::ConstructL(const TDesC& aName,
		ActionNewL aFunctionPointer)
{
	iName = aName.AllocL();
	iFunctionPointer = aFunctionPointer;
}

const TDesC& CMtfTestActionNamePair::Name() const
{
	// iName is guaranteed not to be NULL
	return *iName;
}
