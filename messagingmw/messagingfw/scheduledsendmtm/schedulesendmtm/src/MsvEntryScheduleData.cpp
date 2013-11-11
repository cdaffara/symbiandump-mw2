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
#include <msventryscheduledata.h>

//
//
//	Type Class TMsvEntryScheduleData
//
//

/**
Default constructor.
*/

EXPORT_C TMsvEntryScheduleData::TMsvEntryScheduleData()
	{
	Reset();
	}


/**
Resets the object.

It sets all member data to zero.
*/

EXPORT_C void TMsvEntryScheduleData::Reset()
	{
	iVersion		= KMsvEntryScheduleDataVersion;
	iRetryCount		= 0;
	iTaskId			= KErrNotFound; //must not be 0 because Task Scheduler may create a task with id 0
	iRef.iHandle	= KErrNotFound; //must not be 0 because Task Scheduler may create a schedule with id 0
	}


/**
Tests if all the members are set to their default value.

@return True if all the members are set to their default value.
*/

EXPORT_C TBool TMsvEntryScheduleData::IsReset() const
	{
	return ((iRetryCount == 0) && (iTaskId == KErrNotFound));
	}


/**
Restores the object from a store.

@param aStore
Store to read from.
*/

EXPORT_C void TMsvEntryScheduleData::RestoreL(CMsvStore& aStore)
	{
	Reset();
	
	if (aStore.IsPresentL(KUidMsvFileScheduleData))
		{
		RMsvReadStream readStream;
		readStream.OpenLC(aStore, KUidMsvFileScheduleData);

		//Internalize
		InternalizeL(readStream);

		CleanupStack::PopAndDestroy(); //readStream
		}
	}


/**
Removes the object from a store.

@param aStore 
Store to remove from.

@leave CMsvStore::IsPresentL()
@leave CMsvStore::RemoveL()
*/

EXPORT_C void TMsvEntryScheduleData::RemoveL(CMsvStore& aStore) const
	{
	if (aStore.IsPresentL(KUidMsvFileScheduleData))
		{
		aStore.RemoveL(KUidMsvFileScheduleData);
		}
	}


/**
Stores the object in a store.

This method leaves if the the object's data cannot be written to a 
RMsvWriteStream stream.

@param aStore
Store to write to.
*/

EXPORT_C void TMsvEntryScheduleData::StoreL(CMsvStore& aStore) const
	{
	RMsvWriteStream writeStream;
	writeStream.AssignLC(aStore, KUidMsvFileScheduleData);

	//Externalize
	ExternalizeL(writeStream);

	writeStream.CommitL();
	CleanupStack::PopAndDestroy(); //writeStream
	}


/**
Externalises the object to the specified stream.

@param aWriteStream
Stream to write to.
*/

void TMsvEntryScheduleData::ExternalizeL(RWriteStream& aWriteStream) const
	{
	aWriteStream.WriteInt16L(iVersion);
	aWriteStream.WriteInt32L(iRetryCount);
	aWriteStream.WriteInt32L(iTaskId);
	aWriteStream.WriteInt32L(iRef.iHandle);
	aWriteStream << iRef.iName;
	}


/**
Internalises the object from the specified stream.

@param aReadStream
Stream to read from.
*/

void TMsvEntryScheduleData::InternalizeL(RReadStream& aReadStream)
	{
	iVersion		= aReadStream.ReadInt16L();

	if (iVersion >= 1)
		{
		iRetryCount		= aReadStream.ReadInt32L();
		iTaskId			= aReadStream.ReadInt32L();
		iRef.iHandle	= aReadStream.ReadInt32L();
		aReadStream >> iRef.iName;
		}
	}


/**
Gets how many retries have been made to send this message.

@return Number of retries.
*/

EXPORT_C TInt TMsvEntryScheduleData::Retries() const
	{
	return iRetryCount;
	}


/**
Adds one to the value of the iRetryCount member.
*/

EXPORT_C void TMsvEntryScheduleData::IncreaseRetries()
	{
	iRetryCount++;
	}


/**
Sets the iRetryCount member to zero.
*/

EXPORT_C void TMsvEntryScheduleData::ResetRetries()
	{
	iRetryCount = 0;
	}

