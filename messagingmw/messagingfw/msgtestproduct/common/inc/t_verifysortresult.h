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
 
#ifndef __T_VERIFY_SORT_RESULT_H__
#define __T_VERIFY_SORT_RESULT_H__

/* User includes */
#include "T_MsgSyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KVerifySortResult,"VerifySortResult");

/*
 * Verifies if message entries returned by the SORT mechanism
 * are in expected order.
 */
class CT_MsgVerifySortResult : public CT_MsgSyncStepCommon
	{
public:
	CT_MsgVerifySortResult(CT_MsgSharedDataCommon& aSharedDataCommon);
private:
	void DisplayMsgEntryList(const RArray<TMsvEntry>& aMessageEntries);
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
	};
#endif /*__T_VERIFY_SORT_RESULT_H__*/
