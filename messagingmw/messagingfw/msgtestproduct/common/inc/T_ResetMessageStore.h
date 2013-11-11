/**
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgResetMessageStore 
* 
*
*/



/**
 @file 
*/


#ifndef __T_RESET_MESSAGE_STORE_H__
#define __T_RESET_MESSAGE_STORE_H__



/* User  includes */
#include "T_MsgCommonServer.h"
#include "T_MsgSharedDataCommon.h"
#include <test/testexecutestepbase.h>
#include "T_MsgAsyncStep.h"
#include "T_MsgAsyncStepCommon.h"

/* Epoc includes */
#include <msvapi.h>


/* Forward Declaration */
class CT_MsgUtilsDeleteAllChildren;


/* Literals Used */
_LIT(KResetMessageStore,"ResetMessageStore");


/* 
 * Implements the test step that resets the messages store and initialises 
 * it with the contents of the file msgs.rsc
 */
class CT_MsgResetMessageStore : public CT_MsgAsyncStepCommon
	{
public:
	/* Creates an object of CT_MsgResetMessageStore */
	static CT_MsgResetMessageStore* NewL(CT_MsgSharedDataCommon& aSharedDataCommon);

	~CT_MsgResetMessageStore();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
protected:
	void ConstructL();
private:
	CT_MsgResetMessageStore(CT_MsgSharedDataCommon& aSharedDataCommon);

	/* 
	 * Derived  from CT_MsgAsyncStep
	 * Displays the progress of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* 
	 * Derived from MT_MsgActiveCallback 
	 * Cancels an asynchronous operation
	 */
	virtual void CancelStep();

	/* 
	 * Cleans up the central repository, deleting the POP,
	 * IMAP and SMTP accounts
	 */
	
        void ResetStoreEmailAccountsL();

	void ResetStoreL();
private:
	/* 
	 * Pointer to CT_MsgUtilsDeleteAllChildren to delete the entries under
	 * the specified folder 
	 */
	CT_MsgUtilsDeleteAllChildren* iDeleteAllChildren;
	};
#endif /*__T_RESET_MESSAGE_STORE_H__ */
