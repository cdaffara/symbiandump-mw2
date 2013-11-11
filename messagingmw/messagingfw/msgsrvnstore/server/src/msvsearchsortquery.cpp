
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

#include <msvsearchsortquery.h>
#include <msvids.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvsearchsortconstants.h"
#include "msvconsts.h"
#endif
/** 
Factory function which returns a pointer of CMsvSearchSortQuery to create a search-sort query.
@param NONE 
@return If the function succeeds, this is a pointer to a newly allocated 
and initialised object. 
@leave KErrNoMemory A memory allocation failed 
*/
EXPORT_C CMsvSearchSortQuery* CMsvSearchSortQuery::NewL()
	{
	CMsvSearchSortQuery* self = NewLC();
	CleanupStack::Pop();
	return self;
	}


/** 
Factory function which returns a pointer of CMsvSearchSortQuery to create a search-sort query.
@param NONE 
@return If the function succeeds, this is a pointer to a newly allocated 
and initialised object. 
@leave KErrNoMemory A memory allocation failed 
*/
EXPORT_C CMsvSearchSortQuery* CMsvSearchSortQuery::NewLC()
	{
	CMsvSearchSortQuery* self = new(ELeave) CMsvSearchSortQuery();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

/**
Second phase construction, allocating memory for query table.
@leave KErrNoMemory A memory allocation failed 
*/
void CMsvSearchSortQuery::ConstructL()
	{
	iQueryTable = new(ELeave) TMsvQueryTable[KMaxLevelOfSearchAndSort];
	}


/**
Constructor
*/
CMsvSearchSortQuery::CMsvSearchSortQuery()
	{
	iParentId = KMsvGlobalInBoxIndexEntryId;
	iCaseSensitive = EFalse;
	iWholeWord = EFalse;
	iWildCardCharacter = EFalse;
	iExplicitSort = EFalse;
	iIsSearchQuery = EFalse;
	iSubFolderSearch = EFalse;
	iQueryCount = 0;
	//if sort is not provided by query, there won't any sorting. so it's assigned to '0'
	iSortOnMessagePart = TMsvMessagePart(0);
	iSortOrder = EMsvSortAscending;
	iSearchSortResultType = EMsvResultAsTMsvId;
	iCountMultilevelSort = 0;
	iQueryId = 0;
	}

/** 
Destructor.
*/
EXPORT_C CMsvSearchSortQuery::~CMsvSearchSortQuery()
	{
	//delete iQueryTable
	delete [] iQueryTable;
	}

/**
set aParentId. search and/or sort will be done this aParentId.
@param aParentId  Search and/or sort operation can be performed on a specified root, folder or service. 
*/
EXPORT_C void CMsvSearchSortQuery::SetParentId(const TMsvId aParentId)
	{
	iParentId = aParentId;
	}

/**
Set Case Sensitive search option.
@param aFlag set ETrue or EFalse
*/
EXPORT_C void CMsvSearchSortQuery::SetCaseSensitiveOption (TBool aCaseSensitive)
	{
	iCaseSensitive = aCaseSensitive;
	}

/**
Set Whole word search option.
@param aFlag set ETrue or EFalse
*/
EXPORT_C void CMsvSearchSortQuery::SetWholeWord (TBool aWholeWord)
	{
	iWholeWord = aWholeWord;
	}

/**
Enable wild card character search.
@param aFlag set ETrue or EFalse
*/
EXPORT_C void CMsvSearchSortQuery::SetWildCardSearch (TBool aWildCard)
	{
	iWildCardCharacter = aWildCard;
	}

/**
Client expects search sort results as TMsvId or TMsvEntry
@param aResultType  set result type as TMsvId or TMsvEntry
*/

EXPORT_C void CMsvSearchSortQuery::SetResultType (TMsvSearchSortResultType aSearchSortResultType)
	{
	iSearchSortResultType = aSearchSortResultType;
	}

/**
Enable subfolder search.
@param aFlag set ETrue or EFalse
*/
EXPORT_C void CMsvSearchSortQuery::SetSubFolderSearch (TBool aSubFolderSearch)
	{
	iSubFolderSearch = aSubFolderSearch;
	}


/**
Adds the search options to the search-sort query. 
This function is used when the message part requires descriptor value during 
search-sort operation.

@param aMsgPart A message part on which search operation has to be performed 
for all the messages under a particular folder entry
@param aQueryDesValue The corresponding value of the message part that needs to be searched 
@param aRelOp  An enumeration corresponding to a relational operator used against a 
particular message part for all the messages in a particular folder during 
the search operation
@return None
@leave KErrNone - Query is added successfully to search-sort query

KErrArgument - An invalid value is passed for a particular message part

KErrMsvInvalidOperator - An invalid operation is being performed 
on a particular message part 

KErrMsvQueryMaxLimit - An addition of search-sort operation has lead to a condition 
where the query options have exceeded the maximum number of search-sort options 
supported by messaging framework. The maximum search options that can be added 
to a search-sort query is ‘five’ and the maximum sort option that can be can be 
added is ‘one’ 

KErrQueryTooBig - The search-sort query is too big to be executed. 
This could be due to a bigger value for the search-sort option provided as part of 
the query. The query value added to a particular option is greater then KMsvMaxFindTextLength( = 300)
*/
EXPORT_C void CMsvSearchSortQuery::AddSearchOptionL (TMsvMessagePart aMsgPart, const TDesC& aQueryDesValue, TMsvRelationOp aRelOp)
	{
	//if query string is too big
	if( aQueryDesValue.Length() >= KMsvMaxSearchTextLength)
		{
		User::Leave(KErrQueryTooBig);
		}
	
	//if query string is empty
	if( aQueryDesValue.Length() <= 0)
		{
		User::Leave(KErrArgument);
		}
		
	//check whether it's reached maximum query limit
	if(iQueryCount >= KMaxLevelOfSearchAndSort)
		{
		User::Leave(KErrMsvQueryMaxLimit);
		}

	// validate the query sent by client, before storing into the query structure.
	User::LeaveIfError(ValidateQueryString(aMsgPart, aQueryDesValue, aRelOp));
	
	iIsSearchQuery = ETrue;
	iQueryTable[iQueryCount].iQueryString.Copy(aQueryDesValue);
	iQueryTable[iQueryCount].iMessagePart = aMsgPart;
	iQueryTable[iQueryCount].iRelationOp = aRelOp;
	iQueryCount++;
	}

/**
Adds the search options to the search-sort query. 
This function is used when the message part requires integer value during 
search-sort operation

@param aMsgPart A message part on which search operation has to be performed 
for all the messages under a particular folder entry
@param aQueryIntValue:  The corresponding value of the message part that needs to 
be searched
@param aRelOp An enumeration corresponding to a relational operator used against a 
particular message part for all the messages in a particular folder during the 
search operation 

@return None
@leave KErrNone - Query is added successfully to search-sort query

KErrArgument - An invalid value is passed for a particular message part

KErrMsvInvalidOperator - An invalid operation is being performed 
on a particular message part 

KErrMsvQueryMaxLimit - An addition of search-sort operation has lead to a condition 
where the query options have exceeded the maximum number of search-sort options 
supported by messaging framework. The maximum search options that can be added 
to a search-sort query is ‘five’ and the maximum sort option that can be can be 
added is ‘one’ 
*/
EXPORT_C void CMsvSearchSortQuery::AddSearchOptionL (TMsvMessagePart aMsgPart, const TInt  aQueryIntValue, TMsvRelationOp aRelOp)
	{
	//check whether it's reached maximum query limit
	if(iQueryCount >= KMaxLevelOfSearchAndSort)
		{
		User::Leave(KErrMsvQueryMaxLimit);
		}
 	
	if((aMsgPart == EMsvPriority || aMsgPart == EMsvMtmTypeUID) && (aRelOp == EMsvEqual || aRelOp == EMsvNotEqual))
		{
		iIsSearchQuery = ETrue;
		iQueryTable[iQueryCount].iQueryString.AppendNum(aQueryIntValue);
		iQueryTable[iQueryCount].iMessagePart = aMsgPart;
		iQueryTable[iQueryCount].iRelationOp = aRelOp;
		iQueryCount++;
		}
	else if((aMsgPart == EMsvSize) && (aRelOp == EMsvEqual || aRelOp == EMsvNotEqual || aRelOp == EMsvLessThan || aRelOp == EMsvGreaterThan || aRelOp == EMsvLessThanOrEqual || aRelOp == EMsvGreaterThanOrEqual))
		{
		//validating size field.
		if(aQueryIntValue <= 0)
			{
			User::Leave(KErrArgument);
			}
		//fill size info into query table	
		iIsSearchQuery = ETrue;
		iQueryTable[iQueryCount].iQueryString.AppendNum(aQueryIntValue);
		iQueryTable[iQueryCount].iMessagePart = aMsgPart;
		iQueryTable[iQueryCount].iRelationOp = aRelOp;
		iQueryCount++;
		}
	else if((aMsgPart == EMsvAttachment ) && (aRelOp == EMsvEqual || aRelOp == EMsvNotEqual))
		{
		TInt attachmentValue = 0;
		if(aQueryIntValue == 0)  //to search EMsvFile from CMsvAttachment
			{
			/*
			* Internally assigning value 2 for EMsvFile, 3 for EMsvLinkedFile and 4 for EMsvMessageEntry
			* because values 0 & 1 used to check whether EMsvAttachment present or not. 
			*/
			//for File attachment
			attachmentValue = aQueryIntValue + 2;
			}
		else if(aQueryIntValue == 1) //to search EMsvLinkedFile from CMsvAttachment
			{
			//for Linked file attachment
			attachmentValue = aQueryIntValue + 2;
			}
		else if(aQueryIntValue == 2) //to search EMsvMessageEntry from CMsvAttachment
			{
			//for Message entry attachment
			attachmentValue = aQueryIntValue + 2;
			}
		else
			{
			User::Leave(KErrArgument);
			}
		//fill attachment info into query table	
		iIsSearchQuery = ETrue;
		iQueryTable[iQueryCount].iQueryString.AppendNum(attachmentValue);
		iQueryTable[iQueryCount].iMessagePart = aMsgPart;
		iQueryTable[iQueryCount].iRelationOp = aRelOp;
		iQueryCount++;
		}
	else
		{
		User::Leave(KErrArgument);	
		}
	}
	
/**
Adds the search options to the search-sort query. 
This function is suitable when the message part value to be searched is a flag 
or a message status.

@param aMsgPart A message part on which search operation has to be performed 
for all the messages under a particular folder entry
@param aMessagePartFlagStatus:A Boolean value indicating if the flag indicated 
by the message part is set or not
@return None
@leave KErrNone - Query is added successfully to search-sort query

KErrArgument - An invalid value is passed for a particular message part

KErrMsvInvalidOperator - An invalid operation is being performed 
on a particular message part 

KErrMsvQueryMaxLimit - An addition of search-sort operation has lead to a condition 
where the query options have exceeded the maximum number of search-sort options 
supported by messaging framework. The maximum search options that can be added 
to a search-sort query is ‘five’ and the maximum sort option that can be can be 
added is ‘one’ 
*/

EXPORT_C void CMsvSearchSortQuery::AddSearchOptionL (TMsvMessagePart aMsgPart, TBool aMessagePartFlagStatus)
	{
	//check whether it's reached maximum query limit
	if(iQueryCount >= KMaxLevelOfSearchAndSort)
		{
		User::Leave(KErrMsvQueryMaxLimit);
		}
		
	if(aMsgPart == EMsvNew || aMsgPart == EMsvUnreadMessages || aMsgPart == EMsvAttachment)
		{
		iIsSearchQuery = ETrue;
		
		iQueryTable[iQueryCount].iQueryString.AppendNum(aMessagePartFlagStatus);
		iQueryTable[iQueryCount].iMessagePart = aMsgPart;
		if(aMessagePartFlagStatus)
			{
			iQueryTable[iQueryCount].iRelationOp = EMsvEqual;
			}
		else
			{
			iQueryTable[iQueryCount].iRelationOp = EMsvNotEqual;	
			}
		iQueryCount++;
		}
	else
		{
		User::Leave(KErrArgument);	
		}
	}

/**
Adds sort options to the search-sort query, to enable sorting in ascending or 
descending order based on the message part added to the query

@param aMsgPart: A message part on which a sort operation is performed for all 
the messages under a particular folder entry
@param aSortOder: The sorting order in ascending or descending

@return None
@leave KErrNone - Query is added successfully to search-sort query
KErrNotSupported - for multilevel sort
*/

EXPORT_C void CMsvSearchSortQuery::AddSortOptionL (TMsvMessagePart aMsgPart, TMsvSortOrder aSortOder)
	{
	if(aMsgPart == EMsvBody || iCountMultilevelSort >= 1)
		{
		User::Leave(KErrNotSupported);
		}
	
	if(aMsgPart < EMsvAttachment || aMsgPart > EMsvBody)
		{
		User::Leave(KErrArgument);
		}
				
	if(iIsSearchQuery)
		{
		iExplicitSort = ETrue;
		}
	iSortOnMessagePart = aMsgPart;
	iSortOrder = aSortOder;
	iCountMultilevelSort++;
	}


/**
Validate the query option sent by AddSearchOptionL ()
@param aMsgPart Search needs to be on this TMessagePart
@param aQueryString The text to be found. 
@param aRelOp  TMsgRelationOp enum's needs to used along with TMessagePart to create the query string
@return KErrNone or KErrArgument or KErrMsvInvalidOperator
*/
TInt CMsvSearchSortQuery::ValidateQueryString(TMsvMessagePart aMsgPart, const TDesC& aQueryString, TMsvRelationOp aRelOp)
	{
	// these messageparts should come with EMsvEqual relation operator
	if((ValidateMessagePartAgainstEMsvEqualOperator(aMsgPart)) && aRelOp == EMsvEqual)
		{
		return KErrNone;
		}
	else if((aMsgPart == EMsvDate || aMsgPart == EMsvSize) && (ValidateMessagePartAgainstRelationOp(aRelOp)))
		{
		if(aMsgPart == EMsvDate)
			{
			//validating date field
			//TTime(aQueryString);
			TTime time;
			TInt error = time.Set(aQueryString);
			return error;
			}
		else
			{
			//validating size field.
			TInt size;
			TLex iLex(aQueryString);
			iLex.Val(size);
			if(size <= 0)
				{
				return KErrArgument ;
				}
			return KErrNone;	
			}
		}
	else
		{
		return KErrArgument;
		}
	}


/**
get the message part from Query structure.
@return TBool  ETrue or EFalse
*/
TMsvMessagePart CMsvSearchSortQuery::GetMessagePart(TInt aIndex)
	{
	return iQueryTable[aIndex].iMessagePart;	
	}
	
/**
get the sort field from the Query.
@return TBool  ETrue or EFalse
*/
TMsvMessagePart CMsvSearchSortQuery::GetMessagePart()
	{
	return iSortOnMessagePart;	
	}

/**
get the size of Query.
@return TBool  ETrue or EFalse
*/
TInt CMsvSearchSortQuery::GetMaxQuerySize()
	{
	return iQueryCount;
	}
	

/**
Get Relation operator
@return TSearchSortResultType  results as TMsvId or TMsvEntry
*/
TMsvRelationOp CMsvSearchSortQuery::GetRelationOp(TInt aIndex)
	{
	return iQueryTable[aIndex].iRelationOp;	
	}

/**
Get search sort result type set by client.
@return TSearchSortResultType  results as TMsvId or TMsvEntry
*/
TMsvSearchSortResultType CMsvSearchSortQuery::GetResultType ()
	{
	return iSearchSortResultType;
	}


/**
Function to check whether whole word search enabled or not.
@return ETrue or EFalse
*/
TBool CMsvSearchSortQuery::IsWholeWord()
	{
	return iWholeWord;
	}
	
/**
Function to check whether case sensitivesearch search enabled.
@return ETrue or EFalse
*/
TBool CMsvSearchSortQuery::IsCaseSensitive()
	{
	return iCaseSensitive;
	}

/**
Function to check whether wildcard character search enabled.
@return ETrue or EFalse
*/
TBool CMsvSearchSortQuery::IsWildCardSearch()
	{
	return iWildCardCharacter;
	}

/**
Function to check whether sub folder search enabled.
@return ETrue or EFalse
*/
TBool CMsvSearchSortQuery::IsSubFolderSearch()
	{
	return iSubFolderSearch;
	}

/**
Function to validate, is this search Query.
@return TBool  ETrue or EFalse
*/
TBool CMsvSearchSortQuery::IsSearchQuery()
	{
	return iIsSearchQuery;
	}

//Validate TMsvMessagePart with EMsvEqual Operator
TBool CMsvSearchSortQuery::ValidateMessagePartAgainstEMsvEqualOperator(TMsvMessagePart aMessagePart)
	{
	TBool validate = EFalse;
	switch(aMessagePart)
		{
		case EMsvSubject:
		case EMsvDetails:
		case EMsvDescription:
		case EMsvTo:
		case EMsvFrom:
		case EMsvCc:
		case EMsvBcc:
		case EMsvBody:
			validate = ETrue;
			break;
		
		default:
			validate = EFalse;
		}
	
	return validate;
	}
	
//Validate TMsvMessagePart against TMsvRelationOp Operator
TBool CMsvSearchSortQuery::ValidateMessagePartAgainstRelationOp(TMsvRelationOp aRelationOp)
	{
	TBool validate = EFalse;
	switch(aRelationOp)
		{
		case EMsvEqual:
		case EMsvNotEqual:
		case EMsvLessThan:
		case EMsvLessThanOrEqual:
		case EMsvGreaterThan:
		case EMsvGreaterThanOrEqual:
			validate = ETrue;
			break;
		
		default:
			validate = EFalse;
		}
	
	return validate;
	}
