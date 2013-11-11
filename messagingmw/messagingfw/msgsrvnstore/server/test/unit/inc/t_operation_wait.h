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

#ifndef __T_OPERATION_WAIT_H
#define __T_OPERATION_WAIT_H

#include <e32base.h>
#include <msvapi.h>

class MOperationWaitTestObserver;

class CWaitTest : public CActive
	{
public:
	virtual ~CWaitTest();
	TBool Completed() const;
	TInt Delay() const;
	void Start();
	void SetOperationWaitTestObserver(MOperationWaitTestObserver& aOperationWaitTestObserver);

protected:
	CWaitTest(TInt aDelay);
	void ConstructL();

private:
	virtual void RunL();
	virtual void DoCancel();
	void CompleteRequest(TRequestStatus& aRequestStatus, TInt aErr = KErrNone);
	virtual void Wait() = 0;							//A standard API for waiting using different waiters
	virtual TRequestStatus& WaiterRequestStatus() = 0;	//A standard API for accessing different waiter's TRequestStatuses

private:
	RTimer iTimer;									//The timer object used to delay starting and stopping
	TBool iCompleted;								//A flag set to true once this object has completed
	TInt iDelay;									//The delay (in microseconds) before the timer completes
	MOperationWaitTestObserver* iOverseer;			//Callback to signal the overseer of this object's completion
	};


class COperationWaitTest : public CWaitTest
	{
public:
	static COperationWaitTest* NewL(TInt aDelay);
	virtual ~COperationWaitTest();

private:
	void ConstructL();
	COperationWaitTest(TInt aDelay);
	virtual void Wait();
	virtual TRequestStatus& WaiterRequestStatus();

private:
	CMsvOperationWait* iOperationWait;
	};


class COperationActiveSchedulerWaitTest : public CWaitTest
	{
public:
	static COperationActiveSchedulerWaitTest* NewL(TInt aDelay);
	virtual ~COperationActiveSchedulerWaitTest();

private:
	void ConstructL();
	COperationActiveSchedulerWaitTest(TInt aDelay);
	virtual void Wait();
	virtual TRequestStatus& WaiterRequestStatus();

private:
	CMsvOperationActiveSchedulerWait* iOperationActiveSchedulerWait;
	};

class MOperationWaitTestObserver
	{
public:
	virtual void WaitComplete() = 0;
	};


class COperationWaitTestOverseer : public CBase, MOperationWaitTestObserver
	{
public:
	COperationWaitTestOverseer(CWaitTest* aWaitTestA, CWaitTest* aWaitTestB);
	virtual ~COperationWaitTestOverseer();
	void Start();

public:	//from MOperationWaitTestObserver
	virtual void WaitComplete();

private:
	CWaitTest* iWaitTestA;
	CWaitTest* iWaitTestB;
	CActiveSchedulerWait iActiveSchedulerWait;	//Avoid the direct use of CActiveScheduler::Start()/Stop()
	};

#endif //__T_OPERATION_WAIT_H
