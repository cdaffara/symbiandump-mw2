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


#ifndef __CMTF_SET_CONFIGURATION_TEST_STEP_H__
#define __CMTF_SET_CONFIGURATION_TEST_STEP_H__

#include <test/testexecutestepbase.h>

class CMtfTestServer;

/** This class is used by the framework to handle a SetConfiguration command in the main
script, which allows the script writer to set a particular configuration from the main script.*/
class CMtfSetConfigurationTestStep : public CTestStep
{
public:
	
	virtual  ~CMtfSetConfigurationTestStep();
	static CMtfSetConfigurationTestStep* NewL(CMtfTestServer& aTestServer);
	virtual TVerdict doTestStepPreambleL();  	
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
		
protected:	
	CMtfSetConfigurationTestStep(CMtfTestServer& aTestServer);
 
private:
	CMtfTestServer&	iTestServer;
};

#endif

