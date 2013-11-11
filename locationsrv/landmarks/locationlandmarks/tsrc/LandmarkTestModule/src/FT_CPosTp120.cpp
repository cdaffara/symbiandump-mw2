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
#include "FT_CPosTp120.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLmAreaCriteria.h>
#include <EPos_CPosLMItemIterator.h>    
#include <EPos_TPosLMSortPref.h>
     
// CONSTANTS  
const TInt KNrOfDbs = 5;
const TInt KLmPerDb = 20;
const TInt KTotLms = 105;

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp120::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;
    
    delete iDbArray;

    delete iLmMultiSearch;
    iLmMultiSearch = NULL;
    
    iAreaSearchResults.ResetAndDestroy();
    
    REComSession::FinalClose();
    }

// ---------------------------------------------------------
// CPosTp120::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::StartL()
    {
    // Split databases, NrOfDBs db, LmPerDb lm in each, TotLms is last lm id from original db.
    // Last param is to enable/disable debug
    iDbArray = CreateDatabasesL(KNrOfDbs, KLmPerDb, KTotLms); //, ETrue);    
       
    // Constructing a lm multi search 
    iLmMultiSearch = CPosLmMultiDbSearch::NewL(*iDbArray);

    AppendAreaSearchResultsL();
            
    iLog->Log(_L("Testing search with criterias read from LmDbAreaSearchResult.txt syncronously"));
    SearchL(iAreaSearchResults, ESynchronous);

    iLog->Log(_L("Testing search with criterias read from LmDbAreaSearchResult.txt asyncronously"));
    SearchL(iAreaSearchResults, EAsynchronous);

    
    iLog->Log(_L("Testing search with a defined sortorder"));
    SearchWithSortOrderL(iAreaSearchResults);   

    iLog->Log(_L("Testing search and cancel"));
    SearchAndCancelL();
    

    iLog->Log(_L("Testing maximum number of matches"));
    SearchWithMaximumL();
    
    iLog->Log(_L("Testing searching on the fly"));
    OnTheFlySearchL();  
    
    iLog->Log(_L("Testing a redefined search after landmarks have been renamed"));
    SearchAndRenameL();

    iLog->Log(_L("Testing a redefined search after a landmark has been removed"));
    SearchAndDeleteL();

    }

// ---------------------------------------------------------
// CPosTp120::AreaCriteriaL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLmAreaCriteria* CPosTp120::AreaCriteriaL(const TReal64 &aSouthLat, 
                                             const TReal64 &aNorthLat,
                                             const TReal64 &aWestLong, 
                                             const TReal64 &aEastLong) 
    {
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(aSouthLat, 
                                                                aNorthLat, 
                                                                aWestLong, 
                                                                aEastLong);
    CleanupStack::Pop();
    return areaCriteria;
    }

// ---------------------------------------------------------
// CPosTp120::CheckDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::CheckDatabasesL(
    CDesCArray* aExpectedLmUris,
    TUint aExpectedMaxMatches)
    {
    CDesCArray* dbToSearch = iLmMultiSearch->DatabasesToSearchL();
    CleanupStack::PushL(dbToSearch);
    
    _LIT(KWrongNrDatabases, "Wrong number of databases to search!");
    AssertTrueSecL(aExpectedLmUris != dbToSearch, KWrongNrDatabases);
    AssertTrueSecL(aExpectedLmUris->Count() == dbToSearch->Count(), KWrongNrDatabases);
    CleanupStack::PopAndDestroy(dbToSearch);
    
    AssertTrueSecL(aExpectedLmUris->Count() == iLmMultiSearch->NumOfDatabasesToSearch(), 
        KWrongNrDatabases);
    for (TInt i=0; i<aExpectedLmUris->Count(); i++)
        {
        AssertTrueSecL(aExpectedLmUris->MdcaPoint(i) == iLmMultiSearch->DatabaseUriPtr(i), 
            KWrongNrDatabases);        
        }
    
    _LIT(KWrongNrMatches, "Wrong number of max matches!");
    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == aExpectedMaxMatches, 
        KWrongNrMatches);        
    }

// ---------------------------------------------------------
// CPosTp120::GetLmIdsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::GetLmIdsL(
    TInt aDbId, 
    RArray<TPosLmItemId> aSearchResults,
    RArray<TPosLmItemId>& aLmIdForADb)
    {
    TInt startId = aDbId * KLmPerDb + 1;
    TInt stopId = 0;
    
    if (aDbId == KNrOfDbs - 1)
        {
        stopId = KTotLms;            
        }
    else 
        {
        stopId = (aDbId+1) * KLmPerDb;            
        }
    
    for (TInt i=0; i<aSearchResults.Count(); i++)
        {
        if (aSearchResults[i] >= startId && aSearchResults[i] <= stopId)
            {
            User::LeaveIfError(aLmIdForADb.Append(aSearchResults[i]));
            }
        }
    }
    
// ---------------------------------------------------------
// CPosTp120::CheckEqualsLmsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::CheckEqualsLmsL(
    TPosLmItemId aSource, 
    TPosLmItemId aTarget)
    {
    _LIT(KSortorderErr, "The sortorder is incorrect");

    if (aSource != aTarget)
        {                
        // Check if same name then don't Leave
        CPosLandmark* source = iDatabase->ReadLandmarkLC(aSource);
        CPosLandmark* target = iDatabase->ReadLandmarkLC(aTarget);
        TPtrC sourceName, targetName;
        source->GetLandmarkName(sourceName);
        target->GetLandmarkName(targetName);

        if (EFalse)
            {
            // For debugging purposes
            TBuf<100> buf;
            buf.Append(_L("Name:"));
            buf.Append(sourceName);
            buf.Append(targetName);
            iLog->Log(buf);
            }

        AssertTrueSecL(!sourceName.CompareC(targetName), KSortorderErr, aSource);
        CleanupStack::PopAndDestroy(2, source); // target, source
        }
    }

// ---------------------------------------------------------
// CPosTp120::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::SearchL(
    const RPointerArray<CAreaSearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");
    TBuf<255> info;

    for(TInt i=0; i<aSearchResults.Count(); i++)
        {   
        CPosLmAreaCriteria* areaCriteria = NULL;
        TRAPD(err, areaCriteria = CPosTp120::AreaCriteriaL(aSearchResults[i]->SouthLat(),
                                                aSearchResults[i]->NorthLat(),
                                                aSearchResults[i]->WestLong(),
                                                aSearchResults[i]->EastLong()));
        CleanupStack::PushL(areaCriteria);
        
        if (err != aSearchResults[i]->ErrorCode())
            {
            iLog->Log(_L("Unexpected error code returned"));    
            iLog->Log(_L("Area Search result read from file:"));
            
            aSearchResults[i]->Print(info);
            iLog->Log(info);

            _LIT(KReturned, "Search returned errorCode = %d");
            info.Format(KReturned, err);
            iLog->Log(info);
            User::Leave(KErrGeneral);
            }
            
        if (!err)
            {
            CPosLmOperation* operation = 
                iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, aSearchResults[i]->Redefined());
            CleanupStack::PushL(operation);
             switch (aExecutionMode)
                {
                case ESynchronous:
                    ExecuteAndDeleteLD(operation);
                    CleanupStack::Pop(operation);
                    break;
                case EAsynchronous:
                    RunAsyncOperationLD(operation);
                    CleanupStack::Pop(operation);
                    break;
                case EWaitForRequest:
                    RunAsyncOperationByWaitForReqL(operation);
                    CleanupStack::PopAndDestroy(operation);                
                    break;
                }

            // Check database array
            CheckDatabasesL(iDbArray);

            // Check for errors        
            AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
                _L("Error(s) found during multi search."));

            // Check total number of matches       
            if ((TUint)(aSearchResults[i]->SearchResult()).Count() != 
                iLmMultiSearch->TotalNumOfMatches())
                {
                _LIT(KExpected, "Expected no. of matches: %d");
                _LIT(KReturned, "Returned no. of matches: %d");
                TBuf<100> info;
                info.Format(KExpected, (aSearchResults[i]->SearchResult()).Count());
                iLog->Log(info);
                info.Format(KReturned, iLmMultiSearch->TotalNumOfMatches());
                iLog->Log(info);
               
                iLog->Log(KNumOfMatchesErr);
                User::Leave(-1);
                }

            RArray<TPosLmItemId> lmids;
            CleanupClosePushL(lmids);

            for (TInt j=0; j<iLmMultiSearch->NumOfDatabasesToSearch(); j++) 
                {
                CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(j);
                // Check if there are any matches = iteration exists.
                if (iter) 
                    {
                    CleanupStack::PushL(iter);
                    TPosLmItemId id = iter->NextL();    
                    while (id != KPosLmNullItemId)
                        {
                        // Check that found id is expected
                        AssertTrueSecL(aSearchResults[i]->FindSearchResult(id), KNotFoundErr);
                        User::LeaveIfError(lmids.Append(id));                              
                        id = iter->NextL();
                        }
                    CleanupStack::PopAndDestroy(iter);                
                    }
                else 
                    {
                    _LIT(KIterNull, "MatchIterator returned null for a db");
                    iLog->Log(KIterNull);
                    }
                }

            AssertTrueSecL(aSearchResults[i]->SearchResult().Count() == lmids.Count(), 
                _L("Expected number of hits and found hits don't match"));

            for (TInt k=0; k<lmids.Count(); k++)
                {
                // Check that found id is expected from all ids
                AssertTrueSecL(aSearchResults[i]->FindSearchResult(lmids[k]), KNotFoundErr);
                }

            CleanupStack::PopAndDestroy(&lmids); // lmids
            }
            
        CleanupStack::PopAndDestroy(areaCriteria);

        _LIT(KSearchNr, "Multi search test index #%d executed ok.");
        TBuf<100> buf;
        buf.Format(KSearchNr, i);        
        iLog->Log(buf);
        }
    }

// ---------------------------------------------------------
// CPosTp120::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::SearchWithSortOrderL(const RPointerArray<CAreaSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed syncronously");
   
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
 
    for (TInt j=0; j<aSearchResults.Count(); j++)
        {   
        CPosLmAreaCriteria* areaCriteria = NULL;
        TRAPD(err, areaCriteria = AreaCriteriaL(
                    iAreaSearchResults[j]->SouthLat(),
                    iAreaSearchResults[j]->NorthLat(),
                    iAreaSearchResults[j]->WestLong(),
                    iAreaSearchResults[j]->EastLong()));
        
        CleanupStack::PushL(areaCriteria);
        
        if (err != KErrNone)
            {
            CleanupStack::PopAndDestroy(areaCriteria);
            continue;
            }
            
               
        // 5 and 6 is using WaitForRequest
        for (TInt i=0; i<4; i++)
            {
            switch (i)
                {
                //**** sync
                case 0:
                    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(
                        *areaCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 1:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(
                        *areaCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 2:
                //**** async
                    RunAsyncOperationLD(iLmMultiSearch->StartLandmarkSearchL(
                        *areaCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 3:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    RunAsyncOperationLD(iLmMultiSearch->StartLandmarkSearchL(
                        *areaCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                //**** asych with User::WaitForRequest()
                case 4:
                    {
                    CPosLmOperation* op = iLmMultiSearch->StartLandmarkSearchL(
                        *areaCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined());
                    CleanupStack::PushL(op);
                    RunAsyncOperationByWaitForReqL(op);
                    CleanupStack::PopAndDestroy(op);
                    }
                    break;
                case 5:
                    {
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    CPosLmOperation* op = iLmMultiSearch->StartLandmarkSearchL(
                        *areaCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined());
                    CleanupStack::PushL(op);
                    RunAsyncOperationByWaitForReqL(op);
                    CleanupStack::PopAndDestroy(op);
                    }
                    break;
                }
            }
           
        // Check database array
        CheckDatabasesL(iDbArray);

        // Check for errors        
        AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
            _L("Error(s) found during multi search."));

        // Check total number of matches       
        if ((TUint)(aSearchResults[j]->SearchResult()).Count() != 
            iLmMultiSearch->TotalNumOfMatches())
            {
            _LIT(KExpected, "Expected no. of matches: %d");
            _LIT(KReturned, "Returned no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, (aSearchResults[j]->SearchResult()).Count());
            iLog->Log(info);
            info.Format(KReturned, iLmMultiSearch->TotalNumOfMatches());
            iLog->Log(info);
            
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }
        
        RArray<TPosLmItemId>* searchResults = &aSearchResults[j]->SearchResult();        
        for (TInt n=0; n<iLmMultiSearch->NumOfDatabasesToSearch(); n++) 
            {
            RArray<TPosLmItemId> lmids;
            CleanupClosePushL(lmids);
            GetLmIdsL(n, *searchResults, lmids);            

            iDatabase = CPosLandmarkDatabase::OpenL(iLmMultiSearch->DatabaseUriPtr(n));
            if (iDatabase->IsInitializingNeeded())
           {
           ExecuteAndDeleteLD(iDatabase->InitializeL());
           }

            LandmarksSortL(lmids, sortPref);
        
            CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(n);
            // Check if there are any matches = iteration exists.
            if (iter) 
                {
                CleanupStack::PushL(iter);
                TInt lmid = 0;
                TPosLmItemId id = iter->NextL();
        
                while (id != KPosLmNullItemId)
                    {
                    CheckEqualsLmsL(id, lmids[lmid]);
                    
                    id = iter->NextL();
                    ++lmid;
                    }
                CleanupStack::PopAndDestroy(iter);
                }
            else 
                {
                _LIT(KIterNull, "MatchIterator returned null for a db");
                iLog->Log(KIterNull);
                }
                
            delete iDatabase;
            iDatabase = NULL;
            
            CleanupStack::PopAndDestroy(&lmids);
            }
                   
        CleanupStack::PopAndDestroy(areaCriteria);
        
        _LIT(KSearchNr, "Multi search test index #%d executed ok.");
        TBuf<100> buf;
        buf.Format(KSearchNr, j);        
        iLog->Log(buf);
        }
    }

// ---------------------------------------------------------
// CPosTp120::OnTheFlySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::OnTheFlySearchL()
    {
    const TInt KTestIndex = 4;

   _LIT(KTestNr, "Multi search test, using test index %d.");
    TBuf<100> buf;
    buf.Format(KTestNr, KTestIndex);
    iLog->Log(buf);

           
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[KTestIndex]->SouthLat(),
                    iAreaSearchResults[KTestIndex]->NorthLat(),
                    iAreaSearchResults[KTestIndex]->WestLong(),
                    iAreaSearchResults[KTestIndex]->EastLong());
                        
    COnTheFlyTester* onTheFlyTester = COnTheFlyTester::NewLC();
    onTheFlyTester->StartSearchCampaignL(iLmMultiSearch, 
        iAreaSearchResults[KTestIndex]->SearchResult(), 
        areaCriteria);

    TInt result = onTheFlyTester->Err();
    if (result != KErrNone)
        {
        TPtrC msg;
        onTheFlyTester->GetErrMsg(msg);
        iLog->Log(msg);
        User::Leave(result);
        }
        
    CleanupStack::PopAndDestroy(2, areaCriteria);
    
    iLmMultiSearch->UnsetDisplayData();
    }

// ---------------------------------------------------------
// CPosTp120::SearchAndDeleteL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::SearchAndDeleteL()
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a redefined search is performed"); 
    const TInt KTestIndex = 4;
    
   _LIT(KTestNr, "Multi search test, using test index %d.");
    TBuf<100> buf;
    buf.Format(KTestNr, KTestIndex);
    iLog->Log(buf);

    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    
    CPosLmAreaCriteria* areaCriteria=CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[KTestIndex]->SouthLat(),
                    iAreaSearchResults[KTestIndex]->NorthLat(),
                    iAreaSearchResults[KTestIndex]->WestLong(),
                    iAreaSearchResults[KTestIndex]->EastLong());
        
    // Search #1
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, sortPref));
    
    RArray<TPosLmItemId>* searchResults = &iAreaSearchResults[KTestIndex]->SearchResult();

    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));

    AssertTrueSecL((TUint)searchResults->Count() == iLmMultiSearch->TotalNumOfMatches(), 
        KNumOfMatchesErr);    
    
    TPosLmItemId lmid = (*searchResults)[0];
    iDatabase = CPosLandmarkDatabase::OpenL();   
    if (iDatabase->IsInitializingNeeded())
   {
   ExecuteAndDeleteLD(iDatabase->InitializeL());
   }

    iDatabase->RemoveLandmarkL(lmid); 
    searchResults->Remove(0);
    searchResults->Compress();
    delete iDatabase;
    iDatabase = NULL;

    // Search #2
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, sortPref, ETrue));

    // Check result
    // Check database array
    CheckDatabasesL(iDbArray);

    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));

    AssertTrueSecL((TUint)searchResults->Count() == iLmMultiSearch->TotalNumOfMatches(), 
        KNumOfMatchesErr);    

    RArray<TPosLmItemId> lmids;
    CleanupClosePushL(lmids);
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results");

    for (TInt j=0; j<iLmMultiSearch->NumOfDatabasesToSearch(); j++) 
        {
        CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(j);
        // Check if there are any matches = iteration exists.
        if (iter) 
            {
            CleanupStack::PushL(iter);
            TPosLmItemId id = iter->NextL();    
            while (id != KPosLmNullItemId)
                {
                // Check that found id is expected
                AssertTrueSecL(iAreaSearchResults[KTestIndex]->FindSearchResult(id), KNotFoundErr);
                User::LeaveIfError(lmids.Append(id));                              
                id = iter->NextL();
                }
            CleanupStack::PopAndDestroy(iter);                
            }
        else 
            {
            _LIT(KIterNull, "MatchIterator returned null for a db");
            iLog->Log(KIterNull);
            }
        }

    AssertTrueSecL(iAreaSearchResults[KTestIndex]->SearchResult().Count() == lmids.Count(), 
        _L("Expected number of hits and found hits don't match"));

    for (TInt k=0; k<lmids.Count(); k++)
        {
        // Check that found id is expected from all ids
        AssertTrueSecL(iAreaSearchResults[KTestIndex]->FindSearchResult(lmids[k]), KNotFoundErr);
        }

    CleanupStack::PopAndDestroy(&lmids); // lmids

    CleanupStack::PopAndDestroy(areaCriteria);

    // Prepare for another search with different criteria
    TInt otherTestIndex = KTestIndex + 1;

    buf.Format(KTestNr, otherTestIndex);
    iLog->Log(buf);
    
    areaCriteria = CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[otherTestIndex]->SouthLat(),
                    iAreaSearchResults[otherTestIndex]->NorthLat(),
                    iAreaSearchResults[otherTestIndex]->WestLong(),
                    iAreaSearchResults[otherTestIndex]->EastLong());

    // Search #3
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, sortPref, ETrue));

    // Check result
    // Check database array
    CheckDatabasesL(iDbArray);

    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));

    AssertTrueSecL(iLmMultiSearch->TotalNumOfMatches() != searchResults->Count(), 
        KNumOfMatchesErr);    

    AssertTrueSecL(iLmMultiSearch->TotalNumOfMatches() == 0, KNumOfMatchesErr);    

    CleanupStack::PopAndDestroy(areaCriteria);
    }

// ---------------------------------------------------------
// CPosTp120::SearchAndRenameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::SearchAndRenameL()
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a redefined search is performed"); 
    const TInt KTestIndex = 4;

    _LIT(KTestNr, "Multi search test, using test index %d.");
    TBuf<100> buf;
    buf.Format(KTestNr, KTestIndex);
    iLog->Log(buf);

    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    
    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[KTestIndex]->SouthLat(),
                    iAreaSearchResults[KTestIndex]->NorthLat(),
                    iAreaSearchResults[KTestIndex]->WestLong(),
                    iAreaSearchResults[KTestIndex]->EastLong());

    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, sortPref));
    
    RArray<TPosLmItemId>* searchResults = &iAreaSearchResults[KTestIndex]->SearchResult();
    
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results");
    _LIT(KLmName, "LmTP120");
   
    for (TInt j=0; j<iLmMultiSearch->NumOfDatabasesToSearch(); j++) 
        {
        CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(j);
        iDatabase = 
            CPosLandmarkDatabase::OpenL(iLmMultiSearch->DatabaseUriPtr(j));
        if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

            
        // Check if there are any matches = iteration exists.
        if (iter)
            {
            CleanupStack::PushL(iter);
            TPosLmItemId id = iter->NextL();    
            while (id != KPosLmNullItemId)
                {
                // Check that found id is expected
                AssertTrueSecL(iAreaSearchResults[KTestIndex]->FindSearchResult(id), 
                    KNotFoundErr);
              
                CPosLandmark* lm = iDatabase->ReadLandmarkLC(id);
        
                TBuf<30> lmName;
                lmName = KLmName;
                lm->SetLandmarkNameL(lmName);
                iDatabase->UpdateLandmarkL(*lm);
                CleanupStack::PopAndDestroy(lm);   
                                
                id = iter->NextL();
                }
            CleanupStack::PopAndDestroy(iter);                
            }
        else 
            {
            _LIT(KIterNull, "MatchIterator returned null for a db");
            iLog->Log(KIterNull);
            }
            
        delete iDatabase;
        iDatabase = NULL;
        }
    
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, sortPref, ETrue));
    
    // Check database array
    CheckDatabasesL(iDbArray);

    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));
        
    AssertTrueSecL((TUint) searchResults->Count() == 
        iLmMultiSearch->TotalNumOfMatches(), KNumOfMatchesErr);

    for (TInt j=0; j<iLmMultiSearch->NumOfDatabasesToSearch(); j++) 
        {
        iDatabase = 
            CPosLandmarkDatabase::OpenL(iLmMultiSearch->DatabaseUriPtr(j));
        if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

        CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(j);
            
        // Check if there are any matches = iteration exists.
        if (iter)
            {
            CleanupStack::PushL(iter);
            TPosLmItemId id = iter->NextL();    
            while (id != KPosLmNullItemId)
                {
                // Check that found id is expected
                AssertTrueSecL(iAreaSearchResults[KTestIndex]->FindSearchResult(id), 
                    KNotFoundErr);
   
                CPosLandmark* lm = iDatabase->ReadLandmarkLC(id);
        
                TPtrC16 lmName;
                lm->GetLandmarkName(lmName);
                AssertTrueSecL(!lmName.CompareC(KLmName), _L("Wrong landmark name"));
                
                CleanupStack::PopAndDestroy(lm);   
                 
                id = iter->NextL();
                }
            CleanupStack::PopAndDestroy(iter);                
            }
        else 
            {
            _LIT(KIterNull, "MatchIterator returned null for a db");
            iLog->Log(KIterNull);
            }   
            
        delete iDatabase;
        iDatabase = NULL;
        }

    CleanupStack::PopAndDestroy(areaCriteria);   
    }

// ---------------------------------------------------------
// CPosTp120::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::SearchAndCancelL(const TBool& aInCallback)
    {
    // Test search with one criteria
    const TInt KTestIndex = 0;

    _LIT(KTestNr, "Multi search test, using test index %d.");
    TBuf<100> buf;
    buf.Format(KTestNr, KTestIndex);
    iLog->Log(buf);

    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[KTestIndex]->SouthLat(),
                    iAreaSearchResults[KTestIndex]->NorthLat(),
                    iAreaSearchResults[KTestIndex]->WestLong(),
                    iAreaSearchResults[KTestIndex]->EastLong());
    
    if (aInCallback)
        {
        TReal threshold = 3;
        for (TReal i=0; i<100; i+=10) 
            {
            threshold = i/100;
            RunAsyncOperationAndCancelInCallbackLD(iLmMultiSearch->StartLandmarkSearchL(
                *areaCriteria, iAreaSearchResults[KTestIndex]->Redefined()), threshold);

            _LIT(KCancelNr, "Multi search test, cancel after progress %f, canceled ok.");
            TBuf<100> buf;
            buf.Format(KCancelNr, threshold);
            iLog->Log(buf);
            }
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLmMultiSearch->StartLandmarkSearchL(
                *areaCriteria, iAreaSearchResults[KTestIndex]->Redefined()));
        }

    CleanupStack::PopAndDestroy(areaCriteria);       
    }

// ---------------------------------------------------------
// CPosTp120::SearchWithMaximumL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp120::SearchWithMaximumL()
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchWithMaximumL is performed");

    // Test search with one criteria
    const TInt KTestIndex = 8;
    
    _LIT(KTestNr, "Multi search test, using test index %d.");
    TBuf<100> buf;
    buf.Format(KTestNr, KTestIndex);
    iLog->Log(buf);

    CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(
                    iAreaSearchResults[KTestIndex]->SouthLat(),
                    iAreaSearchResults[KTestIndex]->NorthLat(),
                    iAreaSearchResults[KTestIndex]->WestLong(),
                    iAreaSearchResults[KTestIndex]->EastLong());
   
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);

    // First get the maximum number of matches
    TInt maxMatches = KNrOfDbs;  
    iLmMultiSearch->SetMaxNumOfMatches(maxMatches);
    _LIT(KErrNrOfMatches, "Number of matches is incorrect");
    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == maxMatches, KErrNrOfMatches);
    
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria, sortPref)); 

    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == maxMatches, KErrNrOfMatches);
    RArray<TPosLmItemId>* searchResults = &iAreaSearchResults[KTestIndex]->SearchResult();    
    TInt totalMatches = iLmMultiSearch->TotalNumOfMatches();
    
    // Check database array
    CheckDatabasesL(iDbArray, maxMatches);
    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));
    // Check that nr is not expected number of results
    AssertTrueSecL((TUint) searchResults->Count() != 
        totalMatches, KNumOfMatchesErr);    

    for (TInt n=0; n<iLmMultiSearch->NumOfDatabasesToSearch(); n++) 
        {
        RArray<TPosLmItemId> lmids;
        CleanupClosePushL(lmids);
        GetLmIdsL(n, *searchResults, lmids);            
    
        iDatabase = CPosLandmarkDatabase::OpenL(iLmMultiSearch->DatabaseUriPtr(n));
        if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

        LandmarksSortL(lmids, sortPref);
    
        CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(n);
        // Check if there are any matches = iteration exists.
        if (iter) 
            {
            CleanupStack::PushL(iter);
            AssertTrueSecL(iter->NumOfItemsL() == iLmMultiSearch->NumOfMatches(n), 
                KNumOfMatchesErr);
    
            TInt lmid = 0;
            TPosLmItemId id = iter->NextL();
    
            while (id != KPosLmNullItemId)
                {
                CheckEqualsLmsL(id, lmids[lmid]);
                
                id = iter->NextL();
                ++lmid;
                }
            CleanupStack::PopAndDestroy(iter);
            }
        else 
            {
            _LIT(KIterNull, "MatchIterator returned null for a db");
            iLog->Log(KIterNull);
            }
            
        delete iDatabase;
        iDatabase = NULL;
        
        CleanupStack::PopAndDestroy(&lmids);
        }

    // Test max matches again
    TInt incMatches = 2;
    maxMatches = maxMatches + incMatches;  
    iLmMultiSearch->SetMaxNumOfMatches(maxMatches);
    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == maxMatches, KErrNrOfMatches);
    
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*areaCriteria)); 

    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == maxMatches, KErrNrOfMatches);
    TInt reTotalMatches = iLmMultiSearch->TotalNumOfMatches();
    
    AssertTrueSecL(reTotalMatches != totalMatches, 
        _L("Total number of matches should not be that same."));
    
    // Check database array
    CheckDatabasesL(iDbArray, maxMatches);
    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));
    // Check that nr is not expected number of results
    AssertTrueSecL((TUint) searchResults->Count() != 
        reTotalMatches, KNumOfMatchesErr); 

    CleanupStack::PopAndDestroy(areaCriteria);

    // Unset max number of matches to normal/unlimited again
    iLmMultiSearch->SetMaxNumOfMatches();
    }

// ---------------------------------------------------------
// CPosTp120::CreateDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CDesCArray* CPosTp120::CreateDatabasesL(TInt aNrOfLmDbs, TInt aLmPerDb, 
    TInt aLastId, TBool aDebug)
    {
    
    CDesCArray* iDbArray = new (ELeave) CDesCArrayFlat(aNrOfLmDbs); 
    CleanupStack::PushL(iDbArray);     
         
    RemoveDefaultDbL();
    
    // Populate default db
    iDatabase = UseGeneratedDbFileL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    delete iDatabase;
    iDatabase = NULL;  
      
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    // Create databases
    TInt i=0;
    for (i=1; i < aNrOfLmDbs; i++)
        {
        TBuf<100> db;
        db.Format(KDbNamePath, i * aLmPerDb);
        
        iFileSession.Delete(db);
        User::LeaveIfError(fileMan->Copy(KLmFullDefaultDbPath, db, CFileMan::EOverWrite));    
        }
    CleanupStack::PopAndDestroy(fileMan);               

    for (TInt db=0; db < aNrOfLmDbs; db++)
        {
        TInt dbform = db;
        if (db == aNrOfLmDbs)
            {
            dbform = aNrOfLmDbs - 1;
            }
            
        CPosLandmarkDatabase* lmd = NULL;
        TBuf<100> name;
        if (db == 0)
            {
            _LIT(KDefLmDb, "eposlm.ldb");
            name = KDefLmDb;
            lmd = CPosLandmarkDatabase::OpenL();
            if (lmd->IsInitializingNeeded())
           {
           ExecuteAndDeleteLD(lmd->InitializeL());
           }

            }
        else 
            {
            name.Format(KDbName, dbform * aLmPerDb);        
            lmd = CPosLandmarkDatabase::OpenL(name);
            if (lmd->IsInitializingNeeded())
           {
           ExecuteAndDeleteLD(lmd->InitializeL());
           }

            }
        CleanupStack::PushL(lmd);
        // Append db name to array
        iDbArray->AppendL(name);

        if (aDebug)
            {
            iLog->Log(name);
            }
        
        TInt startItemid = dbform * aLmPerDb + 1;
        TInt stopItemid = dbform * aLmPerDb + aLmPerDb;            
        if (dbform == aNrOfLmDbs - 1)
            {
            stopItemid = aLastId;
            }
        
        for (TInt lmid=1; lmid <= aLastId; lmid++) 
            {
            TBuf<100> lmidbuf;
                
            if (aDebug)
                {
                _LIT(KLmId, "Landmark with LmId: %d");
                lmidbuf.Format(KLmId, lmid);
                iLog->Log(lmidbuf);                                
                }

            if (lmid < startItemid || lmid > stopItemid)
                {
                if (aDebug)
                    {
                    _LIT(KDelLmId, "Removing LmId: %d");
                    lmidbuf.Format(KDelLmId, lmid);
                    iLog->Log(lmidbuf);                                
                    }
                
                TInt err = KErrLocked;
                while (err == KErrLocked)
                    {
                    TRAP(err, lmd->RemoveLandmarkL(lmid));
                    }
                }
            }

        // Compact database
        ExecuteAndDeleteLD(lmd->CompactL());
        CleanupStack::PopAndDestroy(lmd);  
        }
        
    // Log/print landmark databases.
    if (aDebug)
        {
        // DEBUG 
        for (i=0; i < aNrOfLmDbs; i++)
            {
            TBuf<100> dbNo;
            dbNo.Format(_L(" -------- Printing Db %d --------------"), i);
            iLog->Log(dbNo);
                
            CPosLandmarkDatabase* lmd = NULL;
            if (i == 0) 
                {
                _LIT(KDbDefName, "Default Lm DB");
                iLog->Log(KDbDefName);
                lmd = CPosLandmarkDatabase::OpenL();
                if (lmd->IsInitializingNeeded())
               {
               ExecuteAndDeleteLD(lmd->InitializeL());
               }

                }
            else 
                {            
                TInt dbform = i;
                if (i == 5)
                    {
                    dbform = 4;
                    }
                    
                TBuf<100> db;
                db.Format(KDbName, dbform * aLmPerDb);
                iLog->Log(db);
                
                lmd = CPosLandmarkDatabase::OpenL(db);  
                if (lmd->IsInitializingNeeded())
               {
               ExecuteAndDeleteLD(lmd->InitializeL());
               }

                }
            CleanupStack::PushL(lmd);
            CPosLmCategoryManager* lmcat = CPosLmCategoryManager::NewL(*lmd);
            CleanupStack::PushL(lmcat);
            
            CPosLmItemIterator* iter = lmd->LandmarkIteratorL();
            CleanupStack::PushL(iter);

            _LIT(KNrLms, "Nr of Lms: %d");
            TBuf<20> lms;
            lms.Format(KNrLms, iter->NumOfItemsL());
            iLog->Log(lms);
           
            TPosLmItemId id = iter->NextL(); 
            while (id != KPosLmNullItemId)
                {
                CPosLandmark* lm = lmd->ReadLandmarkLC(id);
            
                TBuf<255> info;
                PrintLandmark(*lm, info);
                iLog->Log(info);
                CleanupStack::PopAndDestroy(lm); 
                
                id = iter->NextL();
                }    

            CleanupStack::PopAndDestroy(iter);

            CPosLmItemIterator* catiter = lmcat->CategoryIteratorL();
            CleanupStack::PushL(catiter);

            _LIT(KNrLmCats, "Nr of Lm Cats: %d");
            TBuf<20> lmcats;
            lmcats.Format(KNrLmCats, catiter->NumOfItemsL());
            iLog->Log(lmcats);
           
            id = catiter->NextL(); 
            while (id != KPosLmNullItemId)
                {
                CPosLandmarkCategory* cat = lmcat->ReadCategoryLC(id);
            
                TBuf<255> info;
                // PrintLandmark(*lm, info);
                _LIT(KCatId, "Category id: %d");
                _LIT(KCatGlobalId, "Global Category id: %d");
                _LIT(KCatName, "Category name: %S");
                
                info.Format(KCatId, cat->CategoryId());                
                iLog->Log(info);
                
                info.Format(KCatGlobalId, cat->GlobalCategory());                
                iLog->Log(info);
                
                TPtrC catname;
                TInt err = cat->GetCategoryName(catname);
                info.Format(KCatName, &catname);                
                iLog->Log(info);
                
                CleanupStack::PopAndDestroy(cat); 
                
                id = catiter->NextL();
                }    
           
           
            CleanupStack::PopAndDestroy(3, lmd); // catiter, lmcat
            }        
        }
        
    CleanupStack::Pop(iDbArray);
    return iDbArray;
    }

//  End of File
