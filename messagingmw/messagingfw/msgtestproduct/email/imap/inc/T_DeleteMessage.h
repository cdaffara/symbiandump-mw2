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
// This is the header file for CT_MsgDeleteMsg class
// 
//

#ifndef __T_DELETE_MESSAGE_H__
#define __T_DELETE_MESSAGE_H__

//User  includes
#include "T_MsgAsyncStepIMAP.h"

//Literals Used
_LIT(KDeleteMessage,"DeleteMessage");


// Implements a test step to delete message
class CT_MsgDeleteMsg : public CT_MsgAsyncStepIMAP
	{
public:
	static CT_MsgDeleteMsg* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgDeleteMsg();
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
	
	CT_MsgDeleteMsg(CT_MsgSharedDataImap& aSharedDataIMAP);
	TBool CT_MsgDeleteMsg::ReadIni();
	
	/* 
	 * A Pointer of type CT_MsgUtilsDeleteAllChildren to delete
	 * all the entries under the specifed folder
	 */
	CMsvOperation*	iOperation;
	
	TPtrC iSubject;
	TPtrC iImapAccountName;
	TPtrC iFolderName;
	TPtrC iParentFolderName;
	TPtrC iLookUpIn;
	TInt iExpectedErr;
	};
#endif //__T_DELETE_MESSAGE_H__

