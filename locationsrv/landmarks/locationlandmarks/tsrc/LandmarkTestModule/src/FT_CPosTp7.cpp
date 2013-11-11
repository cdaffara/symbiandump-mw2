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
#include "FT_CPosTp7.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>  
#include <LbsPosition.h> 
#include <e32std.h>
//#include <ss_std.h>
#include "FT_LandmarkConstants.h"
        
const TInt KShortDelayMicroSeconds = 10000; // 0.01 sec

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp7::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp7::StartL()
    {      
    _LIT(KMultipleErr, "Error %d when multiple clients access the landmarks db");
    _LIT(KMultipleAddErr, "All Add request from multiple clients returns KErrLocked");
    _LIT(KExitErr, "Thread was panicked or is still alive");
   
    MakeSurePanicDebugFileExistsL();

    RemoveDefaultDbL();

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }
    CleanupStack::PopAndDestroy(lmd);
    
    iOneSucces=EFalse;
    TFixedArray<TRequestStatus, KNoMultipleClients> statuses;
    
    CreateThreadsL();

    iLog->Log(_L("Logon"));
    for (TInt i=0; i<iThreads.Count(); i++)
        { 
        iThreads[i].Logon(statuses[i]);
        iThreads[i].Resume();
        }
    
    iLog->Log(_L("WaitForRequest"));
    for (TInt j=0; j<iThreads.Count(); j++)
        {
         User::WaitForRequest(statuses[j]);
        }
    
    iLog->Log(_L("Checking Threads"));
    for (TInt t=0; t<iThreads.Count(); t++)
        {
        TInt exitReason = iThreads[t].ExitReason();
      
        AssertTrueSecL(exitReason == KErrNone, KMultipleErr, exitReason);
        AssertTrueSecL(iThreads[t].ExitType() == EExitKill, KExitErr);
        }
    
    AssertTrueSecL(iOneSucces, KMultipleAddErr); 
    }

// ---------------------------------------------------------
// CPosTp7::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp7::CloseTest()
    {
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();
  
    }

// ---------------------------------------------------------
// CPosTp7::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp7::RunTestL(TAny* aData)
    {
    CPosTp7* self = reinterpret_cast<CPosTp7*>(aData);
    TInt threadIndex = ++self->iThreadIndex;

    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       TInt err = KErrLocked;
       while ( ETrue )
           {
           TRAP( err, ExecuteAndDeleteLD(lmd->InitializeL()) ); 
           if ( !err )
               {
               break;
               }
           else if ( err != KErrLocked )
               {
               RDebug::Print( _L("TP7: thread %d, db init failed with %d"),  
                   threadIndex, err );
               User::Leave( err );
               }
           else
               {
               RDebug::Print( _L("TP7: thread %d, db locked, can't init"),  
                   threadIndex);
               }
           User::After( TTimeIntervalMicroSeconds32( KShortDelayMicroSeconds ) );
           }
       }
    
    for ( TInt i = 0; i < KNoAddRequests; i++ )
        {
        CPosLandmark* landmark = CPosLandmark::NewLC();
        
        _LIT(KLmName, "Thread%d Lm %d");
        TBuf<100> lmName;
        lmName.Format(KLmName, threadIndex, i);
        landmark->SetLandmarkNameL(lmName);
    
        RDebug::Print( _L("TP7: thread %d, adding lm %d"), threadIndex, i );
        TRAPD( err, lmd->AddLandmarkL( *landmark ) );
        if ( err != KErrLocked && err != KErrNone && err != KErrInUse )
            {
            RDebug::Print( _L("TP7: thread %d, add lm %d failed with %d"), threadIndex, i, err );
            User::Leave( err );
            }
        else if( err == KErrNone )
            {
            self->iOneSucces = ETrue; // At lest one add request should return KErrNone
            }
        else
            {
            RDebug::Print( _L("TP7: thread %d, db locked, can't add lm %d"), threadIndex, i );
            }

        CleanupStack::PopAndDestroy(landmark);    
        User::After( TTimeIntervalMicroSeconds32( KShortDelayMicroSeconds ) );
        }
    CleanupStack::PopAndDestroy(lmd);
    }


LOCAL_C TInt ThreadFunction(TAny* aData)
    {
    
    CTrapCleanup* cleanup=CTrapCleanup::New(); 

    CActiveScheduler* actSch = new (ELeave) CActiveScheduler;
    CActiveScheduler::Install(actSch);

    TRAPD(err, CPosTp7::RunTestL(aData));
    
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp7::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp7::CreateThreadsL()
    {
    iLog->Log(_L("CreateThreadsL"));
    _LIT(KThreadName, "TP7thread%d");
    _LIT(KCreateThreadErr, "Create thread failed with %d");

    for (TInt i=0; i<KNoMultipleClients; i++)
        {
        RThread thread;
        TBuf<32> name;
	    name.Format(KThreadName, i + 1);
                
        TInt err;
        err = thread.Create(name, ThreadFunction, KDefaultStackSize, KMinHeapSize, KMaxHeapSize, reinterpret_cast<TAny*>(this));

        AssertTrueSecL(err == KErrNone, KCreateThreadErr, err);                                                                  
        
        iThreads.Append(thread);
        }

    iLog->Log(_L("CreateThreadsL Done"));
    }
    
//  End of File
