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
* This is the header file for CT_MsgSetImap4EmailPriority class
* 
*
*/



/**
 @file
*/


#ifndef T_SET_IMAP4_EMAIL_PRIORITY_H
#define T_SET_IMAP4_EMAIL_PRIORITY_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KSetImap4EmailPriority,"SetImap4EmailPriority");


/* Implements a test step to set the priority of an email via the IMAP \Flagged flag */
class CT_MsgSetImap4EmailPriority :  public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgSetImap4EmailPriority */
	static CT_MsgSetImap4EmailPriority* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
   ~CT_MsgSetImap4EmailPriority();

	/* CTestStep implementation */
	virtual TVerdict	doTestStepL();

private:
	CT_MsgSetImap4EmailPriority(CT_MsgSharedDataImap& aSharedDataIMAP);
	TBool ReadIni();

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void	ProgressL(TBool bFinal);

	/* 
	 * Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void	CancelStep();

private:
	/* 
	 * An asynchronously completing operation
	 * containing the status of the operation
	 */
	CMsvOperation* iOperation;
	
	TPtrC iParentFolderName;
	TPtrC iEmailSubject;
	TInt  iEmailPriority;
	TPtrC iLocation;
	TPtrC iImapAccountName;
	};

#endif /* T_SET_IMAP4_EMAIL_PRIORITY_H */
