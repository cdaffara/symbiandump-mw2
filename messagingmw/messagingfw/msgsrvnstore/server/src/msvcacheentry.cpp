// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msvcacheentry.h"

/**
 * NewLC()
 * @param None.
 * @return CMsvCacheEntry* : a handle to a new cache entry.
 * 
 * 
 */
CMsvCacheEntry* CMsvCacheEntry::NewLC()
	{
	CMsvCacheEntry* self = new(ELeave) CMsvCacheEntry();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}



/**
 * NewL()
 * @param None.
 * @return CMsvCacheEntry* : a handle to a new cache entry.
 * 
 * 
 */
CMsvCacheEntry* CMsvCacheEntry::NewL()
	{
	CMsvCacheEntry* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}



/**
 * ConstructL()
 * @param None.
 * @return None.
 * 
 * Second phase construction.
 */
void CMsvCacheEntry::ConstructL()
	{

	}


/**
 * CMsvCacheEntry()
 * @param None.
 * @return None.
 * 
 * Constructor.
 */
CMsvCacheEntry::CMsvCacheEntry()
	{
	}


/**
 * ~CMsvCacheEntry()
 * @param None.
 * @return None.
 * 
 * Destructor.
 */
CMsvCacheEntry::~CMsvCacheEntry()
	{
	delete iDescriptionBuffer;
	delete iDetailsBuffer;

	if(iChildIdArray)
		{
		iChildIdArray->Close();
		delete iChildIdArray;
		iChildIdArray = NULL;
		}
	}


/**
 * ResetEntry()
 * @param None.
 * @return None.
 * 
 * The function resets a cache entry to zero/default values.
 */
void CMsvCacheEntry::ResetEntry()
	{
	delete iDescriptionBuffer;
	iDescriptionBuffer = NULL;
	
	delete iDetailsBuffer;
	iDetailsBuffer = NULL;
	
	iEntry = TMsvEntry();
	iFlags = EMsvServerEntryClearFlag;

	iStoreReaderCount = 0;
	
	if(iChildIdArray)
		{
		iChildIdArray->Close();
		delete iChildIdArray;
		iChildIdArray = NULL;
		}
	}



/**
 * SetEntryL()
 * @param TMsvEntry& : the TMsvEntry with the details to be copied to the cache entry.
 * @return None.
 * 
 * The function sets the TMsvEntry, iDescription and iDetails fields of the cache entry
 * to that of the passed entry.
 */
void CMsvCacheEntry::SetEntryL(TMsvEntry& aEntry)
	{
	iEntry = aEntry;
	if(!iDescriptionBuffer)
		{
		iDescriptionBuffer = aEntry.iDescription.AllocL();
		}
	else if (aEntry.iDescription.Length() > iDescriptionBuffer->Des().MaxLength())
		{
		iDescriptionBuffer = iDescriptionBuffer->ReAllocL(aEntry.iDescription.Length());
		}
	
	
	if(!iDetailsBuffer)
		{
		iDetailsBuffer = aEntry.iDetails.AllocL();
		}
	else if (aEntry.iDetails.Length() > iDetailsBuffer->Des().MaxLength())
		{
		iDetailsBuffer = iDetailsBuffer->ReAllocL(aEntry.iDetails.Length());
		
		}

	*iDescriptionBuffer = aEntry.iDescription;
	*iDetailsBuffer = aEntry.iDetails;
			
	iEntry.iDescription.Set(*iDescriptionBuffer);
	iEntry.iDetails.Set(*iDetailsBuffer);
	}


/*
 * DupNDestroyL()
 * 
 */
void CMsvCacheEntry::DupNDestroyL(CMsvCacheEntry*& aEntry)
	{
	SetEntryL(aEntry->Entry());
	iFlags = aEntry->iFlags;
	iStoreReaderCount = aEntry->iStoreReaderCount;
	iLastChange = aEntry->iLastChange;
	iCreated = aEntry->iCreated;
	iOwnerId = aEntry->iOwnerId;
	iChildIdArray = aEntry->iChildIdArray;
	aEntry->iChildIdArray = NULL;
	}


 

/**
 * LockEntry()
 * @param None.
 * @return TInt : KErrNone on success, KErrLocked if the entry is already locked.
 * 
 * The function locks the cache entry if not already locked.
 */
TInt CMsvCacheEntry::LockEntry()
	{
	if (IsEntryLocked())
		{
		return KErrLocked;
		}

	iFlags |= EMsvServerEntryLockFlag;
	return KErrNone;
	}


/**
 * LockStore()
 * @param None.
 * @return TInt : KErrNone on success, KErrLocked if the store entry is already locked.
 * 
 * The function locks the store entry if not already locked.
 */
TInt CMsvCacheEntry::LockStore()
	{
	if (IsStoreLocked())
		{
		return KErrLocked;
		}

	iFlags |= EMsvServerEntryStoreLockFlag;
	return KErrNone;
	}



/**
 * LockEntryAndStore()
 * @param None.
 * @return TInt : KErrNone on success, KErrLocked if the store and cache entry is already locked.
 * 
 * The function locks the store and cache entry if not already locked.
 */
TInt CMsvCacheEntry::LockEntryAndStore()
	{
	if (IsEntryOrStoreLocked())
		{
		return KErrLocked;
		}

	iFlags |= (EMsvServerEntryStoreLockFlag | EMsvServerEntryLockFlag);
	return KErrNone;
	}



/**
 * CopyEntryL()
 * @param const TMsvEntry& : the entry to be changed to.
 * @param TBool& : output parameter set to ETrue if any private details (iDesciption
 * 				   and iDetails) have been changed.
 * @return None.
 * 
 * The function copies details from the passed TMsvEntry to the cache entry's TMsvEntry.
 * It reallocates buffer sizes if there is difference between them.
 */
void CMsvCacheEntry::CopyEntryL(const TMsvEntry& aEntry, TBool& aChangedPrivateInfo)
	{
	TBool changeDes = (aEntry.iDescription!=*iDescriptionBuffer);
	TBool changeDet = (aEntry.iDetails!=*iDetailsBuffer);

	aChangedPrivateInfo = changeDes || changeDet;

	//Only increase the buffer size, not shrink so as to allow rollback
	TBool isMemoryAllocationDone = EFalse;
	HBufC* bkpDescriptionBuffer = iDescriptionBuffer;
	if (changeDes && aEntry.iDescription.Length() > iDescriptionBuffer->Des().MaxLength())
		{		
		iDescriptionBuffer = HBufC::NewL(aEntry.iDescription.Length());
		isMemoryAllocationDone = ETrue;
		}

	if (changeDet && aEntry.iDetails.Length() > iDetailsBuffer->Des().MaxLength())
		{
		TRAPD(err, iDetailsBuffer = iDetailsBuffer->ReAllocL(aEntry.iDetails.Length()));
		if(err)
		    {
		    if(isMemoryAllocationDone)
		        {
		        delete iDescriptionBuffer;
		        iDescriptionBuffer = bkpDescriptionBuffer; 
		        }
		    User::Leave(err);
		    }
		}

	if(isMemoryAllocationDone)
	    {
	    delete bkpDescriptionBuffer;
	    }
	//Copy the data without leaving
	if (changeDes)
		{
		*iDescriptionBuffer = aEntry.iDescription;
		}

	if (changeDet)
		{
		*iDetailsBuffer = aEntry.iDetails;
		}

	iEntry=aEntry;
	iEntry.iDescription.Set(*iDescriptionBuffer);
	iEntry.iDetails.Set(*iDetailsBuffer);
	}



/**
 * RollBackCopyEntry()
 * @param const TMsvEntry& : the entry to be changed to.
 * @return None.
 * 
 * 
 */
void CMsvCacheEntry::RollBackCopyEntry(const TMsvEntry& aEntry)
	{
	__ASSERT_DEBUG(iDescriptionBuffer->Des().MaxLength() >= aEntry.iDescription.Length(), PanicServer(EMsvBadRollBackCopy1));
	__ASSERT_DEBUG(iDetailsBuffer->Des().MaxLength() >= aEntry.iDetails.Length(), PanicServer(EMsvBadRollBackCopy2));

	Mem::Copy(&iEntry, &aEntry, sizeof(TMsvEntry));

	*iDescriptionBuffer = aEntry.iDescription;
	*iDetailsBuffer = aEntry.iDetails;

	iEntry.iDescription.Set(*iDescriptionBuffer);
	iEntry.iDetails.Set(*iDetailsBuffer);
	}
