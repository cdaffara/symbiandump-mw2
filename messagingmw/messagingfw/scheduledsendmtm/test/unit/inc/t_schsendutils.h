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

#include "msvtestutils.h"
#include "t_schsendshare.h"

class CSchSendTestUtils;
class CMsvScheduleSettings;
class TMsvOffPeakTime;
class TMsvSendErrorAction;
class CMsvSendErrorActions;
class TMsvSchedulePackage;

class CScheduleTestActive : public CActive, public MMsvSessionObserver
	{
	public:

		IMPORT_C static CScheduleTestActive* CScheduleTestActive::NewL(CSchSendTestUtils& aTest, TInt aPriority = CActive::EPriorityStandard);
		IMPORT_C ~CScheduleTestActive();
		IMPORT_C const TSchSendTestProgress& TransferCommandSyncL(TSchSendTestOperation aCommand, const CMsvEntrySelection& aSelection, const TDesC8& aParameter, TBool aStopAtRunL);
		IMPORT_C const TSchSendTestProgress& TransferCommandSyncL(TSchSendTestOperation aCommand, const CMsvEntrySelection& aSelection, TBool aStopAtRunL);
		const TSchSendTestProgress& Progress() const {return iProgress;}
		
	private:

		CScheduleTestActive::CScheduleTestActive(CSchSendTestUtils& aTest, TInt aPriority);
		void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*);
		void RunL();
		void DoCancel() {iOperation->Cancel();}
		TInt RunError(TInt aError);

		TSchSendTestOperation iCommand;
		CSchSendTestUtils&				iTest;
		CMsvSession&					iSession;
		CMsvEntrySelection*				iSelection;
		CMsvOperation*					iOperation;
		TBool							iStopAtRunL;
		TSchSendTestProgress			iProgress;
	};

class CSchSendTestTimer : public CTimer
    {
public:
    IMPORT_C static CSchSendTestTimer* NewL(TInt aPriority = CActive::EPriorityLow);
private:
    CSchSendTestTimer(TInt aPriority);
    void RunL();
    };

const TInt KSchSendTestWaitForStateDefault = 1000000;

class CSchSendTestWaitForState : public CActive, public MMsvSessionObserver
	{
	public:
		IMPORT_C static CSchSendTestWaitForState* NewL(CSchSendTestUtils& aTest);
		IMPORT_C ~CSchSendTestWaitForState();
		void AppendL(TInt aSendingState) {User::LeaveIfError(iSendingStates.Append(aSendingState));}
		IMPORT_C void Reset();
		IMPORT_C void StartL(const CMsvEntrySelection& aSelection);
		IMPORT_C void StartL(const CMsvEntrySelection& aSelection, TTimeIntervalMicroSeconds32 aAfter);

	private:
		CSchSendTestWaitForState(CSchSendTestUtils& aTest);
		void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
		void RunL();
		void DoCancel();
		TInt RunError(TInt aError);
		TBool CheckStateL(TMsvId aId);
		void DoStartL(const CMsvEntrySelection& aSelection, TTimeIntervalMicroSeconds32 aAfter, TBool aWaitForever);

	private:
		CSchSendTestUtils& iTest;
		CMsvEntrySelection* iSelection;
		RArray<TInt> iSendingStates;
		RTimer iTimer;
		TTimeIntervalMicroSeconds32 iAfter;
		TBool iWaitForever;
	};

class CSchSendTestUtils : public CMsvTestUtils, public MMsvSessionObserver
	{
	public:

		IMPORT_C void StartL();
	
	protected:

		IMPORT_C CSchSendTestUtils(RTest& aTest);
		IMPORT_C ~CSchSendTestUtils();
		void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
		IMPORT_C void Panic(TInt aPanic);
		IMPORT_C void ConstructL();

		virtual void ShowMenuL() = 0;
		virtual void RunAutoL() = 0;

		//From CMsvTestUtils
		void InstantiateClientMtmsL(){};
		void InstantiateServerMtmsL(){};
		void DeleteServicesL(){};
		void CreateServicesL(){};
		void FindExistingServicesL(){};
		IMPORT_C void InstallMtmGroupsL();
		IMPORT_C void CreateServerMtmRegsL();

		IMPORT_C TMsvId CreateMessageLC(TMsvEntry& aEntry, TMsvId aDestFolder, TTime aSendTime, TBool aOffPeak = EFalse);
		IMPORT_C TInt DiffInMins(TTime d1, TTime d2);
		IMPORT_C TInt DiffInSecs(TTime d1, TTime d2);

		IMPORT_C void ChangeMessageTimeL(TMsvId aMessage, TTime aTime);
		IMPORT_C void AssertL(TBool b);

		IMPORT_C void CleanScheduleFolderL();
		IMPORT_C void DisplayAllSchedulesL(TInt& rSchCount, TInt& rScheduleSize);
		IMPORT_C TTime RoundUpToMinute(const TTime& aTime) const;
		IMPORT_C TTime RoundDownToMinute(const TTime& aTime) const;
		IMPORT_C TBool CompareRoundedTimes(const TTime& aLeft, const TTime& aRight) const;
	
	protected:
	
		CMsvEntrySelection*		iSelection;
		CScheduleTestActive*	iSchTestActive;
		TInt					iCurrentTest;
		TBool					iHeapTest;
	};

IMPORT_C TBool operator==(const CMsvScheduleSettings& aSettings1, const CMsvScheduleSettings& aSettings2);
IMPORT_C TBool StartsAt(const TMsvOffPeakTime& time1, const TTime& time2);
IMPORT_C TBool operator==(const TMsvOffPeakTime& time1, const TMsvOffPeakTime& time2);
IMPORT_C TBool operator==(const TMsvSendErrorAction& a1, const TMsvSendErrorAction& a2);
IMPORT_C TBool operator==(const CMsvSendErrorActions& one, const CMsvSendErrorActions& two);
IMPORT_C TBool operator==(const TMsvSchedulePackage& aOne, const TMsvSchedulePackage& aTwo);
IMPORT_C RTest& operator<<(RTest& aTest, TTime t);

template<class T>
TBool operator!=(T a, T b)
	{
	return !(a==b);
	}
