// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Description: The header file for CT_MsgVerifyHeaders
//

// @file

#ifndef __T_VERIFY_HEADERS_H__
#define __T_VERIFY_HEADERS_H__

// User includes
#include"T_MsgSyncStepIMAP.h"

// Literals Used
_LIT(KVerifyHeaders,"VerifyHeaders");

/**
Implements the test step which verify the message header parts
*/
class CT_MsgVerifyHeaders : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgVerifyHeaders(CT_MsgSharedDataImap& aSharedDataIMAP);
	// CTestStep implementation
	TVerdict doTestStepL();
private:
	TBool VerifyImHeaderPartsL(CMsvEntrySelection* aEntrySelection);
	TBool VerifyMimeHeaderPartsL(CMsvEntrySelection* aEntrySelection);
	};
#endif //__T_VERIFY_HEADERS_H__
