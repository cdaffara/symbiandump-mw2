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
* This is the header file for CT_MsgDeleteImapAccount
* 
*
*/




#ifndef __T_DELETE_IMAP_ACCOUNT_H__
#define __T_DELETE_IMAP_ACCOUNT_H__


/* User includes */
#include "T_MsgSyncStepIMAP.h"


/* Literals Used */
_LIT(KDeleteImapAccount,"DeleteImapAccount");


/* 
 * Implememts a test step to delete the IMAP account specified or 
 * delete all the IMAP accounts based on the option specified in ini file
 */
class CT_MsgDeleteImapAccount : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgDeleteImapAccount(CT_MsgSharedDataImap& aSharedDataIMAP);
	
	~CT_MsgDeleteImapAccount();
	
	/* CTestStep implementation */
	TVerdict doTestStepL();		
	};
#endif /*__T_DELETE_IMAP_ACCOUNT_H__ */
