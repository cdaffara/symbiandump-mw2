// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "Testsetup.h"
#include "CAsyncWaiter.h"
#include <mmsvattachmentmanager.h>
#include <mmsvattachmentmanagersync.h>
#include <cmsvattachment.h>

_LIT(KShortDescription, "Description");
_LIT(KShortDetails, "Details");

// Globals
RTest test(_L("Test Harness for Rename Attachment API"));
CMsvSession* theSession;
RFs fs;

//

LOCAL_C CMsvEntry* CreateTestEntryLC()
	{
	// Create an Entry 
	CMsvEntry* cEntry = CMsvEntry::NewL(*theSession, KMsvGlobalInBoxIndexEntryId, TMsvSelectionOrdering()); 
	CleanupStack::PushL(cEntry);
	
	TMsvEntry entry;
	entry.iDescription.Set(KShortDescription);
	entry.iDetails.Set(KShortDetails);
	entry.iType = KUidMsvMessageEntry;
	entry.iMtm = KUidMsvLocalServiceMtm;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	cEntry->CreateL(entry);
	cEntry->SetEntryL(entry.Id());
	
	return cEntry;
	}

LOCAL_C void TestRenameAttachmentSyncL()
	{
	CMsvEntry* cEntry = CreateTestEntryLC();
	
	// Open entry file
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
    //Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	TFileName filename;
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	
	RFile file;
	store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment);
	CleanupStack::Pop(attachment); //pass ownership
	attachment = NULL;
	file.Close();
	
	//Commit the Store so the attachment gets stored 
	store->CommitL();
	
	//store the attachment name
	TFileName oldfilepath;
	oldfilepath = filename;
			
	filename.Append(_L("new"));
	store->AttachmentManagerExtensionsL().RenameAttachmentL(0,filename);

	//Commit the Store so the new name gets stored 
	store->CommitL();
	
	// Check that rename has occured
	// and print the new name on the log
	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
	CleanupStack::PushL(attachment);
	
	_LIT(KIntro, "Testing the new name of attachment with sync");
	theUtils->Printf(KIntro);
	
	// Retrieve the filepath and check that the name
	// is same as filename
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(filename)==0);
	
	//Print out the name on the log
	_LIT(KName,"%S");
	theUtils->Printf(KName, &filename);
	
	
	//Test the attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, oldfilepath, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));
	
	CleanupStack::PopAndDestroy(3, cEntry); // attachment, store, cEntry	
	}
	
LOCAL_C void TestRenameSyncTwiceL()
	{
	
	CMsvEntry* cEntry = CreateTestEntryLC();	
	
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);

	//Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
		
	RFile file;
	TFileName filename;
	
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment);
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	//Commit the Store so the attachment gets stored 
	store->CommitL();
		
	//store the attachment name
	TFileName oldfilepath;
	oldfilepath = filename;	
		
	//Test the Rename function
	filename.Append(_L("new"));
	store->AttachmentManagerExtensionsL().RenameAttachmentL(0,filename);
	//Rename again
	filename.Append(_L("name"));
	store->AttachmentManagerExtensionsL().RenameAttachmentL(0,filename);
		 
	//Commit the Store so the new name gets stored 
	store->CommitL();
	
	// Check that rename has occured
	// and print the new name on the log
	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
	CleanupStack::PushL(attachment);
	
	_LIT(intro, "Testing the new name of attachment with sync");
	theUtils->Printf(intro);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(filename)==0);
	
	//Test the attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, oldfilepath, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));

	CleanupStack::PopAndDestroy(3,cEntry); 	
	}


LOCAL_C void TestRenameSyncWithExistNameL()
	{

	CMsvEntry* cEntry = CreateTestEntryLC();
		
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	// Create the first attachment
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	RFile file;
	TFileName filename;
	
	_LIT(KTxtFirstFile, "file1");
	filename.Append(KTxtFirstFile);
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment);
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	// Create the second attachment
	attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
		
	filename.Append(_L("new"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment);
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	// Commit the Store so the attachment gets stored 
	store->CommitL();
	
	// Store the attchment name
	TFileName oldfilepath;
	oldfilepath = filename;
	
	// Get the first attachment	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
 	CleanupStack::PushL(attachment);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(KTxtFirstFile)==0);
	
	// Rename to the second name : this should leave with KErrAlreadyExists
	TRAPD(err, store->AttachmentManagerExtensionsL().RenameAttachmentL(0, filename));
	test(err==KErrAlreadyExists);
		
	//Test the attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, oldfilepath, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));
	
	CleanupStack::PopAndDestroy(3,cEntry); 	
	}


LOCAL_C void TestRenameSyncAndRevertL()
	{
	CMsvEntry* cEntry = CreateTestEntryLC();	
	
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	//Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);

	RFile file;
	TFileName filename, oldfilename;

	filename.Append(_L("file1"));
	oldfilename=filename;
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment);
	CleanupStack::Pop(attachment); //pass ownership
	attachment = NULL;
	
	file.Close();
	
	store->CommitL();
	
	//Test the Rename function
	filename.Append(_L("new"));
	store->AttachmentManagerExtensionsL().RenameAttachmentL(0,filename);
	  	
   	//Revert the Store
  	store->RevertL();
	
	// Check that rename has not occured

	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
 	CleanupStack::PushL(attachment);

	_LIT(intro, "Testing the old name of attachment with sync");
	theUtils->Printf(intro);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(oldfilename)==0);
	
	//check that attachment names are correct after rename revert
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, filename, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));

	CleanupStack::PopAndDestroy(3,cEntry); 	
	}


LOCAL_C void TestRenameAttachmentL()
	{

	CMsvEntry* cEntry = CreateTestEntryLC();	
	
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	//Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	RFile file;
	TFileName filename;
	
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	//Commit the Store so the attachment gets stored 
	store->CommitL();
	
	TFileName oldfilepath;
	oldfilepath = filename;
	
	//Test the Rename function
	filename.Append(_L("new"));
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	store->AttachmentManagerL().RenameAttachmentL(0,filename,waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	 
	//Commit the Store so the new name gets stored 
	store->CommitL();
	
	
	// Check that rename has occured
	// and print the new name on the log
	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
 	CleanupStack::PushL(attachment);
	
	_LIT(intro, "Testing the new name of attachment");
	_LIT(KName,"%S");

	theUtils->Printf(intro);
	
	// Retrieve the filepath and check that the name
	// is same as filename
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(filename)==0);
	
	//Print out the name on the log
	theUtils->Printf(KName,&filename);
	
	CleanupStack::PopAndDestroy(2, waiter); 
	waiter = NULL;
	
	//check that attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, oldfilepath, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));
		
	CleanupStack::PopAndDestroy(2,cEntry); 	
	}


LOCAL_C void TestRenameWithExistNameL()
	{

	CMsvEntry* cEntry = CreateTestEntryLC();
		
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	//Create an attachment
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	RFile file;
	TFileName filename;

	_LIT(KTxtFirstFile, "file1");
	filename.Append(KTxtFirstFile);
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	//Create a 2nd attachment
	attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);


	filename.Append(_L("new"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	CleanupStack::Pop(attachment); //pass ownership
	attachment = NULL;
	file.Close();
	
	//Commit the Store so the attachment gets stored 
	store->CommitL();
	
	TFileName oldfilepath;
	oldfilepath = filename;
	
	// Get the first attachment	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
 	CleanupStack::PushL(attachment);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(KTxtFirstFile)==0);
	
	// Rename to the second name : this should leave with KErrAlreadyExists
	TRAPD(err, store->AttachmentManagerL().RenameAttachmentL(0,filename,waiter->iStatus));
	test(err==KErrAlreadyExists);

	CleanupStack::PopAndDestroy(2, waiter); // waiter, store
	waiter = NULL;
		
		
	//check that attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, oldfilepath, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));
		
	CleanupStack::PopAndDestroy(2,cEntry); 	
	}


LOCAL_C void TestRenameAndRevertL()
	{
	CMsvEntry* cEntry = CreateTestEntryLC();
		
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	//Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	RFile file;
	TFileName filename, oldfilename;

	filename.Append(_L("file1"));
	oldfilename=filename;
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); //pass ownership
	attachment = NULL;
	
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	file.Close();
	
	store->CommitL();
	
	//Test the Rename function
	filename.Append(_L("new"));
	store->AttachmentManagerL().RenameAttachmentL(0,filename,waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
  	
   	//Revert the Store
  	store->RevertL();
	
	// Check that rename has not occured

	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
 	CleanupStack::PushL(attachment);

	_LIT(intro, "Testing the old name of attachment");
	theUtils->Printf(intro);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(oldfilename)==0);

	CleanupStack::PopAndDestroy(3, store); // attachment, waiter, store
	waiter = NULL;
		
	//test the attachment name is correct after rename revert	
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, filename, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));
		
	CleanupStack::PopAndDestroy();//cEntry 	
	}


LOCAL_C void TestRenameTwiceL()
	{
	
	CMsvEntry* cEntry = CreateTestEntryLC();	
	
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
	CAsyncWaiter* waiter = CAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	
	//Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);

	RFile file;
	TFileName filename;
	
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerL().CreateAttachmentL(filename, file, attachment, waiter->iStatus);
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	//Commit the Store so the attachment gets stored 
	store->CommitL();
	
	//store the name of attachment
	TFileName oldfilepath;
	oldfilepath = filename;
	
	//Test the Rename function
	filename.Append(_L("new"));
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	store->AttachmentManagerL().RenameAttachmentL(0,filename,waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
	
	//Rename again
	filename.Append(_L("name"));
	store->AttachmentManagerL().RenameAttachmentL(0,filename,waiter->iStatus);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());
		 
	//Commit the Store so the new name gets stored 
	store->CommitL();
	
	// Check that rename has occured
	// and print the new name on the log
	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
 	CleanupStack::PushL(attachment);
	
	_LIT(intro, "Testing the new name of attachment");
	_LIT(KName,"%S");

	theUtils->Printf(intro);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(filename)==0);

	//Print out the name on the log
	theUtils->Printf(KName,&attachmentName);
	
	CleanupStack::PopAndDestroy(2, waiter); // attachment,waiter
	waiter = NULL;
	
	//check that attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, oldfilepath, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));
		
	CleanupStack::PopAndDestroy(2,cEntry); 	
	}

LOCAL_C void TestRenameSyncTwiceAndRevertL()
	{
	
	CMsvEntry* cEntry = CreateTestEntryLC();	
	
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);

	//Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
		
	RFile file;
	TFileName filename;
	
	filename.Append(_L("file1"));
	attachment->SetAttachmentNameL(filename);
	store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment);
	CleanupStack::Pop(attachment); //pass ownership
	file.Close();
	
	//Commit the Store so the attachment gets stored 
	store->CommitL();
		
	//store the attachment name
	TFileName oldfilepath;
	oldfilepath = filename;	
		
	//Test the Rename function
	filename.Append(_L("new"));
	store->AttachmentManagerExtensionsL().RenameAttachmentL(0,filename);
	//Rename again
	filename.Append(_L("name"));
	store->AttachmentManagerExtensionsL().RenameAttachmentL(0,filename);
		 
	//Commit the Store so the new name gets stored 
	store->RevertL();
	
	// Check that rename has occured
	// and print the new name on the log
	
	attachment = store->AttachmentManagerL().GetAttachmentInfoL(0);
	CleanupStack::PushL(attachment);
	
	_LIT(intro, "Testing the new name of attachment with sync");
	theUtils->Printf(intro);
	
	TFileName attachmentName;
	attachmentName = attachment->FilePath();
	TParse parse;
	parse.Set(attachmentName,NULL,NULL);
	test(parse.NameAndExt().CompareF(oldfilepath)==0);
	
	//Test the attachment names are correct before and after renaming
	User::LeaveIfError(fs.Connect());
	TInt err = file.Open(fs, attachmentName, EFileRead);
	test(err == KErrNone);
	err = file.Open(fs, filename, EFileRead);
	test((err==KErrNotFound) || (err==KErrPathNotFound));

	CleanupStack::PopAndDestroy(3,cEntry); 	
	}

LOCAL_C void TestAddAttachmentUsingLockFileNameL()
	{
	CMsvEntry* cEntry = CreateTestEntryLC();
	
	// Open entry file
	CMsvStore* store = cEntry->EditStoreL();
	CleanupStack::PushL(store);
	
    //Create an attachment and rename it 
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	
	_LIT(KTxtRenameLockFile, "_Rename_._");
	TFileName filename;
	filename.Append(KTxtRenameLockFile);
	attachment->SetAttachmentNameL(filename);
	
	// Create an attachment with the same name as the rename lock file, should not allow
	// attachments to be added with this name
	RFile file;
	TRAPD(err, store->AttachmentManagerExtensionsL().CreateAttachmentL(filename, file, attachment));
	test(err==KErrBadName);
	
	CleanupStack::PopAndDestroy(3, cEntry); // attachment, store, cEntry	
	}

LOCAL_C void doTestsL()
	{
	
	CMsgsTestUtils::CleanMessageFolderL();
	
	// Create a session and observer
	CDummyObserver ob;
	theSession = CMsvSession::OpenSyncL(ob); 
	CleanupStack::PushL(theSession);
	
	PanicSchedulerOnError = EFalse;

	CMsgsTestUtils::Start(_L("Testing RenameAttachment API"));
	TestRenameAttachmentL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Start(_L("Testing Sync RenameAttachment API"));
	TestRenameAttachmentSyncL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Testing Rename with existing name"));
	TestRenameWithExistNameL();
	CMsgsTestUtils::Complete();	
	
	CMsgsTestUtils::Start(_L("Testing Sync Rename with an existing name"));
	TestRenameSyncWithExistNameL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Next(_L("Testing Rename twice before commit"));
	TestRenameTwiceL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Start(_L("Testing Sync Rename twice before commit"));	
	TestRenameSyncTwiceL();
	CMsgsTestUtils::Complete();

	CMsgsTestUtils::Next(_L("Testing Rename and Revert without Commit"));
	TestRenameAndRevertL();
	CMsgsTestUtils::Complete();	
	
	CMsgsTestUtils::Start(_L("Testing Sync Rename and revert before commit"));
	TestRenameSyncAndRevertL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Start(_L("Testing Sync Rename twice before revert"));	
	TestRenameSyncTwiceAndRevertL();
	CMsgsTestUtils::Complete();
	
	CMsgsTestUtils::Start(_L("Testing add attachment with rename lock file name"));	
	TestAddAttachmentUsingLockFileNameL();
	CMsgsTestUtils::Complete();
	
	CleanupStack::PopAndDestroy(theSession);
	}
