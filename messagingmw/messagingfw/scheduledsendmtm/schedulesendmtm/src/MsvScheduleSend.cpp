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

#ifdef _DEBUG
#undef _MSG_NO_LOGGING
#endif

#include <msvschedulesend.h>

#include <msventry.h>
#include <msvids.h>
#include <bautils.h>
#include <e32property.h>
#include <centralrepository.h>

#include <msvschedulesettings.h>
#include <msvoffpeaktime.h>
#include <msvsysagentaction.h>
#include <schsend_panic.h>
#include <tmsvschedulesettingsutils.h>
#include <schinfointernal.h>

#ifndef _MSG_NO_LOGGING
#include <flogger.h>
#endif


_LIT(KSchSendExe, "schsendexe.exe");

LOCAL_C TBool operator==(const CArrayFixFlat<TTaskSchedulerCondition>& aConditions1, const CArrayFixFlat<TTaskSchedulerCondition>& aConditions2)
	{
	TInt count1 = aConditions1.Count();
	TInt count2 = aConditions2.Count();
	
	// Compare the conditions.	
	if( count1 != count2 )
		{
		// Different number of conditions - no match.
		return EFalse;
		}
		
	// Search for all the conditions in info1 are in info2
	for( TInt ii = 0; ii < count1; ++ii )
		{
		TTaskSchedulerCondition condition1 = aConditions1[ii];
		TTaskSchedulerCondition condition2;
		TBool found = EFalse;
		TInt jj = 0;
		while( !found && jj < count2 )
			{
			condition2 = aConditions2[jj];
			
			if( condition2.iKey == condition1.iKey )
				found = ETrue;
			else
				++jj;
			}
		if( !found || condition2.iState != condition1.iState || condition2.iType != condition1.iType )
			{
			// Either aConditions2 has not got this condition or it has but it 
			// does not match - no match.
			return EFalse;
			}
		}
	return ETrue;
	}

/**
Constructor.

@param aServerEntry 
The CMsvServerEntry of the server MTM of which this CMsvScheduleSend 
object is a member.
*/
EXPORT_C CMsvScheduleSend::CMsvScheduleSend(CMsvServerEntry& aServerEntry)
	: iServerEntry(aServerEntry)
	{
	}

/**
Second phase constructor.

This constructor creates instances of the following classes: 
CMsvScheduleSettings, CMsvOffPeakTimes, CMsvSendErrorActions,
CMsvSysAgentActions and CMsvScheduledEntries.
*/
EXPORT_C void CMsvScheduleSend::ConstructL()
	{
	iSettings			= CMsvScheduleSettings::NewL();
	iOffPeakTimes		= new (ELeave) CMsvOffPeakTimes();
	iErrorActions		= CMsvSendErrorActions::NewL();
	iAgentActions		= new (ELeave) CMsvSysAgentActions();
	iSchEntries			= new (ELeave) CMsvScheduledEntries(KMsvSchsendArrayGrowth);

	iSchEntryInfo		= new (ELeave) CArrayFixFlat<TScheduleEntryInfo2>(KMsvSchsendArrayGrowth);
	iSchTaskInfo		= new (ELeave) CArrayFixFlat<TTaskInfo>(KMsvSchsendArrayGrowth);
	}

/**
Destructor.
*/
EXPORT_C CMsvScheduleSend::~CMsvScheduleSend()
	{
	delete iSettings;
	delete iOffPeakTimes;
	delete iErrorActions;
	delete iAgentActions;

	if (iScheduler.Handle())
		iScheduler.Close();

	if (iSchEntries)
		iSchEntries->ResetAndDestroy();

	delete iSchEntries;

	delete iSchTaskInfo;
	delete iSchEntryInfo;
	}

/**
Schedules messages on the task scheduler.

Messages that are successfully scheduled have their sending state set to
KMsvSendStateScheduled.

@param aSelection 
Array of message IDs that need to be scheduled. This array cannot be empty.
All the messages identified in the selection must belong to the same MTM;
be scheduled for the same time; have the same setting for their OffPeak() 
flag; have the scheduled time stored in the iDate member of their 
corresponding TMsvEntry. 

@param aPackage 
Scheduling options

@leave Any error code but KErrLocked and KErrNotFound
The method overloading CMsvScheduledEntry::GetMessageL() left with an error,
i.e. the scheduling info of one of the messages from the selection could not
be retrieved from the message server.

@leave Any error code
Unable to reset the previous scheduling info for a message.

@panic ScheduleSend-DLL 0
The array of message IDs is empty.
Debug build only.

@panic ScheduleSend-DLL 1
At least one of the selected messages is scheduled for a different time
as the others.
Debug build only.

@panic ScheduleSend-DLL 2
At least one of the selected messages does not belong to the same MTM.
Debug build only.

@panic ScheduleSend-DLL 3
At least one of the selected messages does not have the same off-peak 
settings as the others.
Debug build only.
*/
EXPORT_C void CMsvScheduleSend::ScheduleL(const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage)
	{
	__ASSERT_DEBUG(aSelection.Count() > 0, gPanic(EMessageSelectionEmpty));

	iPackage = aPackage;
	GetMessagesL(aSelection);	//Leaves with KErrNotFound if there
																	//are no messages returned in schEntries
	TInt entryCount = iSchEntries->Count();
	SCHSENDLOG(FLog(_L8("Asked to schedule %d msgs"), entryCount));
	
	if (entryCount)
		{
		TTime startTime;

#if defined(_DEBUG)
		CMsvScheduledEntry* firstEntry = (*iSchEntries)[0];
#endif

		while (entryCount--)
			{
			CMsvScheduledEntry* message = iSchEntries->At(entryCount);
			__ASSERT_DEBUG(firstEntry->ScheduleDate() == message->ScheduleDate(), gPanic(EMessagesNotSameTime));

			startTime = message->ScheduleDate();

			// Reset previous scheduling info
			DeleteScheduleForEntryL(*message);
			ResetScheduleInfoForEntryL(*message, EFalse);
			}

		//Schedule the messages
		DoScheduleL(*iSchEntries, KMsvSendStateScheduled, startTime, EFalse);
		}
	}

/** 
Determines when the messages should be re-scheduled on the task scheduler, 
then schedules the messages at the new time(s).

Messages that are successfully re-scheduled are updated. The pending conditions
flag indicates whether the message has been schedule for a set of conditions 
being met (or a timeout occuring) or scheduled for a specified time/date.

NOTE - conditions scheduling is only supoprted from 8.1 onwards.

In the case of time-scheduling, the date field is the scheduled time/date. In 
the case of conditions-scheduling, the date field reflects the timeout value.

There are several cases when messages are not re-scheduled. If all recipients 
have been sent to - in this case the message's sending state set to 
KMsvSendStateSent. If, more commonly, the message's maximum number of re-tries
has been exceeded or the error action was ESendActionFail then the message is 
not changed.

@param aSelection 
Array of message IDs that need to be re-scheduled. This array cannot be empty. 
All the messages identified must belong to the same MTM. It is not a 
precondition that each message has already been scheduled on the task 
scheduler.

@param aPackage
Scheduling options.

@param aErrorAction
The specific action to take with the messages. If this argument is omitted, 
then ReScheduleL() uses the iError member of each TMsvEntry to find the 
related TMsvSendErrorAction in iErrorActions.

@panic ScheduleSend-DLL 0
The array of message IDs is empty.
Debug build only.
*/
EXPORT_C void CMsvScheduleSend::ReScheduleL(const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage, const TMsvSendErrorAction* aErrorAction)
	{
	__ASSERT_DEBUG(aSelection.Count() > 0, gPanic(EMessageSelectionEmpty));
	iPackage = aPackage;
	GetMessagesL(aSelection);

	TInt entryCount = iSchEntries->Count();
	SCHSENDLOG(FLog(_L8("Asked to re-schedule %d msgs"), entryCount));

	if (entryCount)
		{
		TTime curTime;
		curTime.UniversalTime();
		curTime += iSettings->Latency();

		while (entryCount--)
			{
			CMsvScheduledEntry* message = iSchEntries->At(entryCount);
	
			SCHSENDLOG(FLog(_L8("\tAttempting to Re-Schedule msg %d"), message->Id()));

			if (!SetMessageStartTime(*message, curTime, aErrorAction))
				{
				SCHSENDLOG(FLog(_L8("\t\tCannot Re-Schedule msg %d (new sending state %d)"), message->Id(), message->SendingState()));
				DeleteScheduleForEntryL(*message);
				ResetScheduleInfoForEntryL(*message, ETrue);
				delete message;
				iSchEntries->Delete(entryCount);
				}
			}

		//Send the messages if there are any left to send
		if (iSchEntries->Count())
			{
			DoReScheduleL(*iSchEntries);
			}
		else
			{
			SCHSENDLOG(FLog(_L8("\tNo messages to Re-Schedule")));
			}
		}
	}

/**
Delete the schedules for the specified messages from the task scheduler. 

The messages themselves are not deleted.

@param aSelection 
Array of message IDs that need to be deleted from the task scheduler.

@leave Any error code
Unable to connect and register with the scheduler.

@leave Any error code but KErrLocked and KErrNotFound
The method overloading CMsvScheduledEntry::GetMessageL() left with an error,
i.e. the scheduling info of one of the messages from the selection could not
be retrieved from the message server.

@leave Any error code
Unable to reset the previous scheduling info for a message.
*/
EXPORT_C void CMsvScheduleSend::DeleteScheduleL(const CMsvEntrySelection& aSelection)
	{
	ConnectAndRegisterL();

	GetMessagesL(aSelection);

	TInt entryCount = iSchEntries->Count();
	SCHSENDLOG(FLog(_L8("Asked to delete schedule of %d msgs"), entryCount));

	while (entryCount--)
		{
		CMsvScheduledEntry* message = iSchEntries->At(entryCount);
		SCHSENDLOG(FLog(_L8("\tDelete schedule for msg %d (mtm %d, oldSendState %d, newSendState %d)"), message->Id(), message->Mtm().iUid, message->SendingState(), KMsvSendStateSuspended));
		message->SetSendingState(KMsvSendStateSuspended);
		DeleteScheduleForEntryL(*message);
		ResetScheduleInfoForEntryL(*message, ETrue);
		}
	}

/*
	Sets the message's scheduled flag to EFalse
	Resets the schedule data associated with each message
	Resets the number of retries for each recipient
	Stores the data and recipients (in a stream associated with the TMsvEntry)
*/
void CMsvScheduleSend::SendingCompleteL(CMsvScheduledEntry& aMessage, const TBool aChangeEntry)
	{
	__ASSERT_DEBUG(iServerEntry.Entry().Id() == aMessage.Id(), gPanic(EServerEntryNotSetToCorrectEntry));
	
	aMessage.SetScheduled(EFalse);
	aMessage.iData.Reset();
	aMessage.RecipientsResetRetries();

	if (aChangeEntry)
		{
		TMsvEntry entry = iServerEntry.Entry();
		aMessage.Entry(entry);
		User::LeaveIfError(iServerEntry.ChangeEntry(entry));
		}

	CMsvStore* store = iServerEntry.EditStoreL();
	CleanupStack::PushL(store);
	aMessage.StoreL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(store); 
	}

/**
Tells the scheduler that sending is complete.

This method sets the messages's scheduled flag to false, resets the schedule
data associated with each message and the number of retries for each 
recipient and stores the data and recipients in a stream associated with 
the TMsvEntry.

@param aSelection 
Messages that were either successfully sent or which failed all the attempts
to send.
*/
EXPORT_C void CMsvScheduleSend::SendingCompleteL(const CMsvEntrySelection& aSelection)
	{
	TInt count = aSelection.Count();

	while (count--)
		{
		CMsvScheduledEntry* schEntry = GetMessageL(aSelection[count]);
		CleanupStack::PushL(schEntry);
		SendingCompleteL(*schEntry, ETrue);
		CleanupStack::PopAndDestroy(); //schEntry
		}
	}

/**
Tells the scheduler that sending is complete.

This function must be called when a message that had previously been scheduled 
is either sent or has failed. This function:

1. Deletes the TMsvEntryScheduleData associated with the message

2. Sets the Scheduled flag to EFalse

3. If required, calls ChangeEntry() on the message server entry

Note: SendingCompleteL() does not change the sending state of each message, 
nor delete each message from the task scheduler.

@param aEntry 
The message which was either successfully sent or which failed (all the 
attempts) to send. It is not a precondition that the message has already 
been scheduled on the task scheduler.

@param aChangeEntry 
If aChangeEntry is ETrue then SendingCompleteL() will call 
CMsvServerEntry::ChangeEntry() to update the message on the message server. 

@panic ScheduleSend-DLL 24
The server entry is not set to the correct entry.
Debug build only.
*/
EXPORT_C void CMsvScheduleSend::SendingCompleteL(TMsvEntry& aEntry, const TBool aChangeEntry)
	{
	__ASSERT_DEBUG(iServerEntry.Entry().Id() == aEntry.Id(), gPanic(EServerEntryNotSetToCorrectEntry));

	CMsvScheduledEntry* schEntry = GetMessageL(aEntry.Id());
	CleanupStack::PushL(schEntry);

	SendingCompleteL(*schEntry, EFalse);
	schEntry->Entry(aEntry);

	if (aChangeEntry)
		{
		User::LeaveIfError(iServerEntry.ChangeEntry(aEntry));
		}

	CleanupStack::PopAndDestroy(); //schEntry
	}


/**
Loads schedule settings from CenRep

@param aRepository 
CenRep repository to load settings from
*/
EXPORT_C void CMsvScheduleSend::LoadScheduleSettingsL(CRepository& aRepository)
	{
	TMsvScheduleSettingsUtils::LoadScheduleSettingsL(*iSettings, aRepository);
	TMsvScheduleSettingsUtils::LoadOffPeakSettingsL(*iOffPeakTimes, aRepository);
	TMsvScheduleSettingsUtils::LoadSendErrorSettingsL(*iErrorActions, aRepository);
	TMsvScheduleSettingsUtils::LoadSysAgentSettingsL(*iAgentActions, aRepository);	
	}

/**
Verifies that the schedule information stored in specified messages is the 
same as that on the task scheduler.

@param aSelection 
Array of message IDs that need to be checked against the task scheduler.

@panic ScheduleSend-DLL 0
The array of message IDs is empty.
Debug build only.
*/
EXPORT_C void CMsvScheduleSend::CheckScheduleL(const CMsvEntrySelection& aSelection)
	{
	__ASSERT_DEBUG(aSelection.Count(), gPanic(EMessageSelectionEmpty));

	GetMessagesL(aSelection);	//Leaves with KErrNotFound if there are no messages returned in iSchEntries
	TInt entryCount = iSchEntries->Count();
	SCHSENDLOG(FLog(_L8("Asked to check schedule for %d msgs"), entryCount));
	
	ConnectAndRegisterL();

	while (entryCount--)
		{
		TBool found = EFalse;
		TTsTime schTime;

		CMsvScheduledEntry& sEntry = *iSchEntries->At(entryCount);

		if (!sEntry.iData.IsReset())
			{
			TSchedulerItemRef ref;
			TInt size = 0;
			TTaskInfo info;
			
			TInt err = iScheduler.GetTaskDataSize(sEntry.iData.iTaskId, size);

			if (!err)
				{
				HBufC* buf = HBufC::NewLC(size);
				TPtr ptr = buf->Des();

				User::LeaveIfError(iScheduler.GetTaskInfoL(sEntry.iData.iTaskId, info, ptr, ref, schTime));

				CleanupStack::PopAndDestroy(buf);
				found = ETrue;
				}
			else if (err != KErrNotFound)
				{
				User::Leave(err);
				} 
			} 
			
		if (iServerEntry.SetEntry(sEntry.Id()) == KErrNone)
			{
			TMsvEntry entry = iServerEntry.Entry();
			TInt sendingState = entry.SendingState();

			if (sendingState == KMsvSendStateScheduled || sendingState == KMsvSendStateResend || entry.Scheduled())
				{
				if (found)
					{
					entry.SetScheduled(ETrue);
					entry.iDate = schTime.GetUtcTime();
					User::LeaveIfError(iServerEntry.ChangeEntry(entry));
					}
				else
					{
					entry.SetScheduled(EFalse);
					entry.SetSendingState(KMsvSendStateUnknown);
					entry.iDate.UniversalTime();
					User::LeaveIfError(iServerEntry.ChangeEntry(entry));
					SendingCompleteL(sEntry, EFalse);
					} 
				} 
			} 
		} 
	}

/**
Determines the schedule time for a message.

The schedule time is determined by the error action. If no error action is
supplied, then the error action is obtained from the iErrorActions. This is a 
list of error codes associated with an error action. The error code with which
the message failed to be sent is used to determine the error action from this
list.

This function also increments the number of re-tries that the message has 
undergone.

A value of ETrue is returned if the message can be sent or a value of EFalse if
the message cannot be sent. 

If the message can be sent, then its schedule date and pending conditions flag 
are updated.

@see CMsvScheduleSend::ReScheduleL().

@param	aMessage
The message to be re-scheduled.

@param	aFromTime
The current time. Used to calculate the absolute schedule time once the interval
has been established.

@param	aErrorAction
The error action applied to the message. This can be NULL in which case the error
action is obtained from iErrorActions.

@return
A value of ETrue if the message should be sent or EFalse if it should not
*/
TBool CMsvScheduleSend::SetMessageStartTime(CMsvScheduledEntry& aMessage, const TTime& aFromTime, const TMsvSendErrorAction* aErrorAction)
	{
	TBool sendMsg = EFalse;
	TMsvSendErrorAction action;

	if( aErrorAction != NULL )
		{
		sendMsg = aMessage.CanSendToAnyRecipients(*aErrorAction);
		action = *aErrorAction;
		}
	else 
		{
		sendMsg = aMessage.CanSendToAnyRecipients(*iErrorActions, action);
		}

	SCHSENDLOG(FLog(_L8("\t\tCanSendToAnyRecipients() ret %d"), sendMsg));

	if( sendMsg )
		{
		// Increase the number of times this message has been retried.
		if( action.iRetries != ESendRetriesInfinite )
			{
			aMessage.iData.IncreaseRetries();
			aMessage.RecipientsIncreaseRetries();
			}
			
		TBool retryConditionMet = action.iAction == ESendActionRetryConditionMet;
		
		if( retryConditionMet && aMessage.PendingConditions() )
			{
			// There are two cases in which the message being re-scheduled has 
			// already been scheduled for pending conditions met and the error 
			// action is retry when conditions met.
			// 1.	The timeout has expired - current time exceeds the message's 
			//		schedule time (assuming that the message has a timeout).
			// 2.	The conditions were temporarily met and by the time the 
			//		waiting message tried to be sent the conditions were no 
			//		longer true.
			if( aMessage.ScheduleDate()!= Time::MaxTTime() && aFromTime > aMessage.ScheduleDate() )
				{
				// Case 1 - the message should not be re-scheduled (this marks
				// the message as failed).
				sendMsg = EFalse;
				}
			// NOTE - the else is case 2; the message should be re-scheduled,
			// but the timeout value should NOT be re-calculated -> do nothing.
			}
		else
			{
			TTimeIntervalSeconds interval;
			if( GetNextRetry(aMessage, action, interval) )
				{
				if( interval.Int() == 0 && retryConditionMet )
					{
					// The timeout was set to zero - set a schedule time of
					// Time::MaxTTime().
					aMessage.SetScheduleDate(Time::MaxTTime());
					}
				else
					{
					TTime schTime = aFromTime + interval;
					RoundUpToMinute(schTime);
					aMessage.SetScheduleDate(schTime);
					}
				aMessage.SetPendingConditions(retryConditionMet);
				}
			else
				{
				sendMsg = EFalse;
				}

			SCHSENDLOG(FLog(_L8("\t\tGetNextRetry() ret %d"), sendMsg));
			}
		}

	if( !sendMsg )
		{
		TBool failed = ETrue;
		TInt state	 = KMsvSendStateFailed;

		if( aMessage.RecipientsAllSent() )
			{
			failed	= EFalse;
			state	= KMsvSendStateSent;
			}
		else
			{
			aMessage.RecipientsSetFailed();
			}

		aMessage.SetFailed(failed);
		aMessage.SetSendingState(state);
		}

	return sendMsg;
	}

/**
Utility function that rounds a specified time up to the nearest minute.

@param aTime 
On return, the passed value rounded up to the nearest minute.
*/

EXPORT_C void CMsvScheduleSend::RoundUpToMinute(TTime& aTime)
	{
	TDateTime dt(aTime.DateTime());

	if (dt.MicroSecond() != 0 || dt.Second() != 0)
		{
		dt.SetMicroSecond(0);
		dt.SetSecond(0);
		aTime = dt;
		aTime += (TTimeIntervalMinutes) 1;
		}
	}

/*
	Groups messages in aSchEntries by the date (time) they are to be scheduled
	and if they are pending conditions met or not and then schedules each group.
*/

void CMsvScheduleSend::DoReScheduleL(CMsvScheduledEntries& aSchEntries)
	{
	const TInt entryCount = aSchEntries.Count();
	__ASSERT_DEBUG(entryCount > 0, gPanic(EMessageSelectionEmpty));

	// Sort aSchEntries by CMsvScheduledEntry::iEntry.iDate and if they are 
	// pending conditions or not.
	SortByDateAndPendingConditionsL(aSchEntries);

	//New selection of messages that will be used to store messages
	//that are to be sent at the same time.
	CMsvScheduledEntries* sel = new (ELeave) CMsvScheduledEntries(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(sel);

	sel->SetReserveL(entryCount); // so following AppendL()s won't leave

	CMsvScheduledEntry* curMessage = aSchEntries[0];
	TTime lastTime = curMessage->ScheduleDate();
	TBool lastPending = curMessage->PendingConditions();
	sel->AppendL(curMessage);

	// Delete old schedule
	DeleteScheduleForEntryL(*curMessage);

	for( TInt curMsg = 1; curMsg < entryCount; ++curMsg ) // must forward traverse the array
		{
		curMessage = aSchEntries[curMsg];
		TTime thisTime = curMessage->ScheduleDate();
		TBool thisPending = curMessage->PendingConditions();

		// Delete old schedule
		DeleteScheduleForEntryL(*curMessage);
		if( thisTime == lastTime && lastPending == thisPending )
			{
			sel->AppendL(curMessage);
			}
		else
			{
			// Schedule the messages in the selection
			DoScheduleL(*sel, KMsvSendStateResend, lastTime, lastPending);
			sel->Reset(); // not ResetandDestroy because the messages are still in schEntries
			sel->AppendL(curMessage);
			lastTime = thisTime;
			lastPending = thisPending;
			}
		}

	if( sel->Count() )
		{
		// Schedule the remaining messages
		DoScheduleL(*sel, KMsvSendStateResend, lastTime, lastPending);
		}

	sel->Reset(); // not ResetandDestroy because the messages are still in schEntries
	CleanupStack::PopAndDestroy(sel);
	}

/*
	Gets the Message ID stored against the task scheduler task aTaskId

	@param aTaskId ID of task scheduler task
*/

TMsvId CMsvScheduleSend::GetMessageIdForTaskL(TInt aTaskId)
	{
	TInt taskSize = 0;
	TTaskInfo taskInfo;
	TSchedulerItemRef taskSch;
	TTsTime taskDue;

	User::LeaveIfError(iScheduler.GetTaskDataSize(aTaskId, taskSize));

	HBufC* taskData = HBufC::NewLC(taskSize);
	TPtr ptr(taskData->Des());

 	User::LeaveIfError(iScheduler.GetTaskInfoL(aTaskId, taskInfo, ptr, taskSch, taskDue));
 
 	//Restore the TMsvSchedulePackage stored against the task scheduler task
 	TMsvSchedulePackage taskPackage;
 	taskPackage.UnpackL(taskInfo, *taskData);
 
 	CleanupStack::PopAndDestroy(taskData);
 
 	return taskPackage.iId;
 	}
 
/*
	Checks whether the message ID stored against task scheduler task aMessage.iData.iTaskId equals aMessage.Id().
*/

TBool CMsvScheduleSend::TaskAndMessageMatchL(const CMsvScheduledEntry& aMessage)
	{
	TMsvId id = 0;
	TRAPD(err, id = GetMessageIdForTaskL(aMessage.iData.iTaskId));

	SCHSENDLOG(FLog(_L8("\tGetMessageIdForTask [taskId=%d err=%d id=%d aMessage.Id=%d"), aMessage.iData.iTaskId, err, id, aMessage.Id()));
	TBool match = EFalse;
 
	if (err == KErrNone)
		{
		match = (id == aMessage.Id());
		}
	else if (err != KErrNotFound)
		{
		User::Leave(err);
		}
 
	return match;
	}
	
void CMsvScheduleSend::ResetScheduleInfoForEntryL(CMsvScheduledEntry& aMessage, const TBool aChangeEntry)
	{
	// Reset the scheduling info in the entry.
	TMsvId oldId = iServerEntry.Entry().Id();
	User::LeaveIfError(iServerEntry.SetEntry(aMessage.Id()));
	SendingCompleteL(aMessage, aChangeEntry);	
	iServerEntry.SetEntry(oldId); // ignore any error	
	}
	
void CMsvScheduleSend::DeleteScheduleForEntryL(CMsvScheduledEntry& aMessage)
	{
	//Connect and register with the task scheduler
	//Delete the task from the task scheduler
	ConnectAndRegisterL();

	if (TaskAndMessageMatchL(aMessage))
		{
		TInt err = iScheduler.DeleteTask(aMessage.iData.iTaskId);

		SCHSENDLOG(FLog(_L8("\tDeleteTask Task=%d, Err=%d"), aMessage.iData.iTaskId, err));

		if (err != KErrNotFound)
			User::LeaveIfError(err);

		//Delete the schedule if there are no more tasks assigned to it

		//Declare variable to pass into GetScheduleL()
		TScheduleState2 schState;
		TTsTime schDueTime;

		// Get the schedule Type
		TScheduleType scheduleType;
		iScheduler.GetScheduleTypeL(aMessage.iData.iRef.iHandle, scheduleType);
		iSchTaskInfo->Reset();
		
		// Depends on the Schedule Type
		if (scheduleType == ETimeSchedule)
		    {
			//Get details of the existing time schedule
			err = iScheduler.GetScheduleL(aMessage.iData.iRef.iHandle, schState, *iSchEntryInfo, *iSchTaskInfo, schDueTime);

			SCHSENDLOG(FLog(_L8("\tGetScheduleL Sch=%d, Err=%d"), aMessage.iData.iRef.iHandle, err));
		    }

		else if (scheduleType == EConditionSchedule)
		    {
		    CArrayFixFlat<TTaskSchedulerCondition>* schConditions =
		        new (ELeave) CArrayFixFlat<TTaskSchedulerCondition>(KMsvSchsendArrayGrowth);

		    CleanupStack::PushL(schConditions);
			//Get details of the existing condition schedule
		    err = iScheduler.GetScheduleL(aMessage.iData.iRef.iHandle, schState, *schConditions, schDueTime, *iSchTaskInfo);

			SCHSENDLOG(FLog(_L8("\tGetScheduleL Sch=%d, Err=%d"), aMessage.iData.iRef.iHandle, err));

		    CleanupStack::PopAndDestroy(schConditions);

		    }
		else
		    {
		    User::Leave(KErrNotFound);
		    }
		    
		if (!err)
			{
			if (iSchTaskInfo->Count() == 0)
				{
				SCHSENDLOG(FLog(_L8("\tDeleting schedule %d"), aMessage.iData.iRef.iHandle));
				iScheduler.DeleteSchedule(aMessage.iData.iRef.iHandle); //ignore error
				}
			}
		else if (err != KErrNotFound)
			{
			User::Leave(err);
			}
		}
	}

/**
Determines whether the message should be sent and if so the time interval (in
seconds) that must elapse before aMessage should be sent.

The time interval is determined by the error action supplied. In the case of an
error action of ESendActionRetryConditionMet, the time interval indicates how
long the message can be pending conditions to be met before the message is failed.

The actual time period is defined in the iSettings object. This will be either
iPendingConditionsTimeout, iShortInterval, iLongInterval or an element of 
iVariableIntervals.

If the message should be sent then a value of ETrue is returned. A value of 
EFalse is returned if the message should not be sent. In this case the output
argument aInterval is not valid.

@param	aMessage
The message to be re-scheduled.

@param	aErrorAction
The error action that determines the re-schedule behaviour.

@param	aInterval
An output argument that holds the time interval to when the message should be
re-sent or when the message should be failed.

@return
A value of ETrue if the message should be sent or EFalse if it should not
*/
TBool CMsvScheduleSend::GetNextRetry(CMsvScheduledEntry& aMessage, const TMsvSendErrorAction& aErrorAction, TTimeIntervalSeconds& aInterval) const
	{
	TBool retVal = (aErrorAction.iAction != ESendActionFail);
	aInterval = 0;
	
	if( retVal )
		{
		if( aErrorAction.iAction == ESendActionRetryConditionMet )
			{
			// Interval given by the agent actions timeout value.
			aInterval = iSettings->PendingConditionsTimeout().Int() * 60;
			}
		else if( aErrorAction.iAction == ESendActionRetryImmediately )
			{
			aInterval = iSettings->ShortInterval();
			}
		else
			{
			if( aErrorAction.iRetrySpacing == ESendRetrySpacingVariable )
				{
				//Retrieve the variable spacing associated with the retry count
				const CArrayFixFlat<TTimeIntervalSeconds>& varIntervals = iSettings->VariableIntervals();
				const TInt count = varIntervals.Count();
				const TInt retry = aMessage.iData.Retries();

				if( count == 0 )
					{
					aInterval = iSettings->LongInterval();
					}
				else if( retry < count )
					{
					aInterval = varIntervals[retry];
					}
				else
					{
					aInterval = varIntervals[count - 1];
					}
				}
			else 
				{
				// Got here then aScheduleAction.iRetrySpacing == EStatic.
				aInterval = iSettings->LongInterval();
				}

			__ASSERT_DEBUG(aInterval.Int() > 0, gPanic(ERetryIntervalMustByPositive));
			}
		}
	return retVal;
	}

/**
Searches the scheduler for an existing schedule item with a schedule time that
matches with time supplied.

@see RScheduler::GetScheduleL()

@param aScheduler
Handle to the scheduler.

@param aStartTime
Schedule start time.

@param aRef
On return, the schedule item.

@leave KErrNotFound
No schedule found matching the schedule time.
*/
EXPORT_C void CMsvScheduleSend::FindScheduleL(RScheduler& aScheduler, const TTime& aStartTime, TSchedulerItemRef& aRef)
	{
	CArrayFixFlat<TScheduleEntryInfo2>* entryInfos = new (ELeave) CArrayFixFlat<TScheduleEntryInfo2>(1);
	CleanupStack::PushL(entryInfos);

	CArrayFixFlat<TTaskInfo>* taskInfos = new (ELeave) CArrayFixFlat<TTaskInfo>(1);
	CleanupStack::PushL(taskInfos);

	aRef.iHandle = KErrNotFound;

	CArrayFixFlat<TSchedulerItemRef>* refs = new (ELeave) CArrayFixFlat<TSchedulerItemRef>(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(refs);

	User::LeaveIfError(aScheduler.GetTaskRefsL(*refs, EAllSchedules, EMyTasks));

	TInt count = refs->Count();
	TScheduleState2 state;
	TTsTime nextDue;

	while (count-- && aRef.iHandle == KErrNotFound)
		{
		const TSchedulerItemRef& tempRef = (*refs)[count];

		TScheduleType type;
		User::LeaveIfError(aScheduler.GetScheduleTypeL(tempRef.iHandle, type));
		if( type == ETimeSchedule )
			{
			entryInfos->Reset();
			taskInfos->Reset();
			const TInt err = aScheduler.GetScheduleL(tempRef.iHandle, state, *entryInfos, *taskInfos, nextDue);
			if( err == KErrNone && nextDue.GetUtcTime() == aStartTime )
				aRef = tempRef;
			}
		}

	CleanupStack::PopAndDestroy(3, entryInfos);
	if (aRef.iHandle == KErrNotFound)
		User::Leave(KErrNotFound);
	}

/**
Searches the scheduler for an existing conditions schedule item with a set of 
pending conditions and timeout value that matches with those supplied.

@see RScheduler::GetScheduleL

@param aScheduler
Handle to the scheduler.

@param aConditions
The set of System Agent conditions that are required to be met to trigger the
schedule.

@param aTimeout
The timeout value for the schedule.

@param aRef
On return, the schedule item.

@leave KErrNotFound
No schedule found matching the schedule conditions and timeout.
*/
EXPORT_C void CMsvScheduleSend::FindScheduleL(
								RScheduler&										aScheduler, 
								const CArrayFixFlat<TTaskSchedulerCondition>&	aConditions, 
								const TTime&									aTimeout, 
								TSchedulerItemRef& 								aRef)
	{
	CArrayFixFlat<TTaskSchedulerCondition>* schConditions = new (ELeave) CArrayFixFlat<TTaskSchedulerCondition>(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(schConditions);

	CArrayFixFlat<TTaskInfo>* taskInfos = new (ELeave) CArrayFixFlat<TTaskInfo>(1);
	CleanupStack::PushL(taskInfos);

	aRef.iHandle = KErrNotFound;

	CArrayFixFlat<TSchedulerItemRef>* refs = new (ELeave) CArrayFixFlat<TSchedulerItemRef>(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(refs);
	
	User::LeaveIfError(aScheduler.GetTaskRefsL(*refs, EAllSchedules, EMyTasks));

	TInt count = refs->Count();
	TScheduleState2 state;
	TTsTime nextTimeout;
	
	while( count-- && aRef.iHandle == KErrNotFound )
		{
		const TSchedulerItemRef& tempRef = (*refs)[count];

		TScheduleType type;
		User::LeaveIfError(aScheduler.GetScheduleTypeL(tempRef.iHandle, type));
		if( type == EConditionSchedule )
			{
			taskInfos->Reset();
			const TInt err = aScheduler.GetScheduleL(tempRef.iHandle, state, *schConditions, nextTimeout, *taskInfos);
			if( err == KErrNone && nextTimeout.GetUtcTime() == aTimeout && *schConditions == aConditions )
				aRef = tempRef;
			}
		}

	CleanupStack::PopAndDestroy(3, schConditions);
	if (aRef.iHandle == KErrNotFound)
		User::Leave(KErrNotFound);
	}	

void CMsvScheduleSend::FindScheduleL(const TTime& aTime, const CArrayFixFlat<TTaskSchedulerCondition>& aSchConditions, TBool aPendingConditions, TSchedulerItemRef& aRef)
	{
	if( aPendingConditions )
		FindScheduleL(iScheduler, aSchConditions, aTime, aRef);
	else
		FindScheduleL(iScheduler, aTime, aRef);
	}
	
/**
Does the actual scheduling of the supplied messages.

This function is called by ScheduleL() and DoReScheduleL(). The supplied messages
are assumed to have the values for the following data - MTM UID, off-peak flag,
schedule date and pending conditions flag. In debug mode, if these info is 
matching in all the messages then a panic will occur.

@param	aSchEntries
An array with the messages to be scheduled.

@param	aCommandId
The command id that must be used by the Send.Exe to eventually send the messages.

@param	aFinalState
The sending state to set to the messages to if the message is successfully 
scheduled.

@param	aTime
For conditions-scheduled messages this is the timeout, for time-scheduled messages
this is the scheduled time.

@param 	aPendingConditions
A flag indicating whether the schedule should be pending conditions.
*/
void CMsvScheduleSend::DoScheduleL(CMsvScheduledEntries& aSchEntries, const TInt aFinalState, const TTime& aTime, TBool aPendingConditions)
	{
	__ASSERT_DEBUG(aSchEntries.Count(), gPanic(EMessageSelectionEmpty));
	
#ifdef _DEBUG
	// Check that the mtm, time, off peak flag and pending conditions flag are 
	// the same for every message.
	TInt count = aSchEntries.Count();
	CMsvScheduledEntry* schEntry = aSchEntries[0];

	while (count--)
		{
		CMsvScheduledEntry* entry = aSchEntries[count];

		__ASSERT_DEBUG(entry->Mtm() == schEntry->Mtm(), gPanic(EMessagesNotSameMtm));
		__ASSERT_DEBUG((entry->OffPeak() && schEntry->OffPeak()) || (!entry->OffPeak() && !schEntry->OffPeak()), gPanic(EMessagesNotSameOffPeak));
		__ASSERT_DEBUG(entry->ScheduleDate() == schEntry->ScheduleDate(), gPanic(EMessagesNotSameTime));
		__ASSERT_DEBUG(entry->PendingConditions() == schEntry->PendingConditions(), gPanic(EMessagesNotSamePendingConditions));
		schEntry = entry;
		}
#endif

	// Connect and register with the task scheduler
	ConnectAndRegisterL();
	
	// Determine the start time and validity period
	TTime latencyTime;
	latencyTime.UniversalTime();
	latencyTime += iSettings->Latency();
	TTime startTime = latencyTime;

	if( aTime > startTime )
		{
		// The schedule time is in the future or there is no schedule time
		// (the messages are pending conditions) - update the start time.
		startTime = aTime;
		}

	TTimeIntervalMinutes valPeriod = iSettings->ValidityPeriod();
	CMsvScheduledEntry* firstMessage = aSchEntries[0];

	// Determine the start time and validity period of the new schedule - this
	// is not applicable if the messages are schedule for pending conditions.
	if( firstMessage->OffPeak() && !aPendingConditions )
		{
		GetOffPeakL(startTime, startTime, valPeriod);
		}

	CArrayFixFlat<TTaskSchedulerCondition>* schConditions = new (ELeave) CArrayFixFlat<TTaskSchedulerCondition>(KMsvSchsendArrayGrowth);
	CleanupStack::PushL(schConditions);
	if( aPendingConditions )
		PopulateScheduleConditionsL(*schConditions);

	// Create a schedule
	TSchedulerItemRef schItemRef;
	TBool schFound = EFalse;

	// Check to see if a schedule already exists for this particular schedule.
	// Need to consider both the start time and if the schedule is for pending
	// conditions met.
	// NOTE - if the schedule time is not in the future, then there is no need 
	// to search for an existing schedule.	
	if( startTime > latencyTime )
		{
		if( startTime != Time::MaxTTime() )
			RoundUpToMinute(startTime);
		
		TRAPD(err,FindScheduleL(startTime, *schConditions, aPendingConditions, schItemRef));
		if( err == KErrNone )
			{
			SCHSENDLOG(FLog(_L("\tFound schedule %d"), schItemRef.iHandle));
			schFound = ETrue;
			}
		// Else ignore the error and create and new schedule.
		}

	if( !schFound )
		{
		// The appropriate schedule.
		CreateScheduleL(startTime, valPeriod, *schConditions, aPendingConditions, schItemRef);
		}

	CleanupStack::PopAndDestroy(schConditions);
	
	// Disable the schedule so that it doesn't fire while scheduling
	User::LeaveIfError(iScheduler.DisableSchedule(schItemRef.iHandle));

	// Schedule the messages
	const TInt messageCount = aSchEntries.Count();
	TInt schErr = KErrNone;
	TInt curMsg;

	for( curMsg = 0; curMsg < messageCount; ++curMsg ) // must forward traverse the array
		{
		CMsvScheduledEntry* message = aSchEntries[curMsg];

		message->SetScheduleDate(startTime);
		TRAP(schErr, ScheduleEntryL(*message, aFinalState, startTime, schItemRef, aPendingConditions));

		if( schErr != KErrNone )
			break;
		}

	if( schErr != KErrNone && curMsg == 0 && !schFound )
		{
		iScheduler.DeleteSchedule(schItemRef.iHandle); // ignore error
		}
	else
		{
		User::LeaveIfError(iScheduler.EnableSchedule(schItemRef.iHandle));
		}
		
	User::LeaveIfError(schErr);
	}

/**
Creates a new schedule on the task scheduler with which each message can be 
associated. 

The schedule is triggered by a start time being reached.

@see RScheduler::CreatePersistentSchedule

@param aScheduler
Handle to scheduler to update.

@param aSettings
Scheduler settings.

@param aStartTime
Schedule start time.

@param aValidityPeriod
Schedule validity period.

@param aRef 
On return, the new schedule.
*/
EXPORT_C void CMsvScheduleSend::CreateScheduleL(RScheduler& aScheduler, const CMsvScheduleSettings& aSettings, const TTime& aStartTime, const TTimeIntervalMinutes& aValidityPeriod, TSchedulerItemRef& aRef)
	{
	//Determine the new schedule entry	
	TScheduleEntryInfo2 taskInfo;
	taskInfo.SetIntervalType(aSettings.IntervalType());
	taskInfo.SetInterval(1);
	TTsTime startTime(aStartTime,ETrue);
	taskInfo.SetStartTime(startTime);
	taskInfo.SetValidityPeriod(aValidityPeriod);
	
	//Create the TScheduleEntryInfo arrag that is required by CreatePersistentSchedule
	CArrayFixFlat<TScheduleEntryInfo2>* entryInfos = new (ELeave) CArrayFixFlat<TScheduleEntryInfo2>(1);
	CleanupStack::PushL(entryInfos);

	//Append the new schedule entry
	entryInfos->AppendL(taskInfo);

	//Create the schedule
	User::LeaveIfError(aScheduler.CreatePersistentSchedule(aRef, *entryInfos));

	CleanupStack::PopAndDestroy(entryInfos);
	}

/**
Creates a new schedule on the task scheduler with which each message can be 
associated.

The schedule is triggered by either a set of conditions being met or a timeout
being reached.

@see RScheduler::CreatePersistentSchedule
@see TSysAgentCondition

@param aScheduler
Handle to scheduler to update.

@param aConditions
The set of System Agent conditions that are required to be met to trigger the
schedule.

@param aTimeout
The timeout value for the schedule.

@param aRef 
On return, the new schedule.
*/
EXPORT_C void CMsvScheduleSend::CreateScheduleL(
								RScheduler& 									aScheduler, 
								const CArrayFixFlat<TTaskSchedulerCondition>&	aConditions, 
								const TTime& 									aTimeout,
								TSchedulerItemRef& 								aRef)
	{
	TTsTime timeout(aTimeout,ETrue);
	User::LeaveIfError(aScheduler.CreatePersistentSchedule(aRef, aConditions, timeout));	
	}
		
void CMsvScheduleSend::CreateScheduleL(const TTime& aTime, const TTimeIntervalMinutes& aValidityPeriod, const CArrayFixFlat<TTaskSchedulerCondition>& aSchConditions, TBool aPendingConditions, TSchedulerItemRef& aRef)
	{
	if( aPendingConditions )
		{
		CreateScheduleL(iScheduler, aSchConditions, aTime, aRef);
	#ifndef _MSG_NO_LOGGING
		TBuf<32> bufDate;
		aTime.FormatL(bufDate, _L("%D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B"));
		SCHSENDLOG(FLog(_L("\tCreated Schedule %d for pending %d conditions or %S"), aRef.iHandle, aSchConditions.Count(), &bufDate));
	#endif		
		}
	else
		{		
		CreateScheduleL(iScheduler, *iSettings, aTime, aValidityPeriod, aRef);
	#ifndef _MSG_NO_LOGGING
		TBuf<32> bufDate;
		aTime.FormatL(bufDate, _L("%D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B"));
		SCHSENDLOG(FLog(_L("\tCreated Schedule %d for %S"), aRef.iHandle, &bufDate));
	#endif
		}
	}
	
/**
Adds an entry to an existing schedule.

@see RScheduler::ScheduleTask()

@param aScheduler 
Scheduler to access.

@param aRef 
Id of the schedule.

@param aPackage 
Scheduler settings.

@param aInfo 
Information about the entry to be added to the schedule.

@leave KErrNotFound
No existing schedule with the specified Id.
*/
EXPORT_C void CMsvScheduleSend::ScheduleEntryL(RScheduler& aScheduler, const TSchedulerItemRef& aRef, const TMsvSchedulePackage& aPackage, TTaskInfo& aInfo)
	{
	aInfo.iPriority	= EDefaultTaskPriority;
	aInfo.iRepeat	= EDefaultTaskRepeat;

	HBufC* hBuf = NULL;
	aPackage.PackLC(aInfo, hBuf);

	//Schedule the task
	User::LeaveIfError(aScheduler.ScheduleTask(aInfo, *hBuf, aRef.iHandle));

	CleanupStack::PopAndDestroy(hBuf);
	}

/**
Schedules aMessage on the schedule referred to by aRef.

Updates the message entry and the schedule data using the utility function 
UpdateEntryAfterSchedule().

@see	CMsvScheduleSend::UpdateEntryAfterSchedule

@param	aMessage
The message to be scheduled.

@param	aFinalState
The sending state to set to the messages to if the message is successfully 
scheduled.

@param	aFromTime
The time to schedule the messages for sending.

@param	aRef
The ID of the schedule to add this task to.

@param	aPendingConditions
A flag indicating whether this message is schedule for conditions.
*/
void CMsvScheduleSend::ScheduleEntryL(CMsvScheduledEntry& aMessage, const TInt aFinalState, const TTime& aStartTime, const TSchedulerItemRef& aRef, TBool aPendingConditions)
	{
	//Create a new task to associate with the schedule
	TTaskInfo taskInfo;
	iPackage.iId = aMessage.Id();
	
	ScheduleEntryL(iScheduler, aRef, iPackage, taskInfo);

	// Change the scheduled flag and sending state of the message
	if( iServerEntry.SetEntry(aMessage.Id()) == KErrNone )
		{
		TMsvEntry entry(iServerEntry.Entry());

		aMessage.Entry(entry);
		UpdateEntryAfterSchedule(aRef, taskInfo, aStartTime, aFinalState, entry, aMessage.iData);
		entry.SetPendingConditions(aPendingConditions);

		SCHSENDLOG(FLog(_L8("\t\tScheduled msg %d (Mtm=%d, State=%d, Sch=%d, Task=%d, Pending=%d)"), entry.Id(), entry.iMtm.iUid, aFinalState, aMessage.iData.iRef.iHandle, aMessage.iData.iTaskId, entry.PendingConditions()));

		User::LeaveIfError(iServerEntry.ChangeEntry(entry));

		//Store the message data
		CMsvStore* store = iServerEntry.EditStoreL();
		CleanupStack::PushL(store);
		aMessage.StoreL(*store);
		store->CommitL();
		CleanupStack::PopAndDestroy(store);
		iServerEntry.SetEntry(KMsvNullIndexEntryId);
		}
	}

/**
Utility function that updates message index entry fields to reflect 
the message's scheduling.

@param aRef 
Scheduler item.

@param aInfo 
Scheduler task information.

@param aTime 
Schedule start time.

@param aFinalState 
Sending state flag.

@param aEntry 
On return, updated index entry.

@param aData 
On return, populated messaging scheduling data.
*/
EXPORT_C void CMsvScheduleSend::UpdateEntryAfterSchedule(const TSchedulerItemRef& aRef, const TTaskInfo& aInfo, const TTime& aTime, TInt aFinalState, TMsvEntry& aEntry, TMsvEntryScheduleData& aData)
	{
	aEntry.SetScheduled(ETrue);
	aEntry.SetFailed(EFalse);
	aEntry.SetSendingState(aFinalState);
	aEntry.SetConnected(EFalse);
	aEntry.iDate = aTime;
	aData.iRef = aRef;
	aData.iTaskId = aInfo.iTaskId;
	}

/**
Sorts the supplied array of messages.

The messages are sorted by their schedule date (held in the iEntry.iDate member
of CMsvScheduledEntry). Messages with the same schedule time are then further
sorted by if they to be scheduled for pending conditions.

@param	aSchEntries
The input/output parameter that holds the messages to be sorted. This will 
contain the sorted list once the function completes.
*/
void CMsvScheduleSend::SortByDateAndPendingConditionsL(CMsvScheduledEntries& aSchEntries)
	{
	//TO DO: Test this function!!!

	TInt count = aSchEntries.Count();
	CMsvScheduledEntry* curEntry = NULL;
	CMsvScheduledEntry* compEntry = NULL;

	for (TInt i = 0; i < count - 1; ++i)
		{
		curEntry = aSchEntries[i];

		for (TInt j = i + 1; j < count; ++j)
			{
			compEntry = aSchEntries[j];

			//Compare the dates of the two entries
			if( curEntry->ScheduleDate() > compEntry->ScheduleDate() ||
				(curEntry->ScheduleDate() == compEntry->ScheduleDate() &&
				curEntry->PendingConditions() && !compEntry->PendingConditions()) )
				{
				//Swap them
				aSchEntries.Delete(i);
				aSchEntries.InsertL(i, compEntry);
				aSchEntries.Delete(j);
				aSchEntries.InsertL(j, curEntry);
				}
			}
		}
	}

void CMsvScheduleSend::GetMessagesL(const CMsvEntrySelection& aSelection)
	{
	iSchEntries->ResetAndDestroy();

	const TInt entryCount = aSelection.Count();

	iSchEntries->SetReserveL(entryCount); //so following AppendL()s won't leave

	CMsvScheduledEntry* schEntry = NULL;

	for (TInt curMsg = 0; curMsg < entryCount; ++curMsg) //not while because must transverse forward
		{
		//Retrieve each message from the message server
		TMsvId msvId = aSelection.At(curMsg);

		TRAPD(error, schEntry = GetMessageL(msvId));

		if (error == KErrNone)
			{
			CleanupStack::PushL(schEntry);
			iSchEntries->AppendL(schEntry);
			CleanupStack::Pop(schEntry);
			}
		else if (error != KErrLocked && error != KErrNotFound)
			{
			User::Leave(error);
			}
		}
	}

/**
Connects to and registers with the task scheduler.

@param aScheduler 
Handle to the scheduler to connect to.

@param aSettings 
Schedule settings.

@leave Any error code
RScheduler::Connect() returned an error.

@leave Any error code
RFs::Connect() returned an error.

@leave KErrPathNotFound
The .EXE file to be called by the Task Scheduler when messages are due to 
be sent cannot be found. The filename is part of the schedule settings.

@leave Any error code
RScheduler::Register() returned an error.
*/

EXPORT_C void CMsvScheduleSend::ConnectAndRegisterL(RScheduler& aScheduler, const CMsvScheduleSettings& aSettings)
	{
	//Connect to the task scheduler
	User::LeaveIfError(aScheduler.Connect());

	// Register the schsendexe.exe as the executable to run when the schedule
	// fires.
	User::LeaveIfError(aScheduler.Register(KSchSendExe(), aSettings.Priority()));
	}


void CMsvScheduleSend::ConnectAndRegisterL()
	{
	//Check to see whether already registered.
	if (iRegistered)
		{
		return;
		}

	ConnectAndRegisterL(iScheduler, *iSettings);

	//No errors, so set iRegistered to true.
	iRegistered = ETrue;
	}

/*
	Retrieves the system off-peak times and
	updates aStartTime and aValidityPeriod
*/

void CMsvScheduleSend::GetOffPeakL(TTime aFromTime, TTime& aStartTime, TTimeIntervalMinutes& aValidityPeriod) const
	{
	__ASSERT_DEBUG(iOffPeakTimes->Count() > 0, gPanic(EOffPeakTimesEmpty));
	
	TMsvOffPeakTime opTime;

	TTime offPeakStart;
	User::LeaveIfError(iOffPeakTimes->GetNextOffPeakTime(aFromTime, opTime, offPeakStart));

	aValidityPeriod = opTime.ValidityPeriod();

	if (offPeakStart < aFromTime) 
		{ //aFromTime is within an off peak time
		aStartTime = aFromTime;

		TTimeIntervalMinutes mins;
		aStartTime.MinutesFrom(offPeakStart, mins);
		TInt minsInt = mins.Int();

		__ASSERT_DEBUG(minsInt >= 0, gPanic(EInvalidValidityPeriod));

		aValidityPeriod = opTime.ValidityPeriod();

		aValidityPeriod = (TTimeIntervalMinutes) (aValidityPeriod.Int() - minsInt);
		}
	else
		{
		aStartTime = offPeakStart;
		}

	//To Do: Remove next line after testing
	__ASSERT_DEBUG(aValidityPeriod.Int() > 0, gPanic(EInvalidValidityPeriod));
	}

void CMsvScheduleSend::PopulateScheduleConditionsL(CArrayFixFlat<TTaskSchedulerCondition>& aSchConditions)
	{
	aSchConditions.Reset();

	TInt count = iAgentActions->Count();
	TTaskSchedulerCondition condition;
	
	// All system agent condition have a UID of KUidSystemCategory.
	condition.iCategory = KUidSystemCategory;
	
	for( TInt i = 0; i < count; ++i )
		{

		TMsvCondition agentCondition = iAgentActions->At(i).iCondition;

		condition.iKey	 = agentCondition.iVariable.iUid;
		condition.iState = agentCondition.iState;
		
		switch( agentCondition.iType )
			{
		case TMsvCondition::EMsvSchSendEquals:
			condition.iType = TTaskSchedulerCondition::EEquals;
			break;
		case TMsvCondition::EMsvSchSendNotEquals:
			condition.iType = TTaskSchedulerCondition::ENotEquals;
			break;
		case TMsvCondition::EMsvSchSendGreaterThan:
			condition.iType = TTaskSchedulerCondition::EGreaterThan;
			break;
		case TMsvCondition::EMsvSchSendLessThan:
			condition.iType = TTaskSchedulerCondition::ELessThan;
			break;
		default:
			User::Invariant();
			break;
			}
			
		aSchConditions.AppendL(condition);
		}
	}
	
_LIT(KScheduleSendPanic, "ScheduleSend-DLL");

GLDEF_C void gPanic(TScheduleSendPanic aPanic)
	{
	User::Panic(KScheduleSendPanic,aPanic);
	}


#ifndef _MSG_NO_LOGGING
void CMsvScheduleSend::FLog(TRefByValue<const TDesC> aFormat, ...)
	{
	VA_LIST list;
	VA_START(list, aFormat);
	RFileLogger::WriteFormat(KSchSendLogDir, KSchSendLogFile, EFileLoggingModeAppend,
		aFormat, list);
	}

void CMsvScheduleSend::FLog(TRefByValue<const TDesC8> aFormat, ...)
	{
	VA_LIST list;
	VA_START(list, aFormat);
	RFileLogger::WriteFormat(KSchSendLogDir, KSchSendLogFile, EFileLoggingModeAppend,
		aFormat, list);
	}
#endif
