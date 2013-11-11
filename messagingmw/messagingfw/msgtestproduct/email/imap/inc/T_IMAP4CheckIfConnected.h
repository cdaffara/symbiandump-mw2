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
* This is the header file for CT_MsgCheckIfConnected
* 
*
*/



#ifndef __T_IMAP4_CHECK_IF_CONNECTED_H__
#define  __T_IMAP4_CHECK_IF_CONNECTED_H__


// User include
#include "T_MsgAsyncStepIMAP.h"


// Forward Declaration
class CT_MsgImapServer;

// Literals Used
_LIT(KCheckIfConnected,"CheckIfConnected");



// This implements a test step that checks a connection to the IMAP server
class CT_MsgCheckIfConnected :public CT_MsgAsyncStepIMAP
	{
public:
	static CT_MsgCheckIfConnected* NewL(CT_MsgSharedDataImap& aSharedDataIMAP);
	~CT_MsgCheckIfConnected();
	TVerdict doTestStepL();
	
private:
	CT_MsgCheckIfConnected(CT_MsgSharedDataImap& aSharedDataIMAP);
	void ProgressL(TBool bFinal);
    void CancelStep();

	};






#endif //   __T_IMAP4_CHECK_IF_CONNECTED_H__