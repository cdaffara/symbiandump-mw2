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

#include <e32base.h>
#include <msvstd.h>
#include <msventry.h>
#include <msvschedulesettings.h>
#include <schsend_panic.h>

/**
Allocates and creates a new CMsvScheduleSettings object.

@return New object
*/
EXPORT_C CMsvScheduleSettings* CMsvScheduleSettings::NewL()
	{
	CMsvScheduleSettings* self = CMsvScheduleSettings::NewLC();
	CleanupStack::Pop(self); 
	return self;
	}

/**
Allocates and creates a new CMsvScheduleSettings object.

@return New object
*/
EXPORT_C CMsvScheduleSettings* CMsvScheduleSettings::NewLC()
	{
	CMsvScheduleSettings* self = new (ELeave) CMsvScheduleSettings();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CMsvScheduleSettings::ConstructL()
	{
	iVariableIntervals = new (ELeave) CArrayFixFlat<TTimeIntervalSeconds>(1);
	}

/*
	Sets all the members to their default value.
*/
CMsvScheduleSettings::CMsvScheduleSettings()
	{
	Reset();
	}

/**
Destructor.
*/
EXPORT_C CMsvScheduleSettings::~CMsvScheduleSettings()
	{
	delete iVariableIntervals;
	}

/**
Resets the object.

This sets all data members to their default values.
*/
EXPORT_C void CMsvScheduleSettings::Reset()
	{
	iPriority		= EDefaultPriority;
	iValidityPeriod = EDefaultValidityPeriod;
	iIntervalType	= (TIntervalType) EDefaultIntervalType;

	iShortInterval	= EDefaultShortInterval;
	iLongInterval	= EDefaultLongInterval;

	iLatency		= EDefaultLatency;
	iPendingConditionsTimeout = EDefaultPendingConditionsTimeout;

	if (iVariableIntervals)
		iVariableIntervals->Reset();
	}


/**
Gets the priority of the messages on the Task Scheduler. 

@return Priority value
*/

EXPORT_C TInt CMsvScheduleSettings::Priority() const
	{
	return iPriority;
	}

/**
Sets the priority of the messages on the Task Scheduler. 

@param aPriority
Priority value.
*/

EXPORT_C void CMsvScheduleSettings::SetPriority(const TInt aPriority)
	{
	iPriority = aPriority;
	}

/**
Gets the time period for which the messages are valid on the Task Scheduler. 

@return Validity period
*/
EXPORT_C const TTimeIntervalMinutes& CMsvScheduleSettings::ValidityPeriod() const
	{
	return iValidityPeriod;
	}

/**
Sets the time period for which the messages are valid on the Task Scheduler. 

This is ignored if a message has to be sent off-peak.

@param aValidityPeriod
Validity period.

@panic ScheduleSend-DLL 10
The validity period is invalid (negative or null).
*/
EXPORT_C void CMsvScheduleSettings::SetValidityPeriod(const TTimeIntervalMinutes& aValidityPeriod)
	{
	__ASSERT_ALWAYS(aValidityPeriod.Int() > 0, gPanic(EInvalidValidityPeriod));
	
	iValidityPeriod = aValidityPeriod;
	}

/**
Gets the schedule interval type.

@return The schedule interval type
*/

EXPORT_C TIntervalType CMsvScheduleSettings::IntervalType() const
	{
	return iIntervalType;
	}

/**
Sets the schedule interval type.

@param aIntervalType
The schedule interval type.
*/
EXPORT_C void CMsvScheduleSettings::SetIntervalType(const TIntervalType aIntervalType)
	{
	iIntervalType = aIntervalType;
	}

/**
Sets the long retry interval value.

This is used by CMsvScheduleSend to determine when to next send the message, 
if the TMsvSendErrorAction::iAction equals ESendActionRetryLater and 
TMsvSendErrorAction::iRetrySpacing equals ESendRetriesFixed.

@param aInterval
Long interval value.

@panic ScheduleSend-DLL 18
The long interval is out of range (negative or null).
*/
EXPORT_C void CMsvScheduleSettings::SetLongInterval(const TTimeIntervalSeconds& aInterval)
	{
	__ASSERT_ALWAYS(aInterval.Int() > 0, gPanic(ELongIntervalOutOfRange)); //TO DO: Always or Debug only?

	iLongInterval = aInterval;
	}

/**
Gets the long retry interval value.

@return Long interval value

@panic ScheduleSend-DLL 18
The long interval is out of range (negative or null).
*/
EXPORT_C const TTimeIntervalSeconds& CMsvScheduleSettings::LongInterval() const
	{
	__ASSERT_ALWAYS(iLongInterval.Int() > 0, gPanic(ELongIntervalOutOfRange)); //TO DO: Always or Debug only?

	return iLongInterval;
	}

/**
Sets the short retry interval value.

This is used by CMsvScheduleSend to determine when to next send the message, 
if the TMsvSendErrorAction::iAction equals ESendActionRetryImmediately.

@param aInterval
Short interval value.

@panic ScheduleSend-DLL 19
The short interval is out of range (negative or null).
*/
EXPORT_C void CMsvScheduleSettings::SetShortInterval(const TTimeIntervalSeconds& aInterval)
	{
	__ASSERT_ALWAYS(aInterval.Int() > 0, gPanic(EShortIntervalOutOfRange)); //TO DO: Always or Debug only?

	iShortInterval = aInterval;
	}

/**
Gets the short retry interval value.

@return Short interval value

@panic ScheduleSend-DLL 19
The short interval is out of range (negative or null).
*/
EXPORT_C const TTimeIntervalSeconds& CMsvScheduleSettings::ShortInterval() const
	{
	__ASSERT_ALWAYS(iShortInterval.Int() > 0, gPanic(EShortIntervalOutOfRange)); //TO DO: Always or Debug only?

	return iShortInterval;
	}

/**
Sets variable retry intervals.

@param aIntervals
Variable retry intervals.

@leave One of the system wide error codes
One of the intervals could not be appended to the array holding the variable
intervals.

@panic ScheduleSend-DLL 20
At least one of the intervals is out of range (negative or null).
*/
EXPORT_C void CMsvScheduleSettings::SetVariableIntervalsL(const CArrayFixFlat<TTimeIntervalSeconds>& aIntervals)
	{
	iVariableIntervals->Reset();

	TInt count = aIntervals.Count();

	for (TInt curInt = 0; curInt < count; curInt++)
		{
		TTimeIntervalSeconds interval = aIntervals[curInt];

		__ASSERT_ALWAYS(interval.Int() > 0, gPanic(EVariableIntervalOutOfRange)); //TO DO: Always or Debug only?

		iVariableIntervals->AppendL(interval);
		}
	}

/**
Gets variable retry intervals.

This is used by CMsvScheduleSend to determine when to next send the message. 
It is only used if TMsvSendErrorAction::iAction equals ESendActionRetryLater 
and TMsvSendErrorAction::iRetrySpacing equals ESendRetrySpacingVariable.

@return Variable retry intervals
*/
EXPORT_C const CArrayFixFlat<TTimeIntervalSeconds>& CMsvScheduleSettings::VariableIntervals() const
	{
	return *iVariableIntervals;
	}


/**
Sets the minimum message sending latency.

@param aLatency
Minimum message sending latency.

@panic ScheduleSend-DLL 25
The latency is invalid (negative).
*/
EXPORT_C void CMsvScheduleSettings::SetLatency(const TTimeIntervalMicroSeconds32& aLatency)
	{
	__ASSERT_ALWAYS(aLatency.Int() >= 0, gPanic(ELatencyOutOfRange)); //TO DO: Always or Debug only?

	iLatency = aLatency;
	}

/**
Gets the minimum message sending latency.

This is the minimum amount of time from the current time that must elapse 
before the message is sent. This must be greater than or equal to zero. 

If the client specifies that the message should be scheduled in the past, 
then CMsvScheduleSend will actually schedule the message in Latency() seconds 
from the current time.

@return Minimum message sending latency

@panic ScheduleSend-DLL 25
The latency is invalid (negative).
*/
EXPORT_C const TTimeIntervalMicroSeconds32& CMsvScheduleSettings::Latency() const
	{
	__ASSERT_ALWAYS(iLatency.Int() >= 0, gPanic(ELatencyOutOfRange)); //TO DO: Always or Debug only?

	return iLatency;
	}
	
/**
Sets the timeout interval when re-scheduling messages for pending conditions
to be met.

A value of zero indicates that there is no timeout when pending conditions to 
be met. The default value is zero.

@param aTimeout
The timeout interval in minutes.

@panic ScheduleSend-DLL 25
The timeout interval is invalid (negative).
*/
EXPORT_C void CMsvScheduleSettings::SetPendingConditionsTimeout(const TTimeIntervalMinutes& aTimeout)
	{
	__ASSERT_ALWAYS(aTimeout.Int() >= 0, gPanic(EPendingConditionsTimeoutOutOfRange));

	iPendingConditionsTimeout = aTimeout;
	}

/**
Gets the timeout interval when re-scheduling messages for pending conditions
to be met.

A value of zero indicates that there is no timeout when pending conditions to 
be met. The default value is zero.

@return
The timeout interval in minutes.
*/
EXPORT_C const TTimeIntervalMinutes& CMsvScheduleSettings::PendingConditionsTimeout() const
	{
	return iPendingConditionsTimeout;
	}
