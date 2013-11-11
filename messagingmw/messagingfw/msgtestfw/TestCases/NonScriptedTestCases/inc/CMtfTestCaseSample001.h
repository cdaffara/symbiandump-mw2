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


#ifndef __CMTF_TEST_CASE_SAMPLE_001_H__
#define __CMTF_TEST_CASE_SAMPLE_001_H__


#include "CMtfTestCase.h"

_LIT(KTestCaseSample001,"TestCaseSample001");

class CMtfTestCaseSample001 : public CMtfTestCase
{
public:
	static CMtfTestCase* NewL(const CMtfTestServer& aTestServer);
	
private:
	CMtfTestCaseSample001(const CMtfTestServer& aTestServer);
	
	virtual void SetupTestCaseL();
	virtual void ExecuteTestCaseL();
	virtual void CheckPostConditionsL();
};


#endif
