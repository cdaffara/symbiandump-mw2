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
* This is the header file for CT_MsgCopyStoreImapToRemoteDrive class
* 
*
*/




#ifndef T_COPYSTORE_IMAP_TO_REMOTE_DRIVE_H
#define T_COPYSTORE_IMAP_TO_REMOTE_DRIVE_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KCopyStoreImapToRemoteDrive,"CopyStoreImapToRemoteDrive");


/*** 
 * Implements a test step that copies the entries from the specified remote source folder
 * to the specified remote target folder
 */
class CT_MsgCopyStoreImapToRemoteDrive  : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgCopyStoreImapToRemoteDrive */
	static CT_MsgCopyStoreImapToRemoteDrive* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgCopyStoreImapToRemoteDrive();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgCopyStoreImapToRemoteDrive(CT_MsgSharedDataImap& aSharedDataIMAP);
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

	TInt iDriveId;
	};
#endif /* T_COPYSTORE_IMAP_TO_REMOTE_DRIVE_H */
