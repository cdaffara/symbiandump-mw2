// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This is the header file for CT_MsgIsImapMessageHeaderOnly class
// 
//

#ifndef T_IS_IMAP_MESSAGE_HEADER_ONLY_H
#define T_IS_IMAP_MESSAGE_HEADER_ONLY_H

/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"
#include "T_UtilsEmailMessage.h"

/* Literal used */
_LIT(KIsImapMessageHeaderOnly,"IsImapMessageHeaderOnly");

/* Implements a test step to check if the fetched IMAP message for a given account is header only
   or content for it has been downloaded such as body and attachments */
class CT_MsgIsImapMessageHeaderOnly : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgIsImapMessageHeaderOnly */
	static CT_MsgIsImapMessageHeaderOnly* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgIsImapMessageHeaderOnly();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgIsImapMessageHeaderOnly(CT_MsgSharedDataImap& aSharedDataIMAP);
	TBool ReadIni();
	
	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* 
	 * Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	
private:
	/* 
	 * An asynchronously completing operation
	 * containing the status of the operation
	 */
	CMsvOperation* iOperation;

	TPtrC iImapAccountName;
	TPtrC iImapFolderName;
	TPtrC iSubject;
	TBool iHeaderOnly;
	
	// Utility class object for storing the information of the received email message
	CT_MsgUtilsEmailMessage* iRecvMessageInfo;
	
	};
#endif /* T_IS_IMAP_MESSAGE_HEADER_ONLY_H */
