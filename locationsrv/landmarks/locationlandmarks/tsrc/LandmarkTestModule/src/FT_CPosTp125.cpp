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
#include "FT_CPosTp125.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmMultiDbSearch.h> 
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLMItemIterator.h>    
#include <EPos_TPosLMSortPref.h>
#include "FT_CSearchResult.h"
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include "FT_LandmarkConstants.h"
     
// CONSTANTS
    
    _LIT(KPizzeria,          "Pizzeria");           // Db 1, id  1
    _LIT(KHambergerbar,      "Hamburgerbar");       // Db 1, id  2        
    _LIT(KKinarestaurang,    "Kinarestaurang");     // Db 1, id  3
    _LIT(KGaraget,           "Garaget");            // Db 1, id  4
    _LIT(KBilverkstad,       "Bilverkstad");        // Db 1, id  5
    _LIT(KPastaRestaurang,   "Pasta Restaurang");   // Db 2, id  6
    _LIT(KBilia,             "Bilia");              // Db 2, id  7
    _LIT(KVolksWagen,        "VolksWagen");         // Db 2, id  8
    _LIT(KTaxiGoteborg,      "Taxi Göteborg");      // Db 2, id  9
    _LIT(KTaxi,              "Taxi");               // Db 2, id 10
    _LIT(KStadsbibliotek,    "Stadsbibliotek");     // Db 2, id 11
    _LIT(KSystembolag,       "Systembolag");        // Db 2, id 12
    _LIT(KNokiKontor,        "Nokia Kontor");       // Db 2, id 13
    _LIT(KTietoEnatorKontor, "TietoEnator kontor"); // Db 2, id 14
    _LIT(KThaiRestaurang,    "Thai Restaurang");    // Db 3, id 15
    _LIT(KMcDonalds,         "McDonalds");          // Db 3, id 16
    _LIT(KBurgerKing,        "BurgerKing");         // Db 3, id 17
    _LIT(KFrisor,            "Frisör");             // Db 3, id 18
    _LIT(KHunddagis,         "Hunddagis");          // Db 3, id 19
    _LIT(KVeterinar,         "Veterinär");          // Db 3, id 20
    _LIT(KKyrkogard,         "Kyrkogård");          // Db 4, id 21
    _LIT(KMuseum,            "Museum");             // Db 4, id 22
    _LIT(KDagis,             "Dagis");              // Db 4, id 23
    _LIT(KStadspark,         "StadsPark");          // Db 4, id 24
    _LIT(KSjukhus,           "Sjukhus");            // Db 5, id 25
    _LIT(KLakare,            "Läkare");             // Db 5, id 26
 
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp125::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iLandmarksMultiDbSearch;
    iLandmarksMultiDbSearch = NULL;
    
    delete iDatabase;
    iDatabase=NULL;

    iCategorySearchResults.ResetAndDestroy();

    TRAPD(err, RemoveGlobalCategoriesL());
    TBuf<50> buf;
    if (err != KErrNone) 
        {
        buf.Format(_L("RemoveGlobalCategoriesL leaved with %d"), err);
        iLog->Log(buf);
        }
    }

// ---------------------------------------------------------
// CPosTp125::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::StartL()
    {
    
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
    SetupDbArrayL();
    
    _LIT(KEmptyTextCriteriaErr, "An empty string as text criteria should not be allowed");
    CDesCArray* dbList = new (ELeave) CDesCArrayFlat(8);
    
    CleanupStack::PushL(dbList);
    
    dbList->AppendL(KDb20);
    dbList->AppendL(KDb40);
    dbList->AppendL(KDb60);
    dbList->AppendL(KDb80);
    dbList->AppendL(KDb105);
    
    RemoveExtraCategoriesL();
    
    iLandmarksMultiDbSearch = CPosLmMultiDbSearch::NewL(*dbList);
    
    TInt err = KErrNone;
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    
    // Start by testing some error flows, returning KErrArgument
    TRAP(err, iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);

    // Test empty string
    nameCriteria->SetSearchPatternL(_L(""));   
    TRAP(err, iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone));
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);
    CleanupStack::PopAndDestroy(nameCriteria);
    
    AppendCategorySearchResultsL();

// Search synchronously
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt syncronously"));
    SearchL(iCategorySearchResults, ESynchronous);

    iLog->Log(_L("Testing search with sort order"));
    SearchWithSortOrderL(iCategorySearchResults, ESynchronous);


    iLog->Log(_L("Testing search with maximum nr of matches"));
    SearchWithMaximumL(iCategorySearchResults, ESynchronous);


// Search asynchronously
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asyncronously"));
    SearchL(iCategorySearchResults, EAsynchronous);

    iLog->Log(_L("Testing search with sort order asynchronously"));
    SearchWithSortOrderL(iCategorySearchResults, EAsynchronous);

    iLog->Log(_L("Testing search with maximum nr of matches asynchronously"));
    SearchWithMaximumL(iCategorySearchResults, EAsynchronous);

// Search for added categories
    iLog->Log(_L("Testing search after category has been added"));
    SearchAfterAddedCategoryL();

// Cancel search test
    iLog->Log(_L("Testing category search and cancel"));    
    SearchAndCancelL();

    iLog->Log(_L("Testing category search and cancel in callback"));    
    SearchAndCancelL(ETrue);

    delete iDatabase;
    iDatabase = NULL;
    delete iLandmarksMultiDbSearch;
    iLandmarksMultiDbSearch = NULL;
    CPosLmDatabaseManager* dbMan = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbMan);
    dbMan->SetDefaultDatabaseUriL(KDb20);
    
    iDatabase = CPosLandmarkDatabase::OpenL();
    iLandmarksMultiDbSearch = CPosLmMultiDbSearch::NewL(*dbList);
    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    ExecuteAndDeleteLD(iDatabase->InitializeL());
    CleanupStack::PushL(categoryManager);   
    
    CleanupStack::PopAndDestroy(categoryManager);

    iLog->Log(_L("Testing search with global categories included, sync"));
    SearchGlobalL(iCategorySearchResults, ETrue);

    iLog->Log(_L("Testing search with global categories included, asyncronously"));
    SearchGlobalL(iCategorySearchResults, EFalse);
    
    // Test to add the global categories to one more DB
    delete iDatabase;
    iDatabase = NULL;
    delete iLandmarksMultiDbSearch;
    iLandmarksMultiDbSearch = NULL;
    dbMan->SetDefaultDatabaseUriL(KDb40);
    CleanupStack::PopAndDestroy(dbMan);
    
    iDatabase = CPosLandmarkDatabase::OpenL();
    iLandmarksMultiDbSearch = CPosLmMultiDbSearch::NewL(*dbList);
    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager2 = CPosLmCategoryManager::NewL(*iDatabase);
    ExecuteAndDeleteLD(iDatabase->InitializeL());
    CleanupStack::PushL(categoryManager2);   
    CleanupStack::PopAndDestroy(categoryManager2);
    
    iLog->Log(_L("Testing search with duplicate global categories included, sync"));
    TestDuplicateGlobalCategoriesL(iCategorySearchResults, ETrue);
    
    iLog->Log(_L("Testing search with duplicate global categories included, async"));
    TestDuplicateGlobalCategoriesL(iCategorySearchResults, EFalse);

    RemoveGlobalCategoriesL();
    CleanupStack::PopAndDestroy(dbList);

    }

// ---------------------------------------------------------
// CPosTp125::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::SearchL(
    const RPointerArray<CCategorySearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");
    
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);
    for(TInt i=0; i<aSearchResults.Count(); i++)
        {   
        CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
        nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());

        iOperation = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone); 

        // The execution flow
        switch (aExecutionMode)
            {
            case ESynchronous:
                ExecuteAndDeleteLD(iOperation);
                break;
            case EAsynchronous:
                RunAsyncOperationLD(iOperation);
                break;
            }

        
        TInt matchesSum = 0;
        for (TInt count = 0; count < 5; count++)
            {
            matchesSum += iLandmarksMultiDbSearch->NumOfMatches(count);
            }
            
            
        if ((TUint)(aSearchResults[i]->SearchResult()).Count() != matchesSum)
            {
           	iLog->Log(KNumOfMatchesErr);
    		User::Leave(-1);
            }
            
        // Test the display data:
        TInt displayCount = displayData->Count();
        
        if (displayCount != matchesSum)
            {
            iLog->Log(KNumOfMatchesErr);
    		User::Leave(-1);

            }
            
        TInt count = 0;
        TInt dbIndex = 0;
        TInt categoryId = 0;
        TPtrC categoryName;
        TBuf<100> categoryName2;
        _LIT(KErrCatMismatch, "Category wrong in display data");
        _LIT(KErrorWrongType, "Display data doesn't indicate ECategoryItem when it should.");
        
        for (count = 0; count < displayCount; count++)
            {
            CPosLmDisplayItem& displayItem = displayData->DisplayItem(count);
            
            // Check that the item indicates that it contains a category
            AssertTrueSecL(displayItem.DisplayItemType() == CPosLmDisplayItem::ECategoryItem, KErrorWrongType);
            const CPosLandmarkCategory& displayCategory = displayItem.Category();
            dbIndex = displayItem.DatabaseIndex();
            categoryId = displayCategory.CategoryId();
            
            // Check that the db index is correct
            AssertThatIdIsInCorrectDbL(dbIndex, categoryId);
            
            // Check that this match is correct
            AssertTrueSecL(aSearchResults[i]->FindSearchResult(categoryId), KNotFoundErr, categoryId);
            displayCategory.GetCategoryName(categoryName);
            categoryName2 = (*iDbArray)[categoryId - 1]; // Compensate for 0-index
            
            // Check that the category name is correct
            AssertTrueSecL(categoryName == categoryName2, KErrCatMismatch);
            }
        
        
        CPosLmItemIterator* iter = NULL;
        
        for (count = 0; count < 5; count++)
            {
            iter = iLandmarksMultiDbSearch->MatchIteratorL(count);
            CleanupStack::PushL(iter);
            
            TPosLmItemId id = iter->NextL();
            
            while (id != KPosLmNullItemId)
                {
                AssertThatIdIsInCorrectDbL(count, id);
                AssertTrueSecL(aSearchResults[i]->FindSearchResult(id), KNotFoundErr, id);
                id = iter->NextL();            
                }
            CleanupStack::PopAndDestroy(iter);
            iter = NULL;
            }
        
        CleanupStack::PopAndDestroy(nameCriteria);
        }
        iLandmarksMultiDbSearch->UnsetDisplayData();
        CleanupStack::PopAndDestroy(displayData);
    }

// ---------------------------------------------------------
// CPosTp125::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::SearchWithSortOrderL(
    const RPointerArray<CCategorySearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KErrorInSorting, "Error in sorting");
    
    CPosLmCategoryManager::TCategorySortPref sortPref;
    
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);

    for (TInt j=0;j<2;j++)
        {
        for(TInt i=0; i<aSearchResults.Count(); i++)
            {   
            CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
            nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());
            
            if (j==0) 
                {
                sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
                iOperation = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, sortPref);
                }
            else 
                {
                sortPref = CPosLmCategoryManager::ECategorySortOrderNameDescending;
                iOperation = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, sortPref);
                }
            
            // The execution flow
            switch (aExecutionMode)
                {
                case ESynchronous:
                    ExecuteAndDeleteLD(iOperation);
                    break;
                case EAsynchronous:
                    RunAsyncOperationLD(iOperation);
                    break;
                }
                
            TInt matchesSum = 0;
            for (TInt count = 0; count < 5; count++)
                {
                matchesSum += iLandmarksMultiDbSearch->NumOfMatches(count);
                }

            if ((TUint)(aSearchResults[i]->SearchResult()).Count() != matchesSum)
                {
               	iLog->Log(KNumOfMatchesErr);
    			User::Leave(-1);
                }

            TInt displayCount = displayData->Count();
            
            if (displayCount != matchesSum)
                {
                iLog->Log(KNumOfMatchesErr);
    			User::Leave(-1);
                }
             
            // Test the display data:
            // Create a sorted list out of the search result
            RArray<TPosLmItemId> searchResults = aSearchResults[i]->SearchResult();
            // To avoid User-44 create a copy of the aSearchResults[i]
            RArray<TPosLmItemId> searchResultsSorted;
            CleanupClosePushL(searchResultsSorted);
            
            for (TInt kk=0;kk<searchResults.Count();kk++)
                {
                searchResultsSorted.Append(searchResults[kk]);
                }
            
            CDesCArrayFlat* sortedArray = NULL;
            
            SortResultsL(searchResultsSorted, sortedArray);
            CleanupStack::PopAndDestroy(); // searchResultsSorted
            CleanupStack::PushL(sortedArray);
            
            if (sortPref == CPosLmCategoryManager::ECategorySortOrderNameDescending)
                {
                InvertArrayL(sortedArray);
                }
            
            TInt count = 0;
            TInt categoryId = 0;
            TBuf<100> categoryName2;
            TBuf<100> categoryName3;
            
            for (count = 0; count < displayCount; count++)
                {
                CPosLmDisplayItem& displayItem = displayData->DisplayItem(count);
                const CPosLandmarkCategory& displayCategory = displayItem.Category();
                categoryId = displayCategory.CategoryId();
                categoryName2 = (*iDbArray)[categoryId - 1]; // Compensate for 0-index
                categoryName3 = (*sortedArray)[count];
                // Check that sort order is correct
                AssertTrueSecL(categoryName3 == categoryName2, _L("Error in sorting"));
                }
                
            CleanupStack::PopAndDestroy(sortedArray);   
                
            typedef RArray<TInt> myArrayType;
            RArray<myArrayType> resultArray;
            myArrayType array1;
            myArrayType array2;
            myArrayType array3;
            myArrayType array4;
            myArrayType array5;
            
            CleanupClosePushL(resultArray);
            CleanupClosePushL(array1);
            CleanupClosePushL(array2);
            CleanupClosePushL(array3);
            CleanupClosePushL(array4);
            CleanupClosePushL(array5);
            
            if (sortPref == CPosLmCategoryManager::ECategorySortOrderNameAscending)
                {                        
                array1.AppendL(5);
                array1.AppendL(4);
                array1.AppendL(2);
                array1.AppendL(3);
                array1.AppendL(1);
                
                array2.AppendL(7);
                array2.AppendL(13);
                array2.AppendL(6);
                array2.AppendL(11);
                array2.AppendL(12);
                array2.AppendL(10);
                array2.AppendL(9);
                array2.AppendL(14);
                array2.AppendL(8);
                
                array3.AppendL(17);
                array3.AppendL(18);
                array3.AppendL(19);
                array3.AppendL(16);
                array3.AppendL(15);
                array3.AppendL(20);
                
                array4.AppendL(23);
                array4.AppendL(21);
                array4.AppendL(22);
                array4.AppendL(24);
                
                array5.AppendL(26);
                array5.AppendL(25);
                }
            else
                {
                array1.AppendL(1);
                array1.AppendL(3);
                array1.AppendL(2);
                array1.AppendL(4);
                array1.AppendL(5);
                
                array2.AppendL(8);
                array2.AppendL(14);
                array2.AppendL(9);
                array2.AppendL(10);
                array2.AppendL(12);
                array2.AppendL(11);
                array2.AppendL(6);
                array2.AppendL(13);
                array2.AppendL(7);
                
                array3.AppendL(20);
                array3.AppendL(15);
                array3.AppendL(16);
                array3.AppendL(19);
                array3.AppendL(18);
                array3.AppendL(17);
                
                array4.AppendL(24);
                array4.AppendL(22);
                array4.AppendL(21);
                array4.AppendL(23);
                
                array5.AppendL(25);
                array5.AppendL(26);
                }
            
            resultArray.Append(array1);
            resultArray.Append(array2);
            resultArray.Append(array3);
            resultArray.Append(array4);
            resultArray.Append(array5);
            
            CPosLmItemIterator* iter = NULL;
            
            for (TInt dbCount = 0; dbCount < 5; dbCount++)
                {
                iter = iLandmarksMultiDbSearch->MatchIteratorL(dbCount);
                CleanupStack::PushL(iter);
                TPosLmItemId id = iter->NextL();
                TInt itemCount = 0;
                while (id != KPosLmNullItemId)
                    {
                    AssertThatIdIsInCorrectDbL(dbCount, id);
                    // Check that sort order is correct:
                    // Some elements will be missing after the search, so skip those.
                    while (itemCount < resultArray[dbCount].Count()
                           && resultArray[dbCount][itemCount] != id)
                        {
                        itemCount++;
                        }   
                    // Check that a match was found in the loop above.
                    AssertTrueSecL(itemCount < resultArray[dbCount].Count(), KErrorInSorting);        
                    id = iter->NextL();
                    itemCount++;
                    }
                CleanupStack::PopAndDestroy(iter);
                iter = NULL;
                }
            
            CleanupStack::PopAndDestroy(6); // Arrays
            CleanupStack::PopAndDestroy(nameCriteria);
            }
        }
        iLandmarksMultiDbSearch->UnsetDisplayData();
        CleanupStack::PopAndDestroy(displayData);
    }
    

// ---------------------------------------------------------
// CPosTp125::SearchWithMaximumL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::SearchWithMaximumL(
    const RPointerArray<CCategorySearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchWithMaximumL is performed");
    _LIT(KNumOfDisplayMatchesErr, "No. of matches for the display data is incorrect when SearchWithMaximumL is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");

    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);
    
    // Set max nr of matches from 1 to 10 and check that correct nr of matches for each value
    for (TInt j = 1; j <= 10; j++)
        {
        nameCriteria->SetSearchPatternL(_L("*"));
        iLandmarksMultiDbSearch->SetMaxNumOfMatches(j);
        iOperation = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone); 
        
        // The execution flow
        switch (aExecutionMode)
            {
            case ESynchronous:
                ExecuteAndDeleteLD(iOperation);
                break;
            case EAsynchronous:
                RunAsyncOperationLD(iOperation);
                break;
            }
        
        TInt displayCount = displayData->Count();
        
        switch(j)
            {
            case 1:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 1, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 1, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 1, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 1, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 1, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 5, KNumOfDisplayMatchesErr);
            break;
            case 2:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 2, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 2, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 2, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 2, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 10, KNumOfDisplayMatchesErr);
            break;
            case 3:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 3, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 3, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 3, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 3, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 14, KNumOfDisplayMatchesErr);
            break;
            case 4:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 18, KNumOfDisplayMatchesErr);
            break;
            case 5:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 21, KNumOfDisplayMatchesErr);
            break;
            case 6:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 6, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 6, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 23, KNumOfDisplayMatchesErr);
            break;
            case 7:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 7, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 6, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 24, KNumOfDisplayMatchesErr);
            break;
            case 8:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 8, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 6, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 25, KNumOfDisplayMatchesErr);
            break;
            case 9:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 9, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 6, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 26, KNumOfDisplayMatchesErr);
            break;
            case 10:
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 5, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 9, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 6, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 4, KNumOfMatchesErr);
            AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 2, KNumOfMatchesErr);
            AssertTrueSecL(displayCount == 26, KNumOfDisplayMatchesErr);
            break;
            default:
            break;
            }
            
        // Check that the DB:s actually contains correct data:
        CPosLmItemIterator* iter = NULL;
        
        for (TInt count = 0; count < 5; count++)
            {
            iter = iLandmarksMultiDbSearch->MatchIteratorL(count);
            CleanupStack::PushL(iter);
            
            TPosLmItemId id = iter->NextL();
            
            while (id != KPosLmNullItemId)
                {
                AssertThatIdIsInCorrectDbL(count, id);
                // Use search result for position 0, should always be for *
                AssertTrueSecL(aSearchResults[0]->FindSearchResult(id), KNotFoundErr, id);
                id = iter->NextL();            
                }
            CleanupStack::PopAndDestroy(iter);
            iter = NULL;
            }    
        }
    
    iLandmarksMultiDbSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
    CleanupStack::PopAndDestroy(nameCriteria);
    }


// ---------------------------------------------------------
// CPosTp125::SearchAfterAddedCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::SearchAfterAddedCategoryL()
    {
    // _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchAfterAddedCategoryL is performed");
    _LIT(KCategoryNotFound, "Added category could not be found");
    _LIT(KCategory, "StoraKoncernen");
    
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);

    //iDatabase = CP
    iDatabase = CPosLandmarkDatabase::OpenL(KDb40);
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();

    CPosLandmarkCategory* cat1 = CPosLandmarkCategory::NewLC();

    // First do a search with wildcard
    nameCriteria->SetSearchPatternL(_L("S*"));  
    ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 

    // Then add a new category with a name starting with S
    cat1->SetCategoryNameL(KCategory);
    TPosLmItemId categoryId = categoryManager->AddCategoryL(*cat1);
    CleanupStack::PopAndDestroy(cat1);

    // Then use do another search within the results of previous search
    // should return Stadsbibliotek and Stadspark
    nameCriteria->SetSearchPatternL(_L("St*"));
    ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone, ETrue)); 
    
    _LIT(KErrorNrOfMatchesRefined, "Wrong nr matches after refined search after adding a category.");
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 1, KErrorNrOfMatchesRefined); // Stadsbibliotek
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 1, KErrorNrOfMatchesRefined); // Stadspark
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(displayData->Count() == 2, KErrorNrOfMatchesRefined);
    // Set a maximum number of matches, should return only one of Stadsbibliotek and Stadspark
    iLandmarksMultiDbSearch->SetMaxNumOfMatches(1);

    // Do another search just to see that it is possible to find the added category
    nameCriteria->SetSearchPatternL(KCategory);
    ExecuteAndDeleteLD(iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 

    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == 1, KErrorNrOfMatchesRefined); // StoraKoncernen
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == 0, KErrorNrOfMatchesRefined);
    AssertTrueSecL(displayData->Count() == 1, KErrorNrOfMatchesRefined);
    
    CPosLmItemIterator* iter = iLandmarksMultiDbSearch->MatchIteratorL(1);
    CleanupStack::PushL(iter);
    TPosLmItemId id = iter->NextL();
    if (categoryId != id) 
    {
    	iLog->Log(KCategoryNotFound);
    	User::Leave(-1);
    }

    // Remove added category
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(categoryId));

    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(nameCriteria);
    CleanupStack::PopAndDestroy(categoryManager);
    delete iDatabase;
    iDatabase = NULL;
    
    iLandmarksMultiDbSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
    }


// ---------------------------------------------------------
// CPosTp125::SearchGlobalL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::SearchGlobalL(const RPointerArray<CCategorySearchResult>& aSearchResults, const TBool& aSync)
    {
    iLog->Log(_L("SearchGlobalL"));
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchGlobalL is performed");

    // Nr of global categories, defined in resource file
    //const TInt KNrOfGlobalCategories = 8;

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    CPosLmCategoryManager::TCategorySortPref sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
    
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);
    
    for (TInt l=0;l<2;l++)
    {
        if (l==0) sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
        else if (l==1) sortPref = CPosLmCategoryManager::ECategorySortOrderNameDescending;

        // i=0 the first entry in aSearchResults should be wildcard "*"
        TInt i = 0;
        // First do a search with wildcard
        nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());
        iOperation = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, sortPref);
        
        // The asynchronous flow
        if (aSync)
        {
            RunAsyncOperationLD(iOperation);
        }
        else
        {
            ExecuteAndDeleteLD(iOperation);
        }
        
       
	    SetupDbArrayL();
                
     iDbArray->Sort();
        
     if (sortPref == CPosLmCategoryManager::ECategorySortOrderNameDescending)
        {
            InvertArrayL(iDbArray);
        }

        //TUint expNr = (aSearchResults[i]->SearchResult()).Count() + KNrOfGlobalCategories;
        TUint expNr = (aSearchResults[i]->SearchResult()).Count();
        
        
        TInt matchesSum = 0;
        for (TInt count = 0; count < 5; count++)
            {
            matchesSum += iLandmarksMultiDbSearch->NumOfMatches(count);
            }
       
        if (expNr != matchesSum)
            {
            iLog->Log(KNumOfMatchesErr);
    		 		User::Leave(-1);
            }
            
        TInt displayCount = displayData->Count();
        
        if (displayCount != matchesSum)
            {
            iLog->Log(KNumOfMatchesErr);
    				User::Leave(-1);
            }
        
        TInt count = 0;
        TBuf<100> categoryName2;
        TPtrC categoryName;
       	RPointerArray<CCategorySearchResult> searchResults;
      
        for (count = 0; count < displayCount; count++)
            {
            CPosLmDisplayItem& displayItem = displayData->DisplayItem(count);
            const CPosLandmarkCategory& displayCategory = displayItem.Category();
            displayCategory.GetCategoryName(categoryName);
            categoryName2 = (*iDbArray)[count];
            // Check that sort order is correct
            AssertTrueSecL(categoryName == categoryName2, _L("Error in sorting"));
            }

        } // end for (TInt l=0 ...)
        
    iLandmarksMultiDbSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
    CleanupStack::PopAndDestroy(nameCriteria);
    CleanupStack::PopAndDestroy(categoryManager);
    iLog->Log(_L("SearchGlobalL Done"));
    }

// ---------------------------------------------------------
// CPosTp24::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp125::SearchAndCancelL(const TBool& aInCallback)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KCancelErr, "The search is canceled after progress has increased but no search result is found");

    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);
    
    // Test search with one criteria
    const TInt KIndex=0;

    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(iCategorySearchResults[KIndex]->TextCriteria());

    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iLandmarksMultiDbSearch->StartCategorySearchL(
                *nameCriteria, 
                CPosLmCategoryManager::ECategorySortOrderNone, 
                iCategorySearchResults[KIndex]->Redefined()));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLandmarksMultiDbSearch->StartCategorySearchL(
                *nameCriteria, 
                CPosLmCategoryManager::ECategorySortOrderNone, 
                iCategorySearchResults[KIndex]->Redefined()));
        }
 
    
    
    if (aInCallback)
        {
        CPosLmItemIterator* iter0 = iLandmarksMultiDbSearch->MatchIteratorL(0);
        CleanupStack::PushL(iter0);
        CPosLmItemIterator* iter1 = iLandmarksMultiDbSearch->MatchIteratorL(1);
        CleanupStack::PushL(iter1);
        CPosLmItemIterator* iter2 = iLandmarksMultiDbSearch->MatchIteratorL(2);
        CleanupStack::PushL(iter2);
        CPosLmItemIterator* iter3 = iLandmarksMultiDbSearch->MatchIteratorL(3);
        CleanupStack::PushL(iter3);
        CPosLmItemIterator* iter4 = iLandmarksMultiDbSearch->MatchIteratorL(4);
        CleanupStack::PushL(iter4);
        
        AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(0) == iter0->NumOfItemsL(), KNumOfMatchesErr);
        AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(1) == iter1->NumOfItemsL(), KNumOfMatchesErr);
        AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(2) == iter2->NumOfItemsL(), KNumOfMatchesErr);
        AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(3) == iter3->NumOfItemsL(), KNumOfMatchesErr);
        AssertTrueSecL(iLandmarksMultiDbSearch->NumOfMatches(4) == iter4->NumOfItemsL(), KNumOfMatchesErr);

        AssertTrueSecL(displayData->Count() > 0, KNumOfMatchesErr);

        AssertTrueSecL((iter0->NumOfItemsL() + 
                        iter0->NumOfItemsL() + 
                        iter0->NumOfItemsL() + 
                        iter0->NumOfItemsL() + 
                        iter0->NumOfItemsL()) == displayData->Count(), KCancelErr);
             
        CleanupStack::PopAndDestroy(5);
        }
        
    
        
    CleanupStack::PopAndDestroy(nameCriteria);
    iLandmarksMultiDbSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
    }
    
void CPosTp125::TestDuplicateGlobalCategoriesL(const RPointerArray<CCategorySearchResult>& aSearchResults, const TBool& aSync)
    {
    iLog->Log(_L("SearchGlobalL with duplicates"));
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchGlobalL with duplicates is performed");

    // Nr of global categories, defined in resource file
    //const TInt KNrOfGlobalCategories = 15 * 2;

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    CPosLmCategoryManager::TCategorySortPref sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
    
    CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
    CleanupStack::PushL(displayData);
    iLandmarksMultiDbSearch->SetDisplayData(*displayData);
    
    for (TInt l=0;l<2;l++)
    {
        if (l==0) sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
        else if (l==1) sortPref = CPosLmCategoryManager::ECategorySortOrderNameDescending;

        // i=0 the first entry in aSearchResults should be wildcard "*"
        TInt i = 0;
        // First do a search with wildcard
        nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());
        iOperation = iLandmarksMultiDbSearch->StartCategorySearchL(*nameCriteria, sortPref);
        
        // The asynchronous flow
        if (aSync)
        {
            RunAsyncOperationLD(iOperation);
        }
        else
        {
            ExecuteAndDeleteLD(iOperation);
        }
                
        SetupDbArrayL();
        
        iDbArray->Sort();
        
        if (sortPref == CPosLmCategoryManager::ECategorySortOrderNameDescending)
            {
            InvertArrayL(iDbArray);
            }

        TUint expNr = (aSearchResults[i]->SearchResult()).Count();
        
        TInt matchesSum = 0;
        for (TInt count = 0; count < 5; count++)
            {
            matchesSum += iLandmarksMultiDbSearch->NumOfMatches(count);
            }
        
        if (expNr != matchesSum)
            {
            iLog->Log(KNumOfMatchesErr);
    		User::Leave(-1);
            }
            
        TInt displayCount = displayData->Count();
        
        if (displayCount != matchesSum)
            {
            iLog->Log(KNumOfMatchesErr);
    		User::Leave(-1);
            }
        
        TInt count = 0;
        
        TBuf<100> categoryName2;
        TPtrC categoryName;
        
        for (count = 0; count < displayCount; count++)
            {
            CPosLmDisplayItem& displayItem = displayData->DisplayItem(count);
            const CPosLandmarkCategory& displayCategory = displayItem.Category();
            displayCategory.GetCategoryName(categoryName);
            categoryName2 = (*iDbArray)[count];
            // Check that sort order is correct
            AssertTrueSecL(categoryName == categoryName2, _L("Error in sorting"));
            }

        } // end for (TInt l=0 ...)
        
    iLandmarksMultiDbSearch->UnsetDisplayData();
    CleanupStack::PopAndDestroy(displayData);
    CleanupStack::PopAndDestroy(nameCriteria);
    CleanupStack::PopAndDestroy(categoryManager);
    iLog->Log(_L("SearchGlobalL Done"));
    }

void CPosTp125::RemoveExtraCategoriesL()
    {
    TInt err = KErrNone;
    iDatabase = CPosLandmarkDatabase::OpenL(KDb20);
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());               
       }       
       
    iLog->Log(_L(" Default database opened and initialised "));     
    
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(catMan);
    
    ExecuteAndDeleteLD(catMan->ResetGlobalCategoriesL());    
    
    RemoveCategoryL(catMan, 6);
    RemoveCategoryL(catMan, 7);
    RemoveCategoryL(catMan, 8);
    RemoveCategoryL(catMan, 9);
    RemoveCategoryL(catMan, 10);
    RemoveCategoryL(catMan, 11);
    RemoveCategoryL(catMan, 12);
    RemoveCategoryL(catMan, 13);
    RemoveCategoryL(catMan, 14);
    RemoveCategoryL(catMan, 15);
    RemoveCategoryL(catMan, 16);
    RemoveCategoryL(catMan, 17);
    RemoveCategoryL(catMan, 18);
    RemoveCategoryL(catMan, 19);
    RemoveCategoryL(catMan, 20);
    RemoveCategoryL(catMan, 21);
    RemoveCategoryL(catMan, 22);
    RemoveCategoryL(catMan, 23);
    RemoveCategoryL(catMan, 24);
    RemoveCategoryL(catMan, 25);
    RemoveCategoryL(catMan, 26);
    
    
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(3000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(6000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(9000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(12000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(15000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(18000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(21000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(24000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(27000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(30000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(33000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(36000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(39000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(42000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(45000));
    
    CleanupStack::PopAndDestroy(catMan);
    delete iDatabase;
    iDatabase = NULL;
     
    
    iDatabase = CPosLandmarkDatabase::OpenL(KDb40);
    if ( iDatabase->IsInitializingNeeded() )
        {
        TRAP( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL( err == KErrNone, _L("Init db failed"));
        }
    catMan = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(catMan);
    
    
    RemoveCategoryL(catMan, 1);
    RemoveCategoryL(catMan, 2);
    RemoveCategoryL(catMan, 3);
    RemoveCategoryL(catMan, 4);
    RemoveCategoryL(catMan, 5);
    // Existing categories here
    RemoveCategoryL(catMan, 15);
    RemoveCategoryL(catMan, 16);
    RemoveCategoryL(catMan, 17);
    RemoveCategoryL(catMan, 18);
    RemoveCategoryL(catMan, 19);
    RemoveCategoryL(catMan, 20);
    RemoveCategoryL(catMan, 21);
    RemoveCategoryL(catMan, 22);
    RemoveCategoryL(catMan, 23);
    RemoveCategoryL(catMan, 24);
    RemoveCategoryL(catMan, 25);
    RemoveCategoryL(catMan, 26);

    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(3000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(6000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(9000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(12000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(15000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(18000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(21000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(24000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(27000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(30000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(33000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(36000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(39000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(42000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(45000));
    
    CleanupStack::PopAndDestroy(catMan);
    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = CPosLandmarkDatabase::OpenL(KDb60);
    if (  iDatabase->IsInitializingNeeded() )
        {
        TRAP( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL( err == KErrNone, _L("Init db failed"));
        }
    catMan = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(catMan);
    
    RemoveCategoryL(catMan, 1);
    RemoveCategoryL(catMan, 2);
    RemoveCategoryL(catMan, 3);
    RemoveCategoryL(catMan, 4);
    RemoveCategoryL(catMan, 5);
    RemoveCategoryL(catMan, 6);
    RemoveCategoryL(catMan, 7);
    RemoveCategoryL(catMan, 8);
    RemoveCategoryL(catMan, 9);
    RemoveCategoryL(catMan, 10);
    RemoveCategoryL(catMan, 11);
    RemoveCategoryL(catMan, 12);
    RemoveCategoryL(catMan, 13);
    RemoveCategoryL(catMan, 14);
    // Existing categories here
    RemoveCategoryL(catMan, 21);
    RemoveCategoryL(catMan, 22);
    RemoveCategoryL(catMan, 23);
    RemoveCategoryL(catMan, 24);
    RemoveCategoryL(catMan, 25);
    RemoveCategoryL(catMan, 26);
    
    
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(3000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(6000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(9000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(12000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(15000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(18000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(21000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(24000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(27000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(30000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(33000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(36000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(39000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(42000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(45000));
    
    CleanupStack::PopAndDestroy(catMan);
    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = CPosLandmarkDatabase::OpenL(KDb80);
    if ( iDatabase->IsInitializingNeeded() )
        {
        TRAP( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL( err == KErrNone, _L("Init db failed"));
        }
    catMan = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(catMan);
    
    RemoveCategoryL(catMan, 1);
    RemoveCategoryL(catMan, 2);
    RemoveCategoryL(catMan, 3);
    RemoveCategoryL(catMan, 4);
    RemoveCategoryL(catMan, 5);
    RemoveCategoryL(catMan, 6);
    RemoveCategoryL(catMan, 7);
    RemoveCategoryL(catMan, 8);
    RemoveCategoryL(catMan, 9);
    RemoveCategoryL(catMan, 10);
    RemoveCategoryL(catMan, 11);
    RemoveCategoryL(catMan, 12);
    RemoveCategoryL(catMan, 13);
    RemoveCategoryL(catMan, 14);
    RemoveCategoryL(catMan, 15);
    RemoveCategoryL(catMan, 16);
    RemoveCategoryL(catMan, 17);
    RemoveCategoryL(catMan, 18);
    RemoveCategoryL(catMan, 19);
    RemoveCategoryL(catMan, 20);
    // Existing categories here
    RemoveCategoryL(catMan, 25);
    RemoveCategoryL(catMan, 26);
    
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(3000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(6000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(9000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(12000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(15000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(18000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(21000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(24000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(27000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(30000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(33000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(36000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(39000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(42000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(45000));
    
    CleanupStack::PopAndDestroy(catMan);
    delete iDatabase;
    iDatabase = NULL;
    
    iDatabase = CPosLandmarkDatabase::OpenL(KDb105);
    if ( iDatabase->IsInitializingNeeded() )
        {
        TRAP( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL( err == KErrNone, _L("Init db failed"));
        }
    catMan = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(catMan);
    
    RemoveCategoryL(catMan, 1);
    RemoveCategoryL(catMan, 2);
    RemoveCategoryL(catMan, 3);
    RemoveCategoryL(catMan, 4);
    RemoveCategoryL(catMan, 5);
    RemoveCategoryL(catMan, 6);
    RemoveCategoryL(catMan, 7);
    RemoveCategoryL(catMan, 8);
    RemoveCategoryL(catMan, 9);
    RemoveCategoryL(catMan, 10);
    RemoveCategoryL(catMan, 11);
    RemoveCategoryL(catMan, 12);
    RemoveCategoryL(catMan, 13);
    RemoveCategoryL(catMan, 14);
    RemoveCategoryL(catMan, 15);
    RemoveCategoryL(catMan, 16);
    RemoveCategoryL(catMan, 17);
    RemoveCategoryL(catMan, 18);
    RemoveCategoryL(catMan, 19);
    RemoveCategoryL(catMan, 20);
    RemoveCategoryL(catMan, 21);
    RemoveCategoryL(catMan, 22);
    RemoveCategoryL(catMan, 23);
    RemoveCategoryL(catMan, 24);

    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(3000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(6000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(9000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(12000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(15000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(18000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(21000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(24000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(27000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(30000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(33000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(36000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(39000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(42000));
    RemoveCategoryL(catMan, catMan->GetGlobalCategoryL(45000));
    
    CleanupStack::PopAndDestroy(catMan);
    delete iDatabase;
    iDatabase = NULL;
    }
    

 void CPosTp125::RemoveCategoryL(CPosLmCategoryManager* aCatMan,
                                const TPosLmItemId aCatId)
    {    
    ExecuteAndDeleteLD(aCatMan->RemoveCategoryL(aCatId));
    }
    
void CPosTp125::AssertThatIdIsInCorrectDbL(TInt aDbIndex, TInt aId)
    {
    _LIT(KErrorWrongDb, "Id returned from wrong db");
    _LIT(KIllegalIndex, "Illegal index sent to AssertThatIdIsInCorrectDbL()");
    switch(aDbIndex)
        {
        case 0:
        AssertTrueSecL(aId >= 1 && aId <= 5, KErrorWrongDb);
        break;
        case 1:
        AssertTrueSecL(aId >= 6 && aId <= 14, KErrorWrongDb);
        break;
        case 2:
        AssertTrueSecL(aId >= 15 && aId <= 20, KErrorWrongDb);
        break;
        case 3:
        AssertTrueSecL(aId >= 21 && aId <= 24, KErrorWrongDb);
        break;
        case 4:
        AssertTrueSecL(aId >= 25 && aId <= 26, KErrorWrongDb);
        break;
        default:
        iLog->Log(KIllegalIndex);
    	User::Leave(-1);
        break;        
        
        }
    }
    
void CPosTp125::SetupDbArrayL()
    {
    iDbArray = NULL;
    // This array is used when testing display data.
    iDbArray = new(ELeave) CDesCArrayFlat(8);
    
    // Build the array so that the array index is equivalent with the db index, except that this array is 0-based.
    iDbArray->AppendL(KPizzeria);
    iDbArray->AppendL(KHambergerbar);
    iDbArray->AppendL(KKinarestaurang);
    iDbArray->AppendL(KGaraget);
    iDbArray->AppendL(KBilverkstad);
    iDbArray->AppendL(KPastaRestaurang);
    iDbArray->AppendL(KBilia);
    iDbArray->AppendL(KVolksWagen);
    iDbArray->AppendL(KTaxiGoteborg);
    iDbArray->AppendL(KTaxi);
    iDbArray->AppendL(KStadsbibliotek);
    iDbArray->AppendL(KSystembolag);
    iDbArray->AppendL(KNokiKontor);
    iDbArray->AppendL(KTietoEnatorKontor);
    iDbArray->AppendL(KThaiRestaurang);
    iDbArray->AppendL(KMcDonalds);
    iDbArray->AppendL(KBurgerKing);
    iDbArray->AppendL(KFrisor);
    iDbArray->AppendL(KHunddagis);
    iDbArray->AppendL(KVeterinar);
    iDbArray->AppendL(KKyrkogard);
    iDbArray->AppendL(KMuseum);
    iDbArray->AppendL(KDagis);
    iDbArray->AppendL(KStadspark);
    iDbArray->AppendL(KSjukhus);
    iDbArray->AppendL(KLakare);
    }
    
void CPosTp125::SortResultsL(RArray<TPosLmItemId>& aArray,
                             CDesCArrayFlat*& aResultArray)
    {
    TInt count = 0;
    aResultArray = new(ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(aResultArray);
    for (count = 0 ; count < aArray.Count(); count++)
        {
        TInt id = aArray[count];
        aResultArray->AppendL((*iDbArray)[id - 1]); // compensate for 0-based array
        }
        
    aResultArray->Sort();
    CleanupStack::Pop(aResultArray);
    }
    
void CPosTp125::InvertArrayL(CDesCArrayFlat*& aArray)
    {
    TInt maxCount = aArray->Count();
    TInt count = 0;
    
    CDesCArrayFlat* copyArray = new(ELeave) CDesCArrayFlat(8);
    CleanupStack::PushL(copyArray);
    
    for (count = 0; count < maxCount; count++)
        {
        copyArray->AppendL((*aArray)[count]);
        }
        
    aArray->Reset();
    for (count = 0; count < maxCount; count++)
        {
        aArray->AppendL((*copyArray)[maxCount - count - 1]);
        }
        
    CleanupStack::PopAndDestroy(copyArray);
    }

//  End of File
