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
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <EPos_CPosLandmark.h> 
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>    
#include "FT_CSearchResult.h"  
#include <LbsPosition.h> 
#include <e32std.h>
#include <ss_std.h>

#include "FT_CPosTp30.h"


#include "FT_LandmarkConstants.h"
//#include "FT_CActiveSearch.h"
        
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp30::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp30::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP30 - Multiple clients in search");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp30::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp30::CloseTest()
    {
    for (TInt i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }

    iThreads.Close();

    delete iDatabase;
    iDatabase=NULL;

    iUseLogFromThreadIsDisabled = EFalse;

    }

// ---------------------------------------------------------
// CPosTp30::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp30::StartL()
    {
    _LIT(KTimeErr, "The request time for several simultaneously requests is > n*'request time for one request'");
    MakeSurePanicDebugFileExistsL();

    iDatabase = UseGeneratedDbFileL();
    delete iDatabase;
    iDatabase=NULL;
    iUseLogFromThreadIsDisabled = ETrue;
    AppendSearchResultsL();
    iAsyncSearch = ETrue;
    TTime start, stop;
    start.UniversalTime();
    StartMultipleClientsL(1);
    stop.UniversalTime();
	
	TInt64 msecOne = (stop.Int64() - start.Int64())/1000;
	
    _LIT8(KTrace, "%d simultaneously search request(s) = %d mseconds");
	TBuf8<100> msg;
	msg.Format(KTrace, 1, msecOne);
    iLog->Put(msg);

    start.UniversalTime();
    StartMultipleClientsL(KNoMultipleClients);
    stop.UniversalTime();
	iAsyncSearch = EFalse;

	TInt64 msecSeveral = (stop.Int64() - start.Int64())/1000;
    msg.Format(KTrace, KNoMultipleClients, msecSeveral);
    iLog->Put(msg);

    AssertTrueSecL((msecOne*KNoMultipleClients) >= msecSeveral, KTimeErr);

    iLog->Put(_L("Testing simultaneously search syncronously and remove all syncronously"));

    iRemoveTest=ETrue;
    StartMultipleClientsL(3);
    iRemoveTest=EFalse;

    delete iDatabase;
    iDatabase = NULL;
    iDatabase = UseGeneratedDbFileL();
    
    iLog->Put(_L("Testing simultaneously search asyncronously"));
    iAsyncSearch =  ETrue;
    StartMultipleClientsL(KNoMultipleClients);

////

    iLog->Put(_L("Testing simultaneously search asyncronously and remove all syncronously"));
    iRemoveTest=ETrue;
    StartMultipleClientsL(3);
    
    }

// ---------------------------------------------------------
// CPosTp30::StartMultipleClientsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp30::StartMultipleClientsL(const TUint aNoClients)
    {
    _LIT(KMultipleErr, "Error %d when multiple clients access the landmarks db");
    _LIT(KPanicErr, "Thread has panicked or is still alive when multiple clients searched the databse");

    CreateThreadsL(aNoClients);

    RArray<TRequestStatus> statuses;
    CleanupClosePushL(statuses);
   
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
    
    for (i=0; i<iThreads.Count(); i++)
        {
        TInt exitReason = iThreads[i].ExitReason();
        
        AssertTrueSecL(exitReason == KErrNone, KMultipleErr, exitReason);
        AssertTrueSecL(iThreads[i].ExitType() == EExitKill, KPanicErr);
        }
    for (i=0; i<iThreads.Count(); i++)
        {
        iThreads[i].Close();
        }
    iThreadIndex=0;
    iThreads.Close();
    CleanupStack::PopAndDestroy(&statuses);
    }

// ---------------------------------------------------------
// CPosTp30::RunTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp30::RunTestL(TAny* aData)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a multiple search is performed");
    
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);
    
    if (lmd->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(lmd->InitializeL()); 
       }
    
    CPosTp30* self = reinterpret_cast<CPosTp30*>(aData);
    ++self->iThreadIndex;

    if (self->iRemoveTest && self->iThreadIndex == 3)
        {
        CPosLmOperation* lmo=NULL;
        TRAPD(err,lmo = lmd->RemoveAllLandmarksL());

        if (err != KErrNone && err != KErrLocked)
            {
            User::Leave(err);
            }
        else if(!err)
            {
            TRAP(err, ExecuteAndDeleteLD(lmo));
            if (err != KErrNone && err != KErrLocked)
                {
                User::Leave(err);
                }
            }
        }
    else
        {
        CPosLandmarkSearch* lms = CPosLandmarkSearch::NewL(*lmd);
        
        CleanupStack::PushL(lms);

        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
        textCriteria->SetTextL(self->iSearchResults[0]->TextCriteria());
        textCriteria->SetAttributesToSearch(self->iSearchResults[0]->Attributes());  
        textCriteria->SetPositionFieldsToSearchL(self->iSearchResults[0]->PositionFields()); 

        if (!self->iAsyncSearch)
            {
            ExecuteAndDeleteLD(lms->StartLandmarkSearchL(*textCriteria));
            }
        else
            {
            self->RunAsyncOperationLD(lms->StartLandmarkSearchL(*textCriteria));
            }
        
        CPosLmItemIterator* iter = lms->MatchIteratorL();
        CleanupStack::PushL(iter);

        self->AssertTrueSecL((TUint)(self->iSearchResults[0]->SearchResult()).Count() == lms->NumOfMatches(), KNumOfMatchesErr);      
        
        CleanupStack::PopAndDestroy(3, lms);    
        
        }

    CleanupStack::PopAndDestroy(lmd);
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

    TRAPD(err, CPosTp30::RunTestL(aData));
       
    delete actSch;
    delete cleanup;
    return err;
    }

// ---------------------------------------------------------
// CPosTp30::CreateThreadsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp30::CreateThreadsL(const TUint aNoThreads)
    {
    _LIT(KThreadName, "LM test thread%d");
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
    
//  End of File
