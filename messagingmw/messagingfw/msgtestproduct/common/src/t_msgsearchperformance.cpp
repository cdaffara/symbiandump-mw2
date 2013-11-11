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
#include "t_msgsearchperformance.h"

/* Epoc includes */
#include <msvfind.h>
#include <msvids.h>
#include <miuthdr.h>
#include <t_utils.h>
#include <t_utilscentralrepository.h>
#include <t_utilsenumconverter.h>
#include <miutmsg.h>
#include <cimplainbodytext.h>
#include <hal.h>


/* Literals Used */
_LIT(KImapAccountName,"ImapAccountName");
_LIT(KFolderName,"FolderName");
_LIT(KSubject,"Subject");
_LIT(KEmailAddress,"EmailAddress");
_LIT(KRecipientCount,"RecipientCount");
_LIT(KSearchSortField,"SearchSortOption");
_LIT(KDate,"Date");

enum TSearchOption{ESearchForToAddress=1,ESearchForCCField,ESearchForBCCField,ESearchForSubjectField,ESearchForDateField,ESearchForTimeField,ESearchForBodyPart};
/**
  Function : NewL
  Description : Creates an object of CT_MsgSearchPerformance
  @param : aSharedDataCommon		A reference to CT_MsgSharedDataCommon
  @return : A pointer to an object of type CT_MsgSearchPerformance
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgSearchPerformance* CT_MsgSearchPerformance::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgSearchPerformance* self = new(ELeave) CT_MsgSearchPerformance(aSharedDataCommon);
	CleanupStack::PushL(self);
	self->ConstructL(); 
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CT_MsgSearchPerformance
  Description : Constructor
  @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
  @return : N/A
*/
CT_MsgSearchPerformance::CT_MsgSearchPerformance(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgAsyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KSearchPerformance);
	}

/**
  Function : ConstructL
  Description : Calls the ConstrutL of CT_MsgAsyncStepCommon and creates an object of 
				CT_MsgUtilsDeleteAllChildren to delete the entries of the message store
  @return : N/A
*/
void CT_MsgSearchPerformance::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL();
	}

/**
  Function : ~CT_MsgSearchPerformance
  Description : Destructor
  @return : N/A
*/
CT_MsgSearchPerformance::~CT_MsgSearchPerformance()
	{
	}



/**
  Function : doTestStepL
  Description : Perform search operation against the serching criteria on the entries 
  				under specified folder/service.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgSearchPerformance::doTestStepL()
{
	INFO_PRINTF1(_L("Test Step: Search Entry started"));
	TPtrC searchSortString;
	if(!GetStringFromConfig(ConfigSection(),KSearchSortField,searchSortString))
		{
		ERR_PRINTF1(_L("Search Sort Field is not specified"));
		SetTestStepResult(EFail);	
		}
	TInt searchSortOption = CT_MsgUtilsEnumConverter::ConvertDesToSearchSortOption(searchSortString);
	
	switch(searchSortOption)
		{
		case ESearchForToAddress:
		TestSearchPerformanceOnToFieldL();
		break;
		case ESearchForCCField:
		TestSearchPerformanceOnCCFieldL();
		break;
		case ESearchForBCCField:
		TestSearchPerformanceOnBCCFieldL();
		break;
		case ESearchForSubjectField:
		TestSearchPerformanceOnSubjectFieldL();
		break;
		case ESearchForDateField:
		TestSearchPerformanceOnDateFieldL();
		break;
		case ESearchForBodyPart:
		TestSearchPerformanceOnBodyPartL();
		break;
		default:
		INFO_PRINTF1(_L("Incorrect SearchSort Field Option specified in .ini file"));
		}
	INFO_PRINTF1(_L("Test Step: Search Entry Completed"));
	return TestStepResult();
}


void CT_MsgSearchPerformance::TestSearchPerformanceOnToFieldL()
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

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	TMsvId parentId = KMsvNullIndexEntryIdValue;
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	parentId = KMsvRootIndexEntryId;
	TMsvId serviceId;
	TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,imapAccountName, EFalse));
	if(error != KErrNone)
		{
		ERR_PRINTF1(_L("Find Entry for service failed"));	
		}
	entry->SetEntryL(serviceId);
	TMsvId folderId;
	TRAP(error, folderId = CT_MsgUtils::FindEntryByNameL(entry,folderName, EFalse));
	entry->SetEntryL(folderId);
	TUint32 startTest = User::NTickCount();
	CMsvFindOperation* find = NULL;
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, emailAddress, folderId, KMsvMessagePartTo, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);
}

void CT_MsgSearchPerformance::TestSearchPerformanceOnCCFieldL()
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

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	TMsvId parentId = KMsvNullIndexEntryIdValue;
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	parentId = KMsvRootIndexEntryId;
	TMsvId serviceId;
	TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,imapAccountName, EFalse));
	if(error != KErrNone)
		{
		ERR_PRINTF1(_L("Find Entry for service failed"));	
		}
	entry->SetEntryL(serviceId);
	TMsvId folderId;
	TRAP(error, folderId = CT_MsgUtils::FindEntryByNameL(entry,folderName, EFalse));
	entry->SetEntryL(folderId);
	TUint32 startTest = User::NTickCount();
	CMsvFindOperation* find = NULL;
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, emailAddress, folderId, KMsvMessagePartCc, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);
}

void CT_MsgSearchPerformance::TestSearchPerformanceOnBCCFieldL()
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

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	TMsvId parentId = KMsvNullIndexEntryIdValue;
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	parentId = KMsvRootIndexEntryId;
	TMsvId serviceId;
	TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,imapAccountName, EFalse));
	if(error != KErrNone)
		{
		ERR_PRINTF1(_L("Find Entry for service failed"));	
		}
	entry->SetEntryL(serviceId);
	TMsvId folderId;
	TRAP(error, folderId = CT_MsgUtils::FindEntryByNameL(entry,folderName, EFalse));
	entry->SetEntryL(folderId);
	TUint32 startTest = User::NTickCount();
	CMsvFindOperation* find = NULL;
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, emailAddress, folderId, KMsvMessagePartBcc, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);
}

void CT_MsgSearchPerformance::TestSearchPerformanceOnSubjectFieldL()
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
	TPtrC subject;
	if(!GetStringFromConfig( ConfigSection(), KSubject, subject))
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

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	TMsvId parentId = KMsvNullIndexEntryIdValue;
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	parentId = KMsvRootIndexEntryId;
	TMsvId serviceId;
	TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,imapAccountName, EFalse));
	if(error != KErrNone)
		{
		ERR_PRINTF1(_L("Find Entry for service failed"));	
		}
	entry->SetEntryL(serviceId);
	TMsvId folderId;
	TRAP(error, folderId = CT_MsgUtils::FindEntryByNameL(entry,folderName, EFalse));
	entry->SetEntryL(folderId);
	TUint32 startTest = User::NTickCount();
	CMsvFindOperation* find = NULL;
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, subject, folderId, KMsvMessagePartSubject, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);
}

void CT_MsgSearchPerformance::TestSearchPerformanceOnDateFieldL()
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
	TPtrC iniDate;
	if(!GetStringFromConfig(ConfigSection(),KDate,iniDate))
	{
		ERR_PRINTF1(_L("Date is not specified"));
		SetTestStepResult(EFail);				
	}


	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
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
	TUint32 startTest = User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(startTest));
	CMsvFindOperation* find = NULL;
	active->Start();
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, iniDate, tempEntry.Id(), KMsvMessagePartDate, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();
	//find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);
}

void CT_MsgSearchPerformance::TestSearchPerformanceOnBodyPartL()
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
	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	TMsvId parentId = KMsvNullIndexEntryIdValue;
	CMsvEntry* entry = CMsvEntry::NewL(*iSharedDataCommon.iSession,KMsvRootIndexEntryId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone, ETrue));
	CleanupStack::PushL(entry);
	parentId = KMsvRootIndexEntryId;
	TMsvId serviceId;
	TRAPD(error, serviceId = CT_MsgUtils::FindEntryByNameL(entry,imapAccountName, EFalse));
	if(error != KErrNone)
		{
		ERR_PRINTF1(_L("Find Entry for service failed"));	
		}
	entry->SetEntryL(serviceId);
	TMsvId folderId;
	TRAP(error, folderId = CT_MsgUtils::FindEntryByNameL(entry,folderName, EFalse));
	entry->SetEntryL(folderId);
	CMsvFindOperation* find = NULL;
	active->Start();
	//find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, subject, folderId, KMsvMessagePartSubject, /*TMsvFindResultAny(),*/ active->iStatus);
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, _L("part to be added"), folderId, KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
	CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);


/*	TPtrC	imapAccountName;
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

	CMsvOperationWait* active = CMsvOperationWait::NewLC();
	
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
	TUint32 startTest = User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(startTest));
	CMsvFindOperation* find = NULL;
	active->Start();*/
	//find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, _L("part to be added"), tempEntry.Id(), KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
	/*CleanupStack::PushL(find);
	CActiveScheduler::Start();

	// Test the progress
    TPckgBuf<TMsvFindOperationProgress> progress;
    progress.Copy(find->FinalProgress());
	TEST(progress().iError == KErrNone);
	TInt actualRecipientCount = find->GetFindResult().Count();

	TUint32 endTest =  User::NTickCount();
	//INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest));
	if(recipientCount != actualRecipientCount)
	{
		ERR_PRINTF1(_L("Count mismatch"));
		SetTestStepResult(EFail);
	}
	INFO_PRINTF2(_L("Time take to search To List is %d milliseconds"),(endTest-startTest));
	INFO_PRINTF3(_L("Recipient Count,  expected : %d, actual : %d"),recipientCount,actualRecipientCount);
	CleanupStack::PopAndDestroy(find);
	CleanupStack::PopAndDestroy(entry);
	CleanupStack::PopAndDestroy(active);*/
}

/**
  Function : CancelStep
  Description : Used to cancel an asynchronous operation
  @return : none
*/
void CT_MsgSearchPerformance::CancelStep()
	{
	//	Stub
	//  Cancel iDeleteAllChildren
	}
	
	
/**
  Function : ProgressL
  Description : Provides the progress information of the asynchronous operation
  @param :	bFinal	Used to display the status of the asynchronous operation
  @return : none
*/
void CT_MsgSearchPerformance::ProgressL(TBool bFinal)
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
