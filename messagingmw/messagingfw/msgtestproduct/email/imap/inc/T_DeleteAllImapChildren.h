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
* This is the header file for CT_MsgDeleteAllImapChildren class
* 
*
*/




#ifndef __T_DELETE_ALL_IMAP_CHILDREN_H__
#define __T_DELETE_ALL_IMAP_CHILDREN_H__


/* User  includes */
#include "T_MsgAsyncStepIMAP.h"

/* Forward decleration */
class CT_MsgUtilsDeleteAllChildren;

/* Literals Used */
_LIT(KDeleteAllImapChildren,"DeleteAllImapChildren");


/* 
 * Implements a test step to delete all the children entries under the remote inbox folder
 * for IMAP
 */
class CT_MsgDeleteAllImapChildren : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of type CT_MsgDeleteAllImapChildren */
	static CT_MsgDeleteAllImapChildren* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgDeleteAllImapChildren();

	/*	CTestStep implementation */
	virtual TVerdict	doTestStepL();
private:
	CT_MsgDeleteAllImapChildren(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	/* 
	 * A Pointer of type CT_MsgUtilsDeleteAllChildren to delete
	 * all the entries under the specifed folder
	 */
	CT_MsgUtilsDeleteAllChildren* iUtil;
	};
#endif /* __T_DELETE_ALL_IMAP_CHILDREN_H__ */

