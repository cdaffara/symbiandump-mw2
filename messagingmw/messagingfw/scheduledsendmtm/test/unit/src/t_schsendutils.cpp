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

#include "t_schsendutils.h"
#include <msvstd.h>
#include <msvids.h>
#include <msvuids.h>
#include <msvschedulesettings.h>
#include <csch_cli.h>
#include <msvschedulepackage.h>
#include <msvoffpeaktime.h>

EXPORT_C CSchSendTestUtils::CSchSendTestUtils(RTest& aTest)
: CMsvTestUtils(aTest)
	{
	}

EXPORT_C void CSchSendTestUtils::InstallMtmGroupsL()
	{
	}

EXPORT_C void CSchSendTestUtils::CreateServerMtmRegsL()
	{
//CreateServerMtmRegL(KPigeonMtmUid, _L("Pig"), KUidMsvMtmServerDLL, KUidMsvMtmClientDLL, TMsvTestDllInfo(KUidMsvMtmUiDLL,2), TMsvTestDllInfo(KUidMsvMtmUiDLL,1), KPigeonMtmUid, KDataComponentFileName);
	}

EXPORT_C void CSchSendTestUtils::ConstructL()
	{
	CMsvTestUtils::ConstructL(ETuNone);
	CleanScheduleFolderL();
	CreateAllTestDirectories();
	GoClientSideL();

	InstallMtmGroupL(_L("c:\\system\\mtm\\pigmtmu.dat"));

	iSelection = new(ELeave) CMsvEntrySelection;
	iSchTestActive = CScheduleTestActive::NewL(*this);
	}

EXPORT_C CSchSendTestUtils::~CSchSendTestUtils()
	{
	delete iSchTestActive;
	delete iSelection;
	}

EXPORT_C void CSchSendTestUtils::Panic(TInt aPanic)
	{
	User::Panic(_L("SchSend Test"), aPanic);
	}

EXPORT_C void CSchSendTestUtils::StartL()
	{
	TRAPD(err, RunAutoL());

	if (err)
		{
		if (iCurrentTest)
			{
			TestFinish(iCurrentTest, err);
			}

		TestHarnessFailed(err);
		}
	else
		{
		TestHarnessCompleted();
		}
	}

//Returns approximate difference (rounded down?) in minutes
EXPORT_C TInt CSchSendTestUtils::DiffInMins(TTime d1, TTime d2)
	{
	iRTest<<d1<<d2;
	TInt64 diffUs64 = d1.Int64() - d2.Int64();
	TInt64 diffMins64 = diffUs64 / (1000000 * 60);
	TInt diffMins32 = I64INT(diffMins64);
	return diffMins32;
	}

//Returns approximate difference (rounded down?) in seconds
EXPORT_C TInt CSchSendTestUtils::DiffInSecs(TTime d1, TTime d2)
	{
	iRTest<<d1<<d2;
	TInt64 diffUs64 = d1.Int64() - d2.Int64();
	TInt64 diffSecs64 = diffUs64 / (1000000);
	TInt diffSecs32 = I64INT(diffSecs64);
	return diffSecs32;
	}

EXPORT_C void CSchSendTestUtils::ChangeMessageTimeL(TMsvId aMessage, TTime aTime)
	{
	iMsvEntry->SetEntryL(aMessage);
	TMsvEntry entry = iMsvEntry->Entry();
	iRTest<<entry.iDate<<aTime;
	entry.iDate = aTime;

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();
	CMsvOperation*  opert = iMsvEntry->ChangeL(entry, wait->iStatus);
	CleanupStack::PushL(opert);
	CActiveScheduler::Start(); // operation complete
	entry = iMsvEntry->Entry();
	iRTest<<entry.iDate;

	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());

	if(wait->iStatus.Int()!=KErrNone)
		User::Leave(wait->iStatus.Int());

	//TInt err = package().iError;
	//if(err != KErrNone);
	//	User::Leave(package().iError);

	CleanupStack::PopAndDestroy(2);//opert, wait
	}

EXPORT_C void CSchSendTestUtils::AssertL(TBool b)
	{
	if(!b)
		{
		User::Leave(KErrAbort);
		}
	}

EXPORT_C TMsvId CSchSendTestUtils::CreateMessageLC(TMsvEntry& aEntry, TMsvId aDestFolder, TTime aSendTime, TBool aOffPeak)
	{
	//Make a TMsvEntry
	iMsvEntry->SetEntryL(aDestFolder);
	aEntry.iType = KUidMsvMessageEntry;
	aEntry.iMtm = KSchSendTestMtmUid;
	
	aEntry.iServiceId = KMsvLocalServiceIndexEntryId;

	aEntry.iDate = aSendTime;
	aEntry.SetOffPeak(aOffPeak);

	//Create the entry in the server

	CMsvOperationWait* wait = CMsvOperationWait::NewLC();
	wait->Start();
	CMsvOperation*  opert = iMsvEntry->CreateL(aEntry, wait->iStatus);
	CleanupStack::PushL(opert);
	CActiveScheduler::Start(); // operation complete

	//Find out the ID of the entry we made
	TMsvLocalOperationProgress details;
	TPckgC<TMsvLocalOperationProgress> package(details);	
	package.Set(opert->ProgressL());

	if(wait->iStatus.Int()!=KErrNone)
		User::Leave(wait->iStatus.Int());

	if(package().iError!=KErrNone)
		User::Leave(package().iError);
	
	*(TMsvId*)&aEntry = package().iId;
	TMsvId testMsvId = aEntry.Id();

	CleanupStack::PopAndDestroy(2);//opert, wait
	iMsvSession->CleanupEntryPushL(testMsvId);

	return testMsvId;
	}

EXPORT_C void CSchSendTestUtils::DisplayAllSchedulesL(TInt& rSchCount, TInt& rSchSize)
	{
	Printf(_L("Displaying all Schedules:\n"));
	rSchCount = 0;
	rSchSize = 0;

	TParse parse;
	TFileName fileName(_L("\\system\\schedules\\")); 
	_LIT(KCDrive, "C:");
	parse.Set(fileName, &KCDrive, NULL);

	CDir* entryList;
	TPtrC drive(parse.DriveAndPath());
	TInt err = iFs.GetDir(drive, KEntryAttNormal, ESortNone, entryList);
	
	if (err)
		{
		Printf(_L("\tiFs.GetDir(%S) returned %d\n"), &drive, err);
		return;
		}

	CleanupStack::PushL(entryList);
	
	TInt count = entryList->Count();

	while (count--)
		{
		TEntry entry((*entryList)[count]);
		rSchSize += entry.iSize;
		}

	Printf(_L("\tFiles in %S have total size %d\n"), &drive, rSchSize);

	CleanupStack::PopAndDestroy(entryList);

	RScheduler sch;
	err = sch.Connect();

	if (err)
		{
		Printf(_L("\tRScheduler::Connect() returned %d\n"), err);
		return;
		}
	
	CleanupClosePushL(sch);

	IMPORT_C TInt GetScheduleL(const TInt aScheduleHandle, TScheduleState& aState, 
			CArrayFixFlat<TScheduleEntryInfo>& aEntries, CArrayFixFlat<TTaskInfo>& aTasks, TTime& aDueTime);

	CArrayFixFlat<TSchedulerItemRef>* schRefArray = new (ELeave) CArrayFixFlat<TSchedulerItemRef>(10);
	CleanupStack::PushL(schRefArray);

	CArrayFixFlat<TScheduleEntryInfo>* schEntries = new (ELeave) CArrayFixFlat<TScheduleEntryInfo>(10);
	CleanupStack::PushL(schEntries);

	CArrayFixFlat<TTaskInfo>* schTasks = new (ELeave) CArrayFixFlat<TTaskInfo>(10);
	CleanupStack::PushL(schTasks);

	err = sch.GetScheduleRefsL(*schRefArray, EAllSchedules);

	if (!err)
		{
		TScheduleState schState;
		TTime schDueTime;
		count = schRefArray->Count();
		rSchCount = count;
		Printf(_L("\t%d schedules in the task scheduler\n"), count);

		while (count--)
			{
			err = sch.GetScheduleL((*schRefArray)[count].iHandle, schState, *schEntries, *schTasks, schDueTime);

			if (!err)
				{
				TInt taskCount = schTasks->Count();
				Printf(_L("\tSchedule %d (\"%S\") has %d tasks\n"), (*schRefArray)[count].iHandle, &(*schRefArray)[count].iName, taskCount);

				while (taskCount--)
					{
					TTaskInfo taskInfo((*schTasks)[taskCount]);
					TInt size;
					err = sch.GetTaskDataSize(taskInfo.iTaskId, size);

					if (!err)
						{
						HBufC* hBuf = HBufC::NewLC(size);
						TPtr ptr(hBuf->Des());
						TSchedulerItemRef schRef;

						err = sch.GetTaskInfoL(taskInfo.iTaskId, taskInfo, ptr, schRef, schDueTime);

						if (!err)
							{
							TPckgBuf<TMsvSchedulePackage> mtmBuf;
							mtmBuf.Copy(*hBuf);
							Printf(_L("\t\tTask %d: msvId %d, commandId %d, pollProgress %d\n"), taskInfo.iTaskId, mtmBuf().iId, mtmBuf().iCommandId, mtmBuf().iPollProgress.Int());
							}
						else
							{
							Printf(_L("\t\tRScheduler::GetTaskInfoL(%d) returned %d\n"), taskInfo.iTaskId, err);
							}

						CleanupStack::PopAndDestroy(hBuf);
						}
					else
						{
						Printf(_L("\t\tRScheduler::GetTaskDataSize(%d) returned %d\n"), taskInfo.iTaskId, err);
						}
					}
				}
			else
				{
				Printf(_L("\tRScheduler::GetScheduleL(%d (\"%S\")) returned %d\n"), (*schRefArray)[count].iHandle, &(*schRefArray)[count].iName, err);
				}
			}
		}
	else
		{
		Printf(_L("RScheduler::GetScheduleRefsL() returned %d"), err);
		}

	CleanupStack::PopAndDestroy(3); //schRefArray, schEntries, schTasks
	CleanupStack::PopAndDestroy(); //sch
	}

EXPORT_C void CSchSendTestUtils::CleanScheduleFolderL()
	{
	CFileMan* fileMan = CFileMan::NewL(iFs); 
	CleanupStack::PushL(fileMan);
	TParse parse;
	TFileName fileName(_L("\\system\\schedules\\")); 
	TBuf<4> drive=_L("C:");
	parse.Set(fileName, &drive, NULL);
	TInt error = fileMan->RmDir(parse.DriveAndPath()); 
	error = iFs.RmDir(parse.DriveAndPath());
	
	if (!(error==KErrNotFound||error==KErrNone))
		{
        TPtrC driveAndPath = parse.DriveAndPath();
		Printf(_L("Directory %S cannot be removed. Error=%d"), &driveAndPath, error);
		Printf(_L("Please ensure directory is not in use.\n"));
		User::Leave(error);
		}

	iFs.MkDirAll(parse.DriveAndPath());

	CleanupStack::PopAndDestroy(fileMan);
	}

EXPORT_C TBool CSchSendTestUtils::CompareRoundedTimes(const TTime& aLeft, const TTime& aRight) const
	{
	TBool ret = (aLeft == aRight);
	ret = ret || (RoundUpToMinute(aLeft) == RoundUpToMinute(aRight));
	ret = ret || (RoundDownToMinute(aLeft) == RoundDownToMinute(aRight));
	return ret;
	}


EXPORT_C TTime CSchSendTestUtils::RoundUpToMinute(const TTime& aTime) const
	{
	TTime ret(aTime);
	TDateTime dt(ret.DateTime());

	if (dt.Second() != 0 || dt.MicroSecond() != 0)
		{
		dt.SetSecond(0);
		dt.SetMicroSecond(0);
		ret = dt;
		ret += (TTimeIntervalMinutes) 1;
		}

	return ret;
	}

EXPORT_C TTime CSchSendTestUtils::RoundDownToMinute(const TTime& aTime) const
	{
	TTime ret(aTime);
	TDateTime dt(ret.DateTime());

	if (dt.Second() != 0 || dt.MicroSecond() != 0)
		{
		dt.SetSecond(0);
		dt.SetMicroSecond(0);
		ret = dt;
		}

	return ret;
	}

//
//
//

CScheduleTestActive::CScheduleTestActive(CSchSendTestUtils& aTest, TInt aPriority)
: CActive(aPriority), iTest(aTest), iSession(*aTest.iMsvSession)
	{
	CActiveScheduler::Add(this);
	}

EXPORT_C CScheduleTestActive* CScheduleTestActive::NewL(CSchSendTestUtils& aTest, TInt aPriority)
	{
	CScheduleTestActive* self = new (ELeave) CScheduleTestActive(aTest, aPriority);
	CleanupStack::PushL(self);
	self->iSession.AddObserverL(*self);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CScheduleTestActive::~CScheduleTestActive()
	{
	Cancel();
	iSession.RemoveObserver(*this);
	delete iSelection;
	delete iOperation;
	}

//Set aStopAtRunL to true to stop the active scheduler. 
//False will make us wait for someone else to stop it
EXPORT_C const TSchSendTestProgress& CScheduleTestActive::TransferCommandSyncL(TSchSendTestOperation aCommand, const CMsvEntrySelection& aSelection, TBool aStopAtRunL)
	{
	return TransferCommandSyncL(aCommand, aSelection, _L8("aaaa"), aStopAtRunL);
	}

EXPORT_C const TSchSendTestProgress& CScheduleTestActive::TransferCommandSyncL(TSchSendTestOperation aCommand, const CMsvEntrySelection& aSelection, const TDesC8& aParameter, TBool aStopAtRunL)
	{
	iTest.Printf(_L("TransferCommandSyncL Command %d Count %d StopAtRunL %d\n"), aCommand, aSelection.Count(), aStopAtRunL);

	if (IsActive())
		User::Leave(KErrInUse);

	delete iSelection;
	iSelection = NULL;
	iSelection = aSelection.CopyL();
	iCommand = aCommand;
	iStopAtRunL = aStopAtRunL;
	iStatus = KRequestPending;
	SetActive();

	delete iOperation;
	iOperation = NULL;
	iOperation = iSession.TransferCommandL (*iSelection, 
				aCommand,
				aParameter, 
				iStatus);

	CActiveScheduler::Start(); // will not return from this until the command is transferred

	iProgress.iError = iStatus.Int();
	if(iStatus.Int()!=KErrNone)
		User::Leave(iStatus.Int());

	TPckgC<TSchSendTestProgress> pkg(iProgress);
	pkg.Set(iOperation->ProgressL());
	iProgress = pkg();
	return iProgress;
	}


TInt CScheduleTestActive::RunError(TInt aError)
	{
	if (iProgress.iError == KErrNone)
		iProgress.iError = aError;

	CActiveScheduler::Stop();
	return KErrNone;
	}

void CScheduleTestActive::RunL()
	{
	iTest.Printf(_L("CScheduleTestActive::RunL() iStatus %d\n"), iStatus.Int());
	User::LeaveIfError(iStatus.Int());

	if(iStopAtRunL) 
		{
		CActiveScheduler::Stop();
		return;
		}

	switch (iCommand)
		{
		case EScheduleAllL:
		case EReScheduleAllL:
			{
			TInt count = iSelection->Count();
			TBool stopped = EFalse;
			while (!stopped && count--)
				{
				CMsvEntry* entry = iSession.GetEntryL(iSelection->At(count));
				CleanupStack::PushL(entry);
				const TInt state = entry->Entry().SendingState();
				switch(state)
					{
					case KMsvSendStateScheduled:
					case KMsvSendStateResend:
						break;
					default: // The message sent or failed
						stopped = ETrue;
						CActiveScheduler::Stop();
						break;
					}
				CleanupStack::PopAndDestroy(entry);
				}
		
			break;
			}
		default:
			CActiveScheduler::Stop(); //Added by AA 9/6/2000. Is this correct?
			break;
		}
	}

void CScheduleTestActive::HandleSessionEventL(TMsvSessionEvent eventType, TAny* p1, TAny*, TAny*)
	{
	if (iStopAtRunL) return;
	if (eventType != EMsvEntriesCreated) return;

	CMsvEntrySelection* entries = STATIC_CAST(CMsvEntrySelection*, p1);

	CMsvEntry* cEntry = iSession.GetEntryL((*entries)[0]);
	CleanupStack::PushL(cEntry);

	TMsvEntry entry(cEntry->Entry());
	
	if(entry.iDetails == KSchSendTestDetails)
		{
		iTest.Printf(_L("Scheduled message has been sent\n"));

		//We found the right message!
		CActiveScheduler::Stop();

		//Delete the new message
		CMsvOperationWait* wait = CMsvOperationWait::NewLC();
		wait->Start();
		cEntry->SetEntryL(entry.Parent());
		CMsvOperation* op = cEntry->DeleteL(entry.Id(), wait->iStatus);
		CActiveScheduler::Start();
		delete op;
		CleanupStack::PopAndDestroy(wait);
		}

	CleanupStack::PopAndDestroy(cEntry);
	}


//
//
//


//
//
//

EXPORT_C CSchSendTestTimer* CSchSendTestTimer::NewL(TInt aPriority)
    {
    CSchSendTestTimer* self = new (ELeave) CSchSendTestTimer(aPriority);
    CleanupStack::PushL(self);
    self->ConstructL();		// Inherited from CTimer.
    CleanupStack::Pop();
    return self;
    }

CSchSendTestTimer::CSchSendTestTimer(TInt aPriority)
: CTimer(aPriority)
	{
    CActiveScheduler::Add(this);
	}

void CSchSendTestTimer::RunL()
    {
    CActiveScheduler::Stop();
    }

EXPORT_C RTest& operator<<(RTest& aTest, TTime t)
	{
	TBuf<100> buf;
	t.FormatL(buf, _L("%D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B\n"));
	aTest.Printf(buf);
	return aTest;
	}

EXPORT_C TBool operator==(const CMsvSendErrorActions& one, const CMsvSendErrorActions& two)
	{
	TMsvSendErrorAction action1 = one.Default();
	TMsvSendErrorAction action2 = two.Default();
	if(!(action1==action2)) return EFalse;

	const CArrayFixFlat<TMsvSendErrorAction>& actionArray1 = one.Errors();
	const CArrayFixFlat<TMsvSendErrorAction>& actionArray2 = two.Errors();

	if(actionArray1.Count()!=actionArray2.Count()) return EFalse;
	for(int i = 0; i < actionArray1.Count(); i++)
		if(!(actionArray1[i]==actionArray2[i])) return EFalse;

	return ETrue;

	}

EXPORT_C TBool operator==(const TMsvSchedulePackage& a1, const TMsvSchedulePackage& a2)
	{
	if (a1.iId				!= a2.iId) return EFalse;
	if (a1.iCommandId		!= a2.iCommandId) return EFalse;
	if (a1.iPollProgress	!= a2.iPollProgress) return EFalse;
	if (a1.iParameter		!= a2.iParameter) return EFalse;
	return ETrue;
	}


EXPORT_C TBool operator==(const TMsvSendErrorAction& a1, const TMsvSendErrorAction& a2)
	{
	if(a1.iAction			!= a2.iAction) return EFalse;
	if(a1.iRetrySpacing		!= a2.iRetrySpacing) return EFalse;
	if(a1.iError			!= a2.iError) return EFalse;
	if(a1.MaxRetries()		!= a2.MaxRetries()) return EFalse;
	if(a1.iRetries			!= a2.iRetries) return EFalse;
	return ETrue;
	}

EXPORT_C TBool operator==(const TMsvOffPeakTime& time1, const TMsvOffPeakTime& time2)
	{
	if(time1.Day()!=time2.Day()) return EFalse;
	if(time1.Hour()!=time2.Hour()) return EFalse;
	if(time1.Minute()!=time2.Minute()) return EFalse;
	if(time1.ValidityPeriod()!=time2.ValidityPeriod()) return EFalse;
	return ETrue;
	}

EXPORT_C TBool StartsAt(const TMsvOffPeakTime& time1, const TTime& time2)
	{
	TDateTime dateTime2 = time2.DateTime();

	if(time1.Day() != time2.DayNoInWeek()) return EFalse;
	if(time1.Hour() != dateTime2.Hour()) return EFalse;
	if(time1.Minute() != dateTime2.Minute()) return EFalse;
	return ETrue;
	}

EXPORT_C TBool operator==(const CMsvScheduleSettings& aSettings1, const CMsvScheduleSettings& aSettings2)
	{
	if(aSettings1.Priority()!=aSettings2.Priority()) return EFalse;
	if(aSettings1.ValidityPeriod()!=aSettings2.ValidityPeriod()) return EFalse;
	if(aSettings1.IntervalType()!=aSettings2.IntervalType()) return EFalse;
	if(aSettings1.LongInterval()!=aSettings2.LongInterval()) return EFalse;
	if(aSettings1.ShortInterval()!=aSettings2.ShortInterval()) return EFalse;
	if(aSettings1.VariableIntervals().Count()!=aSettings2.VariableIntervals().Count()) return EFalse;
	for(int i = 0; i < aSettings1.VariableIntervals().Count(); i++)
		{
		if(aSettings1.VariableIntervals()[i]!=aSettings2.VariableIntervals()[i])
			return EFalse;
		}
	if(aSettings1.Latency()!=aSettings2.Latency()) return EFalse;
	return ETrue;
	}


//

EXPORT_C CSchSendTestWaitForState* CSchSendTestWaitForState::NewL(CSchSendTestUtils& aTest)
	{
	CSchSendTestWaitForState* self = new (ELeave) CSchSendTestWaitForState(aTest);
	CleanupStack::PushL(self);
	User::LeaveIfError(self->iTimer.CreateLocal());
	aTest.iMsvSession->AddObserverL(*self);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CSchSendTestWaitForState::~CSchSendTestWaitForState()
	{
	Reset();
	iTimer.Close();
	iTest.iMsvSession->RemoveObserver(*this);
	}

void CSchSendTestWaitForState::DoStartL(const CMsvEntrySelection& aSelection, TTimeIntervalMicroSeconds32 aAfter, TBool aWaitForever)
	{
	iTest(iSendingStates.Count() != 0);

	iAfter = aAfter;
	iWaitForever = aWaitForever;

	delete iSelection;
	iSelection = NULL;
	iSelection = aSelection.CopyL();

	iTimer.After(iStatus, iAfter);
	SetActive();

	CActiveScheduler::Start();
	}

EXPORT_C void CSchSendTestWaitForState::StartL(const CMsvEntrySelection& aSelection)
	{
	DoStartL(aSelection, KSchSendTestWaitForStateDefault, ETrue);
	}

EXPORT_C void CSchSendTestWaitForState::StartL(const CMsvEntrySelection& aSelection, TTimeIntervalMicroSeconds32 aAfter)
	{
	DoStartL(aSelection, aAfter, EFalse);
	}


CSchSendTestWaitForState::CSchSendTestWaitForState(CSchSendTestUtils& aTest)
: CActive(CActive::EPriorityStandard), iTest(aTest), iAfter(KSchSendTestWaitForStateDefault)
	{
	CActiveScheduler::Add(this);
	}

void CSchSendTestWaitForState::RunL()
	{
	User::LeaveIfError(iStatus.Int());

	TInt count = iSelection->Count();
	TBool stop = ETrue;
	while (count--)
		{
		const TMsvId id = (*iSelection)[count];
		stop = stop && CheckStateL(id);
		}

	if (stop || !iWaitForever)
		{
		if (!stop)
			iStatus = KErrGeneral;

		CActiveScheduler::Stop();
		}
	}

TBool CSchSendTestWaitForState::CheckStateL(TMsvId aId)
	{
	iTest.SetEntryL(aId);
	iTest.SetEntryL(iTest.Entry().Parent());
	iTest.SetEntryL(aId);

	const TInt sendingState = iTest.Entry().SendingState();
	TInt count = iSendingStates.Count();
	while (count--)
		{
		if (sendingState == iSendingStates[count])
			return ETrue;
		}

	return EFalse;
	}

TInt CSchSendTestWaitForState::RunError(TInt aError)
	{
	if (iStatus.Int() == KErrNone)
		iStatus = aError;

	CActiveScheduler::Stop();
	return KErrNone;
	}

void CSchSendTestWaitForState::DoCancel()
	{
	iTimer.Cancel();
	}

EXPORT_C void CSchSendTestWaitForState::Reset()
	{
	Cancel();
	delete iSelection;
	iSelection = NULL;
	iSendingStates.Reset();
	}

void CSchSendTestWaitForState::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	iTest.Printf(_L("CSchSendTestWaitForState::HandleSessionEventL [Event=%d IsActive=%d Count=%d]\n"), aEvent, IsActive(), iSelection->Count());
	if ((aEvent != EMsvEntriesChanged && aEvent != EMsvEntriesCreated) || !IsActive() || iSelection->Count() == 0)
		return;

	TInt count = iSelection->Count();

	while (count--)
		{
		const TMsvId id = iSelection->At(count);
		if (CheckStateL(id))
			iSelection->Delete(count);
		}

	if (iSelection->Count() == 0)
		{
		Cancel();
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		SetActive();
		}
	}
