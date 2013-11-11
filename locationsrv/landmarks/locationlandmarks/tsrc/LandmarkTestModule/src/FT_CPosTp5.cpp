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

#include "FT_CPosTp5.h"
#include <EPos_CPosLandmark.h> 
#include <LbsPosition.h>
#include <e32def.h> 
#include <e32std.h> 
#include <e32math.h>


#include "FT_LandmarkConstants.h"

// CONSTANTS
// _LIT(KGetNameErr, "GetLandmarks name returns incorrect error code");
_LIT(KNameErr, "Landmark name is incorrect");
_LIT(KIdErr, "Landmark Id is incorrect");
_LIT(KLongitudeErr, "Longitude is incorrect");

_LIT(KLatitudeErr, "Latitude is incorrect");
_LIT(KHorizontalAccErr, "Horizontal accuracy is incorrect");
_LIT(KVerticalAccErr, "Vertical accuracy is incorrect");
_LIT(KAltitudeErr, "Altitude is incorrect");
_LIT(KNumOfAvailablePositionFieldsErr, "NumOfAvailablePositionFields is incorrect");
_LIT(KNumOfategoriesErr, "Number of categories is incorrect");
_LIT(KCoverageRadiusErr, "CoverageRadius is incorrect"); 
_LIT(KIconIndexErr, "Icon index is incorrect");
_LIT(KIconMaskIndexErr, "Icon MaskIndex is incorrect");
_LIT(KMbmFileNameErr, "MbmFileName is incorrect");
_LIT(KDescriptionErr, "Description is incorrect");
_LIT(KPositionFieldErr, "Error in positions fields");
          
_LIT(KNameTp5, "Lm-Tp5");

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp5::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp5::StartL()
    { 
    _LIT(KNameTp5, "Lm-Tp5");
    RemoveDefaultDbL();
    CPosLandmark* landmark = CreateLandmarkLC(KNameTp5);
    
    CheckGetMethodsL(landmark);
    
    CheckRemoveMethodsL(landmark);

    CheckFieldsMaxLengthL();

    CleanupStack::PopAndDestroy(); // landmark

    CheckNaNErrorL();
    }

// ---------------------------------------------------------
// CPosTp5::CheckGetMethods
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp5::CheckGetMethodsL(CPosLandmark* aLandmark)
    {
 
    TPtrC landmarkName;
    aLandmark->GetLandmarkName(landmarkName);  
    AssertTrueSecL(landmarkName.Compare(KNameTp5) == KErrNone, KNameErr);
    AssertTrueSecL(aLandmark->LandmarkId() == 0, KIdErr);
    
    CPosLandmarkDatabase* lmd = OpenDatabaseLC();
    TPosLmItemId id=lmd->AddLandmarkL(*aLandmark);
    AssertTrueSecL(aLandmark->LandmarkId() == id, KIdErr);

    TLocality loc2;
    aLandmark->GetPosition(loc2);
    AssertTrueSecL(loc2.Longitude() == KLongitude, KLongitudeErr);
    AssertTrueSecL(loc2.Latitude() == KLatitude, KLatitudeErr);
    AssertTrueSecL(loc2.HorizontalAccuracy() == KHorizontalAcc, KHorizontalAccErr);
    AssertTrueSecL(loc2.VerticalAccuracy() == KVerticalAcc, KVerticalAccErr);
    AssertTrueSecL(loc2.Altitude() == KAltitude, KAltitudeErr);
    
    TReal32 r;
    aLandmark->GetCoverageRadius(r);
    AssertTrueSecL(r == KCoverageRadius, KCoverageRadiusErr);

    TPtrC mbmFileName;
    TInt index;
    TInt indexMask;
    aLandmark->GetIcon(mbmFileName, index, indexMask);
    AssertTrueSecL(mbmFileName.Compare(KMbmFileName) == KErrNone, KMbmFileNameErr);
    AssertTrueSecL(index == KIconIndex, KIconIndexErr);
    AssertTrueSecL(indexMask == KIconMaskIndex, KIconMaskIndexErr);

    TPtrC desc;
    aLandmark->GetLandmarkDescription(desc);
    AssertTrueSecL(desc.Compare(KDescription) == KErrNone, KDescriptionErr);
    AssertTrueSecL(KNoPosFields == aLandmark->NumOfAvailablePositionFields(), KNumOfAvailablePositionFieldsErr);
    
    TPositionFieldId posFieldId = aLandmark->FirstPositionFieldId();
    for (TUint i=0; i<KNoPosFields-1; i++)
        {
        CheckPositionFieldL(*aLandmark, posFieldId);
        posFieldId=aLandmark->NextPositionFieldId(posFieldId);
        }

    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);
    aLandmark->GetCategoriesL(categories);
    AssertTrueSecL(categories.Count() == KNoCategories, KNumOfategoriesErr);
    CleanupStack::PopAndDestroy(2,lmd); 
    
   	}

// ---------------------------------------------------------
// CPosTp5::CheckRemoveMethodsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//    
void CPosTp5::CheckRemoveMethodsL(CPosLandmark* aLandmark)
    {
    aLandmark->RemoveLandmarkAttributes(CPosLandmark::ENoAttribute);
    aLandmark->RemoveLandmarkAttributes(CPosLandmark::EPosition);
    aLandmark->RemoveLandmarkAttributes(CPosLandmark::ECoverageRadius);
    aLandmark->RemoveLandmarkAttributes(CPosLandmark::ECategoryInfo);
    aLandmark->RemoveLandmarkAttributes(CPosLandmark::EIcon);
    aLandmark->RemoveLandmarkAttributes(CPosLandmark::EDescription);
    
    CheckRemovedL(aLandmark);
    _LIT(KLandmarkName, "Nisse");
    CPosLandmark* landmark = CreateLandmarkLC(KLandmarkName);
    landmark->RemoveLandmarkAttributes(CPosLandmark::EAllAttributes);
    CheckRemovedL(landmark);

    
    TUint16 tmp = (TUint16)EPositionFieldMediaLinksStart;
    landmark->RemovePositionField(EPositionFieldMediaLinksStart);
    landmark->RemovePositionField(EPositionFieldCountry);
    landmark->RemovePositionField(EPositionFieldHorizontalSpeed);
    // FrBo Below has been removed
    
    landmark->RemovePositionField(EPositionFieldMediaLinks); 
    landmark->RemovePositionField(EPositionFieldMediaLinksStart); 
    landmark->RemovePositionField(++tmp);
    landmark->RemovePositionField(++tmp);
    landmark->RemovePositionField(EPositionFieldIdLast);
    
    TPtrC ptr;    
    AssertTrueSecL(landmark->GetPositionField(EPositionFieldMediaLinksStart, ptr) == KErrNotFound, KPositionFieldErr);
    AssertTrueSecL(landmark->GetPositionField(EPositionFieldCountry, ptr) == KErrNotFound, KPositionFieldErr);
    AssertTrueSecL(landmark->GetPositionField(EPositionFieldHorizontalSpeed, ptr) == KErrNotFound, KPositionFieldErr);
    // FrBo Below has been removed
    
    AssertTrueSecL(landmark->GetPositionField(EPositionFieldMediaLinks, ptr) == KErrNotFound, KPositionFieldErr); 
    AssertTrueSecL(landmark->GetPositionField(EPositionFieldMediaLinksStart, ptr) == KErrNotFound, KPositionFieldErr); 
    AssertTrueSecL(landmark->GetPositionField(++tmp, ptr) == KErrNotFound, KPositionFieldErr); 
    AssertTrueSecL(landmark->GetPositionField(++tmp, ptr) == KErrNotFound, KPositionFieldErr); 
    AssertTrueSecL(landmark->GetPositionField(EPositionFieldIdLast, ptr) == KErrNotFound, KPositionFieldErr); 
    
    // Remove some non-existing fields, nothing should happen
    landmark->RemovePositionField(6000);
    AssertTrueSecL(landmark->GetPositionField(6000, ptr) == KErrNotFound, KPositionFieldErr); 
    landmark->RemovePositionField(6001);
    AssertTrueSecL(landmark->GetPositionField(6001, ptr) == KErrNotFound, KPositionFieldErr); 

    CleanupStack::PopAndDestroy(); // landmark 
    
    }

// ---------------------------------------------------------
// CPosTp5::CheckRemovedL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp5::CheckRemovedL(CPosLandmark* aLandmark)
    {
    _LIT(KPositionRemovedErr, "GetPosition is not returning an error code when position is not set");
    TLocality loc;
    AssertTrueSecL(aLandmark->GetPosition(loc) == KErrNotFound, KPositionRemovedErr);
   
    RArray<TPosLmItemId> categories;
    CleanupClosePushL(categories);
    aLandmark->GetCategoriesL(categories);
    AssertTrueSecL(categories.Count() == 0, KNumOfategoriesErr);
    CleanupStack::PopAndDestroy(&categories);

    TReal32 r;
    AssertTrueSecL(aLandmark->GetCoverageRadius(r) == KErrNotFound, KCoverageRadiusErr);
    
    TPtrC mbmFileName;
    TInt index;
    TInt indexMask;
    AssertTrueSecL(aLandmark->GetIcon(mbmFileName, index, indexMask) == KErrNotFound, KMbmFileNameErr);

    TPtrC desc;
    AssertTrueSecL(aLandmark->GetLandmarkDescription(desc) == KErrNotFound, KDescriptionErr);
   }

//---------------------------------------------------------
// CPosTp5::CheckPositionFieldIdL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp5::CheckPositionFieldL(const CPosLandmark& aLandmark, const TUint16 aId)
    {
    
    _LIT(KPositionFieldIdErr, "PositionFieldId is incorrect");
    _LIT(KComparePositionFieldValueErr, "PositionField value is different from set value");
    
    TUint16 tmp = (TUint16)EPositionFieldMediaLinksStart;
    TBuf<100> buf;
    buf.Format(_L("Looking for id:%d "),aId);
    iLog->Log(buf);
    
    TPtrC value;
    if (aId == EPositionFieldCountry)
        {
        aLandmark.GetPositionField(aId, value);             
        AssertTrueSecL(value.Compare(KCountry) == KErrNone, KComparePositionFieldValueErr);
        }
    else if (aId == EPositionFieldHorizontalSpeed)
        {
        aLandmark.GetPositionField(aId, value);
        AssertTrueSecL(value.Compare(KHorizontalSpeed) == KErrNone, KComparePositionFieldValueErr);
        }
       
    else if (aId == EPositionFieldMediaLinks)
        {
        aLandmark.GetPositionField(aId, value);
        AssertTrueSecL(value.Compare(KMediaLinks) == KErrNone, KComparePositionFieldValueErr);
        }
    else if (aId == EPositionFieldMediaLinksStart)
        {
        aLandmark.GetPositionField(aId, value);
        AssertTrueSecL(value.Compare(KMediaLink1) == KErrNone, KComparePositionFieldValueErr);
        iLog->Log(_L("EPositionFieldMediaLinksStart"));
        }
    else if (aId == EPositionFieldIdLast)
        {
        aLandmark.GetPositionField(aId, value);
        AssertTrueSecL(value.Compare(KLast) == KErrNone, KComparePositionFieldValueErr);
        }
    else if (aId == (tmp+1))
        {
        aLandmark.GetPositionField(aId, value);
        AssertTrueSecL(value.Compare(KMediaLink2) == KErrNone, KComparePositionFieldValueErr);
        }
    else if(aId == (tmp+2)) 
        {
        aLandmark.GetPositionField(aId, value);
        AssertTrueSecL(value.Compare(KMediaLink3) == KErrNone, KComparePositionFieldValueErr);
        }
    else
        {
        buf.Format(_L("Looking for id: %d %d %d"), aId, (tmp+1), (tmp+2));
        iLog->Log(buf);
        iLog->Log(KPositionFieldIdErr);
        User::Leave(KErrGeneral);
        }
     }

// ---------------------------------------------------------
// CPosTp5::CheckFieldsMaxLength
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp5::CheckFieldsMaxLengthL()
    {

    _LIT(K255Error, "Text field is not limited to 255 characters");
    _LIT(K4095Error, "Description field is not limited to 4095 characters");
    _LIT(KIconError, "Icon File Name field is not limited to correct nr of characters");

    HBufC* hbuf = HBufC::NewLC(4096);
    while(hbuf->Des().Length() < 255)
        {
        hbuf->Des().Append(_L("X"));
        }

    CPosLandmark* landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(*hbuf);
    landmark->SetLandmarkDescriptionL(*hbuf);      
    landmark->SetPositionFieldL(EPositionFieldCountry, *hbuf);
    hbuf->Des().Append(_L("X"));

    TRAPD(err, landmark->SetLandmarkNameL(*hbuf));
    AssertTrueSecL(err == KErrArgument, K255Error);

    landmark->SetLandmarkDescriptionL(*hbuf);      
    
    TRAP(err, landmark->SetPositionFieldL(EPositionFieldCountry, *hbuf));
    AssertTrueSecL(err == KErrArgument, K255Error);
    
    while(hbuf->Des().Length() < 4095)
       {
        hbuf->Des().Append(_L("X"));
       }
    landmark->SetLandmarkDescriptionL(*hbuf);      
    hbuf->Des().Append(_L("X"));
    TRAP(err, landmark->SetLandmarkDescriptionL(*hbuf));      
    AssertTrueSecL(err == KErrArgument, K4095Error);
    
    CleanupStack::PopAndDestroy(2, hbuf);
    
    // Test setIconL
    // KMaxFileName is defined in e32const.h
    HBufC* icon = HBufC::NewLC(KMaxFileName+1);
    while(icon->Des().Length() < KMaxFileName)
        {
        icon->Des().Append(_L("X"));
        }

    landmark = CPosLandmark::NewLC();
    landmark->SetLandmarkNameL(_L("SomeLandmarkName"));
    landmark->SetLandmarkDescriptionL(_L("SomeLandmarkDescription"));
    landmark->SetIconL(*icon, 1, KPosLmIconMaskNotUsed);
    
    icon->Des().Append(_L("X"));

    TRAP(err, landmark->SetIconL(*icon, 1, KPosLmIconMaskNotUsed));
    AssertTrueSecL(err == KErrArgument, KIconError);
    CleanupStack::PopAndDestroy(landmark);
    CleanupStack::PopAndDestroy(icon); 
    
   
    }

// ---------------------------------------------------------
// CPosTp5::CheckNaNErrorL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp5::CheckNaNErrorL()
    {
    iLog->Log(_L("CheckErrorL"));
    CPosLandmarkDatabase* lmdb = OpenDatabaseLC();
    CPosLandmark* landmark = CPosLandmark::NewLC();
    
    ExecuteAndDeleteLD(lmdb->CompactL());
    TInt errorsFound = KErrNone;
    TRealX NaN;
    NaN.SetNaN();

    // Used for testing SDK:
    // This part should result in log:
    // Not Equal1
    // Not Equal2
    TRealX pos1;
    TRealX pos2;

    iLog->Log(_L("Testing with NaN and 45"));
    pos1.SetNaN();
    pos2 = 45;

    if (pos2 == pos1) 
        iLog->Log(_L("Equal1"));
        else
        iLog->Log(_L("Not Equal1"));

    if (pos2 != pos1) 
        iLog->Log(_L("Not Equal2"));
        else
        iLog->Log(_L("Equal2"));


    iLog->Log(_L("Testing with NaN and 0"));
    pos1.SetNaN();
    pos2 = 0;

    if (pos2 == pos1) 
        iLog->Log(_L("Equal1"));
        else
        iLog->Log(_L("Not Equal1"));

    if (pos2 != pos1) 
        iLog->Log(_L("Not Equal2"));
        else
        iLog->Log(_L("Equal2"));

    iLog->Log(_L("Testing with NaN and NaN"));
    pos1.SetNaN();
    pos2.SetNaN();

    if (pos2 == pos1) 
        iLog->Log(_L("Equal1"));
        else
        iLog->Log(_L("Not Equal1"));

    if (pos2 != pos1) 
        iLog->Log(_L("Not Equal2"));
        else
        iLog->Log(_L("Equal2"));

    landmark->SetLandmarkNameL(_L("TestingLandmarkError"));

    // Only set position with Latitude and Longitude (thus Altitude and Accuracy is NaN)
    TLocality loc;
    loc.SetCoordinate(1,2);
    landmark->SetPositionL(loc);

    TPosLmItemId myId = lmdb->AddLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    //////////////////////////////////////
    // Test Altitude
    landmark = lmdb->ReadLandmarkLC(myId);    
    landmark->GetPosition(loc);

    loc.SetCoordinate(1,2, 123);
    landmark->SetPositionL(loc);

    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    TLocality loc3;
    landmark->GetPosition(loc3);

    if (TRealX(loc3.Altitude()).IsNaN())
        {
        iLog->Log(_L("Error: Altitude is Nan"));
        errorsFound++;
        }
    else if (loc3.Altitude() != 123)
        {
        iLog->Log(_L("Error: Incorrect altitude returned"));
        errorsFound++;
        }

    // Reset landmark
    loc.SetCoordinate(1,2, NaN);
    landmark->SetPositionL(loc);
    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    //////////////////////////////////////
    // Test Horizontal accuracy
    landmark = lmdb->ReadLandmarkLC(myId);
    loc.SetCoordinate(1,2, NaN);
    loc.SetHorizontalAccuracy(12);
    landmark->SetPositionL(loc);

    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->GetPosition(loc3);

    if (TRealX(loc3.HorizontalAccuracy()).IsNaN())
        {
        iLog->Log(_L("Error: Horizontal accuracy is Nan"));
        errorsFound++;
        }
    else if (loc3.HorizontalAccuracy() != 12)
        {
        iLog->Log(_L("Error: Incorrect horizontal accuracy returned"));
        errorsFound++;
        }

    CleanupStack::PopAndDestroy(landmark);

    //////////////////////////////////////
    // Test with NaN for hor acc
    landmark = lmdb->ReadLandmarkLC(myId);
    loc.SetCoordinate(1,2, NaN);
    loc.SetHorizontalAccuracy(NaN);
    landmark->SetPositionL(loc);

    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->GetPosition(loc3);

    if (TRealX(loc3.HorizontalAccuracy()).IsNaN())
        {
        iLog->Log(_L("Horizontal accuracy is correctly set to Nan"));
        }
    else
        {
        iLog->Log(_L("Error: Incorrect horizontal accuracy returned"));
        errorsFound++;
        }

    CleanupStack::PopAndDestroy(landmark);

    //////////////////////////////////////
    // Test Vertical accuracy
    landmark = lmdb->ReadLandmarkLC(myId);
    loc.SetCoordinate(1,2, NaN);
    loc.SetVerticalAccuracy(21);
    landmark->SetPositionL(loc);

    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->GetPosition(loc3);

    if (TRealX(loc3.VerticalAccuracy()).IsNaN())
        {
        iLog->Log(_L("Error: Vertical accuracy is Nan"));
        errorsFound++;
        }
    else if (loc3.VerticalAccuracy() != 21)
        {
        iLog->Log(_L("Error: Incorrect vertical accuracy returned"));
        errorsFound++;
        }

    CleanupStack::PopAndDestroy(landmark);

    //////////////////////////////////////
    // Test landmark description
    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->SetLandmarkDescriptionL(_L("TestingLandmarkErrorDescription"));
    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    TPtrC des;
    landmark->GetLandmarkDescription(des);
    TInt res = des.CompareC(_L("TestingLandmarkErrorDescription"));

    if (res != KErrNone)
        {
        iLog->Log(_L("Error: Incorrect description returned"));
        errorsFound++;
        }

    CleanupStack::PopAndDestroy(landmark);

    //////////////////////////////////////
    // Test coverage radius
    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->SetCoverageRadius(15);
    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->GetPosition(loc3);
    TReal32 radius;
    landmark->GetCoverageRadius(radius);
    if (TRealX(radius).IsNaN())
        {
        iLog->Log(_L("Error: Radius is Nan"));
        errorsFound++;
        }
    else if (radius != 15)
        {
        iLog->Log(_L("Error: Incorrect radius returned"));
        errorsFound++;
        }
    //////////////////////////////////////
    CleanupStack::PopAndDestroy(landmark);
    //////////////////////////////////////
    // Test coverage radius, part2
    // Set coverage radius to NaN
    landmark = lmdb->ReadLandmarkLC(myId);
    TRealX nan;
    nan.SetNaN();
    landmark->SetCoverageRadius(nan);
    lmdb->UpdateLandmarkL(*landmark);
    CleanupStack::PopAndDestroy(landmark);

    landmark = lmdb->ReadLandmarkLC(myId);
    landmark->GetPosition(loc3);

    res = landmark->GetCoverageRadius(radius);
    if (res != KErrNotFound)
        {
        iLog->Log(_L("Error: GetCoverageRadius should return KErrNotFound when set to NaN"));
        errorsFound++;
        }
    //////////////////////////////////////
    CleanupStack::PopAndDestroy(landmark);    
    
    CleanupStack::PopAndDestroy(lmdb);

    if (errorsFound != KErrNone)
        {
        
        iLog->Log(_L("Errors found in part CheckNaNErrorL()"));
        User::Leave(-1);
        }
       
    }
