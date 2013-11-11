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
 
#ifndef __T_MODIFY_INDEX_ENTRY_FLAGS_H__
#define __T_MODIFY_INDEX_ENTRY_FLAGS_H__

/* User includes */
#include "T_MsgSharedDataCommon.h"
#include "T_MsgSyncStepCommon.h"

/* Literals Used */
_LIT(KModifyIndexEntryFlags,"ModifyIndexEntryFlags");


/* 
 * Implements the test step to modify the flags(TMsvEntry) of selected messages.
 */
class CT_MsgModifyIndexEntryFlags : public CT_MsgSyncStepCommon
	{
public:
	CT_MsgModifyIndexEntryFlags(CT_MsgSharedDataCommon& aSharedDataCommon);

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
	};
#endif /*__T_MODIFY_INDEX_ENTRY_FLAGS_H__*/
