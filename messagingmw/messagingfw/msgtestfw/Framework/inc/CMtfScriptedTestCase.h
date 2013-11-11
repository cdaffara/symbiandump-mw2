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

#ifndef __CMTF_SCRIPTED_TEST_CASE_H__
#define __CMTF_SCRIPTED_TEST_CASE_H__

#include "CMtfTestCase.h"

/** Base class for a scripted test case. The functions SetupTestCaseL, ExecuteTestCaseL and
CheckPostConditionsL are defined so that the writer of the test case does not need to define
dummy functions. */
class CMtfScriptedTestCase: public CMtfTestCase
{
public:
	
	/** Creates a new object. The test case may be synchronous or asynchronous. */
	static CMtfScriptedTestCase* NewL(const CMtfTestServer& aTestServer, const TBool& aSynchronous);
    
    virtual ~CMtfScriptedTestCase();
	
private:
	CMtfScriptedTestCase(const CMtfTestServer& aTestServer, const TBool& aSynchronous);
	
	// we need to create an instance of this, so no pure virtuals
	virtual void SetupTestCaseL();
	virtual void ExecuteTestCaseL();
	virtual void CheckPostConditionsL();
};

#endif
