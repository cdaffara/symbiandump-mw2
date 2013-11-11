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
* This is the header file for CT_MsgMove class
* 
*
*/



#ifndef __T_MOVE_MESSAGE_H__
#define __T_MOVE_MESSAGE_H__

// User  includes
#include "T_MsgAsyncStepIMAP.h"

// Literals used
_LIT(KMoveMessage,"MoveMessage");


// Implements a test step to move the message
class CT_MsgMove : public CT_MsgAsyncStepIMAP
	{
public:
	static CT_MsgMove* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgMove();
	TVerdict doTestStepL();

private:

	CT_MsgMove(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	};

#endif //__T_MOVE_MESSAGE_H__

