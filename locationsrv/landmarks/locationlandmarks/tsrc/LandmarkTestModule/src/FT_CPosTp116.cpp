/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*   ?description_line
*
*/


//  INCLUDES
#include "FT_CPosTp116.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <d32dbms.h>
#include <StifLogger.h>
//  CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp116::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp116::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    }

// ---------------------------------------------------------
// CPosTp116::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp116::StartL()
    {
    // First remove all landmark databases
    RemoveAllLmDatabasesL();
    	
    // Start test
    LandmarkEventTestL();
   
    // Remove all landmark databases
   RemoveAllLmDatabasesL();
	}

// ---------------------------------------------------------
// CPosTp28::LandmarkEventTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp116::LandmarkEventTestL()
    {
    iLog->Log(_L("LandmarkEventTestL"));

  //  _LIT(KDatabase, "file://c:eposlm.ldb");
    _LIT(KDatabase, "file://c:Tp116Test.ldb");
    _LIT(KDatabase1, "file://c:Tp116LmDb_1.ldb");

#ifdef __WINS__
    // For WINS use C drive
    _LIT(KDatabase1_OnMedia, "file://c:Tp116LmDb_1_OnMedia.ldb");
    _LIT(KDatabase2_OnMedia, "file://c:Tp116LmDb_2_OnMedia.ldb");
#else
    //Media card must therefore be inserted
    _LIT(KDatabase1_OnMedia, "file://F:Tp116LmDb_1_OnMedia.ldb");
    _LIT(KDatabase2_OnMedia, "file://F:Tp116LmDb_2_OnMedia.ldb");
#endif
    
    _LIT(KDatabase2, "file://c:Tp116LmDb_2.ldb");
    _LIT(KDatabase3, "file://c:Tp116LmDb_3.ldb");
    _LIT(KDatabase4, "file://c:Tp116LmDb_4.ldb");
    
    _LIT(KDatabase5, "file://c:Tp116DummyLmDb_5.ldb");
    _LIT(KDatabase6, "file://c:Tp116DummyLmDb_6.ldb");
    _LIT(KDatabase7, "file://c:Tp116DummyLmDb_7.ldb");
	_LIT(KDatabase8, "file://c:Tp116DummyLmDb_8.ldb");
	_LIT(KDatabase9, "file://c:Tp116DummyLmDb_9.ldb");
	_LIT(KDatabase10, "file://c:Tp116DummyLmDb_10.ldb");
	_LIT(KDatabase11, "file://c:Tp116DummyLmDb_11.ldb");
	_LIT(KDatabase12, "file://c:Tp116DummyLmDb_12.ldb");
	_LIT(KDatabase13, "file://c:Tp116LmDb_13.ldb");
	_LIT(KDatabase14, "file://c:Tp116LmDb_14.ldb");
	
	_LIT(KRemoteDb1, "test://RemoteDb1.ldb");
    _LIT(KRemoteDbName1, "RemoteDb1New");
    _LIT(KNonExistingDb, "ANonExistingDb.ldb");

    TInt err = KErrNone;
    iLog->Log(_L("Warning: this test case requires E: drive"));
    
    iActiveLmEvent = CTP116ActiveLmEvent::NewL(iLog);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    
    HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(KDatabase);
    manager->CreateDatabaseL(*dbInfo);
    CleanupStack::PopAndDestroy(dbInfo);
    
    // This db must exist, set here so that when setting default db next it is different from before
    // otherwise no event
    manager->SetDefaultDatabaseUriL(KDatabase);

    // 1) iActiveLmEvent will listen for multiple database events
    iActiveLmEvent->Start();
    
    iLog->Log(_L("2) EDatabaseCreated "));
    // 2) Create a landmark database, db1 on the c: drive
    HPosLmDatabaseInfo* dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
    manager->CreateDatabaseL(*dbInfo1);
    CleanupStack::PopAndDestroy(dbInfo1);
    
    //iActiveLmEvent->iExpectedEvent = CPosLmDatabaseManager::EDatabaseCreated;
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    
    iActiveLmEvent->SetExpectedDatabase(KDatabase1);
    CActiveScheduler::Start();
      
   	iLog->Log(_L("3) ENewDefaultDbLocation "));
    // 3) Set the defaultdb to point at the newly created db
    manager->SetDefaultDatabaseUriL(KDatabase1);
    iActiveLmEvent->iExpectedEvent = EPosLmDbNewDefaultDbLocation;
    iActiveLmEvent->SetExpectedDatabase(KDatabase1);
    CActiveScheduler::Start();


    iLog->Log(_L("5) EDatabaseCreated "));
    // 5) Copy db1 to media drive
    manager->CopyDatabaseL(KDatabase1, KDatabase1_OnMedia);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase1_OnMedia);
    CActiveScheduler::Start();

    iLog->Log(_L("6) EDatabaseDeleted "));
    // 6) Delete copied db from media card
    manager->DeleteDatabaseL(KDatabase1_OnMedia);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase1_OnMedia);
    CActiveScheduler::Start();

    iLog->Log(_L("7) EDatabaseCreated - EDatabaseDeleted"));
   // 7) Move db1 to media drive as db1_2-> First copy it and then delete (in same operation phase)
    manager->CopyDatabaseL(KDatabase1, KDatabase2_OnMedia);
    manager->DeleteDatabaseL(KDatabase1);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2_OnMedia);
    CActiveScheduler::Start();
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase1);
    CActiveScheduler::Start();

    iLog->Log(_L("8) EDatabaseCreated"));
    //8) Create a db, db2, on media card
    
    HPosLmDatabaseInfo* dbInfo2 = HPosLmDatabaseInfo::NewLC(KDatabase2);
    manager->CreateDatabaseL(*dbInfo2);
    CleanupStack::PopAndDestroy(dbInfo2);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();

    iLog->Log(_L("9) ENewDefaultDbLocation"));
    // 9) Set the defaultdb to point at the newly created db
    manager->SetDefaultDatabaseUriL(KDatabase2);
    iActiveLmEvent->iExpectedEvent = EPosLmDbNewDefaultDbLocation;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();
    
    iLog->Log(_L("9b) EPosLmDbSettingsModified"));
    // 9b) Set the defaultdb to point at the newly created db
    HPosLmDatabaseInfo* dbInfoChange = HPosLmDatabaseInfo::NewLC(KDatabase2);
    TPosLmDatabaseSettings settings = dbInfo2->Settings();
    _LIT(KNewDbName, "TP116 Disp Name");
    TPosLmDatabaseName bufName(KNewDbName);
    settings.SetDatabaseName(bufName);
    manager->ModifyDatabaseSettingsL(KDatabase2, settings);
    CleanupStack::PopAndDestroy(dbInfoChange);
    
    iActiveLmEvent->iExpectedEvent = EPosLmDbSettingsModified;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();

    iLog->Log(_L("10) EDatabaseDeleted"));
    // 10) Delete db2 from media card
    manager->DeleteDatabaseL(KDatabase2);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();

    iLog->Log(_L("11) EDatabaseCreated"));
    // 11) Copy db1_2 to C:\ as db1_3
    manager->CopyDatabaseL(KDatabase2_OnMedia, KDatabase3);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase3);
    CActiveScheduler::Start();

    iLog->Log(_L("12) EDatabaseDeleted"));
    // 12) Move db1_2 from media to C:\ drive as db1_4 -> First 
    // copy it and then delete (in same operation phase)
    manager->CopyDatabaseL(KDatabase2_OnMedia, KDatabase4);
    manager->DeleteDatabaseL(KDatabase2_OnMedia);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase4);
    CActiveScheduler::Start();
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2_OnMedia);
    CActiveScheduler::Start();

    iLog->Log(_L("13) ENewDefaultDbLocation"));
    // 13) Set the defaultdb to point at db1_4
    manager->SetDefaultDatabaseUriL(KDatabase4);
    iActiveLmEvent->iExpectedEvent = EPosLmDbNewDefaultDbLocation;
    iActiveLmEvent->SetExpectedDatabase(KDatabase4);
    CActiveScheduler::Start();

    iLog->Log(_L("14) EDatabaseDeleted"));
    // 14) Delete db1_4
    manager->DeleteDatabaseL(KDatabase4);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase4);
    CActiveScheduler::Start();
    
	iLog->Log(_L("15) --------- "));
	// Try to create a queue of events that is bigger than the internal queue buffer
	// should causes unknown event to happen and no databaseUri
    HPosLmDatabaseInfo* dbInfo13 = HPosLmDatabaseInfo::NewLC(KDatabase13);
    manager->CreateDatabaseL(*dbInfo13);
    CleanupStack::PopAndDestroy(dbInfo13);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase13);
    iActiveLmEvent->iQueueTest = ETrue;
    CActiveScheduler::Start();

	HPosLmDatabaseInfo* dbInfo4 = HPosLmDatabaseInfo::NewLC(KDatabase4);
	HPosLmDatabaseInfo* dbInfo5 = HPosLmDatabaseInfo::NewLC(KDatabase5);
	HPosLmDatabaseInfo* dbInfo6 = HPosLmDatabaseInfo::NewLC(KDatabase6);
	HPosLmDatabaseInfo* dbInfo7 = HPosLmDatabaseInfo::NewLC(KDatabase7);
	HPosLmDatabaseInfo* dbInfo8 = HPosLmDatabaseInfo::NewLC(KDatabase8);
	HPosLmDatabaseInfo* dbInfo9 = HPosLmDatabaseInfo::NewLC(KDatabase9);
	
	manager->CreateDatabaseL(*dbInfo4);
	manager->CreateDatabaseL(*dbInfo5);
	manager->DeleteDatabaseL(KDatabase4);
	manager->CopyDatabaseL(KDatabase5, KDatabase10);
	manager->CreateDatabaseL(*dbInfo6);
	manager->DeleteDatabaseL(KDatabase5);
	manager->CreateDatabaseL(*dbInfo7);
	manager->DeleteDatabaseL(KDatabase6);
	manager->CopyDatabaseL(KDatabase10, KDatabase11);
	manager->CreateDatabaseL(*dbInfo8);
	manager->DeleteDatabaseL(KDatabase7);
	manager->CreateDatabaseL(*dbInfo9);
	manager->CopyDatabaseL(KDatabase10, KDatabase12);
	manager->DeleteDatabaseL(KDatabase9);
	manager->DeleteDatabaseL(KDatabase8);
	
	CleanupStack::PopAndDestroy(dbInfo9);
	CleanupStack::PopAndDestroy(dbInfo8);
	CleanupStack::PopAndDestroy(dbInfo7);
	CleanupStack::PopAndDestroy(dbInfo6);
	CleanupStack::PopAndDestroy(dbInfo5);
	CleanupStack::PopAndDestroy(dbInfo4);
		
    // Check for unknown
	iActiveLmEvent->iExpectedEvent = EPosLmDbUnknownEvent;
    iActiveLmEvent->SetExpectedDatabase(KNullDesC);
    iActiveLmEvent->Start();
    CActiveScheduler::Start();
	
	iActiveLmEvent->iQueueTest = EFalse;
	
	TInt errorsFound = iActiveLmEvent->GetErrors();
    
    iActiveLmEvent->DoCancel();
    
    iActiveLmEvent->Start();
    
	iLog->Log(_L("16) EDatabaseCreated "));
    // 16) Check correct event after unknown event
    // Note: This does not work, new events are added to the queue after unknown event, so instead we 
    // cancel the event handling
    HPosLmDatabaseInfo* dbInfo14 = HPosLmDatabaseInfo::NewLC(KDatabase14);
    manager->CreateDatabaseL(*dbInfo14);
    CleanupStack::PopAndDestroy(dbInfo14);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase14);
    CActiveScheduler::Start();
    
    errorsFound = iActiveLmEvent->GetErrors();
    
    iActiveLmEvent->DoCancel();
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    iActiveLmEvent = CTP116ActiveLmEvent::NewL(iLog);
    
    // Start listening
    iActiveLmEvent->Start();
        
	iLog->Log(_L("17) --------- "));
    // 17) Test that event handling is working for "remote" provider (use the test provider)
    //  as well
    HPosLmDatabaseInfo* db1 = HPosLmDatabaseInfo::NewLC(KRemoteDb1);
	manager->CreateDatabaseL(*db1);
    // Register remote db
    manager->RegisterDatabaseL(*db1);
    
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KRemoteDb1);
    CActiveScheduler::Start();
    
    iLog->Log(_L("18) --------- "));
    // 18) Test that event handling is working for "remote" provider (use the test provider)
    //  as well
    // Modify Settings
    TPosLmDatabaseSettings remoteSettings = db1->Settings();
    TPosLmDatabaseName remoteName(KRemoteDbName1);
    remoteSettings.SetDatabaseName(remoteName);
    manager->ModifyDatabaseSettingsL(KRemoteDb1, remoteSettings);
    
    iActiveLmEvent->iExpectedEvent = EPosLmDbSettingsModified;
    iActiveLmEvent->SetExpectedDatabase(KRemoteDb1);
    CActiveScheduler::Start();
    CleanupStack::PopAndDestroy(db1);
    
    iLog->Log(_L("19) --------- "));
    // 19) Test that event handling is working for "remote" provider (use the test provider)
    //  as well
    // Unregister remote db
    manager->UnregisterDatabaseL(KRemoteDb1);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    iActiveLmEvent->SetExpectedDatabase(KRemoteDb1);
    CActiveScheduler::Start();
       
	iLog->Log(_L("20) --------- "));
    // 20) Set the defaultdb to point at a non-existing db
    TRAP(err, manager->SetDefaultDatabaseUriL(KNonExistingDb));
    if (err != KErrNotFound) 
    {
    	iLog->Log(_L("SetDefaultDatabaseL(KNonExistingDb) should leave with KErrNotFound"));
    	User::Leave(err);
    }
    iLog->Log(_L("20 Done"));
    
    CleanupStack::PopAndDestroy(manager);
        
	errorsFound += iActiveLmEvent->GetErrors();
	
	delete iActiveLmEvent;
    iActiveLmEvent = NULL;
	
	if (errorsFound != KErrNone) 
		{
		iLog->Log(_L("Errors were found in TP116"));
    	User::Leave(-1);
		}

    }

// -----------------------------------------------------------------------------
// CTP116ActiveLmEvent::NewL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
CTP116ActiveLmEvent* CTP116ActiveLmEvent::NewL(CStifLogger* aLog)
    {
    CTP116ActiveLmEvent* self = new(ELeave) CTP116ActiveLmEvent(aLog);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CTP116ActiveLmEvent::ConstructL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTP116ActiveLmEvent::ConstructL()
    {
    iManager = CPosLmDatabaseManager::NewL();
    }

// -----------------------------------------------------------------------------
// CTP116ActiveLmEvent::CTP116ActiveLmEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CTP116ActiveLmEvent::CTP116ActiveLmEvent(CStifLogger* aLog) : 
    CActive(EPriorityNormal),
    iLog(aLog)
    {   
    CActiveScheduler::Add(this);
    iGlobalErr=KErrNone;
    iQueueTest = EFalse;
    }

// Destructor
CTP116ActiveLmEvent::~CTP116ActiveLmEvent()
    {
    Cancel();
    delete iManager;
    iManager = NULL;
    }

// ---------------------------------------------------------
// CTP116ActiveLmEvent::SetExpectedDatabase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP116ActiveLmEvent::SetExpectedDatabase(const TDesC& aDatabaseUri)
    {
    iExpectedDatabase.Zero();
    
    iExpectedDatabase.Append(aDatabaseUri);
    }

// ---------------------------------------------------------
// CTP116ActiveLmEvent::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP116ActiveLmEvent::Start()                          
    {
    // Get ready for event
    iManager->NotifyDatabaseEvent(iEvent, iStatus);
    SetActive();
    }

// ---------------------------------------------------------
// CTP116ActiveLmEvent::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP116ActiveLmEvent::RunL()
    {
     iLog->Log(_L("CTP116ActiveLmEvent::RunL"));

    TPosLmDatabaseEvent event = iEvent;
    VerifyEvent(event);
    if (!iQueueTest)
    	{
    	// Get ready for next event
    	iManager->NotifyDatabaseEvent(iEvent, iStatus); 
    	SetActive();
    	}
    }

// ---------------------------------------------------------
// CActiveLmOperation::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP116ActiveLmEvent::DoCancel()
    {
    iLog->Log(_L("CTP116ActiveLmEvent::DoCancel"));
    iManager->CancelNotifyDatabaseEvent();
    }

// ---------------------------------------------------------
// CActiveLmOperation::GetErrors
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CTP116ActiveLmEvent::GetErrors()
    {
	return iGlobalErr;
    }
    
// ---------------------------------------------------------
// CTP116ActiveLmEvent::VerifyEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP116ActiveLmEvent::VerifyEvent(TPosLmDatabaseEvent event)
    {
    iLog->Log(_L("CTP116ActiveLmEvent::VerifyEvent"));
    TBuf<150> buffe;
    
    buffe.Format(_L("VerifyEvent Event: %d "), event.iEventType);
	iLog->Log(buffe);
	    
    // Verify event here
    if (event.iEventType != iExpectedEvent) 
        {
        buffe.Zero();
        buffe.Format(_L("ERROR: Wrong event: exp %d got %d"),iExpectedEvent, event.iEventType);
        iLog->Log(buffe);
        iGlobalErr++;
        }
    	
	if (event.iEventType != EPosLmDbUnknownEvent)	
		{
		
	    HBufC16* dbUri = iManager->DatabaseUriFromLastEventLC();
	    buffe.Zero();
	    buffe.Format(_L("Event: %d URI %S"), event.iEventType, dbUri);
	    iLog->Log(buffe);
    
		if (iExpectedDatabase.CompareC(*dbUri) != 0)
			{
			iLog->Log(_L("ERROR: Wrong db name"));
			buffe.Zero();
			buffe.Append(_L("Should be "));
			buffe.Append(iExpectedDatabase);
			iLog->Log(buffe);
	        iGlobalErr++;
			}
	    CleanupStack::PopAndDestroy(dbUri);			
		}
	else
		{
		buffe.Format(_L("EUnknownEvent Event: %d "), event.iEventType);
	    iLog->Log(buffe);
		}
        
	CActiveScheduler::Stop();
   
	iLog->Log(_L("VerifyEvent Done"));
    }

//  End of File
