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
* This is the header file for CT_MsgDisConnectImap4Server
* 
*
*/




#ifndef __T_DIS_CONNECT_IMAP4_SERVER_H__
#define __T_DIS_CONNECT_IMAP4_SERVER_H__


/* Epoc includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literals Used */
_LIT(KDisConnectImapServer,"DisConnectImapServer");


/* Implements a test step to disconnect from the IMAP server */
class CT_MsgDisConnectImap4Server : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of type CT_MsgDisConnectImap4Server */
	static CT_MsgDisConnectImap4Server* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgDisConnectImap4Server();

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
private:
	CT_MsgDisConnectImap4Server(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	/* 
	 * A Pointer of type CT_MsgUtilsDeleteAllChildren to delete
	 * all the entries under the specifed folder
	 */
	CMsvOperation*	iOperation;
	};
#endif /*__T_DIS_CONNECT_IMAP4_SERVER_H__*/
