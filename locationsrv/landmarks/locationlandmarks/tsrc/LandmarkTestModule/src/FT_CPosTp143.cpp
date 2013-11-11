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


#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLandmarkDatabaseExtended.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <e32svr.h> // RDebug
#include "FT_CPosTp143.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp143::StartL()
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
    
    CheckCountersL();
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp143::CloseTest()
    {
    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp143::AddLandmarksL()
    {
    iLog->Log(_L("AddLandmarksL"));
    _LIT(KLmName, "LmTP143 - %d");
    _LIT(KLmDescription, "LmTP143Description - %d");
    
    const TInt KNoLandmarks = 1000;
    for (TInt i=0; i< KNoLandmarks ; i++)
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
// ---------------------------------------------------------
//
void CPosTp143::CheckCountersL()
    {
    iLog->Log(_L("CheckCountersL"));

    TTime start, end;
    CPosLandmarkDatabaseExtended* dbExt = CPosLandmarkDatabaseExtended::OpenL();
    CleanupStack::PushL(dbExt);
    
    iLog->Log(_L("checking LandmarksCount"));
    start.UniversalTime();
    TInt lmCount = dbExt->LandmarksCount();
    end.UniversalTime();
    TTimeIntervalMicroSeconds interval = end.MicroSecondsFrom( start );
    iLog->Log(_L("LandmarksCount done in %ld us"), interval.Int64());
    
    if ( lmCount != iLandmarks.Count() )
        {
        iLog->Log( _L("LandmarksCount wrong result, expected %d, actual %d"),
            iLandmarks.Count(), lmCount );
        User::Leave( KErrGeneral );
        }

    CPosLmItemIterator* iter = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iter);

    if ( lmCount != iter->NumOfItemsL() )
        {
        iLog->Log( _L("CategoriesCount wrong result, expected %d, actual %d"),
            iter->NumOfItemsL(), lmCount );
        User::Leave( KErrGeneral );
        }

    CleanupStack::PopAndDestroy( iter );

    iLog->Log(_L("checking CategoriesCount"));
    start.UniversalTime();
    TInt catCount = dbExt->CategoriesCount();
    end.UniversalTime();
    interval = end.MicroSecondsFrom( start );
    iLog->Log(_L("CategoriesCount done in %ld us"), interval.Int64());
    
    CPosLmCategoryManager& catman = dbExt->CategoryManager();
    
    CPosLmItemIterator* catIter = catman.CategoryIteratorL();
    CleanupStack::PushL(catIter);
    
    if ( catCount != catIter->NumOfItemsL() )
        {
        iLog->Log( _L("CategoriesCount wrong result, expected %d, actual %d"),
            catIter->NumOfItemsL(), catCount );
        User::Leave( KErrGeneral );
        }

    CleanupStack::PopAndDestroy( catIter );
    CleanupStack::PopAndDestroy( dbExt );
    }

