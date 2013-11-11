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
#include "FT_CPosTp20.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmDatabaseManager.h>
#include "FT_LandmarkConstants.h"

// CONSTANTS
// See file eposlmglobalcategories02.rss in testdata for category Bar
//_LIT(KICONFILE, "c:\\system\\data\\test2.mbm");
//const TInt KTP20IconIndex = 2;
//const TInt KTP20IconMaskIndex = 2;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp20::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp20::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    
    delete iDatabase;
    iDatabase = NULL;    
    delete iCategoryManager;
    iCategoryManager = NULL;    
    iGlobalCategoryIds.Close();
    iGlobalCategoryNames.Close();    
    iGlobalCategoryTestNames.Close();    
    }

// ---------------------------------------------------------
// CPosTp20::RemoveAllCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp20::RemoveAllCategoriesL()
    {
    iLog->Log(_L("RemoveAllCategoriesL"));
    CPosLmCategoryManager* catMan = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(catMan);
    
    CPosLmItemIterator* iter = catMan->CategoryIteratorL();
    CleanupStack::PushL(iter);

    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);
    if (iter->NumOfItemsL() > 0)
        {
        iter->GetItemIdsL(ids, 0, iter->NumOfItemsL());
	    ExecuteAndDeleteLD(catMan->RemoveCategoriesL(ids));	
        }
    
    CleanupStack::PopAndDestroy(&ids);
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(catMan);
    }
    
// ---------------------------------------------------------
// CPosTp20::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp20::StartL()
    {     
    iLog->Log(_L("**** Testing CPosCategoryManager::ResetGlobalCategories() ********"));         
     /*
      * Open the default landmark database 
      * By default it is initialized with global categories (Assumption based on global categories API specification)
      * Test the Reset global categories functionality       
      */
      
    TBuf<150> info;  
    iDatabase = CPosLandmarkDatabase::OpenL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
    
    iLog->Log(_L("Default database opened and initialised")); 
    
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    // Setup global category info
    SetupGlobalCategoriesL();      
    
    iLog->Log(_L("Execute ResetGlobalCategoriesL() synchronously)"));
    
    // Rename some global categories     
    UpdateGlobalCategoriesInDb(); 
    
    iLog->Log(_L("Global category info updated in database (Renaming category done)")); 
    
    // Execute synchronously
    // Reset the global categories (the name and icon info of the global category will be reset)
    ExecuteAndDeleteLD(iCategoryManager->ResetGlobalCategoriesL());
    
    /*
     * Verification of 'reset' feature of global categories 
     * Check for global category with name 'Hotel' and it should not exist
     * Check for global category having name 'Accomodation'
     */         
    VerifyGlobalCategoriesL();
    
    iLog->Log(_L("Verification of CPosCategoryManager::ResetGlobalCategories() done)")); 
    
    
    iLog->Log(_L("Execute ResetGlobalCategoriesL() asynchronously)"));
    // Rename some global categories     
    UpdateGlobalCategoriesInDb();   
    
    iLog->Log(_L("Global category info updated in database (Renaming category done)"));  
    
    // Execute asynchronously
    RunAsyncOperationLD(iCategoryManager->ResetGlobalCategoriesL());
    VerifyGlobalCategoriesL();
    
    iLog->Log(_L("Verification of CPosCategoryManager::ResetGlobalCategories() done)")); 
    
    
    iLog->Log(_L("Execute ResetGlobalCategoriesL() Asynchronously using User::WaitForRequest())"));
    
    // Rename some global categories     
    UpdateGlobalCategoriesInDb();
    
    iLog->Log(_L("Global category info updated in database (Renaming category done)")); 
    
    // Execute asynchronously using User::WaitForRequest() 
    CPosLmOperation* op = iCategoryManager->ResetGlobalCategoriesL();
    CleanupStack::PushL(op);
    RunAsyncOperationByWaitForReqL(op);
    CleanupStack::PopAndDestroy(op);
    VerifyGlobalCategoriesL();
    
    iLog->Log(_L("Verification of CPosCategoryManager::ResetGlobalCategories() done)")); 
    
    iLog->Log(_L("Execute ResetGlobalCategoriesL() using Cancel immediately, after a while etc..."));
    // Rename some global categories     
    UpdateGlobalCategoriesInDb();    
    
    // Test cancel immediately
    RunAsyncOperationAndCancelLD(iCategoryManager->ResetGlobalCategoriesL());
    
    // Test cancel after a while
    RunAsyncOperationAndCancelInCallbackLD(iCategoryManager->ResetGlobalCategoriesL());
    
    ExecuteAndDeleteLD(iCategoryManager->ResetGlobalCategoriesL());
    VerifyGlobalCategoriesL();
    iLog->Log(_L("Verification of CPosCategoryManager::ResetGlobalCategories() done)"));               
	}


// ---------------------------------------------------------
// CPosTp20::UseGlobalCategoriesL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp20::UpdateGlobalCategoriesInDb()
    {
    /*
    * Change some properties of the existing global category (changing only the name here)
    */

    TBuf<150>info;
    // Get sample global category info     
    TPosLmItemId catId(0);
    TPosLmItemId catId1(0);
    CPosLandmarkCategory* category = NULL;
    
    for (TUint i = 0; i< iGlobalCategoryNames.Count(); i++)    
        {
        catId = iCategoryManager->GetGlobalCategoryL(iGlobalCategoryIds[i]);
        category = iCategoryManager->ReadCategoryLC( catId );
        category->SetCategoryNameL( iGlobalCategoryTestNames[i] );
        iCategoryManager->UpdateCategoryL( *category );    
        CleanupStack::PopAndDestroy( category );
        
        // Verify whether the added category exists with same global category id or not
        catId1 = iCategoryManager->GetCategoryL( iGlobalCategoryTestNames[i] );

        //Read the landmark from the database
        category = iCategoryManager->ReadCategoryLC( catId1 );

        // verify whether this is a global category or not (Accomodation)

        if ( category->GlobalCategory() != iGlobalCategoryIds[i]  ) 
            {// Error in category updation/verification        
            iLog->Log(_L("Error in global category verification"));
            info.Zero();
            info.Format(_L("Expected global category id %d"), iGlobalCategoryIds[i] );    
            info.Format(_L("Returned global category id %d"),  category->GlobalCategory() );    
            iLog->Log( info );        
            info.Zero();      
            User::Leave(-1);  
            }        
        CleanupStack::PopAndDestroy( category );            
        }
    }

// ---------------------------------------------------------
// CPosTp20::SetupGlobalCategoriesL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
void CPosTp20::SetupGlobalCategoriesL()
    {        
    // Testing with only 3 global categories     
    ReadGlobalCategoriesFromResourceL( iGlobalCategoryIds, iGlobalCategoryNames, 3 );
        
    iGlobalCategoryTestNames.Append(_L("Housing"));
    iGlobalCategoryTestNames.Append(_L("Movies"));
    iGlobalCategoryTestNames.Append(_L("Society"));    
    }




// ---------------------------------------------------------
// CPosTp20::VerifyGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp20::VerifyGlobalCategoriesL(TBool /*aCheck */)
    {       
    // Search database based on the new names of the global categories 
    TBuf<100> info;
    info.Zero();
    
    TPosLmItemId catId( 0 );
    
    for ( TUint i = 0; i < iGlobalCategoryNames.Count(); i++ )
        {
        // Search based on the Test names (with which the global category was renamed!)
        catId = iCategoryManager->GetCategoryL(iGlobalCategoryTestNames[i]);
        
        if ( catId != KPosLmNullItemId )
            {
            iLog->Log(_L("Error in global category reset functionality verification"));
            info.Zero();
            info.Format(_L("Expected global category id %d"), KPosLmNullItemId );    
            info.Format(_L("Returned global category id %d"),  catId );    
            iLog->Log( info );        
            info.Zero();      
            User::Leave(-1);                
            }
            
        // Search for old names of the global category 
        catId = iCategoryManager->GetCategoryL( *iGlobalCategoryNames[i] );
        CPosLandmarkCategory* category = iCategoryManager->ReadCategoryLC( catId );
        
        // Get the global category id 
        if ( category->GlobalCategory() )
            {
            if ( category->GlobalCategory() != iGlobalCategoryIds[i] )
                {
                iLog->Log(_L("Error in global category reset functionality verification"));
                info.Zero();
                info.Format(_L("Expected global category id %d"), iGlobalCategoryIds[i] );    
                info.Format(_L("Returned global category id %d"),  catId );    
                iLog->Log( info );        
                info.Zero();      
                User::Leave(-1);      
                }           
            }  
        CleanupStack::PopAndDestroy(category);                     
        }   
    }

// ---------------------------------------------------------
// CPosTp20::RenameAndDeleteGlobalL
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp20::RenameAndDeleteGlobalL()
    {
    const TPosLmGlobalCategory  KRemoveGlobalId1 = 1; 
    const TPosLmGlobalCategory  KRemoveGlobalId2 = 11;

    const TPosLmGlobalCategory  KRenameGlobalId1 = 3;
    const TPosLmGlobalCategory  KRenameGlobalId2 = 12;
    
    TBuf<150> buf;
    ExecuteAndDeleteLD(iDatabase->CompactL());
    TPosLmItemId id = iCategoryManager->GetGlobalCategoryL(KRemoveGlobalId1);
    HBufC* name = iCategoryManager->GlobalCategoryNameL(KRemoveGlobalId1);
    CleanupStack::PushL(name);
    buf.Format(_L("Removing global category %S"), name);
    iLog->Log(buf);
    CleanupStack::PopAndDestroy(name);
    CPosLmOperation* op = iCategoryManager->RemoveCategoryL(id);
    ExecuteAndDeleteLD(op);
    
    id = iCategoryManager->GetGlobalCategoryL(KRemoveGlobalId2);
    name = iCategoryManager->GlobalCategoryNameL(KRemoveGlobalId2);
    CleanupStack::PushL(name);
    buf.Zero();
    buf.Format(_L("Removing global category %S"), name);
    iLog->Log(buf);
    CleanupStack::PopAndDestroy(name);
    op = iCategoryManager->RemoveCategoryL(id);
    ExecuteAndDeleteLD(op);
    
    id = iCategoryManager->GetGlobalCategoryL(KRenameGlobalId1);
    
    name = iCategoryManager->GlobalCategoryNameL(KRenameGlobalId1);
    CleanupStack::PushL(name);
    buf.Format(_L("Renaming global category %S to Jada jada"), name);
    iLog->Log(buf);
    CleanupStack::PopAndDestroy(name);
    
    CPosLandmarkCategory* cm = iCategoryManager->ReadCategoryLC(id);
    cm->SetCategoryNameL(_L("Jada jada"));
    iCategoryManager->UpdateCategoryL(*cm);
    CleanupStack::PopAndDestroy(cm);
    ExecuteAndDeleteLD(iDatabase->CompactL());
    
    id = iCategoryManager->GetGlobalCategoryL(KRenameGlobalId2);
    name = iCategoryManager->GlobalCategoryNameL(KRenameGlobalId2);
    CleanupStack::PushL(name);
    buf.Format(_L("Renaming global category %S to asdfasdas"), name);
    iLog->Log(buf);
    CleanupStack::PopAndDestroy(name);
    cm = iCategoryManager->ReadCategoryLC(id);
    cm->SetCategoryNameL(_L("asdfasdas"));
    iCategoryManager->UpdateCategoryL(*cm);
    CleanupStack::PopAndDestroy(cm);
    }

//  End of File
