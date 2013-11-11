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
// This is the header file for CT_MsgCopyMsg class
// 
//



#ifndef __T_COPY_MESSAGE_H__
#define __T_COPY_MESSAGE_H__

//User  includes
#include "T_MsgAsyncStepIMAP.h"

//Literals Used
_LIT(KCopyMessage,"CopyMessage");


// Implements a test step to copy message
class CT_MsgCopyMsg : public CT_MsgAsyncStepIMAP
	{
public:
	static CT_MsgCopyMsg* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	TBool CT_MsgCopyMsg::ReadIni();
	~CT_MsgCopyMsg();
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
	
	CT_MsgCopyMsg(CT_MsgSharedDataImap& aSharedDataIMAP);
	
	/* 
	 * A Pointer of type CT_MsgUtilsDeleteAllChildren to delete
	 * all the entries under the specifed folder
	 */
	CMsvOperation*	iOperation;
	
	TPtrC iSubject;
	TPtrC iTrgtLookupIn;
	TPtrC iSrcLookupIn;
	TPtrC iSrcfolderName;
	TPtrC iSrcParentFolderName;
	TPtrC iImapAccountName;
	TPtrC iTrgtfolderName;
	TPtrC iTrgtParentFolderName;
	TInt iExpectedErr;
	
	};
#endif //__T_COPY_MESSAGE_H__

