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

#include "FT_CPosTp41.h"
#include <EPos_CPosLandmarkDatabase.h> 
#include <EPos_CPosLMItemIterator.h> 
#include <EPos_CPosLandmark.h>
#include <EPos_TPosLMSortPref.h> 

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp41::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp41::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp41 - Empty Landmark Name and Descriptor");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp41::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp41::StartL()
    {
    RemoveDefaultDbL();
    iDatabase = CPosLandmarkDatabase::OpenL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL()); 
       }
    ExecuteAndDeleteLD(iDatabase->CompactL()); 

    VerifyEmptyLandmarksL();
    }

// ---------------------------------------------------------
// CPosTp41::CloseTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp41::CloseTest()
    {
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CPosTp41::VerifyEmptyLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp41::VerifyEmptyLandmarksL()
    {
    _LIT(KLmName, "LmTp41 - %d");
    _LIT(KLmDescription, "LmTp41Description - %d");

    _LIT(KCompareNameErr, "Landmark names are not equal");
    _LIT(KCompareDescErr, "Landmark descriptions are not equal");
    
    _LIT(KNameErr, "GetLandmarkName returned wrong");
    _LIT(KDescErr, "GetLandmarkDescription returned wrong");
    
    // 1) Create landmark with name and descriptor
    TBuf<100> lmName;
    lmName.Format(KLmName,1);
    TBuf<100> lmDesc;
    lmDesc.Format(KLmDescription,1);
    CPosLandmark* landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(lmName); 
    landmark->SetLandmarkDescriptionL(lmDesc);
    TPosLmItemId id = iDatabase->AddLandmarkL(*landmark);
    User::After(200000);

    // Read landmark from local landmark
    TPtrC sourceDesc;
    TPtrC sourceName;
    TInt sourceErr = landmark->GetLandmarkName(sourceName);
    AssertTrueSecL(sourceErr == KErrNone, KNameErr);
    sourceErr = landmark->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNone, KDescErr);

    AssertTrueSecL(sourceName.Compare(lmName) == KErrNone, KCompareNameErr);
    AssertTrueSecL(sourceDesc.Compare(lmDesc) == KErrNone, KCompareDescErr);

    CleanupStack::PopAndDestroy(landmark);
    // Read landmark from landmark db
    CheckLandmarkL(id, lmName, lmDesc);
    lmName.Zero();
    lmDesc.Zero();

    // 2) Create landmark with name and and empty descriptor
    lmName.Format(KLmName,2);
    lmDesc.Append(_L(""));
    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(lmName); 
    landmark->SetLandmarkDescriptionL(lmDesc); 
    id = iDatabase->AddLandmarkL(*landmark);
    User::After(200000);

    // Read landmark from local landmark
    sourceErr = landmark->GetLandmarkName(sourceName);   
    AssertTrueSecL(sourceErr == KErrNone, KNameErr);
    sourceErr = landmark->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNone, KDescErr);

    AssertTrueSecL(sourceName.Compare(lmName) == KErrNone, KCompareNameErr);
    AssertTrueSecL(sourceDesc.Compare(lmDesc) == KErrNone, KCompareDescErr);

    CleanupStack::PopAndDestroy(landmark);
    // Read landmark from landmark db
    CheckLandmarkL(id, lmName, lmDesc);
    lmName.Zero();
    lmDesc.Zero();

    // 3) Create landmark with name and and no descriptor set
    lmName.Format(KLmName, 2);
    lmDesc.Append(_L(""));
    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(lmName); 
    id = iDatabase->AddLandmarkL(*landmark);
    User::After(200000);

    // Read landmark from local landmark, GetLandmarkDescription should return KErrNotFound since it is not set
    sourceErr = landmark->GetLandmarkName(sourceName);   
    AssertTrueSecL(sourceErr == KErrNone, KNameErr);
    sourceErr = landmark->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNotFound, KDescErr);

    AssertTrueSecL(sourceName.Compare(lmName) == KErrNone, KCompareNameErr);

    CleanupStack::PopAndDestroy(landmark);
    // Read landmark from landmark db
    CheckLandmarkL(id, lmName, lmDesc);
    lmName.Zero();
    lmDesc.Zero();

    // 4) Create landmark with empty name and no description set
    lmName.Append(_L(""));
    lmDesc.Append(_L(""));
    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(lmName); 
    id = iDatabase->AddLandmarkL(*landmark);
    User::After(200000);

    // Read landmark from local landmark, GetLandmarkDescription
    // should return KErrNotFound since it is not set
    sourceErr = landmark->GetLandmarkName(sourceName);   
    AssertTrueSecL(sourceErr == KErrNone, KNameErr);
    sourceErr = landmark->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNotFound, KDescErr);

    AssertTrueSecL(sourceName.Compare(lmName) == KErrNone, KCompareNameErr);

    CleanupStack::PopAndDestroy(landmark);
    // Read landmark from landmark db
    CheckLandmarkL(id, lmName, lmDesc);
    lmName.Zero();
    lmDesc.Zero();

    // 5) Create landmark with no name set but with description
    lmName.Append(_L(""));
    lmDesc.Format(KLmDescription,2);
    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkDescriptionL(lmDesc); 
    id = iDatabase->AddLandmarkL(*landmark);
    User::After(200000);

    // Read landmark from local landmark, GetLandmarkName
    // should return KErrNotFound since it is not set
    sourceErr = landmark->GetLandmarkName(sourceName);   
    AssertTrueSecL(sourceErr == KErrNotFound, KNameErr);
    sourceErr = landmark->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNone, KDescErr);
    AssertTrueSecL(sourceDesc.Compare(lmDesc) == KErrNone, KCompareDescErr);

    CleanupStack::PopAndDestroy(landmark);
    // Read landmark from landmark db
    CheckLandmarkL(id, lmName, lmDesc);
    lmName.Zero();
    lmDesc.Zero();

    // 6) Create empty landmark
    landmark = CPosLandmark::NewLC();
    id = iDatabase->AddLandmarkL(*landmark);
    User::After(200000);
    lmName.Append(_L(""));
    lmDesc.Append(_L(""));

    // Read landmark from local landmark, GetLandmarkName and GetLandmarkDescription
    // should return KErrNotFound since it is not set
    sourceErr = landmark->GetLandmarkName(sourceName);   
    AssertTrueSecL(sourceErr == KErrNotFound, KNameErr);
    sourceErr = landmark->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNotFound, KDescErr);

    CleanupStack::PopAndDestroy(landmark);
    // Read landmark from landmark db
    CheckLandmarkL(id, lmName, lmDesc);
    lmName.Zero();
    lmDesc.Zero();
    }

// ---------------------------------------------------------
// CPosTp41::InternalGet
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp41::CheckLandmarkL(TPosLmItemId aId, TDesC& aName, TDesC& aDesc)
    {
    iLog->Put(_L("CheckLandmarkL"));
    _LIT(KNameErr2, "lm->GetLandmarkName returned wrong");
    _LIT(KDescErr2, "lm->GetLandmarkDescription returned wrong");

    _LIT(KCompareNameErr, "Landmark names are not equal");
    _LIT(KCompareDescErr, "Landmark descriptions are not equal");

    TPtrC sourceDesc;
    TPtrC sourceName;

    CPosLandmark* lm = iDatabase->ReadLandmarkLC(aId);
    
    TInt sourceErr = lm->GetLandmarkName(sourceName);
    AssertTrueSecL(sourceErr == KErrNone, KNameErr2);
    
    AssertTrueSecL(sourceName.Compare(aName) == KErrNone, KCompareNameErr);
    sourceErr = lm->GetLandmarkDescription(sourceDesc);
    AssertTrueSecL(sourceErr == KErrNone, KDescErr2);
    AssertTrueSecL(sourceDesc.Compare(aDesc) == KErrNone, KCompareDescErr);

    CleanupStack::PopAndDestroy(lm);
    }

//  End of File
