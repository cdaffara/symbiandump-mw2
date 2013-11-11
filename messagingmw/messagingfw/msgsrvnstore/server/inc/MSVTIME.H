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

#if !defined(__MSVTIME_H__)
#define __MSVTIME_H__

#if !defined(__E32STD_H__)
#include <e32std.h>
#endif

#if !defined(__S32STRM_H__)
#include <s32strm.h>
#endif

typedef TUint16 TMsvDays;
typedef TUint16 TMsvMinutes;

class TMsvTime
/**
@internalComponent
@released
*/
	{
public:
	TMsvTime();
	//
	void SetTime(const TTime& aTime);
	void GetTime(TTime& aTime) const;
	//
	TBool operator>=(const TTime& aTime) const;
	TBool IsNull() const;
	//
	void InternalizeL(RReadStream& aStream);
	void ExternalizeL(RWriteStream& aStream) const;
	//
private:
	TTime MinTime() const;
	TTime MaxTime() const;
	//
private:
	TMsvDays iDays;
	TMsvMinutes iMinutes;
	};

#endif
