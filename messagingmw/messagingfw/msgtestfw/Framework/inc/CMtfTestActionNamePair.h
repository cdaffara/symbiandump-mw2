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


#ifndef __CMTF_TEST_ACTION_NAME_PAIR_H__
#define __CMTF_TEST_ACTION_NAME_PAIR_H__

#include <e32base.h>
class CMtfTestCase;
class CMtfTestAction;
class CMtfTestActionParameters;

typedef CMtfTestAction* (*ActionNewL)(CMtfTestCase&,CMtfTestActionParameters*);

/** Represents a pair consisting of a test action name and a function pointer.
Used by the framework to maintain a mapping between action names and functions that
can be used to create an instance of the corresponding action. */
class CMtfTestActionNamePair : public CBase
{
public:
	static CMtfTestActionNamePair* NewL(const TDesC& aActionName, 
			ActionNewL aFunctionPointer);
	virtual ~CMtfTestActionNamePair();
	const TDesC& Name() const;
	
	/** Creates a new test action object. The specific derived test action class instance is
	created. Ownership of aActionParameters is taken at the end. */
	CMtfTestAction* CreateTestActionL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters) const;
		
private:
	void ConstructL(const TDesC& aStepName, ActionNewL aFunctionPointer);
	
private:
	HBufC*					iName;
	ActionNewL				iFunctionPointer;
};


#endif
