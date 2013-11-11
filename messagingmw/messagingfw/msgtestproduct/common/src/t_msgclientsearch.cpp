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
#include "t_msgclientsearch.h"

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
  Description : Creates an object of CT_MsgClientSearch
  @param : aSharedDataCommon		A reference to CT_MsgSharedDataCommon
  @return : A pointer to an object of type CT_MsgClientSearch
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgClientSearch* CT_MsgClientSearch::NewL(CT_MsgSharedDataCommon& aSharedDataCommon)
	{
	CT_MsgClientSearch* self = new(ELeave) CT_MsgClientSearch(aSharedDataCommon);
	CleanupStack::PushL(self);
	self->ConstructL(); 
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CT_MsgClientSearch
  Description : Constructor
  @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
  @return : N/A
*/
CT_MsgClientSearch::CT_MsgClientSearch(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgAsyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KClientSearch);
	}

/**
  Function : ConstructL
  Description : Calls the ConstrutL of CT_MsgAsyncStepCommon and creates an object of 
				CT_MsgUtilsDeleteAllChildren to delete the entries of the message store
  @return : N/A
*/
void CT_MsgClientSearch::ConstructL()
	{
	CT_MsgAsyncStepCommon::ConstructL();
	}

/**
  Function : ~CT_MsgClientSearch
  Description : Destructor
  @return : N/A
*/
CT_MsgClientSearch::~CT_MsgClientSearch()
	{
	}



/**
  Function : doTestStepL
  Description : Perform search operation against the serching criteria on the entries 
  				under specified folder/service.
  @return : TVerdict - Test step result
*/
TVerdict CT_MsgClientSearch::doTestStepL()
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
		case ESearchForBodyPart:
		TestSearchForBodyPartL();
		break;
		default:
		INFO_PRINTF1(_L("Incorrect SearchSort Field Option specified in .ini file"));
		}
	INFO_PRINTF1(_L("Test Step: Search Entry Completed"));
	return TestStepResult();
}


void CT_MsgClientSearch::TestSearchForToFieldL()
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
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, emailAddress, tempEntry.Id(), KMsvMessagePartTo, /*TMsvFindResultAny(),*/ active->iStatus);
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

void CT_MsgClientSearch::TestSearchForCCFieldL()
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
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, emailAddress, tempEntry.Id(), KMsvMessagePartCc, /*TMsvFindResultAny(),*/ active->iStatus);
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

void CT_MsgClientSearch::TestSearchForBCCFieldL()
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
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, emailAddress, tempEntry.Id(), KMsvMessagePartBcc, /*TMsvFindResultAny(),*/ active->iStatus);
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

void CT_MsgClientSearch::TestSearchForSubjectFieldL()
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
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, subject, tempEntry.Id(), KMsvMessagePartSubject, /*TMsvFindResultAny(),*/ active->iStatus);
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

void CT_MsgClientSearch::TestSearchForDateFieldL()
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

void CT_MsgClientSearch::TestSearchForBodyPartL()
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
	find = CMsvFindOperation::FindInChildrenL(*iSharedDataCommon.iSession, _L("part to be added"), tempEntry.Id(), KMsvMessagePartBody, /*TMsvFindResultAny(),*/ active->iStatus);
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

/**
  Function : CancelStep
  Description : Used to cancel an asynchronous operation
  @return : none
*/
void CT_MsgClientSearch::CancelStep()
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
void CT_MsgClientSearch::ProgressL(TBool bFinal)
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
