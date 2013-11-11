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
 
#include "CMtfTestParameterStore.h"
#include "CMtfTestAction.h" 
#include "CMtfTestParameter.h"

CMtfTestParameterStore* CMtfTestParameterStore::NewL()
{
	CMtfTestParameterStore* self = new (ELeave) CMtfTestParameterStore;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

CMtfTestParameterStore::CMtfTestParameterStore()
{
}

void CMtfTestParameterStore::ConstructL()
{
}

CMtfTestParameterStore::~CMtfTestParameterStore()
{	
	iParameters.ResetAndDestroy();
}

/** Stores a parameter at the beginning of the array. The parameter is stored at the
beginning of the array so that when the object is deleted, the last stored parameter is
deleted first. The function takes ownership of aParameter at the END. */
void CMtfTestParameterStore::StoreParameterL(CMtfTestParameter* aParameter)
{
	User::LeaveIfError(iParameters.Insert(aParameter,0));
}

TInt CMtfTestParameterStore::ObtainParameterIndexL(const TDesC& aParameterName) const
{
	TInt parameterCount = iParameters.Count();
	TInt count;
	TBool parameterFound = EFalse;
	
	for(count=0; count<parameterCount; count++)
	{
		if (iParameters[count]->Name() == aParameterName)
		{ 
			parameterFound = ETrue;
			break;
		}
	}
	
	if (!parameterFound)
	{
		User::Leave(KErrNotFound);
	}
	
	return count;
}

const CMtfTestParameter& CMtfTestParameterStore::ObtainParameterL(const TDesC& aParameterName) const
{
	return *iParameters[ObtainParameterIndexL(aParameterName)];
}

void CMtfTestParameterStore::DeleteParameterL(const TDesC& aParameterName)
{
	TInt index = ObtainParameterIndexL(aParameterName);
	delete iParameters[index];
	iParameters.Remove(index);
}
