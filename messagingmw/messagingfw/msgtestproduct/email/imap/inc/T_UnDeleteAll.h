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
* This is the header file for CT_MsgUnDeleteAll
* 
*
*/



#ifndef __T_UNDELETE_ALL_H__
#define __T_UNDELETE_ALL_H__


// User includes
#include "T_MsgAsyncStepIMAP.h"

// Epoc includes
#include <imapcmds.h>
#include <imapset.h>

// Literals used
_LIT(KUnDeleteAll,"UnDeleteAll");

//	Implements a test step that undelete all the deleted
//	message from the remote folder specified
class CT_MsgUnDeleteAll : public CT_MsgAsyncStepIMAP
	{
public:
	static CT_MsgUnDeleteAll* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgUnDeleteAll();
	TVerdict doTestStepL();

private:
	CT_MsgUnDeleteAll(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	
	/**
	This is initialised with final progress values that are expected after
	the operation has completed.
	After the operation has completed the expected and actual values are compared.
	The test fails if they are not equal.
	*/
	TImap4CompoundProgress iExpectedProgress;
	};

#endif //__T_UNDELETE_ALL_H__
