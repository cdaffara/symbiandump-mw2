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

#include "FT_CPosTp101.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmDisplayData.h>

// CONSTANTS  
const TInt KNrOfDbs = 5;
const TInt KLmPerDb = 20;
const TInt KTotLms = 105;

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp101::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;
    
    delete iDbArray;

    delete iLmMultiSearch;
    iLmMultiSearch = NULL;
    
    iSearchResults.ResetAndDestroy();
    
    REComSession::FinalClose();
    }
    
// ---------------------------------------------------------
// CPosTp101::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::StartL()
    {    
    // Split databases, NrOfDBs db, LmPerDb lm in each, TotLms is last lm id from original db, debug
    iDbArray = CreateDatabasesL(KNrOfDbs, KLmPerDb, KTotLms); //, ETrue);    
    
    
    // Check setting of databases is correct
    // Should be move to TC119 - Multi Db - Extended testing
    VerifySetOfDatabasesL();

    // Constructing a lm multi search 
    iLmMultiSearch = CPosLmMultiDbSearch::NewL(*iDbArray);
    
    // Check that iterator is NOT NULL before search has started (and that iterator is empty)
    for (TInt i=0;i<iDbArray->Count();i++)
    	{
    	CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(i);
    	CleanupStack::PushL(iter);
    	AssertTrueSecL(iter->NumOfItemsL() == 0, _L("Iterator should be empty"));
    	CleanupStack::PopAndDestroy(iter);
    	}
    
    // Read search information from file    
    AppendSearchResultsL();

    // Do the search testing     
    iLog->Log(_L("Testing multi db search with criterias read from LmDbSearchResult.txt syncronously"));
    SearchL(iSearchResults, ESynchronous);

    iLog->Log(_L("Testing multi db search with criterias read from LmDbSearchResult.txt asyncronously"));
    SearchL(iSearchResults, EAsynchronous);

    // Not possible to use User::WaitForRequest when using Multi Db search!
    // iLog->Log(_L("Testing multi db search with criterias read from LmDbSearchResult.txt asyncronously using User::WaitForRequest"));
    // SearchL(SearchResults(), EWaitForRequest);

    iLog->Log(_L("Testing search with a defined sortorder"));
    SearchWithSortOrderL(iSearchResults);   

    iLog->Log(_L("Testing search and cancel"));
    SearchAndCancelL();
   
    iLog->Log(_L("Testing maximum number of matches"));
    SearchWithMaximumL(iSearchResults);

    iLog->Log(_L("Testing searching on the fly"));
    OnTheFlySearchL();

    iLog->Log(_L("Testing search after a landmark has been removed"));
    SearchAndDeleteL(iSearchResults);

    iLog->Log(_L("Testing search after landmarks have been renamed"));
    SearchAndRenameL(iSearchResults);


    // Test specific Text Criteria error case
    TUint attr = CPosLandmark::ELandmarkName | CPosLandmark::EDescription;
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetAttributesToSearch(attr);
    textCriteria->SetTextL(_L(""));

    _LIT(KEmptyTextCriteriaErr, "An empty string as text criteria should not be allowed");
    TRAPD(err, iLmMultiSearch->StartLandmarkSearchL(*textCriteria)); 
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);

    CleanupStack::PopAndDestroy(textCriteria);
    
    delete iLmMultiSearch;
    iLmMultiSearch = NULL;
    // Constructing a lm multi search 
    iLmMultiSearch = CPosLmMultiDbSearch::NewL(*iDbArray);
    
    // Check that iterator is NOT NULL before search has started (and that iterator is empty)
    for (TInt i=0;i<iDbArray->Count();i++)
    	{
    	CPosLmItemIterator* iter = iLmMultiSearch->MatchIteratorL(i);
    	CleanupStack::PushL(iter);
    	AssertTrueSecL(iter->NumOfItemsL() == 0, _L("Iterator should be empty"));
    	CleanupStack::PopAndDestroy(iter);
    	}
	}

// ---------------------------------------------------------
// CPosTp101::VerifySetOfDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::VerifySetOfDatabasesL()
    {
    // Testing error situations for multi db search
    _LIT(KEmptyError, "KErrArgument should be returned when no db's are specified.");
    CDesCArray* emptyArray = new (ELeave) CDesCArrayFlat(5); 
    CleanupStack::PushL(emptyArray);
    TRAPD(err, CPosLmMultiDbSearch::NewL(*emptyArray));
    AssertTrueSecL(err == KErrArgument, KEmptyError);

    CDesCArray* duplicateArray = new (ELeave) CDesCArrayFlat(5); 
    CleanupStack::PushL(duplicateArray);

    duplicateArray->AppendL(_L("eposlm.ldb"));
    duplicateArray->AppendL(_L("c:eposlm.ldb"));
    duplicateArray->AppendL(_L("file://c:eposlm.ldb"));

    VerifyDatabaseL(duplicateArray);
    
    duplicateArray->Reset();
    duplicateArray->AppendL(_L("eposlm.ldb"));
    duplicateArray->AppendL(_L("eposlm.ldb"));
    duplicateArray->AppendL(_L("eposlm.ldb"));

    VerifyDatabaseL(duplicateArray);

    CDesCArray* dupArray = new (ELeave) CDesCArrayFlat(5); 
    CleanupStack::PushL(dupArray);
    dupArray->AppendL(_L("eposlm.ldb"));
    dupArray->AppendL(_L("eposlm.ldb"));
    dupArray->AppendL(_L("eposlm.ldb"));
    dupArray->AppendL(_L("eposlm.ldb"));
   
    VerifyDatabaseL(duplicateArray, dupArray);

    dupArray->Reset();
    dupArray->AppendL(_L("eposlm.ldb"));
    dupArray->AppendL(_L("eposlm.ldb"));

    VerifyDatabaseL(duplicateArray, dupArray);
   
    CleanupStack::PopAndDestroy(dupArray);
    CleanupStack::PopAndDestroy(duplicateArray);

    // Check setting of empty databases
    iLmMultiSearch = CPosLmMultiDbSearch::NewL(*iDbArray);
 
    TRAP(err, iLmMultiSearch->SetDatabasesToSearchL(*emptyArray));
    AssertTrueSecL(err == KErrArgument, KEmptyError);
    CleanupStack::PopAndDestroy(emptyArray);
    delete iLmMultiSearch;

    VerifyDatabaseL(iDbArray);    
    }

// ---------------------------------------------------------
// CPosTp101::VerifyDatabaseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::VerifyDatabaseL(
    CDesCArray* aLmUris, 
    CDesCArray* aNewLmUris)
    {
    iLmMultiSearch = CPosLmMultiDbSearch::NewL(*aLmUris);
    CheckDatabasesL(aLmUris);
    
    if (aNewLmUris)
        {
        iLmMultiSearch->SetDatabasesToSearchL(*aNewLmUris);
        CheckDatabasesL(aNewLmUris);
        }
        
    delete iLmMultiSearch;
    }

// ---------------------------------------------------------
// CPosTp101::CheckDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::CheckDatabasesL(
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
// CPosTp101::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::SearchL(
    const RPointerArray<CSearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");
    _LIT(KSearchNr, "Multi search test #%d executed ok.");

    for(TInt i=0; i<aSearchResults.Count(); i++)
        {   
        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
        textCriteria->SetTextL(aSearchResults[i]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[i]->Attributes());
        
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[i]->PositionFields()); 

        CPosLmOperation* operation = 
            iLmMultiSearch->StartLandmarkSearchL(*textCriteria, aSearchResults[i]->Redefined());
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
            iLog->Log(_L("Search criteria: "));
            iLog->Log(textCriteria->Text());
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

        CleanupStack::PopAndDestroy(textCriteria); 
        
        TBuf<100> buf;
        buf.Format(KSearchNr, i);        
        iLog->Log(buf);            
        }
    }

// ---------------------------------------------------------
// CPosTp101::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::SearchWithSortOrderL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed syncronously");
    
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);  
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();

    for(TInt j=0; j<aSearchResults.Count(); j++)
        {   
        textCriteria->SetTextL(aSearchResults[j]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[j]->Attributes());
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[j]->PositionFields()); 

        iLog->Log(aSearchResults[j]->TextCriteria());
        
        for (TInt i=0; i<4; i++)
            {
            switch (i)
                {
                //**** sync
                case 0:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 1:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 2:
                //**** async
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
                    RunAsyncOperationLD(iLmMultiSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                case 3:
                    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
                    RunAsyncOperationLD(iLmMultiSearch->StartLandmarkSearchL(
                        *textCriteria, 
                        sortPref, 
                        aSearchResults[j]->Redefined()));
                    break;
                //**** asych with User::WaitForRequest()
                case 4:
                    {
                    CPosLmOperation* op = iLmMultiSearch->StartLandmarkSearchL(
                        *textCriteria, 
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
                        *textCriteria, 
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
            iLog->Log(_L("Search criteria: "));
            iLog->Log(textCriteria->Text());
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

        }
        
    CleanupStack::PopAndDestroy(textCriteria);
    }

// ---------------------------------------------------------
// CPosTp101::GetLmIdsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::GetLmIdsL(
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
// CPosTp101::CheckEqualsLmsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::CheckEqualsLmsL(
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
// CPosTp101::OnTheFlySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::OnTheFlySearchL()
    {
    TInt KTestIndex = 4;
                
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(iSearchResults[KTestIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(iSearchResults[KTestIndex]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(iSearchResults[KTestIndex]->PositionFields()); 
    
    COnTheFlyTester* onTheFlyTester = COnTheFlyTester::NewLC();
    onTheFlyTester->StartSearchCampaignL(iLmMultiSearch, 
        iSearchResults[KTestIndex]->SearchResult(), 
        textCriteria);

    TInt result = onTheFlyTester->Err();
    if (result != KErrNone)
        {
        TPtrC msg;
        onTheFlyTester->GetErrMsg(msg);
        iLog->Log(msg);
        User::Leave(result);
        }
        
    CleanupStack::PopAndDestroy(2, textCriteria);
    
    iLmMultiSearch->UnsetDisplayData();
    }

// ---------------------------------------------------------
// CPosTp101::SearchAndDeleteL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::SearchAndDeleteL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a redefined search is performed"); 
    const TInt KTestIndex = 11;
    
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
    textCriteria->SetTextL(aSearchResults[KTestIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[KTestIndex]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[KTestIndex]->PositionFields()); 
    
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*textCriteria, sortPref));
    
    RArray<TPosLmItemId>* searchResults = &aSearchResults[KTestIndex]->SearchResult();
    
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

    // Search
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*textCriteria, sortPref, ETrue));

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
                AssertTrueSecL(aSearchResults[KTestIndex]->FindSearchResult(id), KNotFoundErr);
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

    AssertTrueSecL(aSearchResults[KTestIndex]->SearchResult().Count() == lmids.Count(), 
        _L("Expected number of hits and found hits don't match"));

    for (TInt k=0; k<lmids.Count(); k++)
        {
        // Check that found id is expected from all ids
        AssertTrueSecL(aSearchResults[KTestIndex]->FindSearchResult(lmids[k]), KNotFoundErr);
        }

    CleanupStack::PopAndDestroy(&lmids); // lmids
       
    CleanupStack::PopAndDestroy(textCriteria);
    }

// ---------------------------------------------------------
// CPosTp101::SearchAndRenameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::SearchAndRenameL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when a redefined search is performed"); 
    const TInt KTestIndex=4;

    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
    textCriteria->SetTextL(aSearchResults[KTestIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(aSearchResults[KTestIndex]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(aSearchResults[KTestIndex]->PositionFields()); 
    
    iLog->Log(_L("Search criteria: "));
    iLog->Log(textCriteria->Text());
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*textCriteria, sortPref));
    
    RArray<TPosLmItemId>* searchResults = &aSearchResults[KTestIndex]->SearchResult();
    
    // Check database array
    CheckDatabasesL(iDbArray);

    // Check for errors        
    AssertTrueSecL(iLmMultiSearch->NumOfSearchErrors() == 0, 
        _L("Error(s) found during multi search."));

    AssertTrueSecL((TUint) searchResults->Count() == 
        iLmMultiSearch->TotalNumOfMatches(), KNumOfMatchesErr);    

    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results");
    
    _LIT(KLmName, "l");

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
                AssertTrueSecL(aSearchResults[KTestIndex]->FindSearchResult(id), 
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
    
    iLog->Log(_L("Search criteria: "));
    iLog->Log(textCriteria->Text());
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*textCriteria, sortPref, ETrue));
    
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
                AssertTrueSecL(aSearchResults[KTestIndex]->FindSearchResult(id), 
                    KNotFoundErr);
   
                CPosLandmark* lm = iDatabase->ReadLandmarkLC(id);
        
                TPtrC16 lmName;
                lm->GetLandmarkName(lmName);
                AssertTrueSecL(lmName == KLmName, _L("Wrong landmark name"));
                
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
  
    CleanupStack::PopAndDestroy(textCriteria);
    }

// ---------------------------------------------------------
// CPosTp101::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::SearchAndCancelL(const TBool& aInCallback)
    {
    // Test search with one criteria
    const TInt KIndex=0;

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(iSearchResults[KIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(iSearchResults[KIndex]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(iSearchResults[KIndex]->PositionFields()); 

    if (aInCallback)
        {
        TReal threshold = 0;
        for (TReal i=0; i<100; i+=10) 
            {
            threshold = i/100;
            RunAsyncOperationAndCancelInCallbackLD(iLmMultiSearch->StartLandmarkSearchL(
                    *textCriteria, iSearchResults[KIndex]->Redefined()), threshold);
                    
            _LIT(KCancelNr, "Multi search test, cancel after progress %f, canceled ok.");
            TBuf<100> buf;
            buf.Format(KCancelNr, threshold);
            iLog->Log(buf);         
            }
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLmMultiSearch->StartLandmarkSearchL(
                *textCriteria, iSearchResults[KIndex]->Redefined()));
        }
        
    CleanupStack::PopAndDestroy(textCriteria);       
    }
     
// ---------------------------------------------------------
// CPosTp101::SearchWithMaximumL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp101::SearchWithMaximumL(
    const RPointerArray<CSearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchWithMaximumL is performed");

    // Test search with one criteria
    const TInt KTestIndex = 0;

    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
 
    textCriteria->SetTextL(iSearchResults[KTestIndex]->TextCriteria());
    textCriteria->SetAttributesToSearch(iSearchResults[KTestIndex]->Attributes());
    textCriteria->SetPositionFieldsToSearchL(iSearchResults[KTestIndex]->PositionFields()); 

    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);

    // First get the maximum number of matches
    TInt maxMatches = KNrOfDbs;  
    iLmMultiSearch->SetMaxNumOfMatches(maxMatches);
    _LIT(KErrNrOfMatches, "Number of matches is incorrect");
    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == maxMatches, KErrNrOfMatches);
    
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*textCriteria, sortPref)); 

    AssertTrueSecL(iLmMultiSearch->MaxNumOfMatches() == maxMatches, KErrNrOfMatches);
    RArray<TPosLmItemId>* searchResults = &aSearchResults[KTestIndex]->SearchResult();    
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
    
    ExecuteAndDeleteLD(iLmMultiSearch->StartLandmarkSearchL(*textCriteria)); 

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

    CleanupStack::PopAndDestroy(textCriteria);

    // Unset max number of matches to normal/unlimited again
    iLmMultiSearch->SetMaxNumOfMatches();
    }

// ---------------------------------------------------------
// CPosTp101::CreateDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CDesCArray* CPosTp101::CreateDatabasesL(TInt aNrOfLmDbs, TInt aLmPerDb, 
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
    ExecuteAndDeleteLD(iDatabase->CompactL());
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
