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

#ifndef __CMTF_TEST_PARAMETER_H__
#define __CMTF_TEST_PARAMETER_H__

#include <e32base.h>
#include "TMtfTestParameterType.h"

/** Represents a parameter stored by the test case. Test case manipulates a set of 
parameters which can be used by test actions of the test case. Each parameter is stored
as a pointer to TAny. Type information is also sored in the form of a generic type id and
a specific type id. Parameter is identified by its name. */
class CMtfTestParameter: public CBase
{
public:

	/** Creates a new parameter instance and takes ownership of the TAny pointer. Ownership
	is taken at the END of the function. aParameter must be on the cleanup stack. */
	static CMtfTestParameter* NewL(const TDesC& aName, TMtfTestParameterGeneralType aGeneralTypeId,
			TMtfTestParameterSpecificType aSpecificTypeId, TAny* aParameter);
	
	TMtfTestParameterGeneralType GeneralTypeId() const;
	TMtfTestParameterSpecificType SpecificTypeId() const;
	TAny* Parameter() const;
	const TDesC& Name() const;
	virtual ~CMtfTestParameter();
	
private:
	CMtfTestParameter(TMtfTestParameterGeneralType aGeneralTypeId,
			 TMtfTestParameterSpecificType aSpecificTypeId);
	void ConstructL(const TDesC& aName);
	
private:	
	TAny* iParameter;
	TMtfTestParameterGeneralType iGeneralTypeId;
	TMtfTestParameterSpecificType iSpecificTypeId;
	HBufC* iName;
};

#endif
