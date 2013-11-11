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

#ifndef __SCHSEND_PANIC_H__
#define __SCHSEND_PANIC_H__

/**
@internalAll
@released
*/
enum TScheduleSendPanic
	{
	EMessageSelectionEmpty,
	EMessagesNotSameTime,
	EMessagesNotSameMtm,
	EMessagesNotSameOffPeak,
	EMessageNotScheduled,
	EScheduleNotCreated,
	ECannotScheduleInPast,
	EAttemptIntervalArrayEmpty,
	ETaskIdMsvIdMismatch,
	EInvalidAttemptInterval,
	EInvalidValidityPeriod,
	EInvalidMaxAttempts,
	EInvalidHour,
	EInvalidMinute,
	EInvalidSecond,
	EStartTimeTooEarly,
	EProgrammingBug,
	EVariableIntervalsNotFound,
	ELongIntervalOutOfRange,
	EShortIntervalOutOfRange,
	EVariableIntervalOutOfRange,
	EMaxRetriesMustBePositive,
	ERetryIntervalMustByPositive,
	EOffPeakTimesEmpty,
	EServerEntryNotSetToCorrectEntry,
	ELatencyOutOfRange,
	ESysAgentConditionOutOfSync,
	ESysAgentConditionTypeNotSupported,
	EReportNotNull,
	EMessagesNotSamePendingConditions,
	EPendingConditionsTimeoutOutOfRange
	};

GLREF_C void gPanic(TScheduleSendPanic aPanic);

#endif
