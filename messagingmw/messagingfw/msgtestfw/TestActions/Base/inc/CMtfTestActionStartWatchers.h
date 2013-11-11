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

#ifndef __CMTF_TEST_ACTION_START_WATCHERS_H__
#define __CMTF_TEST_ACTION_START_WATCHERS_H__

#include "CMtfTestAction.h"

_LIT(KTestActionStartWatchers,"StartWatchers");

class CMtfTestActionUtilsTimer;

class CMtfTestActionStartWatchers : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionStartWatchers(); 

public:
	virtual void ExecuteActionL();
	virtual void DoCancel();
	virtual void RunL();

private:
	CMtfTestActionStartWatchers(CMtfTestCase& aTestCase);

private:
	CMtfTestActionUtilsTimer* iTimer;
	TInt iCounter;
	
	RProcess iWatcherProcess;
	TBool 	 iWatcherStarted;	//watcher started by this action or not
	};

#endif
