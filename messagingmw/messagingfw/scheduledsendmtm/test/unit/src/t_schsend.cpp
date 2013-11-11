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

#include "t_schsend.h"

#include <msvschedulesend.h>
#include <bautils.h>
#include <smss.rsg>
#include <msvschedulesettings.h>
#include <msvsysagentaction.h>
#include <barsc.h>
#include <msvoffpeaktime.h>
#include <csmsaccount.h>

RTest& operator<<(RTest& aTest, TTime t);

CTrapCleanup* theCleanup;
//_LIT(KSmsResourceFile, "z:\\System\\Data\\SMSS.RSC");

//Horrible hack because nmake thinks &myFun is illegal but GCC needs the & sign

#ifdef __WINS__
#define PFUN
#else
#define PFUN &
#endif

void doMainL(RTest& aTest)
	{
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install( scheduler );

	CSchSendTest* schSendTest = CSchSendTest::NewL(aTest);
	CleanupStack::PushL(schSendTest);

	schSendTest->StartL();

	CleanupStack::PopAndDestroy(2); //schSendTest, iScheduler

	aTest.Printf(_L("\nWaiting 3 seconds before closing...\n"));
	User::After(3000000);
	}

GLDEF_C TInt E32Main()
	{
	RTest test(_L("Scheduled Sending Test Harness"));
	__UHEAP_MARK;
	test.Title();
	test.Start(_L("Scheduled Sending Test Harness"));
	theCleanup = CTrapCleanup::New();
	test(theCleanup !=NULL);
	TRAPD(error,doMainL(test));
	test(error==KErrNone);
	delete theCleanup;
	test.Printf(_L("Completed with return code %d"),error);
	test.End();
	test.Close();
	__UHEAP_MARKEND;
	return KErrNone;
	}

CSchSendTest* CSchSendTest::NewL(RTest& aTest)
	{
	CSchSendTest* self = new (ELeave) CSchSendTest(aTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); //self
	return self;
	}

CSchSendTest::CSchSendTest(RTest& aTest)
: CSchSendTestUtils(aTest)
	{
	}

void CSchSendTest::TestPackageL()
	{
	TMsvSchedulePackage pkg;
	pkg.iCommandId = 100000;
	pkg.iId = 123456;

	TestPackageL(pkg);

	pkg.iCommandId = 20000;
	pkg.iPollProgress = 1000000;

	TestPackageL(pkg);

	pkg.iParameter.Append(_L8("Hello"));

	TestPackageL(pkg);

	pkg.iParameter.Fill('a');

	TestPackageL(pkg);
	}

void CSchSendTest::TestPackageL(const TMsvSchedulePackage& aPackage)
	{
	TTaskInfo info;
	TMsvSchedulePackage pkg;
	HBufC* buf = NULL;

	aPackage.PackLC(info, buf);
	pkg.UnpackL(info, *buf);
	AssertL(aPackage == pkg);
	CleanupStack::PopAndDestroy(buf);
	}

void CSchSendTest::RunAutoL()
	{
	// Two groups of tests
	TestSendErrorActionsL();
	TestOffPeakTimesL();

	// Individual tests

	AssertL(DoTest(PFUN CSchSendTest::TestPackageL, _L("Schedule Package")));
	AssertL(DoTest(PFUN TestSysAgentActionsL, _L("Sys Agent Actions")));
	AssertL(DoTest(PFUN TestScheduleSettingsL, _L("Schedule Settings")));
	AssertL(DoTest(PFUN TestSchedulingMessagesL, _L("Scheduling Messages")));
	AssertL(DoTest(PFUN TestSchedulingOldMessagesL, _L("Scheduling Old Messages")));
	AssertL(DoTest(PFUN TestSchedulingMessagesTwiceL, _L("Scheduling Messages Twice")));
	AssertL(DoTest(PFUN TestReschedulingMessagesL, _L("ReScheduling Messages")));
	AssertL(DoTest(PFUN TestReschedulingNonScheduledMessagesL, _L("ReScheduling Non Scheduled Messages")));
	AssertL(DoTest(PFUN TestDeletingMessagesL, _L("Deleting Messages")));
	AssertL(DoTest(PFUN TestDeletingNonScheduledMessagesL, _L("Deleting Non Scheduled Messages")));

	AssertL(DoTest(PFUN TestSchedulingOffPeakMessagesL, _L("Off Peak Messages")));

	//AssertL(DoTest(PFUN TestCheckScheduleL, _L("CheckScheduleL")));

	AssertL(HeapTest(PFUN TestSysAgentActionsL, _L("Sys Agent Actions")));
	AssertL(HeapTest(PFUN TestSchedulingMessagesL, _L("Scheduling Messages")));
	AssertL(HeapTest(PFUN TestSchedulingOffPeakMessagesL, _L("Off Peak Messages")));
	AssertL(HeapTest(PFUN TestSchedulingOldMessagesL, _L("Scheduling Old Messages")));
	AssertL(HeapTest(PFUN TestSchedulingMessagesTwiceL, _L("Scheduling Messages Twice")));
	AssertL(HeapTest(PFUN TestReschedulingMessagesL, _L("ReScheduling Messages")));
	AssertL(HeapTest(PFUN TestReschedulingNonScheduledMessagesL, _L("ReScheduling Non Scheduled Messages")));
	AssertL(HeapTest(PFUN TestDeletingMessagesL, _L("Deleting Messages")));
	AssertL(HeapTest(PFUN TestDeletingNonScheduledMessagesL, _L("Deleting Non Scheduled Messages")));

	}


void CSchSendTest::TestSendErrorActionsL()
	{
/*
	Public functions of TMsvSendErrorAction:

	Tested	Function
	-------	---------------------------------------------------------------------
	Yes		TMsvSendErrorAction();
	Yes		void InternalizeL(RReadStream& aReadStream);
	Yes		void ExternalizeL(RWriRTestream& aWriRTestream) const;
	Yes		void Reset();
	Yes		void SetMaxRetries(const TInt16 aMaxRetries);
	Yes		TInt16 MaxRetries() const;

	Public functions of CMsvSendErrorActions:

	Tested	Function
	-------	---------------------------------------------------------------------
	Yes		static CMsvSendErrorActions* NewL();
	Yes		void AddSendErrorActionL(const TMsvSendErrorAction& aAction);
	Yes		const TInt RemoveSendErrorAction(const TInt aError);
	Yes		const TInt GetSendErrorAction(const TInt aError, TMsvSendErrorAction& aAction) const;
	Yes		void StoreL(CMsvStore& aStore) const;
	Yes		void RestoreL(CMsvStore& aStore);
			void RestoreFromResourceL(TResourceReader& aReader);
	Yes		void SetErrorsL(const CArrayFixFlat<TMsvSendErrorAction>& aActions);
	// but broken
	Yes		const CArrayFixFlat<TMsvSendErrorAction>& Errors() const;
	Yes		const TMsvSendErrorAction& Default() const;
	Yes		void SetDefault(const TMsvSendErrorAction& aAction);
 
*/

	AssertL(DoTest(PFUN TestSendErrorActions1L, _L("TestSendErrorActions1")));
	AssertL(DoTest(PFUN TestDefaultL, _L("TestDefault")));
	AssertL(DoTest(PFUN TestSendErrorActions3L, _L("TestSendErrorActions3")));
	AssertL(DoTest(PFUN TestErrorsL, _L("TestErrors")));
	AssertL(DoTest(PFUN TestStoringL, _L("TestStoring")));
	AssertL(DoTest(PFUN TestRemovingL, _L("TestRemoving")));
	//AssertL(DoTest(PFUN TestSendErrorActions4L, _L("TestSendErrorActions4")));

	}

void CSchSendTest::TestOffPeakTimesL()
	{
/*
	Public functions of TMsvOffPeakTime:

	Tested	Function
	-------	---------------------------------------------------------------------
	Yes		TMsvOffPeakTime();
	Yes		TMsvOffPeakTime(const TDay aDay, const TInt aHour, const TInt aMinute, const TTimeIntervalMinutes aValidityPeriod);
	Yes		void ExternalizeL (RWriRTestream &) const;
    Yes		void InternalizeL (RReadStream &);
	Yes		void Reset();
	Yes		const TDay Day() const;
	Yes		void SetDay(const TDay aDay);
	Yes		const TInt Hour() const;
	Yes		void SetHour(const TInt aHour);
	Yes		const TInt Minute() const;
	Yes		void SetMinute(const TInt aMinute);
	Yes		const TTimeIntervalMinutes ValidityPeriod() const;
	Yes		void SetValidityPeriod(const TTimeIntervalMinutes aValidityPeriod);
	Yes		const TTime NextTimeInclusive(const TTime& aFromTime) const;

	
	Public functions of CMsvOffPeakTimes:

	Tested	Function
	-------	---------------------------------------------------------------------
	Yes		CMsvOffPeakTimes();
	Yes		void StoreL(CMsvStore& aStore) const;
	Yes		void RestoreL(CMsvStore& aStore);
			const TInt GetNextOffPeakTime(const TTime& aFromTime, TMsvOffPeakTime& aNext, TTime& aNextTime) const;
	// This last one tested by TestReschedulingMessagesL()
*/

	AssertL(DoTest(PFUN TestOffPeakTimes1L, _L("TestOffPeakTimes1L")));
	AssertL(DoTest(PFUN TestOffPeakTimes3L, _L("TestOffPeakTimes3L")));

	}

void CSchSendTest::TestSendErrorActions1L()
	{
// --- Testing TMsvSendErrorAction ---

	iRTest.Next(_L("TestSendErrorActions1L()"));

	TMsvSendErrorAction action1, action2;

	action1.iAction			= ESendActionRetryLater;
	action1.iRetries		= ESendRetriesFixed;
	action1.iRetrySpacing	= ESendRetrySpacingStatic;
	action1.iError			= KErrNotFound;
	action1.SetMaxRetries(10);
	
	action2 = action1;

	AssertL(action1==action2);

	action1.SetMaxRetries(1);
	AssertL(action1.MaxRetries() == 1);

	action1.SetMaxRetries(200);
	AssertL(action1.MaxRetries() == 200);

	action1.Reset();
	TMsvSendErrorAction action3;

	AssertL(action1==action3);
	}


void CSchSendTest::TestSendErrorActions3L()
	{
	TMsvSendErrorAction action1, action2;
	action1.iAction			= ESendActionRetryLater;
	action1.iRetries		= ESendRetriesFixed;
	action1.iRetrySpacing	= ESendRetrySpacingStatic;
	action1.iError			= KErrNotFound;
	action1.SetMaxRetries(10);
	action1.Reset();
	AssertL(action1==action2);
	}

void CSchSendTest::TestDefaultL()
	{

	CMsvSendErrorActions* actions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions);

	TMsvSendErrorAction action1, action2;

	action1.iAction			= ESendActionRetryLater;
	action1.iRetries		= ESendRetriesFixed;
	action1.iRetrySpacing	= ESendRetrySpacingStatic;
	action1.iError			= KErrNotFound;
	action1.SetMaxRetries(10);
	
	actions->SetDefault(action1);
	action2 = actions->Default();

	AssertL(action2 == action1);

	CleanupStack::PopAndDestroy(); // actions
	
	}

void CSchSendTest::TestSendErrorActions5L()
	{

	CMsvSendErrorActions* actions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions);

	TMsvSendErrorAction action1, action2, action3;

	action1.iError = KErrGeneral;
	action2.iError = KErrNotSupported;

	actions->AddSendErrorActionL(action1);
	actions->AddSendErrorActionL(action2);

	TInt error = actions->GetSendErrorAction(KErrNotSupported, action3);

	AssertL(error == KErrNone);
	AssertL(action3 == action2);
	
	error = actions->GetSendErrorAction(KErrGeneral, action3);
	
	AssertL(action3 == action1);
	
	error = actions->GetSendErrorAction(KErrNotFound, action3);
	AssertL(error == KErrNotFound);

	CleanupStack::PopAndDestroy(); // actions
	}

void CSchSendTest::TestErrorsL()
	{
	CMsvSendErrorActions* actions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions);

	CArrayFixFlat<TMsvSendErrorAction>* actionArray1 = new (ELeave) CArrayFixFlat<TMsvSendErrorAction>(20);
	CleanupStack::PushL(actionArray1);
	TMsvSendErrorAction test1, test2;
	test1.iError = KErrAbort;
	test2.iError = KErrTooBig;

	TInt offSet = _FOFF(TMsvSendErrorAction, iError);
	TKeyArrayFix key(offSet, ECmpTInt);

	actionArray1->InsertIsqL(test1, key);
	actionArray1->InsertIsqL(test2, key);

	actions->SetErrorsL(*actionArray1);
	const CArrayFixFlat<TMsvSendErrorAction>& actionArray2 = actions->Errors();

	AssertL(actionArray1->Count()==actionArray2.Count());
	for(int i = 0; i < actionArray1->Count(); i++)
		{
		// VC can't find my operator== if I do this the obvious one-line way
		TMsvSendErrorAction one = (*actionArray1)[i];
		TMsvSendErrorAction two = actionArray2[i];
		AssertL(one==two);
		}
	CleanupStack::PopAndDestroy(2); // actions, actionArray1
	}

void CSchSendTest::TestStoringL()
	{
	CMsvSendErrorActions* actions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions);

	CMsvSendErrorActions* actions2 = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions2);

	// access sms account settings
	CSmsAccount* account = CSmsAccount::NewLC();

	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(scheduleSettings);
	CMsvOffPeakTimes* offPeakTimes = new(ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions();
	CleanupStack::PushL(sysAgentActions);

	// read existing settings
	TRAPD(ignor, account->LoadSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions));

	// write
	account->SaveSettingsL(*scheduleSettings, *offPeakTimes, *actions, *sysAgentActions);

	// read 
	account->LoadSettingsL(*scheduleSettings, *offPeakTimes, *actions2, *sysAgentActions);

	// restore settings	
	account->SaveSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);
	CleanupStack::PopAndDestroy(4, scheduleSettings); // sysAgentActions, errorActions, offPeakTimes, scheduleSettings 

	AssertL(*actions==*actions2);
	
	CleanupStack::PopAndDestroy(3); //actions, actions2, entry/account
	}

void CSchSendTest::TestRemovingL()
	{
	TMsvSendErrorAction a1, a2, a3;
	a1.iError = -3;
	a2.iError = -4;
	a3.iError = -5;
	CMsvSendErrorActions* actions1 = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions1);
	CMsvSendErrorActions* actions2 = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions2);

	actions1->AddSendErrorActionL(a1);
	actions1->AddSendErrorActionL(a2);
	actions1->AddSendErrorActionL(a3);

	actions2->AddSendErrorActionL(a1);
	actions2->AddSendErrorActionL(a3);

	actions1->RemoveSendErrorAction(-4);

	AssertL(*actions1==*actions2);

	CleanupStack::PopAndDestroy(2); //actions1 and 2
	}

/*void CSchSendTest::TestSendErrorActions4L()
	{
// --- Reading CMsvSendErrorActions from a resource file ---

	iRTest.Next(_L("TestSendErrorActions4L"));

	CMsvSendErrorActions* actions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(actions);

	TFileName fileName;
	TParse parse;
    parse.Set(KSmsResourceFile, &fileName, 0);
	fileName=parse.FullName();
	BaflUtils::NearestLanguageFile(iFs, fileName);
	RResourceFile iResourceFile;
	iResourceFile.OpenL(iFs, fileName);

	TResourceReader reader;
	HBufC8* rBuf = iResourceFile.AllocReadLC(ERROR_ACTIONS);
	reader.SetBuffer(rBuf);

	actions->RestoreFromResourceL(reader);

	iResourceFile.Close();
	CleanupStack::PopAndDestroy(2); //actions, rBuf
	} */



void CSchSendTest::TestOffPeakTimes1L()
	{
	iRTest.Next(_L("TestOffPeakTimes1L"));


	//Make an off peak time
	TInt hour = 10;
	TInt min = 10;
	TDay day = EMonday;
	TTimeIntervalMinutes valPrd = 5*60;

	TMsvOffPeakTime offPeak;
	offPeak.SetHour(hour);
	offPeak.SetMinute(min);
	offPeak.SetDay(day);
//	offPeak.SetValidityPeriod(-1);  PASSED OK
	offPeak.SetValidityPeriod(valPrd);

	//Check that it worked
	AssertL(offPeak.Hour() == hour);
	AssertL(offPeak.Minute() == min);
	AssertL(offPeak.Day() == day);
	AssertL(offPeak.ValidityPeriod() == valPrd);

	
	TTime now;
	now.HomeTime();
	TTime nextTime = now;
	nextTime = offPeak.NextTimeInclusive(now);

	TBuf<30> dateString;
	now.FormatL(dateString,(_L("%D%M%Y%/0%1%/1%2%/2%3%/3  %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B")));
    Printf(_L("Now: "));
	Printf(dateString);
	Printf(_L("\n"));

	nextTime.FormatL(dateString,(_L("%D%M%Y%/0%1%/1%2%/2%3%/3  %-B%:0%J%:1%T%:2%S%.%*C4%:3%+B")));
    Printf(_L("Next Time: "));
	Printf(dateString);
	Printf(_L("\n"));

	AssertL(StartsAt(offPeak, nextTime));

	TTime fourHoursAgo = now - (TTimeIntervalHours) 4;
	TDateTime dtFourHoursAgo = fourHoursAgo.DateTime();

	hour = dtFourHoursAgo.Hour();
	min  = dtFourHoursAgo.Minute();
	day = fourHoursAgo.DayNoInWeek();
	valPrd = 8*60;  // valid for eight hours
	
	TMsvOffPeakTime nowIsOffPeak(day, hour, min, valPrd);
	nextTime = nowIsOffPeak.NextTimeInclusive(now);

    Printf(_L("Now: "));
	iRTest<<now;
    Printf(_L("\nFour Hours Ago: "));
	iRTest<<fourHoursAgo;
	Printf(_L("\nNext Time: "));
	iRTest<<nextTime;

	AssertL(nextTime < now);
	AssertL(StartsAt(nowIsOffPeak, fourHoursAgo));
	}


void CSchSendTest::TestOffPeakTimes3L()
	{
	iRTest.Next(_L("TestOffPeakTimes3L"));

	CMsvOffPeakTimes* offPeakTimes1 = new (ELeave) CMsvOffPeakTimes();
	CleanupStack::PushL(offPeakTimes1);
	CMsvOffPeakTimes* offPeakTimes2 = new (ELeave) CMsvOffPeakTimes();
	CleanupStack::PushL(offPeakTimes2);

	TMsvOffPeakTime offPeak1, offPeak2;

	offPeakTimes1->AppendL(offPeak1);

	offPeak2.SetDay(EWednesday);
	offPeak2.SetHour(20);
	offPeak2.SetMinute(30);
	offPeak2.SetValidityPeriod(10);

	offPeakTimes1->AppendL(offPeak2);

	// access sms account settings
	CSmsAccount* account = CSmsAccount::NewLC();

	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(scheduleSettings);
	CMsvOffPeakTimes* offPeakTimes = new(ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions();
	CleanupStack::PushL(sysAgentActions);

	// read existing settings
	TRAPD(ignor, account->LoadSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions));

	// write
	account->SaveSettingsL(*scheduleSettings, *offPeakTimes1, *errorActions, *sysAgentActions);

	// read 
	account->LoadSettingsL(*scheduleSettings, *offPeakTimes2, *errorActions, *sysAgentActions);

	// restore settings	
	account->SaveSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);
	CleanupStack::PopAndDestroy(4, scheduleSettings);

	AssertL(offPeakTimes1->Count()==offPeakTimes2->Count());
	for(int i = 0; i < offPeakTimes1->Count(); i++)
		AssertL((*offPeakTimes1)[i]==(*offPeakTimes2)[i]);

	CleanupStack::PopAndDestroy(3, offPeakTimes1); // offPeakTimes1, offPeakTimes2, entry/account
	}




void CSchSendTest::TestScheduleSettingsL()
	{
/*
	Public functions of CMsvScheduleSettings:

	Tested	Function
	-------	---------------------------------------------------------------------
	Yes		void StoreL(CMsvStore&) const;
	Yes		void RestoreL(CMsvStore&);
	Yes		void Reset();
	Yes		void SetSendExe(const TDesC&);
	Yes		const TDesC& SendExe() const;
	Yes		void SetPriority(const TInt);
	Yes		const TInt Priority() const;
	Yes		void SetValidityPeriod(const TTimeIntervalMinutes&);
	Yes		const TTimeIntervalMinutes& ValidityPeriod() const;
	Yes		void SetIntervalType(const TIntervalType);
	Yes		const TIntervalType IntervalType() const;
	Yes		void SetLongInterval(const TTimeIntervalSeconds&);
	Yes		const TTimeIntervalSeconds& LongInterval() const;
	Yes		void SetShortInterval(const TTimeIntervalSeconds&);
	Yes		const TTimeIntervalSeconds& ShortInterval() const;
	Yes		const CArrayFixFlat<TTimeIntervalSeconds>& VariableIntervals() const;
	Yes		void SetVariableIntervalsL(const CArrayFixFlat<TTimeIntervalSeconds>&);
	Yes		const TTimeIntervalSeconds& Latency() const;
	Yes		void SetLatency(const TTimeIntervalSeconds&);
*/
	iRTest.Next(_L("Schedule Settings Test - CMsvScheduleSettings"));
	
	CMsvScheduleSettings* settings1 = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(settings1);

	CMsvScheduleSettings* settings2 = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(settings2);

	settings1->SetIntervalType(EDaily);
	settings1->SetLatency(6);
	settings1->SetLongInterval(6);
	settings1->SetShortInterval(6);
	settings1->SetValidityPeriod(6);
	settings1->SetPriority(1);
	CArrayFixFlat<TTimeIntervalSeconds>* times = new (ELeave) CArrayFixFlat<TTimeIntervalSeconds>(20);
	CleanupStack::PushL(times);
	times->AppendL(6);
	times->AppendL(6);
	times->AppendL(6);
	settings1->SetVariableIntervalsL(*times);

	// access sms account settings
	CSmsAccount* account = CSmsAccount::NewLC();

	CMsvScheduleSettings* scheduleSettings = CMsvScheduleSettings::NewL();
	CleanupStack::PushL(scheduleSettings);
	CMsvOffPeakTimes* offPeakTimes = new(ELeave) CMsvOffPeakTimes;
	CleanupStack::PushL(offPeakTimes);
	CMsvSendErrorActions* errorActions = CMsvSendErrorActions::NewL();
	CleanupStack::PushL(errorActions);
	CMsvSysAgentActions* sysAgentActions = new (ELeave) CMsvSysAgentActions();
	CleanupStack::PushL(sysAgentActions);

	// read existing settings
	TRAPD(ignor, account->LoadSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions));

	// write
	account->SaveSettingsL(*settings1, *offPeakTimes, *errorActions, *sysAgentActions);

	// read 
	account->LoadSettingsL(*settings2, *offPeakTimes, *errorActions, *sysAgentActions);

	// restore settings	
	account->SaveSettingsL(*scheduleSettings, *offPeakTimes, *errorActions, *sysAgentActions);
	CleanupStack::PopAndDestroy(4, scheduleSettings);

// 	Not doing anything?
//	CMsvScheduleSettings* aDefaultSettings = CMsvScheduleSettings::NewL();
//	CleanupStack::PushL(aDefaultSettings);
//	aSettings2->Reset();

//	AssertL(*aDefaultSettings==*aSettings2);

	CleanupStack::PopAndDestroy(4, settings1); // settings1, settings2, times, entry/account
	}

void CSchSendTest::TestSysAgentActionsL()
	{
	CMsvSysAgentActions* actions = new (ELeave) CMsvSysAgentActions();
	CleanupStack::PushL(actions);
	CleanupStack::PopAndDestroy(actions);
	}

void CSchSendTest::TestSchedulingMessagesL()
	{

	//make a message and schedule it in 1 second
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
//	t += TTimeIntervalSeconds(2);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);	

	iRTest.Printf(_L("Message created at "));
	iRTest << t;
	
	iSelection->ResizeL(0);
	iSelection->AppendL(testMessage);

	iMsvEntry->SetEntryL(testMessage);
	AssertL(iMsvEntry->Entry().SendingState() == 0);
	TTime t2 = TestL(EScheduleAllL, *iSelection).iTime;
	
	iRTest.Printf(_L("Message scheduled at "));
	iRTest << t2;
	//Test that the time is still the same
	
	//check that it worked
	
	iMsvEntry->SetEntryL(testMessage);
	iRTest.Printf(_L("Message iDate "));
	iRTest << iMsvEntry->Entry().iDate;

	AssertL(CompareRoundedTimes(iMsvEntry->Entry().iDate, t2));

	const TInt sendState = iMsvEntry->Entry().SendingState();
	AssertL(sendState == KMsvSendStateSent);

	CleanupStack::PopAndDestroy();//testMessage
	}

void CSchSendTest::TestSchedulingOldMessagesL()
	{
	//Make a message and schedule it yesterday
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t -= TTimeIntervalDays(1);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	

	iSelection->ResizeL(0);
	iSelection->AppendL(testMessage);
	
	
	TTime t2(TestL(EScheduleAllL, *iSelection, ETrue).iTime);

	//did it get scheduled?

	iMsvEntry->SetEntryL(testMessage);
	AssertL(iMsvEntry->Entry().SendingState() == KMsvSendStateScheduled);

	TTime now;
	now.HomeTime();

	//should be scheduled in the future 

	iRTest << t2 << now;
	AssertL(t2 > now);

	ChangeMessageTimeL(testMessage, t);

	CleanupStack::PopAndDestroy();//testMessage
	}

void CSchSendTest::TestSchedulingOffPeakMessagesL()
	{
	
	//make a message and schedule it for off-peak
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t += TTimeIntervalSeconds(1);
	iRTest.Printf(_L("Trying to schedule at"));
	iRTest<<t;
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t, ETrue);

	iSelection->ResizeL(0);
	iSelection->AppendL(testMessage);
	
	iSchTestActive->TransferCommandSyncL(ESetNowOffPeak, *iSelection, ETrue);

	ChangeMessageTimeL(testMessage, t);
	TTime t2 = TestL(EScheduleAllL, *iSelection, ETrue).iTime;
	ChangeMessageTimeL(testMessage, t);

	iMsvEntry->SetEntryL(testMessage);
	TMsvEntry resultingEntry = iMsvEntry->Entry();
	AssertL(resultingEntry.SendingState() == KMsvSendStateScheduled);

	TTime now;
	now.HomeTime();

	AssertL(DiffInMins(t2, now) < 5);   // About now

	iSchTestActive->TransferCommandSyncL(ESetNowNotOffPeak, *iSelection, ETrue);
	
	t2 = TestL(EScheduleAllL, *iSelection, ETrue).iTime;


	AssertL(DiffInMins(t2, now) > (60 * 22)); // Tomorrow sometime

	iSchTestActive->TransferCommandSyncL(ESetFirstOffPeakBest, *iSelection, ETrue);
	
	t2 = TestL(EScheduleAllL, *iSelection, ETrue).iTime;

	AssertL(DiffInMins(t2, now) > (60 * 22)); // Tomorrow sometime

	iSchTestActive->TransferCommandSyncL(ESetLastOffPeakBest, *iSelection, ETrue);
	t2 = TestL(EScheduleAllL, *iSelection, ETrue).iTime;

	AssertL(DiffInMins(t2, now) > (60 * 22)); // Tomorrow sometime

	//Tidy up after ourselves!
	iSchTestActive->TransferCommandSyncL(EDeleteScheduleL, *iSelection, ETrue);
	
	CleanupStack::PopAndDestroy();//testMessage
	}


void CSchSendTest::TestReschedulingMessagesL()
	{
	//Make a message and schedule it in 1 second
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t += TTimeIntervalSeconds(4);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	

	iSelection->ResizeL(0);
	iSelection->AppendL(testMessage);

	iSchTestActive->TransferCommandSyncL(EScheduleAllL, *iSelection, ETrue);
	
	//Set the retry time
	iSchTestActive->TransferCommandSyncL(ESetRetryImmediately, *iSelection, ETrue);

	//Reschedule the message

	TTime t2 = TestL(EReScheduleAllL, *iSelection, ETrue).iTime;

	//Still scheduled?

	iMsvEntry->SetEntryL(testMessage);
	AssertL(iMsvEntry->Entry().SendingState() == KMsvSendStateResend);

	//Check the time is in KShortInterval seconds from t

	TTime test = t + TTimeIntervalSeconds(KShortInterval);
	iRTest.Printf(_L("KShortInterval seconds different "));
	iRTest<<t2<< test;
	AssertL(DiffInSecs(test, t2) < 10);

	//Resend several times. Each time AssertL that the time is correct.

	iSchTestActive->TransferCommandSyncL(ESetRetryLater, *iSelection, ETrue);
	t2 = TestL(EReScheduleAllL, *iSelection, ETrue).iTime;

	test = t + TTimeIntervalSeconds(KLongInterval);
	iRTest.Printf(_L("KLongInterval seconds different"));
	iRTest<< t2 << test;
	AssertL(DiffInSecs(test,t2)< 5);

	iSchTestActive->TransferCommandSyncL(ESetNoRetry, *iSelection, ETrue);
	iSchTestActive->TransferCommandSyncL(EReScheduleAllL, *iSelection, ETrue);

	iSchTestActive->TransferCommandSyncL(ESetRetryVariable, *iSelection, ETrue);
	t2 = TestL(EReScheduleAllL, *iSelection, ETrue).iTime;

	test = t + TTimeIntervalSeconds(KFirstInterval);
	iRTest.Printf(_L("KFirstInterval seconds different"));
	iRTest<< t2 << test;
	AssertL(DiffInSecs(test, t2) < 5);

	t2 = TestL(EReScheduleAllL, *iSelection, ETrue).iTime;

	test = t + TTimeIntervalSeconds(KSecondInterval);
	iRTest.Printf(_L("KSecondInterval seconds different"));
	iRTest<<t2<< test;
	AssertL(DiffInSecs(test, t2) < 5);

	t2 = TestL(EReScheduleAllL, *iSelection, ETrue).iTime;

	test = t + TTimeIntervalSeconds(KThirdInterval);
	iRTest.Printf(_L("KThirdInterval seconds different"));
	iRTest<<t2<< test;
	AssertL(DiffInSecs(test, t2) < 5);

	//Tidy up.

	iSchTestActive->TransferCommandSyncL(EDeleteScheduleL, *iSelection, ETrue);

	CleanupStack::PopAndDestroy();//testMessage
	}

void CSchSendTest::TestReschedulingNonScheduledMessagesL()
	{
	//Make a message and schedule it in 1 second
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t += TTimeIntervalSeconds(1);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	
	iSelection->Reset();
	iSelection->AppendL(testMessage);
	
	//Reschedule the message

	iSchTestActive->TransferCommandSyncL(EReScheduleAllL, *iSelection, ETrue);

	//Are we scheduled now?
	
	iMsvEntry->SetEntryL(testMessage);
	AssertL(iMsvEntry->Entry().SendingState() == KMsvSendStateResend);

	//Tidy up.
	iSchTestActive->TransferCommandSyncL(EDeleteScheduleL, *iSelection, ETrue);

	CleanupStack::PopAndDestroy();//testMessage
	}

void CSchSendTest::TestSchedulingMessagesTwiceL()
	{
	//make a message
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t += TTimeIntervalSeconds(1);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	

	//schedule it for real soon now
	iSelection->Reset();
	iSelection->AppendL(testMessage);
	
	iSchTestActive->TransferCommandSyncL(EScheduleAllL, *iSelection, ETrue);

	//schedule it for tomorrow too
	t += TTimeIntervalDays(1);
	ChangeMessageTimeL(testMessage, t);
	
	//TTime t2 = TestL(EScheduleAllL, *iSelection, ETrue));
	
	//assert message scheduled.

	iMsvEntry->SetEntryL(testMessage);
	AssertL(iMsvEntry->Entry().SendingState() == KMsvSendStateScheduled);

	//look for the message

	TTime now;
	now.HomeTime();

	//check its time is tomorrow
//	AssertL(DiffInMins(t2,now) > (23*60)); 

	//Get rid of it
	iSchTestActive->TransferCommandSyncL(EDeleteScheduleL, *iSelection, ETrue);

	CleanupStack::PopAndDestroy();//testMessage
	}


void CSchSendTest::TestDeletingMessagesL()
	{
	//make a message and schedule it for tomorrow
	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t += TTimeIntervalDays(1);
	iSelection->Reset();

	TMsvId testMessage1 = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	iSelection->AppendL(testMessage1);
	TMsvId testMessage2 = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	iSelection->AppendL(testMessage2);
	TMsvId testMessage3 = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);
	iSelection->AppendL(testMessage3);

	TInt schCountOld, schCountNew;
	TInt schSize;
	DisplayAllSchedulesL(schCountOld, schSize);

	iSchTestActive->TransferCommandSyncL(EScheduleAllL, *iSelection, ETrue);
	
	//delete the schedule

	iSchTestActive->TransferCommandSyncL(EDeleteScheduleL, *iSelection, ETrue);

	DisplayAllSchedulesL(schCountNew, schSize);

//	AssertL (schCountNew <= schCountOld);

	//deleted message should now be suspended
	
	iMsvEntry->SetEntryL(testMessage1);
	AssertL (iMsvEntry->Entry().SendingState()==KMsvSendStateSuspended);	
	iMsvEntry->SetEntryL(testMessage2);
	AssertL (iMsvEntry->Entry().SendingState()==KMsvSendStateSuspended);	
	iMsvEntry->SetEntryL(testMessage3);
	AssertL (iMsvEntry->Entry().SendingState()==KMsvSendStateSuspended);	

	CleanupStack::PopAndDestroy(3);//testMessages
	}

void CSchSendTest::TestDeletingNonScheduledMessagesL()
	{
	//make a message but don't schedule it
 	TMsvEntry entry;
	TTime t;
	t.HomeTime();
	t += TTimeIntervalDays(1);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t);

	//delete the message from the scheduler
	iSelection->ResizeL(0);
	iSelection->AppendL(testMessage);

	iSchTestActive->TransferCommandSyncL(EDeleteScheduleL, *iSelection, ETrue);
	
	//deleted message should now be suspended
	
	iMsvEntry->SetEntryL(testMessage);
	AssertL (iMsvEntry->Entry().SendingState()==KMsvSendStateSuspended);

	CleanupStack::PopAndDestroy();//testMessage
	}

void CSchSendTest::TestCheckScheduleL()
	{
	//make a message and schedule it at T1
 	TMsvEntry entry;
	TTime t1;
	t1.HomeTime();
	t1 += TTimeIntervalHours(1);
	TMsvId testMessage = CreateMessageLC(entry, KMsvGlobalInBoxIndexEntryId, t1);
	
	iSchTestActive->TransferCommandSyncL(EScheduleAllL, *iSelection, ETrue);

	//change the date in the message to T2

	TTime t2 = t1 + TTimeIntervalHours(1);
	ChangeMessageTimeL(testMessage, t2);

	//check the schedule
	iSchTestActive->TransferCommandSyncL(ECheckScheduleL, *iSelection, ETrue);

	//the date should now be T1

	iMsvEntry->SetEntryL(testMessage);
	//TMsvEntry newEntry = iMsvEntry->Entry();

	//AssertL(newEntry.iDate == t1);  // not implemented yet

	CleanupStack::PopAndDestroy();//testMessage
	}

// leaves unless the test leaves with KErrNoMemory for every memory failure
const TSchSendTestProgress& CSchSendTest::TestL(TSchSendTestOperation aOp,
						 CMsvEntrySelection& aSel,
						 TBool aBool /*= EFalse*/)
	{
	TInt error = KErrNone;
	if (iHeapTest) 
		{
		iSchTestActive->TransferCommandSyncL(ESetIncrementalHeapFailure, *iSelection, ETrue);
		do
			{
	
			TRAP(error, 
				iSchTestActive->TransferCommandSyncL(aOp, aSel, aBool);
			);
	
			} while(error == KErrNoMemory);

		iSchTestActive->TransferCommandSyncL(ENoIncrementalHeapFailure, *iSelection, ETrue);

		User::LeaveIfError(error);
		return iSchTestActive->Progress();
		} 
	else 
		{
		return iSchTestActive->TransferCommandSyncL(aOp, aSel, aBool);
		}
	}

// Returns true if the test does not leave
// Also if the test leaves with KErrNone, but that would be pathological.

// Logs the test start/finish messages
TBool CSchSendTest::DoTest(void (CSchSendTest::*aTest)(), TPtrC aText)
	{
	TestStart(++iCurrentTest, aText);
	TRAPD(err, (this->*aTest)());
	TestFinish(iCurrentTest, err);
	return (err == KErrNone);
	}

TBool CSchSendTest::HeapTest(void (CSchSendTest::*aTest)(), TPtrC aText)
	{
	TestStart(++iCurrentTest, aText);
//	WriteComment(aText);
	iHeapTest = ETrue;
	TRAPD(err, (this->*aTest)());
	TestFinish(iCurrentTest, err);
	iHeapTest = EFalse;
	return (err == KErrNone);
	}

