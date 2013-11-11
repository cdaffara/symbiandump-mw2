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


#ifndef __CMTF_TEST_CASE_CREATE_SEND_RECEIVE_POP3_EMAIL_H__
#define __CMTF_TEST_CASE_CREATE_SEND_RECEIVE_POP3_EMAIL_H__


#include "CMtfTestCase.h"

_LIT(KTestCaseCreateSendReceivePop3Email,"TestCaseCreateSendReceivePop3Email");

class CMtfTestCaseCreateSendReceivePop3Email : public CMtfTestCase
	{
public:
	static CMtfTestCase* NewL(const CMtfTestServer& aTestServer);
	
private:
	CMtfTestCaseCreateSendReceivePop3Email(const CMtfTestServer& aTestServer);
	void ConstructL();

	virtual void SetupTestCaseL();
	virtual void ExecuteTestCaseL();
	virtual void CheckPostConditionsL();
	};


#endif
