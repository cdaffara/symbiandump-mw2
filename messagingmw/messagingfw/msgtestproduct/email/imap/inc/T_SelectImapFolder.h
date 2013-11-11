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
// This is the header file for CT_MsgSelectImapFolder
// 
//

#ifndef __T_SELECT_IMAP_FOLDER_H__
#define __T_SELECT_IMAP_FOLDER_H__

//User  includes
#include "T_MsgAsyncStepIMAP.h"

// Forward class declaration
class CT_MsgImapServer;

// Literals used
_LIT(KSelectImapFolder,"SelectImapFolder");

// Implements a test step that selects the Imap folder for further operations on it
class CT_MsgSelectImapFolder : public	CT_MsgAsyncStepIMAP
	{	
public : 
	static CT_MsgSelectImapFolder* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgSelectImapFolder();
	TVerdict doTestStepL();
private:
	CT_MsgSelectImapFolder(CT_MsgSharedDataImap& aSharedDataIMAP);
	
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
	 * A Pointer of type CT_MsgUtilsDeleteAllChildren to delete
	 * all the entries under the specifed folder
	 */
	CMsvOperation*	iOperation;
	
	TBool ReadIni();
	
	TPtrC iImapAccountName;
	TPtrC iSrcFolderName;
	TPtrC iSrcParentFolderName;
	TInt iExpectedErr;
	};

#endif //__T_SELECT_IMAP_FOLDER_H__
