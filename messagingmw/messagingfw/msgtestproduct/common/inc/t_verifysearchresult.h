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
 
#ifndef __T_VERIFY_SEARCH_RESULT_H__
#define __T_VERIFY_SEARCH_RESULT_H__

/* User includes */
#include "T_MsgSyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KVerifySearchResult,"VerifySearchResult");

/* 
 * Implements the test step to verify if the result returned by search request are
 * satisfying the search criteria.
 */
class CT_MsgVerifySearchResult : public CT_MsgSyncStepCommon
	{
public:
	CT_MsgVerifySearchResult(CT_MsgSharedDataCommon& aSharedDataCommon);
private:
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	TBool VerifyEmailHeaderFieldL(TMsvEntry& aEntry, TDesC& aSearchString, TMsvMessagePart aMsgPart, TBool aWildcardFlag = EFalse);
	TBool VerifySmsHeaderFieldL(TMsvEntry& aEntry, TDesC& aSearchString, TMsvMessagePart aMsgPart);
	};
#endif /*__T_VERIFY_SEARCH_RESULT_H__*/
