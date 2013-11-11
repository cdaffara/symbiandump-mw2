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

#ifndef __T_SCHEDULESEND_H__
#define __T_SCHEDULESEND_H__

#include <e32base.h>
#include "msvtestutils.h"


class RTest;

class CScheduleSendTestUtils : public CMsvTestUtils
	{
public:

	static CScheduleSendTestUtils* NewL(RTest& aTest, TUint aCreationFlags = ETuMinimalClientSide);
	virtual ~CScheduleSendTestUtils();
	
	void DeletePigeonMessagesL(TMsvId aFolder);
	TMsvId ServiceId();
	
	void NotifySaPhoneOnL();
	void NotifySaPhoneOffL();
	
	void NotifySaSimOkL();
	void NotifySaSimNotPresentL();
	
	void NotifySaNetworkAvailableL();
	void NotifySaNetworkUnAvailableL();
	
	void UpdateSysAgentConditionsL();
	void UpdatePendingConditionsTimeoutL(TTimeIntervalMinutes aTimeout);
	
public:	// methods from CMsvTestUtils

	virtual void InstantiateClientMtmsL(){};
	virtual void InstantiateServerMtmsL(){};
	virtual void DeleteServicesL();
	virtual void CreateServicesL();
	virtual void FindExistingServicesL(){};
	virtual void InstallMtmGroupsL();
	virtual void CreateServerMtmRegsL();
	
public:	// methods from CTestUtils

	virtual void Panic(TInt aPanic);

private:

	CScheduleSendTestUtils(RTest& aTest);
	void ConstructL(TUint aCreationFlags);
	
	void InstallPigeonMtmGroupL();
	void CreatePigeonServerMtmRegL();
	
private:

	TMsvId				iServiceId;
	};
	
class CTestEngine : public CBase
	{
public:

	static CTestEngine* NewL(RTest& aTest);
	virtual ~CTestEngine();
	
	void DoTestsL();
	
private:

	CTestEngine();
	void ConstructL(RTest& aTest);
	
private:

	CScheduleSendTestUtils* iTestUtils;
	
	};
	
class MTestFolderObserverCallback
	{
public:
	virtual void UpdatedMessageL(TMsvId aFolderId, TMsvId aMessage) =0;
	virtual CMsvEntry* CreateEntryL(TMsvId aFolderId) =0;
	};		

class CTestFolderObserver : public CBase,
							public MMsvEntryObserver
	{
public:

	static CTestFolderObserver* NewL(MTestFolderObserverCallback& aCallback, TMsvId aFolderId);
	virtual ~CTestFolderObserver();
	
	void StartObserverL();
	void StopObserver();
	
private:	// methods from MMsvEntryObserver

	virtual void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	
private:

	CTestFolderObserver(MTestFolderObserverCallback& aCallback, TMsvId aFolderId);
	void ConstructL();

private:
	
	MTestFolderObserverCallback&	iCallback;
	TMsvId							iFolderId;
	CMsvEntry*						iEntryForObserver;	
	};							

	
/**
Base class for test cases.

Runs through state machine. Testing is done on sending state, schedule flag and 
the pending conditions flag. 

NOTE - the pending conditions should be checked only if the message has been
scheduled for pending conditions. In this test harness, this indicated by the
error value of the messsage being KErrorActionConditions.

The values are checked after the messages are created - message created done by
derived test cases. Then after the scheduling - again derived classed do the
scheduling. The values are then checked after SchSendExe actually runs the schedule
and then after the pigeon MTM does the send.

NOTE - after the post-scheduling check, the test may may stop - the derived class
controls this.

Sending state and schedule flag values and whether to check the pending conditions
flag or not - 

Post creation 	- KMsvSendStateWaiting, false, false

Post scheduling	- test-case dependent, true, false

Pre-sending		- KMsvSendStateWaiting, true, false

Post-sending	- KMsvSendStateSent, true, false

Also, during the post-sending check, the schedule time is compared against the 
current time. In most cases, the current time should be the same or later than
the schedule time. In the cases where the message was scheduled for pending 
conditions and the conditions were met, then the current time should be before 
the scheduled time. Although for messages scheduled for pending conditions but
the conditions have not been met, then the current time should be the same or
later than the schedule time.
*/	
class CTestScheduleSendBase : public CActive,
							  public MTestFolderObserverCallback
	{
public:

	virtual ~CTestScheduleSendBase();
	void Start();
	
private:	// methods from CActive

	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError);
	
private:	// methods from MTestFolderObserverCallback

	virtual void UpdatedMessageL(TMsvId aFolderId, TMsvId aMessage);
	virtual CMsvEntry* CreateEntryL(TMsvId aFolderId);

protected:

	CTestScheduleSendBase(CScheduleSendTestUtils& aTestUtils);
	void ConstructL();

	void CompleteSelf();
	void ClearFoldersL();
	void TestComplete();
	TMsvId CreateMessageL(TTime& aTime, TInt aError);
	void TransferCommandL(TInt aCommand);
	void CheckMessageDetailsL(TMsvId aId, TInt aState, TBool aScheduled, TBool aPendingConditions);
		
	CMsvSession& Session();
	CMsvEntry& MsvEntry();
	
protected:

	CScheduleSendTestUtils&	iTestUtils;
	CMsvEntrySelection*		iSelection;
	CMsvEntrySelection*		iPendingSent;
	CMsvOperation*			iOperation;
	TInt					iMessagesDone;
	TBool					iConditionsMet;

private:

	const TDesC& GetStateText(TInt aState) const;
	const TDesC& GetBoolText(TBool aBool) const;

	TBool ConditionsMet() const;

	virtual void UpdateSysAgentConditionsL();
	virtual void CheckCreatedMessagesL();
	virtual void CheckScheduledMessagesL();
	virtual TBool NotifyMessagesScheduledL();
	
	virtual void CheckPreSendMessageL(TMsvId aId);
	virtual void CheckSentMessageL(TMsvId aId);

	virtual TInt PreSendState();	
	virtual TInt SentState();

	virtual void CreateMessagesL() =0;
	virtual void ScheduleMessagesL() =0;
	virtual TInt ScheduledState() =0;
	virtual const TDesC& TestName() =0;

private:

	enum TScheduleSendTestState
		{
		EClearFolders,
		EUpdateSysAgentConditions,
		ECreateMessages,
		ECheckCreatedMessages,
		EScheduleMessages,
		ECheckScheduledMessages,
		EPendingMessagesSent,
		EDone
		};			
	
private:
	
	CTestFolderObserver*	iOutboxObserver;
	CTestFolderObserver*	iSentFolderObserver;
	TScheduleSendTestState	iState;
	
	};
	
/**
Tests the CMsvScheduleSend::ScheduleL API.

Creates 2 pigeon messages that are both schedule for immediate sending.

Post scheduling sending state values - KMsvSendStateScheduled.
*/
class CTestSchedule : public CTestScheduleSendBase
	{
public:

	static CTestSchedule* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestSchedule();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();

private:
	
	CTestSchedule(CScheduleSendTestUtils& aTestUtils);
	
	};
	
/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages that are both re-schedule for immediate sending.

Post scheduling sending state values - KMsvSendStateResend.
*/
class CTestReScheduleTime_1 : public CTestScheduleSendBase
	{
public:

	static CTestReScheduleTime_1* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestReScheduleTime_1();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();

private:
	
	CTestReScheduleTime_1(CScheduleSendTestUtils& aTestUtils);
	
	};
	
/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages - one is scheduled for immeduate sending and the other
is scheduled for later sending.

Post scheduling sending state values - KMsvSendStateResend.
*/
class CTestReScheduleTime_2 : public CTestScheduleSendBase
	{
public:

	static CTestReScheduleTime_2* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestReScheduleTime_2();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();

private:
	
	CTestReScheduleTime_2(CScheduleSendTestUtils& aTestUtils);
	
	};
	
/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages that are both re-schedule for pending conditions. No
timeout value is specified. The conditions are met.

Post scheduling sending state values - KMsvSendStateResend.
*/
class CTestRescheduleConditions_1 : public CTestScheduleSendBase
	{
public:

	static CTestRescheduleConditions_1* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestRescheduleConditions_1();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void UpdateSysAgentConditionsL();
	virtual TBool NotifyMessagesScheduledL();
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();
	
private:
	
	CTestRescheduleConditions_1(CScheduleSendTestUtils& aTestUtils);

	};
	
/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages that are both re-schedule for pending conditions. A
short timeout value is specified. The conditions are met.

Post scheduling sending state values - KMsvSendStateResend.
*/
class CTestRescheduleConditions_2 : public CTestScheduleSendBase
	{
public:

	static CTestRescheduleConditions_2* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestRescheduleConditions_2();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void UpdateSysAgentConditionsL();
	virtual TBool NotifyMessagesScheduledL();
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();
	
private:
	
	CTestRescheduleConditions_2(CScheduleSendTestUtils& aTestUtils);

	};
	
/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages that are both re-schedule for pending conditions. A
short timeout value is specified. Only one conditions is met - therefore timeout
should occur.

Post scheduling sending state values - KMsvSendStateResend.
*/
class CTestRescheduleConditions_3 : public CTestScheduleSendBase
	{
public:

	static CTestRescheduleConditions_3* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestRescheduleConditions_3();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void UpdateSysAgentConditionsL();
	virtual TBool NotifyMessagesScheduledL();
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();
	
private:
	
	CTestRescheduleConditions_3(CScheduleSendTestUtils& aTestUtils);

	};

/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages - one is scheduled for immeduate sending and the other
is scheduled for pending conditions.  A short timeout value is specified. The 
conditions are met.

Post scheduling sending state values - KMsvSendStateResend.
*/
class CTestRescheduleMixed_1 : public CTestScheduleSendBase
	{
public:

	static CTestRescheduleMixed_1* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestRescheduleMixed_1();
	
private:	// methods from CTestScheduleSendBase
	
	virtual void UpdateSysAgentConditionsL();
	virtual TBool NotifyMessagesScheduledL();
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();
	
private:
	
	CTestRescheduleMixed_1(CScheduleSendTestUtils& aTestUtils);

	};
	
/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages that are both re-schedule to fail. 

Post scheduling sending state values - KMsvSendStateWaiting. The schedule send
component does not alter messages that are not re-scheduled.
*/
class CTestFailed_1 : public CTestScheduleSendBase
	{
public:

	static CTestFailed_1* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestFailed_1();
	
private:	// methods from CTestScheduleSendBase
	
	virtual TBool NotifyMessagesScheduledL();
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual const TDesC& TestName();
	
private:
	
	CTestFailed_1(CScheduleSendTestUtils& aTestUtils);
	
	};

/**
Tests the CMsvScheduleSend::ReScheduleL API.

Creates 2 pigeon messages that are both re-schedule to fail, but on a second re-schedule. 

Post scheduling sending state values	- KMsvSendStateResend.
Sent sending state value			 	- KMsvSendStateWaiting. The schedule send
component does not alter messages that are not re-scheduled.
*/
class CTestFailed_2 : public CTestScheduleSendBase
	{
public:

	static CTestFailed_2* NewL(CScheduleSendTestUtils& aTestUtils);
	virtual ~CTestFailed_2();
	
private:	// methods from CTestScheduleSendBase
	
	virtual TBool NotifyMessagesScheduledL();
	virtual void CreateMessagesL();
	virtual void ScheduleMessagesL();
	virtual TInt ScheduledState();
	virtual TInt SentState();
	virtual const TDesC& TestName();
	
private:
	
	CTestFailed_2(CScheduleSendTestUtils& aTestUtils);
	
	};

#endif	// __T_SCHEDULESEND_H__
