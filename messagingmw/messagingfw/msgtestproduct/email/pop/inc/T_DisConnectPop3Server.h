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
*
*/



/**
 @file
*/

#ifndef __T_DIS_CONNECT_POP3_SERVER_H__
#define __T_DIS_CONNECT_POP3_SERVER_H__

// User includes
#include"T_MsgAsyncStepPOP.h"
#include "T_MsgSharedDataPop.h"


// Literals Used
_LIT(KDisConnectPop3Server,"DisConnectPop3Server");

/**
Implements a test step to disconnect from the POP server
*/
class CT_MsgDisConnectPop3Server : public CT_MsgAsyncStepPOP
	{
public:
	static CT_MsgDisConnectPop3Server* NewL(CT_MsgSharedDataPop& aSharedDataPOP);
	~CT_MsgDisConnectPop3Server();

	//	CTestStep implementation
	virtual TVerdict	doTestStepL();

private:
	CT_MsgDisConnectPop3Server(CT_MsgSharedDataPop& aSharedDataPOP);

	//	CT_MsgAsyncStep implementation
	virtual void	ProgressL(TBool bFinal);

	//	MT_MsgActiveCallback implementation
	virtual void	CancelStep();

private:
	CMsvOperation*	iOperation;
	};

#endif //__T_DIS_CONNECT_POP3_SERVER_H__

