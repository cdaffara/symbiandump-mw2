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

#ifndef __T_POP_SERVER_H__
#define __T_POP_SERVER_H__

// User defined classes
#include "T_MsgServer.h"
#include "T_MsgSharedDataPop.h"


//Literals Used
_LIT(KPopServer,"T_MsgPopServer");


/**
POP test server which creates and executes the teststeps.
*/
class CT_MsgPopServer : public CT_MsgServer
	{
public :
	static CT_MsgPopServer* NewL();

private:
	CT_MsgPopServer();

	virtual CTestStep*				CreateTestStepL(const TDesC& aStepName);

	// Creates an object of Shared Data
	virtual CT_MsgSharedDataBase* NewSharedDataL();

public:
	CT_MsgSharedDataPop* iSharedDataPOP;
	};
#endif //__T_POP_SERVER_H__
