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
* This is the header file for CT_MsgSendSmtpMessage
* 
*
*/



/**
 @file
*/


#ifndef __T_SEND_SMTP_MESSAGE_H__
#define __T_SEND_SMTP_MESSAGE_H__


// User Includes
#include "T_MsgAsyncStepSMTP.h"
#include "T_MsgSharedDataSmtp.h"


//Literals Used
_LIT(KSendSmtpMessage,"SendSmtpMessage");


/**
Implements a test step to send a message
*/
class CT_MsgSendSmtpMessage :  public CT_MsgAsyncStepSMTP
	{
public:
	static CT_MsgSendSmtpMessage* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
   ~CT_MsgSendSmtpMessage();

	//	CTestStep implementation
	virtual TVerdict	doTestStepL();

private:
	CT_MsgSendSmtpMessage(CT_MsgSharedDataSmtp& aSharedDataSMTP);

	//	CT_MsgAsyncStep implementation
	virtual void	ProgressL(TBool bFinal);

	//	MT_MsgActiveCallback implementation
	virtual void	CancelStep();

private:
	CMsvOperation*	iOperation;
	};

#endif //__T_SEND_SMTP_MESSAGE_H__
