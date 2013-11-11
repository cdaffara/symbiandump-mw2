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

#include "MSVTIME.H"
#include "MSVPANIC.H"

const TMsvDays KMsvNullDays = 0xffff;

TMsvTime::TMsvTime()
: iDays(KMsvNullDays)
	{
	}

void TMsvTime::SetTime(const TTime& aTime)
	{
	if (aTime == Time::NullTTime())
		iDays = KMsvNullDays;
	else
		{
		__ASSERT_ALWAYS(aTime >= MinTime() && aTime <= MaxTime(), PanicServer(EMsvDateOutsideValidRange));
		iDays = (TMsvDays)aTime.DaysFrom(MinTime()).Int();
		iMinutes = (TMsvMinutes)(aTime.DateTime().Hour() * 60 + aTime.DateTime().Minute());
		}
	}

void TMsvTime::GetTime(TTime& aTime) const
	{
	if (IsNull())
		aTime = Time::NullTTime();
	else
		{
		aTime = MinTime();
		aTime += TTimeIntervalDays(iDays);
		aTime += TTimeIntervalMinutes(iMinutes);
		}
	}

TBool TMsvTime::operator>=(const TTime& aTime) const
	{
	TTime thisTime;
	GetTime(thisTime);

	TDateTime dt = aTime.DateTime();
	dt.SetSecond(0);
	dt.SetMicroSecond(0);

	return thisTime >= TTime(dt);
	}

TBool TMsvTime::IsNull() const
	{
	return iDays == KMsvNullDays;
	}

void TMsvTime::InternalizeL(RReadStream& aStream)
	{
	aStream >> iDays;
	aStream >> iMinutes;
	}

void TMsvTime::ExternalizeL(RWriteStream& aStream) const
	{
	aStream << iDays;
	aStream << iMinutes;
	}

TTime TMsvTime::MinTime() const
	{
	return TTime(TDateTime(1900,EJanuary,0,0,0,0,0));
	}

TTime TMsvTime::MaxTime() const
	{
	return TTime(TDateTime(2100,EDecember,30,23,59,59,0));
	}
