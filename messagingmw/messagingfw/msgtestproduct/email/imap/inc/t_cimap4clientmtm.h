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
 
#ifndef __T_CIMAP4CLIENTMTM_H__
#define __T_CIMAP4CLIENTMTM_H__

/* User includes */
#include "T_MsgSyncStepIMAP.h"

/* Literals Used */
_LIT(KUnitTestCImap4ClientMtm,"UnitTestCImap4ClientMtm");

/*
 * A Unit test case to check the functionality of the API's of class CImap4ClientMtm 
 */
class CT_CImap4ClientMtm : public CT_MsgSyncStepIMAP, MMsvSessionObserver
	{
public:
	CT_CImap4ClientMtm(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_CImap4ClientMtm();
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);

private:
	void DisplayMsgEntryList(const RArray<TMsvEntry>& aMessageEntries);
	void TestValidateMessageL();
	void TestReplyL();
	void TestKErrNotSupportedAPI();
	void TestAddAddresseeL();
	void TestRemoveAddresseeL();
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
	TMsvId CreateMessageL();
private:
	CMsvSession *iSession;
	};
#endif /*__T_VERIFY_SORT_RESULT_H__*/
