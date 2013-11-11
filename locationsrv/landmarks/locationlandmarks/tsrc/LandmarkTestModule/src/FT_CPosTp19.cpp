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

#include "FT_CPosTp19.h"
#include <EPos_CPosLandmarkDatabase.h>  
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkSearch.h> 

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp19::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp19::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;
    
    delete iCategoryManager;
    iCategoryManager = NULL;
    
    iCategoryIds.Close();
    iCategoryNames.ResetAndDestroy();
    
    iGlobalCategoryNames.Close();
    iGlobalCategoryIds.Close();    
    
    iCategories.ResetAndDestroy();
    iCategories.Close();    
    }

// ---------------------------------------------------------
// CPosTp19::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp19::StartL()
    {    
    iLog->Log(_L("**** Testing CPosCategoryManager():: GetCategoires methods ****"));    
    
    
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();   
        
    iDatabase = CPosLandmarkDatabase::OpenL(); // return default database    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());        
       }
       
    iLog->Log(_L(" Default database opened and initialised "));    

    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase); 

    // Test with non-global categories   
    TestGetByNameL();        
    
    /*
     * Test with global categories 
     */   
    
    // Added for testing 
    ExecuteAndDeleteLD(iCategoryManager->ResetGlobalCategoriesL()); 
    
    iLog->Log(_L(" ******** Testing with global categories ******"));    
    
    TestGetGlobalL();      
    }

// ---------------------------------------------------------
// CPosTp19::TestGetGlobal
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp19::TestGetGlobalL()
    {
    _LIT(KGetGlobalErr, "GetGlobalCategoryL should return KPosLmNullItemId for category Id = 0");
    _LIT(KGetGlobalErr2, "GetGlobalCategoryL should return KPosLmNullItemId for non existing categories");    // Rajesh : Commented 
    
    SetupGlobalCategoriesL();
    TBuf<100> info;
    info.Zero();
    TPosLmItemId id(0);
    
    /*
     * Test with global categories 
     */        
    iLog->Log(_L("Testing GetGlobalCategoryL() with non existing global category( catId 0 )"));  
    id = iCategoryManager->GetGlobalCategoryL(0);
    AssertTrueSecL(id == KPosLmNullItemId, KGetGlobalErr);

    iLog->Log(_L("Testing GlobalCategoryNameL() with non existing global category (catid 0)"));      
    HBufC* name = iCategoryManager->GlobalCategoryNameL(0);  
    CleanupStack::PushL(name);
    AssertTrueSecL(name == NULL, KGetGlobalErr);
    CleanupStack::PopAndDestroy(name);
    
    // Test with non existing category id    
    iLog->Log(_L("Testing GetGlobalCategoryL() with non existing global category (CatId = 500)"));      
    id = iCategoryManager->GetGlobalCategoryL(500);
    AssertTrueSecL(id == KPosLmNullItemId, KGetGlobalErr2);
    
    iLog->Log(_L("Testing GlobalCategoryNameL() with non existing global category (CatId = 500)"));      
    name = iCategoryManager->GlobalCategoryNameL(500);
    CleanupStack::PushL(name);
    AssertTrueSecL(name == NULL, KGetGlobalErr2);
    CleanupStack::PopAndDestroy(name);      
    
    iLog->Log(_L("Testing GetGlobalCategoryL() and GlobalCategoryNameL() with valid categories)"));        
    
    for (TInt i=0; i<iGlobalCategoryNames.Count(); i++)
        {        
        // Get the id of the global category in the db
        TPosLmItemId catId = iCategoryManager->GetGlobalCategoryL(iGlobalCategoryIds[i]);        
        CPosLandmarkCategory* category = iCategoryManager->ReadCategoryLC( catId );
        
        // Compare the Ids of the global categories        
        if ( category->GlobalCategory() != iGlobalCategoryIds[i])
            {
            iLog->Log(_L("Incorrect category id returned"));
            _LIT(KIds, "Expected: %d, returned %d");         
            info.Format(KIds, iGlobalCategoryIds[i], category->GlobalCategory());
            iLog->Log(info);
            info.Zero();
            User::Leave(KErrGeneral);   
            }                   
        CleanupStack::PopAndDestroy( category);
        
        HBufC* name = iCategoryManager->GlobalCategoryNameL(iGlobalCategoryIds[i]);
        CleanupStack::PushL(name);

        if (name->CompareC(*iGlobalCategoryNames[i]))
            {
            iLog->Log(_L("Incorrect global category name"));
            iLog->Log(_L("Expected: "));
            iLog->Log(*iGlobalCategoryNames[i]);
            iLog->Log(_L("Returned: "));
            iLog->Log(*name);
            User::Leave(KErrGeneral);
            }
        CleanupStack::PopAndDestroy(name);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::SetupGlobalCategoriesL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
   
 void CPosTp19::SetupGlobalCategoriesL()
    {    
    ReadGlobalCategoriesFromResourceL( iGlobalCategoryIds, iGlobalCategoryNames );
    }


// ---------------------------------------------------------
// CPosTp19::TestGetByName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp19::TestGetByNameL()
    {
    _LIT(KEmptyStringErr, "GetCategoryL with an empty string should return KErrArgument, not %d");
    _LIT(KMaxStringErr, "GetCategoryL with an maximum length string should return KErrArgument, not %d");
    _LIT(KNotFoundErr, "GetCategoryL should return KPosLmNullItemId for a category that doesn't exist in the db, not %d");
   
    iLog->Log(_L(" Testing CPosCategoryManager::GetCategoryL() with wrong arguement (Empty name string)"));    
    TRAPD(err, iCategoryManager->GetCategoryL(_L("")));
    AssertTrueSecL(err == KErrArgument, KEmptyStringErr, err);

    iLog->Log(_L(" Testing CPosCategoryManager::GetCategoryL() with wrong arguement (Name not existing)"));    
    TPosLmItemId categoryId = iCategoryManager->GetCategoryL(_L("kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"));
    AssertTrueSecL(categoryId == KPosLmNullItemId, KNotFoundErr, categoryId);    

    // Test with maximum lenght of name    
    iLog->Log(_L(" Testing CPosCategoryManager::GetCategoryL() with wrong arguement (With max length of Name not existing)"));    
    _LIT(KMaxName124, "Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12_ABC");
    categoryId = iCategoryManager->GetCategoryL(KMaxName124);
    AssertTrueSecL(categoryId == KPosLmNullItemId, KNotFoundErr, categoryId);

    // Test with more than maximum lenght of name
    iLog->Log(_L(" Testing CPosCategoryManager::GetCategoryL() with wrong arguement (With more than max length of Name not existing)"));    
    _LIT(KMaxName, "Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12Category12");
    TRAP(err, iCategoryManager->GetCategoryL(KMaxName));
    AssertTrueSecL(err == KErrArgument, KMaxStringErr, err);   
    
    // Add some categories and verify the addition
    iLog->Log(_L(" Testing with some valid categories in the database ( +ve testing )"));    
    AddCategoriesL (10);
    VerifyAddedCategoriesL();
    }
    
// ---------------------------------------------------------
// CPosTp19:AddCategoriesL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp19::AddCategoriesL( TInt aNumofCats )
    {
    _LIT(KName, "Category - %d");
    TInt err = KErrLocked;
    for (TUint i = 0; i<aNumofCats ; i++)
        {
        TBuf<100> categoryName;
        categoryName.Format( KName,i );       
        CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
        lmc->SetCategoryNameL( categoryName );
        err = KErrLocked;
        while ( err == KErrLocked )
            {
            TRAP(err, iCategoryManager->AddCategoryL( *lmc ));
            }        
        iCategories.Append(lmc);         
        CleanupStack::Pop( lmc );
        }   
    }
    
// ---------------------------------------------------------
// CPosTp19:VerifyAddedCategoriesL()
// (other items were commented in a header).
// ---------------------------------------------------------
//     
void CPosTp19::VerifyAddedCategoriesL()
    {    
     TInt noOfCats = iCategoryNames.Count();     
     // Verify the added categories 
     for ( TUint i = 0; i < noOfCats; i++ )     
         {
         CPosLandmarkCategory* cat = iCategories[i];
         TPtrC catName;
         cat->GetCategoryName( catName );                 
         // Get the categories from the database using name        
         TPosLmItemId catId = iCategoryManager->GetCategoryL( catName);
         
         if ( catId != cat->CategoryId())
             {
             TBuf<150> buf;
             buf.Format(_L("Unable to verify the added non-global category Id %d "), cat->CategoryId() );
             iLog->Log(buf);
             User::Leave(-1);   
             }          
         }        
    }    
//  End of File
