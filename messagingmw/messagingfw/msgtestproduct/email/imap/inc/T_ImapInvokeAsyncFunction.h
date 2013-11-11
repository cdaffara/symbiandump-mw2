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
* This is the header file for CT_MsgInvokeAsyncFunction
* 
*
*/





#ifndef __T_IMAP_INVOKE_ASYNC_FUNCTION_H__
#define __T_IMAP_INVOKE_ASYNC_FUNCTION_H__


/* User includes */
#include "T_MsgAsyncStepIMAP.h"


/* Epoc includes */
#include <imapcmds.h>
#include <msvstd.h>
#include <mtclbase.h>



/* Literals used */
_LIT(KImapInvokeAsyncFunction,"ImapInvokeAsyncFunction");



/* 
 * Implements a test step that populates the messages from the remote Imap folder
 * based on the IMAP command spicifies and also compares the messages in the Sent Items
 * folder and the remote folder specified
 */
class CT_MsgImapInvokeAsyncFunction : public CT_MsgAsyncStepIMAP
	{
public:
	/* Creates an object of type CT_MsgImapInvokeAsyncFunction */
	static CT_MsgImapInvokeAsyncFunction* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgImapInvokeAsyncFunction();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();

	/* Populates the messages based on the command given and the download limts set*/
	void InvokeAsyncImap4PopulateCommandL( TImap4Cmds aCommand, CMsvEntrySelection& aMailSelection, 
											TMsvId aImapServiceId , TBool aDownloadLimits, TMsvId aFolderId );
	
	/* Checks if the command requires a selection object */
	TBool IsWithSelection(TImap4Cmds aImapCmd);

	/* Used when the IMAP command requires a selection object to be passed */
	void ImapInvokeAsyncFunctionWithSelectionL(TImap4Cmds aCommand,TMsvId aImapServiceId,
								CMsvEntrySelection& aMailSelection, TBool aDownloadLimits,TMsvId aFolderId);
								
	/* Used when the IMAP command does not require a selection object to be passed */									
	void ImapInvokeAsyncFunctionWithoutSelectionL(TImap4Cmds aCommand,TMsvId aImapServiceId,
									TMsvId aFolderId, TBool aDownloadLimits);
private:
	CT_MsgImapInvokeAsyncFunction(CT_MsgSharedDataImap& aSharedDataIMAP);

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

	/* 
	 * Selection object which will contain the entries to be downloaded
	 * or compared 
	 */
	CMsvEntrySelection*	iMailSelection;

	/* 
	 * Selection object which will contain the entries 
	 * in the specified remote folder
	 */
	CMsvEntrySelection*	iRecvMsgSelectionList;
	};
#endif /*__T_IMAP_INVOKE_ASYNC_FUNCTION_H__ */
