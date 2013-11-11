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



/**
 
 
 Gets the entry's ID.
 
 @return The entry's ID.
*/

inline const TMsvId CMsvScheduledEntry::Id() const
	{
	return iEntry.Id();
	}


/**
Gets the entry's OffPeak() flag.

@return The entry's OffPeak() flag.
*/

inline const TBool CMsvScheduledEntry::OffPeak() const
	{
	return iEntry.OffPeak();
	}


/**
Gets the entry's iMtm data member.

@return The entry's iMtm data member.
*/

inline const TUid CMsvScheduledEntry::Mtm() const
	{
	return iEntry.iMtm;
	}


/**
Gets the entry's iDate member.

@return Entry's iDate member.
*/

inline const TTime& CMsvScheduledEntry::ScheduleDate() const
	{
	return iEntry.iDate;
	}


/**
Sets the entry's iDate member.

@param aDate Entry's iDate member.
*/

inline void CMsvScheduledEntry::SetScheduleDate(const TTime& aDate)
	{
	iEntry.iDate = aDate;
	}


/**
Gets the entry's Scheduled() flag.

@return The entry's Scheduled() flag.
*/

inline const TBool CMsvScheduledEntry::Scheduled() const
	{
	return iEntry.Scheduled();
	}


/**
Sets the entry's Scheduled() flag.

@param aScheduled The entry's Scheduled() flag.
*/

inline void CMsvScheduledEntry::SetScheduled(const TBool aScheduled)
	{
	iEntry.SetScheduled(aScheduled);
	}


/**
Gets the entry's iError data member.

@return The entry's iError data member.
*/

inline const TInt CMsvScheduledEntry::Error() const
	{
	return iEntry.iError;
	}


/**
Gets the entry's SendingState() flag.

@return The entry's SendingState() flag.
*/

inline const TInt CMsvScheduledEntry::SendingState() const
	{
	return iEntry.SendingState();
	}


/**
Sets the entry's SendingState() flag.

@param aState
The entry's SendingState() flag.
*/

inline void CMsvScheduledEntry::SetSendingState(TInt aState)
	{
	iEntry.SetSendingState(aState);
	}


/**
Sets the entry's Failed() flag.

@param aFail
The entry's Failed() flag.
*/

inline void CMsvScheduledEntry::SetFailed(const TBool aFail)
	{
	iEntry.SetFailed(aFail);
	}


/**
Gets the entry's Failed() flag.

@return The entry's Failed() flag.
*/

inline const TBool CMsvScheduledEntry::Failed() const
	{
	return iEntry.Failed();
	}
