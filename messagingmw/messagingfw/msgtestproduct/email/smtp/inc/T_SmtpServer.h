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
* This contains the header file for SMTP server
* 
*
*/



/**
 @file
*/


#ifndef __T_SMTP_SERVER_H__
#define __T_SMTP_SERVER_H__


// User defined classes
#include "T_MsgServer.h"
#include "T_MsgSharedDataSmtp.h"


//Literals Used
_LIT(KSmtpServer,"T_MsgSmtpServer");


/**
Implements the SMTP test server
*/
class CT_MsgSmtpServer : public CT_MsgServer
	{
public:
	static CT_MsgSmtpServer* NewL();

private:
	CT_MsgSmtpServer();
	// Creates the test steps based on the test step name 
	virtual CTestStep*	CreateTestStepL(const TDesC& aStepName);

	// Creates an object of Shared Data
	virtual CT_MsgSharedDataBase* NewSharedDataL();

public:
	// Creates session object and SMTP MTM					
	CT_MsgSharedDataSmtp* iSharedDataSMTP;
	};
#endif //__T_SMTP_SERVER_H__
