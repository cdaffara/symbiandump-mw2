// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MSVSCHEDULESEND_H__
#define __MSVSCHEDULESEND_H__

#ifdef _DEBUG
#undef _MSG_NO_LOGGING
#endif

 


#include <e32base.h>
#include <msvstd.h>
#include <csch_cli.h>
#include <msvschedulepackage.h>
#include <msvscheduledentry.h>

class CMsvEntrySelection;
class TMsvSchedulePackage;
class TMsvSendErrorAction;
class CMsvStore;
class TMsvEntry;
class CMsvOffPeakTimes;
class CMsvScheduleSettings;
class CMsvSendErrorActions;
class CMsvSysAgentActions;
class TMsvSchedulePackage;
class TMsvEntryScheduleData;
class CMsvServerEntry;
class CRepository;

const TUid KUidMsvScheduledSendingDLL	= {0x100056A0};
const TInt KMsvSchsendArrayGrowth		= 10;

#ifdef _MSG_NO_LOGGING
#define SCHSENDLOG(a)
#else
_LIT(KSchSendLogDir, "schsend");
_LIT(KSchSendLogFile,"SchSend.txt");
#define SCHSENDLOG(a) a
#endif

/** 
Provides a Server MTM's interface to the Task Scheduler for messaging 
scheduling.

Server MTM's must provide a derived class that implements GetMessageL() to 
provide a factory for MTM-specific scheduling CMsvScheduledEntry-based objects. 

@publishedAll
@released
*/
class CMsvScheduleSend : public CBase
	{
public:
	IMPORT_C ~CMsvScheduleSend();

	IMPORT_C void ScheduleL(const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage);

	IMPORT_C void ReScheduleL(const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage, const TMsvSendErrorAction* aErrorAction = NULL);

	IMPORT_C void CheckScheduleL(const CMsvEntrySelection& aSelection);
	IMPORT_C void DeleteScheduleL(const CMsvEntrySelection& aSelection);

	IMPORT_C void SendingCompleteL(TMsvEntry& aEntry, const TBool aChangeEntry = EFalse);
	IMPORT_C void SendingCompleteL(const CMsvEntrySelection& aSelection);


	inline CMsvOffPeakTimes& OffPeakTimes() const;
	inline CMsvScheduleSettings& ScheduleSettings() const;
	inline CMsvSendErrorActions& SendErrorActions() const;
	inline CMsvSysAgentActions& AgentActions() const;

	IMPORT_C static void CreateScheduleL(RScheduler& aScheduler, const CMsvScheduleSettings& aSettings, const TTime& aStartTime, const TTimeIntervalMinutes& aValidityPeriod, TSchedulerItemRef& aRef);
	IMPORT_C static void CreateScheduleL(RScheduler& aScheduler, const CArrayFixFlat<TTaskSchedulerCondition>& aConditions, const TTime& aTimeout, TSchedulerItemRef& aRef);
	IMPORT_C static void FindScheduleL(RScheduler& aScheduler, const TTime& aStartTime, TSchedulerItemRef& aRef);
	IMPORT_C static void FindScheduleL(RScheduler& aScheduler, const CArrayFixFlat<TTaskSchedulerCondition>& aConditions, const TTime& aTimeout, TSchedulerItemRef& aRef);
	IMPORT_C static void RoundUpToMinute(TTime& aTime);
	IMPORT_C static void ScheduleEntryL(RScheduler& aScheduler, const TSchedulerItemRef& aRef, const TMsvSchedulePackage& aPackage, TTaskInfo& aInfo);
	IMPORT_C static void UpdateEntryAfterSchedule(const TSchedulerItemRef& aRef, const TTaskInfo& aInfo, const TTime& aTime, TInt aFinalState, TMsvEntry& aEntry, TMsvEntryScheduleData& aData);
	IMPORT_C static void ConnectAndRegisterL(RScheduler& aScheduler, const CMsvScheduleSettings& aSettings);

	IMPORT_C void LoadScheduleSettingsL(CRepository& aRepository);

protected:

	IMPORT_C CMsvScheduleSend(CMsvServerEntry& aServerEntry);
	IMPORT_C virtual void ConstructL();

/**
Gets the schedule data and recipients of a message.

Recipient information is MTM-specific, so the server MTM must provide a 
derived class that implements CMsvScheduledEntry, and return objects of 
that type from this function.

@param	aId 
The message ID.

@return 
The message's schedule data.
*/
	virtual CMsvScheduledEntry* GetMessageL(const TMsvId aId) const = 0;

private:

	enum
		{
		EDefaultTaskPriority	= 1, //used when scheduling a task in the task scheduler
		EDefaultTaskRepeat		= 0	 //used when scheduling a task in the task scheduler
		};
	
	void SendingCompleteL(CMsvScheduledEntry& aScheduledEntry, const TBool aChangeEntry);

	TBool GetNextRetry(CMsvScheduledEntry& aMessage, const TMsvSendErrorAction& aErrorAction, TTimeIntervalSeconds& aInterval) const;

	void DoScheduleL(CMsvScheduledEntries& aSchEntries, const TInt aFinalState, const TTime& aTime, TBool aPendingConditions);
	void DoReScheduleL(CMsvScheduledEntries& aSchEntries);
	
	void GetMessagesL(const CMsvEntrySelection& aSelection);

	void ConnectAndRegisterL();

	void ScheduleEntryL(CMsvScheduledEntry& aMessage, const TInt aFinalState, const TTime& aStartTime, const TSchedulerItemRef& aRef, TBool aPendingConditions);

	void GetOffPeakL(TTime aFromTime, TTime& aStartTime, TTimeIntervalMinutes& aValidityPeriod) const;
	
	void DeleteScheduleForEntryL(CMsvScheduledEntry& aMessage);
	void ResetScheduleInfoForEntryL(CMsvScheduledEntry& aMessage, const TBool aChangeEntry);
	void SortByDateAndPendingConditionsL(CMsvScheduledEntries& aScheduledEntries);
	void CreateScheduleL(const TTime& aStartTime, const TTimeIntervalMinutes& aValidityPeriod, const CArrayFixFlat<TTaskSchedulerCondition>& aSchConditions, TBool aPendingConditions, TSchedulerItemRef& aRef);
	void FindScheduleL(const TTime& aTime, const CArrayFixFlat<TTaskSchedulerCondition>& aSchConditions, TBool aPendingConditions, TSchedulerItemRef& aRef);

	void PopulateScheduleConditionsL(CArrayFixFlat<TTaskSchedulerCondition>& aSchConditions);
	TBool SetMessageStartTime(CMsvScheduledEntry& aMessage, const TTime& aFromTime, const TMsvSendErrorAction* aErrorAction = NULL);

	TBool TaskAndMessageMatchL(const CMsvScheduledEntry& aMessage);
	TMsvId GetMessageIdForTaskL(TInt aTaskId);
#ifndef _MSG_NO_LOGGING
	void FLog(TRefByValue<const TDesC> aFormat, ...);
	void FLog(TRefByValue<const TDesC8> aFormat, ...);
#endif

protected:

	/** The Server MTM specific scheduled message sending settings. */
	CMsvScheduleSettings*	iSettings;
	/** The Server MTM specific off peak time periods. */
	CMsvOffPeakTimes*		iOffPeakTimes;
	/** The Server MTM specific actions to take when particular errors occur during 
	message sending. */
	CMsvSendErrorActions*	iErrorActions;
	/** The System Agent conditions that must be satisfied before a sending attempt. */
	CMsvSysAgentActions*	iAgentActions;

	/** Indicates if this has been registered with the Task Scheduler. 
	
	Initially set to EFalse. */
	TBool					iRegistered;	
	/** Provides an interface to the Task Scheduler. */
	RScheduler				iScheduler;	
	/** The CMsvServerEntry of the server MTM. */
	CMsvServerEntry&		iServerEntry;
	/** Scheduling options. */
	TMsvSchedulePackage		iPackage;

	/** Array of message schedule data. */
	CMsvScheduledEntries*	iSchEntries;

	/** Array of schedule entry data. */
	CArrayFixFlat<TScheduleEntryInfo2>* iSchEntryInfo;
	/** Array of schedule task data. */
	CArrayFixFlat<TTaskInfo>* iSchTaskInfo;
	};

#include <msvschedulesend.inl>

#endif
