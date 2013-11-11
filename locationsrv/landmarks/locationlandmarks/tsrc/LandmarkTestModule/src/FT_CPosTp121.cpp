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
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLMItemIterator.h>    
#include <EPos_CPosLmMultiDbSearch.h>    
#include <EPos_CPosLmDatabaseManager.h> 
#include "FT_CPosTp121.h"
#include "FT_LandmarkConstants.h"
#include "FT_CCategoryLandmarkSearchResult.h"
     
// CONSTANTS
// Used when searching for global categories (Global categories are dynamically appended in the test.
// There are originally 26 local categories in the databases)
//const TPosLmItemId KRestaurant = 27;
const TPosLmItemId KBar = 28;
//const TPosLmItemId KGas_Station = 29;
const TPosLmItemId KHotel = 30;
//const TPosLmItemId KShop = 31;
const TPosLmItemId KSports_Ground = 33;
const TPosLmItemId KGarage = 34;
const TPosLmItemId KBus_Stop = 36;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp121::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::CloseTest()
    {   
    delete iDatabases;
    delete iMultiDbSearch;
    iCategoryLandmarkSearchResults.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp121::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::InitTestL()
    {
    CLandmarkTestProcedureBase::InitTestL();
    
    // PrepareDatabases
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KDb20);
    CopyTestDbFileL(KDb40);
    CopyTestDbFileL(KDb60);
    CopyTestDbFileL(KDb80);
    CopyTestDbFileL(KDb105);
    iNrOfDatabases = 5;
    
    iLog->Log(_L("Test Dbs Copied"));
    
    // Initialize data members
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    iDatabases->Sort(); 
    AssertTrueSecL((iDatabases->Count() == iNrOfDatabases), _L("Wrong number of databases"));
    iMultiDbSearch = CPosLmMultiDbSearch::NewL(*iDatabases);
    CleanupStack::PopAndDestroy(dbManager);
    
    iLog->Log(_L("Members ready"));
        
    TInt err = KErrNone;
    CPosLandmarkDatabase* database = NULL; 
    // Reset Global categories for all databases but EPOSLM_105.LDB and add the global category
    // "Hotel" to the 5 first landmarks in the databases.
    for (TInt i = 0; i < iNrOfDatabases - 1; i++) // Do not use global categories in the last db
        {
        database = CPosLandmarkDatabase::OpenL((*iDatabases)[i]);
        CleanupStack::PushL(database);
        if ( database->IsInitializingNeeded() )
            {
            TRAP( err, ExecuteAndDeleteLD( database->InitializeL() ) );
            AssertTrueSecL( err == KErrNone, _L("Init db failed"));
            }
        CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*database);
        CleanupStack::PushL(catMan);
        TRAP( err, ExecuteAndDeleteLD( catMan->ResetGlobalCategoriesL() ) );
        AssertTrueSecL( err == KErrNone, _L("Reset cats failed"));
        CleanupStack::PopAndDestroy(2, database);
        }   

    iLog->Log(_L("Cats reset"));

    // Initiallize iCategoryLandmarkSearchResults
    AppendCategoryLandmarkSearchResultsL();

    iLog->Log(_L("AppendCategoryLandmarkSearchResultsL done"));
    }

// ---------------------------------------------------------
// CPosTp121::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::StartL()
    {
// 1. Search in various ways    
    iLog->Log(_L("Testing search with criteria read from LmDbSearchResult.txt synchronously, using CategoryName. Sort order: ascending"));
    TestSearchL(iCategoryLandmarkSearchResults, ESynchronous, ESortAscending);

    iLog->Log(_L("Testing search with criteria read from LmDbSearchResult.txt synchronously, using CategoryName. Sort order: descending"));
    TestSearchL(iCategoryLandmarkSearchResults, ESynchronous, ESortDescending);

    iLog->Log(_L("Testing search with criteria read from LmDbSearchResult.txt synchronously, using CategoryName. Sort order: none"));
    TestSearchL(iCategoryLandmarkSearchResults, ESynchronous, ESortNone);

    iLog->Log(_L("Testing search with criteria read from LmDbSearchResult.txt asynchronously, using CategoryName. Sort order: ascending"));
    TestSearchL(iCategoryLandmarkSearchResults, EAsynchronous, ESortAscending);

    iLog->Log(_L("Testing search with criteria read from LmDbSearchResult.txt asynchronously, using CategoryName. Sort order: descending"));
    TestSearchL(iCategoryLandmarkSearchResults, EAsynchronous, ESortDescending);

    iLog->Log(_L("Testing search with criteria read from LmDbSearchResult.txt asynchronously, using CategoryName. Sort order: none"));
    TestSearchL(iCategoryLandmarkSearchResults, EAsynchronous, ESortNone);

// 2. Search non-existing category
    iLog->Log(_L("Searching fon nonexisting entries synchronously"));
    SearchNonExistingL(ESynchronous);

    iLog->Log(_L("Searching for nonexisting entries asynchronously"));
    SearchNonExistingL(EAsynchronous);

// 3. Test maximum number of matches
    iLog->Log(_L("Searching with maximum number of results synchronously"));
    TestMaximumNrOfMatchesL(iCategoryLandmarkSearchResults, ESynchronous);

    iLog->Log(_L("Searching with maximum number of results asynchronously"));
    TestMaximumNrOfMatchesL(iCategoryLandmarkSearchResults, EAsynchronous);

// 4. Test cancel
    iLog->Log(_L("Search and cancel the search"));
    SearchAndCancelL();

// 5. Search in global categories
    // Add some more landmarks associated with global categories
    AddLandmarksToGlobalCategoriesL();

    iLog->Log(_L("SearchForLandmarksInGlobalCategoriesL asynchronously"));
    SearchForLandmarksInGlobalCategoriesL(EAsynchronous);

    iLog->Log(_L("SearchForLandmarksInGlobalCategoriesL synchronously"));
    SearchForLandmarksInGlobalCategoriesL(ESynchronous);

// 6. Test some leave codes
    iLog->Log(_L("Check that StartLandmarkSearchL leaves with correct codes"));
    TestLeaveCodesL();
    
// 7. Test automatically reset of search attributes
    iLog->Log(_L("Check that CPosLmCategoryCriteria resets other criteria when setting a new one"));
    TestResetSearchAttributesL();
    
// 8. Test searching by name is case insensitive
    iLog->Log(_L("Testing case insensitive"));
    TestCaseInsensitiveL();
    
// 9. Test search for categories containing wildcards
    iLog->Log(_L("Testing wild cards"));
    TestWildCardsL();
    
// 10. Test various combinations of refined search
    // Modify database2
    iLog->Log(_L("Testing refined search"));
    RIdArray idArray;
    CleanupClosePushL(idArray);
    idArray.Append(51);
    idArray.Append(52);
    idArray.Append(53);
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[2]);
    CleanupStack::PushL(db);
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(catMan);
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(14, idArray)); // 14 = TietoEnator kontor
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(17, idArray)); // 17 = BurgerKing
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(catMan->GetGlobalCategoryL(15000), idArray)); // 15000 = Entertainment
    CleanupStack::PopAndDestroy(3, &idArray);

    TestRefinedSearchL(ESynchronous);
    TestRefinedSearchL(EAsynchronous);
    
// 11. Test modify list of databases
    iLog->Log(_L("Testing modifying list of databases"));
    RIdArray expectedMatches;
    CleanupClosePushL(expectedMatches);
    expectedMatches.AppendL(iGasStationId1);
    expectedMatches.AppendL(iGasStationId2);
    expectedMatches.AppendL(51);
    expectedMatches.AppendL(52);
    expectedMatches.AppendL(53);
    
    TestDatabaseListL(expectedMatches);

// 12. Test on-the-fly
    iLog->Log(_L("Testing searching on the fly"));
    TestOnTheFlySearchL(expectedMatches);
    
    CleanupStack::PopAndDestroy(&expectedMatches);
    }

// ---------------------------------------------------------
// CPosTp121::TestSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestSearchL(
    const RPointerArray<CCategoryLandmarkSearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode, 
    TTp121SortOrder aSortOrder)
    {
    // Test SetMaxNumOfMatches, first set to one and then back to unlimited
    iMultiDbSearch->SetMaxNumOfMatches(1);
    iMultiDbSearch->SetMaxNumOfMatches();

    for(TInt i = 0; i < aSearchResults.Count(); i++)
        {   
        CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
        
        // Perform search by name
        catSearchCriteria->SetCategoryNameL(aSearchResults[i]->CategoryName());
        iLog->Log(aSearchResults[i]->CategoryName());

        SearchL(catSearchCriteria, aExecutionMode, aSearchResults[i]->Redefined(), aSortOrder);
        
        if (i != 28)
            {
            // Row 28 in xls-file contains an error. If fixed, TP21 starts to fail.
            ValidateSearchResultsL(aSearchResults[i], iMultiDbSearch, aSortOrder);
            ValidateTotalNrOfMatchesL(
                iMultiDbSearch->TotalNumOfMatches(),
                (TUint)(aSearchResults[i]->SearchResult()).Count(),
                catSearchCriteria->CategoryName());
            }

        CleanupStack::PopAndDestroy(catSearchCriteria);
        }
    }

// ---------------------------------------------------------
// CPosTp121::SearchForLandmarksInGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::SearchForLandmarksInGlobalCategoriesL(
    TExecutionMode aExecutionMode)
    {
    iLog->Log(_L("SearchForGlobalCategoriesL"));

    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    RIdArray expectedIds;
    CleanupClosePushL(expectedIds);
    
    // Search for "Accommodation", should result in two matches
    catSearchCriteria->SetGlobalCategory( KAccommodation );
    SearchL(catSearchCriteria, aExecutionMode);
    expectedIds.AppendL(iRestaurantId1);
    expectedIds.AppendL(iRestaurantId2);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    expectedIds.Reset();
    
    // Search for "Entertainment" id using the global category id should result in two matches
   
    catSearchCriteria->SetGlobalCategory(KBusiness);
    SearchL(catSearchCriteria, aExecutionMode);
    expectedIds.AppendL(iGasStationId1);
    expectedIds.AppendL(iGasStationId2);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    expectedIds.Reset();

    // Search for "Accommodation" in the result from previous search
    catSearchCriteria->SetGlobalCategory(KAccommodation);
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    expectedIds.AppendL(iGasStationId2);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    expectedIds.Reset();

    // Search for a non existing global category in the result from previous search
    // Nothing should be found
    catSearchCriteria->SetCategoryNameL(_L("NonExistingGLobalCategory"));
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    expectedIds.Reset();

    // Search for "Accommodation" using the globalcategory id
    catSearchCriteria->SetGlobalCategory(3000);
    SearchL(catSearchCriteria, aExecutionMode);
    expectedIds.AppendL(iRestaurantId1);
    expectedIds.AppendL(iRestaurantId2);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    expectedIds.Reset();

    // Search for "Entertainment" using the globalcategory id from the result 
    // from previous search
    catSearchCriteria->SetGlobalCategory(15000);
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    expectedIds.AppendL(iGasStationId2);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    expectedIds.Reset();
    
    
    CleanupStack::PopAndDestroy(2, catSearchCriteria);
    }

// ---------------------------------------------------------
// CPosTp121::AddLandmarksToGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::AddLandmarksToGlobalCategoriesL()
    {
    iLog->Log(_L("AddLandmarksToGlobalCategoriesL"));
    
    // Add some new landmarks to the dbs, associate some with global categories
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[0]);
    CleanupStack::PushL(db);
    CPosLandmark* lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("LandmarkName1"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription"));    
    
	/*************************************************************/
	// Search for id maintained by database for the global category
	CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL( *db );
	CleanupStack::PushL(catMan);
    TPosLmItemId globCatId1 = catMan->GetGlobalCategoryL( KAccommodation );
    TPosLmItemId globCatId2 = catMan->GetGlobalCategoryL( KEntertainment );

	CPosLandmarkCategory* cat = catMan->ReadCategoryLC( globCatId1 );

	CleanupStack::PopAndDestroy( 2 );	
	/*************************************************************/    
    lm->AddCategoryL( globCatId1 );  
    TPosLmItemId id = db->AddLandmarkL(*lm);
    iRestaurantId1 = id;
    
    CleanupStack::PopAndDestroy(2, db);    
    
    db = CPosLandmarkDatabase::OpenL((*iDatabases)[1]);
    CleanupStack::PushL(db);
    lm = CPosLandmark::NewLC();
    db->AddLandmarkL(*lm); // Add one uncategorized to increase id in order to avoid equal ids between dbs.
    lm->SetLandmarkNameL(_L("LandmarkName2"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription2"));    
    lm->AddCategoryL(KBus_Stop);
    id = db->AddLandmarkL(*lm);
    CleanupStack::PopAndDestroy(2, db);

    db = CPosLandmarkDatabase::OpenL((*iDatabases)[2]);
    CleanupStack::PushL(db);
    lm = CPosLandmark::NewLC();
    db->AddLandmarkL(*lm); // Add two uncategorized to increase id in order to avoid equal ids between dbs.
    db->AddLandmarkL(*lm); 
    lm->SetLandmarkNameL(_L("LandmarkName3"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription3"));    
    lm->AddCategoryL( globCatId2 ); 
    lm->AddCategoryL(KHotel);
    //lm->AddCategoryL(KShop);
    lm->AddCategoryL(KBar);
    lm->AddCategoryL(KSports_Ground);
    lm->AddCategoryL(KGarage);
    id = db->AddLandmarkL(*lm);
    iGarageId1 = id;
    iGasStationId1 = id;
    CleanupStack::PopAndDestroy(2, db);

    db = CPosLandmarkDatabase::OpenL((*iDatabases)[3]);
    CleanupStack::PushL(db);
    lm = CPosLandmark::NewLC();
    db->AddLandmarkL(*lm); // Add three uncategorized to increase id in order to avoid equal ids between dbs.
    db->AddLandmarkL(*lm); 
    db->AddLandmarkL(*lm); 
    lm->SetLandmarkNameL(_L("LandmarkName4"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription4"));    
    lm->AddCategoryL(KBar);
    lm->AddCategoryL( globCatId1 );  
    lm->AddCategoryL( globCatId2 );
    lm->AddCategoryL(KHotel);
   // lm->AddCategoryL(KShop);
   	lm->AddCategoryL(KBar);
    lm->AddCategoryL(1);
    lm->AddCategoryL(2);
    lm->AddCategoryL(3);
    id = db->AddLandmarkL(*lm);
    iRestaurantId2 = id;
    iGasStationId2 = id;
    CleanupStack::PopAndDestroy(lm);

    lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("LandmarkName5"));
    lm->SetLandmarkDescriptionL(_L("SomeLandmarkDescription5"));
    lm->AddCategoryL(KBus_Stop);
    db->AddLandmarkL(*lm);
    CleanupStack::PopAndDestroy(2, db);
    
    }

// ---------------------------------------------------------
// CPosTp121::SearchNonExistingL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::SearchNonExistingL(TExecutionMode aExecutionMode)
    {
    iLog->Log(_L("SearchNonExistingL"));

    _LIT(KCategoryName, "NonExistingLandmark");
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    catSearchCriteria->SetCategoryNameL(KCategoryName);

    SearchL(catSearchCriteria, aExecutionMode);
    ValidateTotalNrOfMatchesL(iMultiDbSearch->TotalNumOfMatches(), 0, KCategoryName);
    ValidateSearchResultsL(NULL, iMultiDbSearch);
    CleanupStack::PopAndDestroy(catSearchCriteria);

    catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    TPosLmGlobalCategory nonExistingId = 56789; 
    catSearchCriteria->SetGlobalCategory(nonExistingId);

    SearchL(catSearchCriteria, aExecutionMode);
    ValidateTotalNrOfMatchesL(iMultiDbSearch->TotalNumOfMatches(), 0, KCategoryName);
    ValidateSearchResultsL(NULL, iMultiDbSearch);
    
    CleanupStack::PopAndDestroy(catSearchCriteria);
    }

// ---------------------------------------------------------
// CPosTp121::TestMaximumNrOfMatchesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestMaximumNrOfMatchesL(
    const RPointerArray<CCategoryLandmarkSearchResult>& aSearchResults, 
    TExecutionMode aExecutionMode)
    {
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    TInt idFromXls = 25; // Use 25 "Läkarna"
    catSearchCriteria->SetCategoryNameL(aSearchResults[idFromXls]->CategoryName());
    iLog->Log(aSearchResults[idFromXls]->CategoryName());
         
    SearchL(catSearchCriteria, aExecutionMode, aSearchResults[idFromXls]->Redefined());
    ValidateTotalNrOfMatchesL(
    		iMultiDbSearch->TotalNumOfMatches(),
    		(TUint)(aSearchResults[idFromXls]->SearchResult()).Count(), 
    		aSearchResults[idFromXls]->CategoryName());
    ValidateSearchResultsL(aSearchResults[idFromXls], iMultiDbSearch);

    // Set maximum number, first set to more than should be found here 13 in each db must be enough
    iLog->Log(_L("Set to 13 * iNrOfDatabases = 65 matches"));
    TUint maxMatches = 13;
    iMultiDbSearch->SetMaxNumOfMatches(13);
    
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    ValidateTotalNrOfMatchesL(
        iMultiDbSearch->TotalNumOfMatches(),
        (TUint)(aSearchResults[idFromXls]->SearchResult()).Count(), 
        aSearchResults[idFromXls]->CategoryName());
    ValidateSearchResultsL(aSearchResults[idFromXls], iMultiDbSearch);
    
    // Set maximum number here to 3 in each db
    iLog->Log(_L("Set to 3 * iNrOfDatabases = 10 matches"));
    maxMatches = 3;
    const TInt KNrOfDbsWithMatches = 3;
    iMultiDbSearch->SetMaxNumOfMatches(maxMatches);
    
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    ValidateTotalNrOfMatchesL(
        iMultiDbSearch->TotalNumOfMatches(),
        maxMatches * KNrOfDbsWithMatches, // only three of the databases contain matches
        aSearchResults[idFromXls]->CategoryName());
    ValidateSearchResultsL(aSearchResults[idFromXls], iMultiDbSearch);
    
    // Set maximum number here to 1 in each db
    maxMatches = 1;
    iLog->Log(_L("Set to 1 * iNrOfDatabases = 5 matches"));
    iMultiDbSearch->SetMaxNumOfMatches(maxMatches);
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    ValidateTotalNrOfMatchesL(
        iMultiDbSearch->TotalNumOfMatches(),
        maxMatches * KNrOfDbsWithMatches, 
        aSearchResults[idFromXls]->CategoryName());
    ValidateSearchResultsL(aSearchResults[idFromXls], iMultiDbSearch);
    
    // Set maximum number here to 2 in each db, however since search is refined only 3 matches should be returned
    maxMatches = 2;
    iLog->Log(_L("Set to 2 * iNrOfDatabases = 10 matches"));
    iMultiDbSearch->SetMaxNumOfMatches(maxMatches);
    SearchL(catSearchCriteria, aExecutionMode, ETrue);
    ValidateTotalNrOfMatchesL(
        iMultiDbSearch->TotalNumOfMatches(),
        1 * KNrOfDbsWithMatches, 
        aSearchResults[idFromXls]->CategoryName());
    ValidateSearchResultsL(aSearchResults[idFromXls], iMultiDbSearch);

    // Same as above but no refined search is made. Should get 2 * 3 matches
    iLog->Log(_L("Set to 2 * iNrOfDatabases = 10 matches"));
    iMultiDbSearch->SetMaxNumOfMatches(maxMatches);
    SearchL(catSearchCriteria, aExecutionMode);
    ValidateTotalNrOfMatchesL(
        iMultiDbSearch->TotalNumOfMatches(),
        maxMatches * KNrOfDbsWithMatches, 
        aSearchResults[idFromXls]->CategoryName());
    ValidateSearchResultsL(aSearchResults[idFromXls], iMultiDbSearch);
    
    // Set back to unlimited
    iMultiDbSearch->SetMaxNumOfMatches();
    
    CleanupStack::PopAndDestroy(catSearchCriteria);
    }

// ---------------------------------------------------------
// CPosTp121::TestLeaveCodesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestLeaveCodesL()
    {
    iLog->Log(_L("TestLeaveCodesL"));
    
    TPosLmSortPref sortPref(CPosLandmark::ECategoryInfo, TPosLmSortPref::EAscending);
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    // Only ECategoryName is supported,all other leave with KErrNotSupported
    TRAPD(err, iMultiDbSearch->StartLandmarkSearchL(*catSearchCriteria, sortPref));
    AssertTrueSecL(err == KErrNotSupported, _L("Should have left with KErrNotSupported"));
       
    // Only ECategoryName is supported,all other leave with KErrNotSupported
    sortPref.SetSortByLandmarkAttribute(CPosLandmark::EDescription, TPosLmSortPref::EDescending);
    TRAP(err, iMultiDbSearch->StartLandmarkSearchL(*catSearchCriteria, sortPref));
    AssertTrueSecL(err == KErrNotSupported, _L("Should have left with KErrNotSupported"));

    // Test search by local category id
    catSearchCriteria->SetCategoryItemId(7);
    TRAP(err, iMultiDbSearch->StartLandmarkSearchL(*catSearchCriteria));
    AssertTrueSecL(err == KErrArgument, _L("Should have left with KErrArgument"));

    // Test search by too long category name > 124 = KPosLmMaxCategoryNameLength
    TBuf<KPosLmMaxCategoryNameLength + 4> name = _L("12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678");
    TRAP(err, catSearchCriteria->SetCategoryNameL(name));
    AssertTrueSecL(err == KErrArgument, _L("Should have left with KErrArgument"));

    CleanupStack::PopAndDestroy(catSearchCriteria);
    }

// ---------------------------------------------------------
// CPosTp121::SearchAndCancelL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::SearchAndCancelL()
    {
    // Test search with one criteria
    const TInt KIndex=0;

    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    catSearchCriteria->SetCategoryNameL(iCategoryLandmarkSearchResults[KIndex]->CategoryName());
    
    TReal32 progress(0);
    while (progress < 1.0)
        {
        RunAsyncOperationAndCancelInCallbackLD(
            iMultiDbSearch->StartLandmarkSearchL(
                *catSearchCriteria, 
                iCategoryLandmarkSearchResults[KIndex]->Redefined()),
            progress);

        TInt nrOfMatches = 0;
        for (TInt j = 0; j < iNrOfDatabases; j++)
            {
            CPosLmItemIterator* iter = iMultiDbSearch->MatchIteratorL(j);
            CleanupStack::PushL(iter);
            nrOfMatches += iter->NumOfItemsL();
            CleanupStack::PopAndDestroy(iter);
            }
        AssertTrueSecL(nrOfMatches == iMultiDbSearch->TotalNumOfMatches(), _L("Total nr of matches doesn't match sum of matches in iterators when cancelling"));
        
        progress = progress + 0.09;
        }
    
    CleanupStack::PopAndDestroy(catSearchCriteria);    
    }


// ---------------------------------------------------------
// CPosTp121::ValidateIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::ValidateIdL(TPosLmItemId aId, TInt aDbIndex)
    {
    TBool condition = EFalse;
    switch (aDbIndex)
        {
        case 0:
            // Db 0 contains landmarks with id = 1-20
            condition = aId >= 1 && aId <= 20;
            break;
        case 1:
            // Db 1 contains landmarks with id = 21-40
            condition = aId >= 21 && aId <= 40;
            break;
        case 2:
            // Db 2 contains landmarks with id = 41-60
            condition = aId >= 41 && aId <= 60;
            break;
        case 3:
            // Db 3 contains landmarks with id = 61-80
            condition = aId >= 61 && aId <= 80;
            break;
        case 4:
            // Db 4 contains landmarks with id = 81-105
            condition = aId >= 81 && aId <= 105;
            break;
        }

    AssertTrueSecL(condition, _L("Invalid match from database reported"));
    }

// ---------------------------------------------------------
// CPosTp121::ValidateTotalNrOfMatchesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::ValidateTotalNrOfMatchesL(
    TUint aRetrievedNrOfMatches,
    TUint aExpectedNrOfMatches,
    const TDesC& aCategoryName)
    {
    _LIT(KNumOfMatchesErr, "No. of matches is incorrect when the search is performed");
        
    if (aRetrievedNrOfMatches != aExpectedNrOfMatches)
        {
        _LIT(KExpected, "Expected no. of matches: %d");
        _LIT(KReturned, "Returned no. of matches: %d");
        TBuf<100> info;
        info.Format(KExpected, aExpectedNrOfMatches);
        iLog->Log(info);
        info.Format(KReturned, aRetrievedNrOfMatches);
        iLog->Log(info);
        _LIT(KFormat, "Search criteria: %S");
        info.Format(KFormat, &aCategoryName);
        iLog->Log(info);
        iLog->Log(KNumOfMatchesErr);
        User::Leave(-1);
        }
    }

// ---------------------------------------------------------
// CPosTp121::ValidateSearchResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::ValidateSearchResultsL(
    CCategoryLandmarkSearchResult* aExpectedSearchResult,
    CPosLmMultiDbSearch* aMultiDbSearcher,
    TTp121SortOrder aSortOrder)
    {
    _LIT(KNotFoundErr, "Id %d not found in parsed Lm search results");

    TInt nrOfMatches = 0;
    for (TInt j = 0; j < iNrOfDatabases; j++)
        {
        CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[j]);
        CleanupStack::PushL(db);
        CPosLmItemIterator* iter = aMultiDbSearcher->MatchIteratorL(j);
        CleanupStack::PushL(iter);
        
        nrOfMatches += iter->NumOfItemsL();
        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            ValidateIdL(id, j);
            AssertTrueSecL(aExpectedSearchResult->FindSearchResult(id), KNotFoundErr, id);
            CPosLandmark* lm1 = db->ReadLandmarkLC(id);
            id = iter->NextL();
            if (aSortOrder != ESortNone && id != KPosLmNullItemId)
                {
                CPosLandmark* lm2 = db->ReadLandmarkLC(id);
                TPtrC name1, name2;
                lm1->GetLandmarkName(name1);
                lm2->GetLandmarkName(name2);
                if (aSortOrder == ESortAscending)
                    {                        
                    AssertTrueSecL(name1.CompareC(name2) <= 0, _L("Not sorted ascending as expected"));
                    }
                else // aSortOrder == ESortDescending
                    {
                    AssertTrueSecL(name1.CompareC(name2) >= 0, _L("Not sorted descending as expected"));
                    }
                CleanupStack::PopAndDestroy(lm2);
                }
            CleanupStack::PopAndDestroy(lm1);
            }
        CleanupStack::PopAndDestroy(2, db);
        }
    AssertTrueSecL(nrOfMatches == aMultiDbSearcher->TotalNumOfMatches(), _L("Total nr of matches doesn't match sum of matches in iterators"));
    AssertTrueSecL(NrOfSearchErrors(aMultiDbSearcher) == 0, _L("Found search errors!"));
    }
    
// ---------------------------------------------------------
// CPosTp121::ValidateSearchResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::ValidateSearchResultsL(
    const RIdArray& aExpectedIds,
    CPosLmMultiDbSearch* aMultiDbSearcher,
    TInt aNrOfExpectedErrors)
    {
    _LIT(KNotFoundErr, "Id %d not found in expected Lm search results");

    TInt nrOfMatches = 0;
    for (TInt j = 0; j < iNrOfDatabases; j++)
        {
        CPosLmItemIterator* iter = aMultiDbSearcher->MatchIteratorL(j);
        CleanupStack::PushL(iter);
        nrOfMatches += iter->NumOfItemsL();
        
        TUint nrOfSearchMatches = aMultiDbSearcher->NumOfMatches( j );
        
        TPosLmItemId id = iter->NextL();
        while (id != KPosLmNullItemId)
            {
            AssertTrueSecL(aExpectedIds.Find(id) != KErrNotFound, KNotFoundErr, id);
            id = iter->NextL();
            }
        CleanupStack::PopAndDestroy(iter);
        }
    AssertTrueSecL(nrOfMatches == aMultiDbSearcher->TotalNumOfMatches(), _L("Total nr of matches doesn't match sum of matches in iterators"));
    AssertTrueSecL(nrOfMatches == aExpectedIds.Count(), _L("Total nr of matches doesn't match expected"));
    AssertTrueSecL(NrOfSearchErrors(aMultiDbSearcher) == aNrOfExpectedErrors, _L("Found search errors!"));
    }
    
// ---------------------------------------------------------
// CPosTp121::SearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::SearchL(
    CPosLmCategoryCriteria* aCatSearchCriteria,
    TExecutionMode aExecutionMode,
    TBool aRefinedSearch,
    TTp121SortOrder aSortOrder)
    {
    TPosLmSortPref ascending(CPosLandmark::ELandmarkName);
    TPosLmSortPref descending(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);

    switch (aSortOrder)
        {
        case ESortNone:
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCatSearchCriteria, aRefinedSearch);
            break;
        case ESortAscending:
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCatSearchCriteria, ascending, aRefinedSearch);
            break;
        case ESortDescending:
            iOperation = iMultiDbSearch->StartLandmarkSearchL(*aCatSearchCriteria, descending, aRefinedSearch);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
        
    switch (aExecutionMode)
        {
        case ESynchronous:
            ExecuteAndDeleteLD(iOperation);
            break;
        case EAsynchronous:
            RunAsyncOperationLD(iOperation);
            break;
        default:
            User::Leave(KErrArgument);
            break;
        }
    }

// ---------------------------------------------------------
// CPosTp121::TestResetSearchAttributesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestResetSearchAttributesL()
    {
    CPosLmCategoryCriteria* crit = CPosLmCategoryCriteria::NewLC();
    
    // Verify default values
    TPosLmItemId id = crit->CategoryItemId();
    AssertTrueSecL(id == KPosLmNullItemId, _L("id != KPosLmNullItemId"));
    TPosLmGlobalCategory globalId = crit->GlobalCategory();
    AssertTrueSecL(globalId == KPosLmNullItemId, _L("globalId != KPosLmNullItemId"));
    TPtrC name = crit->CategoryName();
    AssertTrueSecL(name.Length() == 0, _L("name.Length() != 0"));    
    
    // Set id - other values should be default
    const TPosLmItemId KId = 10;
    crit->SetCategoryItemId(KId);
    AssertTrueSecL(crit->CategoryItemId() == KId, _L("id != KId"));
    AssertTrueSecL(crit->GlobalCategory()== KPosLmNullItemId, _L("globalId != KPosLmNullItemId"));
    AssertTrueSecL(crit->CategoryName().Length() == 0, _L("name.Length() != 0"));    
    
    // Set name - other values should be default
    _LIT(KName, "Category Name");
    crit->SetCategoryNameL(KName);
    AssertTrueSecL(crit->CategoryItemId() == KPosLmNullItemId, _L("id != KPosLmNullItemId"));
    AssertTrueSecL(crit->GlobalCategory()== KPosLmNullItemId, _L("globalId != KPosLmNullItemId"));
    AssertTrueSecL(crit->CategoryName() == KName, _L("name != KName"));    
    
    // Set global id - other values should be default
    const TPosLmGlobalCategory KGlobalCategoryId = 20;
    crit->SetGlobalCategory(KGlobalCategoryId);
    AssertTrueSecL(crit->CategoryItemId() == KPosLmNullItemId, _L("id != KPosLmNullItemId"));
    AssertTrueSecL(crit->GlobalCategory()== KGlobalCategoryId, _L("globalId != KGlobalCategoryId"));
    AssertTrueSecL(crit->CategoryName().Length() == 0, _L("name.Length() != 0"));    
    
    // Set id - other values should be default
    crit->SetCategoryItemId(KId);
    AssertTrueSecL(crit->CategoryItemId() == KId, _L("id != KId"));
    AssertTrueSecL(crit->GlobalCategory()== KPosLmNullItemId, _L("globalId != KPosLmNullItemId"));
    AssertTrueSecL(crit->CategoryName().Length() == 0, _L("name.Length() != 0"));    
    
    CleanupStack::PopAndDestroy(crit);
    }

// ---------------------------------------------------------
// CPosTp121::TestCaseInsensitiveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestCaseInsensitiveL()
    {
    CPosLmCategoryCriteria* crit = CPosLmCategoryCriteria::NewLC();
    RIdArray expectedIds;
    CleanupClosePushL(expectedIds);
    
    // Search for "rEStaUrANt", should result in no match
    TBuf<25> rest = _L("rEStaUrANt");
    crit->SetCategoryNameL(rest);
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    
    // Search for "RESTAURANT", should result in no match
    rest.UpperCase();
    crit->SetCategoryNameL(rest);
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    
    // Search for "Restaurant", should result in two matches
    HBufC* restaurant = GetGlobalCategoryFromResourceLC( KAccommodation );
    crit->SetCategoryNameL(*restaurant);
    SearchL(crit, ESynchronous);
    expectedIds.AppendL(iRestaurantId1);
    expectedIds.AppendL(iRestaurantId2);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    CleanupStack::PopAndDestroy( restaurant );
    
    // Search for "mCDoNAldS", should result in no match
    rest = _L("mCDoNAldS");
    crit->SetCategoryNameL(rest);
    SearchL(crit, EAsynchronous);
    expectedIds.Reset();
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    
    // Search for "MCDONALDS", should result in no match
    rest.UpperCase();
    crit->SetCategoryNameL(rest);
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    
    // Search for "McDonalds", should result in two matches
    rest = _L("McDonalds");
    crit->SetCategoryNameL(rest);
    SearchL(crit, EAsynchronous);
    expectedIds.AppendL(7);
    expectedIds.AppendL(27);
    ValidateSearchResultsL(expectedIds, iMultiDbSearch);
    CleanupStack::PopAndDestroy(2, crit);
    }
    
// ---------------------------------------------------------
// CPosTp121::TestWildCardsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestWildCardsL()
    {    
    CPosLmCategoryCriteria* crit = CPosLmCategoryCriteria::NewLC();
    const TInt KNrOfArrays = 5;
    RIdArray* idArrays[KNrOfArrays];
    for (TInt i = 0; i < KNrOfArrays; i++)
        {
        RIdArray* array = new (ELeave) RIdArray;
        CleanupStack::PushL(array);
        CleanupClosePushL(*array);
        idArrays[i] = array;
        }
    RIdArray empty;
    CleanupClosePushL(empty);
        
    // Search for "*", should result in no matches    
    crit->SetCategoryNameL(_L("*"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
    // Search for "?", should result in no matches    
    crit->SetCategoryNameL(_L("?"));
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
// -------------- Setup databases --------------
    
// Add a category to db0
    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[0]);
    CleanupStack::PushL(db);
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(catMan);    
    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    
    // Add category "*wild card"
    _LIT(KCategory0, "*wild card");
    category->SetCategoryNameL(KCategory0);
    TPosLmItemId id0 = catMan->AddCategoryL(*category);
    idArrays[0]->AppendL(1);
    idArrays[0]->AppendL(2);
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(id0, *idArrays[0]));
    CleanupStack::PopAndDestroy(3, db);
    
// Add categories to db2
    db = CPosLandmarkDatabase::OpenL((*iDatabases)[2]);
    CleanupStack::PushL(db);
    catMan = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(catMan);
    category = CPosLandmarkCategory::NewLC();
    
    // Add category "wi?d c?rd"
    _LIT(KCategory1, "wi?d c?rd");
    category->SetCategoryNameL(KCategory1);
    TPosLmItemId id1 = catMan->AddCategoryL(*category);
    idArrays[1]->AppendL(44);
    idArrays[1]->AppendL(45);
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(id1, *idArrays[1]));
    
    // Add category "***"
    _LIT(KCategory2, "***");
    category->SetCategoryNameL(KCategory2);
    TPosLmItemId id2 = catMan->AddCategoryL(*category);
    idArrays[2]->AppendL(44);
    idArrays[2]->AppendL(47);
    idArrays[2]->AppendL(55);
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(id2, *idArrays[2]));
    CleanupStack::PopAndDestroy(3, db);
    
// Add categories to db4
    db = CPosLandmarkDatabase::OpenL((*iDatabases)[4]);
    CleanupStack::PushL(db);
    catMan = CPosLmCategoryManager::NewL(*db);
    CleanupStack::PushL(catMan);
    category = CPosLandmarkCategory::NewLC();
        
    // Add category "?"
    _LIT(KCategory3, "?");
    category->SetCategoryNameL(KCategory3);
    TPosLmItemId id3 = catMan->AddCategoryL(*category);
    idArrays[3]->AppendL(86);
    idArrays[3]->AppendL(81);
    idArrays[3]->AppendL(104);
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(id3, *idArrays[3]));
    
    // Add category "wi?d ca*"
    _LIT(KCategory4, "wi?d ca*");
    category->SetCategoryNameL(KCategory4);
    TPosLmItemId id4 = catMan->AddCategoryL(*category);
    idArrays[4]->AppendL(101);
    ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(id4, *idArrays[4]));
    CleanupStack::PopAndDestroy(3, db);

// -------------- Search and verify --------------
    
// Search for "*" and verify results - no matches expected
    crit->SetCategoryNameL(_L("*"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
        
// Search for "*wild*" and verify results - no matches expected
    crit->SetCategoryNameL(_L("*wild*"));
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
// Search for "w?ld ca?d" and verify results - no matches expected
    crit->SetCategoryNameL(_L("w?ld ca?d"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
// Search for "wi?d c?rd" and verify results - two matches expected
    crit->SetCategoryNameL(KCategory1);
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(*idArrays[1], iMultiDbSearch);
    
// Search for "****" and verify results - no matches expected
    crit->SetCategoryNameL(_L("****"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
// Search for "***" and verify results - three matches expected
    crit->SetCategoryNameL(KCategory2);
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(*idArrays[2], iMultiDbSearch);
    
// Search for "**" and verify results - no matches expected
    crit->SetCategoryNameL(_L("**"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
// Search for "?" and verify results - two matches expected
    crit->SetCategoryNameL(KCategory3);
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(*idArrays[3], iMultiDbSearch);
    
// Search for "?*" and verify results - no matches expected
    crit->SetCategoryNameL(_L("?*"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);
    
// Search for "wi?d ca*" and verify results - one match expected
    crit->SetCategoryNameL(KCategory4);
    SearchL(crit, EAsynchronous);
    ValidateSearchResultsL(*idArrays[4], iMultiDbSearch);
    
// Search for "wi?d card" and verify results - no matches expected
    crit->SetCategoryNameL(_L("wi?d card"));
    SearchL(crit, ESynchronous);
    ValidateSearchResultsL(empty, iMultiDbSearch);    
    
    CleanupStack::PopAndDestroy(&empty);
    CleanupStack::PopAndDestroy(2 * KNrOfArrays);
    CleanupStack::PopAndDestroy(crit);
    }
    
// ---------------------------------------------------------
// CPosTp121::TestRefinedSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestRefinedSearchL(TExecutionMode aExecutionMode)
    {
    CPosLmCategoryCriteria* crit = CPosLmCategoryCriteria::NewLC();
    RIdArray idArray;
    CleanupClosePushL(idArray);
    
    // Make a search for local category name
    crit->SetCategoryNameL(_L("TietoEnator kontor")); // lm 7,27,51-61 will be found according to xls-sheet and code above
    idArray.AppendL(7);
    idArray.AppendL(27);
    for (TInt i = 51; i <= 61; i++)
        {        
        idArray.AppendL(i);
        }
    SearchL(crit, aExecutionMode);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
    
    // Refine the previous search by specifying another local category name
    crit->SetCategoryNameL(_L("BurgerKing")); // lm 7,27,51-54 will be found according to xls-sheet and the code above
    for (TInt i = idArray.Count() - 1; i >= 5; i--)
        {
        idArray.Remove(i);
        }
    SearchL(crit, aExecutionMode, ETrue);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
    
    // Repeat previous step but set max nr of matches to 1 - only 7,27,51 will be found
    iMultiDbSearch->SetMaxNumOfMatches(1);
    for (TInt i = idArray.Count() - 1; i >= 3; i--)
        {
        idArray.Remove(i);
        }
    SearchL(crit, aExecutionMode, ETrue);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
    
    // Increase max nr of matches to 10 => all qualified lms will be found
    crit->SetCategoryNameL(_L("TietoEnator kontor"));
    iMultiDbSearch->SetMaxNumOfMatches(10);
    for (TInt i = 52; i <= 61; i++)
        {
        idArray.AppendL(i);
        }
    SearchL(crit, aExecutionMode);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
    
    // Refine search with the global category gas station by searching by id 
    crit->SetGlobalCategory(15000); // Entertainment
    idArray.Reset();
    for (TInt i = 51; i <= 53; i++)
        {
        idArray.AppendL(i);
        }
    SearchL(crit, aExecutionMode, ETrue);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
        
    // Refine search with the local category Pizzeria by searching by name
    crit->SetCategoryNameL(_L("Pizzeria")); // only lm 51 should be found
    idArray.Remove(1);
    idArray.Remove(1);
    SearchL(crit, aExecutionMode, ETrue);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
        
    // Refine search with a global id that does not exist
    crit->SetGlobalCategory(10000); // no lms should be found
    idArray.Remove(0);
    SearchL(crit, aExecutionMode, ETrue);
    ValidateSearchResultsL(idArray, iMultiDbSearch);
        
    iMultiDbSearch->SetMaxNumOfMatches();
    CleanupStack::PopAndDestroy(2, crit);
    }

// ---------------------------------------------------------
// CPosTp121::TestDatabaseListL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestDatabaseListL(const RIdArray& aExpectedMatches)
    {
    iLog->Log(_L("TestDatabaseListL"));
    _LIT(KNonExistingDb, "Jimmi.ldb");
    // Append an invalid(missing extension) database and search
    
    iDatabases->AppendL(_L("Jimmi"));
    iNrOfDatabases++;
    iMultiDbSearch->SetDatabasesToSearchL(*iDatabases);
    
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    catSearchCriteria->SetGlobalCategory(15000); // Entertainment
    SearchL(catSearchCriteria, ESynchronous);
    
    ValidateSearchResultsL(aExpectedMatches, iMultiDbSearch, 1); // one error expected
    CPosLmMultiDbSearch::TSearchError searchError;
    iMultiDbSearch->GetSearchError(0, searchError);
    AssertTrueSecL(searchError.iDatabaseIndex == 5, _L("Error in wrong database"));
    AssertTrueSecL(searchError.iErrorCode == KErrArgument, _L("Wrong error in database, %d"), searchError.iErrorCode);
    
    // Remove non-existing database + another one and search - no errors should occur
    TPtrC dbName = (*iDatabases)[iDatabases->Count() - 2];
    iDatabases->Delete(iDatabases->Count() - 2, 2);
    iNrOfDatabases = iNrOfDatabases - 2;
    iMultiDbSearch->SetDatabasesToSearchL(*iDatabases);
    
    SearchL(catSearchCriteria, EAsynchronous);
    
    ValidateSearchResultsL(aExpectedMatches, iMultiDbSearch);
    
    iDatabases->AppendL(KDb105);
    iNrOfDatabases++;
    CleanupStack::PopAndDestroy(catSearchCriteria);

    // Verify that NewL for CPosLmMultiDbSearch is correct
    iDatabases->AppendL(KNonExistingDb);
    iNrOfDatabases++;
    CPosLmMultiDbSearch* multiDb = CPosLmMultiDbSearch::NewL(*iDatabases);
    CleanupStack::PushL(multiDb);
    
    catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    catSearchCriteria->SetGlobalCategory(15000); // Entertainment

    iOperation = multiDb->StartLandmarkSearchL(*catSearchCriteria, EFalse);
    ExecuteAndDeleteLD(iOperation);
    
    ValidateSearchResultsL(aExpectedMatches, multiDb, 1); // one error expected
    multiDb->GetSearchError(0, searchError);
    AssertTrueSecL(searchError.iDatabaseIndex == 5, _L("Error in wrong database"));
    AssertTrueSecL(searchError.iErrorCode == KErrNotFound, _L("Wrong error in database, %d"), searchError.iErrorCode);
    
    // Remove added db from list
    TInt pos = 0;
    
    TInt res = iDatabases->Find(KNonExistingDb, pos);
    if (res == KErrNone) iDatabases->Delete(pos);
    iNrOfDatabases--;
    CleanupStack::PopAndDestroy(catSearchCriteria);
    CleanupStack::PopAndDestroy(multiDb);
    }

// ---------------------------------------------------------
// CPosTp121::TestOnTheFlySearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp121::TestOnTheFlySearchL(const RIdArray& aExpectedMatches)
    {
    iMultiDbSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
            
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    catSearchCriteria->SetGlobalCategory(15000); // Entertainment
    COnTheFlyTester* onTheFlyTester = COnTheFlyTester::NewLC();
    onTheFlyTester->StartSearchCampaignL(iMultiDbSearch, aExpectedMatches, catSearchCriteria);
    TInt result = onTheFlyTester->Err();
    if (result != KErrNone)
        {
        TPtrC msg;
        onTheFlyTester->GetErrMsg(msg);
        iLog->Log(msg);
        User::Leave(result);
        }
    CleanupStack::PopAndDestroy(2, catSearchCriteria);
    }

// ---------------------------------------------------------
// CPosTp121::NrOfSearchErrors
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp121::NrOfSearchErrors(CPosLmMultiDbSearch* aMultiDbSearcher)
    {    
    TInt nrOfSearchErrors = aMultiDbSearcher->NumOfSearchErrors();
    if (nrOfSearchErrors != 0)
        {
        for (TInt i = 0; i < nrOfSearchErrors; i++)
            {
            CPosLmMultiDbSearch::TSearchError error;
            aMultiDbSearcher->GetSearchError(i, error);
            TBuf<100> msg;
            msg.Format(_L("Found error %d in database %d"), error.iErrorCode, error.iDatabaseIndex);
            iLog->Log(msg);
            }
        }
    return nrOfSearchErrors;
    }
    
//  End of File
