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
* This contains the header file for the Common server which contains
* the test steps common across all the servers
* 
*
*/



/**
 @file 
*/

#ifndef __T_SERVER_BASE_H__
#define __T_SERVER_BASE_H__


/* User defined classes */
#include "T_MsgServer.h"
#include "T_MsgSharedDataCommon.h"


/* Literals Used */
_LIT(KMsgCommonServer,"T_MsgCommonServer");


/* 
 * Implements the Common server which creates the test steps common across
 * all the servers
 */
class CT_MsgCommonServer : public CT_MsgServer
	{
public:
	/* Creates an bject of Common Server */
	static CT_MsgCommonServer* NewL();
protected:
	CT_MsgCommonServer();
	/* 
	 * Derived from CT_MsgServer
	 * Creates the test steps based on name specified in script file
	 */
	virtual CTestStep* CreateTestStepL(const TDesC& aStepName);
	/* 
	 * Derived from CT_MsgServer
	 * Creates the respective MTM object based on the UID specified
	 */
	virtual CT_MsgSharedDataBase* NewSharedDataL();

private:
	/* Pointer to shared data object */
	CT_MsgSharedDataCommon*	iSharedDataCommon;
	};
#endif /*__T_SERVER_BASE_H__ */
