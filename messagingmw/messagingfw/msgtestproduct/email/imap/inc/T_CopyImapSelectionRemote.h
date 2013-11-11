/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgCopyImapSelectionRemote class
* 
*
*/




#ifndef T_COPY_IMAP_SELECTION_REMOTE_H
#define T_COPY_IMAP_SELECTION_REMOTE_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KCopyImapSelectionRemote,"CopyImapSelectionRemote");


/*** 
 * Implements a test step that copies the entries from the specified remote source folder
 * to the specified remote target folder
 */
class CT_MsgCopyImapSelectionRemote  : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgCopyImapSelectionRemote */
	static CT_MsgCopyImapSelectionRemote* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgCopyImapSelectionRemote();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgCopyImapSelectionRemote(CT_MsgSharedDataImap& aSharedDataIMAP);
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
	TPtrC iParentSrcFolderName;
	TPtrC iSrcFolderName;
	TPtrC iParentTargetFolderName;
	TPtrC iTargetFolderName;
	};
#endif /* T_COPY_IMAP_SELECTION_REMOTE_H */
