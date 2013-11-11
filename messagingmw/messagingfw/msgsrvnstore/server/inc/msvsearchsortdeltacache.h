// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MSVSEARCHSORTDELTACACHE_H_
#define MSVSEARCHSORTDELTACACHE_H_


#include <e32base.h>
#include <msvstd.h>
#include <sqldb.h>
#include <msvapi.h>
#include "msvsearchsortcachemanager.h"

enum TMsgType
	{
	ENewMsg=1,
	EUpdatedMsg=2,
	EDeletedMsg=3
	};

/**********************************
 * CMsvSearchSortDeltaCache
 * Description : Create a  Search or Sort Cache Entry.
 * @internalComponent
 * @released
 ***********************************/ 

NONSHARABLE_CLASS (CMsvSearchSortDeltaCache) : public CBase
	{
public:
  	~CMsvSearchSortDeltaCache(); //Destructor.
	static CMsvSearchSortDeltaCache* Instance();	
	void EntryInDeltaCache(TMsvId aID,TMsgType aType);
private:
	static CMsvSearchSortDeltaCache* CreateDeltaCacheL();
    void ConstructL();
    CMsvSearchSortDeltaCache();// Default constructor.
    CMsvSearchSortDeltaCache& operator=(const CMsvSearchSortDeltaCache&); //Prevent Assignment operatior.       
private:
	static CMsvSearchSortDeltaCache* iMsvSearchSortDeltaCache;
	RArray<TMsvId> iNewEntry;
	RArray<TMsvId> iUpdateEntry;
	RArray<TMsvId> iDeleteEntry;
	TMsgType iType;
	TBool iDeltaCacheDirtyFlag;
	TBool iIsHeaderSearchEnabled; //The flag is meant to check on the updates that occur on an entry, during search sort operation on header. 

	friend class CMsvServer;
	friend class CMsvIndexAdapter;
	friend class CMSvSearchSortCacheManager;
	friend class CMsvServerSession;

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestSearchSortDeltaCache;
	friend class CTestSearchSortCacheManager;
#endif
 	};

#endif /*MSVSEARCHSORTDELTACACHE_H_*/
