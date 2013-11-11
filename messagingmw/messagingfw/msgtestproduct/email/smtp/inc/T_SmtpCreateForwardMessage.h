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
* This is the header file for CT_SmtpCreateForwadMessage
* 
*
*/




#ifndef __T_SMTPCREATE_FORWARDMESSAGE_H__
#define __T_SMTPCREATE_FORWARDMESSAGE_H__


// User includes
#include "T_MsgAsyncStepSMTP.h"


// Literals used 
_LIT(KSmtpCreateForwadMessage,"SmtpCreateForwadMessage");


/* 
 * Implements a test step that create a forwrad message using the SMTP client MTM command
 */
class CT_SmtpCreateForwadMessage : public CT_MsgAsyncStepSMTP
	{
public:
	static CT_SmtpCreateForwadMessage* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_SmtpCreateForwadMessage();

	// CTestStep implementation 
	virtual TVerdict doTestStepL();

private:
	CT_SmtpCreateForwadMessage(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	void SetHeaderPartL();
	void GetDefaultAddressL();

	virtual void ProgressL(TBool bFinal);
	virtual void CancelStep();

private:
	HBufC* iRecipientAddress;
	CMsvOperation*	iOperation;
	};
#endif // __T_SMTPCREATE_FORWARDMESSAGE_H__
