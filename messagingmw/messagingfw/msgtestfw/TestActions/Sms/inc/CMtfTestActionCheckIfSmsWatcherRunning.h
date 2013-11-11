/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMTF_TEST_ACTION_CHECK_IF_SMS_WATCHER_RUNNING_H__
#define __CMTF_TEST_ACTION_CHECK_IF_SMS_WATCHER_RUNNING_H__


#include "CMtfTestAction.h"


_LIT(KTestActionCheckIfSmsWatcherRunning,"CheckIfSmsWatcherRunning");

class CMtfTestActionUtilsTimer;

class CMtfTestActionCheckIfSmsWatcherRunning : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCheckIfSmsWatcherRunning(); 

public:
	virtual void ExecuteActionL();
	virtual void DoCancel();
	virtual void RunL();

private:
	CMtfTestActionCheckIfSmsWatcherRunning(CMtfTestCase& aTestCase);
	
private:
	TInt iCounter;
	CMtfTestActionUtilsTimer* iTimer;
	};

#endif
