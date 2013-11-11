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

#ifndef MSVSEARCHSORTCACHEMANAGER_H_
#define MSVSEARCHSORTCACHEMANAGER_H_


#include <e32base.h>
#include <msvstd.h>
#include <msvsearchsortquery.h>
#include "msvsearchsortcacheentry.h"


//Forward Declaration
class CMsvSearchSortDeltaCache;
class CMsvSearchSortCacheEntry;
class CMsvEntryFreePool;
class CMsvServer;
class TSearchSortDbWrapper;

/****************************************************
 * CMSvSearchSortCacheManager
 * Description : Create a  Search and Sort Cache Manager .
 * This is a Singleton class
 * @internalComponent
 * @released
 ****************************************************/ 

NONSHARABLE_CLASS (CMSvSearchSortCacheManager) : public  CBase
    {
public:
    ~CMSvSearchSortCacheManager(); //Destructor.
	static CMSvSearchSortCacheManager* Instance(); // To access the manager instance.	

	TInt AddEntryL(const CMsvSearchSortCacheEntry& aEntry); //Add a CMsvSearchSortCacheEntry to manager,
	void MoveEntryAtEnd(CMsvSearchSortCacheEntry* aEntry, TInt aIndex); //Move the Cache entry to the end of the manager RPointer array.
	TInt QueryExists(const CMsvSearchSortCacheEntry& aEntry);
	TInt QueryExists(const TUint32 aQueryID);
	TBool IsInRange(const CMsvSearchSortCacheEntry& aEntry); // Will check the Size of the Cache.
	TBool RemoveEntry(const TInt aRequiredSize); // Will Remove the entry based on the Curent Situation.
	TBool RemoveEndEntry(const TInt aIndex); 
	void InstantiateDBwrapperL();
	
	void TypeOfSearchQuery(CMsvSearchSortCacheEntry& aEntry);
	void TypeOfSortQuery(CMsvSearchSortCacheEntry& aEntry);

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TInt IsTypeOfQuery(const CMsvSearchSortCacheEntry& aEntry);
#else
	TTypeOfQuery IsTypeOfQuery(const CMsvSearchSortCacheEntry& aEntry);	
#endif
	
	TInt DoProcessQueryL(const CMsvSearchSortCacheEntry& aEntry,TInt iteratorCount=0);
	TInt DoProcessQueryL(const TInt aIndex);
	
	TInt FinalResultAsIdL(const CMsvSearchSortCacheEntry& aEntry);
	TInt FindResultAsIdL(const CMsvSearchSortCacheEntry& aEntry);
	TInt ResultCountL(const CMsvSearchSortCacheEntry& aEntry);
	
	TInt ResultCountL(const TInt aIndex);
	void StoreResultL(const TInt aQid, RArray<TMsvId>& aToUpdateDb);
	
	TInt StoreSortResultL(const TInt aIndex,RArray<TMsvIdWithSortField>& aResultIdData);
	TInt ReturnIdCountL(const TInt aQID);
	TInt UpdateQueryResultOnDemandInDBL(const CMsvSearchSortCacheEntry& aEntry,RArray<TMsvId>& aNewEntry,RArray<TMsvId>& aUpdateEntry, RArray<TMsvId>& aDeleteEntry);
	TInt OnDemandUpdateCacheEntryL(const TInt aIndex);
	TInt QueryID();
	
	void FirstResultForInteraratorNewQueryL(const CMsvSearchSortCacheEntry& aEntry);
	void FirstResultForInteraratorQueryIdL(const TInt index);
	void NextResultForInteraratorL(const TInt aIndex);
	
	TInt UnMarkedQuery(const TInt aindex);
	TInt GenerateQueryId(CMsvSearchSortCacheEntry& aEntry) ;
	TInt ReturnProgressInfo() const;
	TInt CancelSearchSortOperation() const;
	void RetrunQuery(TInt aIndex, CMsvSearchSortQuery* aCLientQuery);
	void ResetSearchSortCache();
	TSearchSortDbWrapper* GetDbWrapper();
	//For Simultaneously query
	void AddOutstandingSOSOperation();
	TInt OutstandingSOSOperations();
	
	
private:
    static CMSvSearchSortCacheManager* CreateManagerL(CMsvServer& aServer); //Private CreateL
    void ConstructL(); //Private Constructor.
    CMSvSearchSortCacheManager(CMsvServer& aServer);// Default constructor.
    CMSvSearchSortCacheManager(const CMSvSearchSortCacheManager&) ; //restricted copy contructor.
	CMSvSearchSortCacheManager& operator=(const CMSvSearchSortCacheManager&); //Prevent Assignment operator.       
public:
	RArray<TMsvIdWithSortField> iMsvIdWithSortFieldArray; //Structure array sent by Client .
	RArray<TMsvId> iToFindResultAsIdArray; //ID's to be searched in Client side  for Header and body.
	RArray<TMsvId> iFinalResultAsIdArray; //Final result To send to Client.
	RArray<TMsvId> iUpdateIdsToCacheArray; //
	RArray<TMsvId> iDeltaCacheIdArray; //Delta Cache Array.. which contain the new, updated, deleted message id.
	TInt iFindResultCount;
	TInt iFinalResultCount;
	TBool iCancelFlag; //Syncronous cancel flag 
private:	
	static CMSvSearchSortCacheManager* iMsvSearchSortCacheManager;
	RPointerArray<CMsvSearchSortCacheEntry>* iManagerEntry;
	TInt iMsvMaximumSearchSortCacheSize; 
	TInt iMsvAvailableSearchSortCacheSize;
	TInt iIndexEntryCount;
	TInt iRequiredSize;
	TInt iTotalUsedCacheSize;
	TInt iSearchSortDeltaCacheSize;
	TInt iReturnResultType;
	TBool iExplicitSortOnDateTime;
	TInt iEntryIdCount;
	TInt iResultCount;
	TMsvId iIteratorId;
	TMsvEntry iIteratorEntry;
	TInt iMaxQueryId;
	TInt iIteratorRemainingResultCount;
	TInt iProgress;
	CMsvServer &iServer;
	CMsvSearchSortDeltaCache* iSearchSortDeltaCache;
	TSearchSortDbWrapper* iSearchDbAdapter;
	//For Simultaneously query
	mutable TInt iOutstandingSOSOperations;
	
    friend class CMsvIndexAdapter;
	friend class CMsvServer;
	friend class CMsvSearchSortCacheEntry;
	friend class CMsvServerSession;

#if(defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestSearchSortCacheManager;
	friend class CTestSearchSortCacheEntry;
	friend class CTestSearchSortDBadapter;
	friend class CTestOOMSearchSortCacheManager;
#endif
    };
	
#endif /*MSVSEARCHSORTCACHEMANAGER_H_*/
