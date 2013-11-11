// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CWATCHERLAUNCHER_H__
#define __CWATCHERLAUNCHER_H__

#include <e32base.h>
#include <f32file.h>

class CWatcherLog;
class CActiveWatcher;

/**
@internalComponent
@released
*/
class CWatcherLauncher : public CActive
	{
public:
	static CWatcherLauncher* NewL(const TDesC& aWatcherName, TUid aWatcherUid, RFs& aFs, CWatcherLog& aLog);
	virtual ~CWatcherLauncher();
private:	// methods from CActive
	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);
private:
	CWatcherLauncher(TUid aWatcherUid, RFs& aFs, CWatcherLog& aLog);
	void ConstructL(const TDesC& aWatcherName);
private:
	TUid			iWatcherUid;
	RFs&			iFs;
	CWatcherLog&	iLog;
	HBufC*			iWatcherName;
	CActive*		iWatcher;
	RTimer			iTimer;
	TInt			iStartCount;
	TUid			iDtor_ID_Key;
	};

#endif	// __CWATCHERLAUNCHERARRAY_H__
