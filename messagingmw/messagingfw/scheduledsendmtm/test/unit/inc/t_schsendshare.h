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

#ifndef __T_SCHSENDSHARE_H__
#define __T_SCHSENDSHARE_H__

const TUid KSchSendTestMtmUid = {0x1000127e};
_LIT(KSchSendTestDetails, "sch send test");

enum
	{
	KErrorActionImmediately	= -50,
	KErrorActionLater		= -51,
	KErrorActionVariable	= -52,
	KErrorActionFail		= -53,
	KErrorActionConditions	= -54,
	KErrorActionRetryFail	= -55
	};

// They don't begin with E because they aren't really enum values
enum
	{
	KShortInterval= 5,
	KLongInterval = 120,
	KFirstInterval = 60,
	KSecondInterval = 70,
	KThirdInterval = 80
	};
	
const TInt KPigeonErrFailFirstSend	= -101;

// Each option represents a combination of settings and error action

enum TSchSendTestOperation
	{
	ENoOperation,
	EScheduleAllL,
	EReScheduleAllL,
	EDeleteScheduleL,
	ESendScheduledL,
	ECheckScheduleL,
	ESetRetryImmediately,
	ESetRetryLater,
	ESetRetryVariable,
	ESetNoRetry,
	ESetNowOffPeak,
	ESetNowNotOffPeak,
	ESetFirstOffPeakBest,
	ESetLastOffPeakBest,
	ESetIncrementalHeapFailure,
	EResetIncrementalHeapFailure,
	ENoIncrementalHeapFailure,
	EScheduleOpFail,
	EOpFail,
	EReScheduleRetryAllL,
	EScheduleFailFirstSend
	};



class TSchSendTestProgress
	{
	public:
		inline TSchSendTestProgress();

		inline TSchSendTestOperation Command() const;
		inline void SetCommand(TInt aCommand);

	public:
		TInt iError;
		TTime iTime;

	private:
		TSchSendTestOperation iCommand;
	};

typedef TPckgBuf<TSchSendTestProgress> TSchSendTestProgressBuf;

inline TSchSendTestProgress::TSchSendTestProgress()
: iError(KErrNone), iCommand(ENoOperation)
	{
	iTime.HomeTime();
	}

inline TSchSendTestOperation TSchSendTestProgress::Command() const
	{
	return iCommand;
	}

inline void TSchSendTestProgress::SetCommand(TInt aCommand)
	{
	iCommand = (TSchSendTestOperation) aCommand;
	}

#endif
