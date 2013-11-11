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
* This is the header file for CT_SmtpCreateNewMessageWithPlainBodyText
* 
*
*/




#ifndef __T_SMTP_CREATENEWMESSAGE_WITHPLAINBODYTEXT_H__
#define __T_SMTP_CREATENEWMESSAGE_WITHPLAINBODYTEXT_H__


// User includes
#include "T_MsgAsyncStepSMTP.h"
#include "T_MsgSharedDataSmtp.h"

// Epoc includes 
#include <imapcmds.h>
#include <msvstd.h>
#include <mtclbase.h>

// Literals used 
_LIT(KSmtpCreateNewMessageWithPlainBodyText,"SmtpCreateNewMessageWithPlainBodyText");


/* 
 * Implements a test step that create a message with plain body text
 */
class CT_SmtpCreateNewMessageWithPlainBodyText : public CT_MsgAsyncStepSMTP
	{
public:
	static CT_SmtpCreateNewMessageWithPlainBodyText* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_SmtpCreateNewMessageWithPlainBodyText();

	// CTestStep implementation 
	virtual TVerdict doTestStepL();

private:
	CT_SmtpCreateNewMessageWithPlainBodyText(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	void SetHeaderPartL();
	void GetDefaultAddressL();
	void PopulateBodyPartL();

	virtual void ProgressL(TBool bFinal);
	virtual void CancelStep();
	
private:
	HBufC* iRecipientAddress;	
	CMsvOperation*	iOperation;
	};
#endif // __T_SMTP_CREATENEWMESSAGE_WITHPLAINBODYTEXT_H__
