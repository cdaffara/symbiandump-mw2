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
* This contains the header file for IMAP server
* 
*
*/




#ifndef __T_IMAP_SERVER_H__
#define __T_IMAP_SERVER_H__

/* User defined classes */
#include "T_MsgServer.h"
#include "T_MsgSharedDataImap.h"


/* Literals Used */
_LIT(KImapServer,"T_MsgImapServer");


/* Implements the IMAP server which creates the IMAP test steps */
class CT_MsgImapServer : public CT_MsgServer
	{
public:
	/* Creates an object of type CT_MsgImapServer */
	static CT_MsgImapServer* NewL();

protected:
	CT_MsgImapServer();

	/*
	 * Creates the test steps based on the test step name 
	 * read from the script file
	 */
	virtual CTestStep*	CreateTestStepL(const TDesC& aStepName);

	/* Creates an object of Shared Data specifying the IMAP MTM UID */
	virtual CT_MsgSharedDataBase*	NewSharedDataL();
private:
    /* Creates an object of CT_MsgSharedDataImap object */
	CT_MsgSharedDataImap*	iSharedDataIMAP;
	};
#endif /*__T_IMAP_SERVER_H__ */
