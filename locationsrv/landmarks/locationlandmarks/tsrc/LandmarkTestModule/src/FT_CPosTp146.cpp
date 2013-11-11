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

#include "FT_CPosTp146.h"
#include <e32math.h>
#include <LbsPosition.h>

#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_PosLandmarkSerialization.h>
#include <EPos_PosLmCategorySerialization.h>
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp146::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp146::StartL()
    {  
    
  		CheckLmCatSerializationInvalidL();
  		
	}

// ---------------------------------------------------------
// CPosTp146::CreateDummyLandmarkLC
//
// ---------------------------------------------------------

CPosLandmark* CPosTp146::CreateDummyLandmarkLC()
    {
    CPosLandmark* lm = CPosLandmark::NewLC();

    _LIT(KName, "Name");
    _LIT(KDesc, "Description");
    _LIT(KIcon, "IconFile");
    const TInt KIconIndex = 1, KIconMaskIndex = 2;

    const TPosLmItemId KCat[] = { 3, 4, 5 };
    const TInt KCats = sizeof KCat / sizeof(TPosLmItemId);

    _LIT(KField1Val, "field1");
    _LIT(KField2Val, "field11");
    const TPositionFieldId KField[] =
        {
        EPositionFieldComment,
        EPositionFieldCountry
        };
    const TInt KFields = sizeof KField / sizeof(TPositionFieldId);
    const TPtrC KFieldData[] =
        {
        KField1Val(), KField2Val()
        };

    const TReal32 KRadius = 0.12;
    const TReal32 KHacc = 0.23, KVacc = 0.34;
    const TReal KLat = 12, KLon = -23, KAlt = 34;

    lm->SetLandmarkNameL(KName);
    lm->SetLandmarkDescriptionL(KDesc);
    lm->SetIconL(KIcon, KIconIndex, KIconMaskIndex);

    const TLocality KLoc(TCoordinate(KLat,KLon,KAlt), KHacc, KVacc);
    lm->SetPositionL(KLoc);
    lm->SetCoverageRadius(KRadius);

    for (TInt c = 0; c < KCats; c++)
        {
        lm->AddCategoryL(KCat[c]);
        }

    for (TInt f = 0; f < KFields; f++)
        {
        lm->SetPositionFieldL(KField[f], KFieldData[f]);
        }

    return lm;
    }
 
// ---------------------------------------------------------
// CPosTp146::CreateDummyCategoryLC
//
// ---------------------------------------------------------
 
 
CPosLandmarkCategory* CPosTp146::CreateDummyCategoryLC()
    {
    CPosLandmarkCategory* cat = CPosLandmarkCategory::NewLC();

    TInt KCatId = cat->CategoryId();
    _LIT(KName, "Name");
    _LIT(KIcon, "IconFile");
    const TInt KIconIndex = 1, KIconMaskIndex = 2;
    cat->SetCategoryNameL(KName);
    cat->SetIconL(KIcon, KIconIndex, KIconMaskIndex);
    return cat;
    }

// ---------------------------------------------------------
// CPosTp146::CheckLmCatSerializationInvalidL
//
// ---------------------------------------------------------

void CPosTp146::CheckLmCatSerializationInvalidL()
    {
    DECLARE_ERROR_LOGGING
    

    TInt err = KErrNone;
    HBufC8* buf1 = NULL;
    HBufC8* buf2 = NULL;

    CPosLandmark* lm = CreateDummyLandmarkLC();
    TRAP(err, buf1 = PosLandmarkSerialization::PackL(*lm));
    CleanupStack::PopAndDestroy(lm);
    lm = NULL;

    IF_ERROR(err, "Packing failed");
    User::LeaveIfError(err);
    CleanupStack::PushL(buf1);

    CPosLandmarkCategory* cat = CreateDummyCategoryLC();
    TRAP(err, buf2 = PosLmCategorySerialization::PackL(*cat));
    CleanupStack::PopAndDestroy(cat);
    cat = NULL;

    IF_ERROR(err, "Packing failed");
    User::LeaveIfError(err);
    CleanupStack::PushL(buf2);

    TRAP(err, cat = PosLmCategorySerialization::UnpackL(*buf1));
    delete cat;

    ERROR(err == KErrNone, "Wrong error (lm->cat)");

    TRAP(err, lm = PosLandmarkSerialization::UnpackL(*buf2));
    delete lm;

    ERROR(err == KErrNone, "Wrong error (cat->lm)");

    CleanupStack::PopAndDestroy(buf2);
    CleanupStack::PopAndDestroy(buf1);

    
    LEAVE_IF_ERRORS()
    }

 
    
//  End of File
