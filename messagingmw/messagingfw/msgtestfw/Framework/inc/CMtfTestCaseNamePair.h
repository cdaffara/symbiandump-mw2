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


#ifndef __CMTF_TEST_CASE_NAME_PAIR_H__
#define __CMTF_TEST_CASE_NAME_PAIR_H__

#include <e32base.h>
class CMtfTestServer;
class CMtfTestCase;

typedef CMtfTestCase* (*TestCaseNewL)(const CMtfTestServer&);

/** Represents a pair consisting of a test case name and a function pointer.
Used by the framework to maintain a mapping between test case names and functions that
can be used to create an instance of the corresponding test case. */
class CMtfTestCaseNamePair : public CBase
{
public:
	static CMtfTestCaseNamePair* NewL(const TDesC& aCaseName, 
			TestCaseNewL aFunctionPointer);
	virtual ~CMtfTestCaseNamePair();
	const TDesC& Name() const;
	
	/** Creates a new test case object. The specific derived test case class instance is
	created. */
	CMtfTestCase* CreateTestCaseL(const CMtfTestServer& aTestServer) const;
		
private:
	void ConstructL(const TDesC& aStepName, TestCaseNewL aFunctionPointer);
	
private:
	HBufC*					iName;
	TestCaseNewL			iFunctionPointer;
};



#endif
