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
// [TestStep Name]
// CreateSearchSortQuery
// [Paramaters]
// ServiceName	: 
// ctreates a query for search and/or sort operation
// [APIs Used]
// TMsvCreateRequest::AddSearchQueryOptions
// TMsvCreateRequest::AddSortQueryOptions
// 
//

/* User Includes */
#include "t_createsearchsortquery.h"
#include "t_utils.h"

/* Epoc includes */
#include <msvsearchsortquery.h>
#include <t_utilsenumconverter.h>

// Literals used 
_LIT(KServiceName, "ServiceName");
_LIT(KFolderName, "FolderName");
_LIT(KSearchCriteriaNumber, "SearchCriteriaNumber");
_LIT(KMessagePart, "MessagePart%d");
_LIT(KQueryString, "QueryString%d");
_LIT(KRelationOp, "RelationOp%d");
_LIT(KSortMsgPart, "SortMsgPart");
_LIT(KSortOrder, "SortOrder");
_LIT(KCaseSensitive, "CaseSensitive");
_LIT(KWildcardSearch, "WildcardSearch");
_LIT(KWholeWordOption, "WholeWordOption");
_LIT(KResultAsTMsvEntry, "ResultAsTMsvEntry");
_LIT(KSubFolderSearch, "SubFolderSearch");
_LIT(KAttachmentTypeSearch, "AttachmentTypeSearch");

/**
 Function : CT_MsgCreateSearchSortQuery
 Description : Constructor
 @param : aSharedDataCommon	A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgCreateSearchSortQuery::CT_MsgCreateSearchSortQuery(CT_MsgSharedDataCommon& aSharedDataCommon)
	:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KCreateSearchSortQuery);
	}

/**
 Function : ~CT_MsgCreateSearchSortQuery
 Description : Destructor
 @return : N/A
 */
CT_MsgCreateSearchSortQuery::~CT_MsgCreateSearchSortQuery()
	{
	}

/**
 Function : doTestStepL
 Description : Creates a serch-sort query object and facilitate the same to be shared across other teststeps.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgCreateSearchSortQuery::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : CreateSearchSortQuery"));

	/*
	Get the folder ID within a given service under which messages would be searched for. 
	NOTE: Folder ID for the folder under remote service can be obatained only if the folder
		  is subscried one or including the invisible entries in the children selection.
	*/

	TMsvId parentId = ParentIdForSearchSortL();
	iSharedDataCommon.iSearchSortQuery = CMsvSearchSortQuery::NewL();
	CleanupStack::PushL(iSharedDataCommon.iSearchSortQuery);
	parentId != KMsvNullIndexEntryIdValue ? iSharedDataCommon.iSearchSortQuery->SetParentId(parentId) : ERR_PRINTF1(_L("Invalid Parent ID"));
	
	if( TestStepResult() == EPass )
		{
		TInt searchCriteriaNumber = 0; // Indicates the number of search criterias, if it is zero no search operation would be performed
		if (!GetIntFromConfig(ConfigSection(), KSearchCriteriaNumber, searchCriteriaNumber))
			{
			ERR_PRINTF1(_L("Number of search fields not specified"));
			SetTestStepResult (EFail);
			}
		else
			{
			TBuf<256> param;
			TInt searchError = KErrNone;
			for(TInt i = 1; (i <= searchCriteriaNumber) && (searchError == KErrNone); i++)
				{
				param.Format(KMessagePart, i);
				TPtrC messagePart;
				TPtrC rel;
				
				if ( !GetStringFromConfig(ConfigSection(), param, messagePart))
					{
					ERR_PRINTF1(_L("Message part to be searched is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					// Converts the string into corresponding TMsvMessagePart enumeration
					TMsvMessagePart msgPartValue = CT_MsgUtilsEnumConverter::ConvertDesToMessagePart(messagePart);
					// Read the query String from the ini file
					param.Format(KQueryString, i);
					TPtrC queryString;
					TInt queryInt;
					TBool queryBool = EFalse;	
					TMsvRelationOp relOp;
					TBool attachmentType = EFalse;
					
					switch(msgPartValue)
						{
					case EMsvUnreadMessages:
					case EMsvNew:
						if(!GetBoolFromConfig(ConfigSection(), param, queryBool))
							{
							ERR_PRINTF1(_L("Query value to be searched for is not specified"));
							SetTestStepResult(EFail);
							}
						TRAP(searchError, iSharedDataCommon.iSearchSortQuery->AddSearchOptionL(msgPartValue, queryBool));
						break;
					case EMsvPriority:
					case EMsvSize:
					case EMsvMtmTypeUID:
						if(!GetIntFromConfig(ConfigSection(), param, queryInt))
							{
							ERR_PRINTF1(_L("Query value to be searched for is not specified"));
							SetTestStepResult(EFail);
							}
						// Read the relational operation to be performed for the query
						param.Format(KRelationOp, i);
						GetStringFromConfig( ConfigSection(), param, rel);
						relOp = CT_MsgUtilsEnumConverter::ConvertDesToMsgRelationOp(rel);
						TRAP(searchError, iSharedDataCommon.iSearchSortQuery->AddSearchOptionL(msgPartValue, queryInt, relOp));
						break;
					case EMsvBcc:
					case EMsvBody:
					case EMsvCc:
					case EMsvDate:
					case EMsvDescription:
					case EMsvDetails:
					case EMsvFrom:
					case EMsvSubject:
					case EMsvTo:
						if(!GetStringFromConfig(ConfigSection(), param, queryString))
							{
							ERR_PRINTF1(_L("String to be searched for is not specified"));
							SetTestStepResult(EFail);
							}
						param.Format(KRelationOp, i);
						GetStringFromConfig( ConfigSection(), param, rel);
						relOp = CT_MsgUtilsEnumConverter::ConvertDesToMsgRelationOp(rel);
						TRAP(searchError, iSharedDataCommon.iSearchSortQuery->AddSearchOptionL(msgPartValue, queryString, relOp));
						break;
					case EMsvAttachment:
						GetBoolFromConfig(ConfigSection(), KAttachmentTypeSearch, attachmentType);
						if (attachmentType) // Decide if the search would be performed for attachment flag or attachment type(File, linked or entry attachment)
							{
							if(!GetIntFromConfig(ConfigSection(), param, queryInt))
								{
								ERR_PRINTF1(_L("Query value to be searched for is not specified"));
								SetTestStepResult(EFail);
								}
							// Read the relational operation to be performed for the query
							param.Format(KRelationOp, i);
							GetStringFromConfig( ConfigSection(), param, rel);
							relOp = CT_MsgUtilsEnumConverter::ConvertDesToMsgRelationOp(rel);
							TRAP(searchError, iSharedDataCommon.iSearchSortQuery->AddSearchOptionL(msgPartValue, queryInt, relOp));
							}
						else
							{
							if(!GetBoolFromConfig(ConfigSection(), param, queryBool))
								{
								ERR_PRINTF1(_L("Query value to be searched for is not specified"));
								SetTestStepResult(EFail);
								}
							TRAP(searchError, iSharedDataCommon.iSearchSortQuery->AddSearchOptionL(msgPartValue, queryBool));
							}
						break;
					default:
						SetTestStepResult(EFail);
						ERR_PRINTF1(_L("Invalid Message part for search"));	
						}
					SetTestStepError(searchError);
					}
				}
			if( (TestStepResult() == EPass) && (searchError == KErrNone))
				{
				// Add the message part accroding to which messages would be sorted, and the order of sorting.
				// If sorting is not required  set KSortMsgPart to TMsvMessagePart::EMsvNone
				TPtrC sortMsgPart;
				if (!GetStringFromConfig(ConfigSection(), KSortMsgPart, sortMsgPart))
					{
					INFO_PRINTF1(_L("Sorting operation is not requested"));
					}
				else
					{
					TPtrC sortOrder;
					TMsvSortOrder order = EMsvSortAscending;
					if(GetStringFromConfig(ConfigSection(), KSortOrder, sortOrder))
						{
						order = CT_MsgUtilsEnumConverter::ConvertDesToSortOrder(sortOrder);
						}
					
					TMsvMessagePart  sortPart = CT_MsgUtilsEnumConverter::ConvertDesToMessagePart(sortMsgPart);
					TRAPD(sortError, iSharedDataCommon.iSearchSortQuery->AddSortOptionL(sortPart, order));
					SetTestStepError(sortError);
					}
				}
			ModifyDefaultSearchSortOptions(); // Modify the default search/sort options
			}
		}
	CleanupStack::Pop(); // Ownership of CMsvSearchSortQuery object passed CMsvSearchSortOpeartion through RequestL function
	return TestStepResult();	
	}
		
/**
 Function : ParentIdForSearchSortL
 Description : Find the parent TMsvId to be considered for search/sort operation
 @return : TMsvId Index entry ID
 */
TMsvId CT_MsgCreateSearchSortQuery::ParentIdForSearchSortL()
	{
	TMsvId parentId = KMsvNullIndexEntryIdValue;
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	TPtrC serviceName;
	TMsvId serviceId;
	GetStringFromConfig(ConfigSection(), KServiceName, serviceName);
	TPtrC folderName;
	GetStringFromConfig(ConfigSection(), KFolderName, folderName);
	TMsvId folderId;
	// if service name is null string consider entry ID of Root entry
	if(serviceName.CompareF(_L("")) == 0)
		{
		parentId = KMsvRootIndexEntryId;
		INFO_PRINTF1(_L("<font color=660099><b>SearchSort would be performed on messages under \"KMsvRootIndexEntryId\" </b></font>"));
		}
 	// service name is not null string and folder name is not null string
	else if(folderName.CompareF(_L("")) != 0)
		{
		TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,serviceName, EFalse));
		if(error)
			{
			ERR_PRINTF2(_L("No such service/account with name: \"%S\" does exist"), &serviceName);
			SetTestStepResult (EFail);
			}
		else
			{
			entry->SetEntryL(serviceId);
			
			if((KMsvNullIndexEntryId != serviceId) && (TUid::Uid(KUidMsvServiceEntryValue) == entry->Entry().iType))
				{
				INFO_PRINTF3(_L("<font color=660099><b>Service ID for \"%S\" = %d</b></font>"), &serviceName, serviceId);
				TRAP(error, folderId = CT_MsgUtils::FindEntryByNameL(entry,folderName, EFalse));
				if(error)
					{
					ERR_PRINTF2(_L("No such folder with name: \"%S\" does exist"), &folderName);
					SetTestStepResult (EFail);
					}
				else
					{
					entry->SetEntryL(folderId);
					if ((KMsvNullIndexEntryId != folderId) && (TUid::Uid(KUidMsvFolderEntryValue) == entry->Entry().iType))
						{
						INFO_PRINTF3(_L("<font color=660099><b>Folder ID for \"%S\" = %d</b></font>"),&folderName, folderId);
						// Specify the folder to be cosidered for search/sort operation.
						parentId = folderId;
						}
					}
				}
			}
		}
	else	// if service entry name is not null and folder name is null search/sort messages under the service entry
		{
		TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,serviceName, EFalse));
		if(error)
			{
			ERR_PRINTF2(_L("No such service/account with name: \"%S\" does exist"), &serviceName);
			SetTestStepResult (EFail);
			}
		else
			{
			parentId = serviceId;
			INFO_PRINTF2(_L("<font color=660099><b>SearchSort would be performed on messages under Service ID \"%d\" </b></font>"), serviceId);
			}
		}
	CleanupStack::PopAndDestroy(entry);
	return parentId;
	}
	
/**
 Function : ModifyDefaultSearchSortOptions
 Description : Modifies the default values of TMsvSearchSortQuery object
 @return : none
 */
void CT_MsgCreateSearchSortQuery::ModifyDefaultSearchSortOptions()
	{
	// Set the wildcard cahracter option to be enabled or not
	TBool wildcardSearch = EFalse;
	GetBoolFromConfig(ConfigSection(), KWildcardSearch, wildcardSearch);	
	iSharedDataCommon.iSearchSortQuery->SetWildCardSearch(wildcardSearch);

	// Set the whole word option to be enabled or not
	TBool wholeWordOption = EFalse;
	GetBoolFromConfig(ConfigSection(), KWholeWordOption, wholeWordOption);	
	iSharedDataCommon.iSearchSortQuery->SetWholeWord(wholeWordOption);

	// Set the option for case sensitve/insensitive search option
	TBool caseSensitiveFlag = EFalse;
	GetBoolFromConfig(ConfigSection(), KCaseSensitive, caseSensitiveFlag);
	iSharedDataCommon.iSearchSortQuery->SetCaseSensitiveOption(caseSensitiveFlag);

	// Set the preferred result type flag, default value is TMsvId 
	TBool resInTMsvEntry = EFalse;
	TMsvSearchSortResultType resultType = EMsvResultAsTMsvId;
	GetBoolFromConfig(ConfigSection(), KResultAsTMsvEntry, resInTMsvEntry);	
	if(resInTMsvEntry)
		{
		resultType = EMsvResultAsTMsvEntry;
		}
	iSharedDataCommon.iSearchSortQuery->SetResultType(resultType);
	
	// Set the subfolder search flag
	TBool subfolderFlag = EFalse;
	GetBoolFromConfig(ConfigSection(), KSubFolderSearch, subfolderFlag);	
	if(subfolderFlag)
		{
		iSharedDataCommon.iSearchSortQuery->SetSubFolderSearch(subfolderFlag);
		}
	}

