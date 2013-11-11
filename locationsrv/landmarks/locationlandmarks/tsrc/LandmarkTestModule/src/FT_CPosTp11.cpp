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

#include "FT_CPosTp11.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <EPos_CPosLandmark.h>
#include <EPos_TPosLMSortPref.h> 

//CONSTANTS
const TInt KNoCategoriesTp11  = 100;
const TInt KNoLandmarksTp11 = 100;     

_LIT(KCategoryIteratorNumOfItemsErr, "The iterator returns incorrect number of items");
_LIT(KCategoryIteratorIdErr, "A category id not in the database returned from the iterator");         

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp11::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::StartL()
    {  
    iLog->Log(_L("***** Testing CPosCategoryManager::CategoryIteratorL() and CPosCategoryManager::ReferencedCategoryIteratorL()***** "));
    
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();              
    
    iDatabase = CPosLandmarkDatabase::OpenL();        
    if (iDatabase->IsInitializingNeeded())
       {
        iLog->Log(_L("ExecuteAndDeleteLD(iDatabase->InitializeL())"));
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       } 
       
    iLog->Log(_L("Default database opened and initialized "));       
               
    iCategoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    ExecuteAndDeleteLD(iDatabase->CompactL());          
    
    MakeEmptyLmDatabaseL( *iDatabase, *iCategoryManager);        
    
    iLog->Log(_L("Deleting all landmarks and categories to create empty database Done"));
    
    AddCategoriesL();
    AddLandmarksL();   

    iLog->Log(_L("** 1: Testing CPosCategoryManager::CategoryIteratorL() (Iteration of categories)"));
    IterateCategoriesL();
    
    iLog->Log(_L("** 2: Testing CPosCategoryManager::ReferencedCategoryIteratorL() (Iteration of referenced categories)"));   
    
    IterateReferencedCategoriesL();

    iLog->Log(_L("** 3: Testing CPosCategoryManager::CategoryIteratorL() (Iteration of categories)"));    
    IterateSetOfCategoriesL();
	
	iLog->Log(_L("** 4: Testing CPosCategoryManager::CategoryIteratorL() (Iteration of Sorted categories)"));    
    IterateSortedCategoriesL();    
    
    iLog->Log(_L("** 5: Testing CPosCategoryManager::ReferencedCategoryIteratorL() (Iteration of Sorted referenced categories)"));    
    IterateReferencedSortedCategoriesL();    

    delete iDatabase;
    iDatabase = NULL;

    // Remove lm db
    RemoveDefaultDbL();    
    }

// ---------------------------------------------------------
// CPosTp11::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::CloseTest()
    {
    iLog->Log(_L("CloseTest"));
    iCategories.ResetAndDestroy();
    iCategories.Close();
    
    iReferencedCategories.Close();
    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();
    
    delete iDatabase;
    iDatabase = NULL;
    
    delete iCategoryManager; 
    iCategoryManager = NULL;      
    }
    

// ---------------------------------------------------------
// CPosTp11::AddCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::AddCategoriesL()
    {    
    _LIT(KName, "CategoryTP11 - %d");
    TInt err=KErrLocked;
    for (TInt i=0; i<KNoCategoriesTp11; i++)
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
        CleanupStack::Pop(lmc);
        }    
    iLog->Log(_L("Adding categories to database done "));
    }

// ---------------------------------------------------------
// CPosTp11::AddLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::AddLandmarksL()
    {
    _LIT(KLmName, "LmTP11 - %d");
    TInt err = KErrLocked;
    for (TInt i=0; i<KNoLandmarksTp11; i++)
        {
        TBuf<100> lmName;
        lmName.Format(KLmName,i);
       
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(lmName); 

        if (i % 2)
            {
            landmark->AddCategoryL(iCategories[i]->CategoryId());
            iReferencedCategories.Append(*iCategories[i]);
            }

        err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, iDatabase->AddLandmarkL(*landmark));
            }
        iLandmarks.Append(landmark);
        CleanupStack::Pop(landmark);
        }
    iLog->Log(_L("Adding categories to landmarks and adding landmarks to database done "));
    }


// ---------------------------------------------------------
// CPosTp11::IterateCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::IterateCategoriesL()
    {
    CPosLmItemIterator* iter = iCategoryManager->CategoryIteratorL();
    CleanupStack::PushL(iter);

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)iCategories.Count(), KCategoryIteratorNumOfItemsErr);

    CheckIteratorItemsL(*iter);
 
    iter->Reset();

    CheckIteratorItemsL(*iter);

    CleanupStack::PopAndDestroy(iter);
    }

// ---------------------------------------------------------
// CPosTp11::IterateReferencedCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::IterateReferencedCategoriesL()
    {
    CPosLmItemIterator* iter = iCategoryManager->ReferencedCategoryIteratorL();
    CleanupStack::PushL(iter);

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)(iCategories.Count()/2), KCategoryIteratorNumOfItemsErr);
    
    CheckIteratorItemsL(*iter);
 
    iter->Reset();

    CheckIteratorItemsL(*iter);
    
    CleanupStack::PopAndDestroy(iter);
    }

// ---------------------------------------------------------
// CPosTp11::CheckIteratorItemsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::CheckIteratorItemsL(CPosLmItemIterator& iter)
    {
    TPosLmItemId id = iter.NextL();
    while (id != KPosLmNullItemId)
        {
        CPosLandmarkCategory* source = InternalGet(id);
        AssertTrueSecL(source != NULL, KCategoryIteratorIdErr);
        CPosLandmarkCategory* target = iCategoryManager->ReadCategoryLC(id);
        CompareCategoriesL(*source, *target);
        CleanupStack::PopAndDestroy(target); 
        id = iter.NextL();
        }    
    }

// ---------------------------------------------------------
// CPosTp11::IterateSetOfCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::IterateSetOfCategoriesL()
    {
    CPosLmItemIterator* iter = iCategoryManager->CategoryIteratorL();
    CleanupStack::PushL(iter);

    const TInt startIndex=11;
    const TInt numOfItems=34;

    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    iter->GetItemIdsL(ids, startIndex, numOfItems);
    
    AssertTrueSecL(ids.Count() == numOfItems, KCategoryIteratorNumOfItemsErr);

    for (TInt i=0; i<numOfItems; i++)
        {
        CPosLandmarkCategory* source = InternalGet(ids[i]);
        AssertTrueSecL(source != NULL, KCategoryIteratorIdErr);
        
        CPosLandmarkCategory* target = iCategoryManager->ReadCategoryLC(ids[i]);
        CompareCategoriesL(*source, *target);
        CleanupStack::PopAndDestroy(target); 
        }

    CleanupStack::PopAndDestroy(2, iter);

    }

// ---------------------------------------------------------
// InternalSort
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt InternalSortL(const CPosLandmarkCategory& aSource, const CPosLandmarkCategory& aTarget)
    {
    TPtrC sourceName, targetName;
    
    User::LeaveIfError(aSource.GetCategoryName(sourceName));  
    User::LeaveIfError(aTarget.GetCategoryName(targetName));
    
    return sourceName.Compare(targetName);
    }

// ---------------------------------------------------------
// CPosTp11::IterateSortedCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::IterateSortedCategoriesL()
    {
    
    TLinearOrder<CPosLandmarkCategory> order(InternalSortL);
    iCategories.Sort(order);

    CPosLmItemIterator* iter = iCategoryManager->CategoryIteratorL(
                                CPosLmCategoryManager::ECategorySortOrderNameDescending);
    CleanupStack::PushL(iter);

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)iCategories.Count(), KCategoryIteratorNumOfItemsErr);

    TInt lastIndex=iCategories.Count()-1;
    for (TInt i=lastIndex; i>=0; i--)
        {
        CPosLandmarkCategory* target = iCategoryManager->ReadCategoryLC(iter->NextL());
       
        CompareCategoriesL(*iCategories[i], *target);
        CleanupStack::PopAndDestroy(target); 
        }
    CleanupStack::PopAndDestroy(iter);

    iter = iCategoryManager->CategoryIteratorL(
                                CPosLmCategoryManager::ECategorySortOrderNameAscending);
    CleanupStack::PushL(iter);

    for (TInt j=0; j < iCategories.Count(); j++)
        {
        CPosLandmarkCategory* target = iCategoryManager->ReadCategoryLC(iter->NextL());
       
        CompareCategoriesL(*iCategories[j], *target);
        CleanupStack::PopAndDestroy(target); 
        }

    CleanupStack::PopAndDestroy(iter);
    
    }

// ---------------------------------------------------------
// CPosTp11::IterateReferencedSortedCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp11::IterateReferencedSortedCategoriesL()
    {
    TLinearOrder<CPosLandmarkCategory> order(InternalSortL);

    iReferencedCategories.Sort(order);

    CPosLmItemIterator* iter = iCategoryManager->ReferencedCategoryIteratorL(
                                CPosLmCategoryManager::ECategorySortOrderNameDescending);
    CleanupStack::PushL(iter);

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)(iCategories.Count()/2), KCategoryIteratorNumOfItemsErr);

    TInt lastIndex=iReferencedCategories.Count()-1;
    for (TInt i=lastIndex; i>=0; i--)
        {
        CPosLandmarkCategory* target = iCategoryManager->ReadCategoryLC(iter->NextL());
       
        CompareCategoriesL(iReferencedCategories[i], *target);
        CleanupStack::PopAndDestroy(target); 
        }

    CleanupStack::PopAndDestroy(iter);

    iter = iCategoryManager->ReferencedCategoryIteratorL(
                                CPosLmCategoryManager::ECategorySortOrderNameAscending);
    CleanupStack::PushL(iter);

    for (TInt j=0; j < iReferencedCategories.Count(); j++)
        {
        CPosLandmarkCategory* target = iCategoryManager->ReadCategoryLC(iter->NextL());
       
        CompareCategoriesL(iReferencedCategories[j], *target);
        CleanupStack::PopAndDestroy(target); 
        }

    CleanupStack::PopAndDestroy(iter);
    }

// ---------------------------------------------------------
// CPosT11::InternalGet
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkCategory* CPosTp11::InternalGet(TPosLmItemId id)
    {
    for (TInt i=0; i<iCategories.Count(); i++)
        {   
        if (iCategories[i]->CategoryId() == id)
            {
            return iCategories[i];
            }
        }
  
    return NULL;
    }
                                 
// ---------------------------------------------------------
// CPosT11::CreateEmptyLmDatabaseL() 
// ---------------------------------------------------------
//
void CPosTp11::MakeEmptyLmDatabaseL( CPosLandmarkDatabase& aLdb,
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
