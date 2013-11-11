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
* This contains the header file for the Msg server from which other servers 
* would be derived
* 
*
*/



#ifndef __T_MSG_SERVER_H__
#define __T_MSG_SERVER_H__


/* User defined classes */
#include <test/testexecuteserverbase.h>
#include "T_MsgSharedDataBase.h"

#include "mmsgtestpropertywatcher.h"
#include "cmsgtestpropertywatcher.h"

/* 
 * Implements a base server from which other servers would be
 * derived
 */
class CT_MsgServer : public CTestServer, public MMsgTestPropertyWatcher
	{
protected:
	CT_MsgServer();
	virtual ~CT_MsgServer();

	/*
	 * Derived from CTestStep
	 * This wraps CTestServer::CreateTestStep() with a leave-trap 
	 */
	virtual CTestStep*	CreateTestStepL(const TDesC& aStepName);

	/*
	 * To be implemented by the derived classes
	 * Creates the required shared data object
     */
	virtual CT_MsgSharedDataBase*	NewSharedDataL() = 0;
	
	//From MMsgTestPropertyWatcher
	void HandleEventL(RProperty& aProperty, TUint /*aKey*/);
	
private:
	/* Calls the function CreateTestStepL to create the required test step */
	CTestStep*	CreateTestStep(const TDesC& aStepName);

	/* Creates the active scheduler and the base shared data object */
	void StartupServerL();

	/* Destroys the shared data object and the active scheduler object */
	void ShutdownServer();

protected:
	/* For use by generic Mtm test steps */
	CT_MsgSharedDataBase*	iSharedDataBase; 

private:
	/* Active scheduler object */
	CActiveScheduler*		iActiveScheduler;
	
	//TBD: Remove this...
	CMsgTestPropertyWatcher* iPropertyWatcher;
	};

#endif /*__T_MSG_SERVER_H__ */
