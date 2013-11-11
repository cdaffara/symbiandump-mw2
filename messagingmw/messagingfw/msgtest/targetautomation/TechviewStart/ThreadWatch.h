// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined (__THREADWATCH_H__)
#define __THREADWATCH_H__

#include <e32base.h>

//Forward declaration
class CStarter;

class CThreadWatcher : public CActive
	{
public:
	/**
	 * Creates an instance of an Thread Watcher.
	 *
	 * @param aThreadId	id of the thread to be monitored
	 * @param aCallBack	Callback to be called on thread exit
	 * @return			a pointer to this object
	 */
	static CThreadWatcher* NewL(TInt appType, const TThreadId aThreadId, TFullName fileName, CStarter* aOwner, TUint32 aAppUid, TBool aViewless);
	
	/**
	 * Destructor.
	 */
	~CThreadWatcher();

private:
	/**
	 * Method called when the thread exit
	 */
	void RunL();
	
	/**
	 * Cancel the current monitoring
	 */
	void DoCancel();

	/**
	 * Restart the thread
	 */
	void RestartThreadL();
	
	/**
	 * Contructor
	 */
	CThreadWatcher(TInt appType, TFullName aFullName, CStarter* aOwner, TUint32 aAppUid, TBool aViewless);

	/**
	 * Handles the actual initialization of this object.
	 * @param aThreadId	id of the thread to be monitored
	 */
	void ConstructL(const TThreadId aThreadId);

private:
	TInt iAppType;
	RThread iThread;		// thread monitored
	TBool iMonitoringThread;	// 
	TFullName iFullName;
	CStarter* iOwner;
	TUint32 iAppUid; 
	TBool iViewless;
	

public:
	TSglQueLink iLink;	// Linked list anchor
	};

#endif
