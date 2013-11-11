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




#ifndef __T_CONNECT_AND_SYNC_IMAP4_SERVER_H__
#define __T_CONNECT_AND_SYNC_IMAP4_SERVER_H__


// Epoc includes
#include "T_MsgAsyncStepIMAP.h"


//Forrwad class declaration 
class CT_MsgImapServer;


// Literals Used 
_LIT(KConnectAndSyncImapServer,"ConnectAndSyncImapServer");


/* 
 * Implements a test step to establish connection with the IMAP server
 * and start a background synchronisation
 */
class CT_MsgConnectAndSyncImap4Server : public CT_MsgAsyncStepIMAP
	{
public:
	//Creates an object of type CT_MsgConnectAndSyncImap4Server 
	static CT_MsgConnectAndSyncImap4Server* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgConnectAndSyncImap4Server();

	//CTestStep implementation 
	virtual TVerdict doTestStepL();		
private:
	CT_MsgConnectAndSyncImap4Server(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	CMsvOperation* iOperation;
	};
#endif //__T_CONNECT_AND_SYNC_IMAP4_SERVER_H__ 
