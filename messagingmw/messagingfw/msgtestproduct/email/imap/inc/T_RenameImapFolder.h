// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// @file
// This is the header file for CT_MsgRenameImapFolder
// 
//

#ifndef __T_RENAME_FOLDER_H__
#define __T_RENAME_FOLDER_H__

//User  includes
#include "T_MsgAsyncStepIMAP.h"

// Literals used
_LIT(KRenameImapFolder,"RenameImapFolder");

// Implements a test step that Renames the Imap Folder
class CT_MsgRenameImapFolder : public	CT_MsgAsyncStepIMAP
	{	
public : 
	static CT_MsgRenameImapFolder* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgRenameImapFolder();
	TVerdict doTestStepL();
private:

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	
	/* 
	 * An asynchronously completing operation containing the status of the operation
	 */
	CMsvOperation*	iOperation;
	
	CT_MsgRenameImapFolder(CT_MsgSharedDataImap& aSharedDataIMAP);
	TBool ReadIni();
	
	TPtrC iImapAccountName;
	TPtrC iSrcFolderName;
	TPtrC iSrcParentFolderName;
	TPtrC iNewFolderName;
	};
#endif //__T_RENAME_FOLDER_H__
