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

#include "FT_CPosTp144.h"
#include <e32math.h>
#include <LbsPosition.h>

#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_PosLandmarkSerialization.h>
#include <EPos_PosLmCategorySerialization.h>
           
// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp144::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp144::StartL()
    {  
    
  		CheckLmSerializationL();
  		
	}

// ---------------------------------------------------------
// CPosTp144::DumpBuffer
//
// ---------------------------------------------------------

void CPosTp144::DumpBuffer(const TDesC8& aBuffer)
    {
    DECLARE_ERROR_LOGGING;
    LOG("=== Buffer Dump ===");
    message.Format(_L("Buffer size = %d units"), aBuffer.Length());
    LOG_DES(message);
    for (TInt i = 0; i < aBuffer.Length(); i++)
        {
        _LIT(KByte, "Offset %4d = % 2d");
        _LIT(KChar, "Offset %4d = % 2c");
        TUint8 c = aBuffer[i];
        if (TChar(c).IsAlphaDigit())
            {
            message.Format(KChar, i, c);
            }
        else
            {
            message.Format(KByte, i, TInt(c));
            }
        LOG_DES(message);
        }
    LOG("===================");
    }
// ---------------------------------------------------------
// CPosTp144::CompareBuffers
//
// ---------------------------------------------------------
    
void CPosTp144::CompareBuffers(
    const TDesC8& aBuf1,
    const TDesC8& aBuf2)
    {
    DECLARE_ERROR_LOGGING;

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
// CPosTp144::CreateDummyLandmarkLC
//
// ---------------------------------------------------------
    
CPosLandmark* CPosTp144::CreateDummyLandmarkLC()
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
// CPosTp144::CompareLandmarks
//
// ---------------------------------------------------------

void CPosTp144::CompareLandmarks(
    CPosLandmark& aLm1,
    CPosLandmark& aLm2)
    {
    DECLARE_ERROR_LOGGING;

    TPtrC name1, name2;

    CHECK_EQUAL(aLm1.LandmarkId(), aLm2.LandmarkId(), "Diff id");
    CHECK_EQUAL(aLm1.IsPartial(), aLm2.IsPartial(), "Diff partial state");

    aLm1.GetLandmarkName(name1);
    aLm2.GetLandmarkName(name2);
    ERROR(name1 != name2, "Diff name");

    aLm1.GetLandmarkDescription(name1);
    aLm2.GetLandmarkDescription(name2);
    ERROR(name1 != name2, "Diff description");

    TInt iconIndex1 = 0, iconMaskIndex1 = 0;
    TInt iconIndex2 = 1, iconMaskIndex2 = 2;

    TInt found1 = aLm1.GetIcon(name1, iconIndex1, iconMaskIndex1);
    TInt found2 = aLm2.GetIcon(name2, iconIndex2, iconMaskIndex2);

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
        ERR("Diff icon")
        }

    // location
    TLocality loc1, loc2;
    found1 = aLm1.GetPosition(loc1);
    found2 = aLm2.GetPosition(loc2);

    if (found1 == found2)
        {
        if (found1 != KErrNotFound)
            {
            CHECK_EQUAL(loc1.Latitude(), loc2.Latitude(), "Diff latitude");
            CHECK_EQUAL(loc1.Longitude(), loc2.Longitude(), "Diff longitude");
            CHECK_EQUAL(loc1.Altitude(), loc2.Altitude(), "Diff altitude");
            CHECK_EQUAL(loc1.HorizontalAccuracy(), loc2.HorizontalAccuracy(), "Diff hacc");
            CHECK_EQUAL(loc1.VerticalAccuracy(), loc2.VerticalAccuracy(), "Diff vacc");
            CHECK_EQUAL(loc1.Datum(), loc2.Datum(), "Diff datum");
            }
        }
    else
        {
        ERR("Diff position");
        }

    // categories
    RArray<TPosLmItemId> cats1, cats2;
    aLm1.GetCategoriesL(cats1);
    aLm2.GetCategoriesL(cats2);
    CHECK_EQUAL(cats1.Count(), cats2.Count(), "Diff amount of categories");
    for (TInt c = 0; c < Min(cats1.Count(), cats2.Count()); c++)
        {
        CHECK_EQUAL(cats1[c], cats2[c], "Diff category");
        }
    cats1.Close();
    cats2.Close();

    // fields
    TInt fields1 = aLm1.NumOfAvailablePositionFields();
    TInt fields2 = aLm2.NumOfAvailablePositionFields();
    CHECK_EQUAL(fields1, fields2, "Diff amount of fields");

    TPositionFieldId fid1 = aLm1.FirstPositionFieldId();
    TPositionFieldId fid2 = aLm2.FirstPositionFieldId();
    for (TInt f = 0;
        f < Min(fields1, fields2) &&
        (fid1 != EPositionFieldNone || fid2 != EPositionFieldNone);
        f++)
        {
        CHECK_EQUAL(fid1, fid2, "Diff field");
        aLm1.GetPositionField(fid1, name1);
        aLm2.GetPositionField(fid2, name2);
        ERROR(name1 != name2, "Diff field data");

        fid1 = aLm1.NextPositionFieldId(fid1);
        fid2 = aLm1.NextPositionFieldId(fid2);
        }
    }


// ---------------------------------------------------------
// CPosTp144::CheckLmSerialization_SubL
//
// ---------------------------------------------------------

void CPosTp144::CheckLmSerialization_SubL(CPosLandmark* lm)
 {
    DECLARE_ERROR_LOGGING;

    CPosLandmark* lm1 = lm;
    CPosLandmark* lm2 = NULL;

    TInt err = KErrNone;
    HBufC8* buf1 = NULL;
    HBufC8* buf2 = NULL;

    TRAP(err, buf1 = PosLandmarkSerialization::PackL(*lm1));
    IF_ERROR(err, "Packing failed");
    User::LeaveIfError(err);
    CleanupStack::PushL(buf1);

    TRAP(err, lm2 = PosLandmarkSerialization::UnpackL(*buf1));
    IF_ERROR(err, "Unpacking failed");
    User::LeaveIfError(err);
    CleanupStack::PushL(lm2);

    // read and compare values
    CompareLandmarks(*lm1, *lm2);

    // compare by buffer
    TRAP(err, buf2 = PosLandmarkSerialization::PackL(*lm2));
    CleanupStack::PushL(buf2);

    CompareBuffers(*buf1, *buf2);

    // cleanup
    CleanupStack::PopAndDestroy(buf2);
    CleanupStack::PopAndDestroy(lm2);
    CleanupStack::PopAndDestroy(buf1);
 
    }

// ---------------------------------------------------------
// CPosTp144::CheckLmSerializationL
//
// ---------------------------------------------------------
    
    void CPosTp144::CheckLmSerializationL()
    {
    DECLARE_ERROR_LOGGING;
    

    //LOG("1. Dummy landmark")
    CPosLandmark* lm = CreateDummyLandmarkLC();
    CheckLmSerialization_SubL(lm);
    CleanupStack::PopAndDestroy(lm);

    //LOG("2. Empty landmark")
    lm = CPosLandmark::NewLC();
    CheckLmSerialization_SubL(lm);
    CleanupStack::PopAndDestroy(lm);

    
    LEAVE_IF_ERRORS()  
    }
//  End of File
