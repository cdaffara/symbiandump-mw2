// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// cwatcher.h
//

/**
 * @file 
 * @internalTechnology
 * @released
 */
#ifndef __CWATCHER_H__
#define __CWATCHER_H__

/** 
ECom interface UID for Active Watcher. 
@internalTechnology
@deprecated
*/
const TUid KUidEComWatcher = { 0x10006554 };

//**********************************
// Constants
//**********************************
/** 
ECom interface watcher delay.
@internalTechnology
@deprecated
*/
const TInt KWatcherDelay = 10000000; // 10 seconds


//**********************************
// CWatcher
//**********************************

class CWatcher : public CActive
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C static CWatcher* NewL(TInt aPriority);
	IMPORT_C ~CWatcher();
	//
	IMPORT_C void Start(TRequestStatus& aStatus);
	//
private:
	CWatcher(TInt aPriority);
	void DoCancel();
	void RunL();
	//
private:
	TRequestStatus* iObserver;
	RThread iThread;
	TInt iLaunchCount;

	};

//**********************************
// CWatcherWait
//**********************************

class CWatcherWait: public CActive
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C static CWatcherWait* NewLC(TInt aPriority=EPriorityStandard);
	IMPORT_C ~CWatcherWait();
	IMPORT_C void Start();
	//
protected:
	CWatcherWait(TInt aPriority);
	void RunL();
	void DoCancel();
	};

#endif // __CWATCHER_H__
