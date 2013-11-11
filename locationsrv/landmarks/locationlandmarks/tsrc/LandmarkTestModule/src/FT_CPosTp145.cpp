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

#include "FT_CPosTp145.h"
#include <e32math.h>
#include <LbsPosition.h>

#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_PosLandmarkSerialization.h>
#include <EPos_PosLmCategorySerialization.h>
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp145::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp145::StartL()
    {  
    
  		CheckLmCatSerializationL();
  		
	}

// ---------------------------------------------------------
// CPosTp145::CompareBuffers
//
// ---------------------------------------------------------

 
 void CPosTp145::CompareBuffers(
    const TDesC8& aBuf1,
    const TDesC8& aBuf2)
    {
    DECLARE_ERROR_LOGGING

    CHECK_EQUAL(aBuf1.Length(), aBuf2.Length(), "Different buffer lengths");
    for (TInt i = 0; i < Min(aBuf1.Length(), aBuf2.Length()); i++)
        {
        TInt c1 = aBuf1[i], c2 = aBuf2[i];
        if (c1 != c2)
            {
            _LIT(KMessage, "Different at offset %d (actual = %d, expected %d)");
            message.Format(KMessage, i, c1, c2);
            ERR_DES(message);
            }
        }
    }

// ---------------------------------------------------------
// CPosTp145::CreateDummyCategoryLC
//
// ---------------------------------------------------------
 
CPosLandmarkCategory* CPosTp145::CreateDummyCategoryLC()
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
// CPosTp145::CompareCategoriesL
//
// ---------------------------------------------------------

void CPosTp145::CompareCategoriesL(
    CPosLandmarkCategory& aCat1,
    CPosLandmarkCategory& aCat2)
    {
    DECLARE_ERROR_LOGGING;

    CHECK_EQUAL(aCat1.CategoryId(), aCat2.CategoryId(), "Diff id");
    CHECK_EQUAL(aCat1.GlobalCategory(), aCat2.GlobalCategory(), "Diff global category");

    TPtrC name1, name2;
    aCat1.GetCategoryName(name1);
    aCat2.GetCategoryName(name2);
    ERROR(name1 != name2, "Diff name");

    TInt iconIndex1 = 0, iconMaskIndex1 = 0;
    TInt iconIndex2 = 1, iconMaskIndex2 = 2;

    TInt found1 = aCat1.GetIcon(name1, iconIndex1, iconMaskIndex1);
    TInt found2 = aCat2.GetIcon(name2, iconIndex2, iconMaskIndex2);

    if (found1 == found2)
        {
        if (found1 != KErrNotFound)
            {
            ERROR(name1 != name2, "Diff icon name");
            CHECK_EQUAL(iconIndex1, iconIndex2, "Diff icon index");
            CHECK_EQUAL(iconMaskIndex1, iconMaskIndex2, "Diff icon mask index");
            }
        }
    else
        {
        ERR("Diff icon");
        }
    }

// ---------------------------------------------------------
// CPosTp145::CheckLmCatSerializationL
//
// ---------------------------------------------------------

void CPosTp145::CheckLmCatSerializationL()
    {
    DECLARE_ERROR_LOGGING;
   

    LOG("1. Dummy category")
    CPosLandmarkCategory* cat = CreateDummyCategoryLC();
    CheckLmCatSerialization_SubL(cat);
    CleanupStack::PopAndDestroy(cat);

    LOG("2. Empty category")
    cat = CPosLandmarkCategory::NewLC();
    CheckLmCatSerialization_SubL(cat);
    CleanupStack::PopAndDestroy(cat);

    
    LEAVE_IF_ERRORS()
    }

// ---------------------------------------------------------
// CPosTp145::CheckLmCatSerialization_SubL
//
// ---------------------------------------------------------

void CPosTp145::CheckLmCatSerialization_SubL(CPosLandmarkCategory* aTest)
    {
    DECLARE_ERROR_LOGGING;

    CPosLandmarkCategory* cat1 = aTest;
    CPosLandmarkCategory* cat2 = NULL;

    TInt err = KErrNone;
    HBufC8* buf1 = NULL;
    HBufC8* buf2 = NULL;

    TRAP(err, buf1 = PosLmCategorySerialization::PackL(*cat1));
    IF_ERROR(err, "Packing failed");
    User::LeaveIfError(err);
    CleanupStack::PushL(buf1);

    TRAP(err, cat2 = PosLmCategorySerialization::UnpackL(*buf1));
    IF_ERROR(err, "Unpacking failed");
    User::LeaveIfError(err);
    CleanupStack::PushL(cat2);

    CompareCategoriesL(*cat1, *cat2);

    // compare by buffer
    TRAP(err, buf2 = PosLmCategorySerialization::PackL(*cat2));
    CleanupStack::PushL(buf2);

    CompareBuffers(*buf1, *buf2);

    // cleanup
    CleanupStack::PopAndDestroy(buf2);
    CleanupStack::PopAndDestroy(cat2);
    CleanupStack::PopAndDestroy(buf1);
    }
    
//  End of File
