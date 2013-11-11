// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __PUSHTESTS_H__
#define __PUSHTESTS_H__

#include "t_wappush.h"
#include <msvapi.h>
#include <mtclbase.h>
#include "dummywapstack.h"		// MDummyWapStackObserver

#include <push/pushlog.h>			// for test engine Rfs/Cwatcherlog
#include "ConnMan.h"			// for connection manager
#include <caf/manager.h>
#include <caf/rightsmanager.h>
#include "httptestutils.h"

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
 * 	Derived test case class which implementation the stack observer
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
	virtual TInt RunError(TInt aError);
	virtual void DoCancel();
	virtual const TDesC& TestName();
	virtual TDesC8& Uri() const = 0;

	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase) = 0;	// abstract class now!!!
	virtual TBool ConfirmMessagesSavedL() = 0;
	
	// from MDummyWapStackObserver	
	virtual void DWSOUnitWaitPushL(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOCancelUnitWaitPush(CDummyWSPCLConn& aDummyCLConn);
	virtual void DWSOServerShutDown();
	virtual void DWSOError(const TDesC& aDes, TInt aLineNum);
	virtual void DWSOConnect(CDummyWSPCOConn& aDummyCOConn);
	virtual void DWSOGetEventL(CDummyWSPCOConn& aDummyCOConn);
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

	CDummyWapStack* iWapStack;
	CConnectionManager* iConnMan;
	TBool iHasMsgBeenCompleted;
	TBool iHasTestBeenCompleted;

	TInt iNumberOfTestCases;			
	TInt iCurrentTestCase;

	};

/** Wap Message derived test class */
class CWapPushMessageTest : public CWapPushBaseMessageTest
	{
public:
	CWapPushMessageTest(TInt aIndex, CHTTPTestUtils* aTestUtils);
	virtual ~CWapPushMessageTest();
	void ReadAndSetParamsFromIniFileL();
	void RemoveQuotes(HBufC8* aHBuf8Ptr);
	TDesC8& GetHRef() const;
	TDesC8& GetAction() const;
	
	virtual const TDesC& TestName() = 0;
	virtual void NumberTestCasesToRun() = 0;
	virtual void PrepareTestMessageL(TInt aCurrentTestCase) = 0;
	virtual TBool ConfirmMessagesSavedL() = 0;
	virtual TDesC8& Uri() const = 0;

protected:
	TInt iSectionIndex;
	HBufC8* iUri;
	HBufC8* iHRef;
	HBufC8* iAction;	
	};
	
	
/** SI Specific Derived Test Class **/
class CWapPushSIMessageTest : public CWapPushMessageTest
	{
public:
	CWapPushSIMessageTest(TInt aIndex, CHTTPTestUtils* aTestUtils);
	void ReadAndSetSIParamsFromIniFileL();
	~CWapPushSIMessageTest();
	TDesC8& GetSiId() const;
	TDesC8& GetSICreatedDate() const;
	TDesC8& GetSIExpiresDate() const;
	
	const TDesC& TestName();
	void NumberTestCasesToRun();
	void PrepareTestMessageL(TInt aCurrentTestCase);
	TBool ConfirmMessagesSavedL();
	TDesC8& Uri() const;

private:
	HBufC8* iSiId;
	HBufC8* iSiCreatedDate;
	HBufC8* iSiExpiresDate;	
	};	
	

/** SL Specific Derived Test Class **/
class CWapPushSLMessageTest : public CWapPushMessageTest
	{
public:
	CWapPushSLMessageTest(TInt aIndex, CHTTPTestUtils* aTestUtils);
	
	const TDesC& TestName();
	void NumberTestCasesToRun();
	void PrepareTestMessageL(TInt aCurrentTestCase);
	TBool ConfirmMessagesSavedL();
	TDesC8& Uri() const;
	};		


#endif
