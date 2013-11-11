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

#include <e32math.h>
#include "msvsearchsortcacheentry.h"
#include "msvsearchsortcachemanager.h"

/******************************
 * CreateQueryEntryL()
 * @param const TMsvSearchSortQuery& : a Client request Query , const TBool :Mark the Query 
 * @return :CMsvSearchSortCacheEntry* : a handle to a new search sort cache entry. 
 ******************************/ 
 CMsvSearchSortCacheEntry* CMsvSearchSortCacheEntry::CreateQueryEntryL(const CMsvSearchSortQuery* aQuery, const TBool aMarkQuery,TInt aiteratorCount)
	{
	CMsvSearchSortCacheEntry* self = new(ELeave) CMsvSearchSortCacheEntry(aQuery,aMarkQuery,aiteratorCount);
	CleanupStack::PushL(self);
	self->ConstructQueryNodeL(aQuery);
	CleanupStack::Pop(self);
	return self;
	}
 
/********************************
 * Default Constructor :CMsvSearchSortCacheEntry()
 * @param None.
 * @return None.
 ********************************/
/*CMsvSearchSortCacheEntry::CMsvSearchSortCacheEntry()
	{
	}*/

/********************************
 * Default Constructor :CMsvSearchSortCacheEntry()
 * @param None.
 * @return None.
 ********************************/
CMsvSearchSortCacheEntry::CMsvSearchSortCacheEntry(const CMsvSearchSortQuery* aQuery,const TBool aMarkQuery, TInt aiteratorCount)
	{
	iQueryLevel	= aQuery->iQueryCount;
	iSubFolderOption = aQuery->iSubFolderSearch;
	iteratorCount = aiteratorCount;
	iSortOnHeader = EFalse;
	iParentId = aQuery->iParentId;
	iResultType = aQuery->iSearchSortResultType; //Setting the Result Type
	iMarkQuery = aMarkQuery;  //Mark the Query.
	iQueryID = 0;
	iQueryOptions = 0;
	iMsgQuery.Reset();
	}

/*******************************
 * Destructor : ~CMsvCacheEntry()
 * @param None.
 * @return None.
 * ******************************/
CMsvSearchSortCacheEntry::~CMsvSearchSortCacheEntry()
	{
	iMsgQuery.Close();
	}
	
/*******************************
 * ConstructQueryNodeL() : Create a Search Sort Cache Entry.
 * @param const TMsvSearchSortQuery& : a Client request Query , const TBool :Mark the Query 
 * @return None
 ********************************/
void CMsvSearchSortCacheEntry::ConstructQueryNodeL(const CMsvSearchSortQuery* aQuery)
	{
	iMsgQuery.Reset();
	if(!aQuery->iIsSearchQuery)
		{
		DoSortQuery();//Set Search or Sort Query .
		iMsgExplicitSortPart = aQuery->iSortOnMessagePart;
		}
	if(aQuery->iCaseSensitive)
		{
		DoCaseSensetive();//Case Sensitive  bit is set.
		}
	if(aQuery->iWholeWord)
		{
		DoWholeWord();//Whole Word bit is set.
		}
	if(aQuery->iWildCardCharacter)
		{
		DoWildCharacter();//set Wild Character option.
		}
	if(aQuery->iIsSearchQuery)
		{
		//SearchQuery And Explicit Sort is required.
		//Explicit sort leds thier will be no  default sort based on date and time.
		if(aQuery->iExplicitSort)
			{
			DoExplicitSortReq(); // Set the flag for Explicit sorting .
			iMsgExplicitSortPart = aQuery->iSortOnMessagePart;
			}
		else 
			{
			iMsgExplicitSortPart = aQuery->iSortOnMessagePart;
			}
		}
	if(aQuery->iSortOrder == EMsvSortAscending)
		{
		DoAscendingSort();//Ascending or Descending Sort option bit is set. 
		}
	//Query data Appening to a RArray. 
	for (TInt qCount = 0; qCount < aQuery->iQueryCount; qCount++)
		{
		iQueryData.iMessagePart = aQuery->iQueryTable[qCount].iMessagePart;
		iQueryData.iRelationOp = aQuery->iQueryTable[qCount].iRelationOp;
		iQueryData.iQueryString.Copy(aQuery->iQueryTable[qCount].iQueryString);
		iMsgQuery.AppendL(iQueryData);	 
		}
	
		CMSvSearchSortCacheManager::Instance()->GenerateQueryId(*this);
	}

