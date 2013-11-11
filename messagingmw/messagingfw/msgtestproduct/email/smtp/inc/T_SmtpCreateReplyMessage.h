/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @file 
* This is the header file for CT_SmtpCreateReplyMessage
* 
*
*/




#ifndef __T_SMTPCREATE_REPLYMESSAGE_H__
#define __T_SMTPCREATE_REPLYMESSAGE_H__


// User includes
#include "T_MsgAsyncStepSMTP.h"


// Literals used 
_LIT(KSmtpCreateReplyMessage,"SmtpCreateReplyMessage");


/* 
 * Implements a test step that create a reply message using the SMTP client MTM command
 */
class CT_SmtpCreateReplyMessage : public CT_MsgAsyncStepSMTP
	{
public:
	static CT_SmtpCreateReplyMessage* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_SmtpCreateReplyMessage();

	// CTestStep implementation 
	virtual TVerdict doTestStepL();

private:
	CT_SmtpCreateReplyMessage(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	void SetHeaderPartL();
	
	virtual void ProgressL(TBool bFinal);
	virtual void CancelStep();
	
private:
	CMsvOperation*	iOperation;
	};
#endif // __T_SMTPCREATE_REPLYMESSAGE_H__
