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
// Class API for searching using messaging server API.
// 
//

#ifndef _T_MSGSEARCHENTRY_H_
#define _T_MSHSEARCHENTRY_H_

/* User  includes */
#include "T_MsgAsyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KSearchEntry,"SearchEntry");


/*  
 * Implements a test step to search in the remote inbox folder from the server side.
 * for IMAP
 */
class CT_MsgSearchEntry : public CT_MsgAsyncStepCommon
	{
public:
	/* Creates an object of type CT_MsgSearchEntry */
	static CT_MsgSearchEntry* NewL(CT_MsgSharedDataCommon& aSharedDataCommon);
	~CT_MsgSearchEntry();

	/*	CTestStep implementation */
	virtual TVerdict	doTestStepL();
protected:
	void ConstructL();
private:
	CT_MsgSearchEntry(CT_MsgSharedDataCommon& aSharedDataCommon);

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	TBool FindRecipeints(const CDesCArray& aRecipientArray,const TDesC& recipeintAddress);
	void TestSearchForToFieldL();
	void TestSearchForCCFieldL();
	void TestSearchForBCCFieldL();
	void TestSearchForSubjectFieldL();
	void TestSearchForDateFieldL();
	void TestSearchForTimeFieldL();
	void TestSearchForBodyPartL();
	};
#endif
