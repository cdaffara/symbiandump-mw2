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

#include "FT_CPosTp161.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLandmarkParser.h>
           
// ================= CONSTANTS =======================

  _LIT(KTp161DbFile, "TP161Test.ldb");
    _LIT(KKMLFile, "c:\\system\\test\\testdata\\Tp161KML.kml");

    
    _LIT(KEmpty, "");

    _LIT(K1, "Category nr1 added in TP48");
        
    _LIT(K2, "Läkare");
    _LIT(K3, "Sjukhus");
    _LIT(K4, "Restaurant");
    _LIT(K5, "Shop");
    
    _LIT(K6, "Frisör");
    
    _LIT(K7, "Dagis");
    _LIT(K8, "BurgerKing");
    _LIT(K9, "Hotel");
    _LIT(K10, "Car park");
    
    _LIT(K11, "Garage");
    _LIT(K12, "Bus stop");
    _LIT(K13, "BurgerKing");
    
    _LIT(K14, "Category nr2 added in TP48");
    _LIT(K15_1, "OutSight");
    const TInt KNrOfLandmarks = 8;
                                                    
// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp161::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp161::CloseTest()
    {
    delete iLandmarkParser;
        iLandmarkParser = NULL;
        
        delete iDatabase;
        iDatabase = NULL;
    // Release ECOM stuff
    ReleaseLandmarkResources();
    
    delete iOperation;iOperation=NULL;
    }

// ---------------------------------------------------------
// CPosTp161::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp161::StartL()
    {
    iErrorsFound = 0;
    
       
    // KML
    TestParseL(KKMLFile, KKmlMimeType);
   TestImportL(KKMLFile, KKmlMimeType);
    
    if (iErrorsFound != KErrNone)
        {
        iLog->Log(_L("Errors found in TP161"));
        User::Leave(-1);
        }
	}

// ---------------------------------------------------------
// CPosTp161::TestParseL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp161::TestParseL(const TDesC& aFile, const TDesC8& aMime)
    {
    iLog->Log(_L("----- TestParseL -----"));
    iLog->Log(_L("FILE: %S"), &aFile);
    
    RemoveAllLmDatabasesL();
    
        const TDesC* categoryNames[KNrOfLandmarks][4] = {{&KEmpty, &KEmpty, &KEmpty, &KEmpty},
                                            {&K1, &KEmpty, &KEmpty, &KEmpty},
                                            {&K2, &K3, &K4, &K5},
                                            {&KEmpty, &KEmpty, &KEmpty, &KEmpty},
                                            {&K6, &KEmpty, &KEmpty, &KEmpty},
                                            {&K7, &K8, &K9, &K10},
                                            {&K11, &K12, &K13, &KEmpty},
                                            {&K14, &K15_1, &KEmpty, &KEmpty}};

    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(aMime);
    CleanupStack::PushL(parser);
    
    RFile file; 
    TInt err = file.Open(iFileSession, aFile, EFileRead);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error when opening file"));
        User::Leave(err);
    }
    CleanupClosePushL(file);
    parser->SetInputFileHandleL(file);
    iOperation = parser->ParseContentL();
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress=0;
    
    CDesCArrayFlat* landmarkNames = new (ELeave) CDesCArrayFlat(KNrOfLandmarks);
    landmarkNames->AppendL(_L("Billigt"));
    landmarkNames->AppendL(_L("LOG9"));
    landmarkNames->AppendL(_L("Kalles Hundgård"));
    landmarkNames->AppendL(_L("MLFW"));
    landmarkNames->AppendL(_L("TE, Lund"));
    landmarkNames->AppendL(_L("asdf"));
    landmarkNames->AppendL(_L("TP48LandmarkWithAllXMLFields"));
    landmarkNames->AppendL(_L("TP48Landmark With Empty XML Fields"));
    
    TInt counter=0;
    while (progress != 1)
        {
        iOperation->NextStep(status, progress);
        // Wait for NextStep to complete
        User::WaitForRequest(status);
        CPosLandmark* lm = parser->LandmarkLC();
        TPtrC name;
        lm->GetLandmarkName(name);
        TPtrC expName(landmarkNames->MdcaPoint(counter));
        iLog->Log(_L("Landmark %d, Actual name '%S', Expected '%S'"), counter, &name, &expName);
        if (expName.CompareC(name) != KErrNone)
            {
            iLog->Log(_L("ERROR: Wrong Landmark Name"));
        	iErrorsFound++;
            }
        
        RArray<TPosLmItemId> arr;
        CleanupClosePushL(arr);
        
        lm->GetCategoriesL(arr);
        for (TInt i=0;i<arr.Count();i++)
            {
            CPosLandmarkCategory* cat = parser->LandmarkCategoryLC(arr[i]);
            TPtrC catName;
            cat->GetCategoryName(catName);
            iLog->Log(_L("Category %d, Actual '%S', Expected '%S'"), i, &catName, categoryNames[counter][i]);
            if (catName.CompareC(*categoryNames[counter][i]) != KErrNone)
            	{
            	iLog->Log(_L("ERROR: Wrong Category Name when comparing"));
                iErrorsFound++;
            	}
            CleanupStack::PopAndDestroy(cat);
            }
        
        CleanupStack::PopAndDestroy(&arr);
        CleanupStack::PopAndDestroy(lm);
        counter++;
        }
        
    TUint32 nr = parser->NumOfParsedLandmarks();
    if (nr != KNrOfLandmarks) 
    {
    	iLog->Log(_L("ERROR: Wrong number of landmarks parsed"));
        iErrorsFound++;
    }
    
    iLog->Log(_L("-------------------\n"));
    delete iOperation;iOperation=NULL;
       
    CleanupStack::PopAndDestroy(&file);
    
    landmarkNames->Reset();
    delete landmarkNames;
    landmarkNames = NULL;   
    CleanupStack::PopAndDestroy(parser);    
    }


// ---------------------------------------------------------
// CPosTp161::TestImport1L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp161::TestImportL(const TDesC& aFile, const TDesC8& aMime)
    {
    iLog->Log(_L("----- TestImport1L ------"));
    iLog->Log(_L("FILE: %S"), &aFile);
    RemoveAllLmDatabasesL();
    CopyTestDbFileL(KTp161DbFile);
    iLandmarkParser = CPosLandmarkParser::NewL(aMime);
    iLandmarkParser->SetInputFileL(aFile);
    
    TRAPD( err,iDatabase = CPosLandmarkDatabase::OpenL(KTp161DbFile));
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
// CPosTp161::CheckLandmarkL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp161::CheckLandmarkL(TPosLmItemId aLandmarkId, RArray<TPosLmItemId> aCategoriesList)
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
