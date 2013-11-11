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
// This file contains the declaration of the CWapPushBaseMessageTest which implement the stack observer
// to allow communication with the stack
// 
//

/**
 @file PUSHTESTS.H
*/

#ifndef __PUSHTESTS_H__
#define __PUSHTESTS_H__

#include "t_wappush.h"
#include "dummywapstack.h"		// MDummyWapStackObserver

#include <push/pushlog.h>			// for test engine Rfs/Cwatcherlog
#include "ConnMan.h"			// for connection manager

#include "testscripts.h"

//const TInt KBufferSize = 256;		// imitates KReceiveBufferSize from CLWatcher
//const TInt KBufferSize = 1024;
const TInt KBufferSize = 2048;

class MWapPushLog;
class CWapPushMsgGen;


class MTimeoutCallback
	{
public:
	virtual void Timeout() = 0;
	};

class CTestTimer : public CTimer
	{
public:
	static CTestTimer* NewL(MTimeoutCallback* callback)
		{
		CTestTimer* self = new(ELeave) CTestTimer(callback);
		CleanupStack::PushL(self);
		self->ConstructL(); // CTimer
		CActiveScheduler::Add(self);
		CleanupStack::Pop();
		return self;
		}

protected:
	virtual void RunL() 
		{ 
		iCallback->Timeout(); 
		}
private:
	CTestTimer(MTimeoutCallback* callback): CTimer(EPriorityLow), iCallback(callback)
	{};
	
	MTimeoutCallback* iCallback;
	};


/**
 * 	Derived test case class which implement the stack observer
 * 	to allow communication with the stack
 *
 *	This class has been made abstract to allow different derived classes
 *	to implement creating multiple test messages via PrepareTestMessageL()
 *	and the support of NumberTestCasesToRun()
 *	BOTH must be implimented in any derived class.
 *	ConfirmMessagesSavedL provides confirmation of message storage and 
 *  reports on number of messages found in store.
 */
class CWapPushBaseMessageTest : public CWapPushTest, public MDummyWapStackObserver
	{
public:
	virtual ~CWapPushBaseMessageTest();
	virtual void RunL();
	virtual void RunError();
	virtual void DoCancel();

	/** set the test name for this test case
	@param void
	@return string
		returns test name
	*/
	virtual const TDesC& TestName();

	virtual void NumberTestCasesToRun() = 0;
	virtual void PrepareTestMessageL(TInt aCurrentTestCase) = 0;	// abstract class now!!!
	virtual TBool ConfirmMessagesSavedL() = 0;

	// from MDummyWapStackObserver	
	virtual void DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOCancelUnitWaitPush(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOServerShutDown();
	virtual void DWSOError(const TDesC& aDes, TInt aLineNum);
	virtual void DWSOConnect(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOGetEvent(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOCancelGetEvent(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOAckTransaction(CDummyWSPCOTrans& aDummyCOTrans);
protected:
	void MoveToNextState();
	void CompleteOwnRequest();

	void InitialiseMsgService();
	
protected:
	enum TState
		{
		EInitialisation,
		EWaitForPush,
		EFinished
		};
protected:
	TState						iState;
	
	TBuf8<KBufferSize>			iBodyBuf;
	TBuf8<KBufferSize>			iHeadersBuf;
	HBufC8*						iBody;
	HBufC8*						iHeaders;
	CScriptFile*				iIniFile;

	CDummyWapStack* iWapStack;
	CConnectionManager* iConnMan;
	TBool iHasMsgBeenCompleted;
	TBool iHasTestBeenCompleted;

	TInt iNumberOfTestCases;			
	TInt iCurrentTestCase;

	};


/** 
 *	Script Handler derived test case 
 */
class CWapScriptTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
		
	virtual void NumberTestCasesToRun();

	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	virtual TBool ConfirmMessagesSavedL();
	~CWapScriptTest();
	void SetupAddressL();
	};












#endif
