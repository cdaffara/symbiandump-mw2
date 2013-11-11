// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <cmsvcopystoreoperation.h>
#include "cmsvdeletestoreoperation.h"
#include <sqldb.h>


RTest test(_L("Test Harness for Copy/Delete MailStore"));
_LIT(KIndex, "Index");

//

class CSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*);
public:
	CMsvSession* iSession;
	TMsvSessionEvent iType;
	TInt iArg1;
	TInt iArg2;
	};

void CSessionObserver::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny*)
	{
	if (aEvent == EMsvServerReady ||
		aEvent == EMsvCorruptedIndexRebuilt ||
		aEvent == EMsvMediaIncorrect ||
		aEvent == EMsvCorruptedIndexRebuilding)
		{
		// See if the index is available
		if (aEvent != EMsvMediaIncorrect &&  aEvent != EMsvCorruptedIndexRebuilding)
			{
			TMsvId service;
			TMsvEntry entry;

			// Check we can retrieve the inbox details from the index
			test(iSession->GetEntry(KMsvGlobalInBoxIndexEntryId, service, entry) == KErrNone);
			}

		iType = aEvent;
		if (aArg1)
			iArg1 = *(TInt*)aArg1;
		if (aArg2)
			iArg2 = *(TInt*)aArg2;
		CActiveScheduler::Stop();
		}
	}

//

/* Test to check the new API
** CopyStoreL(TInt aDrive, TMsvOp aOperationId, TRequestStatus& aStatus) 
*/
LOCAL_C void TestCopyStoreOperationL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
    
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);
       
	// Set session in observer
	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; 
	CleanupStack::PushL(session);
    
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	
	//Create an entry in the mail
	
	CMsvEntry* entry=session->GetEntryL(KMsvDraftEntryId);
	CleanupStack::PushL(entry);
	// create entry to work under
	TMsvEntry folder;
	folder.iType = KUidMsvFolderEntry;
	folder.iMtm = KUidMsvLocalServiceMtm;
	folder.iServiceId = KMsvLocalServiceIndexEntryId;
	_LIT(KTestDescription,"A Description");
	_LIT(KTestDetails,"A Details");
	folder.iDescription.Set(KTestDescription);
	folder.iDetails.Set(KTestDetails);
	entry->CreateL(folder);
	entry->SetEntryL(folder.Id());
	CMsvStore* store=entry->EditStoreL();
	CleanupStack::PushL(store);
	RMsvWriteStream stream;
	stream.AssignLC(*store,TUid::Uid(0x1000));
	stream.WriteL(KTestDescription);
	stream.CommitL();
	store->CommitL();
	CleanupStack::PopAndDestroy(3);
            
	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);
     
	// Test Copy MailStore 
	active->StartL();
	CMsvOperation* operation = NULL;
	TDriveUnit unit =(EDriveD);
	operation = session->CopyStoreL(unit, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);
	
	CActiveScheduler::Start();
	
	//Retrieve progress 
	TPckgBuf<TMsvCopyProgress> package;
	package.Copy(operation->ProgressL());
	test(package().iError == KErrNone);
		
	//delete session;
	CleanupStack::PopAndDestroy(4);
		
	//Check that both the source and destination
	//MailStore directory structure is same
	
	CMsgsTestUtils::WaitForServerClose();
	
	CDir 		 *srcDir;
	CDir 	     *destDir;
	_LIT(intro, "Testing the directory structure");
	_LIT(KEntryname,"%S");
	
	theUtils->Printf(intro);
	
	TFileName dest = unit.Name();
	dest.Append(KMsvDefaultFolder2);

	User::LeaveIfError(theUtils->FileSession().GetDir(dest, KEntryAttDir|KEntryAttNormal, ESortByName, destDir));
	
	unit =EDriveC;
	TFileName src = unit.Name();
	src.Append(KMsvDefaultFolder2);

	User::LeaveIfError(theUtils->FileSession().GetDir(src, KEntryAttDir|KEntryAttNormal, ESortByName, srcDir));
	
	TInt counter=0;
	TEntry srcEntry;
	TEntry destEntry;

	while (counter<srcDir->Count())
		{
		srcEntry = (*srcDir)[counter];
		destEntry = (*destDir)[counter];
		//Check the sizes 
		test(srcEntry.iSize == destEntry.iSize);
		//Check the names
		test(srcEntry.iName == destEntry.iName);
		//Print out the names on the log
		theUtils->Printf(KEntryname,&(srcEntry.iName));
		counter++;
		}
		
	delete srcDir;
	delete destDir;
	
	}
	
/* Test to check that CopyStoreL(TInt aDrive, TMsvOp aOperationId, TRequestStatus& aStatus) 
** has worked. An attempt to copy again returns KErrAlreadyExists.
*/

	
LOCAL_C void TestStoreExistsL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);
	
	//Check that message store exists on the drive at this stage
	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);
       
	// Set session in observer
	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; 
	CleanupStack::PushL(session);
    
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);
	
	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);
     
	// Test Copy MailStore 
	active->StartL();
	CMsvOperation* operation = NULL;
	TDriveUnit unit = EDriveD;
	operation = session->CopyStoreL(unit, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);
	
	CActiveScheduler::Start();
	
	//Retrieve progress 
	//Should retrun KErrAlreadyExists
	TPckgBuf<TMsvCopyProgress> package;
	package.Copy(operation->ProgressL());
	test(package().iError == KErrAlreadyExists);
		
	//delete session;
	CleanupStack::PopAndDestroy(4, ob);
	
	CMsgsTestUtils::WaitForServerClose();
		
  	}
	
	
/* Test to check if CopyStoreL(TInt aDrive, TMsvOp aOperationId, TRequestStatus& aStatus) 
** works appropriately by deleting any broken store first and then attempting to 
** copy the store
*/
	
LOCAL_C void TestBrokenStoreL()
	{
	
	TDriveUnit unit =EDriveD;
	TFileName dest = unit.Name();
	dest.Append(KMsvDbFile);
	RSqlDatabase::Delete(dest);
	//Call Copy Mail Store to check any existing broken files are
	//deleted first before copy begins
	
	TestCopyStoreOperationL();	
	}
	
	
	
/* Test to check the new API
** DeleteStoreL(TInt aDrive, TMsvOp aOperationId, TRequestStatus& aStatus) 
*/ 
	
LOCAL_C void TestDeleteStoreOperationL()
	{
	CMsgsTestUtils::SetDriveL(EDriveC);

	CSessionObserver* ob = new(ELeave)CSessionObserver;
	CleanupStack::PushL(ob);

	// Set session in observer
	CMsvSession* session = CMsvSession::OpenAsyncL(*ob);
	ob->iSession = session; 
	CleanupStack::PushL(session);
	
	CActiveScheduler::Start();
	test(ob->iType==MMsvSessionObserver::EMsvServerReady);

	CTestActive* active = new(ELeave)CTestActive;
	CleanupStack::PushL(active);
	
	// Test Delete MailStore 
	active->StartL();
	CMsvOperation* operation = NULL;
	TDriveUnit unit =(EDriveD);
	operation = session->DeleteStoreL(unit, active->iStatus);
	test(operation->Mtm() == KUidMsvServerMtm);
	CleanupStack::PushL(operation);
	
    CActiveScheduler::Start();
    
    //Retrieve progress 
	TPckgBuf<TMsvCopyProgress> package;
	package.Copy(operation->ProgressL());
	test((package().iError == KErrNone) || (package().iError == KErrPathNotFound)) ;
	
	CleanupStack::PopAndDestroy(4);	
	}
	

LOCAL_C void doTestsL()
	{
	PanicSchedulerOnError = EFalse;

	// initialisation and cleanup
	CMsgsTestUtils::RemoveMailFolderL(EDriveC);
	CMsgsTestUtils::RemoveMailFolderL(EDriveD);
	CMsgsTestUtils::CleanMessageFolderL();
	
	CMsgsTestUtils::Start(_L("Testing Copy MailStore"));
	TestCopyStoreOperationL();
	CMsgsTestUtils::Complete();
	CMsgsTestUtils::Next(_L("Testing MailStore Exists"));
	TestStoreExistsL();
	CMsgsTestUtils::Complete();
	/*CMsgsTestUtils::Next(_L("Testing Broken MailStore"));
	TestBrokenStoreL();
	CMsgsTestUtils::Complete();*/
	CMsgsTestUtils::Next(_L("Testing Delete MailStore"));
	TestDeleteStoreOperationL();
	CMsgsTestUtils::Complete();
	}
