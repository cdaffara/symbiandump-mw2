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
 
#ifndef __T_GET_SEARCHSORT_RESULT_COUNT_H__
#define __T_GET_SEARCHSORT_RESULT_COUNT_H__

/* User  includes */
#include "T_MsgAsyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KSearchSortByQueryObject,"SearchSortByQueryObject");

/* 
 * Implements a test step  for getting the number of message entries satisfying a given serach-sort
 * criteria. Search/Sort criteria is embedded within a query object.
 */
class CT_MsgSearchSortByQueryObject : public CT_MsgAsyncStepCommon
	{
public:
	/* Creates an object of CT_MsgSearchSortByQueryObject */
	static CT_MsgSearchSortByQueryObject* NewL(CT_MsgSharedDataCommon& aSharedDataCommon);

	~CT_MsgSearchSortByQueryObject();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
protected:
	void ConstructL();
private:
	CT_MsgSearchSortByQueryObject(CT_MsgSharedDataCommon& aSharedDataCommon);
	void RetriveSearchSortResultL(const TInt aIterationLimit, TMsvSearchSortResultType aResultType);
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
	};
#endif /* __T_GET_SEARCHSORT_RESULT_COUNT_H__ */
