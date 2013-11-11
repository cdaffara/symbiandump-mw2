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
// MSVSEARCHSORTCACHEMANGER.CPP
// 
//

#include "msvinifile.h"
#include "msvsearchsortcacheentry.h"
#include "msvsearchsortcachemanager.h"
#include "msventryfreepool.h"
#include "MSVSERV.H"
#include "msvsearchsortdbwrapper.h"
#include "msvsearchsortdeltacache.h"
#include "msvdbadapter.h"
#include "msvindexadapter.h"

const TInt KPercentageValue = 20 ; //in percentage
const TInt KDefaultMaxCacheSize = 1024 ; //In Bytes.
const TInt KSearchSortDeltaCacheLimit = 350 ; //Number of maximum Delta Cache Entry .

_LIT(KAttachmentYes, "0");
_LIT(KAttachmentNo, "1");
_LIT(KAttachmentTypeEMsvFile, "2");
_LIT(KAttachmentTypeEMsvLinkedFile, "3"); 
_LIT(KAttachmentTypeEMsvMessageEntry, "4"); 


CMSvSearchSortCacheManager* CMSvSearchSortCacheManager::iMsvSearchSortCacheManager =NULL;


 /**
  * CMsvCacheIndexTableEntry()
  * @param CMsvServer& .
  * @return None.
  * Single Ton Class.
  * Constructor
  */
 CMSvSearchSortCacheManager::CMSvSearchSortCacheManager(CMsvServer& aServer):iServer(aServer)
 	{
   	}

 /**
  * ~CMsvCacheIndexTableEntry()
  * @param None.
  * @return None.
  * Destructor
  */
 CMSvSearchSortCacheManager::~CMSvSearchSortCacheManager()
 	{
	iOutstandingSOSOperations = 0;
	iMsvIdWithSortFieldArray.Close();
	iToFindResultAsIdArray.Close();
	iFinalResultAsIdArray.Close();
	iUpdateIdsToCacheArray.Close();
	iDeltaCacheIdArray.Close();

	delete iSearchSortDeltaCache;

	if(iSearchDbAdapter)
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)   	
   		iSearchDbAdapter->CloseResources();
#endif
   		delete iSearchDbAdapter;
   		iSearchDbAdapter = NULL;
		}
   	
   	if(iManagerEntry)
	   	{
	   	if(iManagerEntry->Count()>0)
	   		{
	   		iManagerEntry->ResetAndDestroy();
	   		}
	   	iManagerEntry->Close();	
	   	}
  	delete iManagerEntry;
  	iMsvSearchSortCacheManager = NULL; 
  	}

 /**
 * Instance()
 *
 * The function returns already created instance of
 * this class to the caller. To create a new instance
 * the caller should call CreateL(). If an instance of
 * this object does not already exists, the function
 * throws a panic EMsvSearchSortCacheMangerNotCreated in DEBUG mode.
 *
 * @param CMsvServer& .
 * @return CMSvSearchSortCacheManager*
 *
 */
CMSvSearchSortCacheManager* CMSvSearchSortCacheManager::Instance()
	{
	__ASSERT_DEBUG(iMsvSearchSortCacheManager!=NULL , PanicServer(EMsvSearchSortCacheMangerNotCreated));
	return iMsvSearchSortCacheManager;
	}


/**
 * CreateManagerL()
 * 
 * The only way to create an object of this class.
 * This is a static interface.
 * The function is made private to ensure that only
 * friend function can create instance of this class.
 *
 * @param CMsvServer& .
 * @return CMSvSearchSortCacheManager*
 */
CMSvSearchSortCacheManager* CMSvSearchSortCacheManager::CreateManagerL(CMsvServer& aServer)
	{
	CMSvSearchSortCacheManager* self ;
	self = new (ELeave) CMSvSearchSortCacheManager(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	iMsvSearchSortCacheManager = self;
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
void CMSvSearchSortCacheManager::ConstructL()
	{
	iProgress = KMsvSearchSortOpNone;
	iCancelFlag = EFalse;
	iExplicitSortOnDateTime = EFalse;

	//For Simultaneously query
	iOutstandingSOSOperations = 0;
	
	if(CMsvEntryFreePool::Instance()->iMsvMaximumCacheSize != NULL && CMsvEntryFreePool::Instance()->iMsvSearchSortCache)
		{
		iMsvMaximumSearchSortCacheSize = ((CMsvEntryFreePool::Instance()->iMsvMaximumCacheSize * CMsvEntryFreePool::Instance()->iMsvSearchSortCache)/100)*1024 ;		
		}
	else
		{
		iMsvMaximumSearchSortCacheSize = ((KDefaultMaxCacheSize * KPercentageValue)/100)*1024 ;
		}
	//Default Granularity
	iManagerEntry = new(ELeave) RPointerArray<CMsvSearchSortCacheEntry>();
	iSearchSortDeltaCache = CMsvSearchSortDeltaCache::CreateDeltaCacheL();
	iSearchDbAdapter = NULL;
	}


void CMSvSearchSortCacheManager::InstantiateDBwrapperL()
	{
	//Leave with the errorstate if the Db closed
	if(iServer.Context().IndexAdapter()->GetDbAdapter() == NULL)
		{
		if(iSearchDbAdapter)
			{
	#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)   	
   			iSearchDbAdapter->CloseResources();
	#endif
			delete iSearchDbAdapter;
			iSearchDbAdapter = NULL;
			}
		User::Leave(iServer.Context().IndexAdapter()->ErrorState());
		}
	//If the searchsortwrapper is not instantiated, instantiate it
	else if(iSearchDbAdapter == NULL)
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		iSearchDbAdapter = TSearchSortDbWrapper::NewL(iServer);
#else		
		iSearchDbAdapter = TSearchSortDbWrapper::NewL(*(iServer.Context().IndexAdapter()->GetDbAdapter()));
#endif		
		}
	//If the Db pointer has been changed since the last operation, reinstantiate it
	//so get new iDbAdapter
	else
		{
		if(iSearchDbAdapter->GetDbAdapter() != iServer.Context().IndexAdapter()->GetDbAdapter())
			{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)   	
   			iSearchDbAdapter->CloseResources();
#endif			
			delete iSearchDbAdapter;
			iSearchDbAdapter = NULL;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		iSearchDbAdapter = TSearchSortDbWrapper::NewL(iServer);
#else		
		iSearchDbAdapter = TSearchSortDbWrapper::NewL(*(iServer.Context().IndexAdapter()->GetDbAdapter()));
#endif			
			}
		}
	}
/**
 * AddEntryL()
 * @param CMsvSearchSortCacheEntry*& :  apointer toa reference of CMsvSearchSortCacheEntry.
 * @return TInt
 * 				Return  -1 : Not Added into Manager.
 * 				Return 0 : Sussefuly added into manager.
 */
 TInt CMSvSearchSortCacheManager::AddEntryL(const CMsvSearchSortCacheEntry& aEntry)
	{
	if(QueryExists(aEntry)<0) //Query is not Exist
		{
	 	TInt iRequiredSize = sizeof(aEntry);
	 	if(iRequiredSize > iMsvMaximumSearchSortCacheSize)
	 		{
	 		return KErrNoMemory;
	 		}
	 		
	 	else
		 	{
		 	if(iMsvSearchSortCacheManager->iManagerEntry->Count() == 0)
		 		{
		 		if(IsInRange(aEntry))
		 			{
		 			User::LeaveIfError(iMsvSearchSortCacheManager->iManagerEntry->Append(&aEntry));
		 			}
		 		}
		 	else
		 		{
		 		if(IsInRange(aEntry))
		 			{
	 				User::LeaveIfError(iMsvSearchSortCacheManager->iManagerEntry->Append(&aEntry));
		 			}
		 		else
		 			{
		 			TInt iRequiredSize = sizeof(aEntry);
		 			if(RemoveEntry(iRequiredSize))
		 				{
		 				User::LeaveIfError(iMsvSearchSortCacheManager->iManagerEntry->Append(&aEntry));
	 					}
	 				}
	 			}
	   		}/* End of  if(iRequiredSize > iMsvMaximumSearchSortCacheSize)*/
		}/* END of if(QueryExists(&aEntry)<0) //Query is not Exist */
	else
		{
		//Query Already Exist
		delete &aEntry;
		}
	return 0;
	}


 /**
  * MoveEntryAtEnd()
  * @param CMsvSearchSortCacheEntry* :Pointer to a Cache entry need to add.
  * @param TInt : aIndex 
  * @return None
  */

void CMSvSearchSortCacheManager::MoveEntryAtEnd(CMsvSearchSortCacheEntry* aEntry,TInt aIndex)
	{
	iMsvSearchSortCacheManager->iManagerEntry->Remove(aIndex);
	iMsvSearchSortCacheManager->iManagerEntry->Append(aEntry);
	}
 
 
 /**
  * QueryExists()
  * @param CMsvSearchSortCacheEntry* :Pointer to a Cache entry need to add.
  * @return TInt : index of manager class  if Query Exist , else 0 if query does not exists.
  */
TInt CMSvSearchSortCacheManager::QueryExists(const CMsvSearchSortCacheEntry& aEntry)
	{
	TBool queryNotFound = EFalse;
	TInt return_position  = -1;
	if(iMsvSearchSortCacheManager->iManagerEntry->Count() > 0)
		{
		//Total Number of available Node in SearchSortCacheManager Class.
		for(TInt IndexEntryCount = 0; IndexEntryCount <iMsvSearchSortCacheManager->iManagerEntry->Count(); IndexEntryCount++)
			{
			//Checking ParentID and ResultType
			if((aEntry.iParentId == (*iManagerEntry)[IndexEntryCount]->iParentId) && (aEntry.iQueryLevel == (*iManagerEntry)[IndexEntryCount]->iQueryLevel))/*(aEntry.iResultType == (*iManagerEntry)[IndexEntryCount]->iResultType))*/
				{
				//Checking QueryType(Serach/sort) and Sort Option (Ascending/Desending)
				if(aEntry.IsSortQuery()  == (*iManagerEntry)[IndexEntryCount]->IsSortQuery() && (aEntry.IsAscendingSort() == (*iManagerEntry)[IndexEntryCount]->IsAscendingSort()))
					{
					//Checking WholeWord and WildCharacter
					 if(aEntry.IsWholeWord() == (*iManagerEntry)[IndexEntryCount]->IsWholeWord() && (aEntry.IsWildCharacter() == (*iManagerEntry)[IndexEntryCount]->IsWildCharacter()))
						{
						//Checking ExplicitSort 
						 if(aEntry.IsExplicitSortReq() == (*iManagerEntry)[IndexEntryCount]->IsExplicitSortReq() && (aEntry.iMsgExplicitSortPart == (*iManagerEntry)[IndexEntryCount]->iMsgExplicitSortPart))
							{
							//Checking Explicit Sort part
							if((*iManagerEntry)[IndexEntryCount]->iMsgExplicitSortPart == aEntry.iMsgExplicitSortPart /*EMsgDate*/)
								{
								// For query Level Search
								for (TInt qCount = 0; qCount < aEntry.iQueryLevel; qCount++)
									{
									//Checking mesagepart and relational Op
									if ((*iManagerEntry)[IndexEntryCount]->iMsgQuery[qCount].iMessagePart == aEntry.iMsgQuery[qCount].iMessagePart) 
										{
										//Checking QueryString
										 if(((*iManagerEntry)[IndexEntryCount]->iMsgQuery[qCount].iQueryString.Compare(aEntry.iMsgQuery[qCount].iQueryString) == 0) && ((*iManagerEntry)[IndexEntryCount]->iMsgQuery[qCount].iRelationOp == aEntry.iMsgQuery[qCount].iRelationOp ))
											{
											queryNotFound = EFalse;
											}
										else
											{
											queryNotFound = ETrue;
											}
												
										}
									else
										{
										queryNotFound = ETrue;
										}
									}/* End of  for (TInt qCount = 0; qCount < aEntry.iQueryLevel; qCount++)  */
									return_position = IndexEntryCount;
								}
														
							}
					
						}
					
					}
				
				}
					
			}/*	End of while (iIndexEntryCount--)*/
		
		//All Check match. Query Found. Returning the index.
		if(queryNotFound)
			{
			iProgress = KMsvSearchSortOpInProgress;
			return KErrNotFound;
			}
			
		else
			{
			iProgress = KMsvSearchSortOpInProgress;
			return return_position;
			}
			
		}/*End of if(iMsvSearchSortCacheManager->iManagerEntry->Count() > 0) */
	iProgress = KMsvSearchSortOpInProgress;
	return KErrNotFound;
	}

/**
* QueryExists()
* @param : const TInt : a Query ID. 
* @return :TInt : return index if query exist , else return 0
*/

TInt CMSvSearchSortCacheManager::QueryExists(const TUint32 aQueryID)
	{
	for(TInt IndexEntryCount = 0; IndexEntryCount <iMsvSearchSortCacheManager->iManagerEntry->Count(); IndexEntryCount++)
		{
		if(aQueryID == (*iManagerEntry)[IndexEntryCount]->iQueryID)
			{
			if((*iManagerEntry)[IndexEntryCount]->iMsgExplicitSortPart == EMsvDate)
				{
				iExplicitSortOnDateTime = ETrue;
				}
			else
				{
				iExplicitSortOnDateTime = EFalse;	
				}
			return IndexEntryCount;
			}
		}
	iReturnResultType = KMsvSearchSortQueryIdNotFound;
	return KMsvSearchSortQueryIdNotFound;
	}

/**
* ResetSearchSortCache() : To reste the Search Sort Cache .
* @param :None 
* @return :None.
*/

void CMSvSearchSortCacheManager::ResetSearchSortCache()
	{
	if(iMsvSearchSortCacheManager)
		{
		CMsvSearchSortDeltaCache::Instance()->iNewEntry.Reset();
		CMsvSearchSortDeltaCache::Instance()->iUpdateEntry.Reset();
		CMsvSearchSortDeltaCache::Instance()->iDeleteEntry.Reset();
		iMsvSearchSortCacheManager->iManagerEntry->ResetAndDestroy();
		}
		
	if(iSearchDbAdapter)
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)   	
		iSearchDbAdapter->CloseResources();
#endif			
		delete iSearchDbAdapter;
		iSearchDbAdapter = NULL;
		}
	}

/**
* IsInRange()
* @param :const CMsvSearchSortCacheEntry* 
* @return :TBool : if cache size is available to grow return TRUE, else return False.
*/
TBool CMSvSearchSortCacheManager::IsInRange(const CMsvSearchSortCacheEntry& aEntry)// Will Chek the Size of the Cache.
	{
	iIndexEntryCount = iMsvSearchSortCacheManager->iManagerEntry->Count();
	
	iSearchSortDeltaCache = CMsvSearchSortDeltaCache::Instance();
	iSearchSortDeltaCacheSize = sizeof(*iSearchSortDeltaCache);
	TInt usedSize =0;
	for(TInt ii = 0 ; ii < iIndexEntryCount; ii++)
		{
		usedSize += 4 + sizeof(*((*iManagerEntry)[ii])) ;
		}
	iRequiredSize = 4 + sizeof(aEntry);
	iTotalUsedCacheSize = usedSize + iSearchSortDeltaCacheSize;
	iMsvAvailableSearchSortCacheSize = (iMsvMaximumSearchSortCacheSize - iTotalUsedCacheSize);
	
	if(iMsvAvailableSearchSortCacheSize>iRequiredSize)
		{
		// if delta cache grow too much invalidate the search sort cache and delta cahce.
		if(iSearchSortDeltaCache->iNewEntry.Count() + iSearchSortDeltaCache->iDeleteEntry.Count() + iSearchSortDeltaCache->iUpdateEntry.Count()> KSearchSortDeltaCacheLimit)
			{
			iMsvSearchSortCacheManager->ResetSearchSortCache();
			return ETrue;
			}
		else
			{
			return ETrue;
			}
		}
		
	else
		return EFalse;
	}

/**
* RemoveEntryL()
* @param const TInt : aRequiredSize
* @return TBool :TRUE : if suscessuly remove ,FALSE: if fails to remove from Cache manager.
*/

TBool CMSvSearchSortCacheManager::RemoveEntry(const TInt aRequiredSize) // Will Remove an entry from manager array based on the requirsed Size.
	{
	TBool AllMarkedEntry = EFalse ;
	if(iManagerEntry->Count() != 0)
		{
			TInt count = iManagerEntry->Count();

			for(TInt iStart = 0;  iStart < count;  iStart++)
				{
					TBool  markQuery = (*iManagerEntry)[iStart]->iMarkQuery;
					if(!markQuery) //Entry is Not Marked
						{
						delete (*iManagerEntry)[iStart];
						iMsvSearchSortCacheManager->iManagerEntry->Remove(iStart);

						iIndexEntryCount = iManagerEntry->Count();
						for(TInt ii =0; ii<iIndexEntryCount; ii++)
							{
							iTotalUsedCacheSize +=  4 + sizeof(*((*iManagerEntry)[ii])) ;
							}
						if(iIndexEntryCount == 0)
							{
							iMsvAvailableSearchSortCacheSize = iMsvMaximumSearchSortCacheSize;
							}
						else
							{
							iMsvAvailableSearchSortCacheSize = (iMsvMaximumSearchSortCacheSize - iTotalUsedCacheSize);
							}
						if(aRequiredSize<iMsvAvailableSearchSortCacheSize)
							{
							return ETrue;
							}
						AllMarkedEntry = EFalse;

						}
					else //Marked
						{
						AllMarkedEntry = ETrue;
						}

				}/* end of for loop*/

			if(AllMarkedEntry)	 //All entry are marked.
				{
				for(TInt start=0;  start< count;  start++)
					{
					//TInt entrySize = 4 + sizeof(*((*iManagerEntry)[start]));

					delete (*iManagerEntry)[start];
					iMsvSearchSortCacheManager->iManagerEntry->Remove(start);

					iIndexEntryCount = iManagerEntry->Count();
					for(TInt i =0; i<iIndexEntryCount; i++)
						{
						iTotalUsedCacheSize +=  4 + sizeof(*((*iManagerEntry)[i])) ;
						}
					if(iIndexEntryCount == 0)
						{
						iMsvAvailableSearchSortCacheSize = iMsvMaximumSearchSortCacheSize;
						}
					else
						{
						iMsvAvailableSearchSortCacheSize = (iMsvMaximumSearchSortCacheSize - iTotalUsedCacheSize);
						}
				
					if(aRequiredSize<iMsvAvailableSearchSortCacheSize)
						{
						return ETrue;
						}
			  		 }/* END OF for */

				}/* end of if(iAllMarkedEntry)	 //All entry are marked.*/

		}/*End of if(iManagerEntry->Count()!= 0) */
	return EFalse;
	}


/***********
 * RemoveEndEntry()
 * @param :const TInt : aIndex .
 * @return :TBool 
 *
 ***********/

TBool CMSvSearchSortCacheManager::RemoveEndEntry(const TInt aIndex) 
	{
	delete (*iManagerEntry)[aIndex];
	iMsvSearchSortCacheManager->iManagerEntry->Remove(aIndex);
	return ETrue;
	}


/***********
 * GetTypeOfSearchQuery()
 * @param :CMsvSearchSortCacheEntry& :A Cache Entry .
 * @return :void
 *
 ***********/
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	

void CMSvSearchSortCacheManager::TypeOfSearchQuery(CMsvSearchSortCacheEntry& aEntry)
	{
	
	aEntry.iTypeOfQuery = EQueryInvalid;
	
	TInt iTotalCount=aEntry.iMsgQuery.Count();
	
	//Search
	for(TInt count = 0; count<iTotalCount; count++)
		{
		switch (aEntry.iMsgQuery[count].iMessagePart)
			{
			case EMsvTo:
			case EMsvFrom:
			case EMsvCc:
			case EMsvBcc:
			case EMsvSubject:
				{
				aEntry.iTypeOfQuery |= EQueryOnHeader;
				break;
				}
			
			case EMsvBody:
				{
				aEntry.iTypeOfQuery |= EQueryOnBody;
				break;
				}
				
			case EMsvDescription:
			case EMsvDetails:
			case EMsvDate:  
			case EMsvSize:
			case EMsvAttachment:
				{
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentYes) ==0)
					{
					aEntry.iTypeOfQuery |= EQueryOnIndexEntry;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentNo) ==0)
					{
					aEntry.iTypeOfQuery |= EQueryOnIndexEntry;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentTypeEMsvFile) ==0)
					{
					aEntry.iTypeOfQuery |= EQueryOnBody;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentTypeEMsvLinkedFile) ==0)
					{
					aEntry.iTypeOfQuery |= EQueryOnBody;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentTypeEMsvMessageEntry) ==0)
					{
					aEntry.iTypeOfQuery |= EQueryOnBody;
					break;	
					}
				}
			case EMsvMtmTypeUID:
			case EMsvPriority:
			case EMsvUnreadMessages:
			case EMsvNew:
				{
				aEntry.iTypeOfQuery |= EQueryOnIndexEntry;
				break;
				}
			}
		}
	
	
	if(aEntry.IsExplicitSortReq())
		{
		//Search With explicit sort Sort	
		switch (aEntry.iMsgExplicitSortPart)
			{
			case EMsvDescription:
			case EMsvDetails:
			case EMsvDate:  
			case EMsvSize:
			case EMsvAttachment:
			case EMsvMtmTypeUID:
			case EMsvPriority:
			case EMsvUnreadMessages:
			case EMsvNew:
				{
				aEntry.iSortOnHeader = EFalse;
				}
				break;
				
			case EMsvTo:
			case EMsvFrom:
			case EMsvCc:
			case EMsvBcc:
			case EMsvSubject:
				{
				aEntry.iSortOnHeader = ETrue;
				}
				break;
			
			case EMsvBody:
				{
				aEntry.iSortOnHeader = EFalse;
				}
				break;
			}
		}
	}

#else

void CMSvSearchSortCacheManager::TypeOfSearchQuery(CMsvSearchSortCacheEntry& aEntry)
	{
	TBool aFileAccess = EFalse;
	TBool aIndexAccess =EFalse;
	TInt iTotalCount=aEntry.iMsgQuery.Count();
	//Search
	for(TInt count = 0; count<iTotalCount; count++)
		{
		switch (aEntry.iMsgQuery[count].iMessagePart)
			{
			case EMsvTo:
			case EMsvFrom:
			case EMsvCc:
			case EMsvBcc:
			case EMsvSubject:
			case EMsvBody:
				{
				aFileAccess=ETrue;
				break;
				}
			case EMsvDescription:
			case EMsvDetails:
			case EMsvDate:  
			case EMsvSize:
			case EMsvAttachment:
				{
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentYes) ==0)
					{
					aIndexAccess=ETrue;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentNo) ==0)
					{
					aIndexAccess=ETrue;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentTypeEMsvFile) ==0)
					{
					aFileAccess=ETrue;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentTypeEMsvLinkedFile) ==0)
					{
					aFileAccess=ETrue;
					break;	
					}
				if(aEntry.iMsgQuery[count].iQueryString.Compare(KAttachmentTypeEMsvMessageEntry) ==0)
					{
					aFileAccess=ETrue;
					break;	
					}
				}
			case EMsvMtmTypeUID:
			case EMsvPriority:
			case EMsvUnreadMessages:
			case EMsvNew:
				{
				aIndexAccess=ETrue;
				break;
				}
			}
		}
	
	if(!aEntry.IsExplicitSortReq())	
	{
		if(aFileAccess && !aIndexAccess)
			{
			aEntry.iFileAccess = ETrue;
			aEntry.iQueryType = EHeaderBodyQuery;
			}
		else if(aIndexAccess && !aFileAccess)
			{	
			aEntry.iFileAccess = EFalse;
			aEntry.iQueryType = EIndexQuery;
			}
		else if(aFileAccess && aIndexAccess)
			{
			aEntry.iFileAccess = ETrue;
			aEntry.iQueryType = ECombinedQuery;
			}
		else
			aEntry.iQueryType = ENotValid;
		
	}
	
	else
	{
		
		if(aEntry.IsExplicitSortReq())
		{
			//Search With explicit sort Sort	
			switch (aEntry.iMsgExplicitSortPart)
				{
			case EMsvTo:
			case EMsvFrom:
			case EMsvCc:
			case EMsvBcc:
			case EMsvSubject:
			case EMsvBody:
					{
					aEntry.iSortOnHeader = ETrue;
					break;
					}
			case EMsvDescription:
			case EMsvDetails:
			case EMsvDate:  
			case EMsvSize:
			case EMsvAttachment:
			case EMsvMtmTypeUID:
			case EMsvPriority:
			case EMsvUnreadMessages:
			case EMsvNew:
					{
					aEntry.iSortOnHeader=EFalse;
					break;
					}
				}
			}
			
	

		if(aIndexAccess && !aEntry.iSortOnHeader )
			aEntry.iQueryType = EIndexQuery;
		if(aIndexAccess  && aEntry.iSortOnHeader)
			aEntry.iQueryType = ECombinedQuery;
		if(aFileAccess && aEntry.iSortOnHeader)
			aEntry.iQueryType = EHeaderBodyQuery;
		if(aFileAccess && !aEntry.iSortOnHeader)
		    aEntry.iQueryType = ECombinedQuery;
		
		}
	}

#endif	

	
	

/***********
 * GetTypeOfSortQuery()
 * @param :CMsvSearchSortCacheEntry& :A Cache Entry .
 * @return :void 
 *
 ***********/

void CMSvSearchSortCacheManager::TypeOfSortQuery(CMsvSearchSortCacheEntry& aEntry)
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	aEntry.iTypeOfQuery = EQueryInvalid;
#endif
	
	//Single level of sort
		switch (aEntry.iMsgExplicitSortPart)
			{
			case EMsvTo:
			case EMsvFrom:
			case EMsvCc:
			case EMsvBcc:
			case EMsvSubject:
			case EMsvBody:
				{
			#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
				aEntry.iTypeOfQuery |= EQueryOnHeader;
			#else
				aEntry.iFileAccess = ETrue;
				aEntry.iQueryType = EHeaderBodyQuery;
			#endif	
				aEntry.iSortOnHeader = ETrue;
				break;
				}
			case EMsvDescription:
			case EMsvDetails:
			case EMsvDate:  
			case EMsvSize:
			case EMsvAttachment:
			case EMsvMtmTypeUID:
			case EMsvPriority:
			case EMsvUnreadMessages:
			case EMsvNew:
				{
			#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)				
				aEntry.iTypeOfQuery |= EQueryOnIndexEntry;
			#else	
				aEntry.iFileAccess = EFalse;
				aEntry.iQueryType = EIndexQuery;
			#endif	
				aEntry.iSortOnHeader = EFalse;
				
				break;
				}
			}
	}


/***********
 * GetTypeOfQuery()
 * @param :const CMsvSearchSortCacheEntry& :A Cache Entry .
 * @return :Type of Query 
 *
 ***********/


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
TInt CMSvSearchSortCacheManager::IsTypeOfQuery(const CMsvSearchSortCacheEntry& aEntry)
#else
TTypeOfQuery CMSvSearchSortCacheManager::IsTypeOfQuery(const CMsvSearchSortCacheEntry& aEntry)
#endif
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	return aEntry.iTypeOfQuery;
#else	
	return (aEntry.iQueryType);
#endif	
	}


/***********
 * DoProcessQueryL()
 * @param :const CMsvSearchSortCacheEntry& :A Cache Entry .
 * @return :TInt
 *
 ***********/
//Query is NEW
TInt CMSvSearchSortCacheManager::DoProcessQueryL(const CMsvSearchSortCacheEntry& aEntry, TInt iteratorCount)
	{
	iProgress = KMsvSearchSortOpInProgress;
	if(iteratorCount == 0)
		{//With out iterator.
		iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
		iMsvSearchSortCacheManager->iToFindResultAsIdArray.Reset();

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		
		FinalResultAsIdL(aEntry);
		
		if(iReturnResultType == KFinalResult)
			{
			iProgress = KMsvSearchSortOpCompleted; 
			}
		else
			{
			iProgress = KMsvSearchSortOpInProgress; 
			//New Query it is on Header and Body
			iSearchSortDeltaCache->iIsHeaderSearchEnabled = ETrue;	
			}
				
#else	
		if(EIndexQuery == IsTypeOfQuery(aEntry))
			{
			FinalResultAsIdL(aEntry);
			iReturnResultType = KFinalResult;
			iProgress = KMsvSearchSortOpCompleted; 
			if(iOutstandingSOSOperations>0)
				{
				iOutstandingSOSOperations--;
				}
			}
		else
			{
			FindResultAsIdL(aEntry);
			iReturnResultType = KNewQuery;
			iProgress = KMsvSearchSortOpInProgress; 
			//New Query it is on Header and Body
			iSearchSortDeltaCache->iIsHeaderSearchEnabled = ETrue; 
			}
#endif			
		}
	else
		{//with itarator
		iMsvSearchSortCacheManager->iIteratorId = 0;

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		if(IsTypeOfQuery(aEntry) == EQueryOnIndexEntry)
			{
			FirstResultForInteraratorNewQueryL(aEntry);
			}
#else
		if(EIndexQuery == IsTypeOfQuery(aEntry))
			{
			FirstResultForInteraratorNewQueryL(aEntry);
			iReturnResultType = KFinalResult;
			if(iOutstandingSOSOperations>0)
				{
				iOutstandingSOSOperations--;
				}
			}
#endif
		else 
			{
			return KErrNotSupported;
			// Not Supported.
			}
		}
	//Add the Entry to Manager Object.
	if(!iCancelFlag)
		{
		AddEntryL(aEntry);	
		}
	else
		{
		iProgress = KMsvSearchSortOpCanceled; 
		InstantiateDBwrapperL();
		iSearchDbAdapter->DeleteQueryFromTableL(aEntry.iQueryID);
		iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
		iMsvSearchSortCacheManager->iToFindResultAsIdArray.Reset();
		delete &aEntry; 
		}		
	return KErrNone;
	}


/***********
 * DoProcessQueryL()
 * @param :const TInt aIndex .
 * @return :TInt
 *
 ***********/

//Query is Exist in manager.
TInt CMSvSearchSortCacheManager::DoProcessQueryL(const TInt aIndex)
	{
	iProgress = KMsvSearchSortOpInProgress;
	if((*iManagerEntry)[aIndex]->iteratorCount == 0)
		{
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		if(IsTypeOfQuery(*(*iManagerEntry)[aIndex]) == EQueryOnIndexEntry)
#else					
		if(EIndexQuery == IsTypeOfQuery(*(*iManagerEntry)[aIndex]))
#endif		
			{
			OnDemandUpdateCacheEntryL(aIndex);
			iReturnResultType = KFinalResult;
			iProgress = KMsvSearchSortOpCompleted; 
			if(iOutstandingSOSOperations>0)
				{
				iOutstandingSOSOperations--;
				}
			}
		else
			{
			OnDemandUpdateCacheEntryL(aIndex);
			if(!iSearchSortDeltaCache->iDeltaCacheDirtyFlag)
				{
				iReturnResultType = KFinalResult;
				iProgress = KMsvSearchSortOpCompleted;
				if(iOutstandingSOSOperations>0)
					{
					iOutstandingSOSOperations--;
					}
				}
			else
				{
				for(TInt ii =0; ii<iSearchSortDeltaCache->iNewEntry.Count(); ii++ )
					{
					iDeltaCacheIdArray.AppendL(iSearchSortDeltaCache->iNewEntry[ii]);
					}
			
				for(TInt ii =0; ii<iSearchSortDeltaCache->iUpdateEntry.Count(); ii++ )
					{
					iDeltaCacheIdArray.AppendL(iSearchSortDeltaCache->iUpdateEntry[ii]);
					}
				iReturnResultType = KPartialResult;
				iProgress = KMsvSearchSortOpInProgress;  
				// Partial Query and it is Repetetive  an header and body.
				iSearchSortDeltaCache->iIsHeaderSearchEnabled = ETrue;  
				}	
			}
		}
	else
		{
		iMsvSearchSortCacheManager->iIteratorId = 0;
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		if( IsTypeOfQuery(*(*iManagerEntry)[aIndex]) == EQueryOnIndexEntry )
#else		
		if(EIndexQuery == IsTypeOfQuery(*(*iManagerEntry)[aIndex]))
#endif		
			{
			FirstResultForInteraratorQueryIdL(aIndex);
			iReturnResultType = KFinalResult;
			iProgress = KMsvSearchSortOpIterationEnabled;
			if(iOutstandingSOSOperations>0)
				{
				iOutstandingSOSOperations--;
				}
 			}
		else 
			{
			// Not Supported.
			return KErrNotSupported;
			}
		}
	if(!iCancelFlag)
		{
		if(iMsvSearchSortCacheManager->iManagerEntry->Count()>1)
			{
			MoveEntryAtEnd((*iManagerEntry)[aIndex],aIndex);
			}
		iProgress = KMsvSearchSortOpInProgress;
		}
	else
		{
		iProgress = KMsvSearchSortOpCanceled; 
		}
	return KErrNone;
	}


/***********
 * GetFinalResultAsId()
 * @param :const CMsvSearchSortCacheEntry& :A Cache Entry .
 * @return :TInt : KErrNone: if successful else  return error value.
 *
 ***********/

//New Query on Index entry.
 TInt CMSvSearchSortCacheManager::FinalResultAsIdL(const CMsvSearchSortCacheEntry& aEntry)
	{
	iFinalResultCount = 0;
	iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
	InstantiateDBwrapperL();
 	
 #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
 	TRAPD(err, iSearchDbAdapter->FindL(aEntry, iMsvSearchSortCacheManager->iFinalResultAsIdArray, aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType));
 #else	
 	TRAPD(err,iSearchDbAdapter->FindL(aEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader));
 #endif	
 	if(err!= KErrNone)
 		{
 		iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
 		return err;
 		}
 	iFinalResultCount = iMsvSearchSortCacheManager->iFinalResultAsIdArray.Count();
  	return KErrNone;
	}


/***********
 * GetFindResultAsId()
 * @param :const CMsvSearchSortCacheEntry& :A Cache Entry .
 * @return :TInt : KErrNone: if successful else  return error value.
 *
 ***********/

//New Query on Header and Body.
TInt CMSvSearchSortCacheManager::FindResultAsIdL(const CMsvSearchSortCacheEntry& aEntry)
	{
	iMsvSearchSortCacheManager->iToFindResultAsIdArray.Reset();
	iMsvSearchSortCacheManager->iFindResultCount = 0;
	InstantiateDBwrapperL();
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
 	TRAPD(err, iSearchDbAdapter->FindL(aEntry, iMsvSearchSortCacheManager->iFinalResultAsIdArray, aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType));
 	if(err!= KErrNone)
 		{
 		iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
 		return err;
 		}
	iFinalResultCount = iMsvSearchSortCacheManager->iFinalResultAsIdArray.Count();
#else	
 	TRAPD(err,iSearchDbAdapter->FindL(aEntry,iMsvSearchSortCacheManager->iToFindResultAsIdArray,aEntry.iSortOnHeader));
	if(err!= KErrNone)
 		{
 		iMsvSearchSortCacheManager->iToFindResultAsIdArray.Reset();
 		return err;
 		}
	iMsvSearchSortCacheManager->iFindResultCount = iMsvSearchSortCacheManager->iToFindResultAsIdArray.Count();
#endif	
	return KErrNone;
	}


/**
 * OnDemandUpdateCacheEntryL()
 * @param const TInt aIndex
 * @return TInt
 *
 */

TInt CMSvSearchSortCacheManager::OnDemandUpdateCacheEntryL(const TInt aIndex) //Background task to update the Search and Sort cache.
	{
 	iSearchSortDeltaCache = CMsvSearchSortDeltaCache::Instance();
 	InstantiateDBwrapperL();
	if(iSearchSortDeltaCache->iDeltaCacheDirtyFlag)
		{
		//Delta cache has something
		return UpdateQueryResultOnDemandInDBL(*(*iManagerEntry)[aIndex],iSearchSortDeltaCache->iNewEntry,iSearchSortDeltaCache->iUpdateEntry,iSearchSortDeltaCache->iDeleteEntry);
		}
	else
		{
		iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
		//if nothing is their in delta cache.
		TRAPD(err,iSearchDbAdapter->GetSortedTMsvIdsfromTableL((*iManagerEntry)[aIndex]->iQueryID,iMsvSearchSortCacheManager->iFinalResultAsIdArray, (*iManagerEntry)[aIndex]->IsAscendingSort()/*Default = EFalse*/,(*iManagerEntry)[aIndex]->iMsgExplicitSortPart));
		if(err!= KErrNone)
 			{
 			iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
 			return err;
 			}
	 	}
	return KErrNone;
	}


/**
* UpdateQueryResultOnDemandInDBL(): 
* @param const CMsvSearchSortCacheEntry& aEntry,RArray<TMsvId>& aNewEntry,RArray<TMsvId>& aUpdateEntry, RArray<TMsvId>& aDeleteEntry.
* @return TInt
*/

TInt CMSvSearchSortCacheManager::UpdateQueryResultOnDemandInDBL(const CMsvSearchSortCacheEntry& aEntry,RArray<TMsvId>& aNewEntry,RArray<TMsvId>& aUpdateEntry, RArray<TMsvId>& aDeleteEntry)
	{
	iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
	iMsvSearchSortCacheManager->iToFindResultAsIdArray.Reset();
	InstantiateDBwrapperL();
	
	//TMsvId's that are Deleted can be removed from all Id's as they are invalid
	if(aDeleteEntry.Count()!= 0)
		{
		iSearchDbAdapter->DeleteTMsvIdFromAllQueriesL(aDeleteEntry);
		}
	//TMsvId's that are Updated are removed from the present QueryID,
	//as they are updated they may or may not match the search criteria
	if(aUpdateEntry.Count() !=0)	
		{
		iSearchDbAdapter->DeleteTMsvIdFromQueryL(aEntry.iQueryID,aUpdateEntry);	
		}
	
	//If its EIndexQuery only Query then We need to get the complete result not the updated Id's
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	if(IsTypeOfQuery(aEntry) == EQueryOnIndexEntry)
#else			
	if(EIndexQuery == IsTypeOfQuery(aEntry))
#endif	
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		iSearchDbAdapter->UpdateNewIdsL(aEntry,aUpdateEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else
		iSearchDbAdapter->UpdateNewIdsL(aEntry,aUpdateEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader);
#endif
		if(aNewEntry.Count() >= 0)
			{
			//Resetting IdArray, as we will get the correct result in the next
			//UpdateNewIdsL() call
			iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
			iSearchDbAdapter->UpdateNewIdsL(aEntry,aNewEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else
			iSearchDbAdapter->UpdateNewIdsL(aEntry,aNewEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader);			
#endif			
			}
		}
	
	//If the sort is on Index and ECombinedQuery query we have send the updated and
	// new entries as a single array so that we can get a superset of sorted ids
	// which contains both new and updated id's
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
	else if( IsTypeOfQuery(aEntry) != EQueryOnIndexEntry )
#else
	else if (ECombinedQuery == IsTypeOfQuery(aEntry) && !aEntry.iSortOnHeader)
#endif	
		{
		RArray<TMsvId> updateAndNewIdlist(aUpdateEntry);
		
		if(aNewEntry.Count() >= 0)
			{
			for(TInt index=0; index<aNewEntry.Count(); ++index)
				{
				updateAndNewIdlist.AppendL(aNewEntry[index]);
				}
			}

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		iSearchDbAdapter->UpdateNewIdsL(aEntry,updateAndNewIdlist,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else
		iSearchDbAdapter->UpdateNewIdsL(aEntry,updateAndNewIdlist,iMsvSearchSortCacheManager->iToFindResultAsIdArray,aEntry.iSortOnHeader);
#endif
		
		updateAndNewIdlist.Close();
		}
	
	//When sort is on Header, the expected array is not sorted
	else
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
		iSearchDbAdapter->UpdateNewIdsL(aEntry,aUpdateEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else
		iSearchDbAdapter->UpdateNewIdsL(aEntry,aUpdateEntry,iMsvSearchSortCacheManager->iToFindResultAsIdArray,aEntry.iSortOnHeader);
#endif		
		if(aNewEntry.Count() >= 0)
			{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)		
			iSearchDbAdapter->UpdateNewIdsL(aEntry,aNewEntry,iMsvSearchSortCacheManager->iFinalResultAsIdArray,aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else
			iSearchDbAdapter->UpdateNewIdsL(aEntry,aNewEntry,iMsvSearchSortCacheManager->iToFindResultAsIdArray,aEntry.iSortOnHeader);
#endif			
			}
		}
	return KErrNone;
	}



/**
* GetResultCount(): 
* @param const CMsvSearchSortCacheEntry& aEntry.
* @return TInt
*/

TInt CMSvSearchSortCacheManager::ResultCountL(const CMsvSearchSortCacheEntry& aEntry)
	{
	iMsvSearchSortCacheManager->iFinalResultCount = 0;
	InstantiateDBwrapperL();
	if(!iCancelFlag)
		{
		TRAPD(err,iMsvSearchSortCacheManager->iEntryIdCount = iSearchDbAdapter->ReturnResultCountInTableL(aEntry.iQueryID));
		if(err!= KErrNone)
			{
			iMsvSearchSortCacheManager->iEntryIdCount = 0;
			return err;
			}
		iProgress = KMsvSearchSortOpCompleted;
		return iMsvSearchSortCacheManager->iEntryIdCount;
		}
	else
		{
		return KMsvSearchSortOpCanceled;	
		}
	}
	

/* GetResultCount(): 
 * @param const TInt aIndex.
 * @return TInt
 */

TInt CMSvSearchSortCacheManager::ResultCountL(const TInt aIndex)
	{
	InstantiateDBwrapperL();
	if(!iCancelFlag)
		{
		iMsvSearchSortCacheManager->iFinalResultCount = 0;
		TRAPD(err,iMsvSearchSortCacheManager->iFinalResultCount = iSearchDbAdapter->ReturnResultCountInTableL((*iManagerEntry)[aIndex]->iQueryID));
		if(err!= KErrNone)
			{
			iMsvSearchSortCacheManager->iFinalResultCount = 0;
			return err;
			}
		iProgress = KMsvSearchSortOpCompleted;
		return iMsvSearchSortCacheManager->iFinalResultCount;
		}
	else
		{
		return KMsvSearchSortOpCanceled;	
		}
	}


/**
* GetQueryIDL(): 
* @param None.
* @return TInt
*/

TInt CMSvSearchSortCacheManager::QueryID() 
	{
	if(!iCancelFlag)
		{
		iIndexEntryCount = iMsvSearchSortCacheManager->iManagerEntry->Count();
		if(iIndexEntryCount != 0)
			{
			return (*iManagerEntry)[iIndexEntryCount-1]->iQueryID;	
			}
		else
			{
			return KErrNotFound;
			}	
		}
	else
		{
		return KMsvSearchSortOpCanceled;	
		}
	}


/**
* StoreResult() : 
* @param TInt aIndex, RArray<TMsvId>& aToUpdateDb.
* @return None
*/

void CMSvSearchSortCacheManager::StoreResultL(TInt aIndex, RArray<TMsvId>& aToUpdateDb)
	{
	InstantiateDBwrapperL();
	if(!iCancelFlag)
		{
	 	iSearchDbAdapter->DeleteTMsvIdFromQueryL((*iManagerEntry)[aIndex-1]->iQueryID,aToUpdateDb);
	  	iProgress = KMsvSearchSortOpCompleted;
		}
	
	iMsvSearchSortCacheManager->iFinalResultAsIdArray.Reset();
	iSearchDbAdapter->GetSortedTMsvIdsfromTableL((*iManagerEntry)[aIndex-1]->iQueryID,iMsvSearchSortCacheManager->iFinalResultAsIdArray, (*iManagerEntry)[aIndex-1]->IsAscendingSort()/*Default = EFalse*/,(*iManagerEntry)[aIndex-1]->iMsgExplicitSortPart);
	iReturnResultType = KFinalResult;
	iProgress = KMsvSearchSortOpCompleted; 
	if(iOutstandingSOSOperations>0)
		{
		iOutstandingSOSOperations--;
		}
	aToUpdateDb.Reset();
	}


/**
* StoreResult() : 
* @param TInt aIndex,RArray<TMsvIdWithSortField>& aResultIdData.
* @return None
*/

TInt CMSvSearchSortCacheManager::StoreSortResultL(TInt aIndex,RArray<TMsvIdWithSortField>& aResultIdData)
	{
	InstantiateDBwrapperL();
	if(!iCancelFlag)
		{
		iSearchDbAdapter->AddIdtoDBL((*iManagerEntry)[aIndex-1]->iQueryID, aResultIdData);
		iSearchDbAdapter->GetSortedTMsvIdsfromTableL((*iManagerEntry)[aIndex-1]->iQueryID,iMsvSearchSortCacheManager->iFinalResultAsIdArray, (*iManagerEntry)[aIndex-1]->IsAscendingSort(),(*iManagerEntry)[aIndex-1]->iMsgExplicitSortPart);
		iReturnResultType = KFinalResult;
		iProgress = KMsvSearchSortOpCompleted; 
		}
	iMsvSearchSortCacheManager->iMsvIdWithSortFieldArray.Reset();
    if(iOutstandingSOSOperations>0)
        {
        iOutstandingSOSOperations--;
        }
	return iReturnResultType;
	}


/**
* ReturnIdCount() : Return Total Number of TMsvId .
* @param TInt aQID.
* @return TInt : Total number of TMsvId
*/
	
TInt CMSvSearchSortCacheManager::ReturnIdCountL(TInt aQID)
	{
	InstantiateDBwrapperL();
	iMsvSearchSortCacheManager->iEntryIdCount = iSearchDbAdapter->ReturnResultCountInTableL(aQID);
	return iMsvSearchSortCacheManager->iEntryIdCount;
	} 


/**
* GetFirstForInteraratorL() : 
* @param const CMsvSearchSortCacheEntry& aEntry.
* @return None
*/

void  CMSvSearchSortCacheManager::FirstResultForInteraratorNewQueryL(const CMsvSearchSortCacheEntry& aEntry)
	{
	InstantiateDBwrapperL();
	iProgress = KMsvSearchSortOpIterationInProgress;
	iMsvSearchSortCacheManager->iIteratorId = 0;
	iMsvSearchSortCacheManager->iIteratorId = 0;
 	if(aEntry.iResultType == EMsvResultAsTMsvId)
 		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB) 		
 		iSearchDbAdapter->GetIdsInIteratorNewQueryL(aEntry, aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else 		
 		iSearchDbAdapter->GetIdsInIteratorNewQueryL(aEntry);
#endif 		
  		iProgress = KMsvSearchSortOpCompleted;
		if(iOutstandingSOSOperations>0)
			{
			iOutstandingSOSOperations--;
			}
 		}
	else
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB) 		
 		iSearchDbAdapter->GetIdsInIteratorNewQueryL(aEntry, aEntry.iSortOnHeader, IsTypeOfQuery(aEntry), iReturnResultType);
#else 		
		iSearchDbAdapter->GetIdsInIteratorNewQueryL(aEntry);
#endif
		iProgress = KMsvSearchSortOpCompleted;
		if(iOutstandingSOSOperations>0)
			{
			iOutstandingSOSOperations--;
			}
		}
	}


/**
* FirstResultForInteraratorQueryIdL() : 
* @param const TInt index.
* @return None
*/

void  CMSvSearchSortCacheManager::FirstResultForInteraratorQueryIdL(const TInt index)
	{
	InstantiateDBwrapperL();
	iProgress = KMsvSearchSortOpIterationInProgress;
	iMsvSearchSortCacheManager->iIteratorId = 0;
	//iMsvSearchSortCacheManager->iIteratorId = 0;
 	if((*iManagerEntry)[index]->iResultType == EMsvResultAsTMsvId)
 		{
 		//iSearchDbAdapter->GetIdsInIteratorQueryIdL((*iManagerEntry)[index]->iQueryID);
 		iSearchDbAdapter->GetIdsInIteratorQueryIdL((*iManagerEntry)[index]->iQueryID, (*iManagerEntry)[index]->IsAscendingSort(), (*iManagerEntry)[index]->iMsgExplicitSortPart);
  		iProgress = KMsvSearchSortOpCompleted;
		if(iOutstandingSOSOperations>0)
			{
			iOutstandingSOSOperations--;
			}
   		}
	else
		{
		//iSearchDbAdapter->GetIdsInIteratorQueryIdL((*iManagerEntry)[index]->iQueryID);
		iSearchDbAdapter->GetIdsInIteratorQueryIdL((*iManagerEntry)[index]->iQueryID, (*iManagerEntry)[index]->IsAscendingSort(), (*iManagerEntry)[index]->iMsgExplicitSortPart);
		iProgress = KMsvSearchSortOpCompleted;
		if(iOutstandingSOSOperations>0)
			{
			iOutstandingSOSOperations--;
			}
		}
	}

/**
* GetNextForInteraratorL() : 
* @param TInt aIndex.
* @return None
*/

void  CMSvSearchSortCacheManager::NextResultForInteraratorL(TInt aIndex)
	{
	InstantiateDBwrapperL();
	iMsvSearchSortCacheManager->iIteratorId = 0;
	iProgress = KMsvSearchSortOpIterationInProgress;
 
 	if((*iManagerEntry)[aIndex]->iResultType == EMsvResultAsTMsvId)
 		{
 		//iSearchDbAdapter->GetNextIdL( (*iManagerEntry)[aIndex]->iQueryID , iIteratorId,iIteratorRemainingResultCount);
 		iSearchDbAdapter->GetNextIdL(iIteratorId,iIteratorRemainingResultCount);
 		iProgress = KMsvSearchSortOpCompleted;
		if(iIteratorRemainingResultCount == 0)
			{
			if(iOutstandingSOSOperations>0)
				{
				iOutstandingSOSOperations--;
				}
			}
 		}
	else
		{
		//iIteratorEntry
		//iSearchDbAdapter->GetNextIdL((*iManagerEntry)[aIndex]->iQueryID , iIteratorId,iIteratorRemainingResultCount);
		iSearchDbAdapter->GetNextIdL(iIteratorId,iIteratorRemainingResultCount);
		TMsvEntry *entry;
		iServer.IndexAdapter().GetEntry(iIteratorId,entry);
		iIteratorEntry  = *entry;
		iProgress = KMsvSearchSortOpCompleted;
		if(iIteratorRemainingResultCount == 0)
			{
			if(iOutstandingSOSOperations>0)
				{
				iOutstandingSOSOperations--;
				}
       		}
		}
	}


/**
* UnMarkedQuery() : 
* @param TInt aQid, TInt aindex .
* @return TInt
*/

TInt  CMSvSearchSortCacheManager::UnMarkedQuery(TInt aindex)
	{
	(*iManagerEntry)[aindex]->iMarkQuery = EFalse;
	return KErrNone;
	}


/**
* GenerateQueryId() : 
* @param const CMsvSearchSortCacheEntry.
* @return TInt : Query ID.
*/

TInt CMSvSearchSortCacheManager::GenerateQueryId(CMsvSearchSortCacheEntry& aEntry) 
	{
	TInt queryId = 1001;
	if(iMsvSearchSortCacheManager !=  NULL)
		{
		TInt totalentrycount = iMsvSearchSortCacheManager->iManagerEntry->Count();
		if(iMsvSearchSortCacheManager->iManagerEntry->Count() == 0)
			{
			aEntry.iQueryID = queryId;
			iMaxQueryId = queryId;
			return queryId;
			}
		else
			{
			TInt index = QueryExists(aEntry);
			if(index<0) //Query is not Exist.
				{
				aEntry.iQueryID = iMaxQueryId + 1;
				iMaxQueryId = aEntry.iQueryID ;
				return aEntry.iQueryID;
				}
			else //Query Exist
				{
				queryId = (*iManagerEntry)[index]->iQueryID;
				aEntry.iQueryID = queryId;
				return queryId;
				}
			}
		
		}
	else
		{
		return -1;// EMsvSearchSortCacheMangerNotCreated
		}
	}


/**
* ReturnProgressInfo() : 
* @param None
* @return TInt.
*/

TInt  CMSvSearchSortCacheManager::ReturnProgressInfo() const
	{
	return iProgress;
	}


/**
* CancelOperation()  
* @param None
* @return Tint.
*/

TInt CMSvSearchSortCacheManager::CancelSearchSortOperation() const
	{
	iMsvSearchSortCacheManager->iCancelFlag = ETrue;	
	if(iOutstandingSOSOperations>0)
		{
		iOutstandingSOSOperations--;
		}
	return 0;
	}


/**
* RetrunQuery()  
* @param TMsvSearchSortQuery&
* @return None.
*/

void CMSvSearchSortCacheManager::RetrunQuery(TInt aIndex, CMsvSearchSortQuery* aCLientQuery)
	{

	// iParentId;
	aCLientQuery->iParentId = (*iManagerEntry)[aIndex]->iParentId ;
	//for case sensitive search
	if((*iManagerEntry)[aIndex]->IsCaseSensetive())
		aCLientQuery->iCaseSensitive = ETrue;
	else
		aCLientQuery->iCaseSensitive = EFalse;
	
	//for whole world search
	if((*iManagerEntry)[aIndex]->IsWholeWord())
		aCLientQuery->iWholeWord = ETrue;
	else
		aCLientQuery->iWholeWord = EFalse;
	
	//for Wild Character 
	if((*iManagerEntry)[aIndex]->IsWildCharacter())
		aCLientQuery->iWildCardCharacter = ETrue;
	else
		aCLientQuery->iWildCardCharacter = EFalse;
	
	//for counting level of search
	aCLientQuery->iQueryCount = (*iManagerEntry)[aIndex]->iMsgQuery.Count();
	

	//explicit sort on searched results
	if((*iManagerEntry)[aIndex]->IsExplicitSortReq())
		{
		aCLientQuery->iExplicitSort = ETrue;
		aCLientQuery->iSortOnMessagePart = (*iManagerEntry)[aIndex]->iMsgExplicitSortPart;	
		}
	else
		{
		aCLientQuery->iExplicitSort = EFalse;
		}
	
	//to check whether is this search query 
	if((*iManagerEntry)[aIndex]->IsSortQuery())
		{
		aCLientQuery->iIsSearchQuery = ETrue;
		aCLientQuery->iSortOnMessagePart = (*iManagerEntry)[aIndex]->iMsgExplicitSortPart;	
		}
	else
		aCLientQuery->iIsSearchQuery = EFalse;
		
	// to set result type
	aCLientQuery->iSearchSortResultType = (*iManagerEntry)[aIndex]->iResultType;
	
	// flag for sub folder search
	aCLientQuery->iSubFolderSearch = (*iManagerEntry)[aIndex]->iSubFolderOption;
	
	// Sort option
	if((*iManagerEntry)[aIndex]->IsAscendingSort())
		{
		aCLientQuery->iSortOrder = EMsvSortAscending;
		}
	else
		aCLientQuery->iSortOrder = EMsvSortDescending;
	
	//queryId
	aCLientQuery->iQueryId = (*iManagerEntry)[aIndex]->iQueryID;
	
	TInt ii = (*iManagerEntry)[aIndex]->iMsgQuery.Count();
	for (aCLientQuery->iQueryCount = 0; aCLientQuery->iQueryCount < ii ; aCLientQuery->iQueryCount++)
		{
		aCLientQuery->iQueryTable[aCLientQuery->iQueryCount].iQueryString.Copy((*iManagerEntry)[aIndex]->iQueryData.iQueryString);
		aCLientQuery->iQueryTable[aCLientQuery->iQueryCount].iMessagePart = (*iManagerEntry)[aIndex]->iQueryData.iMessagePart ;
		aCLientQuery->iQueryTable[aCLientQuery->iQueryCount].iRelationOp = (*iManagerEntry)[aIndex]->iQueryData.iRelationOp;
		}
	}

TSearchSortDbWrapper* CMSvSearchSortCacheManager::GetDbWrapper()
	{
	return iSearchDbAdapter;
	}

//For Simultaneously query
void CMSvSearchSortCacheManager::AddOutstandingSOSOperation()
    {
    iOutstandingSOSOperations++;
    }

TInt CMSvSearchSortCacheManager::OutstandingSOSOperations()
    {
    return iOutstandingSOSOperations;
    }
 


