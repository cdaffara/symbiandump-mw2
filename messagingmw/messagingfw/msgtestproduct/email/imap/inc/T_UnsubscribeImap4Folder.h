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
* This is the header file for CT_MsgUnsubscribeImap4Folder class
* 
*
*/




#ifndef T_UNSUBSCRIBE_IMAP4_FOLDER_H
#define T_UNSUBSCRIBE_IMAP4_FOLDER_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KUnsubscribeImap4Folder,"UnsubscribeImap4Folder");


/* Implements a test step that unsubscribes a IMAP4 folder */
class CT_MsgUnsubscribeImap4Folder : public	CT_MsgAsyncStepIMAP
	{	
public:
	/* Creates an object of CT_MsgUnsubscribeImap4Folder */
	static CT_MsgUnsubscribeImap4Folder* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgUnsubscribeImap4Folder();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgUnsubscribeImap4Folder(CT_MsgSharedDataImap& aSharedDataIMAP);
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
	};
#endif /* T_UNSUBSCRIBE_IMAP4_FOLDER_H */
