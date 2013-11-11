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
#include "FT_CPosTp117.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <d32dbms.h>
#include <StifLogger.h>

//#include <ss_std.h>
//  CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp117::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    
    iThread.Close();
    }

// ---------------------------------------------------------
// CPosTp117::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::StartL()
    {
    
    // Remove all landmark databases
   	RemoveAllLmDatabasesL();  
    LandmarkQueuedEvenTestL();
    
    // Remove all landmark databases
   	RemoveAllLmDatabasesL();
    LandmarkQueuedEvenTest2L();

    // Remove all landmark databases
   	RemoveAllLmDatabasesL();
    // Start test
    LandmarkEventTestL();
    
    // Remove all landmark databases
   	RemoveAllLmDatabasesL();
   	
    // Pancic test
    iActiveLmEvent = CTP117ActiveLmEvent::NewL(iLog);
    iExpectedExitType = ExitPanic;
    iLog->Log(_L("Panic test 1"));
    iTestCase = 1;
    /* EPosEventNotifierAlreadyHasOutstandingRequest defined in EPos_LandmarksServerPanics.h */
    iExpectedErrorCode = 2;
    ResumeThreadAndVerifyExit();
    
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    
    
    if (iErrorsFound != KErrNone)
    	{
   
        iLog->Log(_L("Errors found in TP117"));
        User::Leave(-1);

    	}
	}

// ---------------------------------------------------------
// CPosTp28::LandmarkQueuedEvenTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::LandmarkQueuedEvenTestL()
    {
    iLog->Log(_L(">>>>>>>>>>LandmarkQueuedEvenTestL<<<<<<<<<"));
    
    _LIT(KDatabase1, "c:Tp117DummyLmDb_1.ldb");
    _LIT(KDatabase2, "c:Tp117DummyLmDb_2.ldb");
    _LIT(KDatabase3, "c:Tp117DummyLmDb_3.ldb");
    _LIT(KDatabase4, "c:Tp117DummyLmDb_4.ldb");
	
	CTP117ActiveLmEvent* client1 = CTP117ActiveLmEvent::NewL(iLog);
	CleanupStack::PushL(client1);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    
    // Creates db2, which should exist (deleted below)
    client1->Start();
    HPosLmDatabaseInfo* dbInfo2 = HPosLmDatabaseInfo::NewLC(KDatabase2);
    manager->CreateDatabaseL(*dbInfo2);
    CleanupStack::PopAndDestroy(dbInfo2);
    client1->iExpectedEvent = EPosLmDbDatabaseRegistered;
    client1->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();

	// *********Start test*************
	iLog->Log(_L("*********Start test*************"));
	// 1) Enable client1
	client1->Start();
	client1->iExpectedEvent = EPosLmDbDatabaseRegistered;
    client1->SetExpectedDatabase(KDatabase1);
	
    // 2) Client2 creates a database, db1
    iLog->Log(_L("Client2 creates a database, db1"));
    HPosLmDatabaseInfo* dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
    manager->CreateDatabaseL(*dbInfo1);
    CleanupStack::PopAndDestroy(dbInfo1);
    
    // 3) Client1 NotifyDatabaseEvent, should be event from create operation in step2
    iLog->Log(_L("Client1 will get EDatabaseCreated event"));
    CActiveScheduler::Start();
        
    iLog->Log(_L("Delete, Copy, Create db"));
    //4) Client2 deletes db2
    manager->DeleteDatabaseL(KDatabase2);
    
    //5) Client2 copies db1 to db3
    manager->CopyDatabaseL(KDatabase1, KDatabase3);
    
    //6) Client2 creates db4
    HPosLmDatabaseInfo* dbInfo4 = HPosLmDatabaseInfo::NewLC(KDatabase4);
    manager->CreateDatabaseL(*dbInfo4);
    CleanupStack::PopAndDestroy(dbInfo4);
    
    // 7) Client1 NotifyDatabaseEvent, should be event from delete operation in step4
    iLog->Log(_L("7) Client1 NotifyDatabaseEvent"));
    client1->Start();
    // Dont check URI here
    client1->iCheckUri = EFalse;
	client1->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    client1->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();
    
    // 8) Client1 NotifyDatabaseEvent, should be event from copy operation in step5
	iLog->Log(_L("8) Client1 NotifyDatabaseEvent"));
    client1->Start();
    // Dont check URI here
	client1->iExpectedEvent = EPosLmDbDatabaseRegistered;
    client1->SetExpectedDatabase(KDatabase3);
    CActiveScheduler::Start();
    
    // 9) Client1 NotifyDatabaseEvent, should be event from create operation in step6
    iLog->Log(_L("9) Client1 NotifyDatabaseEvent"));
    // Check URI here
    client1->iCheckUri = ETrue;
    client1->Start();
	client1->iExpectedEvent = EPosLmDbDatabaseRegistered;
    client1->SetExpectedDatabase(KDatabase4);
    CActiveScheduler::Start();
    
    iErrorsFound+=client1->GetErrors();
    
    CleanupStack::PopAndDestroy(manager);
    CleanupStack::PopAndDestroy(client1);
    }
    
// ---------------------------------------------------------
// CPosTp28::LandmarkQueuedEvenTest2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::LandmarkQueuedEvenTest2L()
    {
    iLog->Log(_L(">>>>>>>>>>LandmarkQueuedEvenTest2L>>>>>>>>>>"));

    _LIT(KDatabase1, "c:Tp117DummyLmDb_1.ldb");
    _LIT(KDatabase2, "c:Tp117DummyLmDb_2.ldb");
    _LIT(KDatabase3, "c:Tp117DummyLmDb_3.ldb");
    _LIT(KDatabase4, "c:Tp117DummyLmDb_4.ldb");
    _LIT(KDatabase5, "c:Tp117DummyLmDb_5.ldb");
	
	CTP117ActiveLmEvent* client1 = CTP117ActiveLmEvent::NewL(iLog);
	CleanupStack::PushL(client1);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    
    // Creates db2, which should exist (deleted below)
    client1->Start();
    HPosLmDatabaseInfo* dbInfo2 = HPosLmDatabaseInfo::NewLC(KDatabase2);
    manager->CreateDatabaseL(*dbInfo2);
    CleanupStack::PopAndDestroy(dbInfo2);
    client1->iExpectedEvent = EPosLmDbDatabaseRegistered;
    client1->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();

	// *********Start test*************
	iLog->Log(_L("*********Start test*************"));
	// 1) Enable client1
	client1->Start();
	client1->iExpectedEvent = EPosLmDbDatabaseRegistered;
    client1->SetExpectedDatabase(KDatabase1);
	
    // 2) Client2 creates a database, db1
    iLog->Log(_L("Client2 creates a database, db1"));
    HPosLmDatabaseInfo* dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
    manager->CreateDatabaseL(*dbInfo1);
    CleanupStack::PopAndDestroy(dbInfo1);
    
    // 3) Client1 NotifyDatabaseEvent, should be event from create operation in step2
    iLog->Log(_L("Client1 will get EDatabaseCreated event"));
    CActiveScheduler::Start();
        
    iLog->Log(_L("Delete, Copy, Create, Delete, Copy, Delete"));
    //4) Client2 deletes db2
    manager->DeleteDatabaseL(KDatabase2);
    
    //5) Client2 copies db1 to db3
    manager->CopyDatabaseL(KDatabase1, KDatabase3);
    
    //6) Client2 creates db4
    HPosLmDatabaseInfo* dbInfo4 = HPosLmDatabaseInfo::NewLC(KDatabase4);
    manager->CreateDatabaseL(*dbInfo4);
    CleanupStack::PopAndDestroy(dbInfo4);
    
    //7) Client2 deletes db3
    manager->DeleteDatabaseL(KDatabase3);
    
    //8) Client2 copies db2 to db5
    manager->CopyDatabaseL(KDatabase1, KDatabase5);
    
    //7) Client2 deletes db2
    manager->DeleteDatabaseL(KDatabase5);
       
    // 9) Client1 NotifyDatabaseEvent, should be unknown event since too many events in queue
    // After unknown event the queue should be empty again, verified in step10
    iLog->Log(_L("9) Client1 NotifyDatabaseEvent"));    
    client1->Start();
	client1->iExpectedEvent = EPosLmDbUnknownEvent;
    client1->SetExpectedDatabase(KNullDesC);
    CActiveScheduler::Start();
    
    //10) Client1 deletes db4
    client1->Start();    
	iLog->Log(_L("10) Client1 NotifyDatabaseEvent"));    
    manager->DeleteDatabaseL(KDatabase4);
	client1->iExpectedEvent = EPosLmDbDatabaseUnregistered;
    client1->SetExpectedDatabase(KDatabase4);
    CActiveScheduler::Start();   
    
    iErrorsFound+=client1->GetErrors();
    
    CleanupStack::PopAndDestroy(manager);
    CleanupStack::PopAndDestroy(client1);
    }
        
// ---------------------------------------------------------
// CPosTp28::LandmarkEventTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::LandmarkEventTestL()
    {
    iLog->Log(_L(">>>>>>>>>>LandmarkEventTestL<<<<<<<<<<<<<<"));

    _LIT(KDatabase1, "c:Tp117DummyLmDb_1.ldb");
    _LIT(KDatabase2, "c:Tp117DummyLmDb_2.ldb");
    _LIT(KDatabase3, "c:Tp117DummyLmDb_3.ldb");
    _LIT(KDatabase4, "c:Tp117DummyLmDb_4.ldb");
    
    _LIT(KDatabase5, "c:Tp117DummyLmDb_5.ldb");
    _LIT(KDatabase6, "c:Tp117DummyLmDb_6.ldb");
    
	iActiveLmEvent = CTP117ActiveLmEvent::NewL(iLog);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // 1) Just do something to cause an event
    iActiveLmEvent->Start();
    HPosLmDatabaseInfo* dbInfo1 = HPosLmDatabaseInfo::NewLC(KDatabase1);
    manager->CreateDatabaseL(*dbInfo1);
    CleanupStack::PopAndDestroy(dbInfo1);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase1);
    CActiveScheduler::Start();
       
    iLog->Log(_L("Test 1_1"));
    iActiveLmEvent->Start();
    HPosLmDatabaseInfo* dbInfo2 = HPosLmDatabaseInfo::NewLC(KDatabase2);
    manager->CreateDatabaseL(*dbInfo2);
    CleanupStack::PopAndDestroy(dbInfo2);
    
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase2);
    CActiveScheduler::Start();

    iLog->Log(_L("Cancel NotifyDatabaseEvent 1"));    
    // Cancel NotifyDatabaseEvent
    iActiveLmEvent->Start();
    iActiveLmEvent->Cancel();
	iActiveLmEvent->Start();
	iActiveLmEvent->Cancel();
           
    iActiveLmEvent->Start();
    HPosLmDatabaseInfo* dbInfo3 = HPosLmDatabaseInfo::NewLC(KDatabase3);
    manager->CreateDatabaseL(*dbInfo3);
    CleanupStack::PopAndDestroy(dbInfo3);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase3);
    CActiveScheduler::Start();
    
    iLog->Log(_L("Cancel NotifyDatabaseEvent 2"));       
    // Cancel NotifyDatabaseEvent
    iActiveLmEvent->Start();
    iActiveLmEvent->Cancel();
	iActiveLmEvent->Cancel();
        
    iLog->Log(_L("Test event after cancel"));
    iActiveLmEvent->Start();
    HPosLmDatabaseInfo* dbInfo4 = HPosLmDatabaseInfo::NewLC(KDatabase4);
    manager->CreateDatabaseL(*dbInfo4);
    CleanupStack::PopAndDestroy(dbInfo4);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase4);
    CActiveScheduler::Start();
       
	iLog->Log(_L("Test to call DatabaseUriFromLastEventLC before events occurr"));
    iActiveLmEvent->Start();
    HPosLmDatabaseInfo* dbInfo5 = HPosLmDatabaseInfo::NewLC(KDatabase5);
    manager->CreateDatabaseL(*dbInfo5);
    CleanupStack::PopAndDestroy(dbInfo5);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase5);
    iActiveLmEvent->CallDatabaseUriFromLastEventLC();
    // Need to cancel this notifyDatabaseEvent call
    iActiveLmEvent->Cancel();
    
    iLog->Log(_L("Test event3 after DatabaseUriFromLastEventLC"));
    iActiveLmEvent->Start();
    HPosLmDatabaseInfo* dbInfo6 = HPosLmDatabaseInfo::NewLC(KDatabase6);   
    manager->CreateDatabaseL(*dbInfo6);
    CleanupStack::PopAndDestroy(dbInfo6);
    iActiveLmEvent->iExpectedEvent = EPosLmDbDatabaseRegistered;
    iActiveLmEvent->SetExpectedDatabase(KDatabase6);
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy(manager);
    
    iErrorsFound+=iActiveLmEvent->GetErrors();
    
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;
    }

// ---------------------------------------------------------
// CPosTp117::ResumeThreadAndVerifyExit
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::ResumeThreadAndVerifyExit()
    {
    iLog->Log(_L("ResumeThreadAndVerifyExit"));
    _LIT(KPanicErr, "Method didn't Panic when expected"); 
    _LIT(KPanicCodeErr, "Paniced with unexpected panic code");
    _LIT(KLeaveErr, "Method didn't end correctly"); 
    _LIT(KLeaveCodeErr, "Unexpected leave error");

    CreateThreadL();
    TRequestStatus status;
    iThread.Logon(status);
    iThread.Resume();
        
    User::WaitForRequest(status);
    
    // Used for debugging
    TBuf<100> buf;
    buf.Format(_L("ExitType %d Exit reason %d"), iThread.ExitType(), iThread.ExitReason());
    iLog->Log(buf);

    if (iExpectedExitType == ExitPanic)
        {
        AssertTrueSecL(iThread.ExitType() == EExitPanic, KPanicErr);
        AssertTrueSecL(iThread.ExitReason() == iExpectedErrorCode, KPanicCodeErr);
        }
    else if (iExpectedExitType == ExitLeave)
        {
        AssertTrueSecL(iThread.ExitType() == EExitKill, KLeaveErr);
        AssertTrueSecL(iThread.ExitReason() == iExpectedErrorCode, KLeaveCodeErr);
        }
    iThread.Close();
    }
    

// ---------------------------------------------------------
// CPosTp117::RunPanicTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CPosTp117::RunPanicTestL(TAny* /*aData*/)
    {
    
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    
    TPosLmDatabaseEvent event;
    TRequestStatus status;
    manager->NotifyDatabaseEvent(event, status);
    
    // TBD There might be timing problems on target
    User::After(100000);
    // The second call causes Panic EPosEventNotifierAlreadyHasOutstandingRequest
    manager->NotifyDatabaseEvent(event, status);
    User::After(100000);
    
    CleanupStack::PopAndDestroy(manager);

    }
    
// ---------------------------------------------------------
// LOCAL_C ThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp117::RunPanicTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp117::CreateThreadL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp117::CreateThreadL()
    {
    _LIT(KThreadName, "Tp117 test thread");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

     TInt err=0;
     err = iThread.Create(KThreadName, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, this);
     AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
    }
        
// -----------------------------------------------------------------------------
// CTP117ActiveLmEvent::NewL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
CTP117ActiveLmEvent* CTP117ActiveLmEvent::NewL(CStifLogger* aLog)
    {
    CTP117ActiveLmEvent* self = new(ELeave) CTP117ActiveLmEvent(aLog);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CTP117ActiveLmEvent::ConstructL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTP117ActiveLmEvent::ConstructL()
    {
    iManager = CPosLmDatabaseManager::NewL();
    }

// -----------------------------------------------------------------------------
// CTP117ActiveLmEvent::CTP117ActiveLmEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CTP117ActiveLmEvent::CTP117ActiveLmEvent(CStifLogger* aLog) : 
    CActive(EPriorityNormal),
    iLog(aLog)
    {   
    CActiveScheduler::Add(this);
    iGlobalErr=KErrNone;
    iCheckUri = ETrue;
    }

// Destructor
CTP117ActiveLmEvent::~CTP117ActiveLmEvent()
    {
    Cancel();
    delete iManager;
    iManager = NULL;
    }

// ---------------------------------------------------------
// CTP117ActiveLmEvent::SetExpectedDatabase
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::SetExpectedDatabase(const TDesC& aDatabaseUri)
    {
    iExpectedDatabase.Zero();
    iExpectedDatabase.Append(_L("file://"));
    iExpectedDatabase.Append(aDatabaseUri);
    }

// ---------------------------------------------------------
// CTP117ActiveLmEvent::CallNotifyDatabaseEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::CallNotifyDatabaseEvent()                          
    {
    iLog->Log(_L("CallNotifyDatabaseEvent"));
    iManager->NotifyDatabaseEvent(iEvent, iStatus);
    SetActive();
    }

// ---------------------------------------------------------
// CTP117ActiveLmEvent::CallDatabaseUriFromLastEventLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::CallDatabaseUriFromLastEventLC()
    {
    iLog->Log(_L("CallDatabaseUriFromLastEventLC"));
	TRAPD(err,
    		CleanupStack::PopAndDestroy(iManager->DatabaseUriFromLastEventLC()));
    	if (err !=KErrNone)
    		{
    		if (err == KErrNotFound)
    			{
    			iLog->Log(_L("DatabaseUriFromLastEventLC leaved with KErrNotFound"));
    			}
    		else
    			{
    			iLog->Log(_L("DatabaseUriFromLastEventLC leaved with error"));
    			}
    		}
    	else
    		{
    		// No error from second call to DatabaseUriFromLastEventLC
    	
    		}
    }    

// ---------------------------------------------------------
// CTP117ActiveLmEvent::Start
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::Start()                          
    {
    iLog->Log(_L("CTP117ActiveLmEvent::Start"));
    // Get ready for event
    iManager->NotifyDatabaseEvent(iEvent, iStatus);
    SetActive();
    }   

// ---------------------------------------------------------
// CTP117ActiveLmEvent::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::RunL()
    {  
	iLog->Log(_L("CTP117ActiveLmEvent::RunL"));
    VerifyEventL();
    }
    
// ---------------------------------------------------------
// CTP117ActiveLmEvent::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CTP117ActiveLmEvent::RunError(TInt aError)
    {  
	iLog->Log(_L("CTP117ActiveLmEvent::RunError"));
	TBuf<100> buf;
	buf.Format(_L("Errorcode in RunError: %d"), aError);
	iLog->Log(buf);
	iGlobalErr++;
    //Error in RunL
	Cancel();
    CActiveScheduler::Stop();
    return aError;
    }
    
// ---------------------------------------------------------
// CActiveLmOperation::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::DoCancel()
    {
    iLog->Log(_L("CTP117ActiveLmEvent::DoCancel"));
    iManager->CancelNotifyDatabaseEvent();
    iLog->Log(_L("CTP117ActiveLmEvent::DoCancel Done"));
    }
    
// ---------------------------------------------------------
// CActiveLmOperation::GetErrors
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CTP117ActiveLmEvent::GetErrors()
    {
	return iGlobalErr;
    }

// ---------------------------------------------------------
// CTP117ActiveLmEvent::VerifyEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CTP117ActiveLmEvent::VerifyEventL()
    {
    iLog->Log(_L("CTP117ActiveLmEvent::VerifyEvent"));
    
    TBuf<150> buffe;
    
    buffe.Format(_L("VerifyEvent Event: %d "), iEvent.iEventType);
	iLog->Log(buffe);
	
	    // Verify event here
    if (iEvent.iEventType != iExpectedEvent) 
        {
        buffe.Zero();
        buffe.Format(_L("ERROR: Wrong event: exp %d got %d"),iExpectedEvent, iEvent.iEventType);
        iLog->Log(buffe);
        iGlobalErr++;
        }
        
    // Only do check of DatabaseUriFromLastEventLC when event is differet from unknown
    // because when this event occurs the uri is not set
	if (iCheckUri && iEvent.iEventType != EPosLmDbUnknownEvent)	
		{
	    HBufC16* dbUri = iManager->DatabaseUriFromLastEventLC();
	    buffe.Zero();
	    buffe.Format(_L("Event: %d URI %S"), iEvent.iEventType, dbUri);
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
	    
	    // Call DatabaseUriFromLastEventLC again
	    TRAPD(err,
    		CleanupStack::PopAndDestroy(iManager->DatabaseUriFromLastEventLC()));
    	if (err !=KErrNone)
    		{
    		if (err == KErrNotFound)
    			{
    			iLog->Log(_L("Second call to DatabaseUriFromLastEventLC leaved with KErrNotFound"));
    			}
    		else
    			{
    			iLog->Log(_L("Second call to DatabaseUriFromLastEventLC leaved with error"));
    			}
    		}
    	else
    		{
    		// No error from second call to DatabaseUriFromLastEventLC
    		
    		}
		}
		else
		{
		iLog->Log(_L("URI was not checked"));	
		}
				
	CActiveScheduler::Stop();
   
	iLog->Log(_L("VerifyEvent Done"));
    }

//  End of File
