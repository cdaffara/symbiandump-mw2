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
// VerifySortResult
// [Paramaters]
// MessagePart
// SearchValue
// Verify if message entries returned by sort mechanism are in the intended order.
// [APIs Used]
// 
//

/* User Includes */
#include "t_verifysortresult.h"

/* Epoc includes */
#include <smut.h>
#include <txtrich.h>
#include <imcmutil.h>
#include <miutmsg.h>
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>
#include <t_utilsenumconverter.h>


// Literals used 
_LIT(KSortMsgPart, "SortMsgPart");
_LIT(KSortOrder, "SortOrder");
_LIT(KCountOfResult, "CountOfResult");
_LIT(KExpectedResultCount, "ExpectedResultCount");

/**
 Function : CT_MsgVerifySortResult
 Description : Constructor
 @param : aSharedDataCommon A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgVerifySortResult::CT_MsgVerifySortResult(CT_MsgSharedDataCommon& aSharedDataCommon) 
: CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KVerifySortResult);
	}


/**
 Function : doTestStepL
 Description : Verify if message part(s) of the message entries returned by sort mechanism are
			   in the intended order.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgVerifySortResult::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : VerifySortResult"));
	TPtrC fieldName;
	TBuf<1> buf;
	TBool sortedByTo = ETrue;
	TBool sortedByFrom = ETrue;
	TBool sortedByCc = ETrue;
	TBool sortedByBcc = ETrue;
	TBool sortedBySubject = ETrue;
	TBool sortedByUnRead = ETrue;
	TBool sortedByDetails = ETrue;
	TBool sortedByDescription = ETrue;
	TBool sortedByDate = ETrue;
	TBool sortedBySize = ETrue;
	TBool sortedByAttachment = ETrue;
	TBool sortedByPriority = ETrue;
	TBool sortedById = ETrue;
	TBool sortedByNewFlag = ETrue;

	TInt expectedSearchResultCount = 0;
	GetIntFromConfig(ConfigSection(), KExpectedResultCount, expectedSearchResultCount); // Read search result count

	TInt resultCount = 0;
	GetIntFromConfig(ConfigSection(), KCountOfResult, resultCount); // Read search result count

	if(expectedSearchResultCount > 0 && resultCount == 0)
		{
		ERR_PRINTF2(_L("Expected SORT Result Count > 0 and Actual SORT result count = %d"), resultCount);		
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
		DisplayMsgEntryList(messageEntries); // Log the message entry details
		if ( !GetStringFromConfig( ConfigSection(), KSortMsgPart, fieldName))
			{
			ERR_PRINTF1(_L("Message part to be verified for is not specified"));
			SetTestStepResult (EFail);
			}
		TMsvMessagePart msgPartValue = CT_MsgUtilsEnumConverter::ConvertDesToMessagePart(fieldName);		

		TPtrC sortOrder;
		GetStringFromConfig(ConfigSection(), KSortOrder, sortOrder);
		TMsvSortOrder sortOption = CT_MsgUtilsEnumConverter::ConvertDesToSortOrder(sortOrder);

		CMsvEntry *entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, KMsvRootIndexEntryId, TMsvSelectionOrdering());
		CleanupStack::PushL(entry);

		switch(msgPartValue)
			{
		case EMsvTo:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByTo; ++j)
					{
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					
					HBufC* address1=HBufC::NewL(50); 
					if(header1->ToRecipients().Count())
						{
						address1 = (header1->ToRecipients()[0]).AllocL();					
						}
	
					CleanupStack::PopAndDestroy(2,store1);
					
					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
					
					HBufC* address2=HBufC::NewL(50); 
					//  FIx when no fields are there
					if(header2->ToRecipients().Count())
						{
						 address2 = (header2->ToRecipients()[0]).AllocL();
						}
					CleanupStack::PopAndDestroy(2,store2);	

					if(address1->CompareF(*address2) > 0)// Checks if any messages is not in Ascending order   w.r.t to TO field
						{
						ERR_PRINTF1(_L("Sorting by TO: Not in Ascending order"));
						sortedByTo = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByTo; ++j)
					{
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					HBufC* address1=HBufC::NewL(50); 
					if(header1->ToRecipients().Count())
						{
						address1 = (header1->ToRecipients()[0]).AllocL();					
						}
					CleanupStack::PopAndDestroy(2,store1);
					
					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
					HBufC* address2=HBufC::NewL(50); 
					//  FIx when no fields are there
					if(header2->ToRecipients().Count())
						{
						 address2 = (header2->ToRecipients()[0]).AllocL();
						}
					CleanupStack::PopAndDestroy(2,store2);	

					if(address1->CompareF(*address2) < 0)// Checks if any messages is not in Descending order  w.r.t to TO field
						{
						ERR_PRINTF1(_L("Sorting by TO: Not in Descending order"));
						sortedByTo = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
				}
			break;
		case EMsvFrom:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByFrom; ++j)
					{
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					HBufC* address1 = header1->From().AllocL();
					CleanupStack::PopAndDestroy(2,store1);

					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
					HBufC* address2 = header2->From().AllocL();
					CleanupStack::PopAndDestroy(2,store2);	

					if(address1->CompareF(*address2) > 0)// Checks if any messages is not in Ascending order w.r.t to FROM field
						{
						ERR_PRINTF1(_L("Sorting by From: Not in Ascending order"));
						sortedByFrom = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByFrom; ++j)
					{
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					HBufC* address1 = header1->From().AllocL();
					CleanupStack::PopAndDestroy(2,store1);
					
					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
					HBufC* address2 = header2->From().AllocL();
					CleanupStack::PopAndDestroy(2,store2);	

					if(address1->CompareF(*address2) < 0)// Checks if any messages is not in Descending order w.r.t to FROM field
						{
						ERR_PRINTF1(_L("Sorting by From: Not in Descending order"));
						sortedByFrom = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
				}
			break;
		case EMsvCc:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByCc; ++j)
					{
					HBufC* address1 = HBufC::NewL(50); 
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					
					if(header1->CcRecipients().Count())
						{
						address1 = (header1->CcRecipients()[0]).AllocL();					
						}
					CleanupStack::PopAndDestroy(2,store1);
					
					HBufC* address2 = HBufC::NewL(50); 
					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
				
					if(header2->CcRecipients().Count())
						{
						address2 = (header2->CcRecipients()[0]).AllocL();					
						}
					CleanupStack::PopAndDestroy(2,store2);
				
					if(address1->CompareF(*address2) > 0)// Checks if any messages is not in Ascending order w.r.t to CC field
						{
						ERR_PRINTF1(_L("Sorting by CC: Not in Ascending order"));
						sortedByCc = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByCc; ++j)
					{
					HBufC* address1 = HBufC::NewL(50); 
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					
					if(header1->CcRecipients().Count())
						{
						address1 = (header1->CcRecipients()[0]).AllocL();					
						}
					CleanupStack::PopAndDestroy(2,store1);
					
					
					HBufC* address2 = HBufC::NewL(50); 
					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
				
					if(header2->CcRecipients().Count())
						{
						address2 = (header2->CcRecipients()[0]).AllocL();					
						}
					CleanupStack::PopAndDestroy(2,store2);
				
									
					if(address1->CompareF(*address2) < 0)// Checks if any messages is not in Descending order w.r.t to CC field
						{
						ERR_PRINTF1(_L("Sorting by CC: Not in Descending order"));
						sortedByCc = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
				}
			break;
		case EMsvBcc:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByBcc; ++j)
					{
					HBufC* address1 = HBufC::NewL(50); 
					if(messageEntries[j].iMtm != KUidMsgTypeSMS)
						{
						entry->SetEntryL(messageEntries[j].Id());
						CMsvStore* store1 = entry->ReadStoreL();
						CleanupStack::PushL(store1);
						CImHeader* header1 = CImHeader::NewLC(); 
						header1->RestoreL(*store1); // Retrieves the email message header
						if(header1->BccRecipients().Count())
							{
							address1 = (header1->BccRecipients()[0]).AllocL();					
							}
						CleanupStack::PopAndDestroy(2,store1);
						}
					else
						{
						buf.Copy(_L(""));
						address1 = buf.AllocL();
						}
					
					HBufC* address2 = HBufC::NewL(50); 
					if(messageEntries[j+1].iMtm != KUidMsgTypeSMS)
						{
						entry->SetEntryL(messageEntries[j+1].Id());
						CMsvStore* store2 = entry->ReadStoreL();
						CleanupStack::PushL(store2);
						CImHeader* header2 = CImHeader::NewLC(); 
						header2->RestoreL(*store2); // Retrieves the email message header
						if(header2->BccRecipients().Count())
							{
							address2 = (header2->BccRecipients()[0]).AllocL();					
							}
						CleanupStack::PopAndDestroy(2,store2);
						}
					else
						{
						buf.Copy(_L(""));
						address2 = buf.AllocL();
						}

					if(address1->CompareF(*address2) > 0)// Checks if any messages is not in Ascending order w.r.t to BCC field
						{
						ERR_PRINTF1(_L("Sorting by BCC: Not in Ascending order"));
						sortedByBcc = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
				
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByBcc; ++j)
					{
					HBufC* address1 = HBufC::NewL(50); 
					if(messageEntries[j].iMtm != KUidMsgTypeSMS)
						{
						entry->SetEntryL(messageEntries[j].Id());
						CMsvStore* store1 = entry->ReadStoreL();
						CleanupStack::PushL(store1);
						CImHeader* header1 = CImHeader::NewLC(); 
						header1->RestoreL(*store1); // Retrieves the email message header
						if(header1->BccRecipients().Count())
							{
							address1 = (header1->BccRecipients()[0]).AllocL();					
							}
						CleanupStack::PopAndDestroy(2,store1);
						}
					else
						{
						buf.Copy(_L(""));
						address1 = buf.AllocL();
						}
					HBufC* address2 = HBufC::NewL(50); 
					if(messageEntries[j].iMtm != KUidMsgTypeSMS)
						{
						entry->SetEntryL(messageEntries[j+1].Id());
						CMsvStore* store2 = entry->ReadStoreL();
						CleanupStack::PushL(store2);
						CImHeader* header2 = CImHeader::NewLC(); 
						header2->RestoreL(*store2); // Retrieves the email message header
						if(header2->BccRecipients().Count())
							{
							address2 = (header2->BccRecipients()[0]).AllocL();					
							}
						CleanupStack::PopAndDestroy(2,store2);
						}
					else
						{
						buf.Copy(_L(""));
						address2 = buf.AllocL();
						}

					if(address1->CompareF(*address2) < 0)// Checks if any messages is not in Descending order w.r.t to BCC field
						{
						ERR_PRINTF1(_L("Sorting by BCC: Not in Descending order"));
						sortedByBcc = EFalse;
						SetTestStepResult(EFail);
						}
					delete address2;
					delete address1;	
					}
				break;
				}
			break;
		case EMsvSubject:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedBySubject; ++j)
					{
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					HBufC* subject1 = header1->Subject().AllocL();
					CleanupStack::PopAndDestroy(2,store1);

					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header
					HBufC* subject2 = header2->Subject().AllocL();
					CleanupStack::PopAndDestroy(2,store2);

					if(subject1->CompareF(*subject2) > 0)// Checks if any messages is not in Ascending order w.r.t to SUBJECT field
						{
						ERR_PRINTF1(_L("Sorting by Subject: Not in Ascending order"));
						sortedBySubject = EFalse;
						SetTestStepResult(EFail);
						}
					delete subject2;
					delete subject1;	
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedBySubject; ++j)
					{
					entry->SetEntryL(messageEntries[j].Id());
					CMsvStore* store1 = entry->ReadStoreL();
					CleanupStack::PushL(store1);
					CImHeader* header1 = CImHeader::NewLC(); 
					header1->RestoreL(*store1); // Retrieves the email message header
					
					entry->SetEntryL(messageEntries[j+1].Id());
					CMsvStore* store2 = entry->ReadStoreL();
					CleanupStack::PushL(store2);
					CImHeader* header2 = CImHeader::NewLC(); 
					header2->RestoreL(*store2); // Retrieves the email message header

					if(header1->Subject().CompareF(header2->Subject()) < 0)// Checks if any messages is not in Descending order w.r.t to SUBJECT field
						{
						ERR_PRINTF1(_L("Sorting by Subject: Not in Descending order"));
						sortedBySubject = EFalse;
						SetTestStepResult(EFail);
						}
					CleanupStack::PopAndDestroy(4, store1);	
					}
				break;
				}
			break;
		case EMsvUnreadMessages:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByUnRead; ++j)
					{
					if(messageEntries[j].Unread() > messageEntries[j+1].Unread()) // Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by UnReadFlag: Not in Ascending order"));
						sortedByUnRead = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByUnRead; ++j)
					{
					if(messageEntries[j].Unread() < messageEntries[j+1].Unread()) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by UnReadFlag: Not in Descending order"));
						sortedByUnRead = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvDetails:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByDetails; ++j)
					{
					if(messageEntries[j].iDetails.CompareF(messageEntries[j+1].iDetails) > 0)// Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by Details: Not in Ascending order"));
						sortedByDetails = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByDetails; ++j)
					{
					if(messageEntries[j].iDetails.CompareF(messageEntries[j+1].iDetails) < 0) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by Details: Not in Descending order"));
						sortedByDetails = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvDescription:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByDescription; ++j)
					{
					if(messageEntries[j].iDescription.CompareF(messageEntries[j+1].iDescription) > 0)// Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by Description: Not in Ascending order"));
						sortedByDescription = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByDescription; ++j)
					{
					if(messageEntries[j].iDescription.CompareF(messageEntries[j+1].iDescription) < 0) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by Description: Not in Descending order"));
						sortedByDescription = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvDate:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByDate; ++j)
					{
					if(messageEntries[j].iDate > messageEntries[j+1].iDate) // Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by Date: Not in Ascending order"));
						sortedByDate = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByDate; ++j)
					{
					if(messageEntries[j].iDate < messageEntries[j+1].iDate) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by Date: Not in Descending order"));
						sortedByDate = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvSize:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedBySize; ++j)
					{
					if(messageEntries[j].iSize > messageEntries[j+1].iSize) // Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by Size: Not in Ascending order"));
						sortedBySize = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedBySize; ++j)
					{
					if(messageEntries[j].iSize < messageEntries[j+1].iSize) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by Size: Not in Descending order"));
						sortedBySize = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvAttachment:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByAttachment; ++j)
					{
					if(messageEntries[j].Attachment() > messageEntries[j+1].Attachment()) // Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by AttachmentFlag: Not in Ascending order"));
						sortedByAttachment = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByAttachment; ++j)
					{
					if(messageEntries[j].Attachment() < messageEntries[j+1].Attachment()) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by AttachmentFlag: Not in Descending order"));
						sortedByAttachment = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvNew:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByNewFlag; ++j)
					{
					if(messageEntries[j].New() < messageEntries[j+1].New()) // Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by NewFlag: Not in Ascending order"));
						sortedByNewFlag = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByNewFlag; ++j)
					{
					if(messageEntries[j].New() > messageEntries[j+1].New()) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by NewFlag: Not in Descending order"));
						sortedByNewFlag = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvBody:
			SetTestStepError(KErrNotSupported);
			break;
		case EMsvPriority:
			switch(sortOption)
				{
			case EMsvSortAscending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByPriority; ++j)
					{
					if(messageEntries[j].Priority() < messageEntries[j+1].Priority()) // Checks if any messages is not in Ascending order
						{
						ERR_PRINTF1(_L("Sorting by AttachmentFlag: Not in Ascending order"));
						sortedByPriority = EFalse;
						SetTestStepResult(EFail);
						}
					}
				break;
			case EMsvSortDescending:
				for(TInt j = 0; j < messageEntries.Count() - 1 && sortedByPriority; ++j)
					{
					if(messageEntries[j].Priority() > messageEntries[j+1].Priority()) // Checks if any messages is not in Descending order
						{
						ERR_PRINTF1(_L("Sorting by AttachmentFlag: Not in Descending order"));
						sortedByPriority = EFalse;
						SetTestStepResult(EFail);
						}
					}
				}
			break;
		case EMsvMtmTypeUID:
			break;
			}
		CleanupStack::PopAndDestroy(1, entry);
		}
	return TestStepResult();	
	}


/**
 Function : DisplayMsgEntryList
 Description : Prints message entry details into the log file
 @param aMessageEntries Array to hold index entries corresponding to the Search/Sort result.
 @return : TVerdict - Test step result
 */
void CT_MsgVerifySortResult::DisplayMsgEntryList(const RArray<TMsvEntry>& aMessageEntries)
	{
	for(TInt j = 0; j < aMessageEntries.Count(); ++j)
		{
		INFO_PRINTF4(_L("Messagge Entry ID = %d , ServiceID = %d, Size = %d"), aMessageEntries[j].Id(), aMessageEntries[j].iServiceId, aMessageEntries[j].iSize);
		}
	}

