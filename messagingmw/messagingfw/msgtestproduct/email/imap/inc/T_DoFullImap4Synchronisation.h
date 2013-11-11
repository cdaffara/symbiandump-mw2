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
* This is the header file for CT_MsgDoFullImap4Synchronisation
* 
*
*/




#ifndef __T_DO_FULL_IMAP4_SYNCHRONISATION_H__
#define __T_DO_FULL_IMAP4_SYNCHRONISATION_H__


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/*  Epoc include */
#include <mtclbase.h>


/* Forward class declaration */
class CT_MsgImapServer;

/* Literals used */
_LIT(KDoFullImap4Synchronisation,"DoFullImap4Synchronisation");


/* Implements a test step to perform synchronisation with the Imap server */
class CT_MsgDoFullImap4Synchronisation : public	CT_MsgAsyncStepIMAP
	{	
public : 
	/* Creates an object of type CT_MsgDoFullImap4Synchronisation */
	static CT_MsgDoFullImap4Synchronisation* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgDoFullImap4Synchronisation();

	/*	CTestStep implementation */
	virtual TVerdict	doTestStepL();
private:
	CT_MsgDoFullImap4Synchronisation(CT_MsgSharedDataImap& aSharedDataIMAP);

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
#endif /*__T_DO_FULL_IMAP4_SYNCHRONISATION_H__*/
