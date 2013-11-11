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
// SearchEntry
// [Paramaters]
// EntryType : Type of the entry(Folder/Service) under which message would be searched for.
// EntryName : Name of the entry(Folder/Service) under which message would be searched for.
// PartList : Part of the message to be searched for. Constants for part list are defined in MTMDEF.H
// SearchString : Parameter against which search operation need to be performed.
// List all message entries those satisfy the search criteria under a specific 
// folder/service.
// [APIs Used]
// CMsvFindOperation::FindInChildrenL
// 
//

/**
 @file  
*/



/* User Includes */
#include "t_msgsearchentry.h"

/* Epoc includes */
#include <msvfind.h>
#include <msvids.h>
#include <miuthdr.h>
#include <t_utils.h>
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <miutmsg.h>
#include <cimplainbodytext.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KFolderName,"FolderName");
_LIT(KSubject,"Subject");
_LIT(KEmailAddress,"EmailAddress");
_LIT(KRecipientCount,"RecipientCount");
_LIT(KSearchSortField,"SearchSortOption");
_LIT(KDate,"Date");
_LIT(KTime, "Time");

enum TSearchOption{ESearchForToAddress=1,ESearchForCCField,ESearchForBCCField,ESearchForSubjectField,ESearchForDateField,ESearchForTimeField,ESearchForBodyPart};
/**
  Function : NewL
  Description : Creates an object of CT_MsgSearchEntry
  @param : aSharedDataCommon		A reference to CT_MsgSharedDataCommon
  @return : A pointer to an object of type CT_MsgResetMessageStore
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgSearchEntry* CT_MsgSearchEntry::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgSearchEntry* self = new(ELeave) CT_MsgSearchEntry(aSharedDataCommon);
	CleanupStack::PushL(self);
	self->ConstructL(); 
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CT_MsgSearchEntry
  Description : Constructor
  @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
  @return : N/A
*/
CT_MsgSearchEntry::CT_MsgSearchEntry(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgAsyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KSearchEntry);
	}

/**
  Function : ConstructL
  Description : Calls the ConstrutL of CT_MsgAsyncStepCommon and creates an object of 
				CT_MsgUtilsDeleteAllChildren to delete the entries of the message store
  @return : N/A
*/
void CT_MsgSearchEntry::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL();
	}

/**
  Function : ~CT_MsgSearchEntry
  Description : Destructor
  @return : N/A
*/
CT_MsgSearchEntry::~CT_MsgSearchEntry()
	{
	}



/**
  Function : doTestStepL
  Description : Perform search operation against the serching criteria on the entries 
  				under specified folder/service.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgSearchEntry::doTestStepL()
{
	INFO_PRINTF1(_L("Test Step: Search Entry started"));
	TPtrC searchSortString;
	if(!GetStringFromConfig(ConfigSection(),KSearchSortField,searchSortString))
		{
		ERR_PRINTF1(_L("Search Sort Field is not specified"));
		SetTestStepResult(EFail);	
		return TestStepResult();
		}
	TInt searchSortOption = CT_MsgUtilsEnumConverter::ConvertDesToSearchSortOption(searchSortString);
	
	switch(searchSortOption)
		{
		case ESearchForToAddress:
		TestSearchForToFieldL();
		break;
		case ESearchForCCField:
		TestSearchForCCFieldL();
		break;
		case ESearchForBCCField:
		TestSearchForBCCFieldL();
		break;
		case ESearchForSubjectField:
		TestSearchForSubjectFieldL();
		break;
		case ESearchForDateField:
		TestSearchForDateFieldL();
		break;
		case ESearchForTimeField:
		TestSearchForTimeFieldL();
		break;
		case ESearchForBodyPart:
		TestSearchForBodyPartL();
		break;
		default:
		INFO_PRINTF1(_L("Incorrect SearchSort Field Option specified in .ini file"));
		}
	INFO_PRINTF1(_L("Test Step: Search Entry Completed"));
	return TestStepResult();
}


void CT_MsgSearchEntry::TestSearchForToFieldL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TPtrC emailAddress;
	if(!GetStringFromConfig( ConfigSection(), KEmailAddress, emailAddress))
		{
		ERR_PRINTF1(_L("Address name is not specified"));
		SetTestStepResult(EFail);		
		}

	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt actualRecipientCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			entry->SetEntryL(tempEntry.Id());
			CMsvStore* store = entry->ReadStoreL();
			CImHeader* header = CImHeader::NewLC();
			header->RestoreL(*store); // Retrieves the email message header part from the message store
			//TInt dataSize = header->DataSize();
			if(!FindRecipeints(header->ToRecipients(),emailAddress))
			{
				actualRecipientCount+=1;
			}
			CleanupStack::PopAndDestroy(header);
			delete store;
			}
		}
	if(recipientCount != actualRecipientCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(entry);	
}

void CT_MsgSearchEntry::TestSearchForCCFieldL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TPtrC emailAddress;
	if(!GetStringFromConfig( ConfigSection(), KEmailAddress, emailAddress))
		{
		ERR_PRINTF1(_L("Address is not specified"));
		SetTestStepResult(EFail);		
		}
	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt actualRecipientCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			entry->SetEntryL(tempEntry.Id());
			CMsvStore* store = entry->ReadStoreL();
			CImHeader* header = CImHeader::NewLC();
			header->RestoreL(*store); // Retrieves the email message header part from the message store
			//TInt dataSize = header->DataSize();
			if(!FindRecipeints(header->CcRecipients(), emailAddress))
				{
				actualRecipientCount+=1;
				}
			CleanupStack::PopAndDestroy(header);
			delete store;
			}
		}
	if(recipientCount != actualRecipientCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	
	CleanupStack::PopAndDestroy(entry);	
}

void CT_MsgSearchEntry::TestSearchForBCCFieldL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TPtrC emailAddress;
	if(!GetStringFromConfig( ConfigSection(), KEmailAddress, emailAddress))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt actualRecipientCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			entry->SetEntryL(tempEntry.Id());
			CMsvStore* store = entry->ReadStoreL();
			CImHeader* header = CImHeader::NewLC();
			header->RestoreL(*store); // Retrieves the email message header part from the message store
			//TInt dataSize = header->DataSize();
			if(!FindRecipeints(header->BccRecipients(), emailAddress))
			{
				actualRecipientCount+=1;
			}
			CleanupStack::PopAndDestroy(header);
			delete store;
			}
		}
	if(recipientCount != actualRecipientCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	
	CleanupStack::PopAndDestroy(entry);	
}

void CT_MsgSearchEntry::TestSearchForSubjectFieldL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC subject;
	if(!GetStringFromConfig( ConfigSection(), KSubject, subject))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt actualRecipientCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDescription.Compare(subject)==0)
				{
				actualRecipientCount+=1;
				}
			}
		}
	if(recipientCount != actualRecipientCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	
	CleanupStack::PopAndDestroy(entry);	
}

void CT_MsgSearchEntry::TestSearchForDateFieldL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TPtrC iniDate;
	if(!GetStringFromConfig(ConfigSection(),KDate,iniDate))
	{
		ERR_PRINTF1(_L("Date is not specified"));
		SetTestStepResult(EFail);				
	}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt actualRecipientCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			TBuf<128> dateString;
			tempEntry.iDate.FormatL(dateString,(_L("%D%M%Y%/0%1%/1%2%/2%3%/3")));
			//INFO_PRINTF2(_L("Date of the mail is %S"),&dateString);
			TBuf<128> compDate(iniDate);
			if(compDate == dateString)
				{
				actualRecipientCount += 1;
				}
			}
		}
	if(recipientCount != actualRecipientCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	
	CleanupStack::PopAndDestroy(entry);
}

void CT_MsgSearchEntry::TestSearchForTimeFieldL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TPtrC iniTime;
	if(!GetStringFromConfig(ConfigSection(),KTime,iniTime))
	{
		ERR_PRINTF1(_L("Date is not specified"));
		SetTestStepResult(EFail);				
	}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt actualRecipientCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			TBuf<128> dateString;
			tempEntry.iDate.FormatL(dateString,(_L("%J%:1%T%:2%S%:3%B")));
			//INFO_PRINTF2(_L("Date of the mail is %S"),&dateString);
			TBuf<128> compDate(iniTime);
			if(compDate == dateString)
				{
				actualRecipientCount += 1;
				}
			}
		}
	if(recipientCount != actualRecipientCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	
	CleanupStack::PopAndDestroy(entry);	
}

void CT_MsgSearchEntry::TestSearchForBodyPartL()
{
	TPtrC	imapAccountName;
	if(!GetStringFromConfig( ConfigSection(), KImapAccountName, imapAccountName))
		{
		ERR_PRINTF1(_L("Imap account name is not specified"));
		SetTestStepResult(EFail);
		}
	TPtrC folderName;
	if(!GetStringFromConfig( ConfigSection(), KFolderName, folderName))
		{
		ERR_PRINTF1(_L("Folder name is not specified"));
		SetTestStepResult(EFail);		
		}
	TInt recipientCount;
	if(!GetIntFromConfig( ConfigSection(), KRecipientCount, recipientCount))
		{
		ERR_PRINTF1(_L("Recipient Count is not specified"));
		SetTestStepResult(EFail);		
		}
	TUint32 startTest = User::NTickCount();
	TMsvId imapServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL((TDes&)imapAccountName);
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession, imapServiceId, TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	CleanupStack::PushL(entry);
	entry->SetEntryL(imapServiceId);
	CMsvEntrySelection*	children;
	children = entry->ChildrenL();
	TInt childrenCount = children->Count();
	TMsvId tempServiceId;
	TMsvEntry tempEntry;
	for (TInt child = 0; child < children->Count(); ++child)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(child), tempServiceId, tempEntry) == KErrNone)
			{
			if(tempEntry.iDetails.Compare(folderName)==0)
				{
				child=childrenCount;
				}
			}
		}
	entry->SetEntryL(tempEntry.Id());
	children = entry->ChildrenL();
	childrenCount = children->Count();
	TInt bodyCount = 0;
	for(int childCount=0;childCount<childrenCount;childCount++)
		{
		if (iSharedDataCommon.iSession->GetEntry(children->At(childCount), tempServiceId, tempEntry) == KErrNone)
			{
			entry->SetEntryL(tempEntry.Id());
			TInt chunkSize(tempEntry.iSize);
			CImEmailMessage *emailMessage = CImEmailMessage::NewLC(*entry);
			CImPlainBodyText* bodytext = emailMessage->OpenPlainBodyTextForReadL(CImEmailMessage::EThisMessageOnly, chunkSize);
			HBufC*  fileData =  HBufC::NewLC(chunkSize);
			TPtr readData = fileData->Des();
			bodytext->NextChunkL(readData);
			TInt position = -1;
			_LIT(KServerMessage, "part to be added");
			position = readData.Find(KServerMessage);
			if(position < 0)
				{
				INFO_PRINTF1(_L("Body not found"));
				}
			else
				{
				INFO_PRINTF2(_L("Body Found at position %d"),position);
				bodyCount+=1;
				readData.Delete(0, chunkSize);
				}	
			delete bodytext;
			CleanupStack::PopAndDestroy(fileData);
			CleanupStack::PopAndDestroy(emailMessage);
			}
		}
	if(recipientCount != bodyCount)
		{
		ERR_PRINTF1(_L("Recipient Count is not Correct"));
		SetTestStepResult(EFail);		
		}
	TUint32 endTest =  User::NTickCount();
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,bodyCount);
	
	CleanupStack::PopAndDestroy(entry);	
}

/**
  Function : ProgressL
  Description : Provides the progress information of the asynchronous operation
  @param :	bFinal	Used to display the status of the asynchronous operation
  @return : none
*/
void CT_MsgSearchEntry::ProgressL(TBool bFinal)
	{
	//	Stub
	if ( bFinal )
		{
		/*	Display final progress iDeleteAllChildren */
		}
	else
		{
		/*	Display current progress iDeleteAllChildren*/
		}
	}




/**
  Function : CancelStep
  Description : Used to cancel an asynchronous operation
  @return : none
*/
void CT_MsgSearchEntry::CancelStep()
	{
	//	Stub
	//  Cancel iDeleteAllChildren
	}


TBool CT_MsgSearchEntry::FindRecipeints(const CDesCArray& aRecipientArray,const TDesC16& aRecipientAddress)
	{
	TBool found = EFail;

	TPtrC rec;
	rec.Set(aRecipientAddress);
	TInt pos = -1;
	TInt recipientCount = aRecipientArray.Count();
	TInt comp = aRecipientArray.Find(rec,pos);
	if(pos < recipientCount && pos > -1)
		{
			found = EPass;
		}
		return found;
	}
