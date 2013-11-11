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
// @file 
// [TestStep Name]
// UnitTestCMsvEntry
// [Paramaters]
// ImapAccountName		: Name of the Imap account to be created
// SettingsFileName	: Config file from which the Imap account settings are read 
// Invokes the API of CMsvEntry class to improve the code coverage.
// [APIs Used]
// CMsvEntry::XXXXXXXX 
// 
//



/* User include*/
#include "t_cmsventry.h"
#include <t_utilsreademailsettingsfromconfigfile.h>



/* epoc inlcudes */
#include <cemailaccounts.h>
#include <iapprefs.h>
#include <imapset.h>
#include <smtpset.h>
#include <msvfind.h>


/* Literals used */
_LIT(KShortDescription ,"Description");
_LIT(KShortDetails, "Details");
_LIT(KDataComponentFilename, "c:\\system\\mtm\\testsmtm.dat");

const TUid KUidTestServerMtmType={0x10001304};
/**
  Function : CT_MsgCreateImapAccount
  Description : Constructor
  @return : N/A
*/
CT_CMsvEntry::CT_CMsvEntry(CT_MsgSharedDataCommon& aSharedDataCommon)
:CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName(KUnitTestCMsvEntry);
	}

void CT_CMsvEntry::CheckNumberOfEntriesL(CMsvEntry &aEntry,TMsvId aId,TInt aRequiredNo)
	{
	if(aEntry.EntryId()==aId)
		{
		TEST(aEntry.Count()==aRequiredNo);
		CMsvOperationWait* active = CMsvOperationWait::NewLC();
		if(aRequiredNo==0)
			{
			TEST(aEntry.Entry().Owner()==EFalse);
			}
		else
			{
			TEST(aEntry.Entry().Owner()!=EFalse);
			}
		active->Start();
		User::After(100000);
		CActiveScheduler::Start();
		TEST(aEntry.Count()==aRequiredNo);
		if(aRequiredNo==0)
			{
			TEST(aEntry.Entry().Owner()==EFalse);
			}
		else
			{
			TEST(aEntry.Entry().Owner()!=EFalse);
			}
		aEntry.SetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PopAndDestroy(active);
		}
	aEntry.SetEntryL(aId);
	TEST(aEntry.Count()==aRequiredNo);
	if(aRequiredNo==0)
		{
		TEST(aEntry.Entry().Owner()==EFalse);
		}
	else
		{
		TEST(aEntry.Entry().Owner()!=EFalse);
		}
	}

void CT_CMsvEntry::TestCopySyncL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: Cop -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));
	
	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, order);
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);

	cEntry->SetSortTypeL(TMsvSelectionOrdering());
	
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	cEntry->CopyL(entry.Id(),folder2.Id());

	CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	
	CleanupStack::PopAndDestroy(cEntry); // ob, session, cEntry
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with session priority and fileserver to the server -- ended"));

	}

void CT_CMsvEntry::TestCopyAsyncL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());
	CMsvEntrySelection* selection;
	selection = cEntry->ChildrenL();
	selection->AppendL(folder1.Id());
	CleanupStack::PushL(selection);
	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	TMsvLocalOperationProgress progress = TMsvLocalOperationProgress();
	TRAP (error, cEntry->CopyL(*selection,folder2.Id(),progress));
	TEST (error == KErrNone);
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry); // ob, session, cEntry
	//CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with session priority and fileserver to the server -- ended"));	
	}

void CT_CMsvEntry::TestMoveSyncL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of synchronous session to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));


	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());

	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	cEntry->MoveL(entry.Id(),folder2.Id());

	CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),0);
	
	//cEntry->MoveL(entry.Id(), folder1.Id());

	CleanupStack::PopAndDestroy(cEntry); // ob, session, cEntry
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of synchronous session with session priority and fileserver to the server -- ended"));

	}

void CT_CMsvEntry::TestMoveAsyncL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session to the server -- started"));
	TRAP(error, iSession = CMsvSession::OpenSyncL(*this));

	CMsvEntry* cEntry = CMsvEntry::NewL(*iSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering());
	CleanupStack::PushL(cEntry);

	// add two folder to work under
	TMsvEntry folder1;
	folder1.iDescription.Set(KShortDescription);
	folder1.iDetails.Set(KShortDetails);
	folder1.iType = KUidMsvFolderEntry;
	folder1.iMtm = KUidMsvLocalServiceMtm;
	folder1.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder1);

	TMsvEntry folder2;
	folder2.iDescription.Set(KShortDescription);
	folder2.iDetails.Set(KShortDetails);
	folder2.iType = KUidMsvFolderEntry;
	folder2.iMtm = KUidMsvLocalServiceMtm;
	folder2.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(folder2);

	cEntry->SetEntryL(folder1.Id());
	CMsvEntrySelection* selection;
	selection = cEntry->ChildrenL();
	selection->AppendL(folder1.Id());
	CleanupStack::PushL(selection);
	// add entry
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),0);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	TMsvLocalOperationProgress progress = TMsvLocalOperationProgress();
	cEntry->MoveL(*selection,folder2.Id(),progress);
	CheckNumberOfEntriesL(*cEntry,folder2.Id(),1);
	CheckNumberOfEntriesL(*cEntry,folder1.Id(),1);
	
	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(cEntry); // ob, session, cEntry
	//CleanupStack::PopAndDestroy(active);
	delete iSession;
	INFO_PRINTF1(_L("Testing: creation of asynchronous session with session priority and fileserver to the server -- ended"));	
	}

void CT_CMsvEntry::HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*)
	{
	INFO_PRINTF1(_L("Entered Handlesession EventL"));
	switch(aEvent)
		{
			case EMsvEntriesCreated:
			INFO_PRINTF1(_L("Entries created"));
			break;
			case EMsvEntriesChanged:
			INFO_PRINTF1(_L("Entries Changed"));
			break;
			case EMsvEntriesDeleted:
			INFO_PRINTF1(_L("Entried Deleted"));
			break;
			case EMsvEntriesMoved:
			INFO_PRINTF1(_L("Entried Moved"));
			break;
			case EMsvMtmGroupInstalled:
			INFO_PRINTF1(_L("Mtm Group Installed"));
			break;
			case EMsvMtmGroupDeInstalled:
			INFO_PRINTF1(_L("Mtm Group De-installed"));
			break;
			case EMsvGeneralError:
			INFO_PRINTF1(_L("General Error occured"));
			break;
			case EMsvCloseSession:
			INFO_PRINTF1(_L("Session Closed"));
			break;
			case EMsvServerReady:
			INFO_PRINTF1(_L("Server Ready"));
			break;
			case EMsvServerFailedToStart:
			INFO_PRINTF1(_L("Failed to start"));
			break;
			case EMsvCorruptedIndexRebuilt:
			INFO_PRINTF1(_L("Corrupted Intex rebuilt"));
			break;
			case EMsvServerTerminated:
			INFO_PRINTF1(_L("Server terminated"));
			break;
			case EMsvMediaChanged:
			INFO_PRINTF1(_L("Media Changed"));
			break;
			EMsvMediaUnavailable:
			INFO_PRINTF1(_L("Media Unavailable"));
			break;
			case EMsvMediaAvailable:
			INFO_PRINTF1(_L("Media Available"));
			break;
			case EMsvMediaIncorrect:
			INFO_PRINTF1(_L("Media Incorrect"));
			break;
			case EMsvCorruptedIndexRebuilding:
			INFO_PRINTF1(_L("Corrupted Index Rebuilding"));
			break;
		}
	INFO_PRINTF1(_L("Exited Handlesession EventL"));
	}
	
/**
  Function : doTestStepL
  Description : Reads the Imap ccount name and the settings file name from the ini file.
				Creates an Imap account by reading the settings from the given settings file
  @return : TVerdict - Test step result
*/
TVerdict CT_CMsvEntry::doTestStepL()
	{
	TInt error = KErrGeneral;
	INFO_PRINTF1(_L("Test Step: CMsvSession Unit Test Start"));
	TRAP(error, TestCopySyncL());
	TRAP(error, TestCopyAsyncL());
	TRAP(error, TestMoveSyncL());
	TRAP(error, TestMoveAsyncL());

	INFO_PRINTF1(_L("Test Step: CMsvSession Unit Test Completed"));
	return TestStepResult();
	}
