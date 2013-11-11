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
* This is the header file for CT_MsgCreateImapAccount
* 
*
*/




#ifndef __T_CREATE_IMAP_ACCOUNT_H__
#define __T_CREATE_IMAP_ACCOUNT_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"


/* Literals used */
_LIT(KCreateImapAccount,"CreateImapAccount");


/* Implements a test step to  create an IMAP account
 * reading the settings from the file specified
 */
class CT_MsgCreateImapAccount : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgCreateImapAccount(CT_MsgSharedDataImap& aSharedDataIMAP);

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
	};
#endif /*__T_CREATE_IMAP_ACCOUNT_H__ */
