/**
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
*
*/



/**
 @file
*/


#ifndef __T_CREATE_SMTP_MESSAGE_FROM_EMAIL_FILE_H__
#define __T_CREATE_SMTP_MESSAGE_FROM_EMAIL_FILE_H__


// User Includes 
#include "T_MsgSyncStepSMTP.h"
#include "T_MsgSharedDataSmtp.h"


// Literals Used  
_LIT(KCreateSmtpMessageFromEmailFile,"CreateSmtpMessageFromEmailFile");


/**
This class implements the test step that creates the SMTP message
*/
class CT_MsgCreateSmtpMessageFromEmailFile : public CT_MsgSyncStepSMTP
	{
public:	
	static CT_MsgCreateSmtpMessageFromEmailFile* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_MsgCreateSmtpMessageFromEmailFile();

	//	CTestStep implementation
	TVerdict doTestStepL(); 	
private:
	CT_MsgCreateSmtpMessageFromEmailFile(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	};
#endif //__T_CREATE_SMTP_MESSAGE_FROM_EMAIL_FILE_H__
