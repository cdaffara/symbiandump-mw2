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
// @file 
// Class API for searching using Client API.
// 
//

#ifndef _T_MSGCLIENTSEARCH_H_
#define _T_MSHCLIENTSEARCH_H_

/* User  includes */
#include "T_MsgAsyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KClientSearch,"ClientSearch");


/*  
 * Implements a test step to search for "To, CC, BCC, Subject and Body Field" in email account under the remote inbox folder
 * for IMAP
 */
class CT_MsgClientSearch : public CT_MsgAsyncStepCommon
	{
public:
	/* Creates an object of type CT_MsgClientSearch */
	static CT_MsgClientSearch* NewL(CT_MsgSharedDataCommon& aSharedDataCommon);
	~CT_MsgClientSearch();

	/*	CTestStep implementation */
	virtual TVerdict	doTestStepL();
protected:
	void ConstructL();
private:
	CT_MsgClientSearch(CT_MsgSharedDataCommon& aSharedDataCommon);

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	void TestSearchForToFieldL();
	void TestSearchForCCFieldL();
	void TestSearchForBCCFieldL();
	void TestSearchForSubjectFieldL();
	void TestSearchForDateFieldL();
	void TestSearchForBodyPartL();
	};
#endif
