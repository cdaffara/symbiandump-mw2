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
// t_csmvsession.h
// 
//
 
#ifndef __T_CMSVSESSION_H__
#define __T_CMSVSESSION_H__

/* User includes */
#include "T_MsgSyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KUnitTestCMsvSession,"UnitTestCMsvSession");

/*
 * A Unit test case to check the functionality of the API's of class CMsvSession 
 */
class CT_CMsvSession : public CT_MsgSyncStepCommon, MMsvSessionObserver
	{
public:
	CT_CMsvSession(CT_MsgSharedDataCommon& aSharedDataCommon);
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
private:
	void DisplayMsgEntryList(const RArray<TMsvEntry>& aMessageEntries);
	void TestOpenSyncL();
	void TestOpenAsyncL();
	void TestOpenAsObserverL();
	void TestAddObserverL();
	void TestRemoveObserver();
	void TestSetReceiveEntryEvents();
	void TestGetEntryL();
	void TestTransferCommandL();
	void TestIncPcSyncCountL();
	void TestDecPcSyncCountL();
	void TestGetChildIdsL();
	void TestChangeAttributesL();
	void TestChangeDriveL();
	void TestOutstandingOperationsL();
	void TestCopyStoreL();
	void TestDeleteStoreL();
	void TestCleanupEntry();
	void TestCleanupEntryPushL();
	void TestCleanupEntryPop();
	void TestRemoveEntry();
	void TestInstallMtmGroup();
	void TestDeInstallMtmGroup();
	void TestStopService();
	void TestServiceActive();
	void TestServiceProgress();
	void TestCloseMessageServer();
	void TestFileSession();
	void TestGetMtmRequiredCapabilitiesL();
	void TestStoreManager();
	void TestGetAndClearIndexCorruptFlagL();
	void TestCurrentDriveL();
	void TestDriveContainsStoreL();
	void TestMessageStoreDrivePresentL();
	void TestServiceAccessPointId();
	void TestNotifyAllObserversL();
	void TestCleanupEntryDelete();
	void TestDoRunL();
	void TestHandleNotifyL();
	void TestDoHandleNotifyL();
	void TestGetMessageFolderL();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
private:
	CMsvSession *iSession;
	};
#endif /*__T_VERIFY_SORT_RESULT_H__*/
