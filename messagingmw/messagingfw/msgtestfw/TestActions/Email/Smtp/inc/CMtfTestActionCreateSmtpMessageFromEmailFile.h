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


#ifndef __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_FROM_EMAIL_FILE_H__
#define __CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_FROM_EMAIL_FILE_H__


//User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionCreateSmtpMessageFromEmailFile,"CreateSmtpMessageFromEmailFile");
/*@}*/



/**
  Overview: 
  CMtfTestActionCreateSmtpMessageFromEmailFile class derived from CMtfSynchronousTestAction
  Implements the action to parse a Email file, create a email message from the contents of 
  the file in the specified folder.  This Test Action creates simple emails, emails with HTML, 
  attachments, multipart and embedded emails.
  @internalTechnology
*/
class CMtfTestActionCreateSmtpMessageFromEmailFile : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCreateSmtpMessageFromEmailFile(); 

public:
	// Override the base class function.
	virtual void ExecuteActionL();

private:
	CMtfTestActionCreateSmtpMessageFromEmailFile(CMtfTestCase& aTestCase);
	};


#endif //__CMTF_TEST_ACTION_CREATE_SMTP_MESSAGE_FROM_EMAIL_FILE_H__
