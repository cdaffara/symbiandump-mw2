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
 
#ifndef __T_CREATE_SEARCH_SORT_QUERY_H__
#define __T_CREATE_SEARCH_SORT_QUERY_H__

/* User  includes */
#include "T_MsgSyncStepCommon.h"
#include "T_MsgSharedDataCommon.h"

/* Literals Used */
_LIT(KCreateSearchSortQuery,"CreateSearchSortQuery");

/* 
 * Implements the test step that ctreates a query for search and/or sort operation
 */
class CT_MsgCreateSearchSortQuery : public CT_MsgSyncStepCommon
	{
public:
	CT_MsgCreateSearchSortQuery(CT_MsgSharedDataCommon& aSharedDataCommon);
	~CT_MsgCreateSearchSortQuery();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
private:
	TMsvId ParentIdForSearchSortL(void);
	void ModifyDefaultSearchSortOptions(void);
	};
#endif /* __T_CREATE_SEARCH_SORT_QUERY_H__ */
