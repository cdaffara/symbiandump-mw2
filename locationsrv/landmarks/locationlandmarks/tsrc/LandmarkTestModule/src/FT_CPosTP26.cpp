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
#include "FT_CPosTp26.h"

#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp26::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp26::CloseTest()
    {
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CPosTp26::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp26::StartL()
    {
    iDatabase = UseGeneratedDbFileL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    AppendSearchResultsL();
    
    iLog->Log(_L("DoLandmarkSearchL(iSearchResults);"));
    DoLandmarkSearchL(iSearchResults);

    AppendCategorySearchResultsL();
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    iLog->Log(_L("DoLandmarkCategorySearchL(iCategorySearchResults);"));
    DoLandmarkCategorySearchL(iCategorySearchResults);
    }

// ---------------------------------------------------------
// CPosTp26::DoLandmarkCategorySearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp26::DoLandmarkCategorySearchL(const RPointerArray<CCategorySearchResult>& aSearchResults)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");
    _LIT(KWrongIdErr, "Wrong id returned from iterator after reset");

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    // Use wildcard * as search pattern
    nameCriteria->SetSearchPatternL(aSearchResults[0]->TextCriteria());
    
    ExecuteAndDeleteLD(iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 
    
    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    if ((TUint)(aSearchResults[0]->SearchResult()).Count() != iLandmarkSearch->NumOfMatches())
        {
        _LIT(KExpected, "Expected no. of matches: %d");
        _LIT(KReturned, "Returned no. of matches: %d");
        TBuf<100> info;
        info.Format(KExpected, (aSearchResults[0]->SearchResult()).Count());
        iLog->Log(info);
        info.Format(KReturned, iLandmarkSearch->NumOfMatches());
        iLog->Log(info);
        iLog->Log(_L("Name criteria: "));
        iLog->Log(nameCriteria->SearchPattern());
        
        iLog->Log(KNumOfMatchesErr);
       	User::Leave(-1);
        }

    // Get the first category in list, id1
    TPosLmItemId id1 = iter->NextL();
    CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(id1);
    TPtrC name;
    cat->GetCategoryName(name);
    iLog->Log(_L("First time"));
    iLog->Log(name);
    CleanupStack::PopAndDestroy(cat);

    //Reset iterator and make sure that id1 is returned again
    iter->Reset();
    TPosLmItemId id11 = iter->NextL();
    AssertTrueSecL(id1 == id11, KWrongIdErr);

    iLog->Log(_L("******************"));
    // Iterate through whole iterator
    while (id11 != KPosLmNullItemId)
        {
        AssertTrueSecL(aSearchResults[0]->FindSearchResult(id11), KNotFoundErr, id11);
        CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(id11);
        TPtrC name;
        cat->GetCategoryName(name);
        iLog->Log(name);
        CleanupStack::PopAndDestroy(cat);
        id11 = iter->NextL();
        }
    iLog->Log(_L("******************"));
    RArray<TPosLmItemId> lmIdArray;
    CleanupClosePushL(lmIdArray);

    // Get a sequence of items from the iterated set
    // lmIdArray should be reset in GetItemIdsL
    // Use some magic numbers here, check landmarksDb.xls for more info
    iter->GetItemIdsL(lmIdArray, 5, 10);
    TInt nr = lmIdArray.Count();
    for (TInt j=0;j<nr; j++)
        {
        AssertTrueSecL(aSearchResults[0]->FindSearchResult(lmIdArray[j]), KNotFoundErr);
        CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(lmIdArray[j]);
        TPtrC name;
        cat->GetCategoryName(name);
        iLog->Log(name);
        CleanupStack::PopAndDestroy(cat);
        }

    CleanupStack::PopAndDestroy(&lmIdArray);
    CleanupStack::PopAndDestroy(2, nameCriteria);
    CleanupStack::PopAndDestroy(categoryManager);
    }

// ---------------------------------------------------------
// CPosTp26::DoLandmarkSearch
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp26::DoLandmarkSearchL(const RPointerArray<CSearchResult>& aSearchResults)
    {
    iLog->Log(_L("DoLandmarkSearch()"));
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed asyncronously");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed asyncronously");
    _LIT(KNumOfMatchesInIteratorErr, "Wrong number of matches in iterator");
    _LIT(KWrongIdErr, "Wrong id returned from iterator after reset");
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);

    TBuf<150> buf;
    _LIT(KDebug, "Searching for ");
    
    // Put this RArray outside loop so that if reset of the array is missing in the code
    // it will be noticed
    RArray<TPosLmItemId> lmIdArray;
    CleanupClosePushL(lmIdArray);

    for(TInt i=0; i<aSearchResults.Count(); i++)
        {
        CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
        
        textCriteria->SetTextL(aSearchResults[i]->TextCriteria());
        textCriteria->SetAttributesToSearch(aSearchResults[i]->Attributes());
        textCriteria->SetPositionFieldsToSearchL(aSearchResults[i]->PositionFields()); 
       
        buf.Zero();
        buf.Append(KDebug);
        buf.Append(aSearchResults[i]->TextCriteria());
        iLog->Log(buf);

        iOperation = iLandmarkSearch->StartLandmarkSearchL(*textCriteria, aSearchResults[i]->Redefined());
        RunAsyncOperationLD(iOperation);

        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);

        buf.Zero();
        buf.Format(_L("Found %d nr of matches, should found %d nr of matches"), iLandmarkSearch->NumOfMatches(), (aSearchResults[i]->SearchResult()).Count());
        iLog->Log(buf);
        TUint nrOfMatches = iLandmarkSearch->NumOfMatches();
        AssertTrueSecL((TUint)(aSearchResults[i]->SearchResult()).Count() == nrOfMatches, KNumOfMatchesErr);
        
        TPosLmItemId id1 = iter->NextL();
        TPosLmItemId id = KPosLmNullItemId;
        if (id1 != KPosLmNullItemId)
            {
            CPosLandmark* lm = iDatabase->ReadLandmarkLC(id1);
            CleanupStack::PopAndDestroy(lm);
            // Reset the iterator and make sure that the first id is returned again
            iter->Reset();
            id = iter->NextL();
            }

        AssertTrueSecL(id1 == id, KWrongIdErr);
        TUint counter=0;
        while (id != KPosLmNullItemId)
            {
            AssertTrueSecL(aSearchResults[i]->FindSearchResult(id), KNotFoundErr);
            CPosLandmark* lm = iDatabase->ReadLandmarkLC(id);
            CleanupStack::PopAndDestroy(lm);
            id = iter->NextL();
            counter++;
            }
        TUint nrInIterator = iter->NumOfItemsL();
        if (nrOfMatches != counter || nrOfMatches != nrInIterator)
            {
            
            iLog->Log(KNumOfMatchesInIteratorErr);
       		User::Leave(-1);
            }

        // Get a sequence of items and check ids
        TInt startIndex = 0;
        // Just to get some different starting index depending on nr of ids
        if (nrOfMatches>2) startIndex = 2;
        if (nrOfMatches>5) startIndex = 5;
        if (nrOfMatches>20) startIndex = 20;

        // Do this if there are some matches
        if (nrOfMatches-startIndex>0)
            {
            buf.Zero();
            buf.Format(_L("GetItemIdsL from index %d nr of items index %d"), startIndex, nrOfMatches-startIndex);
            iLog->Log(buf);
            // lmIdArray should be reset in GetItemIdsL
            iter->GetItemIdsL(lmIdArray, startIndex, nrOfMatches-startIndex);
            
            TInt nr = lmIdArray.Count();
            for (TInt j=0;j<nr; j++)
                {
                AssertTrueSecL(aSearchResults[i]->FindSearchResult(lmIdArray[j]), KNotFoundErr);
                CPosLandmark* lm = iDatabase->ReadLandmarkLC(lmIdArray[j]);
                CleanupStack::PopAndDestroy(lm);
                }
            }

        CleanupStack::PopAndDestroy(2, textCriteria);
    } // end for
    CleanupStack::PopAndDestroy(&lmIdArray);
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    }

// End of file

