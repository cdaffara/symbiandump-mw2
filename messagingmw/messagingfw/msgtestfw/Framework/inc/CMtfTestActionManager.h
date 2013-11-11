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


#ifndef __CMTF_TEST_ACTION_MANAGER_H__
#define __CMTF_TEST_ACTION_MANAGER_H__

#include <test/testexecutestepbase.h>

class CMtfTestServer;
class CMtfTestAction;

/** CMtfTestActionManager is used by the framework to create action instances. */
class CMtfTestActionManager : public CTestStep
{
public:
	virtual  ~CMtfTestActionManager();
	
	/** Creates a new action manager for the specified test action. Test action is identified
	by its name. */
	static CMtfTestActionManager* NewL(CMtfTestServer& aTestServer,
			const TDesC& aActionName);
	
	/** Creates a new dummy action manager, which does nothing. Used by the framework to handle 
	script section for which don't require any action to be performed. */
	static CMtfTestActionManager* NewL(CMtfTestServer& aTestServer);
	
	/** This function is required by TestExecute. */
	virtual TVerdict doTestStepPreambleL();  	
	
	/** This function is required by TestExecute. */
	virtual TVerdict doTestStepL();
	
	/** This function is required by TestExecute. */
	virtual TVerdict doTestStepPostambleL();
		
protected:	
	CMtfTestActionManager(CMtfTestServer& aTestServer);

private:
	void ConstructL(const TDesC& aActionName);
	
private:
	CMtfTestServer&	iTestServer;
	HBufC*	iActionName; 
};

#endif

