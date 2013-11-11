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
* This is the header file for CT_MsgCheckImap4EmailPriority class
* 
*
*/



/**
 @file
*/


#ifndef T_CHECK_IMAP4_EMAIL_PRIORITY_H
#define T_CHECK_IMAP4_EMAIL_PRIORITY_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KCheckImap4EmailPriority,"CheckImap4EmailPriority");


/* Implements a test step to check the priority of an email via the IMAP \Flagged flag */
class CT_MsgCheckImap4EmailPriority : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgCheckImap4EmailPriority */
	static CT_MsgCheckImap4EmailPriority* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
   ~CT_MsgCheckImap4EmailPriority();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();

private:
	CT_MsgCheckImap4EmailPriority(CT_MsgSharedDataImap& aSharedDataIMAP);
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
	
	TPtrC iParentFolderName;
	TPtrC iEmailSubject;
	TPtrC iTarget;
	TPtrC iImapAccountName;
	TInt  iExpectedPriority;
	};

#endif /* T_CHECK_IMAP4_EMAIL_PRIORITY_H */
