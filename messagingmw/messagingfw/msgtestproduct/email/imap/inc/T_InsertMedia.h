/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_InsertMedia class
* 
*
*/




#ifndef __T_INSERT_MEDIA_H__
#define __T_INSERT_MEDIA_H__

// User  includes
#include "T_MsgAsyncStepIMAP.h"

// Literals used
_LIT(KInsertMedia,"InsertMedia");

// Implements a test step to insert the media
class CT_InsertMedia : public CT_MsgAsyncStepIMAP
	{
public:
	static CT_InsertMedia* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_InsertMedia(); 

	TVerdict doTestStepL();

private:
	CT_InsertMedia(CT_MsgSharedDataImap& aSharedDataIMAP);

	enum TMsvFailure // as defined in msvserv.h
		{
		EHeapFailure,
		EDiskFailure
		};

	/*
	 * Derived from CT_MsgAsyncStep
	 * Displays the progress information of the asynchronous operation
	 */
	virtual void ProgressL(TBool bFinal);

	/* Derived from MT_MsgActiveCallback
	 * Cancels the asynchronous operation
	 */
	virtual void CancelStep();
	};

#endif // __T_INSERT_MEDIA_H__
