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

#include "FT_CPosTp147.h"
#include <e32math.h>
#include <LbsPosition.h>

#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_PosLandmarkSerialization.h>
#include <EPos_PosLmCategorySerialization.h>
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp147::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp147::StartL()
    {  
    
  		CheckSerializationMemoryL();
  		
	}

// ---------------------------------------------------------
// CPosTp147::CreateDummyLandmarkLC
//
// ---------------------------------------------------------

CPosLandmark* CPosTp147::CreateDummyLandmarkLC()
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
// CPosTp147::CreateDummyCategoryLC
//
// ---------------------------------------------------------
 
 
CPosLandmarkCategory* CPosTp147::CreateDummyCategoryLC()
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
// CPosTp147::CheckSerializationMemoryL
//
// ---------------------------------------------------------

void CPosTp147::CheckSerializationMemoryL()
    {
    CheckLmSerialization_MemoryL();
    CheckLmCatSerialization_MemoryL();
    }

// ---------------------------------------------------------
// CPosTp147::CheckLmSerialization_MemoryL
//
// ---------------------------------------------------------

void CPosTp147::CheckLmSerialization_MemoryL()
    {
    DECLARE_ERROR_LOGGING;

    LOG("Packing and checking uheap");

    CPosLandmark* lm = CreateDummyLandmarkLC();

    __UHEAP_MARK;
    HBufC8* buf = PosLandmarkSerialization::PackL(*lm);
    delete buf;
    __UHEAP_MARKEND;
    
    LOG("Unpacking and checking uheap");

    buf = PosLandmarkSerialization::PackL(*lm);
    CleanupStack::PopAndDestroy(lm);
    CleanupStack::PushL(buf);

    __UHEAP_MARK;
    lm = PosLandmarkSerialization::UnpackL(*buf);
    delete lm;
    __UHEAP_MARKEND;

    LOG("Done");
    CleanupStack::PopAndDestroy(buf);
    }

// ---------------------------------------------------------
// CPosTp147::CheckLmCatSerialization_MemoryL
//
// ---------------------------------------------------------

void CPosTp147::CheckLmCatSerialization_MemoryL()
    {
    DECLARE_ERROR_LOGGING;

    LOG("Packing and checking uheap");

    CPosLandmarkCategory* cat = CreateDummyCategoryLC();

    __UHEAP_MARK;
    HBufC8* buf = PosLmCategorySerialization::PackL(*cat);
    delete buf;
    __UHEAP_MARKEND;
    
    LOG("Unpacking and checking uheap");

    buf = PosLmCategorySerialization::PackL(*cat);
    CleanupStack::PopAndDestroy(cat);
    CleanupStack::PushL(buf);

    __UHEAP_MARK;
    cat = PosLmCategorySerialization::UnpackL(*buf);
    delete cat;
    __UHEAP_MARKEND;

    LOG("Done");
    CleanupStack::PopAndDestroy(buf);
    }
    


 
    
//  End of File
