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


// INCLUDES
#include "FT_CPosTp123.h"
#include "FT_LandmarkConstants.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmDisplayItem.h>
     
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp123::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*void CPosTp123::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp123 - Uncategorized LM MultiDb Search");
    aName = KTestName;
    }
*/
// ---------------------------------------------------------
// CPosTp123::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::CloseTest()
    {
    delete iDatabases;
    delete iLandmarkSearch;
    for (TInt i = 0; i < iIdArrays.Count(); i++)
        {
        iIdArrays[i].Close();
        }
    iIdArrays.Close();
    }

// ---------------------------------------------------------
// CPosTp123::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::InitTestL()
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
    
    // Initialize data members
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);
    iDatabases = dbManager->ListDatabasesLC();
    CleanupStack::Pop(iDatabases);
    iDatabases->Sort(); 
    AssertTrueSecL((iDatabases->Count() == iNrOfDatabases), _L("Wrong number of databases"));
    iLandmarkSearch = CPosLmMultiDbSearch::NewL(*iDatabases);
    CleanupStack::PopAndDestroy(dbManager);
    
    iLog->Log( _L("Multi db search created"));
    
    // Enable use of global categories
    // CPosLandmarkDatabase* database = UseGlobalCategoriesL();
    // delete database;
    CPosLandmarkDatabase* database = NULL;
    // Reset Global categories for all databases but EPOSLM_105.LDB and add the global category
    // "Hotel" to the 5 first landmarks in the databases.
    for (TInt i = 0; i < iNrOfDatabases - 1; i++) // Do not use global categories in 
        {
        database = CPosLandmarkDatabase::OpenL((*iDatabases)[i]);
        CleanupStack::PushL(database);
        if ( database->IsInitializingNeeded() )
            {
            TRAPD( err, ExecuteAndDeleteLD( database->InitializeL() ) );
            AssertTrueSecL( err == KErrNone, _L("Init db failed"));
            }
        CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*database);
        CleanupStack::PushL(catMan);
        ExecuteAndDeleteLD(catMan->ResetGlobalCategoriesL());
        CPosLmItemIterator* lmIter = database->LandmarkIteratorL();
        CleanupStack::PushL(lmIter);
        RIdArray idArray;
        CleanupClosePushL(idArray);
        lmIter->GetItemIdsL(idArray, 0, 5);
        
        // Remove all existing categories from the 5 first landmarks
        for (TInt i = 0; i < idArray.Count(); i++)
            {
            CPosLandmark* lm = database->ReadLandmarkLC(idArray[i]);
            lm->RemoveLandmarkAttributes(CPosLandmark::ECategoryInfo);
            TInt err = KErrLocked;
            while (err==KErrLocked)
                {
                TRAP(err, database->UpdateLandmarkL(*lm));
                }
            CleanupStack::PopAndDestroy(lm);
            }
        
        // Add global category hotel to 5 first LMs
        TPosLmGlobalCategory accommodation(3000); // 27 == Global category id for accommodation according to .the global ids added for BC testing
        User::After(200000);
        ExecuteAndDeleteLD(catMan->AddCategoryToLandmarksL(catMan->GetGlobalCategoryL(accommodation), idArray));
        
        CleanupStack::PopAndDestroy(4, database);
        }
        
    // Initialize arrays that will contain expected search results
    RIdArray array20;
    User::LeaveIfError(iIdArrays.Append(array20));
    RIdArray array40;
    User::LeaveIfError(iIdArrays.Append(array40));
    RIdArray array60;
    User::LeaveIfError(iIdArrays.Append(array60));
    RIdArray array80;
    User::LeaveIfError(iIdArrays.Append(array80));
    RIdArray array105;
    User::LeaveIfError(iIdArrays.Append(array105));
    }

// ---------------------------------------------------------
// CPosTp123::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::StartL()
    {
// 1. Verify KErrArgument is returned from API
    iLog->Log(_L("<<< 1. Verify KErrArgument is returned from API >>>"));
    for (TBool runSynch = EFalse; runSynch <= ETrue; runSynch++)
        {
        iLog->Log(_L("Should leave with KErrArgument..."));
        TRAPD(err, TestSearchCategoriesL(runSynch));
        AssertTrueSecL(err == KErrArgument, _L("Should have left with KErrArgument but failed with %d"), err);
        iLog->Log(_L("Function left correctly with KErrArgument"));
        }

// 2. We should not find any uncategorized landmarks since every landmark in the db
// belongs to a category
    iLog->Log(_L("<<< 2. Search uncategorized LMs (none exist) >>>"));
    SearchForUncategorizedLMsL(0);

// 3. Remove all categories from M landmarks in each database
    iLog->Log(_L("<<< 3. Remove all categories from M landmarks in each database >>>"));
    const TInt M = 2;
    RemoveCategoriesFromLandmarksL(M);
    // Check that correct number of uncategorized landmarks are found
    SearchForUncategorizedLMsL(M * iNrOfDatabases);
    
// 4. Remove a local category that Q LMs belong to and add an uncategorized lm
    iLog->Log(_L("<<< 4. Remove a local category that Q LMs belong to and add an uncategorized lm >>>"));
    const TInt Q = 6;
    TInt dbIndex = 3;
    TPosLmItemId localCatId = 26; // Läkare
    RemoveCategoryFromDbL(localCatId, (*iDatabases)[dbIndex]);
    // Landmark 72-77 should now be uncategorized.
    for (TInt i = 72; i <= 77; i++)
        {        
        iIdArrays[dbIndex].Append(i);
        }
        
    // Add a lm
    CPosLandmark* lm = CPosLandmark::NewLC();
    lm->SetLandmarkNameL(_L("Guru"));
    CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL((*iDatabases)[dbIndex]);
    CleanupStack::PushL(database);
    TPosLmItemId id = database->AddLandmarkL(*lm);
    iIdArrays[dbIndex].Append(id);
    CleanupStack::PopAndDestroy(2, lm);
        
    // Check that correct number of uncategorized landmarks are found
    SearchForUncategorizedLMsL(Q + 1 + M*iNrOfDatabases);

// 5. Associate an uncategorized landmark with a category, e.g. lm with id 76
    iLog->Log(_L("<<< 5. Associate an uncategorized landmark with a category >>>"));
    id = 76;
    database = CPosLandmarkDatabase::OpenL((*iDatabases)[dbIndex]);
    CleanupStack::PushL(database);
    lm = database->ReadLandmarkLC(id);
    lm->AddCategoryL(localCatId - 1); // Sjukhus
    database->UpdateLandmarkL(*lm);
    TInt index = iIdArrays[dbIndex].Find(id);
    AssertTrueSecL(index > 0, _L("Unable to find landmark in array"));
    iIdArrays[dbIndex].Remove(index);
    CleanupStack::PopAndDestroy(2, database);

    // Check that correct number of uncategorized landmarks are found
    SearchForUncategorizedLMsL(Q + M*iNrOfDatabases);
    
// 6. Remove the global category Hotel, that K LMs belong to, from 2 databases 
    iLog->Log(_L("<<< 6. Remove the global category Hotel, that K LMs belong to, from 2 databases >>>"));
    //const TPosLmGlobalCategory KHotel = 4;
    const TPosLmGlobalCategory KAccommodation = 3000;
    const TInt K = 2*3; // 2 dbs and three in each have hotel as category
    for (TInt dbIndex = 0; dbIndex <= 1; dbIndex++)
        {
        CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL((*iDatabases)[dbIndex]);
        CleanupStack::PushL(db);
        CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*db);
        CleanupStack::PushL(catMan);
        TPosLmItemId itemId = catMan->GetGlobalCategoryL(KAccommodation);
        RemoveCategoryFromDbL(itemId, (*iDatabases)[dbIndex]);
        CleanupStack::PopAndDestroy(2, db);
        }
    
    // Landmarks 3-5 and 23-25 should now be uncategorized.
    dbIndex = 0;
    for (TInt i = 3; i <= 5; i++)
        {
        iIdArrays[dbIndex].Append(i);
        }
    for (TInt i = 23; i <= 25; i++)
        {
        iIdArrays[dbIndex + 1].Append(i);
        }
        
    // Check that correct number of uncategorized landmarks are found
    SearchForUncategorizedLMsL(K + Q + M*iNrOfDatabases);

// 7. Exclude one database (the last)
    iLog->Log(_L("<<< 7. Exclude one database >>>"));
    iDatabases->Delete(4);
    iNrOfDatabases--;
    iLandmarkSearch->SetDatabasesToSearchL(*iDatabases);
    SearchForUncategorizedLMsL(K + Q + M*iNrOfDatabases);
    
// 8. Test maximum nr of matches
    iLog->Log(_L("<<< 8. Test maximum nr of matches >>>"));
    const TInt P = 1;
    iLandmarkSearch->SetMaxNumOfMatches(P);
    SearchForUncategorizedLMsL(P * iNrOfDatabases);
    
// 9. Test maximum nr of matches and sort
    iLog->Log(_L("<<< 9. Test maximum nr of matches and sort >>>"));
    iLandmarkSearch->SetMaxNumOfMatches(2);
    SearchForUncategorizedLMsL(2 * iNrOfDatabases, ETrue);
    
// 10. Test on-the-fly sorting
    iLog->Log(_L("<<< 10. Test on-the-fly sorting >>>"));
    iLandmarkSearch->SetMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited);
    
    // Create one big array with all expected matches
    RIdArray expectedMatches;
    CleanupClosePushL(expectedMatches);
    for (TInt i = 0; i < iNrOfDatabases; i++)
        {
        for (TInt j = 0; j < iIdArrays[i].Count(); j++)
            {            
            User::LeaveIfError(expectedMatches.Append(iIdArrays[i][j]));
            }
        }
        
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
    COnTheFlyTester* onTheFlyTester = COnTheFlyTester::NewLC();
    onTheFlyTester->StartSearchCampaignL(iLandmarkSearch, expectedMatches, catSearchCriteria);
    TInt result = onTheFlyTester->Err();
    if (result != KErrNone)
        {
        TPtrC msg;
        onTheFlyTester->GetErrMsg(msg);
        iLog->Log(msg);
        User::Leave(result);
        }
    CleanupStack::PopAndDestroy(3, &expectedMatches);
	}

// ---------------------------------------------------------
// CPosTp123::TestSearchL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::TestSearchL(
    TBool aExecuteSync, 
    TSearchCriterion aCriterion, 
    TInt aNrOfHits,
    TBool aSorted)
    {
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    // Specified in header file EPos_CPosLmCategoryCriteria.h says that
    // specifying no criteria should return uncategorized landmarks
    // SetCategoryItemId(KPosLmNullItemId) should return uncategorized landmarks and that
    // SetGlobalCategory(KPosLmNullGlobalCategory) should return uncategorized landmarks and that
    // SetCategoryNameL(KNullDesC) should return uncategorized landmarks
    switch (aCriterion)
        {
        case ENoCriterion:
            // Do nothing. Default initialized CPosLmCategoryCriteria is fine.
            break;
        case ELocalCategoryId:
            catSearchCriteria->SetCategoryItemId(KPosLmNullItemId);
            break;
        case EGlobalCategoryId:
            catSearchCriteria->SetGlobalCategory(KPosLmNullGlobalCategory);
            break;
        case ECategoryName:
            catSearchCriteria->SetCategoryNameL(KNullDesC);
            break;
        }

    if (aSorted)
        {
        TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria, sortPref);
        }
    else
        {        
        iOperation = iLandmarkSearch->StartLandmarkSearchL(*catSearchCriteria);
        }

    if (aExecuteSync)
        {
        ExecuteAndDeleteLD(iOperation);
        }
    else
        {
        RunAsyncOperationLD(iOperation);
        }

    VerifySearchResultL(aNrOfHits, aSorted);
    
    CleanupStack::PopAndDestroy(catSearchCriteria);
	}

// ---------------------------------------------------------
// CPosTp123::TestSearchCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::TestSearchCategoriesL(TBool aExecuteSync)
    {
    CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();

    // This method should leave according to EPos_CPosLmCategoryCriteria.h
    iOperation = iLandmarkSearch->StartCategorySearchL(
        *catSearchCriteria, CPosLmCategoryManager::ECategorySortOrderNone);

    if (aExecuteSync)
        {
        ExecuteAndDeleteLD(iOperation);
        }
    else
        {
        RunAsyncOperationLD(iOperation);
        }

    CleanupStack::PopAndDestroy(catSearchCriteria);
    }

// ---------------------------------------------------------
// CPosTp123::RemoveCategoriesFromLandmarksL
// Use this method to remove the category from a specified number of landmarks
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::RemoveCategoriesFromLandmarksL(TInt aNrOfLandmarks)
    {
    for (TInt i = 0; i < iNrOfDatabases; i++)
        {
        CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL((*iDatabases)[i]);
        CleanupStack::PushL(database);
        ExecuteAndDeleteLD(database->CompactL());
        CPosLmItemIterator* iter = database->LandmarkIteratorL();
        CleanupStack::PushL(iter);
        
        TPosLmItemId id = iter->NextL();
        TInt counter=0;
        
        // Remove the categories from aNrOfLandmarks nr of landmarks
        while ((id != KPosLmNullItemId) && (counter < aNrOfLandmarks))
            {
            CPosLandmark* landmark = database->ReadLandmarkLC(id);
            landmark->RemoveLandmarkAttributes(CPosLandmark::ECategoryInfo);
            TInt err = KErrLocked;
            while (err == KErrLocked)
                {
                TRAP(err, database->UpdateLandmarkL(*landmark));
                }
            CleanupStack::PopAndDestroy(landmark);
            User::LeaveIfError(iIdArrays[i].Append(id));
            
            id = iter->NextL();
            if (id == KPosLmNullItemId) 
                {
                iLog->Log(_L("KPosLmNullItemId"));
                }
            counter++;
            }
            
        CleanupStack::PopAndDestroy(2, database);
        }
    }

// ---------------------------------------------------------
// CPosTp123::SearchForUncategorizedLMsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::SearchForUncategorizedLMsL(TInt aNrOfHits, TBool aSorted)
    {
    for (TBool runSynch = EFalse; runSynch < 2; runSynch++)
        {
        for (TSearchCriterion searchCriterion = ENoCriterion; 
             searchCriterion <= ECategoryName; 
             searchCriterion = (TSearchCriterion)((TInt)searchCriterion + 1))
            {
            TestSearchL(runSynch, searchCriterion, aNrOfHits, aSorted);
            }
        }
    }
    
// ---------------------------------------------------------
// CPosTp123::RemoveCategoryFromDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::RemoveCategoryFromDbL(
    TPosLmItemId aCategoryId, 
    const TDesC& aDbUri)
    {
    CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL(aDbUri);
    CleanupStack::PushL(database);
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*database);
    CleanupStack::PushL(catMan);
    
    ExecuteAndDeleteLD(catMan->RemoveCategoryL(aCategoryId));
        
    CleanupStack::PopAndDestroy(2, database);
    }
    
// ---------------------------------------------------------
// CPosTp123::VerifySearchResultL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::VerifySearchResultL(
    TInt aNrOfHits, 
    TBool aSorted)
    {    
    AssertTrueSecL(iLandmarkSearch->NumOfSearchErrors() == 0, _L("Found search errors!"));
    
    // Verify total number of matches is correct
    TInt totNrOfMatches = iLandmarkSearch->TotalNumOfMatches();
    if (totNrOfMatches != aNrOfHits)
        {
        iBuf.Format(_L("Wrong number of landmarks returned when searching for uncategorized landmarks, found: %d expected %d"), totNrOfMatches, aNrOfHits);
        iLog->Log(iBuf);
        User::Leave(-1);
        }
        
        
    for (TUint i = 0; i < iNrOfDatabases; i++)
        {
        CPosLandmarkDatabase* database = CPosLandmarkDatabase::OpenL((*iDatabases)[i]);
        CleanupStack::PushL(database);
        CPosLmItemIterator* iter = iLandmarkSearch->MatchIteratorL(i);
        CleanupStack::PushL(iter);
        
        // Verify number of matches in each database is correct
        TInt nrInIter = iter->NumOfItemsL();
        AssertTrueSecL(nrInIter == iLandmarkSearch->NumOfMatches(i), _L("wrong number of items returned in a specific iterator!"));

        // Verify that correct landmarks were found
        TPosLmItemId id = iter->NextL();
        TInt index = 0;
        while (id != KPosLmNullItemId)
            {
            TInt found = iIdArrays[i].Find(id);
            if (found == KErrNotFound) 
                {
                iLog->Log(_L("Not found"));
        		User::Leave(KErrNotFound);
                }
            else 
                {
                index++;
                CPosLandmark* landmark = database->ReadLandmarkLC(id);
                CPosLandmark* landmark2 = database->ReadLandmarkLC(iIdArrays[i][found]);

                CompareLandmarksL(*landmark, *landmark2);

                CleanupStack::PopAndDestroy(landmark2);
                CleanupStack::PopAndDestroy(landmark);             
                }
            id = iter->NextL();
            }

        if (index != nrInIter) 
            {
            iLog->Log(_L("The correct landmark was never found"));
        	User::Leave(-1);
                
            }
        if (aSorted)
            {
            VerifySortOrderL(iter, database);
            }
        CleanupStack::PopAndDestroy(2, database);
        }
    }
    
// ---------------------------------------------------------
// CPosTp123::VerifySortOrderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp123::VerifySortOrderL(
    CPosLmItemIterator* aIter, 
    CPosLandmarkDatabase* aDb)
    {
    RIdArray idArray;
    CleanupClosePushL(idArray);
    TInt nrOfItems = aIter->NumOfItemsL();
    aIter->GetItemIdsL(idArray, 0, nrOfItems);
    
    for (TInt i = 0; i < nrOfItems; i++)
        {
        if (i < nrOfItems - 1)
            {
            CPosLandmark* lm1 = aDb->ReadLandmarkLC(idArray[i]);
            CPosLandmark* lm2 = aDb->ReadLandmarkLC(idArray[i + 1]);
            TPtrC name1, name2;
            lm1->GetLandmarkName(name1);
            lm2->GetLandmarkName(name2);
            AssertTrueSecL(name1.CompareC(name2) <= 0, _L("lm1 is greater than lm2 - should not be the case since ascending order is used."));
            CleanupStack::PopAndDestroy(2, lm1);            
            }
        }
    CleanupStack::PopAndDestroy(&idArray);
    }

//  End of File
