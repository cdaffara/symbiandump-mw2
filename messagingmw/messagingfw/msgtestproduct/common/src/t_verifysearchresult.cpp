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
// VerifySearchResult
// [Paramaters]
// MessagePart
// SearchValue
// Verify if message entries returned by search/sort mechanism are as per the  search/sort criteria
// [APIs Used]
// TMsvCreateRequest::AddSearchQueryOptions
// 
//

/* User Includes */
#include "t_verifysearchresult.h"

/* Epoc includes */
#include <smut.h>
#include <smuthdr.h>
#include <txtrich.h>
#include <imcmutil.h>
#include <miutmsg.h>
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>
#include <t_utilsenumconverter.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>

// Literals used 
_LIT(KSearchCriteriaNumber, "SearchCriteriaNumber");
_LIT(KMessagePart, "MessagePart%d");
_LIT(KQueryString, "QueryString%d");
_LIT(KRelationOp, "RelationOp%d");
_LIT(KCountOfResult, "CountOfResult");
_LIT(KExpectedResultCount, "ExpectedResultCount");
_LIT(KCaseSensitive, "CaseSensitive");
_LIT(KWildcardSearch, "WildcardSearch");
_LIT(KWholeWordOption, "WholeWordOption");
_LIT(KAttachmentTypeSearch, "AttachmentTypeSearch");

/**
 Function : CT_MsgVerifySearchResult
 Description : Constructor
 @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgVerifySearchResult::CT_MsgVerifySearchResult(CT_MsgSharedDataCommon& aSharedDataCommon) 
: CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KVerifySearchResult);
	}


/**
 Function : doTestStepL
 Description : Verify if message part(s) of the message entries returned by search/sort mechanism are
			   as per the search/sort criteria.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgVerifySearchResult::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : VerifySearchResult"));
	TPtrC fieldName;
	TInt expectedIntValue;
	TPtrC expectedStringValue;
	TBool expectedBoolValue;
	TBool attachmentType = EFalse;
	
	TBool toFieldMatched = ETrue;
	TBool fromFieldMatched = ETrue;
	TBool ccFieldMatched = ETrue;
	TBool bccFieldMatched = ETrue;
	TBool subjectFieldMatched = ETrue;
	TBool sizeMatched = ETrue;
	TBool priorityFlagMatched = ETrue;
	TBool detailsFieldMatched = ETrue;
	TBool descriptionFieldMatched = ETrue;
	TBool attacmentMatched = ETrue;
	TBool bodyPartMatched = ETrue;
	TBool readFlagMatched = ETrue;
	TBool newFlagMatched = ETrue;
	TBool testPassed = ETrue;
	TBool dateMatched = ETrue;
	TBool msgMtmTypeMatched = ETrue;
	TBool wildcardSearch = EFalse;
	TBool caseSensitive = EFalse;
	TBool wholeWord = EFalse;
	
	TInt expectedSearchResultCount = 0;
	GetIntFromConfig(ConfigSection(), KExpectedResultCount, expectedSearchResultCount); // Read search result count

	TInt resultCount = 0;
	GetIntFromConfig(ConfigSection(), KCountOfResult, resultCount); // Read search result count

	if(expectedSearchResultCount > 0 && resultCount == 0)
		{
		ERR_PRINTF2(_L("Expected search Result Count > 0 and Actual search result count = %d"), resultCount);
		SetTestStepResult(EFail);
		}
	else if(expectedSearchResultCount == 0 && resultCount == 0)
		{
		SetTestStepResult(EPass);
		}
	else
		{
		RArray<TMsvEntry> messageEntries = iSharedDataCommon.iSearchSortResultArray;
		INFO_PRINTF2(_L("Number of entries to be verified = %d"), messageEntries.Count());
		TInt searchCriteriaNumber = 1;
		if (!GetIntFromConfig(ConfigSection(), KSearchCriteriaNumber, searchCriteriaNumber))
			{
			ERR_PRINTF1(_L("Number of queries to be processed is not specified"));
			SetTestStepResult (EFail);
			}
		for(TInt i = 1; i <= searchCriteriaNumber && testPassed; ++i)
			{
			TBuf<256> param;
			param.Format(KMessagePart, i);
			if ( !GetStringFromConfig( ConfigSection(), param, fieldName))
				{
				ERR_PRINTF1(_L("Message part to be verified for is not specified"));
				SetTestStepResult (EFail);
				}

			GetBoolFromConfig(ConfigSection(), KWildcardSearch, wildcardSearch);	
			TMsvMessagePart msgPartValue = CT_MsgUtilsEnumConverter::ConvertDesToMessagePart(fieldName);		
			param.Format(KRelationOp, i);
			TPtrC rel;
			// Read the relational operation to be performaed for the query
			GetStringFromConfig( ConfigSection(), param, rel);
			TMsvRelationOp relOp = CT_MsgUtilsEnumConverter::ConvertDesToMsgRelationOp(rel);

			switch(msgPartValue)
				{
			case EMsvTo:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected TO recipent address is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && toFieldMatched; ++index)
						{
						if(messageEntries[index].iMtm == KUidMsgTypeIMAP4 || messageEntries[index].iMtm == KUidMsgTypePOP3 || messageEntries[index].iMtm == KUidMsgTypeSMTP)
							{
							toFieldMatched = VerifyEmailHeaderFieldL(messageEntries[index], expectedStringValue, EMsvTo, wildcardSearch);
							}
						else if(messageEntries[index].iMtm == KUidMsgTypeSMS)
							{
							toFieldMatched = VerifySmsHeaderFieldL(messageEntries[index], expectedStringValue, EMsvTo);
							}
						else
							{
							INFO_PRINTF1(_L("Index entry is not of type EMAIL/SMS"));
							}
						}
					}
				break;
				}
			case EMsvFrom:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected value for the message part is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && fromFieldMatched; ++index)
						{
						if(messageEntries[index].iMtm == KUidMsgTypeIMAP4 || messageEntries[index].iMtm == KUidMsgTypePOP3 || messageEntries[index].iMtm == KUidMsgTypeSMTP)
							{
							fromFieldMatched = VerifyEmailHeaderFieldL(messageEntries[index], expectedStringValue, EMsvFrom, wildcardSearch);
							}
						else if(messageEntries[index].iMtm == KUidMsgTypeSMS)
							{
							fromFieldMatched = VerifySmsHeaderFieldL(messageEntries[index], expectedStringValue, EMsvFrom);
							}
						else
							{
							INFO_PRINTF1(_L("Index entry is not of type EMAIL/SMS"));
							}
						}
					}
					
				break;
				}
			case EMsvCc:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected value for the message part is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && fromFieldMatched; ++index)
						{
						if(messageEntries[index].iMtm == KUidMsgTypeIMAP4 || messageEntries[index].iMtm == KUidMsgTypePOP3 || messageEntries[index].iMtm == KUidMsgTypeSMTP)
							{
							fromFieldMatched = VerifyEmailHeaderFieldL(messageEntries[index], expectedStringValue, EMsvCc, wildcardSearch);
							}
						else
							{
							INFO_PRINTF1(_L("Index entry is not of type EMAIL"));
							}
						}
					}
				break;
				}
			case EMsvBcc:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected value for the message part is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && fromFieldMatched; ++index)
						{
						if(messageEntries[index].iMtm == KUidMsgTypeIMAP4 || messageEntries[index].iMtm == KUidMsgTypePOP3 || messageEntries[index].iMtm == KUidMsgTypeSMTP)
							{
							fromFieldMatched = VerifyEmailHeaderFieldL(messageEntries[index], expectedStringValue, EMsvBcc, wildcardSearch);
							}
						else
							{
							INFO_PRINTF1(_L("Index entry is not of type EMAIL"));
							}
						}
					}
										
				break;
				}
			case EMsvDetails:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected value for the message Details is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && detailsFieldMatched; ++index)
						{
						if(wildcardSearch)
							{
							if(KErrNotFound == messageEntries[index].iDetails.MatchC(expectedStringValue))
								{
								ERR_PRINTF3(_L("EMsvDetails***EMsvEqual test: Message ID \"%d\" Details field \"%S\""), messageEntries[index].Id(), &messageEntries[index].iDetails);
								detailsFieldMatched = EFalse;
								}
							}
						else
							{
							if(KErrNotFound == messageEntries[index].iDetails.FindF(expectedStringValue))
								{
								ERR_PRINTF3(_L("EMsvDetails***EMsvEqual test: Message ID \"%d\" Details field \"%S\""), messageEntries[index].Id(), &messageEntries[index].iDetails);
								detailsFieldMatched = EFalse;
								}
							}
						}
					}
				break;
				}
			case EMsvDescription:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected value for the message Description is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && descriptionFieldMatched; ++index)
						{
						if(wildcardSearch)
							{
							if(KErrNotFound == messageEntries[index].iDescription.MatchC(expectedStringValue))
								{
								ERR_PRINTF3(_L("EMsvDetails***EMsvEqual test: Message ID \"%d\" Description field \"%S\""), messageEntries[index].Id(), &messageEntries[index].iDetails);
								detailsFieldMatched = EFalse;
								}
							}
						else
							{
							if(KErrNotFound == messageEntries[index].iDescription.FindF(expectedStringValue))
								{
								ERR_PRINTF3(_L("EMsvDescription***EMsvEqual test: Message ID \"%d\" Description field \"%S\""), messageEntries[index].Id(), &messageEntries[index].iDescription);
								descriptionFieldMatched = EFalse;
								}
							}
						}
					}
				break;
				}
			case EMsvSubject:
				{
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected value for the message part is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					for(TInt index = 0; (index < messageEntries.Count()) && fromFieldMatched; ++index)
						{
						if(messageEntries[index].iMtm == KUidMsgTypeIMAP4 || messageEntries[index].iMtm == KUidMsgTypePOP3 || messageEntries[index].iMtm == KUidMsgTypeSMTP)
							{
							fromFieldMatched = VerifyEmailHeaderFieldL(messageEntries[index], expectedStringValue, EMsvSubject, wildcardSearch);
							}
						else
							{
							INFO_PRINTF1(_L("Index entry is not of type EMAIL/SMS"));
							}
						}
					}
				break;
				}	
			case EMsvUnreadMessages:
				readFlagMatched = ETrue;
				param.Format(KQueryString, i);
				if (!GetBoolFromConfig(ConfigSection(), param, expectedBoolValue))
					{
					ERR_PRINTF1(_L("Expected UnRead flag is not specified"));
					SetTestStepResult (EFail);
					}
				switch(expectedBoolValue)
					{
				case ETrue:
					for(TInt index = 0; (index < messageEntries.Count() && readFlagMatched); ++index)
						{
						if(!messageEntries[index].Unread()) // Checks if the messages
							{
							ERR_PRINTF2(_L("EMsvUnreadMessages***ETrue test: Message enty with TMsvId \"%d\" is with unread flag EFalse"), messageEntries[index].Id());
							readFlagMatched = EFalse;
							}
						}
					break;
				case EFalse:
					for(TInt index = 0; (index < messageEntries.Count() && readFlagMatched); ++index)
						{
						if(messageEntries[index].Unread()) // Checks if the messages
							{
							ERR_PRINTF2(_L("EMsvUnreadMessages***EFalse test: Message enty with TMsvId \"%d\" is with unread flag ETrue"), messageEntries[index].Id());
							readFlagMatched = EFalse;
							}
						}
					break;
				default:
					SetTestStepResult(EFail);
					ERR_PRINTF1(_L("Invalid Boolean value for Unread flag"));	
					}
				break;	
			case EMsvNew:
				newFlagMatched = ETrue;
				param.Format(KQueryString, i);
				if (!GetBoolFromConfig(ConfigSection(), param, expectedBoolValue))
					{
					ERR_PRINTF1(_L("Expected New flag is not specified"));
					SetTestStepResult (EFail);
					}
				switch(expectedBoolValue)
					{
				case ETrue:
					for(TInt index = 0; (index < messageEntries.Count() && newFlagMatched); ++index)
						{
						if(!messageEntries[index].New()) // Checks if the messages
							{
							ERR_PRINTF2(_L("EMsvNew***ETrue test: Message enty with TMsvId \"%d\" is with New flag EFalse"), messageEntries[index].Id());
							newFlagMatched = EFalse;
							}
						}
					break;
				case EFalse:
					for(TInt index = 0; (index < messageEntries.Count() && newFlagMatched); ++index)
						{
						if(!messageEntries[index].New()) // Checks if the messages
							{
							ERR_PRINTF2(_L("EMsvNew***EFalse test: Message enty with TMsvId \"%d\" is with New flag ETrue"), messageEntries[index].Id());
							newFlagMatched = EFalse;
							}
						}
					break;
				default:
					SetTestStepResult(EFail);
					ERR_PRINTF1(_L("Invalid Boolean value for New flag"));	
					}
				break;	
			case EMsvDate:
				{
				param.Format(KQueryString, i);
				
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected Date for the Date field is not specified"));
					SetTestStepResult(EFail);
					}
				TTime time;
				TInt err = time.Parse(expectedStringValue);
				
				switch(relOp)
					{
				case EMsvNotEqual:
					for(TInt j = 0; j < messageEntries.Count() && dateMatched; ++j)
						{
						if(messageEntries[j].iDate == time)
							{
							ERR_PRINTF4(_L("EMsvDate *** ENotEqual test: Message enty with TMsvId \"%d\" is of time \"%S\" : Query value for time = \"%d\""), messageEntries[j].Id(), &messageEntries[j].iDate, &expectedIntValue);
							dateMatched = EFalse;
							}
						}
					break;
				case EMsvLessThan:
					for(TInt j = 0; j < messageEntries.Count() && dateMatched; ++j)
						{
						if(messageEntries[j].iDate >= time)
							{
							ERR_PRINTF4(_L("EMsvDate *** EMsvLessThan test: Message enty with TMsvId \"%d\" is of time \"%S\" : Query value for time = \"%d\""), messageEntries[j].Id(), &messageEntries[j].iSize, &expectedIntValue);
							dateMatched = EFalse;
							}
						}
					break;	
				case EMsvGreaterThan:
					for(TInt j = 0; j < messageEntries.Count() && dateMatched; ++j)
						{
						if(messageEntries[j].iDate <= time)
							{
							ERR_PRINTF4(_L("EMsvDate *** EMsvGreaterThan test: Message enty with TMsvId \"%d\" is of time \"%S\" : Query value for time = \"%d\""), messageEntries[j].Id(), &messageEntries[j].iSize, &expectedIntValue);
							dateMatched = EFalse;
							}
						}
					break;	
				case EMsvLessThanOrEqual:
					for(TInt j = 0; j < messageEntries.Count() && dateMatched; ++j)
						{
						if(messageEntries[j].iDate > time)
							{
							ERR_PRINTF4(_L("EMsvDate *** EMsvLessThanOrEqual test: Message enty with TMsvId \"%d\" is of time \"%S\" : Query value for time = \"%d\""), messageEntries[j].Id(), &messageEntries[j].iSize, &expectedIntValue);
							dateMatched = EFalse;
							}
						}
					break;
				case EMsvGreaterThanOrEqual:
					for(TInt j = 0; j < messageEntries.Count() && dateMatched; ++j)
						{
						if(messageEntries[j].iDate < time)
							{
							ERR_PRINTF4(_L("EMsvDate *** EMsvGreaterThanOrEqual test: Message enty with TMsvId \"%d\" is of time \"%S\" : Query value for time = \"%d\""), messageEntries[j].Id(), &messageEntries[j].iSize, &expectedIntValue);
							dateMatched = EFalse;
							}
						}
					break;
				default:
					ERR_PRINTF1(_L("Invalid Relational operator for DATE field"));
					SetTestStepResult(EFail);
					}
				break;
				}
			case EMsvSize:
				{
				param.Format(KQueryString, i);
				if (!GetIntFromConfig(ConfigSection(), param, expectedIntValue))
					{
					ERR_PRINTF1(_L("Expected value for the message part is not specified"));
					SetTestStepResult (EFail);
					}
				switch(relOp)
					{
				case EMsvNotEqual:
					for(TInt j = 0; j < messageEntries.Count() && sizeMatched; ++j)
						{
						if(messageEntries[j].iSize == expectedIntValue)
							{
							ERR_PRINTF4(_L("EMsgSize *** EMsvNotEqual test: Message enty with TMsvId \"%d\" is of size \"%d\" : Query value for size = \"%d\""), messageEntries[j].Id(), messageEntries[j].iSize, expectedIntValue);
							sizeMatched = EFalse;
							}
						}
					break;	
				case EMsvLessThan:
					for(TInt j = 0; j < messageEntries.Count() && sizeMatched; ++j)
						{
						if(messageEntries[j].iSize >= expectedIntValue)
							{
							ERR_PRINTF4(_L("EMsvSize *** EMsvLessThan test: Message enty with TMsvId \"%d\" is of size \"%d\" : Query value for size = \"%d\""), messageEntries[j].Id(), messageEntries[j].iSize, expectedIntValue);
							sizeMatched = EFalse;
							}
						}
					break;	
				case EMsvGreaterThan:
					for(TInt j = 0; j < messageEntries.Count() && sizeMatched; ++j)
						{
						if(messageEntries[j].iSize <= expectedIntValue)
							{
							ERR_PRINTF4(_L("EMsvSize *** EMsvGreaterThan test: Message enty with TMsvId \"%d\" is of size \"%d\" : Query value for size = \"%d\""), messageEntries[j].Id(), messageEntries[j].iSize, expectedIntValue);
							sizeMatched = EFalse;
							}
						}
					break;	
				case EMsvLessThanOrEqual:
					for(TInt j = 0; j < messageEntries.Count() && sizeMatched; ++j)
						{
						if(messageEntries[j].iSize > expectedIntValue)
							{
							ERR_PRINTF4(_L("EMsvSize *** EMsvLessThanOrEqual test: Message enty with TMsvId \"%d\" is of size \"%d\" : Query value for size = \"%d\""), messageEntries[j].Id(), messageEntries[j].iSize, expectedIntValue);
							sizeMatched = EFalse;
							}
						}
					break;
				case EMsvGreaterThanOrEqual:
					for(TInt j = 0; j < messageEntries.Count() && sizeMatched; ++j)
						{
						if(messageEntries[j].iSize < expectedIntValue)
							{
							ERR_PRINTF4(_L("EMsvSize *** EMsvGreaterThanOrEqual test: Message enty with TMsvId \"%d\" is of size \"%d\" : Query value for size = \"%d\""), messageEntries[j].Id(), messageEntries[j].iSize, expectedIntValue);
							sizeMatched = EFalse;
							}
						}
					break;
				case EMsvEqual:
					for(TInt j = 0; j < messageEntries.Count() && sizeMatched; ++j)
						{
						if(messageEntries[j].iSize != expectedIntValue) // Checks if the messages
							{
							ERR_PRINTF4(_L("EMsvSize *** EMsvEqual test: Message enty with TMsvId \"%d\" is of size \"%d\" : Query value for size = \"%d\""), messageEntries[j].Id(), messageEntries[j].iSize, expectedIntValue);
							sizeMatched = EFalse;
							}
						}
					}
					break;	
				default:
					ERR_PRINTF1(_L("Invalid Relational operator for SIZE field"));
					SetTestStepResult(EFail);
					}
				break;	
			case EMsvAttachment: // Verification for Attachment search
				param.Format(KQueryString, i);
				GetBoolFromConfig(ConfigSection(), KAttachmentTypeSearch, attachmentType);
				if(attachmentType)
					{
					if (!GetIntFromConfig(ConfigSection(), param, expectedIntValue))
						{
						ERR_PRINTF1(_L("Expected Attachment Type value is not specified"));
						SetTestStepResult (EFail);
						}
					switch(expectedIntValue)
						{
					case 0:
						for(TInt j = 0; j < messageEntries.Count() && attacmentMatched; ++j)
							{
							CMsvEntry* entry = iSharedDataCommon.iSession->GetEntryL(messageEntries[j].Id());
			//				CMsvEntry* entry = iSharedDataCommon.iSession->GetEntryL(1048632);
							CleanupStack::PushL(entry);
							CMsvStore* store = entry->ReadStoreL();
							CleanupStack::PushL(store);
							
							MMsvAttachmentManager& manager = store->AttachmentManagerL();
							TInt numberOfAttachments = manager.AttachmentCount();
							while(numberOfAttachments > 0)
								{
								CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(--numberOfAttachments);
								CleanupStack::PushL(attachInfo);
								if(attachInfo->Type() == CMsvAttachment::EMsvFile) // Checks if the message has file attachment
									{
									attacmentMatched = ETrue;
									CleanupStack::PopAndDestroy(1, attachInfo);
									break;
									}
								else
									{
									attacmentMatched = EFalse;
									}
								CleanupStack::PopAndDestroy(1, attachInfo);
								}
							if(!attacmentMatched)
								{
								ERR_PRINTF2(_L("EMsvAttachment *** EMsvFile test: Message enty with TMsvId \"%d\" doesn't have File attachment"), messageEntries[j].Id());
								}
							CleanupStack::PopAndDestroy(2, entry);
							}
						break;	
					case 1:
						for(TInt j = 0; j < messageEntries.Count() && attacmentMatched; ++j)
							{
							CMsvEntry* entry = iSharedDataCommon.iSession->GetEntryL(messageEntries[j].Id());
							CleanupStack::PushL(entry);
							CMsvStore* store = entry->ReadStoreL();
							CleanupStack::PushL(store);
							
							MMsvAttachmentManager& manager = store->AttachmentManagerL();
							TInt numberOfAttachments = manager.AttachmentCount();
							while(numberOfAttachments > 0)
								{
								CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(--numberOfAttachments);
								CleanupStack::PushL(attachInfo);
								if(attachInfo->Type() == CMsvAttachment::EMsvLinkedFile) // Checks if the message has file attachment
									{
									attacmentMatched = ETrue;
									CleanupStack::PopAndDestroy(1, attachInfo);
									break;
									}
								else
									{
									attacmentMatched = EFalse;
									}
								CleanupStack::PopAndDestroy(1, attachInfo);
								}
							if(!attacmentMatched)
								{
								ERR_PRINTF2(_L("EMsvAttachment *** EMsvFile test: Message enty with TMsvId \"%d\" doesn't have File attachment"), messageEntries[j].Id());
								}
							CleanupStack::PopAndDestroy(2, entry);
							}
						break;	
					case 2:
						for(TInt j = 0; j < messageEntries.Count() && attacmentMatched; ++j)
							{
							CMsvEntry* entry = iSharedDataCommon.iSession->GetEntryL(messageEntries[j].Id());
							CleanupStack::PushL(entry);
							CMsvStore* store = entry->ReadStoreL();
							CleanupStack::PushL(store);
							
							MMsvAttachmentManager& manager = store->AttachmentManagerL();
							TInt numberOfAttachments = manager.AttachmentCount();
							while(numberOfAttachments > 0)
								{
								CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(--numberOfAttachments);
								CleanupStack::PushL(attachInfo);
								if(attachInfo->Type() == CMsvAttachment::EMsvMessageEntry) // Checks if the message has file attachment
									{
									attacmentMatched = ETrue;
									CleanupStack::PopAndDestroy(1, attachInfo);
									break;
									}
								else
									{
									attacmentMatched = EFalse;
									}
								CleanupStack::PopAndDestroy(1, attachInfo);
								}
							if(!attacmentMatched)
								{
								ERR_PRINTF2(_L("EMsvAttachment *** EMsvFile test: Message enty with TMsvId \"%d\" doesn't have File attachment"), messageEntries[j].Id());
								}
							CleanupStack::PopAndDestroy(2, entry);
							}
						break;
					default:
						SetTestStepResult(EFail);
						ERR_PRINTF1(_L("Invalid Integer value for Attachment Type"));	
						}
					}
				else
					{
					if (!GetBoolFromConfig(ConfigSection(), param, expectedBoolValue))
						{
						ERR_PRINTF1(_L("Expected Attachment flag is not specified"));
						SetTestStepResult (EFail);
						}
					switch(expectedBoolValue)
						{
					case ETrue:
						for(TInt j = 0; j < messageEntries.Count() && attacmentMatched; ++j)
							{
							if(!messageEntries[j].Attachment()) // Checks if the message has no attachment
								{
								ERR_PRINTF3(_L("EMsvAttachment *** ETrue test: Message enty with TMsvId \"%d\" is with attachment flag \"%d\""), messageEntries[j].Id(), messageEntries[j].Attachment());
								attacmentMatched = EFalse;
								}
							}
						break;
					case EFalse:
						for(TInt j = 0; j < messageEntries.Count() && attacmentMatched; ++j)
							{
							if(messageEntries[j].Attachment()) // Checks if the message has no attachment
								{
								ERR_PRINTF3(_L("EMsvAttachment *** EFalse test: Message enty with TMsvId \"%d\" is with attachment flag \"%d\""), messageEntries[j].Id(), messageEntries[j].Attachment());
								attacmentMatched = EFalse;
								}
							}
						break;
					default:
						SetTestStepResult(EFail);
						ERR_PRINTF1(_L("Invalid Boolean value for Attachment flag"));	
						}
					}
				break;
			case EMsvMtmTypeUID:
				{
				param.Format(KQueryString, i);
				if (!GetIntFromConfig(ConfigSection(), param, expectedIntValue))
					{
					ERR_PRINTF1(_L("Expected MTM UID is not specified"));
					SetTestStepResult (EFail);
					}
				TUid mtmUid=TUid::Uid(expectedIntValue);
				for(TInt j = 0; j < messageEntries.Count(); ++j)
					{
					if(messageEntries[j].iMtm != mtmUid)
						{
						INFO_PRINTF2(_L("Message ID %d is of MTM type"), messageEntries[j].Id());
						msgMtmTypeMatched = EFalse;
						break;
						}
					}
				break;
				}
			case EMsvBody: // Verification for Body part search
				{
				TMsvPartList partlist = KMsvMessagePartNone;
				TMsvPartList state = KMsvMessagePartNone;
				param.Format(KQueryString, i);
				if (!GetStringFromConfig(ConfigSection(), param, expectedStringValue))
					{
					ERR_PRINTF1(_L("Expected string in the bodypart is not specified"));
					SetTestStepResult (EFail);
					}
				else
					{
					CImClientMTMUtils *cliMmtUtil = CImClientMTMUtils::NewL();
					CleanupStack::PushL(cliMmtUtil);
					CParaFormatLayer*	paraFormatLayer = CParaFormatLayer::NewL();
					CleanupStack::PushL(paraFormatLayer);
					CCharFormatLayer*	charFormatLayer = CCharFormatLayer::NewL(); 
					CleanupStack::PushL(charFormatLayer);
					CRichText*	bodyText  = CRichText::NewL(paraFormatLayer, charFormatLayer, CEditableText::EFlatStorage);
					CleanupStack::PushL(bodyText);
					if(wildcardSearch)
						{
						partlist|=KMsvFindUsingWildCard;	
						}
					GetBoolFromConfig(ConfigSection(), KCaseSensitive, caseSensitive);
					if(caseSensitive)
						{
						partlist|=KMsvFindCaseSensitive;	
						}
					GetBoolFromConfig(ConfigSection(), KWholeWordOption, wholeWord);
					if(wholeWord)
						{
						partlist|=KMsvFindWholeWord;	
						}
					
					CMsvEntry *entry = CMsvEntry::NewL(*(iSharedDataCommon.iSession), KMsvRootIndexEntryId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
					CleanupStack::PushL(entry);
					for(TInt j = 0; j < messageEntries.Count() && bodyPartMatched; ++j)
						{
						if(wildcardSearch)
							{
							TInt pos1=expectedStringValue.Locate('*');
							TInt pos2=expectedStringValue.Locate('?');
					
							if(pos1 < 0 && pos2 < 0 )
								{
								partlist-=KMsvFindUsingWildCard;
								}
							}
											
						entry->SetEntryL(messageEntries[j].Id());
						CImEmailMessage *emailMessage = CImEmailMessage::NewLC(*entry);
						emailMessage->GetBodyTextL(messageEntries[j].Id(), CImEmailMessage::EThisMessageOnly, *bodyText, *paraFormatLayer, *charFormatLayer);
						state = cliMmtUtil->FindInBodyL(expectedStringValue, *bodyText, partlist); 
						if(state != KMsvMessagePartBody)
							{
							bodyPartMatched = EFalse;
							ERR_PRINTF3(_L("EMsvBody *** EMsvEqual test: Body Part of Message with TMsvId \"%d\" doesn't contain \"%S\""), messageEntries[j].Id(), &expectedStringValue);
							}
						if(wildcardSearch)
							{
							partlist|=KMsvFindUsingWildCard;
							}
						
						CleanupStack::PopAndDestroy(emailMessage);
						}
					CleanupStack::PopAndDestroy(5,cliMmtUtil);	
					}
				break;
				}
			case EMsvPriority: // Verification for message with specified priority
				param.Format(KQueryString, i);
				if (!GetIntFromConfig(ConfigSection(), param, expectedIntValue))
					{
					ERR_PRINTF1(_L("Expected value for the message priority is not specified"));
					SetTestStepResult (EFail);
					}
				for(TInt j = 0; j < messageEntries.Count(); ++j)
					{
					if(messageEntries[j].Priority() != expectedIntValue)
						{
						ERR_PRINTF4(_L("EMsvPriority *** EMsvEqual test: Message enty with TMsvId \"%d\" is of priority \"%d\" : Query value for priority = \"%d\""), messageEntries[j].Id(), messageEntries[j].Priority(), expectedIntValue);
						priorityFlagMatched = EFalse;
						}
					}
				break;
				}
			testPassed = toFieldMatched && fromFieldMatched && ccFieldMatched && bccFieldMatched && subjectFieldMatched && sizeMatched && priorityFlagMatched && readFlagMatched && detailsFieldMatched && 
			descriptionFieldMatched && attacmentMatched && bodyPartMatched && msgMtmTypeMatched && newFlagMatched;
			}			
		if(testPassed)
			{
			INFO_PRINTF1(_L("All the message parts are verified against their search criteria"));
			}
		else
			{
			SetTestStepResult(EFail);
			}
		}
	return TestStepResult();	
	}

/**
 Function : VerifyEmailHeaderFieldL
 Description : Verify the EMAIL message header contains expected search string.
 @param :	aEntry			Index entry of the message
 @param :	aSearchString	String to be searched for the presence
 @param :	aMsgPart		Header field to be looked into
 @return: 	TBool			Boolean flag to indicate search string found
 */
TBool CT_MsgVerifySearchResult::VerifyEmailHeaderFieldL(TMsvEntry& aEntry, TDesC& aSearchString, TMsvMessagePart aMsgPart, TBool aWildcardFlag)
	{
	TBool emailHeaderMatched = ETrue;
	TInt count = 0;
	CMsvEntry *entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(aEntry.Id());
	CMsvStore *store;
	store = entry->ReadStoreL();
	CleanupStack::PushL(store);
	CImHeader* header = CImHeader::NewLC(); 
	header->RestoreL(*store); // Retrieves the email message header
	// Check if the message header contains expected email address in TO field.emailHeaderMatched = EFalse;
	switch(aMsgPart)
		{
	case EMsvTo:
		emailHeaderMatched = EFalse;
		count = header->ToRecipients().Count();
		for(TInt ii = 0; ii < count; ++ii)
			{
			if((header->ToRecipients()[ii].FindF(aSearchString)) >= 0)
				{
				emailHeaderMatched = ETrue; // TO field has the expected recipient address
				break;
				}
			}
		break;	
	case EMsvFrom:
		if(header->From().CompareF(aSearchString) != 0)
			{
			emailHeaderMatched = EFalse; // From filed doesn't have the expected email address
			ERR_PRINTF3(_L("EMsvFrom***EMsvEqual test: Message enty with TMsvId \"%d\" does not have \"%S\" in FROM field"), aEntry.Id(), &aSearchString);
			}
		break;
	case EMsvCc:		
		emailHeaderMatched = EFalse;
		count = header->CcRecipients().Count();
		for(TInt ii = 0; ii < count; ++ii)
			{
			if((header->CcRecipients()[ii].FindF(aSearchString)) >= 0)
				{
				emailHeaderMatched = ETrue; // CC field has the expected recipient address
				break;
				}
			}
		break;	
	case EMsvBcc:
		emailHeaderMatched = EFalse;
		count = header->BccRecipients().Count();
		for(TInt ii = 0; ii< count; ++ii)
			{
			if((header->BccRecipients()[ii].FindF(aSearchString)) >= 0)
				{
				emailHeaderMatched = ETrue;
				break;
				}
			}
		break;
	case EMsvSubject:
		if(aWildcardFlag)
			{
			if(header->Subject().MatchF(aSearchString) == KErrNotFound)
				{
				ERR_PRINTF3(_L("EMsvSubject***EMsvEqual test: EMAIL Message with TMsvId \"%d\" does not have \"%S\" in SUBJECT field"), aEntry.Id(), &aSearchString);
				emailHeaderMatched = EFalse;
				}
			}
		else
			{
			if(header->Subject().FindF(aSearchString) == KErrNotFound)
				{
				ERR_PRINTF3(_L("EMsvSubject***EMsvEqual test: EMAIL Message with TMsvId \"%d\" does not have \"%S\" in SUBJECT field"), aEntry.Id(), &aSearchString);
				emailHeaderMatched = EFalse;
				}
			}
		break;	
	default:
		break;	
		}
	CleanupStack::PopAndDestroy(3, entry);
	return emailHeaderMatched;
	}
	
/**
 Function : VerifyEmailHeaderFieldL
 Description : Verify the SMS message header contains expected search string.
 @param :	aEntry			Index entry of the message
 @param :	aSearchString	String to be searched for the presence
 @param :	aMsgPart		Header field to be looked into
 @return: 	TBool			Boolean flag to indicate search string found
 */
TBool CT_MsgVerifySearchResult::VerifySmsHeaderFieldL(TMsvEntry& aEntry, TDesC& aSearchString, TMsvMessagePart aMsgPart)
	{
	TBool smsHeaderMatched = ETrue;
	TInt count = 0;
	CMsvEntry *entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	entry->SetEntryL(aEntry.Id());
	CMsvStore *store;
	store = entry->ReadStoreL();
	CleanupStack::PushL(store);
	CSmsHeader* header = CSmsHeader::NewL(CSmsMessage::NewL(iSharedDataCommon.iSession->FileSession(), CSmsPDU::ESmsSubmit, CSmsBuffer::NewL()));
	CleanupStack::PushL(header);
	header->RestoreL(*store); // Retrieves the SMS message header
	CArrayPtrFlat<CSmsNumber>& recipeints = header->Recipients();
	switch(aMsgPart)
		{
	case EMsvTo:
		smsHeaderMatched = EFalse;
		count = header->Recipients().Count();
		for(TInt ii = 0; ii < count; ++ii)
			{
			if((recipeints[ii]->Address().FindF(aSearchString)) >= 0)
				{
				smsHeaderMatched = ETrue;
				break;
				}
			}
		break;
	case EMsvFrom:
		if(header->FromAddress().CompareF(aSearchString) != 0)
			{
			smsHeaderMatched = EFalse; // From filed doesn't have the expected SMS address
			ERR_PRINTF3(_L("EMsvFrom***EMsvEqual test: SMS Message enty with TMsvId \"%d\" does not have \"%S\" in FROM field"), aEntry.Id(), &aSearchString);
			}
		break;
	default:
		break;
		}
	CleanupStack::PopAndDestroy(3, entry);
	return smsHeaderMatched;
	}

