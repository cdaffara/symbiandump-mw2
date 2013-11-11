/*
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
* @file 
* This is the header file for CT_MsgInboxNewEmailsSync
* 
*
*/




#ifndef __T_INBOXNEW_EMAILS_SYNC_H__
#define __T_INBOXNEW_EMAILS_SYNC_H__


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/*  Epoc include */
#include <mtclbase.h>


/* Forward class declaration */
class CT_MsgImapServer;

/* Literals used */
_LIT(KInboxNewEmailsSync,"InboxNewEmailsSync");


/* 
 * Implements a test step to select the remote inbox and check for new mails
 * (but doesn't delete ones that have gone). 
 */
class CT_MsgInboxNewEmailsSync : public	CT_MsgAsyncStepIMAP
	{	
public : 
	/* Creates an object of type CT_MsgInboxNewEmailsSync */
	static CT_MsgInboxNewEmailsSync* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgInboxNewEmailsSync();

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
private:
	CT_MsgInboxNewEmailsSync(CT_MsgSharedDataImap& aSharedDataIMAP);

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
private:
	/* 
	 * A Pointer of type CMsvOperation to retrieve the status of the operation
	 */
	CMsvOperation*	iOperation;
	};
#endif /*__T_INBOXNEW_EMAILS_SYNC_H__*/
