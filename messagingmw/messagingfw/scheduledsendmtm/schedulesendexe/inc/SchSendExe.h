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


#ifndef SCHSENDEXE_H_
#define SCHSENDEXE_H_

#ifdef _DEBUG
#undef _MSG_NO_LOGGING
#endif


#include <msvschedulesend.h> //for SCHSENDLOG and KSchSendLogDir

#ifdef _DEBUG
//#define _SCHSENDEXE_TEST //undefine when submitting code
#endif

#ifdef _SCHSENDEXE_TEST
#pragma message("WARNING: Test code enabled")
const TInt KMsvSendExeOneMinute = 1000000 * 60 / 30;
#else
const TInt KMsvSendExeOneMinute = 1000000 * 60;
#endif

const TInt KSendExeActiveTimeout = KMsvSendExeOneMinute / 12; // 10 secs
const TInt KSchSendExeMinReschedule = KMsvSendExeOneMinute * 1; // 1 mins
const TInt KSchSendExeMaxReschedule = KMsvSendExeOneMinute * 3; // 4 mins
const TInt KDefaultPriority = CActive::EPriorityStandard;
const TInt KMsvSendExeGrowth = 5;

class CScheduledTask;

//
//
//	Local Function Declarations
//
//

enum TSchSendExePanic
	{
	EMultipleMtms,
	ENoMessagesInSelection,
	ENoSendCommand,
	EOperationCompletedTooSoon,
	ETooManyOperationsCompleted,
	EObjectNotNull
	};

void gPanic(TSchSendExePanic aPanic);
void gPanic(TInt aPanic);

TInt Execute(const TDesC& aFileName, RFile& aFile );
void DoExecuteL(const TDesC& aFileName, RFile& aFile );

class CMsvSendExeActive;


/**

The CProgressTimer class
Calls iOperation.ProgressL() every iPollInterval microseconds.
Used by CMsvSendExeActive.

*/
class CProgressTimer : public CTimer
	{
public:
	static CProgressTimer* NewL(CMsvOperation& aOperation, const TTimeIntervalMicroSeconds32& aPollInterval);
	void Start();
	~CProgressTimer();
	
private:
	CProgressTimer(CMsvOperation& aOperation, const TTimeIntervalMicroSeconds32& aPollInterval);
	void RunL();
	CMsvOperation& iOperation;
	TTimeIntervalMicroSeconds32 iPollInterval;
	};



/**

The CMsvSendExe class
Reads the task file and starts the corresponding messaging operations.

*/
class CMsvSendExe : public MMsvSessionObserver, CActive
	{
public:
	
	static CMsvSendExe* StartLC(const TDesC& aFileName, RFile& aFile );

	~CMsvSendExe();

	void HandleSessionEventL(MMsvSessionObserver::TMsvSessionEvent /*aEvent*/, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/) {};
	TBool IsSendActive(); // {return IsActive();}
	void OperationComplete(const CMsvSendExeActive& aActive, TInt aError);

#ifndef _MSG_NO_LOGGING
	static void FLog(const TDesC& aFileName, TRefByValue<const TDesC16> aFmt, ...);
#endif

private:

	CMsvSendExe(const TDesC& aFileName, RFile& aFile );

	void ConstructL();

	void DoCancel();
	void RunL();

	void ProcessFileL();
	void CallMtmL();
	
	void RetrievePackagesL();
	void AddPackageL(const CScheduledTask& aTask);

	void RetrieveMessagesL();

	void AddTaskL(const TMsvSchedulePackage& aPackage);
	void AddTaskL(const TMsvSchedulePackage& aPackage, const TUid& aMtm);

	void RescheduleOnError(const CMsvSendExeActive& aActive, TInt aError);
	void RescheduleOnError(const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage, TInt aError);
	void RescheduleAllL(TInt aError);
	void CompleteReschedule(RScheduler& aScheduler, const TSchedulerItemRef& aRef, TInt aCount);
	
	void DoResheduleOnErrorL(RScheduler& aScheduler, const CMsvEntrySelection& aSelection, const TMsvSchedulePackage& aPackage, TInt aError, TSchedulerItemRef& aRef, TInt& aCount, TTime& aStartTime);
	void FailOnError(const CMsvEntrySelection& aActive, TInt aError);
	void RestoreScheduleSettingsL(TMsvId aServiceId, const TUid& aMtm, CMsvScheduleSettings& aSettings);
	TBool FindorCreateScheduleL(RScheduler& aScheduler, TTime& aStartTime, const CMsvScheduleSettings& aSettings, TSchedulerItemRef& aRef);
	void UpdateEntryL(CMsvEntry& aMsvEntry, TMsvEntry& aEntry, const TSchedulerItemRef& aRef, const TTaskInfo& aInfo, const TTime& aStartTime);

	CMsvSession* iSession;
	const TDesC& iFileName;
	
	RFile& iTaskFile;

	TTime iCreated;
	RPointerArray<CMsvSendExeActive> iOperations;
	RArray<TMsvSchedulePackage> iPackages;
	CActiveSchedulerWait iActiveSchedulerWait;
	
#ifdef _DEBUG
	TInt iOperationsComplete;
#endif
	};

class CMsvSendExeActive : public CActive
	{
	public:
		static CMsvSendExeActive* NewLC(CMsvSession& aSession, CMsvSendExe& aParent, const TMsvSchedulePackage& aPackage, const TUid& aMtm, TInt aPriority = EPriorityStandard);
		~CMsvSendExeActive();
		void StartL();
		void AppendL(TMsvId aId);
		inline TInt MsgCount() const;
		inline const TMsvSchedulePackage& Package() const;
		inline const TUid& Mtm() const;
		inline const CMsvEntrySelection& Selection() const;
		
	private:
		CMsvSendExeActive(CMsvSession& aSession, CMsvSendExe& aParent, const TMsvSchedulePackage& aPackage, const TUid& aMtm, TInt aPriority);
		void ConstructL();
		void RunL();
		void DoCancel();

	private:

		CMsvSession& iSession;
		CMsvSendExe& iParent;
		CProgressTimer* iTimer;
		CMsvOperation* iOperation;
		CMsvEntrySelection* iSelection;
		TMsvSchedulePackage iPackage;
		TUid iMtm;
	};

inline TInt CMsvSendExeActive::MsgCount() const
	{
	return iSelection->Count();
	}

inline const CMsvEntrySelection& CMsvSendExeActive::Selection() const
	{
	return *iSelection;
	}

inline const TMsvSchedulePackage& CMsvSendExeActive::Package() const
	{
	return iPackage;
	}

inline const TUid& CMsvSendExeActive::Mtm() const
	{
	return iMtm;
	}

#endif
