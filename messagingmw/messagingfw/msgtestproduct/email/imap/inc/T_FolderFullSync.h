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
* This is the header file for CT_MsgFolderFullSync
* 
*
*/


 

#ifndef T_FOLDER_FULL_SYNC_H
#define T_FOLDER_FULL_SYNC_H


/* User includes */
#include "T_MsgAsyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literal used */
_LIT(KFolderFullSync,"FolderFullSync");


/* Implements a test step that selects and synchronises the folder specified */
class CT_MsgFolderFullSync : public CT_MsgAsyncStepIMAP
	{	
public:
	/* Creates an object of CT_MsgFolderFullSync */
	static CT_MsgFolderFullSync* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgFolderFullSync();
	
	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	
private:
	CT_MsgFolderFullSync(CT_MsgSharedDataImap& aSharedDataIMAP);
	TBool ReadIni();
	
	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool aFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
private:
	/* An asynchronously completing operation
	 * containing the status of the operation
	 */
	CMsvOperation*	iOperation;

	TPtrC iImapAccountName;
	TPtrC iParentName;
	TPtrC iFolderName;
	TInt iExpectedErr;
	};
#endif /* T_FOLDER_FULL_SYNC_H */
