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

#include <msvids.h>
#include <msventry.h>
#include <msvschedulesettings.h>
#include "pigeonservermtm.h"

#include <msvoffpeaktime.h>
#include <flogger.h>
#include <msvuids.h>
#include <msvsysagentaction.h>

#include <tmsvschedulesettingsutils.h>
#include <centralrepository.h>

_LIT(KPigeonLogFile, "pigeon.txt");

CPigeonServerMtm::CPigeonServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry) : CScheduleBaseServerMtm(aRegisteredMtmDll, aServerEntry)
	{
	}

CPigeonServerMtm::~CPigeonServerMtm()
	{
	delete iScheduleSend;
	}

void CPigeonServerMtm::ConstructL()
	{
	iScheduleSend = CPigeonScheduledSend::NewL(*iServerEntry);
	
	// Get the entry id for the pigeon service entry.
	User::LeaveIfError(iServerEntry->SetEntry(KMsvRootIndexEntryId));
	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection();
	CleanupStack::PushL(sel);
	User::LeaveIfError(iServerEntry->GetChildrenWithMtm(KUidMsgTypePigeon, *sel));
	TInt count = sel->Count();
	if( count > 1 )	// should only be one service entry
		User::Leave(KErrCorrupt);
	if( count == 0 )
		{
		// Create the settings
		TMsvEntry serviceEntry;
		serviceEntry.iType= KUidMsvServiceEntry;
		serviceEntry.iMtm = KUidMsgTypePigeon;

		User::LeaveIfError(iServerEntry->CreateEntry(serviceEntry));
		iServiceId = serviceEntry.Id();		
		}
	else
		{
		iServiceId = sel->At(0);
		}
	CleanupStack::PopAndDestroy(sel);
	User::LeaveIfError(iServerEntry->SetEntry(KMsvNullIndexEntryId));	
	iHeapFailure = EFalse;
	}

EXPORT_C CPigeonServerMtm* CPigeonServerMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry& aServerEntry)
	{
	CPigeonServerMtm* self = new (ELeave) CPigeonServerMtm(aRegisteredMtmDll, &aServerEntry);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

const TDesC8&	CPigeonServerMtm::Progress()
	{
	RFileLogger::Write(KSchSendLogDir, KPigeonLogFile, EFileLoggingModeAppend,
		_L("CPigeonServerMtm::Progress()"));

	return iProgress;
	}

void CPigeonServerMtm::ScheduleL(CMsvEntrySelection& aSelection, const TBool /*aMove*/, const TDesC8& aParameter, TRequestStatus& /*aStatus*/)
	{
	TMsvSchedulePackage package;
	package.iCommandId = ESendScheduledL;
	package.iParameter = aParameter;
	iScheduleSend->ScheduleL(aSelection, package);
	}
	
void CPigeonServerMtm::RestoreSettingsL()
	{
	// Restore the system agent actions/conditions and the pending conditions
	// timeout value - these are the only ones that pigeon clients can set.
	CRepository* repository = CRepository::NewLC(KUidMsgTypePigeon);
	iScheduleSend->LoadSysAgentActionsL(*repository);
	iScheduleSend->LoadPendingConditionsTimeoutL(*repository);
	CleanupStack::PopAndDestroy(repository);
	}

void CPigeonServerMtm::SendScheduledL(CMsvEntrySelection& aSelection, const TBool /*aMove*/, const TDesC8& /*aParameter*/, TRequestStatus& /*aStatus*/)
	{
	TBool leave = EFalse;
	for ( TInt i = 0; i < aSelection.Count(); ++i )
		{
		// Move message to the sent folder, and update sending state to Sent.
		TInt err = iServerEntry->SetEntry(aSelection[i]);
		TMsvEntry entry = iServerEntry->Entry();
		
		// Check each message to see if it should be sent...
		if( entry.iError == KPigeonErrFailFirstSend )
			{
			leave = ETrue;
			}
		else
			{
			entry.SetSendingState(KMsvSendStateSent);
			entry.iDetails.Set(KSchSendTestDetails);
			err = iServerEntry->ChangeEntry(entry);
			TMsvId id = entry.Id();
			err = iServerEntry->SetEntry(KMsvSentEntryId);
			if(KErrNone == err)
				{
				err = iServerEntry->SetEntry(KMsvGlobalOutBoxIndexEntryId);
				if(KErrNone == err)
					{
					// Move it...
					err = iServerEntry->MoveEntryWithinService(id, KMsvSentEntryId);
					}
				}
			}
		}
	iServerEntry->SetEntry(KMsvNullIndexEntryId);
	
	if( leave )
		User::Leave(KErrDisconnected);
	}
	
void CPigeonServerMtm::SetFailFirstSendL(CMsvEntrySelection& aSelection)
	{
	for( TInt i = 0; i < aSelection.Count(); ++i )
		{
		// Copy message to the sent folder, but update sending state to Sent.
		TInt err = iServerEntry->SetEntry(aSelection[i]);
		TMsvEntry entry = iServerEntry->Entry();
		
		// Set the error...
		entry.iError = KPigeonErrFailFirstSend;
		err = iServerEntry->ChangeEntry(entry);
		}
	iServerEntry->SetEntry(KMsvNullIndexEntryId);
	}
	
void CPigeonServerMtm::StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus)
	{
	RestoreSettingsL();
	
	iProgress().SetCommand(aCommand);
	const TMsvId id = iServerEntry->Entry().Id();
	
	RFileLogger::WriteFormat(KSchSendLogDir, KPigeonLogFile, EFileLoggingModeAppend,
		_L("CPigeonServerMtm::StartCommandL(aSelection.Count() == %d, aCommand == %d, aParamter.Length() == %d)"),
		aSelection.Count(),
		aCommand,
		aParameter.Length());

	TRAP(iProgress().iError, DoStartCommandL(aSelection, iProgress().Command(), aParameter, aStatus));
	
	TRequestStatus* bStatus = &aStatus;
	User::RequestComplete(bStatus, iProgress().iError);
	iServerEntry->SetEntry(id); //ignore error
	}

void CPigeonServerMtm::DoStartCommandL(CMsvEntrySelection& aSelection, TSchSendTestOperation aCommand, const TDesC8& aParameter, TRequestStatus& aStatus)
	{

	if(iHeapFailure) 
		__UHEAP_FAILNEXT(iNextFailure++);
	else
		__UHEAP_RESET;
	
	TMsvSchedulePackage package;
	package.iCommandId = ESendScheduledL;
	package.iParameter = aParameter;

	switch(aCommand)
		{
		case EOpFail:
			{
			TPckgC<TInt> pkg(0);
			pkg.Set(aParameter);
			User::Leave(pkg());
			}
			break;
		case EScheduleOpFail:
			{
			package.iCommandId = EOpFail;
			//pass through
			}
		case EScheduleAllL:
			{
			iScheduleSend->ScheduleL(aSelection, package);
			UpdateProgressL(aSelection);
			} break;
		case EReScheduleRetryAllL:
			{
			// Change the iCommandId to do a re-schedule
			package.iCommandId = EReScheduleRetryAllL;
			iScheduleSend->ReScheduleL(aSelection, package);
			UpdateProgressL(aSelection);
			
			// Bit of a hack here!! Check the selection to see if the messages
			// have been re-scheduled. If they have not been re-scheduled, then
			// copy to the Sent folder...
			for( TInt i=0; i<aSelection.Count(); ++i )
				{
				User::LeaveIfError(iServerEntry->SetEntry(aSelection[i]));
				TMsvEntry entry = iServerEntry->Entry();
				
				if( entry.SendingState() == KMsvSendStateWaiting )
					{
					User::LeaveIfError(iServerEntry->SetEntry(KMsvSentEntryId));
					User::LeaveIfError(iServerEntry->CreateEntry(entry));
					}
				}
			User::LeaveIfError(iServerEntry->SetEntry(KMsvNullIndexEntryId));
			} break;
		case EReScheduleAllL:
			{
			// Pass through action to be performed on reschedule. (Default is to do nothing.)
			TMsvSendErrorAction laterAction;
			laterAction.iAction = ESendActionRetryImmediately;
			laterAction.iRetries = ESendRetriesFixed;
			laterAction.iRetrySpacing = ESendRetrySpacingStatic;
	
			iScheduleSend->ReScheduleL(aSelection, package, &laterAction);
			UpdateProgressL(aSelection);
			} break;
		case EDeleteScheduleL:
			{
			iScheduleSend->DeleteScheduleL(aSelection);
			} break;
		case ESendScheduledL:
			{
			SendScheduledL(aSelection, aCommand, aParameter, aStatus);
			} break;
		case ECheckScheduleL:
			{
			iScheduleSend->CheckScheduleL(aSelection);
			} break;
		case ESetRetryImmediately:
		case ESetRetryLater:
		case ESetRetryVariable:
		case ESetNoRetry:
			{
			iScheduleSend->SetupL(aCommand);

			CRepository* repository = CRepository::NewLC(KUidMsgTypePigeon);
			iScheduleSend->SaveSysAgentActionsL(*repository);
			CleanupStack::PopAndDestroy(repository);			
			} break;
		case ESetNowOffPeak:
		case ESetNowNotOffPeak:
		case ESetFirstOffPeakBest:
		case ESetLastOffPeakBest:
			{
			iScheduleSend->SetupL(aCommand);
			} break;
		case ESetIncrementalHeapFailure:
			{
			iHeapFailure = ETrue;
			// drop through
			}
		case EResetIncrementalHeapFailure:
			{
			iNextFailure = 0;
			} break;
		case ENoIncrementalHeapFailure:
			{
			iHeapFailure = EFalse;
			__UHEAP_RESET;
			} break;
		case EScheduleFailFirstSend:
			{
			SetFailFirstSendL(aSelection);

			iScheduleSend->ScheduleL(aSelection, package);
			UpdateProgressL(aSelection);
			}break;
		default:
			User::Panic(_L("pigeon server"), 1);
			break;
		}

	if(iHeapFailure)
		{
		__UHEAP_RESET;
		}

	}

void CPigeonServerMtm::UpdateProgressL(const CMsvEntrySelection& aSelection)
	{
	RFileLogger::WriteFormat(KSchSendLogDir, KPigeonLogFile, EFileLoggingModeAppend,
		_L("CPigeonServerMtm::UpdateProgressL(aSelection.Count() == %d)"),
		aSelection.Count());

	//Uses Progess to return the time that the task scheduler
	//reports that the first message in the selection is scheduled for

	if (aSelection.Count()==0) return;
 
	//Make a big load of reference parameters
	TInt size = 0;

	RScheduler scheduler;
	User::LeaveIfError(scheduler.Connect());
	CleanupClosePushL(scheduler);
	CMsvScheduledEntry* entry = ScheduledEntryLC(aSelection[0]);
	const TInt taskId = entry->iData.iTaskId;
	const TInt error = scheduler.GetTaskDataSize(taskId, size);

	if(error == KErrNone)
		{
		TTaskInfo info;
		HBufC* buf = HBufC::NewLC(size);
		TPtr ptr(buf->Des());
		TSchedulerItemRef ref;
		TTime time;

		User::LeaveIfError(scheduler.GetTaskInfoL(taskId, info, ptr, ref, time)); 
		
		iProgress().iTime = time;
		CleanupStack::PopAndDestroy(buf);
		}
	else if (error == KErrNotFound)
		{
		iProgress().iTime = entry->ScheduleDate();
		}
	else
		User::Leave(error);

	CleanupStack::PopAndDestroy(2); //entry, scheduler
	
	}

CMsvScheduledEntry* CPigeonServerMtm::ScheduledEntryLC(TMsvId aId)
	{
	CMsvScheduledEntry* entry = iScheduleSend->GetMessageL(aId);
	CleanupStack::PushL(entry);
	return entry;
	}
	
CMsvStore* CPigeonServerMtm::GetServiceEntryEditStoreLC()
	{
	User::LeaveIfError(iServerEntry->SetEntry(iServiceId));	
	CMsvStore* store = iServerEntry->EditStoreL();
	CleanupStack::PushL(store);
	User::LeaveIfError(iServerEntry->SetEntry(KMsvNullIndexEntryId));	
		
	return store;
	}
	

EXPORT_C CPigeonScheduledEntry*	CPigeonScheduledEntry::NewL(const TMsvEntry& aMsvEntry)
	{
	CPigeonScheduledEntry* pse = new (ELeave) CPigeonScheduledEntry(aMsvEntry);
	CleanupStack::PushL(pse);
	pse->ConstructL();
	CleanupStack::Pop();
	return pse;
	}

void CPigeonScheduledEntry::ConstructL()
	{

	}

EXPORT_C CPigeonScheduledSend* CPigeonScheduledSend::NewL(CMsvServerEntry& aServerEntry)
	{
	CPigeonScheduledSend* ss = new (ELeave) CPigeonScheduledSend(aServerEntry);
	CleanupStack::PushL(ss);
	ss->ConstructL();
	CleanupStack::Pop();
	return ss;
	}

void CPigeonScheduledSend::ConstructL()
	{
	CMsvScheduleSend::ConstructL();

	//Lists of all the possible settings, actions and offpeak times
	//which can be set dynamically by Setup()
	PopulateErrorListL();
	PopulateSettingsL();
	iActionsList = new (ELeave) CArrayFixFlat<TMsvSendErrorAction>(20);
	PopulateActionsListL();
	iOffPeakList = new (ELeave) CArrayFixFlat<TMsvOffPeakTime>(20);
	PopulateOffPeakListL();
	}

//Puts the existing settings and some new settings in the list of settings
//Now both will be deleted on exit.
void CPigeonScheduledSend::PopulateSettingsL()
	{
	delete iSettings;
	iSettings = NULL;

	//Make some shiny new settings instead
	iSettings = CMsvScheduleSettings::NewL();
	iSettings->SetShortInterval(KShortInterval);
	iSettings->SetLongInterval(KLongInterval);

	CArrayFixFlat<TTimeIntervalSeconds>* intervals = new(ELeave)CArrayFixFlat<TTimeIntervalSeconds>(20);
	CleanupStack::PushL(intervals);
	intervals->AppendL(KFirstInterval);
	intervals->AppendL(KSecondInterval);
	intervals->AppendL(KThirdInterval);

	iSettings->SetVariableIntervalsL(*intervals);  // copies intervals

	CleanupStack::PopAndDestroy();  //intervals
//	iSettingsList->AppendL(custom);
	}

void CPigeonScheduledSend::PopulateActionsListL()
	{
	iActionsList->Reset();
	iActionsList->AppendL(iErrorActions->Default());
	
	TMsvSendErrorAction immediatelyAction;
	immediatelyAction.iAction = ESendActionRetryImmediately;
	immediatelyAction.iRetries = ESendRetriesFixed;
	immediatelyAction.iRetrySpacing = ESendRetrySpacingStatic;

	iActionsList->AppendL(immediatelyAction);

	TMsvSendErrorAction laterAction;
	laterAction.iAction = ESendActionRetryLater;
	laterAction.iRetries = ESendRetriesFixed;
	laterAction.iRetrySpacing = ESendRetrySpacingStatic;

	iActionsList->AppendL(laterAction);

	TMsvSendErrorAction variableAction;
	variableAction.iAction = ESendActionRetryLater;
	variableAction.iRetries = ESendRetriesFixed;
	variableAction.iRetrySpacing = ESendRetrySpacingVariable;

	iActionsList->AppendL(variableAction);

	TMsvSendErrorAction failAction;
	failAction.iAction = ESendActionFail;
	failAction.iRetries = ESendRetriesFixed;
	failAction.iRetrySpacing = ESendRetrySpacingStatic;

	iActionsList->AppendL(failAction);
	}

void CPigeonScheduledSend::PopulateErrorListL()
	{
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	
	TMsvSendErrorAction immediatelyAction;
	immediatelyAction.iAction = ESendActionRetryImmediately;
	immediatelyAction.iRetries = ESendRetriesFixed;
	immediatelyAction.iRetrySpacing = ESendRetrySpacingStatic;
	immediatelyAction.iError = KErrorActionImmediately;
	
	errorActions->AddSendErrorActionL(immediatelyAction);

	TMsvSendErrorAction laterAction;
	laterAction.iAction = ESendActionRetryLater;
	laterAction.iRetries = ESendRetriesFixed;
	laterAction.iRetrySpacing = ESendRetrySpacingStatic;
	laterAction.iError = KErrorActionLater;

	errorActions->AddSendErrorActionL(laterAction);

	TMsvSendErrorAction variableAction;
	variableAction.iAction = ESendActionRetryLater;
	variableAction.iRetries = ESendRetriesFixed;
	variableAction.iRetrySpacing = ESendRetrySpacingVariable;
	variableAction.iError = KErrorActionVariable;

	errorActions->AddSendErrorActionL(variableAction);

	TMsvSendErrorAction failAction;
	failAction.iAction = ESendActionFail;
	failAction.iRetries = ESendRetriesFixed;
	failAction.iRetrySpacing = ESendRetrySpacingStatic;
	failAction.iError = KErrorActionFail;

	errorActions->AddSendErrorActionL(failAction);
	
	TMsvSendErrorAction conditionsAction;
	conditionsAction.iAction = ESendActionRetryConditionMet;
	conditionsAction.iRetries = ESendRetriesFixed;
	conditionsAction.iRetrySpacing = ESendRetrySpacingStatic;
	conditionsAction.iError = KErrorActionConditions;

	errorActions->AddSendErrorActionL(conditionsAction);	

	TMsvSendErrorAction retryfailAction;
	retryfailAction.iAction = ESendActionRetryImmediately;
	retryfailAction.iRetries = ESendRetriesFixed;
	retryfailAction.iRetrySpacing = ESendRetrySpacingStatic;
	retryfailAction.iError = KErrorActionRetryFail;
	retryfailAction.SetMaxRetries(1);

	errorActions->AddSendErrorActionL(retryfailAction);	

	delete iErrorActions;
	iErrorActions = errorActions;
	CleanupStack::Pop(errorActions);		
	}

void CPigeonScheduledSend::PopulateOffPeakListL()
	{
	//There are no existing off peak times set up
	//So no need to remember them
	iOffPeakList->Reset();

	TTime t;
	TDateTime d;
	t.HomeTime();
	t -= TTimeIntervalHours(1);
	d = t.DateTime();

	TTimeIntervalMinutes twoHours = 120;

	TMsvOffPeakTime current(t.DayNoInWeek(), d.Hour(), d.Minute(), twoHours);
	iOffPeakList->AppendL(current);

	t -= TTimeIntervalDays(1);
	d = t.DateTime();
	TMsvOffPeakTime yesterday(t.DayNoInWeek(), d.Hour(), d.Minute(), twoHours);
	iOffPeakList->AppendL(yesterday);

	t += TTimeIntervalDays(2);
	d = t.DateTime();
	TMsvOffPeakTime tomorrow(t.DayNoInWeek(), d.Hour(), d.Minute(), twoHours);
	iOffPeakList->AppendL(tomorrow);
	}

void CPigeonScheduledSend::SetupL(TSchSendTestOperation aOption)
	{
	if (aOption < 0) 
		User::Panic(_L("Pigeon"), 23);
	switch(aOption)
		{
		case ESetRetryImmediately:
			SetActionL(EActionImmediately);
			break;
		case ESetRetryLater:
			SetActionL(EActionLater);
			break;
		case ESetRetryVariable:
			SetActionL(EActionVariable);
			break;
		case ESetNoRetry:
			SetActionL(EActionFail);
			break;
		case ESetNowOffPeak:
			{
			iOffPeakTimes->ResizeL(0);
			TMsvOffPeakTime opt = (*iOffPeakList)[EOffPeakCurrent];
			iOffPeakTimes->AppendL(opt);
			break;
			}
		case ESetNowNotOffPeak:
			iOffPeakTimes->ResizeL(0);
			iOffPeakTimes->AppendL((*iOffPeakList)[EOffPeakFuture]);
			break;
		case ESetFirstOffPeakBest:
			iOffPeakTimes->ResizeL(0);
			iOffPeakTimes->AppendL((*iOffPeakList)[EOffPeakFuture]);
			iOffPeakTimes->AppendL((*iOffPeakList)[EOffPeakFinished]);
			break;
		case ESetLastOffPeakBest:
			iOffPeakTimes->ResizeL(0);
			iOffPeakTimes->AppendL((*iOffPeakList)[EOffPeakFinished]);
			iOffPeakTimes->AppendL((*iOffPeakList)[EOffPeakFuture]);
			break;
		default:
			User::Leave(KErrArgument);
		}
	}
	

void CPigeonScheduledSend::SaveSysAgentActionsL(CRepository& aRepository)
	{
	TMsvScheduleSettingsUtils::SaveSysAgentSettingsL(*iAgentActions, aRepository);	
	}
	
void CPigeonScheduledSend::LoadSysAgentActionsL(CRepository& aRepository)
	{
	TMsvScheduleSettingsUtils::LoadSysAgentSettingsL(*iAgentActions, aRepository);	
	}
	
void CPigeonScheduledSend::LoadPendingConditionsTimeoutL(CRepository& aRepository)
	{
	// Just load the pending conditions timeout - don't care about anything else.
	CMsvScheduleSettings* settings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(settings);
	TMsvScheduleSettingsUtils::LoadScheduleSettingsL(*settings, aRepository);
	iSettings->SetPendingConditionsTimeout(settings->PendingConditionsTimeout());
	CleanupStack::PopAndDestroy(settings);	
	}
	
void CPigeonScheduledSend::SetActionL(TActions aAction)
	{
	iAgentActions->iDefault = (*iActionsList)[aAction];
	}

CPigeonScheduledSend::~CPigeonScheduledSend()
	{
	delete iActionsList;
	delete iOffPeakList;
	}

CMsvScheduledEntry* CPigeonScheduledSend::GetMessageL(const TMsvId aId) const
	{
	const TMsvId id = iServerEntry.Entry().Id();
	iServerEntry.SetEntry(aId);
	CPigeonScheduledEntry* entry = CPigeonScheduledEntry::NewL(iServerEntry.Entry());
	CleanupStack::PushL(entry);

	if (iServerEntry.HasStoreL())
		{
		CMsvStore* store = iServerEntry.ReadStoreL();
		CleanupStack::PushL(store);

		//Restore the entry from the message's store.
		entry->RestoreL(*store);
		CleanupStack::PopAndDestroy(store);
		}

	CleanupStack::Pop(entry);
	iServerEntry.SetEntry(id); //ignore error
	return entry;
	}

TBool CPigeonScheduledEntry::CanSendToAnyRecipients(const TMsvSendErrorAction& aAction)
	{
	return (iData.Retries() < aAction.MaxRetries());
	}

TBool CPigeonScheduledEntry::CanSendToAnyRecipients(const CMsvSendErrorActions& aErrorActions, TMsvSendErrorAction& aAction)
	{
	if( aErrorActions.GetSendErrorAction(Error(), aAction) != KErrNone )
		{
		aAction = aErrorActions.Default();
		}
	return (iData.Retries() < aAction.MaxRetries());
	}

TBool CPigeonScheduledEntry::RecipientsAllSent() const
	{
	if( Error() == KErrorActionFail || Error() == KErrorActionRetryFail )
		return EFalse;
	return ETrue;
	}
