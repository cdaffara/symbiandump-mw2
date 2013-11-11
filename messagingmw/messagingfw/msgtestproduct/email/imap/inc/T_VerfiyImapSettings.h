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
* This is the header file for CT_MsgVerifyImapSettings
* 
*
*/




#ifndef __T_VERIFY_IMAP_SETTINGS_H__
#define __T_VERIFY_IMAP_SETTINGS_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"


/* Literals used */
_LIT(KVerifyImapSettings,"VerifyImapSettings");


/* 
 * Implements a test step that verifies the settings of the IMAP account
 * created with the default settings
 */
class CT_MsgVerifyImapSettings : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgVerifyImapSettings(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgVerifyImapSettings();
	
	/* CTestStep implementation */
	TVerdict doTestStepL();
	};
#endif /*__T_VERIFY_IMAP_SETTINGS_H__*/
