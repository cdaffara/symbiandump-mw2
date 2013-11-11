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
* This is the header file for CT_MsgVerifyImapAccountsCount
* 
*
*/




#ifndef __T_VERIFY_IMAP_ACCOUNTS_COUNT_H__
#define __T_VERIFY_IMAP_ACCOUNTS_COUNT_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"


/* Literals Used */
_LIT(KVerifyImapAccountsCount,"VerifyImapAccountsCount");


/* Implements a test step to verify the number of IMAP accounts created */
class CT_MsgVerifyImapAccountsCount: public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgVerifyImapAccountsCount(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgVerifyImapAccountsCount();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};
#endif /*__T_VERIFY_IMAP_ACCOUNTS_COUNT_H__*/
