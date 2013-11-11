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
* This is the header file for CT_MsgSubscribeImap4Folder class
* 
*
*/


 

#ifndef T_SUBSCRIBE_IMAP4_FOLDER_H
#define T_SUBSCRIBE_IMAP4_FOLDER_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KSubscribeImap4Folder,"SubscribeImap4Folder");


/* Implements a test step that subscribes to a IMAP4 folder */
class CT_MsgSubscribeImap4Folder : public CT_MsgAsyncStepIMAP
	{	
public:
	/* Creates an object of CT_MsgSubscribeImap4Folder */
	static CT_MsgSubscribeImap4Folder* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgSubscribeImap4Folder();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgSubscribeImap4Folder(CT_MsgSharedDataImap& aSharedDataIMAP);
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
#endif /* T_SUBSCRIBE_IMAP4_FOLDER_H */
