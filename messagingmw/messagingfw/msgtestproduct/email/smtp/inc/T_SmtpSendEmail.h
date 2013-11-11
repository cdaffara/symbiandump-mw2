/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgSmtpSendEmail
* 
*
*/



/**
 @file
*/


#ifndef __T_SMTP_SEND_EMAIL_H__
#define __T_SMTP_SEND_EMAIL_H__


// user includes
#include <test/testexecutestepbase.h>
#include "T_MsgAsyncStepSMTP.h"
#include "T_MsgSharedDataSmtp.h"


//Literals Used
_LIT(KSmtpSendEmail,"SmtpSendEmail");
_LIT(KNone, "NONE");


// Forward declaration
class CT_MsgUtilsSendEmail;
class CT_MsgUtilsConfigFileMachineName;


/**
Implements a test step to send an email
*/
class CT_MsgSmtpSendEmail :  public CT_MsgAsyncStepSMTP
	{
public:
	static CT_MsgSmtpSendEmail* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_MsgSmtpSendEmail();

	//	CTestStep implementation
	virtual TVerdict	doTestStepL();

private:
	CT_MsgSmtpSendEmail(CT_MsgSharedDataSmtp& aSharedDataSMTP);

	//	CT_MsgAsyncStep implementation
	virtual void	ProgressL(TBool bFinal);

	//	MT_MsgActiveCallback implementation
	virtual void	CancelStep();

public:
 	CT_MsgUtilsSendEmail*				iSendEmail;  
 	CT_MsgUtilsConfigFileMachineName*	iMachineNameFile;   
	};

#endif //__T_SMTP_SEND_EMAIL_H__
