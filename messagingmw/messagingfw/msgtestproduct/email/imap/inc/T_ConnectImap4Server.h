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
* This is the header file for CT_MsgConnectImap4Server
* 
*
*/




#ifndef __T_CONNECT_IMAP4_SERVER_H__
#define __T_CONNECT_IMAP4_SERVER_H__


/* User include */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Epoc includes */
#include <msvapi.h>


/* Literals Used */
_LIT(KConnectImap4Server,"ConnectImap4Server");


/* Implements a test step to establish connection with the IMAP server */
class CT_MsgConnectImap4Server : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgConnectImap4Server */
	static CT_MsgConnectImap4Server* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgConnectImap4Server();

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
private:
	CT_MsgConnectImap4Server(CT_MsgSharedDataImap& aSharedDataIMAP);

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
private:
	/* An asynchronously completing operation
	 * containing the status of the operation
	 */
	CMsvOperation*	iOperation;
	};
#endif /*__T_CONNECT_IMAP4_SERVER_H__*/
