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
#include "FT_CPosTp25.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLMItemIterator.h>    
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
#include <EPos_TPosLMSortPref.h>
#include "FT_CSearchResult.h"
#include <EPos_Landmarks.h>
#include "FT_LandmarkConstants.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp25::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    delete iDatabase;
    iDatabase=NULL;

    iCategorySearchResults.ResetAndDestroy();

    }

// ---------------------------------------------------------
// CPosTp25::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::StartL()
    {
    _LIT(KEmptyTextCriteriaErr, "An empty string as text criteria should not be allowed");
    _LIT(KEmptyTextCriteriaErr1, "An empty text criteria should not be allowed");
    // copy test db to the private path
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(KTp25TestDb);

    TInt er = fileMan->Copy(srcPath, KLmTp25DefaultDbPath,
            CFileMan::EOverWrite);
    if (er != KErrNone)
        iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    
    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KTp25TestDb);
    CleanupStack::PopAndDestroy(manager);

    iDatabase = CPosLandmarkDatabase::OpenL(KTp25TestDb);
    if (iDatabase->IsInitializingNeeded())
        {
        TRAPD( err, ExecuteAndDeleteLD( iDatabase->InitializeL() ) );
        AssertTrueSecL(err == KErrNone, _L("Init db failed"));
        }
    
    

    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
        
    TInt err = KErrNone;
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
       
    // Start by testing some error flows, returning KErrArgument
    TRAP(err, iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr1);


    // Test with a landmark search criteria into a category search operation
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    textCriteria->SetTextL(_L("*"));
    TRAP(err, iLandmarkSearch->StartCategorySearchL(*textCriteria, CPosLmCategoryManager::ECategorySortOrderNone));
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);
    CleanupStack::PopAndDestroy(textCriteria);

    // Test empty string
    nameCriteria->SetSearchPatternL(_L(""));   
    TRAP(err, iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone));
    AssertTrueSecL(err == KErrArgument, KEmptyTextCriteriaErr);
    CleanupStack::PopAndDestroy(nameCriteria);
    
    // test with maximum nr of characters in search pattern
    TestSearchPatternLengthL();
    
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

	// Search asynchronously with User::WaitForRequest()
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asyncronously using User::WaitForRequest()"));
    SearchL(iCategorySearchResults, EWaitForRequest);

    iLog->Log(_L("Testing search with sort order asynchronously using User::WaitForRequest()"));
    SearchWithSortOrderL(iCategorySearchResults, EWaitForRequest);

    iLog->Log(_L("Testing search with maximum nr of matches asynchronously using User::WaitForRequest()"));
    SearchWithMaximumL(iCategorySearchResults, EWaitForRequest);

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
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    iDatabase = UseGlobalCategoriesL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
       
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);

    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);   
    ExecuteAndDeleteLD(categoryManager->ResetGlobalCategoriesL());
    CleanupStack::PopAndDestroy(categoryManager);

    iLog->Log(_L("Testing search with global categories included, sync"));
    SearchGlobalL(iCategorySearchResults, ETrue);

    iLog->Log(_L("Testing search with global categories included, asyncronously"));
    SearchGlobalL(iCategorySearchResults, EFalse);

    // Verify error report "ESLI-62DBXR Landmarks - Sort order is not correctly implemented"
    // Check that CPosLandmarkSearch::MatchIteratorL are CPosLandmarkDatabase::LandmarkIteratorL
    // are sorted correctly (should be identically sorted)
    VerifySortOrderL();

    RemoveGlobalCategoriesL();
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    }

// ---------------------------------------------------------
// CPosTp25::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::SearchL(
    const RPointerArray<CCategorySearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");

    for(TInt i=0; i<aSearchResults.Count(); i++)
        {   
        CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
        nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());
        
        CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
		    CleanupStack::PushL(displayData);
        iLandmarkSearch->SetDisplayData(*displayData);

        iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone); 

        // The execution flow
        switch (aExecutionMode)
            {
            case ESynchronous:
                ExecuteAndDeleteLD(iOperation);
                break;
            case EAsynchronous:
                RunAsyncOperationLD(iOperation);
                break;
            case EWaitForRequest:
                RunAsyncOperationByWaitForReqL(iOperation);
                delete iOperation;
                break;
            }

        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        
        if ((TUint)(aSearchResults[i]->SearchResult()).Count() != iLandmarkSearch->NumOfMatches())
            {
            _LIT(KExpected, "Expected no. of matches: %d");
            _LIT(KReturned, "Returned no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, (aSearchResults[i]->SearchResult()).Count());
            iLog->Log(info);
            info.Format(KReturned, iLandmarkSearch->NumOfMatches());
            iLog->Log(info);
            iLog->Log(_L("Name criteria: "));
            iLog->Log(nameCriteria->SearchPattern());
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }

        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            AssertTrueSecL(aSearchResults[i]->FindSearchResult(id), KNotFoundErr, id);

            id = iter->NextL();
            }

		// Check DisplayData below                
        TInt nrr = displayData->Count();
        
            if ((TUint)(aSearchResults[i]->SearchResult()).Count() != displayData->Count())
            {
            _LIT(KExpected, "Expected no. of displaydata matches: %d");
            _LIT(KReturned, "Returned no. of displaydata matches: %d");
            TBuf<100> info;
            info.Format(KExpected, (aSearchResults[i]->SearchResult()).Count());
            iLog->Log(info);
            info.Format(KReturned, displayData->Count());
            iLog->Log(info);
            iLog->Log(_L("Name criteria: "));
            iLog->Log(nameCriteria->SearchPattern());
                        
            iLog->Log(KNumOfMatchesErr);
            User::Leave(-1);
            }

        TInt itemId = displayData->NewItemIndex();
        while (itemId != KPosLmNoNewItems)
            {
            CPosLmDisplayItem& item = displayData->DisplayItem(itemId);
            
            if (item.DisplayItemType() != CPosLmDisplayItem::ECategoryItem)
            	{
            	iLog->Log(_L("ERROR :::::"));
            	iLog->Log(_L("Wrong item returned, only ECategoryItem should be returned"));
            	User::Leave(-1);
            	}
            
            if (item.DisplayItemType() == CPosLmDisplayItem::ELandmarkItem)
            	{
            	iLog->Log(_L("Is ELandmarkItem:::::"));
            	}
            
            if (item.DatabaseIndex() != 0)
            	{
            	// When single search, index should always be 0
            	iLog->Log(_L("Wrong DatabaseIndex, should be 0"));
            	User::Leave(-1);
            	
            	}
			
			      TInt categoryId = item.Category().CategoryId();

            if (!aSearchResults[i]->FindSearchResult(categoryId))
            	{
            	iLog->Log(_L("Not Found"));
            	TPtrC name;
				      item.Category().GetCategoryName(name);
    			    iLog->Log(name);
            	iLog->Log(_L("Could not find LandmarkCategory"));
            	User::Leave(-1);
            	
            	}
            	itemId = displayData->NewItemIndex();
            }
        
        CleanupStack::PopAndDestroy(iter);
				iLandmarkSearch->UnsetDisplayData();
				CleanupStack::PopAndDestroy(displayData);
        CleanupStack::PopAndDestroy(nameCriteria);
        }
    }

// ---------------------------------------------------------
// CPosTp25::TestSearchPatternLengthL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::TestSearchPatternLengthL()
    {
    iLog->Log(_L("TestSearchPatternLengthL"));
    
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();  
    //Test with maximum size of search pattern
    //KPosLmMaxCategoryNameLength is 124 (from EPos_Landmarks.h)
    HBufC* hbuf = HBufC::NewLC(KPosLmMaxCategoryNameLength+1);
    while(hbuf->Des().Length() < KPosLmMaxCategoryNameLength)
        {
        hbuf->Des().Append(_L("*"));
        }
    nameCriteria->SetSearchPatternL(*hbuf);
        
    hbuf->Des().Append(_L("*"));
    TRAPD(err, nameCriteria->SetSearchPatternL(*hbuf));

    AssertTrueSecL(err == KErrArgument, _L("It should not be possible to set search pattern"));            
    
    CleanupStack::PopAndDestroy(hbuf);
    CleanupStack::PopAndDestroy(nameCriteria);
    }

// ---------------------------------------------------------
// CPosTp25::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::SearchWithSortOrderL(
    const RPointerArray<CCategorySearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    
    CPosLmCategoryManager::TCategorySortPref sortPref;

    for (TInt j=0;j<2;j++)
        {
        for(TInt i=0; i<aSearchResults.Count(); i++)
            {   
            CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
            nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());
            
            if (j==0) 
                {
                sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
                iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPref);
                }
            else 
                {
                sortPref = CPosLmCategoryManager::ECategorySortOrderNameDescending;
                iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPref);
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
                case EWaitForRequest:
                    RunAsyncOperationByWaitForReqL(iOperation);
                    delete iOperation;
                    break;
                }
            
            CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
            CleanupStack::PushL(iter);
            
            // Create a sorted list out of the search result
            RArray<TPosLmItemId> searchResults = aSearchResults[i]->SearchResult();
            // To avoid User-44 create a copy of the aSearchResults[i]
            RArray<TPosLmItemId> searchResultsSorted;
            CleanupClosePushL(searchResultsSorted);
            
            for (TInt kk=0;kk<searchResults.Count();kk++)
                {
                searchResultsSorted.Append(searchResults[kk]);
                }

            CategoriesSortL(searchResultsSorted, sortPref);

            if ((TUint)(aSearchResults[i]->SearchResult()).Count() != iLandmarkSearch->NumOfMatches())
                {
                _LIT(KExpected, "Expected no. of matches: %d");
                _LIT(KReturned, "Returned no. of matches: %d");
                TBuf<100> info;
                info.Format(KExpected, (aSearchResults[i]->SearchResult()).Count());
                iLog->Log(info);
                info.Format(KReturned, iLandmarkSearch->NumOfMatches());
                iLog->Log(info);
                iLog->Log(_L("Name criteria: "));
                iLog->Log(nameCriteria->SearchPattern());
                          
                iLog->Log(KNumOfMatchesErr);
            		User::Leave(-1);
            	
                }
            
            TPosLmItemId id = iter->NextL();
            TInt counter=0;
            while (id != KPosLmNullItemId)
                {
                // Check that sort order is correct
                AssertTrueSecL(searchResultsSorted[counter] == id, _L("Error in sorting"));
                id = iter->NextL();
                counter++;
                }
            CleanupStack::PopAndDestroy(&searchResultsSorted);
            CleanupStack::PopAndDestroy(2, nameCriteria);
            }
        }
    }

// ---------------------------------------------------------
// CPosTp25::SearchWithMaximumL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::SearchWithMaximumL(
    const RPointerArray<CCategorySearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchWithMaximumL is performed");
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results when the search is performed syncronously");

    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();

    // First get the maximum number of matches
    nameCriteria->SetSearchPatternL(_L("*"));  
    ExecuteAndDeleteLD(iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 
    TInt max = iLandmarkSearch->NumOfMatches();

    for (TInt j=1;j<=max;j++)
        {
        nameCriteria->SetSearchPatternL(_L("*"));
        iLandmarkSearch->SetMaxNumOfMatches(j);
        iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone); 
        
        // The execution flow
        switch (aExecutionMode)
            {
            case ESynchronous:
                ExecuteAndDeleteLD(iOperation);
                break;
            case EAsynchronous:
                RunAsyncOperationLD(iOperation);
                break;
            case EWaitForRequest:
                RunAsyncOperationByWaitForReqL(iOperation);
                delete iOperation;
                break;
            }

        AssertTrueSecL((TUint)j == iLandmarkSearch->NumOfMatches(), KNumOfMatchesErr);

        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);

        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            // Use search result for position 0, should always be for *
            AssertTrueSecL(aSearchResults[0]->FindSearchResult(id), KNotFoundErr, id);
            id = iter->NextL();
            }
        CleanupStack::PopAndDestroy(iter);
        }
        
    CleanupStack::PopAndDestroy(nameCriteria);
    }

// ---------------------------------------------------------
// CPosTp25::SearchAfterAddedCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::SearchAfterAddedCategoryL()
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchAfterAddedCategoryL is performed");
    _LIT(KCategoryNotFound, "Added category could not be found");
    _LIT(KCategory, "StoraKoncernen");

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();

    CPosLandmarkCategory* cat1 = CPosLandmarkCategory::NewLC();

    // First do a search with wildcard
    nameCriteria->SetSearchPatternL(_L("S*"));  
    ExecuteAndDeleteLD(iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 

    // Then add a new category with a name starting with S
    cat1->SetCategoryNameL(KCategory);
    TPosLmItemId categoryId = categoryManager->AddCategoryL(*cat1);
    CleanupStack::PopAndDestroy(cat1);

    // Then use do another search within the results of previous search
    // should return Stadsbibliotek and Stadspark
    nameCriteria->SetSearchPatternL(_L("St*"));
    ExecuteAndDeleteLD(iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone, ETrue)); 
    TInt max = iLandmarkSearch->NumOfMatches();

    if (max != 2) 
    	{
				iLog->Log(KNumOfMatchesErr);
        User::Leave(-1);
			}
    // Set a maximum number of matches, should return only one of Stadsbibliotek and Stadspark
    iLandmarkSearch->SetMaxNumOfMatches(1);

    // Do another search just to see that it is possible to find the added category
    nameCriteria->SetSearchPatternL(KCategory);
    ExecuteAndDeleteLD(iLandmarkSearch->StartCategorySearchL(*nameCriteria, CPosLmCategoryManager::ECategorySortOrderNone)); 

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
    
    TPosLmItemId id = iter->NextL();
    max = iLandmarkSearch->NumOfMatches();
    
    if (max != 1)
    	{
    		iLog->Log(KNumOfMatchesErr);
        User::Leave(-1);
    	}
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
    }

// ---------------------------------------------------------
// CPosTp25::SearchGlobalL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::SearchGlobalL(const RPointerArray<CCategorySearchResult>& aSearchResults, const TBool& aSync)
    {
    iLog->Log(_L("SearchGlobalL"));
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when SearchGlobalL is performed");

    // Nr of global categories, defined in resource file
    const TInt KNrOfGlobalCategories = 15;

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    CPosLmCategoryManager::TCategorySortPref sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
    
    for (TInt l=0;l<2;l++)
    {
        if (l==0) sortPref = CPosLmCategoryManager::ECategorySortOrderNameAscending;
        else if (l==1) sortPref = CPosLmCategoryManager::ECategorySortOrderNameDescending;

        // i=0 the first entry in aSearchResults should be wildcard "*"
        TInt i = 0;
        // First do a search with wildcard
        nameCriteria->SetSearchPatternL(aSearchResults[i]->TextCriteria());
        iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPref);
        
        // The asynchronous flow
        if (aSync)
        {
            RunAsyncOperationLD(iOperation);
        }
        else
        {
            ExecuteAndDeleteLD(iOperation);
        }
        
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        
        // Create a sorted list out of the search result
        RArray<TPosLmItemId> searchResults = aSearchResults[i]->SearchResult();
        RArray<TPosLmItemId> searchResultsSorted;
        
        for (TInt kk=0;kk<searchResults.Count();kk++)
        {
            searchResultsSorted.Append(searchResults[kk]);
        }
        
			        searchResultsSorted.Append(28);
			        searchResultsSorted.Append(29);
			        searchResultsSorted.Append(30);
			        searchResultsSorted.Append(31);
			        searchResultsSorted.Append(32);
			        searchResultsSorted.Append(33);
			        searchResultsSorted.Append(34);
			        searchResultsSorted.Append(35);
							searchResultsSorted.Append(36);
							searchResultsSorted.Append(37);
							searchResultsSorted.Append(38);
							searchResultsSorted.Append(39);
							searchResultsSorted.Append(40);
							searchResultsSorted.Append(41);
							searchResultsSorted.Append(42);

        // Sort list according to sortPref
        CategoriesSortL(searchResultsSorted, sortPref);
        TUint expNr = (aSearchResults[i]->SearchResult()).Count() + KNrOfGlobalCategories;
        TUint expNr2 = iLandmarkSearch->NumOfMatches();
        
        if (expNr != expNr2)
        {
            _LIT(KExpected, "Expected no. of matches: %d");
            _LIT(KReturned, "Returned no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, expNr);
            iLog->Log(info);
            info.Format(KReturned, expNr2);
            iLog->Log(info);
            iLog->Log(_L("Name criteria: "));
            iLog->Log(nameCriteria->SearchPattern());
            
            iLog->Log(KNumOfMatchesErr);
       		  User::Leave(-1);
        }
        
        TPosLmItemId id = iter->NextL();
        TInt counter=0;
        while (id != KPosLmNullItemId)
        {
            // Check that sort order is correct
            AssertTrueSecL(searchResultsSorted[counter] == id, _L("Error in sorting"));
            id = iter->NextL();
            counter++;
        }
        
        CleanupStack::PopAndDestroy(iter);
        searchResultsSorted.Reset();
        searchResultsSorted.Close();

    } // end for (TInt l=0 ...)
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
void CPosTp25::SearchAndCancelL(const TBool& aInCallback)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KCancelErr, "The search is canceled after progress has increased but no search result is found");

    // Test search with one criteria
    const TInt KIndex=0;

    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(iCategorySearchResults[KIndex]->TextCriteria());
    iLandmarkSearch->SetMaxNumOfMatches();

    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(iLandmarkSearch->StartCategorySearchL(
                *nameCriteria, 
                CPosLmCategoryManager::ECategorySortOrderNone, 
                iCategorySearchResults[KIndex]->Redefined()));
        }
    else
        {
        RunAsyncOperationAndCancelLD(iLandmarkSearch->StartCategorySearchL(
                *nameCriteria, 
                CPosLmCategoryManager::ECategorySortOrderNone, 
                iCategorySearchResults[KIndex]->Redefined()));
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
 
    if (aInCallback)
        {
        if (iter->NumOfItemsL() != iLandmarkSearch->NumOfMatches())
            {
		           	iLog->Log(_L("Iterators no. of items is not equal to LandmarksSearch no. matches"));
		            _LIT(KExpected, "Iter. of matches: %d");
		            _LIT(KReturned, "LandmarksSearch no. of matches: %d");
		            TBuf<100> info;
		            info.Format(KExpected, iter->NumOfItemsL());
		            iLog->Log(info);
		            info.Format(KReturned, iLandmarkSearch->NumOfMatches());
		            iLog->Log(info);
		            iLog->Log(KNumOfMatchesErr);
		       		  User::Leave(-1);
            }
        TBuf<100> info2;
        _LIT(KNof, "No. of matches found before cancel = %d");
        info2.Format(KNof, iter->NumOfItemsL());
        iLog->Log(info2);

        AssertTrueSecL(iter->NumOfItemsL() != 0, KCancelErr);
        }
        
    CleanupStack::PopAndDestroy(2, nameCriteria);   
    }

// ---------------------------------------------------------
// CPosTp25::VerifySortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp25::VerifySortOrderL()
    {
    iLog->Log(_L("VerifySortOrderL"));
    // Search pattern "*" should match all existing landmarks -> it is possible to compare
    // sort order of CPosLandmarkSearch::MatchIteratorL with CPosLandmarkDatabase::LandmarkIteratorL

    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KNumOfMatchesErr2, "No. of matches in DisplayData is incorrect when the search is performed");
    
    _LIT(KSortorderErr, "The sortorder is incorrect");
    _LIT(KNameMatchError, "The landmark category name does not match");

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    // Just in case, add some categories
    CPosLandmarkCategory* cat1 = CPosLandmarkCategory::NewLC();
    cat1->SetCategoryNameL(_L("CategoryA"));
    TPosLmItemId categoryId1 = categoryManager->AddCategoryL(*cat1);
    CleanupStack::PopAndDestroy(cat1);

    cat1 = CPosLandmarkCategory::NewLC();
    cat1->SetCategoryNameL(_L("Categorya"));
    TPosLmItemId categoryId2 = categoryManager->AddCategoryL(*cat1);
    CleanupStack::PopAndDestroy(cat1);

    cat1 = CPosLandmarkCategory::NewLC();
    cat1->SetCategoryNameL(_L("AA"));
    TPosLmItemId categoryId3 = categoryManager->AddCategoryL(*cat1);
    CleanupStack::PopAndDestroy(cat1);

    cat1 = CPosLandmarkCategory::NewLC();
    cat1->SetCategoryNameL(_L("aa"));
    TPosLmItemId categoryId4 = categoryManager->AddCategoryL(*cat1);
    CleanupStack::PopAndDestroy(cat1);

    CleanupStack::PopAndDestroy(categoryManager);

    CPosLmCategoryManager::TCategorySortPref sortPrefAsc = CPosLmCategoryManager::ECategorySortOrderNameAscending;;
    CPosLmCategoryManager::TCategorySortPref sortPrefDesc = CPosLmCategoryManager::ECategorySortOrderNameDescending;;
    CPosLmItemIterator* categoryDbIter = NULL;

    for (TInt j=0;j<6;j++)
        {
        categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
        CleanupStack::PushL(categoryManager);

        CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
        nameCriteria->SetSearchPatternL(_L("*"));
        
        CPosLmDisplayData* displayData = CPosLmDisplayData::NewL();
				CleanupStack::PushL(displayData);
        iLandmarkSearch->SetDisplayData(*displayData);
        
        if (j==0) 
            {
            // Sorted Ascending, sync
            iLog->Log(_L("Sorted Ascending, Sync"));
            categoryDbIter = categoryManager->CategoryIteratorL(sortPrefAsc);
            CleanupStack::PushL(categoryDbIter);
            iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPrefAsc);
            ExecuteAndDeleteLD(iOperation);
            }
        else if (j==1)
            {
            // Sorted Descending, sync
            iLog->Log(_L("Sorted Descending, Sync"));
            categoryDbIter = categoryManager->CategoryIteratorL(sortPrefDesc);
            CleanupStack::PushL(categoryDbIter);
            iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPrefDesc);
            ExecuteAndDeleteLD(iOperation);
            }
        else if (j==2)
            {
            // Sorted Ascending, async
            iLog->Log(_L("Sorted Ascending, Async"));
            categoryDbIter = categoryManager->CategoryIteratorL(sortPrefAsc);
            CleanupStack::PushL(categoryDbIter);
            iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPrefAsc);
            RunAsyncOperationLD(iOperation);
            }
        else if (j==3)
            {
            // Sorted Descending, async
            iLog->Log(_L("Sorted Descending, Async"));
            categoryDbIter = categoryManager->CategoryIteratorL(sortPrefDesc);
            CleanupStack::PushL(categoryDbIter);
            iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPrefDesc);
            RunAsyncOperationLD(iOperation);
            }
        else if (j==4)
            {
            iLog->Log(_L("Sorted Ascending, Async using User::WaitForRequest"));
            // Sorted Ascending, async using User::WaitForRequest()
            categoryDbIter = categoryManager->CategoryIteratorL(sortPrefAsc);
            CleanupStack::PushL(categoryDbIter);
            iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPrefAsc);
            RunAsyncOperationByWaitForReqL(iOperation);
            delete iOperation;
            }
        else if (j==5)
            {
            iLog->Log(_L("Sorted Descending, Async using User::WaitForRequest"));
            // Sorted Descending, async using User::WaitForRequest()
            categoryDbIter = categoryManager->CategoryIteratorL(sortPrefDesc);
            CleanupStack::PushL(categoryDbIter);
            iOperation = iLandmarkSearch->StartCategorySearchL(*nameCriteria, sortPrefDesc);
            RunAsyncOperationByWaitForReqL(iOperation);
            delete iOperation;
            }
        
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        
        AssertTrueSecL(iter->NumOfItemsL() == categoryDbIter->NumOfItemsL(), KNumOfMatchesErr);
        
        // Check correct displayData
        AssertTrueSecL(iter->NumOfItemsL() == displayData->Count(), KNumOfMatchesErr2);
        
        TInt itemId = displayData->NewItemIndex();
           
        TPosLmItemId id = iter->NextL();
        TPosLmItemId categoryDbId = categoryDbIter->NextL();
        // check that the two iterators are sorted identically
        while (id != KPosLmNullItemId)
            {
            CPosLandmarkCategory* source = categoryManager->ReadCategoryLC(id);
            CPosLandmarkCategory* target = categoryManager->ReadCategoryLC(categoryDbId);
            
            CPosLmDisplayItem& item = displayData->DisplayItem(itemId);
        	TInt categoryId = item.Category().CategoryId();

        	// For debugging purposes
        		TPtrC sourceName, targetName;
            source->GetCategoryName(sourceName);
            target->GetCategoryName(targetName);
            TPtrC name;
						item.Category().GetCategoryName(name);
            		
            TBuf<150> buf;
            buf.Append(_L("Name:"));
            buf.Append(sourceName);
            buf.Append(_L("<->"));
            buf.Append(targetName);
            buf.Append(_L("<->"));
            buf.Append(name);
            iLog->Log(buf);
            
            AssertTrueSecL(id == categoryDbId, KSortorderErr);
        		AssertTrueSecL(id == categoryId, _L("Wrong displaydata item when sorting"));
        		// Should always be the same, since we check that id are equal above
            AssertTrueSecL(sourceName.Compare(targetName) == KErrNone, KNameMatchError);
            AssertTrueSecL(sourceName.Compare(name) == KErrNone, _L("Wrong DisplayData item name when sorting"));
            
            CleanupStack::PopAndDestroy(target);
            CleanupStack::PopAndDestroy(source);
            
            id = iter->NextL();
            categoryDbId = categoryDbIter->NextL();
            itemId = displayData->NewItemIndex();
            }
        iLog->Log(_L("Done"));
        CleanupStack::PopAndDestroy(iter);
        CleanupStack::PopAndDestroy(categoryDbIter);
        
        iLandmarkSearch->UnsetDisplayData();
		    CleanupStack::PopAndDestroy(displayData);
		
        CleanupStack::PopAndDestroy(nameCriteria);
        CleanupStack::PopAndDestroy(categoryManager);
        }

    // Delete the categories added in the beginning
    categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    iOperation = categoryManager->RemoveCategoryL(categoryId1);
    ExecuteAndDeleteLD(iOperation);
    iOperation = categoryManager->RemoveCategoryL(categoryId2);
    ExecuteAndDeleteLD(iOperation);
    iOperation = categoryManager->RemoveCategoryL(categoryId3);
    ExecuteAndDeleteLD(iOperation);
    iOperation = categoryManager->RemoveCategoryL(categoryId4);
    ExecuteAndDeleteLD(iOperation);
    
    CleanupStack::PopAndDestroy(categoryManager);
    iLog->Log(_L("VerifySortOrderL Done"));
    }

//  End of File
