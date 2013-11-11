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

#include "CMtfTestActionParameters.h"	
#include "CMtfTestAction.h"

CMtfTestActionParameters::~CMtfTestActionParameters()	
{
	iParameters.ResetAndDestroy();
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL()
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC();
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewLC()
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	return self;
}
	
CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	self->AddParameterL(aParam3);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2,aParam3);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	self->AddParameterL(aParam3);
	self->AddParameterL(aParam4);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2,aParam3,aParam4);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	self->AddParameterL(aParam3);
	self->AddParameterL(aParam4);
	self->AddParameterL(aParam5);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2,aParam3,
			aParam4,aParam5);
	CleanupStack::Pop(self);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5,
		const TDesC& aParam6)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	self->AddParameterL(aParam3);
	self->AddParameterL(aParam4);
	self->AddParameterL(aParam5);
	self->AddParameterL(aParam6);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5,
		const TDesC& aParam6)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2,aParam3,
			aParam4,aParam5,aParam6);
	CleanupStack::Pop(self);
	return self;
}
	
CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5,
		const TDesC& aParam6, const TDesC& aParam7)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	self->AddParameterL(aParam3);
	self->AddParameterL(aParam4);
	self->AddParameterL(aParam5);
	self->AddParameterL(aParam6);
	self->AddParameterL(aParam7);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5,
		const TDesC& aParam6, const TDesC& aParam7)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2,aParam3,
			aParam4,aParam5,aParam6,aParam7);
	CleanupStack::Pop(self);
	return self;
}
	
CMtfTestActionParameters* CMtfTestActionParameters::NewLC(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5,
		const TDesC& aParam6, const TDesC& aParam7, const TDesC& aParam8)
{
	CMtfTestActionParameters* self = new(ELeave) CMtfTestActionParameters;
	CleanupStack::PushL(self);
	self->AddParameterL(aParam1);
	self->AddParameterL(aParam2);
	self->AddParameterL(aParam3);
	self->AddParameterL(aParam4);
	self->AddParameterL(aParam5);
	self->AddParameterL(aParam6);
	self->AddParameterL(aParam7);
	self->AddParameterL(aParam8);
	return self;
}

CMtfTestActionParameters* CMtfTestActionParameters::NewL(const TDesC& aParam1,
		const TDesC& aParam2, const TDesC& aParam3, const TDesC& aParam4, const TDesC& aParam5,
		const TDesC& aParam6, const TDesC& aParam7, const TDesC& aParam8)
{
	CMtfTestActionParameters* self = CMtfTestActionParameters::NewLC(aParam1,aParam2,aParam3,
			aParam4,aParam5,aParam6,aParam7,aParam8);
	CleanupStack::Pop(self);
	return self;
}
		
void CMtfTestActionParameters::AddParameterL(const TDesC& aParameter)
{
	HBufC* newParameter = aParameter.AllocLC();
	User::LeaveIfError(iParameters.Append(newParameter));
	CleanupStack::Pop(newParameter);
}
	
TInt CMtfTestActionParameters::Count() const
{
	return iParameters.Count();
}
	
const TDesC& CMtfTestActionParameters::Parameter(TInt aParameterIndex) const
{
	__ASSERT_ALWAYS(aParameterIndex>=0,CMtfTestAction::Panic(CMtfTestAction::EMtfNegativeParameterIndex));
	__ASSERT_ALWAYS(aParameterIndex<iParameters.Count(),CMtfTestAction::Panic(CMtfTestAction::EMtfParameterIndexOutOfRange));
	return *iParameters[aParameterIndex];
}
