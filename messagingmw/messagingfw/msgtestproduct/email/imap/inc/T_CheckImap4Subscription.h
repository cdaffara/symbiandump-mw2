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
* This is the header file for CT_CheckImap4Subscription class
* 
*
*/




#ifndef T_CHECK_IMAP4_SUBSCRIPTION_H
#define T_CHECK_IMAP4_SUBSCRIPTION_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KCheckImap4Subscription,"CheckImap4Subscription");


/* Implements a test step that checks if a IMAP4 folder is subscribed or unsubscribed */
class CT_MsgCheckImap4Subscription : public CT_MsgAsyncStepIMAP
	{	
public : 
	/* Creates an object of CT_MsgCheckImap4Subscription */
	static CT_MsgCheckImap4Subscription* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgCheckImap4Subscription();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgCheckImap4Subscription(CT_MsgSharedDataImap& aSharedDataIMAP);
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
	TPtrC iParentName;
	TPtrC iFolderName;
	TPtrC iExpectedSubscription;
	TPtrC iLocation;
	};
#endif /* T_CHECK_IMAP4_SUBSCRIPTION_H */
