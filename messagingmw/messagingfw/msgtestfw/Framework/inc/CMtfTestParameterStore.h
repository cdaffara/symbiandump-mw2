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

#ifndef __CMTF_TEST_PARAMETER_STORE_H__
#define __CMTF_TEST_PARAMETER_STORE_H__

#include <e32base.h>

class CMtfTestAction;
class CMtfTestParameter;

/** This class is used by a test case to store all test case parameters . */
class CMtfTestParameterStore: public CBase
{
public:
	static CMtfTestParameterStore* NewL();
	virtual ~CMtfTestParameterStore();
	
	/** Stores a parameter. The function takes ownership at the END. */
	void StoreParameterL(CMtfTestParameter* aParameter);
	
	const CMtfTestParameter& ObtainParameterL(const TDesC& aName) const;
	void DeleteParameterL(const TDesC& aName);
	TInt ParameterCount() const;
	
private:
	CMtfTestParameterStore();
	void ConstructL();
	TInt ObtainParameterIndexL(const TDesC& aParameterName) const;
    
private:
	RPointerArray<CMtfTestParameter> iParameters;
};

#endif
