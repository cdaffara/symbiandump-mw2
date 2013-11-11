// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// @file
// This is the header file for CT_MsgCancelOfflineOperations
// 
//


#ifndef __T_CANCEL_OFFLINE_OPERATIONS_H__
#define __T_CANCEL_OFFLINE_OPERATIONS_H__


//User  includes
#include "T_MsgAsyncStepIMAP.h"
//#include "t_shareddataimap.h"


// Epoc Include
#include <mtclbase.h>


// Forward class declaration
class CT_MsgImapServer;

// Literals used
_LIT(KCancelOfflineOperations,"CancelOfflineOperations");

// Implements a test step that performs a synchronisation with the Imap server
class CT_MsgCancelOfflineOperations : public	CT_MsgAsyncStepIMAP
	{	
public : 
	static CT_MsgCancelOfflineOperations* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgCancelOfflineOperations();
	TVerdict doTestStepL();
private:

	/*	
	 * Derived from CT_MsgAsyncStep 
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback 
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	
	/* 
	 * A Pointer of type CT_MsgUtilsDeleteAllChildren to delete
	 * all the entries under the specifed folder
	 */
	CMsvOperation*	iOperation;
	
	CT_MsgCancelOfflineOperations(CT_MsgSharedDataImap& aSharedDataIMAP);
	};
#endif //__T_CANCEL_OFFLINE_OPERATIONS_H__
