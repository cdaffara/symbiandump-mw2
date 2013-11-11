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

#ifndef __MSVSCHEDULESETTINGS_H__
#define __MSVSCHEDULESETTINGS_H__

 


#include <msvrcpt.h>
#include <schinfo.h>
#include <msvsenderroraction.h>





/**
Stores the MTM-specific (general) settings for scheduling messages on the
Task Scheduler. 

These settings include the latency (minimum amount of time between now and when
the message is scheduled for), the intervals (short, long or variable) to wait 
for before attempting to re-send the message and the timout value for messages
pending conditions to be met.

The other MTM scheduling releated settings are stored in CMsvOffPeakTimes 
and CMsvSendErrorActions. 

@publishedAll
@released
*/

class CMsvScheduleSettings : public CBase
	{
public:

	IMPORT_C static CMsvScheduleSettings* NewL();
	IMPORT_C static CMsvScheduleSettings* NewLC();	
	IMPORT_C ~CMsvScheduleSettings();


	IMPORT_C void Reset();


	IMPORT_C void SetPriority(const TInt aPriority);
	IMPORT_C TInt Priority() const;

	IMPORT_C void SetValidityPeriod(const TTimeIntervalMinutes& aValidityPeriod);
	IMPORT_C const TTimeIntervalMinutes& ValidityPeriod() const;

	IMPORT_C void SetIntervalType(const TIntervalType aIntervalType);
	IMPORT_C TIntervalType IntervalType() const;

	IMPORT_C void SetLongInterval(const TTimeIntervalSeconds& aInterval);
	IMPORT_C const TTimeIntervalSeconds& LongInterval() const;
	
	IMPORT_C void SetShortInterval(const TTimeIntervalSeconds& aInterval);
	IMPORT_C const TTimeIntervalSeconds& ShortInterval() const;

	IMPORT_C const CArrayFixFlat<TTimeIntervalSeconds>& VariableIntervals() const;
	IMPORT_C void SetVariableIntervalsL(const CArrayFixFlat<TTimeIntervalSeconds>& aIntervals);

	IMPORT_C const TTimeIntervalMicroSeconds32& Latency() const;
	IMPORT_C void SetLatency(const TTimeIntervalMicroSeconds32& aLatency);
	
	IMPORT_C void SetPendingConditionsTimeout(const TTimeIntervalMinutes& aTimeout);
	IMPORT_C const TTimeIntervalMinutes& PendingConditionsTimeout() const;

	/** Default scheduler settings values. */
	enum
		{
		/** Default priority. */
		EDefaultPriority		= 27,
		/** Default validity period: 10080 Minutes in 1 week. */
		EDefaultValidityPeriod	= 10080,
		/** Default interval type: EYearly */	
		EDefaultIntervalType	= 3,
		/** Default short retry interval. */		
		EDefaultShortInterval	= 5,
		/** Default long retry interval. */
		EDefaultLongInterval	= 120,
		/** Default latency. */
		EDefaultLatency			= 1500000,
		/** Default timeout. Zero minutes - no timeout. */
		EDefaultPendingConditionsTimeout	= 0
		};

private:
	CMsvScheduleSettings();

	void ConstructL();


	TInt					iPriority;		//Priority of the schedules on the task scheduler
	TTimeIntervalMinutes	iValidityPeriod;//Validity period of message on the task scheduler
	TIntervalType			iIntervalType;	//See TIntervalType declaration and Task Scheduler user documentation
	TTimeIntervalMicroSeconds32 iLatency;		//Minimum amount of time between now and when the message is scheduled for. Cannot be negative.
	TTimeIntervalMinutes	iPendingConditionsTimeout;

	TTimeIntervalSeconds iLongInterval;
	TTimeIntervalSeconds iShortInterval;
	CArrayFixFlat<TTimeIntervalSeconds>* iVariableIntervals;
	};

#endif	// __MSVSCHEDULESETTINGS_H__
