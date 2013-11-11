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
* This is the header file for CT_MsgInitialiseImap4Account class
* 
*
*/



/**
 @file 
*/


#ifndef T_INITIALISE_IMAP4_ACCOUNT_H
#define T_INITIALISE_IMAP4_ACCOUNT_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal Used */
_LIT(KInitialiseImap4Account,"InitialiseImap4Account");


/* Implements the test step which initialises the remote IMAP4 account */
class CT_MsgInitialiseImap4Account : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of CT_MsgInitialiseImap4Account */
	static CT_MsgInitialiseImap4Account* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgInitialiseImap4Account();
	
	/* CTestStep implementation*/
	virtual TVerdict doTestStepL();
private:
	CT_MsgInitialiseImap4Account(CT_MsgSharedDataImap& aSharedDataIMAP);
	void DeleteChildrenL(TMsvId aFolderId, CMsvSession& aSession);
	TInt GetFolderCountL(TMsvId aFolderId, CMsvSession& aSession);
	void DeleteFolderL(TMsvId aFolderId);
	void CreateDefaultFoldersL(TMsvId aFolderId);
	void CreateEntryL(TMsvId aFolderId, const TDesC& aParamDetails);
	void PrintFolderTreeL(TMsvId aFolderId, CMsvSession& aSession);
	void UnsubscribeChildrenL(TMsvId aFolderId, CMsvSession& aSession);
	void UnsubscribeFolderL(TMsvId aFolderId);
	void SynchroniseAccountL();
	TBool ReadIni();
	
	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* 
	 * Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	
private:
	/* 
	 * An asynchronously completing operation containing the status of the operation
	 */
	CMsvOperation*	iOperation;

	TPtrC  iImapAccountName;
	TMsvId iImapServiceId;
	TInt   iFolderCounterForDelete;
	TInt   iFolderCounterForUnsubscribe;
	};

#endif /* T_INITIALISE_IMAP4_ACCOUNT_H */
