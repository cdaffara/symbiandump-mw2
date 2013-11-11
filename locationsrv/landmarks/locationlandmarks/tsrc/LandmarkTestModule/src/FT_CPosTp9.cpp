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

#include "FT_CPosTp9.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <EPos_CPosLandmark.h>
#include <EPos_TPosLMSortPref.h> 
#include <e32svr.h> // RDebug

// CONSTANTS
const TInt KNoLandmarks = 50;
_LIT(KIteratorNumOfItemsErr, "The iterator returns incorrect number of items");
_LIT(KIteratorIdErr, "A landmarks id not in the database returned from the iterator");         

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp9::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::StartL()
    {

    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();
    
    iDatabase = CPosLandmarkDatabase::OpenL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
    ExecuteAndDeleteLD(iDatabase->CompactL());
    AddLandmarksL();
    IterateLandmarksL();
	IterateSetOfLandmarksL();
    IterateSortedLandmarksL();
    }

// ---------------------------------------------------------
// CPosTp9::CloseTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::CloseTest()
    {
    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CPosTp9::AddLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::AddLandmarksL()
    {
    iLog->Log(_L("AddLandmarksL"));
    _LIT(KLmName, "LmTP9 - %d");
    _LIT(KLmDescription, "LmTP9Description - %d");
    
    for (TInt i=0; i<(KNoLandmarks); i++)
        {
        TBuf<100> lmName;
        lmName.Format(KLmName,i);
        TBuf<100> lmDesc;
        lmDesc.Format(KLmDescription,i);
       
        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(lmName); 
        landmark->SetLandmarkDescriptionL(lmDesc);
        TInt err = KErrLocked;
        while (err == KErrLocked)
            {
            TRAP(err, iDatabase->AddLandmarkL(*landmark));
            }
        
        iLandmarks.Append(landmark);
       
        CleanupStack::Pop(landmark); 
        }
    }

// ---------------------------------------------------------
// CPosTp9::IterateLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::IterateLandmarksL()
    {
     iLog->Log(_L("IterateLandmarksL"));
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    TPosLmItemId id = iter->NextL();
    while (id != KPosLmNullItemId)
        {
        CPosLandmark* source = InternalGet(id);
        AssertTrueSecL(source != NULL, KIteratorIdErr);
        
        CPosLandmark* target = iDatabase->ReadLandmarkLC(id);
        CompareLandmarksL(*source, *target);
        CleanupStack::PopAndDestroy(); // target

        id = iter->NextL();
        }

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)iLandmarks.Count(), KIteratorNumOfItemsErr);
    
    iter->Reset();
    id = iter->NextL();
    while (id != KPosLmNullItemId)
        {
        CPosLandmark* source = InternalGet(id);
        AssertTrueSecL(source != NULL, KIteratorIdErr);
        
        CPosLandmark* target = iDatabase->ReadLandmarkLC(id);
        CompareLandmarksL(*source, *target);
        CleanupStack::PopAndDestroy(); // target

        id = iter->NextL();
        }

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)iLandmarks.Count(), KIteratorNumOfItemsErr);

    CleanupStack::PopAndDestroy(iter);
    }

// ---------------------------------------------------------
// CPosTp9::IterateSetOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::IterateSetOfLandmarksL()
    {
    iLog->Log(_L("IterateSetOfLandmarksL"));
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    const TInt startIndex=5;
    const TInt numOfItems=20;

    RArray<TPosLmItemId> ids;
    CleanupClosePushL(ids);

    iter->GetItemIdsL(ids, startIndex, numOfItems);
    
    AssertTrueSecL(ids.Count() == numOfItems, KIteratorNumOfItemsErr);

    for (TInt i=0; i<numOfItems; i++)
        {
        CPosLandmark* source = InternalGet(ids[i]);
        AssertTrueSecL(source != NULL, KIteratorIdErr);
        
        CPosLandmark* target = iDatabase->ReadLandmarkLC(ids[i]);
        CompareLandmarksL(*source, *target);
        CleanupStack::PopAndDestroy(); // target
        }

    CleanupStack::PopAndDestroy(2, iter);
    }

// ---------------------------------------------------------
// CPosTp9::InternalSort
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt InternalSortL(const CPosLandmark& aSource, const CPosLandmark& aTarget)
    {
    TPtrC sourceName, targetName;
    
    User::LeaveIfError(aSource.GetLandmarkName(sourceName));  
    User::LeaveIfError(aTarget.GetLandmarkName(targetName));
    
    return sourceName.Compare(targetName);
    }
    
// ---------------------------------------------------------
// CPosTp9::IterateLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::IterateSortedLandmarksL()
    {  
    iLog->Log(_L("IterateSortedLandmarksL")); 
    CheckIteratorArgL(CPosLandmark::EPosition);
    CheckIteratorArgL(CPosLandmark::ECoverageRadius);
    CheckIteratorArgL(CPosLandmark::ECategoryInfo);
    CheckIteratorArgL(CPosLandmark::EIcon);
    CheckIteratorArgL(CPosLandmark::EDescription);
    
    TLinearOrder<CPosLandmark> order(InternalSortL);
    iLandmarks.Sort(order);

    TPosLmSortPref sortDesc(CPosLandmark::ELandmarkName, TPosLmSortPref::EDescending);
    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL(sortDesc);
    CleanupStack::PushL(iter);

    AssertTrueSecL(iter->NumOfItemsL() == (TUint)iLandmarks.Count(), KIteratorNumOfItemsErr);

    TInt lastIndex=iLandmarks.Count()-1;
    for (TInt i=lastIndex; i>=0; i--)
        {
        CPosLandmark* target = iDatabase->ReadLandmarkLC(iter->NextL());
       
        CompareLandmarksL(*iLandmarks[i], *target);
        CleanupStack::PopAndDestroy(); // target
        }
    CleanupStack::PopAndDestroy(iter);

    TPosLmSortPref sortAsc(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    iter = iDatabase->LandmarkIteratorL(sortAsc);
    CleanupStack::PushL(iter);

    for (TInt j=0; j < iLandmarks.Count(); j++)
        {
        CPosLandmark* target = iDatabase->ReadLandmarkLC(iter->NextL());
       
        CompareLandmarksL(*iLandmarks[j], *target);
        CleanupStack::PopAndDestroy(); // target
        }

    CleanupStack::PopAndDestroy(iter);
    iLog->Log(_L("IterateSortedLandmarksL completed"));
    }

// ---------------------------------------------------------
// CPosTp9::
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp9::CheckIteratorArgL(const CPosLandmark::TAttributes& aAttr)
    {
    _LIT(KSortByNameErr, "Only sorting by landmark name should be supported");
    iLog->Log(_L("CheckIteratorArgL")); 
    TPosLmSortPref sortPref(aAttr);
    CPosLmItemIterator* iter = NULL;
    TRAPD(err, iter = iDatabase->LandmarkIteratorL(sortPref));
    delete iter;
    AssertTrueSecL(err==KErrNotSupported, KSortByNameErr);
    }

// ---------------------------------------------------------
// CPosTp9::InternalGet
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmark* CPosTp9::InternalGet(TPosLmItemId id)
    {
    for (TInt i=0; i<iLandmarks.Count(); i++)
        {   
        if (iLandmarks[i]->LandmarkId() == id)
            {
            return iLandmarks[i];
            }
        }
  
    return NULL;
    }

//  End of File
