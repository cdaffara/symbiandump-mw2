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

#include "CMtfEnumeratorConverter.h"

_LIT(KMtfEnumeratorSeparator,"::");

/** Takes a full enumerator value and creates a new empty object.
It discards the enumerator value and only uses the class name (optional) and
enumeration name */
CMtfEnumeratorConverter* CMtfEnumeratorConverter::NewL(const TDesC& aEnumeratorValue)
{
	return CMtfEnumeratorConverter::NewL(
		ParseEnumeratorValueL(aEnumeratorValue,EMtfEnumeratorClassName),
		ParseEnumeratorValueL(aEnumeratorValue,EMtfEnumeratorEnumerationName));	
}
	
CMtfEnumeratorConverter* CMtfEnumeratorConverter::NewL(const TDesC& aClassName, const TDesC& aEnumerationName)
{
	CMtfEnumeratorConverter* self = new (ELeave) CMtfEnumeratorConverter();
	CleanupStack::PushL(self);
	self->ConstructL(aClassName,aEnumerationName);
	CleanupStack::Pop(self);
	return self;
}

/** ConstructL allocates the class name and the enumeration name. */
void CMtfEnumeratorConverter::ConstructL(const TDesC& aClassName, const TDesC& aEnumerationName)
{
	iClassName = aClassName.AllocL();
	iEnumerationName = aEnumerationName.AllocL();
}

/** If '::' sequence occurs in the given parameter then it is assumed to be a 
constant enumerator value, as opposed to a parameter name. If KErrNotFound is returned 
the function returns EFalse. If a positive offset or any other error occur the function 
returns ETrue. If an error has occurred (and it is not KErrNotFound) then when this 
parameter is attempted to be used as a constant parameter the error will occur again 
and it will be handled. */
TBool CMtfEnumeratorConverter::IsConstantEnumerator(const TDesC& aParameter)
{
	return (aParameter.Find(KMtfEnumeratorSeparator) != KErrNotFound);
}

/** Takes a full enumerator value and returns the required part. Leaves if the required
part does not exist. 
@param aEnumeratorValue Full enumerator value including the enumerator constant
@param aPart Required part
@return Descriptor containing the required part */
TPtrC CMtfEnumeratorConverter::ParseEnumeratorValueL(const TDesC& aEnumeratorValue,
		TMtfEnumeratorPart aPart)
{
	if (!CMtfEnumeratorConverter::IsConstantEnumerator(aEnumeratorValue))
	{
		// there is no '::' in the string so assume it is a value
		if (aPart != EMtfEnumeratorValueName)
		{
			User::Leave(KErrNotFound);
		}
		
		return aEnumeratorValue;
	}
	
	TPtrC className;
	TPtrC enumerationName;
	TPtrC valueName;
	
	// '::' is a substring of aEnumeratorValue
	TInt offset;
	
	// the string '::' must exist since it is a constant 
	User::LeaveIfError(offset = aEnumeratorValue.Find(KMtfEnumeratorSeparator));
		
	// take the portion of the string from the start to the sequence '::'
	// this is either class name or enumeration name
	TPtrC firstName = aEnumeratorValue.Mid(0,offset);
	
	// move over '::' and take the second part of the string
	TPtrC remainder = aEnumeratorValue.Mid(offset+KMtfEnumeratorSeparator().Length());
	
	// attempt to find a second '::' substring
	TInt secondOffset = remainder.Find(KMtfEnumeratorSeparator);
		
	if (secondOffset == KErrNotFound)
	{	
		// there is no class name, use an empty string
		className.Set(KNullDesC());
		enumerationName.Set(firstName);
		valueName.Set(remainder);
	}
	else if (secondOffset <= 0)
	{
		// some other error has occurred, if 0 then the string ends with '::' which
		// is invalid
		User::Leave(KErrNotFound);
	}
	else
	{
		// class name exists
		className.Set(firstName);
		enumerationName.Set(remainder.Mid(0,secondOffset));
		valueName.Set(remainder.Mid(secondOffset+KMtfEnumeratorSeparator().Length()));
	}
		
	switch(aPart)
	{
		case EMtfEnumeratorClassName:
			return className;
		case EMtfEnumeratorEnumerationName:
			return enumerationName;
		case EMtfEnumeratorValueName:
			return valueName;
		default:
			User::Leave(KErrGeneral);
	}
	
	// dead code but required to prevent a compiler warning
	return valueName;
}
	
CMtfEnumeratorConverter::~CMtfEnumeratorConverter()
{
	delete iClassName;
	delete iEnumerationName;
	iEnumeratorValues.ResetAndDestroy();
	iValues.Reset();
}
	
/** Compares the class name and the enumeration name. Doesn't compare actual
enumerator values. */
TBool CMtfEnumeratorConverter::operator==(const CMtfEnumeratorConverter& aConverter) const
{
	return ((*iClassName == *aConverter.iClassName) && 
			(*iEnumerationName == *aConverter.iEnumerationName));
}

/** Compares the class name and the enumeration name. Doesn't compare actual
enumerator values. */
TBool CMtfEnumeratorConverter::operator!=(const CMtfEnumeratorConverter& aConverter) const
{
	return !(*this==aConverter);
}

/** If the enumerator value already exists then leave, 
otherwise add the new constant/value pair. */
void CMtfEnumeratorConverter::AddEnumeratorValueL(const TDesC& aEnumeratorValue, TInt aValue)
{
	TInt err=0;
	TRAP(err,ConvertL(aEnumeratorValue));
	
	if (err == KErrNone)
	{
		User::Leave(KErrAlreadyExists);
	}
	else if (err != KErrNotFound)
	{
		User::Leave(err);
	}

	User::LeaveIfError(iValues.Append(aValue));
	
	// If AllocL leaves then the object is left in an inconsistent state (there is a 
	// value but no constant associated with that value. That will not cause any problems since
	// if we leave at this point the whole test case will fail.
	
	HBufC* enumeratorValue = aEnumeratorValue.AllocLC();
	User::LeaveIfError(iEnumeratorValues.Append(enumeratorValue));
	CleanupStack::Pop(enumeratorValue);
}
	
/** 
@param aEnumeratorValue Enumerator value to convert. It can be full name (including class name,
enumeration name and constant, or just the constant name. If the value contains '::' then 
it is parsed, otherwise it is assumed to be a constant of this enumeration.
@return Numerical value associated with the constant */
TInt CMtfEnumeratorConverter::ConvertL(const TDesC& aEnumeratorValue) const
{
	TPtrC value = aEnumeratorValue;
	
	// does it contain '::' ?
	if (CMtfEnumeratorConverter::IsConstantEnumerator(aEnumeratorValue))
	{
		// extract the name of the enumerator value
		CMtfEnumeratorConverter* converter = CMtfEnumeratorConverter::NewL(aEnumeratorValue);
		CleanupStack::PushL(converter);
		if (*converter != *this)
		{
			// wrong converter is being used
			User::Leave(KErrNotFound);
		}
		
		value.Set(CMtfEnumeratorConverter::ParseEnumeratorValueL(aEnumeratorValue,EMtfEnumeratorValueName));
		CleanupStack::PopAndDestroy(converter);
	}
	
	TInt i;
	TBool found = EFalse;
	
	TInt count = iEnumeratorValues.Count(); 
	
	for (i=0; i<count; i++)
	{
		if (*iEnumeratorValues[i] == value)
		{
			found = ETrue;
			break;
		}
	}
	
	if (!found)
	{
		User::Leave(KErrNotFound);
	}
	
	return iValues[i];
}
	

CMtfEnumeratorConverter::CMtfEnumeratorConverter()
{
}
			
