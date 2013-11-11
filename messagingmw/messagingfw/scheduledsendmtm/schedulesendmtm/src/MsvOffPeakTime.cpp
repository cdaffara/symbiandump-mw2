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
#include <e32std.h>
#include <msvstd.h>
#include <msventry.h>
#include <msvoffpeaktime.h>
#include <schsend_panic.h>


//
//
//	Type Class TMsvOffPeakTime 
//
//

/**
Default constructor.
*/

EXPORT_C TMsvOffPeakTime::TMsvOffPeakTime()
	{
	Reset();
	}


/**
Constructor specifying off-peak time parameters.
Note that the offpeak start time must be specified in UTC (Universal Time Coordinated)time.

@param aDay
Day start time.

@param aHour
Hour start time.

@param aMinute
Minute start time.

@param aValidityPeriod
Duration (in minutes).
*/

EXPORT_C TMsvOffPeakTime::TMsvOffPeakTime(const TDay aDay, const TInt aHour, const TInt aMinute, const TTimeIntervalMinutes aValidityPeriod)
	{
	SetDay(aDay);
	SetHour(aHour);
	SetMinute(aMinute);
	SetValidityPeriod(aValidityPeriod);
	}


/**
Resets the object to default settings.
*/

EXPORT_C void TMsvOffPeakTime::Reset()
	{
	iHour = 0;
	iMinute = 0;
	iDay = EMonday;
	iValidityPeriod = (TTimeIntervalMinutes) 0;
	}


/*
	Hour Access Functions
*/

/**
Gets the start time hour parameter.

@return Start time hour parameter
*/

EXPORT_C TInt TMsvOffPeakTime::Hour() const
	{
	return iHour;
	}


/**
Sets the start time hour parameter.
Note that the offpeak start time must be specified in UTC (Universal Time Coordinated)time.

@param aHour
Start time hour parameter.

@panic ScheduleSend-DLL 12
The hour parameter is invalid (negative or more than 23).
*/

EXPORT_C void TMsvOffPeakTime::SetHour(const TInt aHour)
	{
	__ASSERT_ALWAYS((aHour >= 0) && (aHour < 24), gPanic(EInvalidHour));

	iHour	= (TInt8) aHour;
	}


/*
	Minute Access Functions
*/

/**
Gets the start time minute parameter.

@return Start time minute parameter.
*/

EXPORT_C TInt TMsvOffPeakTime::Minute() const
	{
	return iMinute;
	}


/**
Sets the start time minute parameter.
Note that the offpeak start time must be specified in UTC (Universal Time Coordinated)time.

@param aMinute
Start time minute parameter.

@panic ScheduleSend-DLL 13
The minute parameter is invalid (negative or more than 59).
*/

EXPORT_C void TMsvOffPeakTime::SetMinute(const TInt aMinute)
	{
	__ASSERT_ALWAYS((aMinute >= 0) && (aMinute < 60), gPanic(EInvalidMinute));

	iMinute	= (TInt8) aMinute;
	}


/*
	Day Access Functions
*/

/**
Gets the start time day parameter.

@return Start time day parameter.
*/

EXPORT_C TDay TMsvOffPeakTime::Day() const
	{
	return iDay;
	}


/**
Sets the start time day parameter.
Note that the offpeak start time must be specified in UTC (Universal Time Coordinated)time.

@param aDay
Start time day parameter.
*/

EXPORT_C void TMsvOffPeakTime::SetDay(const TDay aDay)
	{
	iDay = aDay;
	}


/*
	ValidityPeriod Access Functions	
*/

/**
Gets the duration parameter.

@return Duration parameter.
*/

EXPORT_C const TTimeIntervalMinutes TMsvOffPeakTime::ValidityPeriod() const
	{
	return iValidityPeriod;
	}


/**
Sets the duration parameter.

@param aValidityPeriod
Duration parameter.

@panic ScheduleSend-DLL 10
The duration period is invalid (negative or more than 24 hours).
*/

EXPORT_C void TMsvOffPeakTime::SetValidityPeriod(const TTimeIntervalMinutes aValidityPeriod)
	{
	__ASSERT_ALWAYS((aValidityPeriod.Int() >= 0) && (aValidityPeriod.Int() <= (24 * 60)), gPanic(EInvalidValidityPeriod));

	iValidityPeriod = aValidityPeriod;
	}



/**
Gets the next time, after a specified time, when this off-peak time becomes
active. 

If aFromTime is within the validity period of this off-peak time, then the 
next time will be less than or equal to aFromTime. 

@param aFromTime
Specified time.

@return Next time
*/

EXPORT_C const TTime TMsvOffPeakTime::NextTimeInclusive(const TTime& aFromTime) const
	{
	TTime nextTime = aFromTime;
	TDateTime from = nextTime.DateTime();

	from.SetHour(iHour);
	from.SetMinute(iMinute);
	from.SetSecond(0);
	from.SetMicroSecond(0);

	nextTime = TTime(from);

	while (nextTime.DayNoInWeek() != iDay || nextTime > aFromTime)
		{
		nextTime -= (TTimeIntervalDays) 1;
		}

	TTimeIntervalMinutes mins;
	aFromTime.MinutesFrom(nextTime, mins);

	__ASSERT_DEBUG(mins.Int() >= 0, gPanic(EProgrammingBug));

	//mins must be greater than or equal to zero (0) because of the above while loop
	if (mins.Int() < iValidityPeriod.Int()) //TO DO: Perhaps <= instead
		{
		return nextTime;
		}

	while (nextTime < aFromTime)
		{
		nextTime += (TTimeIntervalDays) 7;
		}

	return nextTime;
	}


//
//
//	Class CMsvOffPeakTimes
//
//

/**
Default constructor.
*/

EXPORT_C CMsvOffPeakTimes::CMsvOffPeakTimes()
	: CArrayFixFlat<TMsvOffPeakTime>(1)
	{
	}


/**
Gets the off-peak time period that is after and closest to a specified time. 

If aFromTime is within an off-peak time period, then that off-peak time period 
is returned.

@param aFromTime
Time to find.

@param aNext
On return, the off-peak time period that is closest to aFromTime.

@param aNextTime
On return, the next start of aNext from aFromTime. 

@see TMsvOffPeakTime::NextTimeInclusive() for details.

@return KErrNotFound The array does not contain any off-peak time periods.
*/

EXPORT_C TInt CMsvOffPeakTimes::GetNextOffPeakTime(const TTime& aFromTime, TMsvOffPeakTime& aNext, TTime& aNextTime) const
	{
	const TInt count = Count();

	if (count < 1)
		{
		return KErrNotFound;
		}

	aNext = At(0);
	aNextTime = aNext.NextTimeInclusive(aFromTime);

	for (TInt curTime = 1; curTime < count; curTime++)
		{
		TMsvOffPeakTime time = At(curTime);
		TTime tempTime = time.NextTimeInclusive(aFromTime);

		if (tempTime < aNextTime)
			{
			aNextTime = tempTime;
			aNext = time;
			}
		}

	return KErrNone;
	}

