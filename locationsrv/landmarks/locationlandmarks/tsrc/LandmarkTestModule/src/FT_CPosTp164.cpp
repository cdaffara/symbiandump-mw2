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

#include "FT_CPosTp164.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLandmarkParser.h>

#include <LbsPosition.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
           
_LIT(KTp164DbFile, "TP164Test.ldb");


// ================= CONSTANTS =======================


	_LIT(KKMZFile, "c:\\system\\test\\testdata\\Tp164KMZ.kmz");


#define DEBUG ETrue
                                                    
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp164::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp164::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CPosTp164::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp164::StartL()
    {
    iErrorsFound = 0;
    
      
     // KML
    TestImportEmptyDbL( KKMZFile, KKmzMimeType );
    TestImport1L( KKMZFile, KKmzMimeType );

    if (iErrorsFound != KErrNone)
        {
        iLog->Log(_L("Errors found in TP164"));
        User::Leave(-1);
        }
    }

// ---------------------------------------------------------
// CPosTp164::TestImportEmptyDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp164::TestImportEmptyDbL(const TDesC& aFile, const TDesC8& aMime)
    {
    iLog->Log(_L("----- TestImportEmptyDbL -----"));
    iLog->Log(_L("FILE: %S"), &aFile);
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KTp164DbFile);
    iLandmarkParser = CPosLandmarkParser::NewL(aMime);
    iLandmarkParser->SetInputFileL(aFile);
    
    iDatabase = CPosLandmarkDatabase::OpenL(KTp164DbFile);
    if (iDatabase->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(iDatabase->InitializeL());
        }
        
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    
    CPosLmItemIterator* catIter = categoryManager->CategoryIteratorL();
    CleanupStack::PushL(catIter);
    TInt nrOfcategoriesBefore = catIter->NumOfItemsL();
    CleanupStack::PopAndDestroy(catIter);
    
    iLog->Log(_L("iDatabase->ImportLandmarksL"));
    // No categories should be imported, since ESupressCategoryCreation is used and the landmarks
    // db is empty when starting
    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*iLandmarkParser, CPosLandmarkDatabase::ESupressCategoryCreation| CPosLandmarkDatabase::EIncludeCategories));

    catIter = categoryManager->CategoryIteratorL();
    CleanupStack::PushL(catIter);
    TInt nrOfcategoriesAfter = catIter->NumOfItemsL();
    CleanupStack::PopAndDestroy(catIter);
    
    CleanupStack::PopAndDestroy(categoryManager);
    
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    
    delete iDatabase;
    iDatabase = NULL;
    
    if (nrOfcategoriesAfter != nrOfcategoriesBefore)
    	{
    	iLog->Log(_L("ERROR: No categories should have been imported"));
        iErrorsFound++;
    	}
    iLog->Log(_L("-------------------\n"));
    }
    
// ---------------------------------------------------------
// CPosTp164::TestImport1L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp164::TestImport1L(const TDesC& aFile, const TDesC8& aMime)
    {
    iLog->Log(_L("----- TestImport1L ------"));
    iLog->Log(_L("FILE: %S"), &aFile);
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KTp164DbFile);
    iLandmarkParser = CPosLandmarkParser::NewL(aMime);
    iLandmarkParser->SetInputFileL(aFile);
    
    TRAPD( err,iDatabase = CPosLandmarkDatabase::OpenL(KTp164DbFile));
    iLog->Log(_L("Error after CPosLandmarkDatabase::OpenL = %d"),err);
   
    if (iDatabase->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(iDatabase->InitializeL());
        }
    
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    // Create two local categories, "Dagis" and "BurgerKing" belongs to landmark "asdf"
    CPosLandmarkCategory* category1 = CPosLandmarkCategory::NewLC();
    category1->SetCategoryNameL(_L("Dagis"));
    TPosLmItemId id1 = categoryManager->AddCategoryL(*category1);
    CleanupStack::PopAndDestroy(category1);
    
    CPosLandmarkCategory* category2 = CPosLandmarkCategory::NewLC();
    category2->SetCategoryNameL(_L("BurgerKing"));
    TPosLmItemId id2 = categoryManager->AddCategoryL(*category2);
    CleanupStack::PopAndDestroy(category2);
    
    // Create "Frisör" belongs to landmark "TE, Lund"
    CPosLandmarkCategory* category3 = CPosLandmarkCategory::NewLC();
    category3->SetCategoryNameL(_L("Frisör"));
    TPosLmItemId id3 = categoryManager->AddCategoryL(*category3);
    CleanupStack::PopAndDestroy(category3);
    
    // Remove global category  with global id 1
    // Belongs to "Kalles Hundgård"
    TPosLmItemId globalId = categoryManager->GetGlobalCategoryL(3000);
    ExecuteAndDeleteLD(categoryManager->RemoveCategoryL(globalId));
    
    CPosLmItemIterator* catIter = categoryManager->CategoryIteratorL();
    CleanupStack::PushL(catIter);
    TInt nrOfcategoriesBefore = catIter->NumOfItemsL();
    CleanupStack::PopAndDestroy(catIter);   
    
    iLog->Log(_L("iDatabase->ImportLandmarksL"));

    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*iLandmarkParser, CPosLandmarkDatabase::EIncludeGlobalCategoryNames | CPosLandmarkDatabase::ESupressCategoryCreation| CPosLandmarkDatabase::EIncludeCategories));
    

    catIter = categoryManager->CategoryIteratorL();
    CleanupStack::PushL(catIter);
    TInt nrOfcategoriesAfter = catIter->NumOfItemsL();
    CleanupStack::PopAndDestroy(catIter);
    if (nrOfcategoriesAfter != nrOfcategoriesBefore) 
    {
    	iLog->Log(_L("ERROR: Three categories should have been added"));
        iErrorsFound++;
    }
    
    // Check that landmark "TE, Lund" has "Frisör" and
    // landmark "asdf" has "Dagis" and "BurgerKing" (and also already existing global categories 
    
    TBuf<100> buf;
    
    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);    
    
    TPosLmItemId idde1;
   
    // Check id=5 Te, Lund
    idde1 = categoryManager->GetCategoryL(_L("Frisör"));    
    categories.Append(idde1);
    CheckLandmarkL(5, categories);
    categories.Reset();
    
    // Check id=6 asdf
    idde1 = categoryManager->GetCategoryL(_L("Dagis"));
    
    categories.Append(idde1);
    
    CheckLandmarkL(6, categories);
    categories.Reset();
    
    CleanupStack::PopAndDestroy(&categories);
    CleanupStack::PopAndDestroy(categoryManager);
    
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    
    delete iDatabase;
    iDatabase = NULL;
    iLog->Log(_L("-------------------\n"));
    }

// ---------------------------------------------------------
// CPosTp164::CheckLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp164::CheckLandmarkL(TPosLmItemId aLandmarkId, RArray<TPosLmItemId> aCategoriesList)
    {
    iLog->Log(_L("CheckLandmarkL"));
    
    // Get landmark
    CPosLandmark* lm1 = iDatabase->ReadLandmarkLC(aLandmarkId);
    TPtrC name1;
    lm1->GetLandmarkName(name1);
    iLog->Log(name1);
    
    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);
    // Get all categories attached to this landmark
    lm1->GetCategoriesL(categories);
   
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    
    if ( aCategoriesList.Count() != categories.Count() )
        {
        iLog->Log(_L("ERROR: Wrong number of categories, actual %d, expected %d"),
            categories.Count(), aCategoriesList.Count() );
        iErrorsFound++;
        }
    
    for (TInt i=0;i<categories.Count();i++)
        {
        if (aCategoriesList.Find(categories[i]) != KErrNotFound)
            {
            iLog->Log(_L("Found"));
            }
        else 
            {
            iLog->Log(_L("ERROR: Category '%S' was not found"));
            iErrorsFound++;
            }
        }
    
    CleanupStack::PopAndDestroy(categoryManager);

    CleanupStack::PopAndDestroy(&categories);
    CleanupStack::PopAndDestroy(lm1);
    }
    
  

	
//  End of File
