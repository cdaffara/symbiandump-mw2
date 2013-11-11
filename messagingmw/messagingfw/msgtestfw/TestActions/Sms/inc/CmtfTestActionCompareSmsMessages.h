/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/

#ifndef __CMTF_TEST_ACTION_COMPARE_SMS_MESSAGES_H__
#define __CMTF_TEST_ACTION_COMPARE_SMS_MESSAGES_H__

#include "CMtfSynchronousTestAction.h"

class CMsvEntry;
class CSmsHeader;

_LIT(KTestActionCompareSmsMessages,"CompareSmsMessages");


class CMtfTestActionCompareSmsMessages : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCompareSmsMessages();

public:
	virtual void ExecuteActionL();
public:
	enum TResult
	{
	EAllMatched = 0,
	EFailedToMatchHeader,
	EFailedToMatchBody,
	EFailedToMatchType,
	EFailedToMatchServiceId,
	EFailedToMatchPriority,
	EFailedToMatchVisibility,
	EFailedToMatchSize
	};

private:
	CMsvSession* iParamSession;

private:
	CMtfTestActionCompareSmsMessages(CMtfTestCase& aTestCase);
	HBufC* GetBodyTextL(CMsvEntry& aMessage);
	CSmsHeader*  GetSubmitHeaderL(CMsvEntry& aMessage);
	CSmsHeader*  GetDeliverHeaderL(CMsvEntry& aMessage);
	TBool  IsMatchL(CMsvEntry& aFolder1Entry, CMsvEntry& aFolder2Entry);
	TBool  IsMatchHeaderL(CMsvEntry& aFolder1Entry, CMsvEntry& aFolder2Entry);
	};


#endif

