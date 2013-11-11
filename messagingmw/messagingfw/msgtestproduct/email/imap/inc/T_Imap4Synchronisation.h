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
* This is the header file for CT_MsgImap4Synchronisation
* 
*
*/




#ifndef __T_IMAP4_SYNCHRONISATION_H__
#define __T_IMAP4_SYNCHRONISATION_H__


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/*  Epoc include */
#include <mtclbase.h>


/* Forward class declaration */
class CT_MsgImapServer;

/* Literals used */
_LIT(KImap4Synchronisation,"Imap4Synchronisation");


/* 
 * Implements a test step to synchronise the current folder that is selected 
 * on the remote IMAP server
 */
class CT_MsgImap4Synchronisation : public	CT_MsgAsyncStepIMAP
	{	
public : 
	/* Creates an object of type CT_MsgImap4Synchronisation */
	static CT_MsgImap4Synchronisation* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgImap4Synchronisation();

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
private:
	CT_MsgImap4Synchronisation(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	 * A Pointer of type CMsvOperation to retrieve the status of the operation
	 */
	CMsvOperation*	iOperation;
	};
#endif /*__T_IMAP4_SYNCHRONISATION_H__*/
