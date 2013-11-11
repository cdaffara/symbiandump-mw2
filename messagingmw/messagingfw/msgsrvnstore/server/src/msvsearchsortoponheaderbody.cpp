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


#include "msvsearchsortoponheaderbody.h"
#include <mmsvattachmentmanager.h>
#include <msvenhancesearchsortutil.h>

const TInt KMtmArrayGranularity = 4;

/** 
Allocates and constructs a CMsvSearchsortOpOnHeaderBody object.
@param aMsvSession: The client’s Message Server session 
@param aPriority: Priority
@return If function succeeds, pointer to a newly allocated 
and initialised object. 
@leave  one of the leave code if leave occurs in creation of object or clinet reigistry,
*/
EXPORT_C CMsvSearchsortOpOnHeaderBody* CMsvSearchsortOpOnHeaderBody::NewL(CMsvSession& aSession, TInt aPriority)
	{
	CMsvSearchsortOpOnHeaderBody* self = new(ELeave) CMsvSearchsortOpOnHeaderBody(aSession, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
	
CMsvSearchsortOpOnHeaderBody::CMsvSearchsortOpOnHeaderBody(CMsvSession& aSession, TInt aPriority) : CActive(aPriority), iSession(aSession)
	{
	iIdToSearch = 0;
	}
/**
Destructor
*/
	
EXPORT_C CMsvSearchsortOpOnHeaderBody::~CMsvSearchsortOpOnHeaderBody()
	{
	Cancel();
	iIdArray.Close();
	iEntryArray.Close();
	iResultIds.Close();
	iInvalidIds.Close();
	iSelectionArray.Close();
	if (iMtmArray) // Destroy mtm array
		{
		iMtmArray->ResetAndDestroy();
		delete iMtmArray;
		}
	if (iClientRegistry)
		{
		delete iClientRegistry;
		}
	}
	
/**
Second phase construction
*/
void CMsvSearchsortOpOnHeaderBody::ConstructL()
	{
	// Holds details of the all the Client-side MTMs
	iClientRegistry = CClientMtmRegistry::NewL(iSession);
	// An array of MTM list .
	iMtmArray = new(ELeave)CArrayPtrFlat<CBaseMtm>(KMtmArrayGranularity);
	
	CActiveScheduler::Add(this);
	}

/**
SearchSortOnHeaderBodyL
Initialzes the search sort operation. This copies the array of ids  and initialzes different parameters
for the operartion
*/
EXPORT_C void CMsvSearchsortOpOnHeaderBody::SearchSortOnHeaderBodyL(CMsvSearchSortQuery* aQuery, RArray<TMsvId> aSourceArray, TBool aSortOnIndexEntry, TRequestStatus& aStatus)
	{
	//
	ProcessQuery(aQuery, aSourceArray, aSortOnIndexEntry, aStatus);
	StartL();
	}

/**
ProcessQuery
Initialzes the search sort operation. This copies the array of ids  and initialzes different parameters
for the operartion
*/
void CMsvSearchsortOpOnHeaderBody::ProcessQuery(CMsvSearchSortQuery* aQuery, const RArray<TMsvId> aIdArray, TBool aSortOnIndexEntry, TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	iQuery = aQuery;
	iRequestType = EMsvSearchSortOnHeaderBody;
	iSortOnIndexEntry = aSortOnIndexEntry;
	
	for (TInt count = 0; count < aIdArray.Count(); ++count)
		{
		iIdArray.AppendL(aIdArray[count]);	
		}
	//set initial partList list flags
	InitializePartList();
	}
	
/**
 GetResultAsTMsvIdL()
 Returns a list of TMsvIds and the Sort field data
 
@param aArrayOfIdWithSortField  An array holding list of TMsvIds which are final results
@return none.
*/	
EXPORT_C void CMsvSearchsortOpOnHeaderBody::GetResultAsTMsvIdL(RArray<TMsvId>& aIdArray)
	{
	//valid id's
	for(TInt index = 0; index < iResultIds.Count(); ++index)
		{
		aIdArray.AppendL(iResultIds[index]);
		}
	iResultIds.Reset();
	}

/**
 GetResultAsTMsvIdWithSortFieldL()
 Returns a list of TMsvIds and the Sort field data
 
@param aArrayOfIdWithSortField  An array holding list of invalid TMsvIds and sort string
@return none.
*/	
EXPORT_C void CMsvSearchsortOpOnHeaderBody::GetResultAsTMsvIdWithSortFieldL(RArray<TMsvIdWithSortField>& aArrayOfIdWithSortField)
	{
	while(iSelectionArray.Count()!= 0)
		{
		aArrayOfIdWithSortField.AppendL(iSelectionArray[0]);
		iSelectionArray.Remove(0);
		}
	iSelectionArray.Reset();
	}
	
/**
 GetResultAsInvalidTMsvIdL()
 Returns a list of TMsvIds that do not satisfy the search criteria and are not 
 part of the final result set
 
@param aInValidIdArray  An array holding list of invalid TMsvIds
@return none.
*/	

EXPORT_C void CMsvSearchsortOpOnHeaderBody::GetResultAsInvalidTMsvIdL(RArray<TMsvId>& aInValidIdArray)
	{
	//invalid id's
	for(TInt index = 0; index < iInvalidIds.Count(); ++index)
		{
		aInValidIdArray.AppendL(iInvalidIds[index]);
		}
	iInvalidIds.Reset();
	}
	
/**
 GetResultAsTmsvEntryL()

@return An RArray holding TMsvEntry
*/	
EXPORT_C void CMsvSearchsortOpOnHeaderBody::GetResultAsTmsvEntryL(RArray<TMsvEntry>& aEntryArray)
 	{
	while(iEntryArray.Count()!= 0)
		{
		aEntryArray.AppendL(iEntryArray[0]);
		iEntryArray.Remove(0);
		}
	iEntryArray.Reset();
 	}

	

/**
 DoGetEntryForAllIdL()
 Gets TMsvEntry for all Id's in RArray asynchronously
 
@param aIdArray  An array holding list of id's
@param aStatus   Request status
@return none.
*/	
EXPORT_C void CMsvSearchsortOpOnHeaderBody::DoGetEntryForAllIdL(const RArray<TMsvId>& aIdArray, TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	iRequestType = EMsvGetEntryForIds;
	iIdArray.Reset();
	// Get TMsvEntries for the final result
	for(TInt index = 0; index < aIdArray.Count(); ++index)
		{
		iIdArray.AppendL(aIdArray[index]);
		}
	iIdToSearch=0;	
	StartL();
	}

void CMsvSearchsortOpOnHeaderBody::StartL()
	{
	TMsvId service;
	
	// Gets TMsvEntry for the TMsvId specified
	User::LeaveIfError(iSession.GetEntry(iIdArray[iIdToSearch], service, iEntry));
	iIdToSearch++;
	
	if (iEntry.iType == KUidMsvMessageEntry)
		{
		if(iRequestType == EMsvGetEntryForIds)
			{
			iEntryArray.AppendL(iEntry);
			}
		CompleteSelf();
		// set the user request to KRequestPending
		*iRequestStatus = KRequestPending;
		SetActive();
		}
	else
		{
		iInvalidIds.AppendL(iEntry.Id()); // send invalid
		if (iIdToSearch < iIdArray.Count())
			{
			StartL();
			}
		else
			{
			CompleteSelf();
			}
		}
	}

// complete self	
void CMsvSearchsortOpOnHeaderBody::CompleteSelf()
	{
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status, KErrNone);
	}
	

void CMsvSearchsortOpOnHeaderBody::RunL()
	{
	TRAPD(error, DoRunL());
	if (error != KErrNone)
		{
		Complete(error);
		}
	}
	

void CMsvSearchsortOpOnHeaderBody::DoRunL()
	{
	// search on header & body
	if(iRequestType == EMsvSearchSortOnHeaderBody)
		{
		FindInHeaderBodyL();
		}
	
	if (iIdToSearch < iIdArray.Count())
		{
		StartL();		
		}
	else
		{
		Complete(KErrNone);
		}
	
	}
	

/**
FindInHeaderBodyL()
This function performs the actual search, sort or search-sort operation. 
Steps: 1) Initializes partlist flags. These flags are used in the client mtm code to identify
          different message parts and other information.
       2) Checks mtm type for each TMsvId specified in the array. If the mtm is found, continues
          with the operation else registers the particular mtm type and continues.
       3) Load message. This sets the context to the message
       4) If it is a search operation, than it matches each query option specified in the query table
          If all the options are succesfully matched, adds it to resultant list
       5) If sorting is specified than copies the sort data only when the last of the query table condition
          is successfully matched
       6) In case of sorting, call the pure virtual Find method to copy only the sort data.
       7) In case of search by attachment types, restores the attachment manager and identifies the attachment.

@param aQuery: An instance of TMsvCreateSearchSortQuery class. This represents the query.
@param aSourceArray : A RArray containing the TMsvId's that are to looked under.
@param aSortOnIndexEntry : A boolean variable representing if the sort on Index Entry table is true
@param aStatus   :   The TRequestStatus parameter for this request.
@return NONE.

*/
void CMsvSearchsortOpOnHeaderBody::FindInHeaderBodyL()
	{
	
	TMsvPartList parts = 0;
	TMsvIdWithSortField aResultStruct;
	TBool found=EFalse;
	TBool mtmCreationFlag = ETrue;
	TBool cmsventryCreationFlag = EFalse;
	
	CBaseMtm* baseMtm = NULL;
	TInt count = iMtmArray->Count();

	while(count--)  // Check if the MTM type is in array
		{
		if (iMtmArray->At(count)->Type() == iEntry.iMtm)
			{
			baseMtm = iMtmArray->At(count);
			baseMtm->SwitchCurrentEntryL(iEntry.Id());
			break;
			}
		}

	if (!baseMtm)  // If it is a new MTM, add it to the array
		{
		TRAPD(err, baseMtm = iClientRegistry->NewMtmL(iEntry.iMtm));
		if(err != KErrNotFound)
			{
			CleanupStack::PushL(baseMtm);
			iMtmArray->AppendL(baseMtm);
			CleanupStack::Pop(); // baseMtm 

			// Note: SetCurrentEntry takes ownership on entry
			iCMsvEntry = CMsvEntry::NewL(iSession, iEntry.Id(), TMsvSelectionOrdering());
			baseMtm->SetCurrentEntryL(iCMsvEntry);
			cmsventryCreationFlag=ETrue;
			}
		else
			{
			mtmCreationFlag = EFalse;
			iInvalidIds.AppendL(iEntry.Id());
			}
		}
	
	if(mtmCreationFlag)
		{
		baseMtm->SetExtensionData(&iEnhanceSearch);
		TRAPD(error, baseMtm->LoadMessageL());
		if (error == KErrNotSupported) // not all MTMs support LoadMessageL()
			User::Leave(error);
		
		found=ETrue;	
		}
	TBool findexecuted = EFalse;
	
	if (iQuery->IsSearchQuery()) // Check if it is search query
		{
		
		TBool validSearchPart= EFalse;
		// Level of search 
		TInt queryIndex=0;
		
		while( (queryIndex < iQueryCount) && (found != EFalse) )
			{
			validSearchPart=EFalse;
			parts=NULL;
			
			if(iQuery->GetMessagePart(queryIndex) == EMsvAttachment && iQuery->iQueryTable[queryIndex].iQueryString != _L("0") && iQuery->iQueryTable[queryIndex].iQueryString != _L("1"))
				{
				if(!cmsventryCreationFlag)
					{
					iCMsvEntry = CMsvEntry::NewL(iSession, iEntry.Id(), TMsvSelectionOrdering());
					baseMtm->SetCurrentEntryL(iCMsvEntry);
					}
				found = DoAttachmentTypeSearchL(queryIndex);
				}
			else
				{
				// Search based on this flag which could be To, Cc, Bcc , From , Subject,Body
				validSearchPart=AddSearchFlag(iQuery->GetMessagePart(queryIndex),iPartList);
				// invalid searchpart TRUE if the valid message part
			
				if(validSearchPart )
					{
					iTextToFind.Copy(iQuery->iQueryTable[queryIndex].iQueryString);
			    
				    /*
				    if wild card search is enabled than turn off the wild card flag search since 
			    	the descriptor APIs Match() works for wild card only  and Find() works for simple search
			    	*/
				
					if(iQuery->IsWildCardSearch())
						{
						TInt pos1=iTextToFind.Locate('*');
						TInt pos2=iTextToFind.Locate('?');
						
						if(pos1 < 0 && pos2 < 0 )
							{
							iPartList &= ~KMsvFindUsingWildCard;
							}
						}
					/*
					Add a flag specifying that this is the final query option and hence if sorting is
					specified, copy the sort data
					*/		
					if(queryIndex == iQueryCount-1)
						{
						iEnhanceSearch.SetSearchSortSetting(EMessagePartLastQueryOption);
						}
			
					// Perform the search operation
					// If successfull than parts contain the partlist where the text was found
				
					parts = baseMtm->Find(iTextToFind, iPartList);
				
					/* 
					Turn ON wild card flag if wild card search
					*/
					if(iQuery->IsWildCardSearch())
						{
						iPartList|=KMsvFindUsingWildCard;
						}
					
					findexecuted=ETrue;
					
					if (parts == KMsvMessagePartNone && iQuery->GetRelationOp(queryIndex)==EMsvNotEqual)
						{	
						found=ETrue;
						}
					else if (parts == KMsvMessagePartNone && iQuery->GetRelationOp(queryIndex)==EMsvEqual)
						{
						found=EFalse;
						}
					RemoveSearchFlag(iQuery->GetMessagePart(queryIndex),iPartList);
					}
				}
				queryIndex++;
			}
		
		iEnhanceSearch.RemoveSearchSortSetting(EMessagePartLastQueryOption);
		if (found && !iSortOnIndexEntry) // Add to result if successfull
			{
			if(iQuery->iExplicitSort )// search and sort
				{
				/*  
				Executes Find to copy the sort data. This is a cse when the search is on
				Index Entry data and the sort is on header fields
				*/
				if(!findexecuted)
					{
					parts = baseMtm->Find(iTextToFind, iPartList);
					}
					// Copies the sort data for client MTM's	
					
				if(!(iEnhanceSearch.GetSearchSortSetting() & EMessagePartInvalidSortField))
					{
					DoCopySortDataL(baseMtm);
					}
					
				}
			else 
				{
				// these are only valid search results
				iResultIds.AppendL(iEntry.Id()); 
				}
			}
		else if(found && iSortOnIndexEntry)
			{
			iResultIds.AppendL(iEntry.Id());
			}
		else if(!found && iSortOnIndexEntry )
			{
			iInvalidIds.AppendL(iEntry.Id()); 
			}
		else if(!found && !iSortOnIndexEntry)
			{
			iInvalidIds.AppendL(iEntry.Id());
			}
		else if(!found)
			{
			iInvalidIds.AppendL(iEntry.Id());
			}
		else
			{
			// do nothing for removing the warning
			}
		}
	else  if(mtmCreationFlag)  // Only sorting to be done
		{
		// Call Find (This copies the message part to be sorted)
		iTextToFind.Copy(_L(""));
	
		parts = baseMtm->Find(iTextToFind, iPartList);
		
		if(!(iEnhanceSearch.GetSearchSortSetting() & EMessagePartInvalidSortField))
			{
			DoCopySortDataL(baseMtm);			
			}
		}
	if(iEnhanceSearch.GetSearchSortSetting() & EMessagePartInvalidSortField)
		{
		iEnhanceSearch.RemoveSearchSortSetting(EMessagePartInvalidSortField);
		}
	if(iEnhanceSearch.GetSearchSortSetting() & EMessagePartBodyLoaded)
		{
		iEnhanceSearch.RemoveSearchSortSetting(EMessagePartBodyLoaded);
		}
	if(iEnhanceSearch.GetSearchSortSetting() & EMessagePartHeaderLoaded)
		{
		iEnhanceSearch.RemoveSearchSortSetting(EMessagePartHeaderLoaded);
		}
	
	if(baseMtm)
		{
		baseMtm->SetExtensionData(NULL);		
		}
	}

/**
 InitializePartList()
 Adds flags to parts variable before performing the actual operation.
 This method gets called once and the flag value persists until all the
 TMsvIds are exhausted.
 
@param none
@return none.
*/	
	
void CMsvSearchsortOpOnHeaderBody::InitializePartList()
	{
	TBool invalidSortPart= EFalse;
	
	if (iQuery->IsSearchQuery() && !IsOnlySortOperation()) // Check if it is search query
		{
		
		iQueryCount=iQuery->GetMaxQuerySize();
		
		if ( iQuery->IsWholeWord())  
			{
			iPartList|=KMsvFindWholeWord;
			}
		
		// Set flag specifying case sensitive search
		if (iQuery->IsCaseSensitive()) 
			{
			iPartList|=KMsvFindCaseSensitive;
			}
				
		// Set flag specifying wild cards
		if (iQuery->IsWildCardSearch())
			{
			iPartList|=KMsvFindUsingWildCard;
			}
		// Sort based on this flag (Note:Sort on body is not supported)
		if(iQuery->iExplicitSort)
			{
			invalidSortPart=AddSortFlag(iQuery->GetMessagePart());
			if(!invalidSortPart)
				{
				iEnhanceSearch.SetSearchSortSetting(EMessagePartSearchSort);
				}	
			}
		else
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartSearch);
			}
		}
	else
		{
		invalidSortPart=AddSortFlag(iQuery->GetMessagePart());	
		if(!invalidSortPart)
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartSort);
			}
		}	

	}
	
/**
 AddSearchFlag()
 Adds the search specific flag specifying the field on which the search operation is to be 
 performed. 
 
@param TMsvPartList: partlist identifying the field to be searched.
@param TMsvPartList&: partlist modified to set the search flag.
@return none.
*/

TBool CMsvSearchsortOpOnHeaderBody::AddSearchFlag(TMsvPartList aSearchPartList,TMsvPartList& aPartList)
	{
	
	switch(aSearchPartList)
		{
		case EMsvTo:
			{
			aPartList|=KMsvMessagePartTo;
			break;
			}
					
		case EMsvFrom:
			{
			aPartList|=KMsvMessagePartFrom;
			break;
			}

		case EMsvCc:
			{
			aPartList|=KMsvMessagePartCc;
			break;
			}
					
		case EMsvBcc:
			{
			aPartList|=KMsvMessagePartBcc;
			break;
			}

		case EMsvSubject:
			{
			aPartList|=KMsvMessagePartSubject;
			break;
			}

		case EMsvBody:
			{
			aPartList|=KMsvMessagePartBody;
			break;
			}
					
		default:
			{
			return(EFalse);
			}
		}
	return(ETrue);
	}
	
/**
 AddSortFlag()
 Adds the sort specific flag specifying the field on which the result would be sorted.
 
@param TMsvPartList: partlist identifying the field to be sorted.
@param TMsvPartList&: partlist modified to set the sort flag.
@return none.

*/

TBool CMsvSearchsortOpOnHeaderBody::AddSortFlag(TMsvPartList aSortPartList)
	{
	
	switch(aSortPartList)	
		{
		case EMsvTo:
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartToSort);
			break;
			}
		
		case EMsvFrom:
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartFromSort);
			break;
			}
		
		case EMsvCc:
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartCcSort);
			break;
			}
					
		case EMsvBcc:
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartBccSort);
			break;
			}

		case EMsvSubject:
			{
			iEnhanceSearch.SetSearchSortSetting(EMessagePartSubjectSort);
			break;
			}
		
		default:
			return(ETrue);
		}
	
	return(EFalse);
	}

/**
RemoveSearchFlag()
 Removes the search partlist and resets that bit.
 The next query option could be on different field.
   
@param TMsvPartList: partlist identifying the field to be removed.
@param TMsvPartList&: partlist modified to remove the above search flag.
@return none.
*/

void CMsvSearchsortOpOnHeaderBody::RemoveSearchFlag(TMsvPartList aSearchPartList,TMsvPartList& aPartList)
	{
	
	switch(aSearchPartList)
		{
		case EMsvTo:
			{
			aPartList &= ~KMsvMessagePartTo;
			break;
			}
					
		case EMsvFrom:
			{
			aPartList &= ~KMsvMessagePartFrom;
			break;
			}

		case EMsvCc:
			{
			aPartList &= ~KMsvMessagePartCc;
			break;
			}
					
		case EMsvBcc:
			{
			aPartList &= ~KMsvMessagePartBcc;
			break;
			}

		case EMsvSubject:
			{
			aPartList &= ~KMsvMessagePartSubject;
			break;
			}

		case EMsvBody:
			{
			aPartList &= ~KMsvMessagePartBody;
			break;
			}
					
		default:
			{
			break;
			}
		}
	}
/**
DoCopySortDataL()
 copies the sort data. This is obtained by calling the basemtm  getextension method.
   
@param aResultStruct: A temp structure of TMsvId and TBuf buffer.
@param aBaseMtm: An object of CBaseMtm class.
@return none.
*/	

void CMsvSearchsortOpOnHeaderBody::DoCopySortDataL(CBaseMtm* aBaseMtm)
	{
	TMsvIdWithSortField sortdata;
	TInt len=0;
	sortdata.iMessageId=iEntry.Id();
		
	if(iQuery->GetMessagePart() == EMsvTo || iQuery->GetMessagePart() == EMsvCc || iQuery->GetMessagePart() == EMsvBcc )
		{
		CDesCArray* array = static_cast<CDesCArray*>(aBaseMtm->GetExtensionData());
    	TInt toCount = array->Count();
    	if(toCount)
    		{
    		sortdata.iContentMessagePart = array->MdcaPoint(0).Left(KLengthSortData);	
    		}
    	array->Reset();
    	}
	else if(iQuery->GetMessagePart() == EMsvFrom)
		{
		const TPtrC& from = reinterpret_cast<const TUint16* >(aBaseMtm->GetExtensionData());/*fromptr*/
		len= from.Length();
		if(len > KLengthSortData)
			{
			len = KLengthSortData;
			}
		if(len)
			{
			sortdata.iContentMessagePart = from.Left(len);
			}
		}
	else if(iQuery->GetMessagePart() == EMsvSubject)
		{
		const TPtrC& subptr= reinterpret_cast<const TUint16* >(aBaseMtm->GetExtensionData());
		len= subptr.Length();
		if(len > KLengthSortData)
			{
			len = KLengthSortData;
			}
		if(len)
			{
			sortdata.iContentMessagePart = subptr.Left(len);
			}
		}
	iSelectionArray.AppendL(sortdata);			
	}
	
	
void CMsvSearchsortOpOnHeaderBody::DoCancel()
	{
	Complete(KErrCancel);
	}
	
void CMsvSearchsortOpOnHeaderBody::Complete(TInt aStatus)
	{
	TRequestStatus* st = iRequestStatus;
	User::RequestComplete(st, aStatus);
	}
	
TBool CMsvSearchsortOpOnHeaderBody::IsOnlySortOperation()
	{
	TMsvMessagePart part;
	for(TInt queryIndex=0; queryIndex <iQuery->GetMaxQuerySize();queryIndex++)
		{
		part = iQuery->GetMessagePart(queryIndex);
		if(part == EMsvTo || part == EMsvCc || part == EMsvBcc || part == EMsvFrom || part == EMsvSubject || part == EMsvBody || part == EMsvAttachment)
			{
			return EFalse;
			}
		}
	return ETrue;
	}
	
TBool CMsvSearchsortOpOnHeaderBody::DoAttachmentTypeSearchL(TInt aIndex)
	{
	
	TBool found = EFalse;
	
	CMsvStore* store = iCMsvEntry->EditStoreL();
	CleanupStack::PushL(store);
	MMsvAttachmentManager& attachmentMgr = store->AttachmentManagerL();
		
	if(attachmentMgr.AttachmentCount() > 0)
		{
		for (TInt ii = 0; ii <attachmentMgr.AttachmentCount() ; ii++)
			{
			CMsvAttachment* attachmentInfo = attachmentMgr.GetAttachmentInfoL(ii);
			CleanupStack::PushL(attachmentInfo);
				
			if(iQuery->iQueryTable[aIndex].iQueryString == _L("2"))
				{
				if (attachmentInfo->Type() == CMsvAttachment::EMsvFile)
					{
					found=ETrue;
					}
				}
			else if(iQuery->iQueryTable[aIndex].iQueryString == _L("3"))
				{
				if (attachmentInfo->Type() == CMsvAttachment::EMsvLinkedFile)
					{
					found=ETrue;
					}
				}
			else if(iQuery->iQueryTable[aIndex].iQueryString == _L("4"))
				{
				if(attachmentInfo->Type() == CMsvAttachment::EMsvMessageEntry)
					{
					found=ETrue;
					}
				}
			CleanupStack::Pop(attachmentInfo);
			attachmentInfo = NULL;
			}
		}
	CleanupStack::PopAndDestroy(store);
	return found;
	}

	

