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
* This is the header file for CT_MsgCopyImapSelection
* 
*
*/




#ifndef __T_COPY_IMAP_SELECTION_H__
#define __T_COPY_IMAP_SELECTION_H__


/* User includes */
#include "T_MsgAsyncStepIMAP.h"


/* Literals Used */
_LIT(KCopyImapSelection,"CopyImapSelection");


/* 
 * Implements a test step to copy the entries from the specified remote folder
 * to the specified target folder read from the ini file
 */
class CT_MsgCopyImapSelection  : public CT_MsgAsyncStepIMAP
	{
public: 
	/* Creates an object of CT_MsgCopyImapSelection */
	static CT_MsgCopyImapSelection* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgCopyImapSelection();

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
private:
	CT_MsgCopyImapSelection(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	/* An asynchronously completing operation
	 * containing the status of the operation
	 */
	CMsvOperation*	iOperation;
	};
#endif /* __T_COPY_IMAP_SELECTION_H__ */
