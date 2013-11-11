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

#include <msvscheduledentry.h>

#include <e32base.h>
#include <msvstd.h>
#include <msventry.h>

#include <msvschedulesend.h>

/**
Constructor.

@param aEntry 
Scheduled message.
*/

EXPORT_C CMsvScheduledEntry::CMsvScheduledEntry(const TMsvEntry& aEntry)
	: iEntry(aEntry)
	{
	}


/**
Gets the message entry.

@param aEntry
On return, the message entry.
*/

EXPORT_C void CMsvScheduledEntry::Entry(TMsvEntry& aEntry) const
	{
	aEntry.iDate = iEntry.iDate;
	aEntry.SetScheduled(iEntry.Scheduled());
	aEntry.SetSendingState(iEntry.SendingState());
	aEntry.SetFailed(iEntry.Failed());
	}


/**
Stores the object.

@param aStore
Store to write to.

@leave TMsvEntryScheduleData::StoreL()
@leave RecipientsStoreL() 
*/

EXPORT_C void CMsvScheduledEntry::StoreL(CMsvStore& aStore) const
	{
	if (iData.IsReset())
		{
		iData.RemoveL(aStore);
		}
	else
		{
		iData.StoreL(aStore);
		}

	RecipientsStoreL(aStore);
	}


/**
Restores the object.

@param aStore
Store to read from.

@leave TMsvEntryScheduleData::RestoreL()
@leave RecipientsRestoreL() 
*/

EXPORT_C void CMsvScheduledEntry::RestoreL(CMsvStore& aStore)
	{
	iData.RestoreL(aStore);
	RecipientsRestoreL(aStore);
	}


/**
Gets the entry's pending conditions flag.

@return 
The entry's pending conditions flag.
*/
EXPORT_C TBool CMsvScheduledEntry::PendingConditions() const
	{
	return iEntry.PendingConditions();	
	}
	
/**
sets the entry's pending conditions flag.

@param	aPendingConitions 
The entry's pending conditions flag.
*/
EXPORT_C void CMsvScheduledEntry::SetPendingConditions(TBool aPendingConditions)
	{
	iEntry.SetPendingConditions(aPendingConditions);
	}

