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

#ifndef __CMTF_TEST_BEGIN_MANAGER_H__
#define __CMTF_TEST_BEGIN_MANAGER_H__

#include <test/testexecutestepbase.h>
class CMtfTestServer;

/** Used by the framework to create a new scripted test case. */
class CMtfTestBeginManager : public CTestStep
{
public:
	virtual ~CMtfTestBeginManager();
	
	static CMtfTestBeginManager* NewL(CMtfTestServer& aTestServer, 
			const TBool& aSynchronous);

	/** Required by TestExecute. */
	virtual TVerdict doTestStepPreambleL();  
	
	/** Required by TestExecute. */	
	virtual TVerdict doTestStepL();
	
	/** Required by TestExecute. */
	virtual TVerdict doTestStepPostambleL();   
	
private:
	CMtfTestBeginManager(CMtfTestServer& aTestServer,
			const TBool& aSynchronous);
	
private:
	CMtfTestServer&	iTestServer;
	TBool 			iSynchronous;
};

#endif

