// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <csch_cli.h>
#include <msvapi.h>
#include <msvschedulepackage.h>
#include "SchSendExe.h"
#include <msvschedulesend.h>
#include <msvschedulesettings.h>
#include <msvids.h>
#include <msvuids.h>
#include <tmsvschedulesettingsutils.h>
#include <centralrepository.h>


#ifndef _MSG_NO_LOGGING
	#include <flogger.h>
	_LIT(KSchSendExeLogFile, "SchSendExe.txt");
	const TFileLoggingMode KSchSendExeLoggingMode = EFileLoggingModeAppend;
#endif


_LIT_SECURE_ID(KSchSendExeSid, 0x100056A1);
_LIT_SECURE_ID(KTaskSchedulerSid, 0x10005399);

_LIT(KSchSendThreadName, "SchSend");

#include <u32std.h>
#include <miutset.h>


GLDEF_C TInt E32Main()
	{
	if(User::CreatorSecureId() != KTaskSchedulerSid)
		{
		// Not launched by Task Scheduler - exit
		return KErrPermissionDenied;
		}

	TBuf<0x100> fileName;
	RFile taskFile;
 	// Adopt the task file using the RFs handle into environment  slot 
 	// 1 and the RFile handle into slot 2
 	TInt err = taskFile.AdoptFromCreator(1,2);
 	if(KErrNone !=err)
 		{
 		return err;
 		}
 	taskFile.Name(fileName);
 	return Execute(fileName, taskFile);
 	
	}



TInt Execute(const TDesC& aFileName, RFile& aFile )
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	SCHSENDLOG(CMsvSendExe::FLog(aFileName, _L("Processing file %S"), &aFileName));

	TRAPD(leaveValue, DoExecuteL(aFileName, aFile ));

	if (leaveValue != KErrNone)
		{
		// DoExecuteL will only leave in the unlikely event that available memory is too low
		// to create an instance of the active scheduler or the CMsvSendExe object or in the
		// event that we cannot rename the thread. Once the CMsvSendObject has been created
		// leaves are handled cleanly by the object. We could retry the call to DoExecuteL,
		// but it's unlikely to succeed a second time around.
		}

	SCHSENDLOG(CMsvSendExe::FLog(aFileName, _L("DoExecuteL completed with error %d"), leaveValue));

	delete cleanup;
	__UHEAP_MARKEND;
	
	return leaveValue;

	}

void DoExecuteL(const TDesC& aFileName, RFile& aFile )
	{
	CleanupClosePushL(aFile);

	User::LeaveIfError(User::RenameThread(KSchSendThreadName));

	CActiveScheduler* activeSch = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(activeSch);

	CActiveScheduler::Install(activeSch);

	CMsvSendExe::StartLC(aFileName, aFile );

	CleanupStack::PopAndDestroy(3, &aFile);
	}

//
// CMsvSendExe Implementation
//

/**

The CMsvSendExe Constructor

@return			
@internalAll
@param				 aFile
*/
CMsvSendExe::CMsvSendExe(const TDesC& aFileName, RFile& aFile )
	: CActive(KDefaultPriority), iFileName(aFileName), iTaskFile(aFile )
	{
	//Add this to the Active Scheduler.
	//This is a requirement if a class is derived from CActive
	CActiveScheduler::Add(this);
	iCreated.UniversalTime();
	}

CMsvSendExe::~CMsvSendExe()
/*
	CMsvSendExe Destructor
*/
	{
	Cancel();
	delete iSession;

	iOperations.ResetAndDestroy();
	iPackages.Reset();
	}


/**

The CMsvSendExe::StartLC method
1. Constructs CMsvSendExe
2. Processes the task scheduler file
3. Attempts to send the messages

@return			
@internalAll
@param				 aFile File containing the task information
*/
CMsvSendExe* CMsvSendExe::StartLC(const TDesC& aFileName, RFile& aFile )
	{
	CMsvSendExe* self = new (ELeave) CMsvSendExe(aFileName,aFile );

	CleanupStack::PushL(self);

	self->ConstructL();
	return self;
	}

void CMsvSendExe::ConstructL()
	{
	TRAPD(err, ProcessFileL());
	
	if(err == KErrPermissionDenied)
		{
		//security violation
		User::Leave(err);
		}

	if (err != KErrNone)
		RescheduleAllL(err);

	}

void CMsvSendExe::ProcessFileL()
/*
	ProcessFileL
*/
	{
	RetrievePackagesL();
	RetrieveMessagesL();

	if (iOperations.Count())
		{
		CallMtmL();
		iActiveSchedulerWait.Start();
		}
	else
		{
		SCHSENDLOG(FLog(iFileName, _L("\tNO msg to send!")));
		}
	}


void CMsvSendExe::RetrievePackagesL()
	{
	
	//open the filestore 	
	CFileStore* store = CDirectFileStore::FromLC(iTaskFile);//pushes store

	RStoreReadStream instream;
	instream.OpenLC(*store,store->Root());//pushes instream

	//get task count
	TInt taskCount = instream.ReadInt32L();

	SCHSENDLOG(FLog(iFileName, _L("\tTask Count=%d"), taskCount));

	for (TInt curTask = 0; curTask < taskCount; curTask++)
		{
		CScheduledTask* task = CScheduledTask::NewLC(instream);

		AddPackageL(*task);

		CleanupStack::PopAndDestroy(task);
		}

	CleanupStack::PopAndDestroy(2, store);
	}

#ifndef _MSG_NO_LOGGING
void CMsvSendExe::FLog(const TDesC& aFileName, TRefByValue<const TDesC16> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list, aFmt);

	TParse parse;
	parse.Set(aFileName, NULL, NULL);

	TBuf<256> buf;
	buf.Append(parse.Name());
	buf.Append(_L(": "));
	buf.AppendFormatList(aFmt, list);

	RFileLogger::Write(KSchSendLogDir, KSchSendExeLogFile, KSchSendExeLoggingMode,
				buf);
	}
#endif


/**

The CMsvSendExe::CallMtmL method
Attempts to send the messages in the operation selection

@internalAll
*/
void CMsvSendExe::CallMtmL()
	{
	__ASSERT_DEBUG(iOperations.Count(), gPanic(ENoMessagesInSelection));

	TInt count = iOperations.Count();

	while (count--)
		{
		CMsvSendExeActive& active = *iOperations[count];
		SCHSENDLOG(FLog(iFileName, _L("\tStarting operation for %d msgs (Mtm=%d CommandId=%d)"), active.MsgCount(), active.Mtm().iUid, active.Package().iCommandId));
		active.StartL();
		SCHSENDLOG(FLog(iFileName, _L("\tStarted (Mtm=%d CommandId=%d)"), active.Mtm().iUid, active.Package().iCommandId));
		}

	iStatus = KRequestPending;
	SetActive();
	}


/**

The CMsvSendExe::DoCancel method
Cancels all outstanding operations and stops the active scheduler.

@return			
@internalAll
*/
void CMsvSendExe::DoCancel()
	{
	SCHSENDLOG(FLog(iFileName, _L("\tOperations cancelled"), iStatus.Int()));

	TInt count = iOperations.Count();
	while (count--)
		iOperations[count]->Cancel();

	iActiveSchedulerWait.AsyncStop();
	}


/**

The CMsvSendExe::RunL method
Called when all operations in iOperations have completed.
Stops the active scheduler.

@return			
@internalAll
*/
void CMsvSendExe::RunL()
	{
	SCHSENDLOG(FLog(iFileName, _L("All operations completed")));
	iActiveSchedulerWait.AsyncStop();
	}

void CMsvSendExe::AddPackageL(const CScheduledTask& aTask)
	{
	TMsvSchedulePackage pkg;
	TSecurityInfo securityInfo = aTask.SecurityInfo();
	
	if( (securityInfo.iSecureId.iId != KSchSendExeSid) && (securityInfo.iSecureId.iId != KMsvServerId) )
		{
		if( PlatSec::ConfigSetting(PlatSec::EPlatSecDiagnotics) )
			{
             _LIT(KMessage,"CMsvSendExe::AddPackageL ERROR %d");
             RDebug::Print(KMessage,  KErrPermissionDenied);
			}
		if( PlatSec::ConfigSetting(PlatSec::EPlatSecEnforcement) )
			{
			//security violation
			User::Leave(KErrPermissionDenied);
			}
		}

	
	pkg.UnpackL(aTask.Info(), aTask.Data());
	User::LeaveIfError(iPackages.Append(pkg));
	}

/**

The CMsvSendExe::RetrieveMessagesL method
Retrieves the message selection from the task file.

@internalAll
*/
void CMsvSendExe::RetrieveMessagesL()
	{
	iSession = CMsvSession::OpenSyncL(*this);
	TInt count = iPackages.Count();

	while(count--)
		{

		AddTaskL(iPackages[count]);

		iPackages.Remove(count);
		}
	}


void CMsvSendExe::AddTaskL(const TMsvSchedulePackage& aPackage)
	{
	CMsvEntry* cEntry = NULL;
	TRAPD(err, cEntry = iSession->GetEntryL(aPackage.iId));

	if (err != KErrNotFound)
		{
		User::LeaveIfError(err);
		CleanupStack::PushL(cEntry);

		TMsvEntry entry = cEntry->Entry();
		const TInt sendState = entry.SendingState();

		//Only send the message if sending state is Scheduled or Resend.
		if (entry.Scheduled() && (sendState == KMsvSendStateScheduled || sendState == KMsvSendStateResend))
			{
			entry.SetSendingState(KMsvSendStateWaiting);

			// Fix for DEF000924: Need to be able to send/cancel an sms while another is being sent
			if (entry.iServiceId == KMsvLocalServiceIndexEntryId && entry.iRelatedId != KMsvNullIndexEntryId)
				{
				SCHSENDLOG(  FLog(iFileName, _L("Changing service from %x to %x"), entry.iServiceId, entry.iRelatedId));
				entry.iServiceId = entry.iRelatedId;
				}
			else
				{
				SCHSENDLOG(  FLog(iFileName, _L("Not changing service from %x (related=%x)"), entry.iServiceId, entry.iRelatedId));
				}
			// End of fix

 			cEntry->ChangeL(entry);
			AddTaskL(aPackage, entry.iMtm);
			SCHSENDLOG(FLog(iFileName, _L("\t\tMsg=%d [Mtm=%d SendState=%d]"), aPackage.iId, entry.iMtm.iUid, entry.SendingState()));
			}
		else
			{
			SCHSENDLOG(FLog(iFileName, _L("\t\tIGNORING Msg=%d (Mtm=%d SendState=%d Scheduled=%d)"), aPackage.iId, entry.iMtm.iUid, sendState, entry.Scheduled()));
			}

		CleanupStack::PopAndDestroy(cEntry);
		}
	else
		{
		SCHSENDLOG(FLog(iFileName, _L("\t\tIGNORING Msg=%d: NOT FOUND"), aPackage.iId));
		}
	}

/**

The CMsvSendExe::AddTaskL method
Finds a CMsvSendExeActive in iOperations that corresponds to aPackage and aMtm.
If not found, then creates a new CMsvSendExeActive, and appends it to iOperations.
Then the message in aPackage is appended to the CMsvSendExeActive.

@internalAll
@param				aPackage	Information about what operation to start
@param				aMtm		The MTM of the message identified in aPackage
*/
void CMsvSendExe::AddTaskL(const TMsvSchedulePackage& aPackage, const TUid& aMtm)
	{
	//Add the task to an object in iOperations

	CMsvSendExeActive* active = NULL;
	TInt count = iOperations.Count();

	while (active == NULL && count--)
		{
		CMsvSendExeActive* tempActive = iOperations[count];

		if (tempActive->Mtm() == aMtm &&
			tempActive->Package().iCommandId == aPackage.iCommandId &&
			tempActive->Package().iParameter == aPackage.iParameter &&
			tempActive->Package().iPollProgress == aPackage.iPollProgress)
			active = tempActive;
		}

	if (active == NULL)
		{
		active = CMsvSendExeActive::NewLC(*iSession, *this, aPackage, aMtm, Priority());
		User::LeaveIfError(iOperations.Append(active));
		CleanupStack::Pop(active);
		}

	active->AppendL(aPackage.iId);
	}


/**

The CMsvSendExe::OperationComplete method
Called by a CMsvSendExeActive when it completes
If all operations in iOperations are complete, then CMsvSendExe completes itself

@return			
@internalAll
@param				 aActive The operation that has just completed
@param				 aError  
*/
void CMsvSendExe::OperationComplete(const CMsvSendExeActive& aActive, TInt aError)
	{
	SCHSENDLOG(FLog(iFileName, _L("\tOperation Complete (Mtm=%d CommandId=%d Err=%d)"), aActive.Mtm().iUid, aActive.Package().iCommandId, aError));

	if (aError != KErrNone)
		RescheduleOnError(aActive, aError);

#ifdef _DEBUG
	iOperationsComplete++;
#endif

	__ASSERT_DEBUG(!aActive.IsActive(), gPanic(EOperationCompletedTooSoon));

	TInt count = iOperations.Count();

	while (count--)
		{
		if (iOperations[count]->IsActive())
			{
			__ASSERT_DEBUG(iOperationsComplete < iOperations.Count(), gPanic(ETooManyOperationsCompleted));
			return;
			}
		}

	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	}

_LIT(KSchSendExePanic, "SchSendExe");

GLDEF_C void gPanic(TSchSendExePanic aPanic)
	{
	User::Panic(KSchSendExePanic,aPanic);
	}

GLDEF_C void gPanic(TInt aPanic)
	{
	User::Panic(KSchSendExePanic,aPanic);
	}


void CMsvSendExe::RescheduleOnError(const CMsvSendExeActive& aActive, TInt aError)
	{
	SCHSENDLOG(FLog(iFileName, _L("\tRescheduleOnError (Mtm=%d CommandId=%d InCount=%d)"), aActive.Mtm().iUid, aActive.Package().iCommandId, aActive.Selection().Count()));
	RescheduleOnError(aActive.Selection(), aActive.Package(), aError);
	}

void CMsvSendExe::RescheduleAllL(TInt aError)
	{
	SCHSENDLOG(FLog(iFileName, _L("\tRescheduleAllL [aError=%d]"), aError));
	RScheduler scheduler;
	TSchedulerItemRef ref;
	ref.iHandle = KErrNotFound;
	TInt count = 0;
	TInt err = KErrNone;
	TTime start;
	start.UniversalTime();

	TInt activeCount = iOperations.Count();
	while (activeCount--)
		{
		CMsvSendExeActive& active = *iOperations[activeCount];
		active.Cancel();
		TRAP(err, DoResheduleOnErrorL(scheduler, active.Selection(), active.Package(), aError, ref, count, start)); //ignore error
		FailOnError(active.Selection(), aError);
		}

	activeCount = iPackages.Count();
	CMsvEntrySelection* sel = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(sel);
	while (activeCount--)
		{
		const TMsvSchedulePackage& package = iPackages[activeCount];
		sel->Reset();
		sel->AppendL(package.iId);
		TRAP(err, DoResheduleOnErrorL(scheduler, *sel, package, aError, ref, count, start)); //ignore error
		FailOnError(*sel, aError);
		}
	CleanupStack::PopAndDestroy(sel);

	CompleteReschedule(scheduler, ref, count);
	}

void CMsvSendExe::CompleteReschedule(RScheduler& aScheduler, const TSchedulerItemRef& aRef, TInt aCount)
	{
	if (aCount != 0)
		{
		aScheduler.EnableSchedule(aRef.iHandle);
		}
	else if (aRef.iHandle != KErrNotFound)
		{
		aScheduler.DeleteSchedule(aRef.iHandle);
		}
	aScheduler.Close();

	SCHSENDLOG(FLog(iFileName, _L("\tCompleteReschedule [Ref=%d OutCount=%d]"), aRef.iHandle, aCount));
	}

void CMsvSendExe::RescheduleOnError(const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage, TInt aError)
	{
	RScheduler scheduler;
	TTime start;
	start.UniversalTime();
	TSchedulerItemRef ref;
	ref.iHandle = KErrNotFound;
	TInt count = 0;
	TRAPD(err, DoResheduleOnErrorL(scheduler, aSelection, aPackage, aError, ref, count, start)); //ignore error

	CompleteReschedule(scheduler, ref, count);
	FailOnError(aSelection, aError); //fails outstanding messages that weren't rescheduled. Ignore error
	}


void CMsvSendExe::DoResheduleOnErrorL(RScheduler& aScheduler, const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage, TInt aError, TSchedulerItemRef& aRef, TInt& aCount, TTime& aStartTime)
	{
	SCHSENDLOG(FLog(iFileName, _L("\tDoRescheduleOnError [aError=%d]"), aError));

	CMsvScheduleSettings* settings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(settings);

	TMsvSchedulePackage package(aPackage);
	TInt selCount = aSelection.Count();
	TTaskInfo info;
	TInt err = KErrNone;

	//Schedule each message
	while (selCount--)
		{
		const TMsvId id = aSelection[selCount];

		TBool scheduleMessage = ETrue;
		CMsvEntry* cEntry = NULL;
		TMsvEntry entry;

		if (iSession != NULL)
			TRAP(err, cEntry = iSession->GetEntryL(id));

		SCHSENDLOG(FLog(iFileName, _L("\t\tScheduling Task [Id=%d Err=%d iSession=0x%X cEntry=0x%X]..."), id, err, iSession, cEntry));

		if (cEntry != NULL)
			{
			CleanupStack::PushL(cEntry);
			entry = cEntry->Entry();
			switch (entry.SendingState())
				{
				case KMsvSendStateWaiting:
				case KMsvSendStateScheduled:
				case KMsvSendStateResend:

					scheduleMessage = ETrue;
					break;

				case KMsvSendStateFailed:
				case KMsvSendStateSent:
				case KMsvSendStateSuspended:
				default:

					scheduleMessage = EFalse;
					SCHSENDLOG(FLog(iFileName, _L("\t\tNot Scheduled Task [Id=%d State=%d Err=%d]"), id, entry.SendingState(), entry.iError));
					break;
				}
			} //end if

		if (scheduleMessage)
			{
			if (aRef.iHandle == KErrNotFound)
				{
				//Find or create the schedule
				if (cEntry != NULL)
					{
					TRAP(err, RestoreScheduleSettingsL(entry.iServiceId, entry.iMtm, *settings)); //use default if err != KErrNone
					SCHSENDLOG(FLog(iFileName, _L("\t\tRestoreScheduleSettings [Err=%d]"), err));
					}

				CMsvScheduleSend::ConnectAndRegisterL(aScheduler, *settings);
				
				if (FindorCreateScheduleL(aScheduler, aStartTime, *settings, aRef))
					aCount++;
				
				User::LeaveIfError(aScheduler.DisableSchedule(aRef.iHandle));
				}
			
			//Schedule the message
			package.iId = id;
			CMsvScheduleSend::ScheduleEntryL(aScheduler, aRef, package, info);
			SCHSENDLOG(FLog(iFileName, _L("\t\tScheduleEntryL [Id=%d Task=%d]"), id, info.iTaskId));

			if (cEntry != NULL)
				{
				//Update the message
				entry.iError = aError;
				TRAP(err, UpdateEntryL(*cEntry, entry, aRef, info, aStartTime));
				SCHSENDLOG(FLog(iFileName, _L("\t\tEntry updated [Err=%d]"), err));
				}

			aCount++;

			SCHSENDLOG(FLog(iFileName, _L("\t\tScheduled Task Complete [Id=%d Task=%d]"), id, info.iTaskId));
			}

		if (cEntry)
			CleanupStack::PopAndDestroy(cEntry);
		} //end while

	CleanupStack::PopAndDestroy(settings);
	}

TBool CMsvSendExe::FindorCreateScheduleL(RScheduler& aScheduler, TTime& aStartTime, const CMsvScheduleSettings& aSettings, TSchedulerItemRef& aRef)
	{
	aStartTime.UniversalTime();
	aStartTime += (TTimeIntervalMicroSeconds32) (KSchSendExeMinReschedule - KMsvSendExeOneMinute);
	TInt err = KErrNotFound;
	const TInt max = (KSchSendExeMaxReschedule - KSchSendExeMinReschedule) / KMsvSendExeOneMinute;

	for (TInt i = 0; err != KErrNone && i < max; i++)
		{
		aStartTime += (TTimeIntervalMicroSeconds32) KMsvSendExeOneMinute;
		CMsvScheduleSend::RoundUpToMinute(aStartTime);
		TRAP(err, CMsvScheduleSend::FindScheduleL(aScheduler, aStartTime, aRef));
		}

	if (err != KErrNone)
		{
		CMsvScheduleSend::CreateScheduleL(aScheduler, aSettings, aStartTime, aSettings.ValidityPeriod(), aRef);
		SCHSENDLOG(FLog(iFileName, _L("\t\tSchedule Created (Ref=%d) for:"), aRef.iHandle));
		}
	else
		SCHSENDLOG(FLog(iFileName, _L("\t\tSchedule Found (Ref=%d) for:"), aRef.iHandle));

#ifndef _MSG_NO_LOGGING
	TBuf<32> bufDate;
	aStartTime.FormatL(bufDate, _L("%D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B"));
	SCHSENDLOG(FLog(iFileName, _L("\t\t%S"), &bufDate));
#endif

	return (err == KErrNone);
	}

void CMsvSendExe::RestoreScheduleSettingsL(TMsvId aServiceId, const TUid& aMtm, CMsvScheduleSettings& aSettings)
	{
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvRootIndexEntryId, order);
	CleanupStack::PushL(cEntry);

	if (aServiceId == KMsvLocalServiceIndexEntryId)
		{
		const TInt count = cEntry->Count();

		for (TInt i = 0; i < count; i++)
			{
			const TMsvEntry& entry = (*cEntry)[i];
			if (entry.iType == KUidMsvServiceEntry && entry.iMtm == aMtm)
				{
				aServiceId = entry.Id();
				break;
				}
			}
		}

	if (aServiceId == KMsvLocalServiceIndexEntryId)
		User::Leave(KErrNotFound);
	
	CRepository* repository = CRepository::NewLC(aMtm);
	TMsvScheduleSettingsUtils::LoadScheduleSettingsL(aSettings, *repository);
	CleanupStack::PopAndDestroy(2, cEntry); // repository, cEntry
	}

void CMsvSendExe::FailOnError(const CMsvEntrySelection& aSelection, TInt aError)
	{
	if (iSession == NULL)
		return;

	TInt selCount = aSelection.Count();
	TInt err = KErrNone;
	CMsvEntry* cEntry = NULL;

	while (selCount--)
		{
		const TMsvId id = aSelection[selCount];
		TRAP(err, cEntry = iSession->GetEntryL(id));
		//no need for the cleanup stack

		if (err == KErrNone)
			{
			TMsvEntry entry(cEntry->Entry());
			switch (entry.SendingState())
				{
				case KMsvSendStateWaiting:
				case KMsvSendStateScheduled:
				case KMsvSendStateResend:

					if (entry.iDate < iCreated)
						{
						//Message has not been rescheduled
						entry.SetSendingState(KMsvSendStateFailed);
						entry.SetScheduled(EFalse);
						entry.SetFailed(ETrue);
						entry.iError = aError;
						TRAP(err, cEntry->ChangeL(entry)); //ignore error
						}

					break;

				default:
					break;
				}

			delete cEntry;
			cEntry = NULL;
			} //end if
		} //end while
	}

void CMsvSendExe::UpdateEntryL(CMsvEntry& aMsvEntry, TMsvEntry& aEntry, const TSchedulerItemRef& aRef, const TTaskInfo& aInfo, const TTime& aStartTime)
	{
	CMsvStore* store = aMsvEntry.EditStoreL();
	CleanupStack::PushL(store);

	TMsvEntryScheduleData data;
	data.RestoreL(*store);

	CMsvScheduleSend::UpdateEntryAfterSchedule(aRef, aInfo, aStartTime, KMsvSendStateResend, aEntry, data);

	data.StoreL(*store);
	store->CommitL();

	CleanupStack::PopAndDestroy(store);
	aMsvEntry.ChangeL(aEntry);
	}

