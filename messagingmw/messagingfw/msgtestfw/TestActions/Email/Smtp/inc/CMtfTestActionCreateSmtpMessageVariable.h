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


#ifndef __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_VARIABLE_H__
#define __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_VARIABLE_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionCreateSmtpMessageVariable,"CreateSmtpMessageVariable");


class CMtfTestActionCreateSmtpMessageVariable : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCreateSmtpMessageVariable(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionCreateSmtpMessageVariable(CMtfTestCase& aTestCase);
	};


#endif
