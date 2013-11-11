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
 
#ifndef __T_CPOP3CLIENTMTM_H__
#define __T_CPOP3CLIENTMTM_H__

/* User includes */
#include "T_MsgSyncStepPop.h"

/* Literals Used */
_LIT(KUnitTestCPop3ClientMtm,"UnitTestCPop3ClientMtm");

/*
 * A Unit test case to check the functionality of the API's of class CImap4ClientMtm 
 */
class CT_CPop3ClientMtm : public CT_MsgSyncStepPOP, MMsvSessionObserver
	{
public:
	CT_CPop3ClientMtm(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_CPop3ClientMtm();
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
#endif /*__T_CPOP3CLIENTMTM_H__*/
