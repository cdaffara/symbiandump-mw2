// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//
 
#ifndef __T_DELETE_MESSAGE_SELECTION_H__
#define __T_DELETE_MESSAGE_SELECTION_H__

/* User  includes */
#include "T_MsgAsyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KDeleteMessageSelection,"DeleteMessageSelection");

/* 
 * Implements a test step  for deleting all/some of the messages from the selction 
 * list(CT_MsgSharedDataCommon::iSearchSortResultArray).
 */
class CT_MsgDeleteMessageSelection : public CT_MsgAsyncStepCommon
	{
public:
	/* Creates an object of CT_MsgDeleteMessageSelection */
	static CT_MsgDeleteMessageSelection* NewL(CT_MsgSharedDataCommon& aSharedDataCommon);

	~CT_MsgDeleteMessageSelection();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
protected:
	void ConstructL();
private:
	CT_MsgDeleteMessageSelection(CT_MsgSharedDataCommon& aSharedDataCommon);
	/* 
	 * Derived  from CT_MsgAsyncStep
	 * Displays the progress of the asynchronous operation
	 */
	virtual void ProgressL(TBool aFinal);

	/* 
	 * Derived from MT_MsgActiveCallback 
	 * Cancels an asynchronous operation
	 */
	virtual void CancelStep();
private:	
	CMsvOperation*	iOperation;
	};
#endif /*__T_DELETE_MESSAGE_SELECTION_H__*/
