/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @file 
* This is the header file for CT_MsgFindEntryByName
* 
*
*/





#ifndef __T_FIND_ENTRY_BY_NAME_H__
#define __T_FIND_ENTRY_BY_NAME_H__


/* User includes */
#include "T_MsgSyncStepIMAP.h"


/* Literals Used */
_LIT(KFindEntryByName,"FindEntryByName");


/* 
 * Implements a test step that checks if a specified entry 
 * is present under the specified folder 
 */
class CT_MsgFindEntryByName  : public CT_MsgSyncStepIMAP
	{
public: 
	CT_MsgFindEntryByName(CT_MsgSharedDataImap& aSharedDataIMAP);
	
	~CT_MsgFindEntryByName();

	/* CTestStep implementation*/
	virtual TVerdict doTestStepL();
	};
#endif /* __T_FIND_ENTRY_BY_NAME_H__*/
