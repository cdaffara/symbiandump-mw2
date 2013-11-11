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
* This is the header file for CT_UpdateToExternalDrive class
* 
*
*/




#ifndef __T_UPDATE_TO_EXTERNAL_DRIVE_H__
#define __T_UPDATE_TO_EXTERNAL_DRIVE_H__

// User  includes
#include "T_MsgAsyncStepIMAP.h"


// Literals used
_LIT(KUpdateToExternalDrive,"UpdateToExternalDrive");


// Implements a test step to change the external drive
class CT_UpdateToExternalDrive : public CT_MsgAsyncStepIMAP
	{
public:
	static CT_UpdateToExternalDrive* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	virtual ~CT_UpdateToExternalDrive(); 

public:
	TVerdict doTestStepL();
private:
	CT_UpdateToExternalDrive(CT_MsgSharedDataImap& aSharedDataIMAP);

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
	 * A Pointer of type CMsvOperation
	 */
	CMsvOperation*	iOperation;
	};


#endif // __T_UPDATE_TO_EXTERNAL_DRIVE_H__
