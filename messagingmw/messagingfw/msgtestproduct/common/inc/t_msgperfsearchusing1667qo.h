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
 
#ifndef __T_MSGPERFSEARCHUSING1667QO_H__
#define __T_MSGPERFSEARCHUSING1667QO_H__

/* User  includes */
#include "T_MsgAsyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KPerfSearchUsing1667QO,"PerfSearchUsing1667QO");

/* 
 * Implements a test step  for getting the number of message entries satisfying a given serach-sort
 * criteria. Search/Sort criteria is embedded within a query object.
 */
class CT_MsgPerfSearchUsing1667QO : public CT_MsgAsyncStepCommon
	{
public:
	/* Creates an object of CT_MsgPerfSearchUsing1667QO */
	static CT_MsgPerfSearchUsing1667QO* NewL(CT_MsgSharedDataCommon& aSharedDataCommon);

	~CT_MsgPerfSearchUsing1667QO();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
protected:
	void ConstructL();
private:
	CT_MsgPerfSearchUsing1667QO(CT_MsgSharedDataCommon& aSharedDataCommon);
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
