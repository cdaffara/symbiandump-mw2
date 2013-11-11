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


#ifndef __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_FROM_PARAM_EMAIL_FILE_H__
#define __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_FROM_PARAM_EMAIL_FILE_H__

#include <sendas.h>

#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionCreateSmtpMessageFromParamEmailFile, "CreateSmtpMessageFromParamFile");

class CPop3MailInfo;

class CMtfTestActionCreateSmtpMessageFromParamEmailFile : public CMtfSynchronousTestAction, MSendAsObserver
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCreateSmtpMessageFromParamEmailFile();
	virtual void ExecuteActionL();
	TBool CapabilityOK(TUid /*aCapabilty*/, TInt /*aResponse*/ ) { return ETrue; }

private:
	CMtfTestActionCreateSmtpMessageFromParamEmailFile(CMtfTestCase& aTestCase);
	TMsvId CreateAndSendMessageL( CPop3MailInfo &mailInfo );
	};


#endif // __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_FROM_PARAM_EMAIL_FILE_H__
