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
#include <EPos_CPosLandmarkSearch.h> 
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLMItemIterator.h>    

#include "FT_CPosTp21.h"
#include "FT_CCategoryLandmarkSearchResult.h"
#include <EPos_CPosLmCatNameCriteria.h>

//CONSTANTS
const TInt KNoCategoriesTp21  = 40;
const TInt KNoLandmarksTp21 = 40; 

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp29::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::CloseTest()
    {       
    iCategories.ResetAndDestroy();
    iCategories.Close();        
    iReferencedCategories.Close();    
    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();         
    
    delete iCategoryManager; 
    iCategoryManager = NULL;  
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;    
    delete iDatabase;
    iDatabase = NULL;    
    }

// ---------------------------------------------------------
// CPosTp21::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::StartL()
    {        
    
    iLog->Log(_L("***Testing CPosLandmarkSearch:: ( LM Search Category Criteria) *** "));
    
    iDatabase = CPosLandmarkDatabase::OpenL();  
        
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }   
       
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase);    
    iLog->Log(_L("Default database initialized"));
    
    
    // Remove all the categories and landmarks from database to make it empty
    MakeEmptyLmDatabaseL( *iDatabase, *iCategoryManager);    
    
    // Add some landmarks to db
    // Add some categories to db
    // Add categories to lms in the database    
    AddCategoriesL();
    AddLandmarksL();    
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    
    // Test some leave codes
    iLog->Log(_L("Check that StartLandmarkSearchL leaves with correct codes "));
    SearchWithSortOrderL();   
  
	// Search synchronously
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt syncronously, \
using CategoryName"));

    SearchL(iCategoryLandmarkSearchResults, ETrue);
 
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt syncronously, \
using CategoryId"));
    SearchL(iCategoryLandmarkSearchResults);

    iLog->Log(_L("Searching fon nonexisting entries synchronously"));
    SearchNonExistingL(ESynchronous);    

	// Search asynchronously
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asynchronously, \
using CategoryName"));
    SearchL(iCategoryLandmarkSearchResults, ETrue, EAsynchronous);

    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asynchronously, \
using Categoryid"));
    SearchL(iCategoryLandmarkSearchResults, EFalse, EAsynchronous);

    iLog->Log(_L("Searching for nonexisting entries asynchronously"));
    SearchNonExistingL(EAsynchronous);   

	// Search asynchronously using User::WaitForRequest()
    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asynchronously, \
with User::WaitForRequest() using CategoryName"));
    SearchL(iCategoryLandmarkSearchResults, ETrue, EWaitForRequest);

    iLog->Log(_L("Testing search with criterias read from LmDbSearchResult.txt asynchronously, \
with User::WaitForRequest() using Categoryid"));
    SearchL(iCategoryLandmarkSearchResults, EFalse, EWaitForRequest);

    iLog->Log(_L("Searching for nonexisting entries asynchronously"));
    SearchNonExistingL(EWaitForRequest);


	// Test cancel
    iLog->Log(_L("Search and cancels the search"));
    SearchAndCancelL();

    iLog->Log(_L("Search and cancels the search in a callback"));
    SearchAndCancelL(ETrue);

	// Search in global categories
    iLog->Log(_L("SearchForLandmarksInGlobalCategoriesL asynchronously"));
    SearchForLandmarksInGlobalCategoriesL(EAsynchronous);

    iLog->Log(_L("SearchForLandmarksInGlobalCategoriesL synchronously"));
    SearchForLandmarksInGlobalCategoriesL(ESynchronous);
    
    iLog->Log(_L("SearchForLandmarksInGlobalCategoriesL synchronously"));
    SearchForLandmarksInGlobalCategoriesL(EWaitForRequest);   
   
    iLog->Log(_L("Check maximum length of search pattern"));
    TestSearchPatternLengthL();
    }

// ---------------------------------------------------------
// CPosTp21::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CPosTp21::SearchL(
    const RPointerArray<CCategoryLandmarkSearchResult>& /* aSearchResults */, 
    TBool aNameOrId, 
    TExecutionMode aExecutionMode)
    {
    /*
     * Positive test cases 
     * Test for referenced category search
     */
         
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");

    //Test SetMaxNumOfMatches, first set to one and then back to unlimited
    iLandmarkSearch->SetMaxNumOfMatches(1);
    iLandmarkSearch->SetMaxNumOfMatches();    
        
    TPtrC refCatName;
    TPosLmItemId refCatId(0);
    
    for ( TInt i = 0; i < iReferencedCategories.Count(); i++ )    
        {
        CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
        // Perform search with either name or id
        if (aNameOrId)
            {
            // Set cearch criteria based on category name             
            TInt retErr = iReferencedCategories[i]->GetCategoryName( refCatName );
            if ( retErr == KErrNone)
                {
                catSearchCriteria->SetCategoryNameL( refCatName );
                //iLog->Log( refCatName );
                }   
            }            
        else
            {
            refCatId = iReferencedCategories[i]->CategoryId();
            catSearchCriteria->SetCategoryItemId( refCatId );
            }   
                    
        //aSearchOnlyPreviousMatches is set to false by default    
        iOperation = iLandmarkSearch->StartLandmarkSearchL( *catSearchCriteria );         

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
        
        /* iter->NumOfItems() contains the number of items that are contained in this search 
         *  iteration
         */  

        TPosLmItemId id = iter->NextL(); // First item from the search results

        if ( iLandmarkSearch->NumOfMatches() != iter->NumOfItemsL() )
            {
            _LIT(KExpected, "Expected no. of matches: %d");
            _LIT(KReturned, "Returned no. of matches: %d");
            TBuf<100> info;
            info.Format( KExpected, iter->NumOfItemsL() );
            iLog->Log( info );
            info.Format( KReturned, iLandmarkSearch->NumOfMatches() );
            iLog->Log(info);                        
            iLog->Log(KNumOfMatchesErr);            
            CleanupStack::PopAndDestroy( 2 ); // Iter,catSearchCriteria                
            User::Leave(-1);
            }
            
        CleanupStack::PopAndDestroy( iter );
            
        // Test by setting Max num of matches and also set aSearchOnlyPreviousMatches to true        
        
        iLandmarkSearch->SetMaxNumOfMatches(1);

        if ( aNameOrId )
            {            
            refCatId = iReferencedCategories[i]->CategoryId();
            catSearchCriteria->SetCategoryItemId( refCatId );
            }            
        else
            {
            // Set cearch criteria based on category name             
            TInt retErr = iReferencedCategories[i]->GetCategoryName( refCatName );
            if ( retErr == KErrNone)
                {
                catSearchCriteria->SetCategoryNameL( refCatName );
                //iLog->Log( refCatName );
                }                
            } 
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria, ETrue);
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
            
        iter = iLandmarkSearch->MatchIteratorL();
        CleanupStack::PushL(iter);
        id = iter->NextL();

        if (iLandmarkSearch->NumOfMatches() != 1)
            {
            _LIT(KExpected, "Expected no. of matches: %d");
            _LIT(KReturned, "Returned no. of matches: %d");
            TBuf<100> info;
            info.Format(KExpected, 1);
            iLog->Log(info);
            info.Format(KReturned, iLandmarkSearch->NumOfMatches());
            iLog->Log(info);
            iLog->Log(_L("Search criteria: "));
            iLog->Log(catSearchCriteria->CategoryName());            
            iLog->Log(KNumOfMatchesErr);
            CleanupStack::PopAndDestroy( 2 ); // Iter,catSearchCriteria                
            User::Leave(-1);
            }   
    
        // Set back to unlimited
        iLandmarkSearch->SetMaxNumOfMatches();     
        CleanupStack::PopAndDestroy( 2 ); // Iter,catSearchCriteria                           
        }                 
    }
    
// ---------------------------------------------------------
// CPosTp21::SearchForLandmarksInGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::SearchForLandmarksInGlobalCategoriesL(
    TExecutionMode aExecutionMode)
    {        
    iLog->Log(_L("SearchForGlobalCategoriesL"));
    TBuf<150> buf; // Buffer used for log messages
    buf.Zero(); 
    
    /*  
     *Default db is open in the beginning of the test procedure anyway
     *Use it for global categories related testing
     */      
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;
    
    iLandmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);

    // Using default database with global cateegories initialized    
    // Add some more landmarks
    AddLandmarksToGlobalCategoriesL();     

    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    
    // Set the criteria for global category search     
    catSearchCriteria->SetGlobalCategory( KAccommodation );    
    if ( catSearchCriteria->GlobalCategory() == KPosLmNullGlobalCategory ) // Error in setting category search criteria
        {
        iLog->Log(_L("Error in setting Search criteria for global category search"));
        buf.Format(_L("What should be set : global cat id %d, "), KAccommodation);
        buf.Format(_L("What is set now    : global cat id %d, "), catSearchCriteria->GlobalCategory() );         
        iLog->Log( buf );
        buf.Zero();
        CleanupStack::PopAndDestroy( catSearchCriteria );
        User::Leave(-1);    
        }
                   
    iOperation = iLandmarkSearch->StartLandmarkSearchL( *catSearchCriteria );
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

    /*
     * Verify the search result
     * iter->NumOfItems holds the total number of searched items in this iteration
     */
     
    if ( iLandmarkSearch->NumOfMatches() != iter->NumOfItemsL() )     
        {
        iLog->Log(_L("Wrong number of landmarks returned global category search (Accomodation)"));    
        buf.Format(_L("Expected : Num %d, "), iter->NumOfItemsL());
        buf.Format(_L("Found    : Num %d, "), iLandmarkSearch->NumOfMatches() );
        buf.Zero();
        CleanupStack::PopAndDestroy(iter);
        CleanupStack::PopAndDestroy(catSearchCriteria);
        User::Leave( -1 );  
        }
        
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(catSearchCriteria);   
    }

// ---------------------------------------------------------
// CPosTp21::AddLandmarksToGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::AddLandmarksToGlobalCategoriesL()
    {      
    iLog->Log(_L("AddLandmarksToGlobalCategoriesL"));
    ExecuteAndDeleteLD(iDatabase->CompactL());    
    CPosLandmark* lm = NULL;
    
    // Add some new landmarks to the db, associate some with global categories
    
    // lm1
    lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("LandmarkName1"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription"));    
    lm->AddCategoryL( KAccommodation );
    lm->AddCategoryL( KBusiness );

    TPosLmItemId id = iDatabase->AddLandmarkL(*lm);
    iRestaurantId1 = id;    
    CleanupStack::PopAndDestroy(lm);
    
    // lm2
    lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("LandmarkName2"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription2"));    
    lm->AddCategoryL(KAccommodation );
    lm->AddCategoryL( KBusiness );
    id = iDatabase->AddLandmarkL(*lm);
    CleanupStack::PopAndDestroy(lm);
       
    // lm3 
    lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("LandmarkName3"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription3"));    
    lm->AddCategoryL( KAccommodation );
    lm->AddCategoryL( KBusiness );
    lm->AddCategoryL( KEducational );
    id = iDatabase->AddLandmarkL(*lm);
    CleanupStack::PopAndDestroy(lm);
    }

// ---------------------------------------------------------
// CPosTp21::SearchNonExistingL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::SearchNonExistingL(TExecutionMode aExecutionMode)
    {
    iLog->Log(_L("SearchNonExistingL"));

    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    catSearchCriteria->SetCategoryNameL(_L("NonExistingLandmark"));

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria);        
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
            
    TPosLmItemId id = iter->NextL();
    if (id != KPosLmNullItemId)
        {
        //LogErrorAndLeave(_L("Should return KPosLmNullItemId"));
        iLog->Log(_L("Should return KPosLmNullItemId"));
        User::Leave(-1);
        }
    CleanupStack::PopAndDestroy(iter);

    TPosLmItemId nonExistingId = 56789; 
    catSearchCriteria->SetCategoryItemId(nonExistingId);

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria);
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
    
    iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
            
    id = iter->NextL();
    if (id != KPosLmNullItemId)
        {
        //LogErrorAndLeave(_L("Should return KPosLmNullItemId"));
        iLog->Log(_L("Should return KPosLmNullItemId"));
        User::Leave(-1);
        }

    CleanupStack::PopAndDestroy(iter);
    
    //Test with invalid id as global category id
    catSearchCriteria->SetGlobalCategory((TPosLmGlobalCategory)nonExistingId);

    iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria);
    ExecuteAndDeleteLD(iOperation);
    
    iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
            
    id = iter->NextL();
    if (id != KPosLmNullItemId)
        {
        //LogErrorAndLeave(_L("Should return KPosLmNullItemId"));
        iLog->Log(_L("Should return KPosLmNullItemId"));
        User::Leave(-1);
        }

    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(catSearchCriteria);
    }
        
// ---------------------------------------------------------
// CPosTp21::SearchWithSortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::SearchWithSortOrderL()
    {
    iLog->Log(_L("SearchWithSortOrderL"));
    
    TPosLmSortPref sortPref(CPosLandmark::ECategoryInfo, TPosLmSortPref::EAscending);
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    // Only ECategoryName is supported,all other leave with KErrNotSupported
    TRAPD(err, iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria, sortPref));
    if (err != KErrNotSupported)     //LogErrorAndLeave(_L("Should leave with KErrNotSupported1"));
        {
        iLog->Log(_L("Should leave with KErrNotSupported1"));
        User::Leave(err);
        }
    
    // Only ECategoryName is supported,all other leave with KErrNotSupported
    catSearchCriteria->SetCategoryNameL(_L("SomeCategory"));
    TRAP(err, iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria, sortPref));
    if (err != KErrNotSupported)     //LogErrorAndLeave(_L("Should leave with KErrNotSupported2"));
        {
        iLog->Log(_L("Should leave with KErrNotSupported2"));
        User::Leave(err);	
        }
       
    // Only ECategoryName is supported,all other leave with KErrNotSupported
    sortPref.SetSortByLandmarkAttribute(CPosLandmark::EDescription, TPosLmSortPref::EDescending);
    TRAP(err, iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria, sortPref));
    if (err != KErrNotSupported)// LogErrorAndLeave(_L("Should leave with KErrNotSupported3"));
        {
        iLog->Log(_L("Should leave with KErrNotSupported3"));
        User::Leave(err);	
        }
    CleanupStack::PopAndDestroy(catSearchCriteria);

    // Also test with a category criteria into a landmarksearch operation
    sortPref.SetSortByLandmarkAttribute(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
    CPosLmCatNameCriteria* nameCriteria = CPosLmCatNameCriteria::NewLC();
    nameCriteria->SetSearchPatternL(_L("SomeCategoryPattern"));
    TRAP(err, iLandmarkSearch->StartLandmarkSearchL(*nameCriteria, sortPref));
    if (err != KErrArgument) //LogErrorAndLeave(_L("Should leave with KErrNotSupported4"), err);
        {
        iLog->Log(_L("Should leave with KErrNotSupported4"));
        User::Leave(err);	
        }
    CleanupStack::PopAndDestroy(nameCriteria);
    }

//---------------------------------------------------------
// CPosTp21::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::SearchAndCancelL(const TBool& aInCallback)
    {            
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
    _LIT(KCancelErr, "The search is canceled after progress has increased but no search result is found");

    // Test search with one criteria
    
    const TInt KIndex = 10;    // Try testing with referenced category at index 10
    TPtrC refCatName;

    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    
    TInt err = iReferencedCategories[KIndex]->GetCategoryName( refCatName );
    if (err == KErrNone) // Got the category name
        {
        catSearchCriteria->SetCategoryNameL( refCatName );    
        }
    else 
        {
        iLog->Log(_L("Error in setting category search criteria"));   
        CleanupStack::PopAndDestroy(); // catSearchCriteria   
        User::Leave(-1);    
        }    
    
    if (aInCallback)
        {
        RunAsyncOperationAndCancelInCallbackLD(
                         iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria));
        }
    else
        {
        RunAsyncOperationAndCancelLD( 
              iLandmarkSearch->StartLandmarkSearchL(
                               *catSearchCriteria));
        }

    CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);
 
    if (aInCallback)
        {
        if (iter->NumOfItemsL() != iLandmarkSearch->NumOfMatches())
            {
            iLog->Log(_L("Iterators no. of items is not equal to LandmarksSearch no. matches"));
            _LIT( KExpected, "Iter. of matches: %d" );
            _LIT( KReturned, "LandmarksSearch no. of matches: %d" );
            TBuf<100> info;
            info.Format( KExpected, iter->NumOfItemsL() );            
            info.Format( KReturned, iLandmarkSearch->NumOfMatches());
            iLog->Log( info );            
            iLog->Log( KNumOfMatchesErr  );
            CleanupStack::PopAndDestroy ( 2 );
       		User::Leave(-1);
            }
            
        TBuf<100> info2;
        _LIT(KNof, "No. of matches found before cancel = %d");
        info2.Format(KNof, iter->NumOfItemsL());
        iLog->Log(info2);

        AssertTrueSecL(iter->NumOfItemsL() != 0, KCancelErr);
        }
        
    CleanupStack::PopAndDestroy( 2 );// Iter, catSearchCriteria    
    
    
    
    }

// ---------------------------------------------------------
// CPosTp21::TestSearchPatternLengthL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::TestSearchPatternLengthL()
    {
    iLog->Log(_L("TestSearchPatternLengthL"));
    
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    
    //Test with maximum size of search pattern
    //KPosLmMaxCategoryNameLength is 124 (from EPos_Landmarks.h)
    HBufC* hbuf = HBufC::NewLC(KPosLmMaxCategoryNameLength+1);
    while(hbuf->Des().Length() < KPosLmMaxCategoryNameLength)
        {
        hbuf->Des().Append(_L("*"));
        }
    catSearchCriteria->SetCategoryNameL(*hbuf);
        
    hbuf->Des().Append(_L("*"));
    
    TRAPD(err, catSearchCriteria->SetCategoryNameL(*hbuf));

    AssertTrueSecL(err == KErrArgument, _L("It should not be possible to set search pattern"));
    CleanupStack::PopAndDestroy(hbuf);
    CleanupStack::PopAndDestroy(catSearchCriteria);
    }


/*
 * Added for BC testing 
 */

    
// ---------------------------------------------------------
// CPosTp11::AddCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::AddCategoriesL()
    {
    _LIT(KName, "CategoryTP21 - %d");
    TInt err=KErrLocked;
    for (TInt i=0; i<KNoCategoriesTp21; i++)
        {
        TBuf<100> categoryName;
        categoryName.Format(KName,i);
       
        CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
        lmc->SetCategoryNameL(categoryName);
        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, iCategoryManager->AddCategoryL(*lmc));
            }
    
        iCategories.Append(lmc);  
        //TPosLmItemId catId = iCategories [i]->CategoryId();     
        CleanupStack::Pop(lmc);
        }    
    }

// ---------------------------------------------------------
// CPosTp11::AddLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp21::AddLandmarksL()
    {
    _LIT(KLmName, "LmTP21 - %d");
    TInt err = KErrLocked;
    TPosLmItemId catId(0);
    for (TInt i=0; i<KNoLandmarksTp21; i++)
        {
        TBuf<100> lmName;
        lmName.Format(KLmName,i);
       
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(lmName); 

        if (i % 2)
            {
            catId = iCategories[i]->CategoryId();
            landmark->AddCategoryL( catId );
            iReferencedCategories.Append(iCategories[i]);
            }

        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, iDatabase->AddLandmarkL(*landmark));
            }
        iLandmarks.Append(landmark);
        CleanupStack::Pop(landmark);
        }
    }
    
// ---------------------------------------------------------
// CPosTp21::MakeEmptyLmDatabaseL()
// ---------------------------------------------------------
//
void CPosTp21::MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
                                     CPosLmCategoryManager& aCatMgr )    
    {
    CPosLmOperation* operation1 = aLdb.RemoveAllLandmarksL();
    
    //Remove all landmarks from default db : Sync call     
    ExecuteAndDeleteLD( operation1 ); 

    RArray<TPosLmItemId> catArray;
    CleanupClosePushL( catArray );
    
    // Get all the categories from the database and delete them if necessary 
    CPosLmItemIterator* iter = aCatMgr.CategoryIteratorL();
    CleanupStack::PushL( iter );

    TUint count = iter->NumOfItemsL();
    if ( count > 0 )
        { // can only be called if there are some items
        iter->GetItemIdsL( catArray, 0, count );
        }
        
    // Remove all the categories here Sync call     
    ExecuteAndDeleteLD(aCatMgr.RemoveCategoriesL( catArray ));
    
    CleanupStack::PopAndDestroy( 2 ); // catArray, iter
    }
        
    

//  End of File
