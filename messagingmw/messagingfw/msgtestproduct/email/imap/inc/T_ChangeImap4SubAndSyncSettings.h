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
* This is the header file for CT_MsgChangeImap4SubAndSyncSettings class
* 
*
*/




#ifndef T_CHANGE_IMAP4_SUB_AND_SYNC_SETTINGS_H
#define T_CHANGE_IMAP4_SUB_AND_SYNC_SETTINGS_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KChangeImap4SubAndSyncSettings,"ChangeImap4SubAndSyncSettings");


/* Implements a test step to change the IMAP4 Subscribe and Synchronise settings */
class CT_MsgChangeImap4SubAndSyncSettings : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgChangeImap4SubAndSyncSettings */
	static CT_MsgChangeImap4SubAndSyncSettings* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgChangeImap4SubAndSyncSettings();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgChangeImap4SubAndSyncSettings(CT_MsgSharedDataImap& aSharedDataIMAP);
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
	
	TPtrC iImapSubscribeMethod;
	TPtrC iImapSynchroniseMethod;
	TPtrC iImapAccountName;
	};
	
#endif /* T_CHANGE_IMAP4_SUB_AND_SYNC_SETTINGS_H */
