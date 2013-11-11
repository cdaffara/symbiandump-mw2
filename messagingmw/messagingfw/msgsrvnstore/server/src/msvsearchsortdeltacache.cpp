// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msvsearchsortdeltacache.h"
#include "MSVPANIC.H"


CMsvSearchSortDeltaCache* CMsvSearchSortDeltaCache::iMsvSearchSortDeltaCache =NULL;

/**
* CMsvSearchSortDeltaCache()
* 
*/

CMsvSearchSortDeltaCache::CMsvSearchSortDeltaCache()
	{
	iNewEntry.Reset();
	iUpdateEntry.Reset();
	iDeleteEntry.Reset();
	}

/**
* ~CMsvSearchSortDeltaCache()
*
*/

CMsvSearchSortDeltaCache::~CMsvSearchSortDeltaCache() 
	{
	iDeleteEntry.Close();
	iNewEntry.Close();
	iUpdateEntry.Close();
	iMsvSearchSortDeltaCache = NULL;
	}


 /**
 * Instance()
 *
 * The function returns already created instance of
 * this class to the caller. To create a new instance
 * the caller should call CreateL(). If an instance of
 * this object does not already exists, the function 
 * throws a panic EMsvFreePoolNotCreated in DEBUG mode.
 */ 
CMsvSearchSortDeltaCache* CMsvSearchSortDeltaCache::Instance()
	{
	__ASSERT_DEBUG(iMsvSearchSortDeltaCache!=NULL , PanicServer(EMsvSearchSortDeltaCacheNotCreated));
	return iMsvSearchSortDeltaCache;
	}


/**
 * CreateDeltaCacheL()
 *
 * The only way to create an object of this class.
 * The function ensures that only one instance of
 * the object is created. This is a static interface.
 * The function is made private to ensure that only
 * friend function can create instance of this class.
 */ 
CMsvSearchSortDeltaCache* CMsvSearchSortDeltaCache::CreateDeltaCacheL()
	{
	CMsvSearchSortDeltaCache* self = new (ELeave) CMsvSearchSortDeltaCache();
	//Push object in to cleanup stack so that it 
	// will be handles properly incase of a leave.
	CleanupStack::PushL(self);
	self->ConstructL();
	//Pop from cleanupstack
	CleanupStack::Pop(self);
	iMsvSearchSortDeltaCache = self;
	return self;
	}


/**
 * ConstructL()
 *
 * The function is called from InstanceL() and used to
 * initialize the instance of this class. It firsts reads
 * cache configuration information from msgcache.ini file
 * and stores them into member variable. It then allocates
 * creates initial set of entries as described in conf file. 
 */
void CMsvSearchSortDeltaCache::ConstructL()
	{
	}

/**
* EntryInDeltaCache()
* Add three different type (New, Update, Delete) of entry  in Delta Cache.
* @param :aID :a TMsvId ;aType = a TMsgType
* @return :TInt
*/
	
void CMsvSearchSortDeltaCache::EntryInDeltaCache(TMsvId aID,TMsgType aType)
	{
	if(aType == ENewMsg ) //New Msg Entry
		{
		iMsvSearchSortDeltaCache->iNewEntry.AppendL(aID);
		iMsvSearchSortDeltaCache->iDeltaCacheDirtyFlag = ETrue;
		}
	// This conditional statement is required to check if any msg updation is going  on while doing search or not.
	// And if any TMsvId  is  alreday added it should not add it again.
	else if(aType == EUpdatedMsg && !iIsHeaderSearchEnabled) //Updated msg Entry
			{
			//check if we already have aID in iNewEntry array list.
			TInt foundPos = iMsvSearchSortDeltaCache->iNewEntry.Find(aID);
						
			if( foundPos < 0 &&
				iUpdateEntry.Find(aID) == KErrNotFound )
				{
				iMsvSearchSortDeltaCache->iUpdateEntry.AppendL(aID);
				iMsvSearchSortDeltaCache->iDeltaCacheDirtyFlag = ETrue;
				}
							
			}
	else if(aType == EDeletedMsg) //Deleted msg Entry.
		{
		iMsvSearchSortDeltaCache->iDeleteEntry.AppendL(aID);
		iMsvSearchSortDeltaCache->iDeltaCacheDirtyFlag = ETrue;
		}
	}

