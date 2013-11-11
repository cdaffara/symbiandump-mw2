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
// t_csmventry.h
// 
//
 
#ifndef __T_CMSVENTRY_H__
#define __T_CMSVENTRY_H__

/* User includes */
#include "T_MsgSyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KUnitTestCMsvEntry,"UnitTestCMsvEntry");

/*
 * A Unit test case to check the functionality of the API's of class CMsvSession 
 */
class CT_CMsvEntry : public CT_MsgSyncStepCommon, MMsvSessionObserver
	{
public:
	CT_CMsvEntry(CT_MsgSharedDataCommon& aSharedDataCommon);
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
private:
	void DisplayMsgEntryList(const RArray<TMsvEntry>& aMessageEntries);
	void TestCopySyncL();
	void TestCopyAsyncL();
	void TestMoveSyncL();
	void TestMoveAsyncL();
	void CheckNumberOfEntriesL(CMsvEntry &aEntry,TMsvId aId,TInt aRequiredNo);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
private:
	CMsvSession *iSession;
	};
#endif /*__T_CMSVENTRY_H__*/
