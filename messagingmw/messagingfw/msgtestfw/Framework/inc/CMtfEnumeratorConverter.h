/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/


#ifndef __CMTF_ENUMERATOR_CONVERTER_H__
#define __CMTF_ENUMERATOR_CONVERTER_H__


#include <e32base.h>

/** CMtfEnumeratorConverter provides the functionality to convert a descriptor containing 
an enumerator value to the correct numerical value. This class contains the description of 
an enumeration. */
class CMtfEnumeratorConverter: public CBase
{
public:

	/** TMtfEnumeratorPart defines the different component parts of an enumerator value
	that is contained in a descriptor. */
	enum TMtfEnumeratorPart
	{
		EMtfEnumeratorClassName,
		EMtfEnumeratorEnumerationName,
		EMtfEnumeratorValueName
	};
	
	/** Creates a new empty object. aEnumeratorValue must be full enumerator value, e.g.,
	CMtfClass::TMtfEnumeratorXYZ::EMtfValueXYZ. */
	static CMtfEnumeratorConverter* NewL(const TDesC& aEnumeratorValue);
	
	/** Creates a new empty object from the class name and enumeration name. Class name is
	optional and may be empty. */
	static CMtfEnumeratorConverter* NewL(const TDesC& aClassName, const TDesC& aEnumerationName);	
	
	/** Returns true if the given parameter is a constant enumerator value of any enumeration. */
	static TBool IsConstantEnumerator(const TDesC& aParameter);
	
	/** Extracts the required part from the given enumerator value. */
	static TPtrC ParseEnumeratorValueL(const TDesC& aEnumeratorValue, TMtfEnumeratorPart aPart);
	
	virtual ~CMtfEnumeratorConverter();
	
	/* Compares two enumerator converters. Only the names are compared. */
	TBool operator==(const CMtfEnumeratorConverter& aConverter) const;
	
	/* Compares two enumerator converters. Only the names are compared. */
	TBool operator!=(const CMtfEnumeratorConverter& aConverter) const;

	
	/** Adds a new enumerator constant and its associated value. */
	void AddEnumeratorValueL(const TDesC& aEnumeratorValue, TInt aValue);
	
	/** Convert a constant enumerator value to the associated numerical value. The
	enumerator value can be full name (i.e., including class name, enumeration name and constant, 
	or just the constant name. */
	TInt ConvertL(const TDesC& aEnumeratorValue) const;
	
private:
	CMtfEnumeratorConverter();
	void ConstructL(const TDesC& aClassName, const TDesC& aEnumerationName);
			
private:
	HBufC*					iClassName;
	HBufC*					iEnumerationName;
	RPointerArray<HBufC>	iEnumeratorValues;
	RArray<TInt>			iValues;
};

#endif
