/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_CREATE_EXP_POP3_MAIL_INFO_H__
#define __CMTF_TEST_ACTION_CREATE_EXP_POP3_MAIL_INFO_H__

#include <popcmtm.h>

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionCreateExpPop3MailInfo,"CreateExpPop3MailInfo");



class CMtfTestActionCreateExpPop3MailInfo : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);
	virtual ~CMtfTestActionCreateExpPop3MailInfo(); 
	virtual void ExecuteActionL();
private:
	CMtfTestActionCreateExpPop3MailInfo(CMtfTestCase& aTestCase);
	};



#endif  // ! defined _CMTF_TEST_ACTION_CREATE_EXP_POP3_MAIL_INFO_H__
