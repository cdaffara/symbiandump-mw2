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

#ifndef __IRWATCHER_H__
#define __IRWATCHER_H__

#include <e32base.h>


//**********************************
// CIrWatcher
//**********************************

class CWatcherLog;

class CIrWatcher : public CActive
	{
public:
	static CIrWatcher* NewL(TAny* aWatcherParams);
	~CIrWatcher();
	//
private:
	CIrWatcher(TInt aPriority, CWatcherLog& aLog);
	void ConstructL();
	//
	void RunL();
	void DoCancel();
	//
	void DisplayState(TInt aState);
	//
private:
	CWatcherLog& iLog;
	RProperty iProperty;
	};

#endif
