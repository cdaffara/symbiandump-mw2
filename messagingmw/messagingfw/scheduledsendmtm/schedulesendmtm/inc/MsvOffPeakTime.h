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

#ifndef __MSVOFFPEAKTIME_H__
#define __MSVOFFPEAKTIME_H__

 


//
//
//	Constants
//
//


//
//
//	TMsvOffPeakTime Declaration
//
//

/**
Defines an off-peak period for message sending.

An off-peak period is specified as a start time and a duration. 

@publishedAll
@released
*/

class TMsvOffPeakTime
	{
public:
	IMPORT_C TMsvOffPeakTime();
	IMPORT_C TMsvOffPeakTime(const TDay aDay, const TInt aHour, const TInt aMinute, const TTimeIntervalMinutes aValidityPeriod);


	IMPORT_C void Reset();

	IMPORT_C TDay Day() const;
	IMPORT_C void SetDay(const TDay aDay);

	IMPORT_C TInt Hour() const;
	IMPORT_C void SetHour(const TInt aHour);

	IMPORT_C TInt Minute() const;
	IMPORT_C void SetMinute(const TInt aMinute);

	IMPORT_C const TTimeIntervalMinutes ValidityPeriod() const;
	IMPORT_C void SetValidityPeriod(const TTimeIntervalMinutes aValidityPeriod);

	IMPORT_C const TTime NextTimeInclusive(const TTime& aFromTime) const;

private:
	TDay iDay;
	TInt8 iHour;							//Start hour
	TInt8 iMinute;							//Start minute
	TTimeIntervalMinutes iValidityPeriod;	//Validity period. Must be less than 24 hours
	};

//
//
//	CMsvOffPeakTimes Declaration
//
//

/**
Array of off-peak time data.

This represents all the off-peak time periods in each week. 

@publishedAll
@released
*/

class CMsvOffPeakTimes : public CArrayFixFlat<TMsvOffPeakTime>
	{
public:
	IMPORT_C CMsvOffPeakTimes();


	IMPORT_C TInt GetNextOffPeakTime(const TTime& aFromTime, TMsvOffPeakTime& aNext, TTime& aNextTime) const;

protected:


private:
	};

#endif	// __MSVOFFPEAKTIME_H__
