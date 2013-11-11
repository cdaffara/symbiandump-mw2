// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Description:
//

#ifndef __TESTWATCHER_H__
#define __TESTWATCHER_H__

#include <e32base.h>

//**********************************
// CTestWatcher
//**********************************

class RFs;

class CTestWatcher : public CTimer
	{
public:
	static CTestWatcher* NewL(TAny* aWatcherParams);
	~CTestWatcher();
	//
private:
	CTestWatcher(TInt aPriority, RFs& aFs, CWatcherLog& aLog);
	void ConstructL();
	void RunL();
	//
private:
	CWatcherLog& iLog;
	RFs& iFs;
	};
	
#endif	// __TESTWATCHER_H__
