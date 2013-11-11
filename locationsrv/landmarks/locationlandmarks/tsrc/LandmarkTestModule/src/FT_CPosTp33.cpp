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

#include "FT_CPosTp33.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <ss_std.h>

//EPosLmServer
_LIT(KPosLandmarksServerName, "*eposlmserver*");
    
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp33::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp33 - LM Server shutdown");
    aName = KTestName;
    }

void CPosTp33::CloseTest()
    {
    delete iLmd1;
    iLmd1 = NULL;
    delete iLmd2;
    iLmd2 = NULL;
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();
    }


// ---------------------------------------------------------
// CPosTp33::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::StartL()
    {  
    MakeSurePanicDebugFileExistsL();
    RemoveDefaultDbL();
    
/*    ListAllServers(_L("*"));
	ListAllProcesses(_L("*"));
	ListAllThreads(_L("*"));
*/	
	// When running on security SDK, not possible to kill LM Server
    //KillTestL();
    
    RemoveDefaultDbL();

    iLmd1 = CPosLandmarkDatabase::OpenL();
    
    TBool alive = ServerAlive(KPosLandmarksServerName);
    
    MonitorHeapL(ETrue);

	ListAllServers(_L("*"));
	ListAllProcesses(_L("*"));
	ListAllThreads(_L("*"));
	
    alive = ServerAlive(KPosLandmarksServerName);
    if(!alive) LogErrorAndLeave(_L("Server not alive"));

    if (iLmd1->IsInitializingNeeded())
        {
        iLog -> Put(_L("Doing InitializeL()"));
        ExecuteAndDeleteLD(iLmd1->InitializeL()); 
        }

    MonitorHeapL(ETrue);
    
    alive = ServerAlive(KPosLandmarksServerName);
    if(!alive) LogErrorAndLeave(_L("Server not alive"));

    iLmd2 = CPosLandmarkDatabase::OpenL();

    alive = ServerAlive(KPosLandmarksServerName);
    if(!alive) LogErrorAndLeave(_L("Server not alive"));

    delete iLmd1;
    iLmd1 = NULL;
    User::After(4000000); //just to make sure that timing does not matter

    alive = ServerAlive(KPosLandmarksServerName);
    if(!alive) LogErrorAndLeave(_L("Server not alive"));

    delete iLmd2;
    iLmd2 = NULL;
    User::After(4000000);

    alive = ServerAlive(KPosLandmarksServerName);
    if(alive) LogErrorAndLeave(_L("Server alive"));

    //stress test below
    StressTestL();

    //thread test to verify that heap does not grow when threads are killed.
    ThreadTestL();
	}

// ---------------------------------------------------------
// CPosTp33::StressTestL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::StressTestL()
    {
    iLog -> Put(_L("StressTestL() -->"));
    for(TInt i = 0; i < 50; i++)
        {
        iLmd1 = CPosLandmarkDatabase::OpenL();
        iLmd2 = CPosLandmarkDatabase::OpenL();
        delete iLmd1;
        iLmd1 = NULL;
        delete iLmd2;
        iLmd2 = NULL;
        }
	//    MonitorHeapL();
	MonitorHeapL(ETrue);
    TBool alive = ServerAlive(KPosLandmarksServerName);
    if(!alive) LogErrorAndLeave(_L("Server not alive in StressTestL"));

    User::After(4000000);

    alive = ServerAlive(KPosLandmarksServerName);
    if(alive) LogErrorAndLeave(_L("Server alive in StressTestL"));
    iLog -> Put(_L("StressTestL() <--"));
    }
// ---------------------------------------------------------
// CPosTp33::ListAllServers
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::ListAllServers(const TDesC& aServerName)
    {
	TFindServer find(aServerName);
    TFullName   name;
    
    //TraceL(_L("ListAllServers"), aTraceFile);
    
    iLog->Put(_L("ListAllServers"));

	while (find.Next(name) == KErrNone)
		{
		//iLog->Put(name);
        //TraceL(name, aTraceFile);
        iLog->Put(name);

		}
	//TraceL(_L("--------------------"), aTraceFile);
	iLog->Put(_L("--------------------"));
    }

// ---------------------------------------------------------
// CPosTp33::ListAllProcesses
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::ListAllProcesses(const TDesC& aProcessName)
    {
	TFindProcess find(aProcessName);
    TFullName   name;
    
    iLog->Put(_L("ListAllProcesses"));
    //TraceL(_L("ListAllProcesses"), aTraceFile);

	while (find.Next(name) == KErrNone)
		{
		//iLog->Put(name);
        //TraceL(name, aTraceFile);
        iLog->Put(name);
		}
	//TraceL(_L("--------------------"), aTraceFile);
	iLog->Put(_L("--------------------"));
    }

// ---------------------------------------------------------
// CPosTp33::ListAllThreads
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::ListAllThreads(const TDesC& aThreadName)
    {
	TFindThread find(aThreadName);
    TFullName   name;

	iLog->Put(_L("ListAllThreads"));
    //TraceL(_L("ListAllThreads"), aTraceFile);
	while (find.Next(name) == KErrNone)
		{
		//TraceL(name, aTraceFile);
		iLog->Put(name);
		}
	//TraceL(_L("--------------------"), aTraceFile);
	iLog->Put(_L("--------------------"));
    }
    
// ---------------------------------------------------------
// CPosTp33::KillTestL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::KillTestL()
    {

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(lmd->InitializeL()); 
        }
    
	TFullName targetName;

    #ifndef __WINS__
	    TFindProcess finderLMSERV(KPosLandmarksServerName);
        RProcess process_or_thread_lmserv;    
    #else
        TFindThread finderLMSERV(KPosLandmarksServerName);
        RThread process_or_thread_lmserv;
    #endif
    
     if (finderLMSERV.Next(targetName) == KErrNone)
		{		
		process_or_thread_lmserv.Open(finderLMSERV);
		process_or_thread_lmserv.Kill(KErrGeneral);
		process_or_thread_lmserv.Close();
		}

    if (ServerAlive(KPosLandmarksServerName))
        {
        LogErrorAndLeave(_L("Server alive, error in TC!"));
        }

    CPosLandmark* landmark = CPosLandmark::NewLC();
        
    landmark->SetLandmarkNameL(_L("TP33")); 
    
    TRAPD(err, lmd->AddLandmarkL(*landmark));
    if (err != KErrServerTerminated)
        {
        LogErrorAndLeave(_L("AddLanmdark does not Leave with -15 after the server has been killed"));
        }

    TRAP(err, lmd->UpdateLandmarkL(*landmark));
    if (err != KErrServerTerminated)
        {
        LogErrorAndLeave(_L("UpdateLandmark does not Leave with -15 after the server has been killed"));
        }

    CleanupStack::PopAndDestroy(2, lmd); 
    }

// ---------------------------------------------------------
// CPosTp33::MonitorHeapL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::MonitorHeapL(TBool aMaxSet)
    {
    iLog->Put(_L("MonitorHeapL"));
    TInt landmarkHeap=0, landmarkHeapUsed=0;

    TBuf<150> buf;
    TInt err = GetHeapSizeL(KPosLandmarksServerName, landmarkHeap, landmarkHeapUsed);
    if (err != KErrNone)
        {
        buf.Zero();
        buf.Format(_L("GetHeapSizeL for Landmark server returned error, err %d"), err);
        iLog->Put(buf);
        }
    if (aMaxSet) 
        {
        iMaxHeap = 2*landmarkHeapUsed;
        }
    else
        {
        if(landmarkHeapUsed > iMaxHeap)
            {
            LogErrorAndLeave(_L("Used heap to large"));
            }
        }

    buf.Zero();
    buf.Format(_L("Landmark Heap; %d Landmark Heap Used: %d"), landmarkHeap, landmarkHeapUsed);
    iLog->Put(buf);
    }


// ---------------------------------------------------------
// CPosTp33::GetHeapSizeL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp33::GetHeapSizeL(const TDesC& aThreadName, TInt& aSize, TInt& aUsed)
    {
    RThread thread;
    TFindThread findThread(aThreadName);

    TFullName fullName;
    
    if (findThread.Next(fullName) != KErrNone)
        {
        iLog->Put(_L("Thread not found"));
        aSize=-1;aUsed=-1;
        return KErrNotFound;
        }

    TInt err = thread.Open(fullName);

    if (err != KErrNone)
        {
        iLog->Put(_L("Thread could not be opened"));
        aSize=-1;aUsed=-1;
        return err;
        }
    CleanupClosePushL(thread);

    //TInt tmp;

#ifndef __WINS__
// For some reason cannot access heap in target
//   thread.GetRamSizes(aSize,tmp);
//   aUsed = aSize;
    
    // FrBo Commented since not supported on ARMV5 (on SDK Series60_30_2005_wk02_incl_LBSpre4)
    //thread.GetRamSizes(aSize, aUsed);
    // Use below for now
    aSize=-1;aUsed=-1;
    
    TThreadStackInfo stackInfo;
    thread.StackInfo(stackInfo);
    
    aSize = stackInfo.iBase - stackInfo.iLimit;
    aUsed = aSize;

#else

    // FrBo Commented since not supported on WINSCW (on SDK Series60_30_2005_wk04_Mars)
    // Use below for now
    aSize=-1;aUsed=-1;
    
    TThreadStackInfo stackInfo;
    thread.StackInfo(stackInfo);
    
    aSize = stackInfo.iBase - stackInfo.iLimit;
    aUsed = aSize;
    
    /*
    // In WINS we can get both the size of the heap, and amount
    // of bytes actually used. This method Panics on ARMI
    RHeap* heap = NULL;
    heap = thread.Heap();
    if (heap == NULL)
        {
        iLog->Put(_L("Could not open heap."));
        aSize=-1;aUsed=-1;
        return KErrGeneral;
        LogErrorAndLeave(_L("Could not open heap."));
        }
    TInt tmp;
    aSize = heap->Size();
    aUsed = heap->Size() - (heap->Available(tmp));
    */

#endif
    CleanupStack::PopAndDestroy(&thread); // thread
    return KErrNone;
    }


void CPosTp33::ThreadTestL()
    {
    iLog -> Put(_L("ThreadTestL() -->"));
    StartMultipleClientsL(5);
    MonitorHeapL();
    User::After(4000000);
    TBool alive = ServerAlive(KPosLandmarksServerName);
    if(alive) LogErrorAndLeave(_L("Server alive"));
    iLog -> Put(_L("ThreadTestL() <--"));    
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

    TRAPD(err, CPosTp33::RunTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

void CPosTp33::StartMultipleClientsL(const TUint aNoClients)
    {
    _LIT(KMultipleErr, "Error %d from client %d when multiple clients access the landmarks db");
    _LIT(KPanicErr, "Thread exit with incorrect code");

    CreateThreadsL(aNoClients);

    RArray<TRequestStatus> statuses;
    CleanupClosePushL(statuses);
   
    for (TUint j=0; j<aNoClients; j++)
        {
        TRequestStatus status;
        statuses.Append(status);
        }

    TInt i=0;
    TInt errrorsFound = KErrNone;
    for (i=0; i<iThreads.Count(); i++)
        { 
        iThreads[i].Logon(statuses[i]);
        iThreads[i].Resume();
        // This prevents some weird CentralRepository alloc to accur, which 
        // caused the test to fail
        User::After(2000000);
        }
    
    for (i=0; i<iThreads.Count(); i++)
        {
        User::WaitForRequest(statuses[i]);
        }
    
    for (i=0; i<iThreads.Count(); i++)
        {

        iLog -> Put(_L("Checking Thread"));
        TInt exitReason = iThreads[i].ExitReason();
        
        //AssertTrueSecL(exitReason == KErrNone || exitReason == KErrNotFound, KMultipleErr, exitReason);
        if (exitReason != KErrNone && exitReason != KErrNotFound)
	        {
	        TBuf<100> buf;
	        buf.Format(KMultipleErr, exitReason, i);
	        iLog->Put(buf);
	        errrorsFound++;
	        }

#ifndef __WINS__ 
        AssertTrueSecL(iThreads[i].ExitType() == EExitPanic, KPanicErr); //does not currently work use exit on wins instead
#else
        AssertTrueSecL(iThreads[i].ExitType() == EExitKill, KPanicErr); 
#endif
        }
    for (i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }
    iThreadIndex=0;
    iThreads.Close();
    CleanupStack::PopAndDestroy(&statuses);
    
    if (errrorsFound != KErrNone) LogErrorAndLeave(_L("Error from thread client"));
    }

// ---------------------------------------------------------
// CPosTp33::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::CreateThreadsL(const TUint aNoThreads)
    {
    _LIT(KThreadName, "LM TP33 test thread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

    for (TUint i=0; i<aNoThreads; i++)
        {
        RThread thread;
        TBuf<32> name;
	    name.Format(KThreadName, 1+iThreads.Count());
                
        TInt err;
        err = thread.Create(name, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, reinterpret_cast<TAny*>(this));

        AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);
        
        iThreads.Append(thread);
        }
    }


// ---------------------------------------------------------
// CPosTp30::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp33::RunTestL(TAny* /*aData*/)
    {
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }

    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd); 
    CleanupStack::PushL(cm);

    TRequestStatus status;
    TPosLmEvent event;
    lmd->NotifyDatabaseEvent(event, status); 

    //XXX fix this when dialogs works again
#ifndef __WINS__ 
    User::Panic(_L("Panic before cleanup"),-1); //does not currently work use exit on wins instead
#else
    User::Exit(-1);
#endif

    lmd->CancelNotifyDatabaseEvent();
    CleanupStack::PopAndDestroy(2, lmd);
    
    }

//  End of File
