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
#include <e32std.h>
#include <ss_std.h>

#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLmDatabaseManager.h>

#include "FT_CPosTp106.h"

// Nr of simultaneous clients (threads)
const TInt KNoMultipleClients = 5;

// Nr of created dbs
const TInt KNrOfDBs=25;
const TInt KCheckIfExistDBs=10;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp106::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();

    delete iDatabase;
    iDatabase=NULL;

    iUseLogFromThreadIsDisabled = EFalse;
    
    TRAPD(err, RemoveAllLmDatabasesL());
    if (err != KErrNone) iLog->Log(_L("Error when deleting all lm dbs"));

    }

// ---------------------------------------------------------
// CPosTp106::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::StartL()
    {
    MakeSurePanicDebugFileExistsL();
           
    RemoveAllLmDatabasesL();
    
    iUseLogFromThreadIsDisabled = ETrue;
	
	TBuf<150> buf;
	
	_LIT(KTrace1, "Create %d x %d (=%d) nr of dbs took = %d msec");
	_LIT(KTrace2, "Copy %d x %d (=%d) nr of dbs took = %d msec");
	_LIT(KTrace3, "Get and Set default URI for %d x %d (=%d) nr of dbs took = %d msec");
	_LIT(KTrace4, "Get display name for %d x %d (=%d) nr of dbs took = %d msec");
	_LIT(KTrace5, "Check if db exist for %d x %d (=%d) nr of dbs took = %d msec");
	_LIT(KTrace6, "Delete %d x %d (=%d) nr of dbs took = %d msec");
	
    iTestStep=Step1;
    StartMultipleClientsL(KNoMultipleClients);
    TInt64 msec = (iStop.Int64() - iStart.Int64())/1000;
   	buf.Format(KTrace1, KNrOfDBs, KNoMultipleClients, KNrOfDBs*KNoMultipleClients, msec);
    iLog->Log(buf);
    
    iTestStep=Step2;
    StartMultipleClientsL(KNoMultipleClients);
    msec = (iStop.Int64() - iStart.Int64())/1000;
   	buf.Format(KTrace2, KNrOfDBs, KNoMultipleClients, KNrOfDBs*KNoMultipleClients, msec);
    iLog->Log(buf);
    
    iTestStep=Step3;
    StartMultipleClientsL(KNoMultipleClients);
    msec = (iStop.Int64() - iStart.Int64())/1000;
   	buf.Format(KTrace3, KNrOfDBs, KNoMultipleClients, KNrOfDBs*KNoMultipleClients, msec);
    iLog->Log(buf);
    
    iTestStep=Step4;
    StartMultipleClientsL(KNoMultipleClients);
    msec = (iStop.Int64() - iStart.Int64())/1000;
   	buf.Format(KTrace4, KNrOfDBs, KNoMultipleClients, KNrOfDBs*KNoMultipleClients, msec);
    iLog->Log(buf);
    
    iTestStep=Step5;
    StartMultipleClientsL(KNoMultipleClients);
    msec = (iStop.Int64() - iStart.Int64())/1000;
   	buf.Format(KTrace5, KNrOfDBs, KNoMultipleClients, KNrOfDBs*KNoMultipleClients, msec);
    iLog->Log(buf);
   
    iTestStep=Step6;
    StartMultipleClientsL(KNoMultipleClients);
    msec = (iStop.Int64() - iStart.Int64())/1000;
   	buf.Format(KTrace6, KNrOfDBs, KNoMultipleClients, KNrOfDBs*KNoMultipleClients, msec);
    iLog->Log(buf);                
    
    }

// ---------------------------------------------------------
// CPosTp106::StartMultipleClientsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::StartMultipleClientsL(const TUint aNoClients)
    {
    _LIT(KMultipleErr, "Error %d from thread %d");
    _LIT(KPanicErr, "Thread %d has panicked or is alive");
    _LIT(KSuccess, "Thread %d is successful");
      
    CreateThreadsL(aNoClients);

    RArray<TRequestStatus> statuses;
    CleanupClosePushL(statuses);
   	
   	// Start measuring time
   	iStart.UniversalTime();
   	
    for (TUint j=0; j<aNoClients; j++)
        {
        TRequestStatus status;
        statuses.Append(status);
        }

    TInt i=0; 
    for (i=0; i<iThreads.Count(); i++)
        { 
        iThreads[i].Logon(statuses[i]);
        iThreads[i].Resume();
        }
    
    for (i=0; i<iThreads.Count(); i++)
        {
        User::WaitForRequest(statuses[i]);
        }
    
    TInt errors=0; 
    for (i=0; i<iThreads.Count(); i++)
        {
        TInt exitReason = iThreads[i].ExitReason();
        TBuf<100> info;
            
        if (exitReason != KErrNone)
            {
            errors++;
            info.Format(KMultipleErr, exitReason, i+1);
            }  
         else
            {
            info.Format(KSuccess, i+1);
            }
        iLog->Log(info);

        AssertTrueSecL(iThreads[i].ExitType() == EExitKill, KPanicErr, i+1);
        AssertTrueSecL(exitReason == KErrNone || exitReason == KErrLocked, info);
        }
        
    // Stop measuring time
	iStop.UniversalTime();
/*        
    if (iTestStep != EReadLockTaken)
        {
        AssertTrueSecL(errors == KNoMultipleClients - 1, KAllErr);        
        }
    else
        {
        // All imports should have failed since the started search should have taken a read lock
        AssertTrueSecL(errors == KNoMultipleClients, KAllNotFailedErr);        
        }
       */
    
    for (i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }
    
    iThreadIndex=0;
    iThreads.Close();
    CleanupStack::PopAndDestroy(&statuses);
    }

// ---------------------------------------------------------
// CPosTp106::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::RunTestL(TAny* aData)
    {
    CPosTp106* self = reinterpret_cast<CPosTp106*>(aData);
    TInt index = ++self->iThreadIndex;
   
   	TBuf<100> info;
    _LIT(KInfo2, "Thread %d before execute");
    info.Format(KInfo2, index);
    RDebug::Print(info);

    switch (self->iTestStep)
        {
        case Step1:
        	self->CreateAndListDatabasesL(index);
        break;
        case Step2:
        	self->CopyDatabasesL(index);
        break;
        case Step3:
        	self->SetAndGetUriL(index);
        break;
        case Step4:
        	self->GetDisplayNameL();
        break;
        case Step5:
        	self->CheckIfExistL(index);
        break;
        case Step6:
        	self->DeleteDatabasesL();
        break;                
        }
    
    _LIT(KInfo3, "Thread %d after execute");
    info.Format(KInfo3, index);
    RDebug::Print(info);
    }

// ---------------------------------------------------------
// CPosTp106::CreateAndListDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::CreateAndListDatabasesL(TInt index)
    {
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    _LIT(KLmName, "Thread_%d_LmDbIndex%d.ldb");
    _LIT(KLmURI, "file://C:Thread_%d_LmDbIndex%d.ldb");
    TBuf<50> lmDbName;
    TBuf<50> lmDbUri;
    TInt i=0;
    TInt NrOfFoundDbs=0;
    // Create KNrOfDBs nr of  landmark dbs
    
    HPosLmDatabaseInfo* dbInfo = NULL;
    for (i=0;i<KNrOfDBs;i++)
    	{
    	lmDbName.Zero();
    	lmDbName.Format(KLmName, index, i);
    	dbInfo = HPosLmDatabaseInfo::NewLC(lmDbName);
    	manager->CreateDatabaseL(*dbInfo);
    	CleanupStack::PopAndDestroy(dbInfo);
    	/*
    	This increases the filesize to much
    	CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(lmDbName);
    	CleanupStack::PushL(db);
    	
    	if (db->IsInitializingNeeded())
        	{
        	ExecuteAndDeleteLD(db->InitializeL());
        	}
    	CleanupStack::PopAndDestroy(db);
    	*/
    	}
    
    // List all dbs    
    CDesCArray* dbArray = manager->ListDatabasesLC();
    TInt nr = dbArray->Count();
	
	// Check that at least KNrOfDBs nr of dbs exist
	if (nr < KNrOfDBs) User::Panic(_L("Wrong Nr"), -666);
	
	// Check that all created dbs are listed
	for (TInt j=0;j<KNrOfDBs;j++)
		{
		lmDbUri.Zero();
		lmDbUri.Format(KLmURI, index, j);
		for (i=0;i<nr;i++)
			{
			if ((*dbArray)[i].CompareC(lmDbUri) == KErrNone)
				{
				// Found
				i=nr;
				NrOfFoundDbs++;
				}
			}
		}
    
    if (NrOfFoundDbs != KNrOfDBs) User::Panic(_L("Wrong Nr"), -667);
    
    CleanupStack::PopAndDestroy(dbArray);
    CleanupStack::PopAndDestroy(manager);
    }
    
// ---------------------------------------------------------
// CPosTp106::CopyDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::CopyDatabasesL(TInt index)
    {
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    _LIT(KLmName, "Thread_%d_LmDbIndex%d.ldb");
    _LIT(KLmNameCopy, "Thread_%d_LmDbIndex%d_Copy.ldb");
    _LIT(KLmURICopy, "file://C:Thread_%d_LmDbIndex%d_Copy.ldb");
    TBuf<50> lmDbName;
    TBuf<50> lmDbNameCopy;
    TBuf<50> lmDbUriCopy;
    TInt i=0;
    TInt NrOfFoundDbs=0;
    // Create KNrOfDBs nr of  landmark dbs
    for (i=0;i<KNrOfDBs;i++)
    	{
    	lmDbName.Zero();
    	lmDbName.Format(KLmName, index, i);
    	
    	lmDbNameCopy.Zero();
    	lmDbNameCopy.Format(KLmNameCopy, index, i);
    	manager->CopyDatabaseL(lmDbName, lmDbNameCopy);
    	}
    
    // List all dbs
    CDesCArray* dbArray = manager->ListDatabasesLC();
    TInt nr = dbArray->Count();
	
	// Check that at least all copied dbs are listed
	for (TInt j=0;j<KNrOfDBs;j++)
		{
		lmDbUriCopy.Zero();
		lmDbUriCopy.Format(KLmURICopy, index, j);
		for (i=0;i<nr;i++)
			{
			if ((*dbArray)[i].Compare(lmDbUriCopy) == KErrNone)
				{
				// Found
				i=nr;
				NrOfFoundDbs++;
				}
			}
		}
    
    if (NrOfFoundDbs != KNrOfDBs) User::Panic(_L("WrongNrOfCopiedDb"), -667);
    
    CleanupStack::PopAndDestroy(dbArray);
    CleanupStack::PopAndDestroy(manager);
    }
     
// ---------------------------------------------------------
// CPosTp106::DeleteDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::DeleteDatabasesL()
    {
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // _LIT(KLmURICopy, "file://C:Thread_%d_LmDbIndex%d_Copy.LDB");
    // _LIT(KLmURI, "file://C:THREAD_%d_LMDBINDEX%d.LDB");
    
    TBuf<50> lmDbUri;
    TBuf<50> lmDbUriCopy;
	
	CDesCArray* dbArray = manager->ListDatabasesLC();
    TInt nr = dbArray->Count();
    
    // Delete all created dbs and copied dbs
	for (TInt j=0;j<nr;j++)
		{
		TRAPD(err, manager->DeleteDatabaseL((*dbArray)[j]));
		}
    
    /*
	// Delete all created dbs and copied dbs
	for (TInt j=0;j<KNrOfDBs;j++)
		{
		lmDbUriCopy.Zero();
		lmDbUri.Zero();
		lmDbUriCopy.Format(KLmURICopy, index, j);
		lmDbUri.Format(KLmURI, index, j);
		
		manager->DeleteDatabaseL(lmDbUriCopy);
		manager->DeleteDatabaseL(lmDbUri);
		}
		*/
    
    CleanupStack::PopAndDestroy(dbArray);
    CleanupStack::PopAndDestroy(manager);
    }

// ---------------------------------------------------------
// CPosTp106::SetAndGetUriL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::SetAndGetUriL(TInt /*index*/)
    {
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    _LIT(KLmURI, "file://C:Thread_%d_LmDbIndex%d.ldb");
    
    // Try to set (and get) defaultdb to dbs created by first thread
    TBuf<50> name;

	// index = 0;
	for (TInt j=0;j<KNrOfDBs;j++)
		{
		name.Zero();
		// Use first thread index 1
		name.Format(KLmURI, 1, j);
		manager->SetDefaultDatabaseUriL(name);
		
		HBufC* defaultName = manager->DefaultDatabaseUriLC();
		
		// TBD Maybe some other client has succeeded in changing the default name already
		if (defaultName->Compare(name) != KErrNone)
			{
			User::Panic(_L("WrongDefaultDb"), -671);
			}
		CleanupStack::PopAndDestroy(defaultName);	
		}
    CleanupStack::PopAndDestroy(manager);
    }

// ---------------------------------------------------------
// CPosTp106::GetDisplayNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::GetDisplayNameL()
    {
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
		
	// Get display name for default db
	for (TInt j=0;j<KNrOfDBs;j++)
		{	
		HBufC* dbUri = manager->DefaultDatabaseUriLC();
		HPosLmDatabaseInfo* dbInfo = HPosLmDatabaseInfo::NewLC(dbUri->Des());
		TPosLmDatabaseSettings settings = dbInfo->Settings();
		TPtrC displayName = settings.DatabaseName();
		CleanupStack::PopAndDestroy(2);
		}
		
	CleanupStack::PopAndDestroy(manager);
    }

// ---------------------------------------------------------
// CPosTp106::CheckIfExistL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::CheckIfExistL(TInt index)
    {
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    _LIT(KLmURICopy, "file://C:Thread_%d_LmDbIndex%d_Copy.ldb");
    _LIT(KLmURI, "file://C:Thread_%d_LmDbIndex%d.ldb");
    
    _LIT(KLmNonExistingURI, "file://C:THREAD_%d_PELLE_%d.LDB");
    
    TBuf<50> lmDbUri;
    TBuf<50> lmDbUriCopy;
        TBuf<50> lmDbNonExistingUri;

	TInt j=0;
	// Check that some created dbs and copied dbs exist
	// Check that non existing db does not exist
	
	// Check files created by first thread only
	index=1;
	
	for (j=0;j<KCheckIfExistDBs;j++)
		{
		lmDbUriCopy.Zero();
		lmDbUri.Zero();
		lmDbUriCopy.Format(KLmURICopy, index, j);
		lmDbUri.Format(KLmURI, index, j);
		
		lmDbNonExistingUri.Zero();
		lmDbNonExistingUri.Format(KLmNonExistingURI, index, j);
		
		// Should exist
		TBool res = manager->DatabaseExistsL(lmDbUriCopy);
		if (!res) User::Panic(_L("DbNotExist"), -668);
		
		// Should exist
		res = manager->DatabaseExistsL(lmDbUri);
		if (!res) User::Panic(_L("DbNotExist"), -669);
		
		// Should NOT exist
		res = manager->DatabaseExistsL(lmDbNonExistingUri);
		if (res) User::Panic(_L("DbExist"), -670);
		}
    
    CleanupStack::PopAndDestroy(manager);
    }
    
// ---------------------------------------------------------
// ThreadFunction
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp106::RunTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp106::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp106::CreateThreadsL(const TUint aNoThreads)
    {
    _LIT(KThreadName, "Tp106testthread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

    for (TUint i=0; i<aNoThreads; i++)
        {
        RThread thread;
        TBuf<32> name;
	    name.Format(KThreadName, 1 + iThreadNum++);
                
        TInt err;
        err = thread.Create(name, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, reinterpret_cast<TAny*>(this));

        AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);                                                                  
        
        iThreads.Append(thread);
        }
    }
        
//  End of File
