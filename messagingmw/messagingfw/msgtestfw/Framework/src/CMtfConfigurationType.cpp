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
	
#include "CMtfConfigurationType.h"

TInt CMtfConfigurationType::FindConfigurationL(const TMtfConfigurationType& aType, const RPointerArray<CMtfConfigurationType>& aConfigurations)
{
	TInt count = aConfigurations.Count();
	TBool found = EFalse;
	TInt i;
	
	for (i=0; i<count; i++)
	{
		if (aConfigurations[i]->ConfigurationType() == aType)
		{
			found = ETrue;
			break;
		}
	}
	
	if (found==EFalse)
	{
		User::Leave(KErrNotFound);
	}
	
	return i;
}

/** This function takes ownership of aConfiguration immediately
and therefore SHOULD NOT be called with aConfiguration already on the Cleanup stack.
@param aConfiguration Configuration type to be added to the set of configurations
@param aConfigurations The set of configurations to which the new type should be added
@leave KErrAlreadyExists If the given type already exists in the set of configurations, the function leaves */
void CMtfConfigurationType::SetConfigurationTypeL(CMtfConfigurationType* aConfiguration,
		RPointerArray<CMtfConfigurationType>& aConfigurations)
{
	CleanupStack::PushL(aConfiguration);
	
	TInt error=0;
	TRAP(error, CMtfConfigurationType::FindConfigurationL(aConfiguration->ConfigurationType(),aConfigurations));
	
	if (error==KErrNotFound)
	{
		// this type does not already exist, so add it in
		User::LeaveIfError(aConfigurations.Append(aConfiguration)); 
		
		// the function has taken ownership, so it can pop the object
		CleanupStack::Pop(aConfiguration);
	}
	else if (error==KErrNone)
	{
		User::Leave(KErrAlreadyExists);
	}
	else
	{
		User::Leave(error);
	}
}

CMtfConfigurationType* CMtfConfigurationType::NewL(const TMtfConfigurationType& aConfigurationType)
{
	CMtfConfigurationType* self =  new (ELeave) CMtfConfigurationType(aConfigurationType);
	return self;	
}

CMtfConfigurationType::CMtfConfigurationType(const TMtfConfigurationType& aConfigurationType)
: iConfigurationType(aConfigurationType)
{
}
	
CMtfConfigurationType::~CMtfConfigurationType()
{
	iFilenames.ResetAndDestroy();
}

const CMtfConfigurationType::TMtfConfigurationType& CMtfConfigurationType::ConfigurationType() const
{
	return iConfigurationType;
}

void CMtfConfigurationType::AddConfigurationFilenameL(const TDesC& aFilename)
{
	// ISSUE: the parameter should be of type TFileName to prevent aFilename
	// being longer than the maximum length of a filename.
	HBufC* fileName = aFilename.AllocLC();
	User::LeaveIfError(iFilenames.Append(fileName));
	CleanupStack::Pop(fileName);
}

const TDesC& CMtfConfigurationType::ConfigurationFileL(TInt aIndex) const
{
	if ((aIndex >= iFilenames.Count()) || (aIndex<0))
	{
		User::Leave(KErrArgument);
	}
	
	return *iFilenames[aIndex];
}

TInt CMtfConfigurationType::Count() const
{
	return iFilenames.Count();
}

