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
#include "FT_CLandmarkTestProcedureBase.h"

#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLMCategoryManager.h>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_CPosLmDatabaseManager.h>
#include <EPos_CPosLMOperation.h>
#include <EPos_TPosLMSortPref.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLmDisplayItem.h>
#include <LbsPosition.h>
#include <lbs.h>
#include <bautils.h>
#include <s32file.h>
#include "FT_LandmarkConstants.h"
#include <e32def.h>
#include <BaRsc2.H>
#include <BaRsRead2.H>
#include <BAUTILS.H>
#include <utf.h>

// CONSTANTS
const TInt KBufSize=100;
const TInt KMaxFieldLength = 80;

const TInt KFirstNextStepLogLimit = 100000; //µs

// CONSTANTS
_LIT(KLt,           "<");
_LIT(KGt,           ">");
_LIT(KAmp,          "&");
_LIT(KApos,         "'");
_LIT(KQuot,         "\"");

_LIT(KLtXmlStr,     "&lt;");
_LIT(KGtXmlStr,     "&gt;");
_LIT(KAmpXmlStr,    "&amp;");
_LIT(KAposXmlStr,   "&apos;");
_LIT(KQuotXmlStr,   "&quot;");

// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// CLandMarksTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//



// C++ default constructor can NOT contain any code, that
// might leave.
//
CLandmarkTestProcedureBase::CLandmarkTestProcedureBase(
						TInt aModuleId, CStifLogger* aLog)
						: iModuleId(aModuleId),
    					  KFirstDelayTime(5000),
    					  KIntervalTime(5000),
                          iLog(aLog)
    {
    iUseLogFromThreadIsDisabled = EFalse;
    iFileSession.Connect();
    }



// Destructor
CLandmarkTestProcedureBase::~CLandmarkTestProcedureBase()
    {
    iSearchResults.ResetAndDestroy();
    iAreaSearchResults.ResetAndDestroy();

    iCategorySearchResults.ResetAndDestroy();
    iCategoryLandmarkSearchResults.ResetAndDestroy();
    iFileSession.Close();
    ReleaseLandmarkResources();
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::InitTestL()
    {
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::StartL()
    {
    iLog->Log(_L("StartL not overridden"));
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CloseTest()
    {
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RemoveAllLmDatabasesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RemoveAllLmDatabasesL()
    {
    CPosLmDatabaseManager* dbManager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbManager);

    CDesCArray* dbArray = dbManager->ListDatabasesLC();
    TInt count = dbArray->Count();

    for (TInt i=0;i<count;i++)
        {
        iLog->Log((*dbArray)[i]);
        TRAPD(error,dbManager->DeleteDatabaseL((*dbArray)[i]));
        if (error != KErrNone)
            {
            if( error == KErrInUse )
                {
                iLog->Log(_L("Cannot delete database since it is in use"));
                }
            else
                {
            iLog->Log(_L("Delete database leaves with error = %d"),error);
                User::Leave( error);
                }

            }
        }

    CleanupStack::PopAndDestroy(dbArray);
    CleanupStack::PopAndDestroy(dbManager);
    iLog->Log(_L("DeleteAllDatabasesL Done"));
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RemoveDefaultDb
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RemoveDefaultDbL()
    {



    iLog->Log(_L("In CLandmarkTestProcedureBase::RemoveDefaultDbL() "));


    // Retrieve default db from db manager and then delete

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();

    CleanupStack::PushL(manager);

    HBufC* defaultDbUri;
    defaultDbUri = manager->DefaultDatabaseUriLC();

    TRAPD(err, manager->DeleteDatabaseL(*defaultDbUri));
    if (err != KErrNone && err != KErrNotFound)
        {
        if( err == KErrInUse)
            {
            iLog->Log(_L("Cannot delete database since it is in use"));
            }
        else
            {
            iLog->Log(_L("Delete of the default landmarks database failed with %d"),err);
            User::Leave(err);
            }
        }

    CleanupStack::PopAndDestroy(defaultDbUri);
    CleanupStack::PopAndDestroy(manager);


    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CreateCategoryLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkCategory* CLandmarkTestProcedureBase::CreateCategoryLC(const TDesC& aName)
    {
    _LIT(KGetCategoryNameErr, "GetCategory name returns incorrect error code");

    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();

    TInt err=0;
    TPtrC name;
    err=category->GetCategoryName(name);
    AssertTrueSecL(err == KErrNotFound, KGetCategoryNameErr);

    category->SetCategoryNameL(aName);

    category->SetIconL(KMbmFileName, KIconIndex, KIconMaskIndex);

    return category;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CreateLandmarkLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmark* CLandmarkTestProcedureBase::CreateLandmarkLC(const TDesC& aName)
    {
    _LIT(KGetNameErr, "GetLandmarks name returns incorrect error code");
    _LIT(KFirstPositionFieldIdErr, "FirstPositionFieldId returns incorrect id");


    CPosLandmark* landmark = CPosLandmark::NewLC();
    TInt err=0;
    TPtrC name;
    err=landmark->GetLandmarkName(name);
    AssertTrueSecL(err == KErrNotFound, KGetNameErr);

    landmark->SetLandmarkNameL(aName);

    TCoordinate coord(KLatitude, KLongitude, KAltitude);
    TLocality loc(coord, KHorizontalAcc, KVerticalAcc);
    TTime time;
    time.UniversalTime();
    TPosition pos(loc, time);
    landmark->SetPositionL(pos);

    landmark->SetIconL(KMbmFileName, KIconIndex, KIconMaskIndex);
    landmark->SetLandmarkDescriptionL(KDescription);
    landmark->SetCoverageRadius(KCoverageRadius);

    AssertTrueSecL(landmark->FirstPositionFieldId()==EPositionFieldNone, KFirstPositionFieldIdErr);

    AddFieldsL(landmark);

    _LIT(KCategoryName, "%S Category %d");
    for(TInt i=0; i<KNoCategories; i++)
        {
        TBuf<100> name;
        HBufC* tmp = aName.Alloc();
        name.Format(KCategoryName,tmp,i);
        delete tmp;
        landmark->AddCategoryL(CreateCategoryL(name));
        }
    return landmark;

    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AddFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::AddFieldsL(CPosLandmark* aLandmark)
    {
    _LIT(KPositionFieldNoneErr, "SetPositionFieldL should return KErrArgumnet with EPositionFieldNone");

    aLandmark->SetPositionFieldL(EPositionFieldCountry, KCountry);
    aLandmark->SetPositionFieldL(EPositionFieldHorizontalSpeed, KHorizontalSpeed);
    // Removed from LBSFieldsIds.h

    aLandmark->SetPositionFieldL(EPositionFieldMediaLinks, KMediaLinks);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, KMediaLink1);

    TUint16 tmp = (TUint16)EPositionFieldMediaLinksStart;
    aLandmark->SetPositionFieldL(++tmp, KMediaLink2);
    aLandmark->SetPositionFieldL(++tmp, KMediaLink3);

    aLandmark->SetPositionFieldL(EPositionFieldIdLast, KOrientation);

    TRAPD(err, aLandmark->SetPositionFieldL(EPositionFieldNone, KMediaLink3));
    AssertTrueSecL(err == KErrArgument, KPositionFieldNoneErr);

    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AddFields2L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::AddFields2L(CPosLandmark* aLandmark)
    {
    aLandmark->SetPositionFieldL(EPositionFieldCountry, KCountry2);
    aLandmark->SetPositionFieldL(EPositionFieldHorizontalSpeed, KHorizontalSpeed2);
    // Removed from LBSFieldsIds.h

    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, KMediaLinks2);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+1, KMediaLink4);

    TUint16 tmp = (TUint16)EPositionFieldMediaLinksStart;
    aLandmark->SetPositionFieldL(++tmp, KMediaLink5);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AddFields3L
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::AddFields3L(CPosLandmark* aLandmark)
    {
    aLandmark->SetPositionFieldL(EPositionFieldCountryCode, KCountryCode);
    aLandmark->SetPositionFieldL(EPositionFieldState, KState);
    aLandmark->SetPositionFieldL(EPositionFieldCity, KCity);
    aLandmark->SetPositionFieldL(EPositionFieldDistrict, KDistrict);
    aLandmark->SetPositionFieldL(EPositionFieldStreet, KStreet);
    aLandmark->SetPositionFieldL(EPositionFieldStreetExtension, KStreetExtension);
    aLandmark->SetPositionFieldL(EPositionFieldLocationName, KLocationName);
    aLandmark->SetPositionFieldL(EPositionFieldPostalCode, KPostalCode);
    aLandmark->SetPositionFieldL(EPositionFieldLocality, KLocality);
    aLandmark->SetPositionFieldL(EPositionFieldCrossing1, KCrossing1);
    aLandmark->SetPositionFieldL(EPositionFieldCrossing2, KCrossing2);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingName, KBuildingName);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingFloor, KBuildingFloor);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingRoom, KBuildingRoom);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingZone, KBuildingZone);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, KBuildingTelephone);
    }


// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CreateXMLLandmarkLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmark* CLandmarkTestProcedureBase::CreateXMLLandmarkLC(const TDesC& aName)
    {
    _LIT(KGetNameErr, "GetLandmarks name returns incorrect error code");
    _LIT(KFirstPositionFieldIdErr, "FirstPositionFieldId returns incorrect id");

    CPosLandmark* landmark = CPosLandmark::NewLC();

    TInt err=0;
    TPtrC name;
    err=landmark->GetLandmarkName(name);
    AssertTrueSecL(err == KErrNotFound, KGetNameErr);

    landmark->SetLandmarkNameL(aName);

    TCoordinate coord(KLatitude, KLongitude, KAltitude);
    TLocality loc(coord, KHorizontalAcc, KVerticalAcc);
    TTime time;
    time.UniversalTime();
    TPosition pos(loc, time);
    landmark->SetPositionL(pos);

    landmark->SetIconL(KMbmFileName, KIconIndex, KPosLmIconMaskNotUsed);

    landmark->SetLandmarkDescriptionL(KDescription);

    landmark->SetCoverageRadius(KCoverageRadius);

    AssertTrueSecL(landmark->FirstPositionFieldId()==EPositionFieldNone, KFirstPositionFieldIdErr);

    return landmark;
    }


// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AddAllXMLFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::AddAllXMLFieldsL(CPosLandmark* aLandmark)
    {
    aLandmark->SetPositionFieldL(EPositionFieldCountry, KCountry2);
    aLandmark->SetPositionFieldL(EPositionFieldCountryCode, KCountryCode);
    aLandmark->SetPositionFieldL(EPositionFieldState, KState);
    aLandmark->SetPositionFieldL(EPositionFieldCounty, KCounty);
    aLandmark->SetPositionFieldL(EPositionFieldCity, KCity);
    aLandmark->SetPositionFieldL(EPositionFieldDistrict, KDistrict);
    aLandmark->SetPositionFieldL(EPositionFieldPostalCode, KPostalCode);

    aLandmark->SetPositionFieldL(EPositionFieldCrossing1, KCrossing1);
    aLandmark->SetPositionFieldL(EPositionFieldCrossing2, KCrossing2);
    aLandmark->SetPositionFieldL(EPositionFieldStreet, KStreet);

    aLandmark->SetPositionFieldL(EPositionFieldBuildingName, KBuildingName);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingFloor, KBuildingFloor);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingRoom, KBuildingRoom);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingZone, KBuildingZone);

    aLandmark->SetPositionFieldL(EPositionFieldStreetExtension, KStreetExtension);
    aLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, KBuildingTelephone);

    // Set number of medialink fields
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinks, _L("13"));
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, KXMLMediaLink1);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+1, KXMLMediaLink2);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+2, KXMLMediaLink3);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+3, KXMLMediaLink4);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+4, KXMLMediaLink5);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+5, KXMLMediaLink6);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+6, KXMLMediaLink7);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+7, KXMLMediaLink8);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+8, KXMLMediaLink9);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+9, KXMLMediaLink10);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+10, KXMLMediaLink11);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+11, KXMLMediaLink12);
    aLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart+12, KXMLMediaLink13);


    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AddAllXMLFieldsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::AddAllEmptyXMLFieldsL(CPosLandmark* aLandmark)
    {
    aLandmark->SetPositionFieldL(EPositionFieldCountry, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldCountryCode, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldState, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldCounty, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldCity, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldDistrict, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldPostalCode, _L(""));

    aLandmark->SetPositionFieldL(EPositionFieldCrossing1, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldCrossing2, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldStreet, _L(""));

    aLandmark->SetPositionFieldL(EPositionFieldBuildingName, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldBuildingFloor, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldBuildingRoom, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldBuildingZone, _L(""));

    aLandmark->SetPositionFieldL(EPositionFieldStreetExtension, _L(""));
    aLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, _L(""));

    aLandmark->SetPositionFieldL(EPositionFieldMediaLinks, _L("0"));


    aLandmark->SetLandmarkDescriptionL(_L(""));


    }

//---------------------------------------------------------
// CLandmarkTestProcedureBase::WriteBufferToFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CLandmarkTestProcedureBase::WriteBufferToFileL(const CBufBase* aBuffer, const TDesC& aFile)
    {
    iFileSession.Delete(aFile);

    RFile file;
    file.Create(iFileSession, aFile, EFileWrite);
    CleanupClosePushL(file);

    RFileWriteStream stream(file);
    CleanupClosePushL(stream);

    TInt pos=0;
    TInt size = aBuffer->Size();
    TInt length;
    if (size < 255 )
        {
        length = size;
        }
    else
        {
        length = 255;
        }

    while (pos < size)
        {
        TBuf8<255> xml;

        aBuffer->Read(pos, xml, length);

        stream.WriteL(xml);

        pos += 255;

        if (pos + 255 > size )
            {
            length = size - pos;
            }
        }

    CleanupStack::PopAndDestroy(2, &file);
    }

//---------------------------------------------------------
// CLandmarkTestProcedureBase::CreateCategoryL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPosLmItemId CLandmarkTestProcedureBase::CreateCategoryL(const TDesC& aName)
    {
    _LIT(KCategoryIdErr, "Category id is incorrect");

    CPosLandmarkDatabase* lmd = OpenDatabaseLC();

    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*lmd);
    CleanupStack::PushL(cm);

    TPosLmItemId id=0;



        CPosLandmarkCategory* lmc = CPosLandmarkCategory::NewLC();
        lmc->SetCategoryNameL(aName);

        id = cm->AddCategoryL(*lmc);

        AssertTrueSecL(id == lmc->CategoryId(), KCategoryIdErr);
        CleanupStack::PopAndDestroy(lmc);


    CleanupStack::PopAndDestroy(2, lmd);

    return id;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::OpenLandmarkDatabaseLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::OpenDatabaseLC()
    {
    CPosLandmarkDatabase* lmd = CPosLandmarkDatabase::OpenL();
    CleanupStack::PushL(lmd);

    if (lmd->IsInitializingNeeded())
       {
       CPosLmOperation* op = lmd->InitializeL(); // Synchronous since no argument
       op->ExecuteL();
       delete op;
       }
    return lmd;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CompareLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CompareLandmarksL(const CPosLandmark& aSource, const CPosLandmark& aTarget)
    {
    _LIT(KCompareIdErr, "Landmark id's are not equal");
    _LIT(KComparePositionErr, "Landmark positions are not equal");
    _LIT(KComparePositionErr1, "Landmark longitude are not equal");
    _LIT(KComparePositionErr2, "Landmark latitude are not equal");
    _LIT(KComparePositionErr3, "Landmark horizontal accuracy are not equal");
    _LIT(KComparePositionErr4, "Landmark vertical accuracy are not equal");
    _LIT(KComparePositionErr5, "Landmark altitude are not equal");
    _LIT(KCompareNameErr, "Landmark names are not equal");
    _LIT(KCompareCoverageRadiusErr, "Landmark coverage radiuses are not equal");
    _LIT(KCompareIconErr, "Landmark icons are not equal");
    _LIT(KCompareDescErr, "Landmark descriptions are not equal");
    _LIT(KCompareNoPositionFieldsErr, "Landmark no positioning fields are not equal");
    _LIT(KComparePositionFieldIdErr, "Position field id's are not equal");
    _LIT(KComparePositionFieldErr, "Position fields are not equal");
    _LIT(KCompareNumOfCategoriesErr, "Number of categories is not equal");
    _LIT(KCompareSetOfCategoriesErr, "Set of categories is not equal");

    TInt sourceErr, targetErr;
    TPtrC sourceName;
    sourceErr=aSource.GetLandmarkName(sourceName);

    TPtrC targetName;
    targetErr=aTarget.GetLandmarkName(targetName);

    AssertTrueSecL(targetErr == sourceErr, KCompareNameErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceName.Compare(targetName) == KErrNone, KCompareNameErr);
        AssertTrueSecL(aSource.LandmarkId() == aTarget.LandmarkId(), KCompareIdErr);
        }

    TLocality sourceLoc, targetLoc;
    sourceErr = aSource.GetPosition(sourceLoc);
    targetErr = aTarget.GetPosition(targetLoc);
    AssertTrueSecL(sourceErr == targetErr, KComparePositionErr);
    if (!sourceErr)
        {
        // Difference between WINS  and WINSCW/TARGET
        // For WINS comparing TReal NaN == NaN evaluates to true
        // but for TARGET and WINSCW it evaluates to false
        TRealX vertAcc(sourceLoc.VerticalAccuracy());
        TRealX targetVertAcc(targetLoc.VerticalAccuracy());
        TRealX horAcc(sourceLoc.HorizontalAccuracy());
        TRealX targetHorAcc(targetLoc.HorizontalAccuracy());
        TRealX horAlt(sourceLoc.Altitude());
        TRealX targetHorAlt(targetLoc.Altitude());

        AssertTrueSecL(sourceLoc.Longitude() == targetLoc.Longitude(),KComparePositionErr1);
        AssertTrueSecL(sourceLoc.Latitude() == targetLoc.Latitude(), KComparePositionErr2);

        AssertTrueSecL((horAcc.IsNaN() && targetHorAcc.IsNaN()) || horAcc == targetHorAcc, KComparePositionErr3);
        AssertTrueSecL((vertAcc.IsNaN() && targetVertAcc.IsNaN()) || vertAcc == targetVertAcc, KComparePositionErr4);
        AssertTrueSecL((targetHorAlt.IsNaN() && horAlt.IsNaN()) || horAlt == targetHorAlt , KComparePositionErr5);
        }

    TReal32 sourceR, targetR;
    sourceErr = aSource.GetCoverageRadius(sourceR);
    targetErr = aTarget.GetCoverageRadius(targetR);
    AssertTrueSecL(sourceErr == targetErr, KCompareCoverageRadiusErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceR == targetR, KCompareCoverageRadiusErr);
        }

    TPtrC sourceMbmFileName, targetMbmFileName;
    TInt sourceIndex, targetIndex;
    TInt sourceMaskIndex, targetMaskIndex;

    sourceErr = aSource.GetIcon(sourceMbmFileName, sourceIndex, sourceMaskIndex);
    targetErr = aTarget.GetIcon(targetMbmFileName, targetIndex, targetMaskIndex);

    AssertTrueSecL(sourceErr == targetErr, KCompareIconErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceMbmFileName.Compare(targetMbmFileName) == KErrNone, KCompareIconErr);
        AssertTrueSecL(sourceIndex == targetIndex, KCompareIconErr);
        }

    TPtrC sourceDesc, targetDesc;
    sourceErr = aSource.GetLandmarkDescription(sourceDesc);
    targetErr = aTarget.GetLandmarkDescription(targetDesc);
    AssertTrueSecL(sourceErr == targetErr, KCompareDescErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceDesc.Compare(targetDesc) == KErrNone, KCompareDescErr);
        }

    AssertTrueSecL(aSource.NumOfAvailablePositionFields() == aTarget.NumOfAvailablePositionFields(), KCompareNoPositionFieldsErr);

    TPositionFieldId sourceFieldId = aSource.FirstPositionFieldId();
    TPositionFieldId targetFieldId = aTarget.FirstPositionFieldId();
    AssertTrueSecL(sourceFieldId == targetFieldId, KComparePositionFieldIdErr);

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue, targetValue;

        aSource.GetPositionField(sourceFieldId, sourceValue);
        aTarget.GetPositionField(targetFieldId, targetValue);

        AssertTrueSecL(sourceValue.Compare(targetValue) == KErrNone, KComparePositionFieldErr);

        sourceFieldId = aSource.NextPositionFieldId(sourceFieldId);
        targetFieldId = aTarget.NextPositionFieldId(targetFieldId);

        AssertTrueSecL(sourceFieldId == targetFieldId, KComparePositionFieldIdErr);
        }

    RArray<TPosLmItemId> sourceCategories;
    CleanupClosePushL(sourceCategories);
    RArray<TPosLmItemId> targetCategories;
    CleanupClosePushL(targetCategories);

    aSource.GetCategoriesL(sourceCategories);
    aTarget.GetCategoriesL(targetCategories);

    AssertTrueSecL(sourceCategories.Count() == targetCategories.Count(), KCompareNumOfCategoriesErr);

    for(TInt i=0; i<sourceCategories.Count(); i++)
        {
        TInt targetPos = targetCategories.Find( sourceCategories[i] );
        AssertTrueSecL( targetPos >= 0, KCompareSetOfCategoriesErr );
        }
    CleanupStack::PopAndDestroy(2, &sourceCategories);
    }

// ---------------------------------------------------------
// CPSYTesterTestProcedureBase::CompareCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CompareCategoriesL(
                        const TPosLmItemId& aSourceId,
                        const TPosLmItemId& aTargetId)
    {

    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(cm);

    CPosLandmarkCategory* source = cm->ReadCategoryLC(aSourceId);
    CPosLandmarkCategory* target = cm->ReadCategoryLC(aTargetId);

    CompareCategoriesL(*source, *target);


    CleanupStack::PopAndDestroy(3);
    }

// ---------------------------------------------------------
// CPSYTesterTestProcedureBase::CompareCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CompareCategoriesL(
                        const CPosLandmarkCategory& aSource,
                        const CPosLandmarkCategory& aTarget)
    {
    _LIT(KCompareCategoryNameErr, "Category names are not equal");
    _LIT(KCompareCategoryIdErr, "Category id's are not equal");
    _LIT(KCompareGlobalCategoryIdErr, "Category global id's are not equal");
    _LIT(KCompareCategoryIconErr, "Category icons are not equal");

    TInt sourceErr, targetErr;
    TPtrC sourceName, targetName;
    sourceErr = aSource.GetCategoryName(sourceName);
    targetErr = aTarget.GetCategoryName(targetName);

    AssertTrueSecL(sourceErr == targetErr, KCompareCategoryNameErr);
    AssertTrueSecL(sourceName.Compare(targetName) == KErrNone, KCompareCategoryNameErr);

    AssertTrueSecL(aSource.CategoryId() == aTarget.CategoryId(), KCompareCategoryIdErr);
    AssertTrueSecL(aSource.GlobalCategory() == aTarget.GlobalCategory(), KCompareGlobalCategoryIdErr);

    TPtrC sourceMbmFileName, targetMbmFileName;
    TInt sourceIndex, targetIndex;
    TInt sourceMaskIndex, targetMaskIndex;

    sourceErr = aSource.GetIcon(sourceMbmFileName, sourceIndex, sourceMaskIndex);
    targetErr = aTarget.GetIcon(targetMbmFileName, targetIndex, targetMaskIndex);

    AssertTrueSecL(sourceErr == targetErr, KCompareCategoryIconErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceMbmFileName.Compare(targetMbmFileName) == KErrNone, KCompareCategoryIconErr);
        AssertTrueSecL(sourceIndex == targetIndex, KCompareCategoryIconErr);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CompareXMLLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CompareXMLLandmarksL(const CPosLandmark& aSource, const CPosLandmark& aTarget, TBool aDebug)
    {
    _LIT(KComparePositionErr, "Landmark positions are not equal");
    _LIT(KComparePositionErr1, "Landmark longitude are not equal");
    _LIT(KComparePositionErr2, "Landmark latitude are not equal");
    _LIT(KComparePositionErr3, "Landmark horizontal accuracy are not equal");
    _LIT(KComparePositionErr4, "Landmark vertical accuracy are not equal");
    _LIT(KComparePositionErr5, "Landmark altitude are not equal");
    _LIT(KCompareNameErr, "Landmark names are not equal, src %S, trg %S");
    _LIT(KCompareCoverageRadiusErr, "Landmark coverage radiuses are not equal");
    _LIT(KCompareDescErr, "Landmark descriptions are not equal");
    _LIT(KCompareNoPositionFieldsErr, "ERROR: Landmark nr of positioning fields are not equal");
    _LIT(KComparePositionFieldIdErr, "ERROR: Position field id's are not equal, src '%d', trg '%d'");
    _LIT(KComparePositionFieldErr, "ERROR: Position fields are not equal, src '%S', trg '%S'");

    if (aDebug) iLog->Log(_L("CompareXMLLandmarksL"));

    TInt errorsFound = KErrNone;

    TInt sourceErr, targetErr;
    TPtrC sourceName;
    sourceErr=aSource.GetLandmarkName(sourceName);
    //iLog->Log(sourceName);

    TPtrC targetName;
    targetErr=aTarget.GetLandmarkName(targetName);
    //iLog->Log(targetName);

    AssertTrueSecL(targetErr == sourceErr, KCompareNameErr);
    if (!sourceErr)
        {
        if (sourceName.Compare(targetName) != KErrNone)
            {
            iLog->Log(KCompareNameErr, &sourceName, &targetName);
            }
        }

    TLocality sourceLoc, targetLoc;
    sourceErr = aSource.GetPosition(sourceLoc);
    targetErr = aTarget.GetPosition(targetLoc);
    AssertTrueSecL(sourceErr == targetErr, KComparePositionErr);
    if (!sourceErr)
        {
        // Difference between WINS  and WINSCW/TARGET
        // For WINS comparing TReal NaN == NaN evaluates to true
        // but for TARGET and WINSCW it evaluates to false
        TRealX vertAcc(sourceLoc.VerticalAccuracy());
        TRealX targetVertAcc(targetLoc.VerticalAccuracy());
        TRealX horAcc(sourceLoc.HorizontalAccuracy());
        TRealX targetHorAcc(targetLoc.HorizontalAccuracy());
        TRealX horAlt(sourceLoc.Altitude());
        TRealX targetHorAlt(targetLoc.Altitude());

        AssertTrueSecL(sourceLoc.Longitude() == targetLoc.Longitude(),KComparePositionErr1);
        AssertTrueSecL(sourceLoc.Latitude() == targetLoc.Latitude(), KComparePositionErr2);

        AssertTrueSecL((horAcc.IsNaN() && targetHorAcc.IsNaN()) || horAcc == targetHorAcc, KComparePositionErr3);
        AssertTrueSecL((vertAcc.IsNaN() && targetVertAcc.IsNaN()) || vertAcc == targetVertAcc, KComparePositionErr4);
        AssertTrueSecL((targetHorAlt.IsNaN() && horAlt.IsNaN()) || horAlt == targetHorAlt , KComparePositionErr5);
        }

    TReal32 sourceR, targetR;
    sourceErr = aSource.GetCoverageRadius(sourceR);
    targetErr = aTarget.GetCoverageRadius(targetR);
    AssertTrueSecL(sourceErr == targetErr, KCompareCoverageRadiusErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceR == targetR, KCompareCoverageRadiusErr);
        }
    TPtrC sourceDesc, targetDesc;
    sourceErr = aSource.GetLandmarkDescription(sourceDesc);
    targetErr = aTarget.GetLandmarkDescription(targetDesc);
    AssertTrueSecL(sourceErr == targetErr, KCompareDescErr);
    if (!sourceErr)
        {
        AssertTrueSecL(sourceDesc.Compare(targetDesc) == KErrNone, KCompareDescErr);
        }

    if (aSource.NumOfAvailablePositionFields() != aTarget.NumOfAvailablePositionFields())
        {
        iLog->Log(KCompareNoPositionFieldsErr);
        errorsFound++;
        //PrintLandmarkFieldsL(aSource);
        //PrintLandmarkFieldsL(aTarget);
        }

    TPositionFieldId sourceFieldId = aSource.FirstPositionFieldId();
    TPositionFieldId targetFieldId = aTarget.FirstPositionFieldId();

    if (sourceFieldId != targetFieldId)
        {
        iLog->Log(KComparePositionFieldIdErr);
        errorsFound++;
        }

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue, targetValue;

        HBufC* buf1 = NULL;
        HBufC* buf2 = NULL;

        aSource.GetPositionField(sourceFieldId, sourceValue);
        aTarget.GetPositionField(sourceFieldId, targetValue);

        // Remove // from string starting with //
        // The encoder adds "//" to medialinks with unknown mimetype
        if (sourceValue.Find(_L("//")) == 0)
            {
            TPtrC sourceValue2 = sourceValue.Mid(2);
            buf1 = sourceValue2.Alloc();
            }
        else buf1 = sourceValue.Alloc();
        CleanupStack::PushL(buf1);

        // Remove // from string starting with //
        // The encoder adds "//" to medialinks with unknown mimetype
        if (targetValue.Find(_L("//")) == 0)
            {
            TPtrC targetValue2 = targetValue.Mid(2);
            buf2 = targetValue2.Alloc();
            }
        else buf2 = targetValue.Alloc();
        CleanupStack::PushL(buf2);

        // Check if strings are equal
       if (buf1->Des().Compare(buf2->Des()))
            {
            iLog->Log(KComparePositionFieldErr, sourceFieldId, &sourceValue, &targetValue);
            errorsFound++;
            }

        CleanupStack::PopAndDestroy(buf2);
        CleanupStack::PopAndDestroy(buf1);

        sourceFieldId = aSource.NextPositionFieldId(sourceFieldId);
        }

    return errorsFound;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CompareXMLCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CompareXMLCategoriesL(
    const CPosLandmarkCategory& aSource,
    const CPosLandmarkCategory& aTarget, TBool /*aDebug*/)
    {
    _LIT(KCompareCategoryNameErr1, "ERROR: Category names not correct found");
    _LIT(KCompareCategoryNameErr2, "ERROR: Category names are not equal");
    TInt errorsFound = KErrNone;
    iLog->Log(_L("CompareXMLCategoriesL"));

    TInt sourceErr, targetErr;
    TPtrC sourceName, targetName;
    sourceErr = aSource.GetCategoryName(sourceName);
    targetErr = aTarget.GetCategoryName(targetName);

    AssertTrueSecL(sourceErr == targetErr, KCompareCategoryNameErr1);
    iLog->Log(_L("Source %S, target %S"), &sourceName, &targetName);

    if (sourceName.Compare(targetName) != KErrNone)
        {
        iLog->Log(KCompareCategoryNameErr2);
        errorsFound++;
        }

    if (aSource.GlobalCategory() != aTarget.GlobalCategory())
        {
        iLog->Log(KCompareCategoryNameErr2);
        errorsFound++;
        }

    iLog->Log(_L("CompareXMLCategoriesL Done"));
    return errorsFound;
    }


// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AssertTrueSecL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::AssertTrueSecL(TBool aCondition, const TDesC& aErrorMsg, TInt aErrorCode)
    {
  	if (!aCondition)
		{
		TBuf<KBufSize> buf;
		buf.Format(aErrorMsg, aErrorCode);
        // Necessary to separate the case when aErrorCode==KErrNone

        if (aErrorCode == KErrNone)
            {
            iLog->Log(buf);
            User::Leave(KErrNone);

            }
        else
            {
    	    iLog->Log(buf);
    		User::Leave(aErrorCode);
            }
		}
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CheckDefaultDbExitsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CheckDefaultDbExistL()
    {
    _LIT(KDefaultDbMissingErr,  "Default landmarks db is missing, open returns %d");

    RFile file;
    TInt err = file.Open(iFileSession, KDefaultDb, EFileRead);
    file.Close();
    AssertTrueSecL(err==KErrNone, KDefaultDbMissingErr, err);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::BackupDbFileL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::BackupDbFileL()
    {

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    HBufC* defaultDbUri = manager->DefaultDatabaseUriLC();
    TBuf<255> buf;
    // Extract URI except characters ".ldb"
    buf.Append(defaultDbUri->Left(defaultDbUri->Length()-4));
    buf.Append(_L("COPY.LDB"));
    // Delete in case it already exist
    TRAPD(err, manager->DeleteDatabaseL(buf));
    if (err != KErrNone && err != KErrNotFound) iLog->Log(_L("Could not delete file"));
    manager->CopyDatabaseL(*defaultDbUri, buf);

    CleanupStack::PopAndDestroy(defaultDbUri);
    CleanupStack::PopAndDestroy(manager);
    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RestoreDbFileL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::RestoreDbFileL()
    {


    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    HBufC* defaultDbUri = manager->DefaultDatabaseUriLC();
    TBuf<255> buf;
    // Extract URI except characters ".ldb"
    buf.Append(defaultDbUri->Left(defaultDbUri->Length()-4));
    buf.Append(_L("COPY.LDB"));
    // Delete in case it already exist
    TRAPD(err, manager->DeleteDatabaseL(*defaultDbUri));
    if (err != KErrNone && err != KErrNotFound) iLog->Log(_L("Could not delete file"));
    manager->CopyDatabaseL(buf, *defaultDbUri);

    CleanupStack::PopAndDestroy(defaultDbUri);
    CleanupStack::PopAndDestroy(manager);
    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CopyDbFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//


void CLandmarkTestProcedureBase::CopyDbFileL(const TDesC& aFileName)
    {
    _LIT(KTestPath, "c:\\system\\test\\testdata\\");

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;

    srcPath.Append(KTestPath);
    srcPath.Append(aFileName);


    TInt err = fileMan->Copy(srcPath, KLmFullDefaultDbPath, CFileMan::EOverWrite);
    if (err != KErrNone)
    	iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CopyTestDbFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CopyTestDbFileL(const TDesC& aFileName)
    {
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);

    TBuf<150> srcPath;
    TBuf<150> destPath;

    srcPath.Append(KTestPath);
    srcPath.Append(aFileName);

    destPath.Append(KLmDbPath);
    destPath.Append(aFileName);

    TInt err = fileMan->Copy(srcPath, destPath, CFileMan::EOverWrite);
    if (err != KErrNone) iLog->Log(_L("Error when copying file"));

    CleanupStack::PopAndDestroy(fileMan);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::UseGeneratedDbFile
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::UseGeneratedDbFileL()
    {
    CopyDbFileL(KDefaultDb);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);

    // This db must exist
    manager->SetDefaultDatabaseUriL(KDefaultDb);
    CleanupStack::PopAndDestroy(manager);

    CPosLandmarkDatabase* db = CPosLandmarkDatabase::OpenL(KDefaultDb);
    if ( db->IsInitializingNeeded() )
        {
        TRAPD( err, ExecuteAndDeleteLD( db->InitializeL() ) );
        AssertTrueSecL( err == KErrNone, _L("Init db failed"));
        }

    return db;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::UseCompositeLandmarksDbFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::UseCompositeLandmarksDbFileL()
    {
    iLog->Log(_L("UseCompositeLandmarksDbFileL"));


    _LIT(KDbFile, "compositeeposlm.ldb");
    CopyTestDbFileL(KDbFile);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    // This db must exist
    manager->SetDefaultDatabaseUriL(KDbFile);
    CleanupStack::PopAndDestroy(manager);
    return CPosLandmarkDatabase::OpenL(KDbFile);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::UseEmptyDbFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::UseEmptyDbFileL()
    {

    CopyTestDbFileL(KLandmarksEmptyDb);

    CPosLmDatabaseManager* manager = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(manager);
    // This db must exist
    manager->SetDefaultDatabaseUriL(KLandmarksEmptyDb);
    CleanupStack::PopAndDestroy(manager);

    return CPosLandmarkDatabase::OpenL(KLandmarksEmptyDb);

    }


// ---------------------------------------------------------
// CPosLandmarkDatabase::UseGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLandmarkDatabase* CLandmarkTestProcedureBase::UseGlobalCategoriesL()
    {
    _LIT(KGlobalCategoryResFile, "c:\\system\\test\\testdata\\EPOSLMGLOBALCATEGORIES.R01");

    _LIT(KGlobalCategoryResPath, "c:\\resource\\");

    iLog->Log(_L("UseGlobalCategoriesL"));

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);
    TInt err = KErrNone;
    //check if landmark is flashed
    TInt Value=BaflUtils::FileExists(iFileSession, KGlobalCategoryResFileROM);
    // if (!BaflUtils::FileExists(fs, KGlobalCategoryResFileROM))
    if(!Value)
        {
         iLog->Log(_L("Landmark is NOT flashed, rename global categories"));
        //Landmark is not flashed rename the file before copy a own defiend file.
        //since landmark is not flashed the file should exist hence leaving if it is not found!

         err = fileMan->Rename(KGlobalCategoryResFileWINS, KGlobalCategoryResFileCOPY, CFileMan::EOverWrite);
         if (err != KErrNone)
            {
            TBuf<100> buf;
            buf.Format(_L("ErrCode: from fileMan->Rename %d"),err);
            iLog->Log(buf);
            }

        }
     else
        {
        iLog->Log(_L("Landmark is flashed, copy global categories"));
        }

    // Recurse copy -> Create directory if it does not exist and overwrite file if it exists
    err = fileMan->Copy(KGlobalCategoryResFile, KGlobalCategoryResPath, CFileMan::EOverWrite);
    if (err != KErrNone)
        {
        TBuf<100> buf;
        buf.Format(_L("ErrCode: from fileMan->Copy %d"),err);
        iLog->Log(buf);
        }

    CleanupStack::PopAndDestroy(fileMan);

    return CPosLandmarkDatabase::OpenL();
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RemoveGlobalCategoriesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RemoveGlobalCategoriesL()
    {

    _LIT(KGlobalCategoryResFile, "c:\\resource\\EPOSLMGLOBALCATEGORIES.R01");

    iLog->Log(_L("RemoveGlobalCategoriesL"));

    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);
    TInt err = KErrNone;

    //check if the global category file exist in rom
    //if landmarks is flushed it is just okej to remove
    //any global gategory files in on the c drive.
    if (BaflUtils::FileExists(iFileSession, KGlobalCategoryResFileROM))
       {
        iLog->Log(_L("Landmark is flashed, delete global categories"));
        //in this case it is same just to remove it form c if it exists:
        if (BaflUtils::FileExists(iFileSession, KGlobalCategoryResFile))
            {
            err = fileMan->Delete(KGlobalCategoryResFile);
            if (err != KErrNone)
                {
                TBuf<100> buf;
                buf.Format(_L("ErrCode: from fileMan->Delete %d"),err);
                iLog->Log(buf);
                }
            }
        }
    else
        {
        iLog->Log(_L("Landmark is NOT flashed, delete global categories and rename the old file"));
        //If a copy exist this should be used, but if it does not
        //exist the file is not removed since it is hard to know
        //if anything has gone wrong.
        if(BaflUtils::FileExists(iFileSession, KGlobalCategoryResFileCOPY))
            {
            //first delete the used file, if it exist
            if (BaflUtils::FileExists(iFileSession, KGlobalCategoryResFile))
                {

                err = fileMan->Delete(KGlobalCategoryResFile);
                if (err != KErrNone)
                    {
                    TBuf<100> buf;
                    buf.Format(_L("ErrCode: from fileMan->Delete %d"),err);
                    iLog->Log(buf);
                    }
                }
            //Rename the copy file

            err = fileMan->Rename(KGlobalCategoryResFileCOPY, KGlobalCategoryResFile, CFileMan::EOverWrite);
            if (err != KErrNone)
                {
                TBuf<100> buf;
                buf.Format(_L("ErrCode: from fileMan->Rename %d"),err);
                iLog->Log(buf);
                }
            }
        }
     iLog->Log(_L("RemoveGlobalCategoriesL Done in CLandmarkTestProcedureBase"));
    CleanupStack::PopAndDestroy(fileMan);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::Trace
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::Trace(TBuf8<KMsgBufSize> aMsg)
    {
     _LIT(KFileTrace, "c:\\LandmarksTestsTrace.txt");
    _LIT8(KEnd,"\r\n");

    RFs fs;
    fs.Connect();

    RFile file;
    TInt err = file.Open(fs, KFileTrace, EFileWrite);

    if ( err )
		{
        err = file.Create(fs, KFileTrace, EFileWrite);
		}

    if ( !err )
        {
        TInt pos = 0;
        file.Seek( ESeekEnd, pos );
        file.Write( aMsg );
        file.Write( KEnd() );
        }

    file.Close();
    fs.Close();
	}

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AppendSearchResultsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CLandmarkTestProcedureBase::AppendSearchResultsL()
    {
    if (iSearchResults.Count() == 0 )
        {
        _LIT(KFileTestValues, "c:\\system\\test\\testdata\\LmDbSearchResult.txt");

        RFile file;
        User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));
        CleanupClosePushL(file);

        TBuf<KMaxLineLength> line;
        while (ReadLn(file, line) != KErrEof)
            {
            TLex lex(line);
            if (lex.Peek() != '#' && lex.Peek() != '\t')
                {
                CSearchResult* searchResult = CSearchResult::NewL(line);
                CleanupStack::PushL(searchResult);

                User::LeaveIfError(iSearchResults.Append(searchResult));
                CleanupStack::Pop(searchResult);
                }
            }
        CleanupStack::PopAndDestroy(&file);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AppendAreaSearchResultsL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CLandmarkTestProcedureBase::AppendAreaSearchResultsL()
    {
    if (iSearchResults.Count() == 0)
        {
        _LIT(KFileTestValues, "c:\\system\\test\\testdata\\lmDbAreaSearchResult.txt");

        RFile file;
        User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));
        CleanupClosePushL(file);

        TBuf<KMaxLineLength> line;
        while (ReadLn(file, line) != KErrEof)
            {
            TLex lex(line);
            if (lex.Peek() != '#' && lex.Peek() != '\t')
                {
                CAreaSearchResult* searchResult = CAreaSearchResult::NewL(line);
                CleanupStack::PushL(searchResult);

                User::LeaveIfError(iAreaSearchResults.Append(searchResult));
                CleanupStack::Pop(searchResult);
                }
            }
        CleanupStack::PopAndDestroy(&file);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AppendCategoryLandmarkSearchResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CLandmarkTestProcedureBase::AppendCategoryLandmarkSearchResultsL()
    {
    if (iCategoryLandmarkSearchResults.Count() == 0 )
        {
        _LIT(KFileTestValues, "c:\\system\\test\\testdata\\LmDbCatLandmarkSearchResult.txt");

        RFile file;
        TInt err = file.Open(iFileSession, KFileTestValues,  EFileRead);
        AssertTrueSecL( err == KErrNone, _L("AppendCategoryLandmarkSearchResultsL file error"));
        CleanupClosePushL(file);

        TBuf<KMaxLineLength> line;
        while (ReadLn(file, line) != KErrEof)
            {
            TLex lex(line);
            if (lex.Peek() != '#' && lex.Peek() != '\t')
                {
                CCategoryLandmarkSearchResult* oneLineResult = CCategoryLandmarkSearchResult::NewL(line);
                CleanupStack::PushL(oneLineResult);

                User::LeaveIfError(iCategoryLandmarkSearchResults.Append(oneLineResult));
                CleanupStack::Pop(oneLineResult);

                }
            }

        CleanupStack::PopAndDestroy(&file);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AppendCategorySearchResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CLandmarkTestProcedureBase::AppendCategorySearchResultsL()
    {
    if (iCategorySearchResults.Count() == 0 )
        {
        _LIT(KFileTestValues, "c:\\system\\test\\testdata\\LmDbCategorySearchResult.txt");

        RFile file;
        User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));
        CleanupClosePushL(file);


        TBuf<KMaxLineLength> line;
        while (ReadLn(file, line) != KErrEof)
            {
            TLex lex(line);
            if (lex.Peek() != '#' && lex.Peek() != '\t')
                {
                CCategorySearchResult* oneLineResult = CCategorySearchResult::NewL(line);
                CleanupStack::PushL(oneLineResult);

                User::LeaveIfError(iCategorySearchResults.Append(oneLineResult));
                CleanupStack::Pop(oneLineResult);
                //iLog->Log(line);
                }
            }

        CleanupStack::PopAndDestroy(&file);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::SeachResults
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
const RPointerArray<CSearchResult>& CLandmarkTestProcedureBase::SearchResults()
    {
    return iSearchResults;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::AreaSeachResults
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
const RPointerArray<CAreaSearchResult>& CLandmarkTestProcedureBase::AreaSearchResults()
    {
    return iAreaSearchResults;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::ReadLn
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::ReadLn(RFile& aFile, TDes& aDes)
    {
    aDes = KNullDesC;
    TBuf8<KMaxLineLength> line;

	TInt err = aFile.Read(line);
    if (line.Length() == 0)
        {
        return KErrEof;
        }
	if (err)
		{
		return err;
		}

    // Find line feed
    TInt i = 0;
	TLex8 fileData;
	fileData.Assign(line);
	while (fileData.Peek() != '\n' && !(fileData.Peek()).Eos())
		{
        fileData.Inc();
        if (++i == KMaxLineLength)
            {
            // In case no carriage return or end of string characters are present.
            break;
            }
        }

    // Update file position
    TInt diff = -(line.Length() - i - 1);
    aFile.Seek(ESeekCurrent, diff);

    // Convert to 16-bit
    line.SetLength(i);
    aDes.Copy(line);

    return KErrNone;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CompareL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CompareL(
    const TDesC& aSourceFileName,
    const TDesC& aTargetFileName)
    {
    _LIT(KCompareErr, "Compared file %S is not equal to %S");
    _LIT(KLineErr, "Line %S is different to line %S");

    iLog->Log(_L("CompareL: source file '%S' with target file '%S'"), &aSourceFileName, &aTargetFileName );

    RFile targetFile, sourceFile;

    TInt err = KErrNone;
    err = sourceFile.Open(iFileSession, aSourceFileName, EFileRead);
    if ( err )
        {
        iLog->Log(_L("Failed to open source file '%S', err % d"), &aSourceFileName, err );
        User::Leave( err );
        }
    CleanupClosePushL(sourceFile);

    err = targetFile.Open(iFileSession, aTargetFileName, EFileRead);
    if ( err )
        {
        iLog->Log(_L("Failed to open target file '%S', err % d"), &aTargetFileName, err );
        User::Leave( err );
        }
    CleanupClosePushL(targetFile);

    TBuf<255> sourceLine;
    TBuf<255> targetLine;

    while (ReadLn(sourceFile, sourceLine) != KErrEof)
        {
        ReadLn(targetFile, targetLine);

        if (sourceLine.Compare(targetLine) != 0)
            {
            TBuf<255> info;
            info.Format(KCompareErr, &aSourceFileName, &aTargetFileName);
            iLog->Log(info);

            info.Format(KLineErr, &targetLine, &sourceLine);
            iLog->Log(info);

            err = KErrGeneral;
            break;
            }
        }

    CleanupStack::PopAndDestroy(&targetFile);
    CleanupStack::PopAndDestroy(&sourceFile);
    return err;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CompareL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CompareL(const TDesC& aFileName)
    {
    _LIT(KTargetPath, "c:\\");

    _LIT(KSourcePath, "c:\\system\\test\\testdata\\");

    TBuf<100> target;
    target.Append(KTargetPath);
    target.Append(aFileName);

    TBuf<100> source;
    source.Append(KSourcePath);
    source.Append(aFileName);

    return CompareL(source, target);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::PrintLandmark
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::PrintLandmark(const CPosLandmark& aLandmark, TDes& aPrint)
    {


    TChar del(',');

    _LIT(KIntInfo, "%d");
    TBuf<KMaxFieldLength> info;
    TPosLmItemId id = aLandmark.LandmarkId();
    if (id != 0)
        {
        info.Format(KIntInfo, id);
        aPrint.Append(info);
        }
    aPrint.Append(del);

    RArray<TPosLmItemId> lmcats;
    // How to do this in test code, with non leaving function? // fran
    CleanupClosePushL(lmcats);
    TRAPD(error, aLandmark.GetCategoriesL(lmcats));

    if (error == KErrNone)
        {
        _LIT(KStart, "{");
        _LIT(KStop, "}");
        aPrint.Append(KStart);
        for (TInt i=0; i<lmcats.Count(); i++)
            {
            info.Format(KIntInfo, lmcats[i]);
            aPrint.Append(info);
            aPrint.Append(del);
            }
        aPrint.Append(KStop);
        }
    CleanupStack::PopAndDestroy(&lmcats);

    TPtrC name;
    aLandmark.GetLandmarkName(name);
    aPrint.Append(name);
    aPrint.Append(del);

    TInt err;
    _LIT(KRealInfo, "%f");
    TReal32 coverageR;

    err = aLandmark.GetCoverageRadius(coverageR);
    if (!err)
        {
        info.Format(KRealInfo, coverageR);
        aPrint.Append(info);
        }

    aPrint.Append(del);
    TPtrC desc;
    err = aLandmark.GetLandmarkDescription(desc);
    if (!err)
        {
        aPrint.Append(desc);
        }

    aPrint.Append(del);

    TPtrC iconName;
    TInt iconIndex;
    TInt iconMaskIndex;
    err = aLandmark.GetIcon(iconName, iconIndex, iconMaskIndex);
    if (!err)
        {
        aPrint.Append(iconName);
        aPrint.Append(del);
        info.Format(KIntInfo, iconIndex);
        aPrint.Append(info);
        }

    aPrint.Append(del);
    TPtrC street;
    err = aLandmark.GetPositionField(EPositionFieldStreet, street);
    if (!err)
        {
        aPrint.Append(street);
        }

    aPrint.Append(del);
    TPtrC postalCode;
    err = aLandmark.GetPositionField(EPositionFieldPostalCode, postalCode);
    if (!err)
        {
        aPrint.Append(postalCode);
        }

    aPrint.Append(del);
    TPtrC city;
    err = aLandmark.GetPositionField(EPositionFieldCity, city);
    if (!err)
        {
        aPrint.Append(city);
        }

    aPrint.Append(del);
    TPtrC locationName;
    err = aLandmark.GetPositionField(EPositionFieldLocationName, locationName);
    if (!err)
        {
        aPrint.Append(locationName);
        }

    aPrint.Append(del);
    TPtrC country;
    err = aLandmark.GetPositionField(EPositionFieldCountry, country);
    if (!err)
        {
        aPrint.Append(country);
        }

    aPrint.Append(del);
    TPtrC mediaLink;
    err = aLandmark.GetPositionField(EPositionFieldMediaLinksStart, mediaLink);
    if (!err)
        {
        aPrint.Append(mediaLink);
        }

    aPrint.Append(del);
    TUint16 tmp = (TUint16)EPositionFieldMediaLinksStart;
    TPtrC mediaLink2;
    err = aLandmark.GetPositionField(++tmp, mediaLink2);
    if (!err)
        {
        aPrint.Append(mediaLink2);
        }

    aPrint.Append(del);
    TLocality loc;
    err = aLandmark.GetPosition(loc);
    if (!err)
        {
        info.Format(KRealInfo, loc.Latitude());
        aPrint.Append(info);
        aPrint.Append(del);
        info.Format(KRealInfo, loc.Longitude());
        aPrint.Append(info);
        aPrint.Append(del);
        if (!Math::IsNaN(loc.HorizontalAccuracy()))
            {
            info.Format(KRealInfo, loc.HorizontalAccuracy());
            aPrint.Append(info);
            aPrint.Append(del);
            }
        if (!Math::IsNaN(loc.VerticalAccuracy()))
            {
            info.Format(KRealInfo, loc.VerticalAccuracy());
            aPrint.Append(info);
            aPrint.Append(del);
            }
        if (!Math::IsNaN(loc.Altitude()))
            {
            info.Format(KRealInfo, loc.Altitude());
            aPrint.Append(info);
            aPrint.Append(del);
            }
        }


    }

TInt InternalLandmarksSortL(const CPosLandmark& aSource, const CPosLandmark& aTarget)
    {
    TPtrC sourceName, targetName;

    User::LeaveIfError(aSource.GetLandmarkName(sourceName));
    User::LeaveIfError(aTarget.GetLandmarkName(targetName));

    return sourceName.CompareC(targetName);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::LandmarksSortL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::LandmarksSortL(RArray<TPosLmItemId>& aArray,
                                                const TPosLmSortPref& aSortPref)
    {
    RPointerArray<CPosLandmark> landmarks;
    CleanupClosePushL(landmarks);

    for (TInt i=0; i<aArray.Count(); i++)
        {
        CPosLandmark* lm = iDatabase->ReadLandmarkLC(aArray[i]);
        landmarks.Append(lm);
        CleanupStack::Pop(lm);
        }

    TLinearOrder<CPosLandmark> order(InternalLandmarksSortL);
    landmarks.Sort(order);

    aArray.Reset();

    if (aSortPref.SortOrder() == TPosLmSortPref::EAscending)
        {
        for (TInt j=0; j<landmarks.Count(); j++)
            {
            aArray.Append(landmarks[j]->LandmarkId());
            }
        }
    else
        {
        for (TInt k=landmarks.Count()-1; k>=0; k--)
            {
            aArray.Append(landmarks[k]->LandmarkId());
            }
        }
    landmarks.ResetAndDestroy();
    CleanupStack::PopAndDestroy(&landmarks);
    }


// ---------------------------------------------------------
// InternalCategorySortL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt InternalCategorySortL(const CPosLandmarkCategory& aSource, const CPosLandmarkCategory& aTarget)
    {
    TPtrC sourceName, targetName;

    User::LeaveIfError(aSource.GetCategoryName(sourceName));
    User::LeaveIfError(aTarget.GetCategoryName(targetName));

    return sourceName.CompareC(targetName);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CategoriesSortL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CategoriesSortL(RArray<TPosLmItemId>& aArray,
                               CPosLmCategoryManager::TCategorySortPref aSortPref)
    {
    RPointerArray<CPosLandmarkCategory> categories;
    CleanupClosePushL(categories);

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);

    for (TInt i=0; i<aArray.Count(); i++)
        {
        CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(aArray[i]);
        categories.Append(cat);
        CleanupStack::Pop(cat);
        }

    TLinearOrder<CPosLandmarkCategory> order(InternalCategorySortL);
    categories.Sort(order);

    aArray.Reset();

    if (aSortPref == CPosLmCategoryManager::ECategorySortOrderNameAscending)
        {
        for (TInt j=0; j<categories.Count(); j++)
            {
            aArray.Append(categories[j]->CategoryId());
            }
        }
    else
        {
        for (TInt k=categories.Count()-1; k>=0; k--)
            {
            aArray.Append(categories[k]->CategoryId());
            }
        }
    categories.ResetAndDestroy();
    CleanupStack::PopAndDestroy(categoryManager);
    CleanupStack::PopAndDestroy(&categories);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::LogToFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::LogToFileL(const TDesC& aFileName, TBuf8<KMsgBufSize> aMsg)
    {
    _LIT8(KEnd,"\r\n");

    BaflUtils::EnsurePathExistsL( iFileSession, aFileName );

    RFile file;
    TInt err = file.Open(iFileSession, aFileName, EFileWrite);
    if ( err )
		{
        err = file.Create(iFileSession, aFileName, EFileWrite);
		}

    if ( !err )
        {
        TInt pos = 0;
        file.Seek( ESeekEnd, pos );
        file.Write( aMsg );
        file.Write( KEnd );
        }

    file.Close();
	}

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RunAsyncOperationLD
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RunAsyncOperationLD(CPosLmOperation* aOperation)
    {
    CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(iLog, iUseLogFromThreadIsDisabled);
    CleanupStack::PushL(activeLmOperation);

    activeLmOperation->Start(aOperation);

    CActiveScheduler::Start();

    activeLmOperation->CheckOperationL();
    activeLmOperation->DeleteOperationD();

    CleanupStack::PopAndDestroy(activeLmOperation);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RunAsyncOperationLD
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RunAsyncOperationL(
    CPosLmOperation* aOperation)
    {
    CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(iLog, iUseLogFromThreadIsDisabled);
    CleanupStack::PushL(activeLmOperation);

    activeLmOperation->Start(aOperation);

    CActiveScheduler::Start();

    activeLmOperation->CheckOperationL();

    CleanupStack::PopAndDestroy(activeLmOperation);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RunAsyncOperationByWaitForReqL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RunAsyncOperationByWaitForReqL(
    CPosLmOperation* aOperation)
    {
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress = 0;

    while (status == KPosLmOperationNotComplete)
        {
        aOperation->NextStep(status, progress);
        User::WaitForRequest(status);
        }

    _LIT(KStatusError, "Executing CPosLmOperation by using User::WaitForRequest() failed with %d");
    AssertTrueSecL(status.Int() == KErrNone, KStatusError, status.Int());

    _LIT(KProgError, "Progress not 1.0 but %d when executing CPosLmOperation with User::WaitForRequest()");
    AssertTrueSecL(progress == 1.0, KProgError, (TInt) (progress * 100));
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RunAsyncOperationAndCancelLD
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RunAsyncOperationAndCancelLD(CPosLmOperation* aOperation)
    {
    CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(iLog, iUseLogFromThreadIsDisabled);
    CleanupStack::PushL(activeLmOperation);

    activeLmOperation->Start(aOperation);

    activeLmOperation->DeleteOperationD();

    CActiveScheduler::Start();

    CleanupStack::PopAndDestroy(activeLmOperation);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::RunAsyncOperationAndCancelInCallbackLD
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::RunAsyncOperationAndCancelInCallbackLD(
    CPosLmOperation* aOperation,
    TReal32 aThreshold)
    {
    if (aThreshold > 0)
        {
        CActiveLmOperation* activeLmOperation = new(ELeave) CActiveLmOperation(
            iLog, iUseLogFromThreadIsDisabled, aThreshold);
        CleanupStack::PushL(activeLmOperation);

        TCallBack callback(CancelTest, activeLmOperation);

        CPeriodic* periodicTimer = CPeriodic::NewL(CActive::EPriorityHigh);
        CleanupStack::PushL(periodicTimer);

        periodicTimer->Start(1, 1, callback);

        activeLmOperation->SetCancelTest();
        activeLmOperation->Start(aOperation);

        CActiveScheduler::Start();

        periodicTimer->Cancel();

        if (activeLmOperation->Operation())
            {
            // In case of no cancel
            activeLmOperation->DeleteOperationD();
            if (activeLmOperation->DeltaProgress() < 1 - aThreshold)
                {
                iLog->Log(_L("Test was never cancelled!"));
                }
            }

        CleanupStack::PopAndDestroy(2, activeLmOperation);
        }
    else
        {
        // Cancel immediately
        RunAsyncOperationAndCancelLD(aOperation);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::CancelTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CancelTest(TAny* aSelf)
    {
    CActiveLmOperation* activeLmOperation = static_cast<CActiveLmOperation*>(aSelf);

    if (activeLmOperation->Progress() >= activeLmOperation->Threshold())
        {
        activeLmOperation->DeleteOperationD();
        }

    return 0;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::DeleteFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::DeleteFileL(const TDesC& aFile)
    {
    TInt err = iFileSession.Delete(aFile);

    TBuf<512> buf;

    if (err == KErrNone)
        {
        _LIT(KDebugText, "DeleteFileL: Specified file '%S' has been deleted");
        buf.Format(KDebugText, &aFile, err);
        iLog->Log(buf);
        }
    else
        {
        _LIT(KErrText, "DeleteFileL: Specified file '%S' could not be deleted, error %d");
        buf.Format(KErrText, &aFile, err);
        iLog->Log(buf);
        switch ( err )
            {
            case KErrNotFound:
            case KErrPathNotFound:
                break;
            default:
                User::Leave(err);
                break;
            }
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::ServerAliveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::MakeSurePanicDebugFileExistsL()
    {
    _LIT(KFileName, "c:\\system\\bootdata\\ErrRd");
    _LIT(KFilePath, "c:\\system\\bootdata\\");

    TInt res = iFileSession.MkDirAll(KFilePath);
    if (res != KErrAlreadyExists && res != KErrNone)
        {
        iLog->Log(_L("Problem creating directory c:\\system\\bootdata\\"));
        User::Leave(res);
        }

    RFile panicDebugFile;
    res = panicDebugFile.Create(iFileSession, KFileName, EFileRead);
    CleanupClosePushL(panicDebugFile);

    if (res != KErrAlreadyExists && res != KErrNone)
        {
        iLog->Log(_L("Problem creating file c:\\system\\bootdata\\ErrRd"));
        User::Leave(res);
        }

    CleanupStack::PopAndDestroy(&panicDebugFile);
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::ServerAliveL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CLandmarkTestProcedureBase::ServerAlive(const TDesC& aServerName)
    {
	TFindServer find(aServerName);
    TFullName   name;
    if (find.Next(name) == KErrNone) // server found
		{
		TBuf<100> buf;
		buf.Append(_L("Found server: "));
		buf.Append(name);
		iLog->Log(buf);
		return ETrue;
		}
	else
		{
		return EFalse;
		}
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::ListLandmarkDbsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CDesCArray* CLandmarkTestProcedureBase::ListLandmarkDbsL()
    {
    CPosLmDatabaseManager* dbman = CPosLmDatabaseManager::NewL();
    CleanupStack::PushL(dbman);
    CDesCArray* dbs = dbman->ListDatabasesLC();
    CleanupStack::Pop(dbs);
    CleanupStack::PopAndDestroy(dbman);
    return dbs;
    }

// -----------------------------------------------------------------------------
// CActiveLmOperation::CActiveLmOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CActiveLmOperation::CActiveLmOperation(
    CStifLogger* aLog,
    TBool aUseLogFromThreadIsDisabled,
    TReal32 aThreshold)
: CActive(EPriorityNormal),
    iThreshold(aThreshold),
    iUseLogFromThreadIsDisabled(aUseLogFromThreadIsDisabled),
    iLog(aLog)
    {
    CActiveScheduler::Add(this);
    }


// Destructor
CActiveLmOperation::~CActiveLmOperation()
    {
    Cancel();

    }

// ---------------------------------------------------------
// CActiveLmOperation::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmOperation::Start(
    CPosLmOperation* aOperation,
    MProgressObserver* aObserver)
    {
    iOperation = aOperation;
    iObserver = aObserver;

    TTime start, stop;
    start.UniversalTime();
    iOperation->NextStep(iStatus, iPassedProgress);
    stop.UniversalTime();

    TInt64 usec = (stop.Int64() - start.Int64());

    TBuf8<KMsgBufSize> msg;
	_LIT8(KTrace, "First next step took %d µs");
	msg.Format(KTrace, usec);
    // Dont log if used from different threads
    // ex TP30,TP31
    // Log if step took more than 0.1 seconds
    if (!iUseLogFromThreadIsDisabled && usec > KFirstNextStepLogLimit)
        {
        iLog->Log(msg);
        }

    SetActive();
    }

// ---------------------------------------------------------
// CActiveLmOperation::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmOperation::RunL()
    {
    if (iPassedProgress < iLastProgress)
        {
        iAsyncProgressErr = ETrue;
        }

    iDeltaProgress = iPassedProgress - iLastProgress;
    iLastProgress = iPassedProgress;
    if (iObserver)
        {
        iObserver->NotifyProgress(iPassedProgress);
        }

    if (iStatus == KPosLmOperationNotComplete && iOperation)
        {
        if (iLastProgress == 1.0)
            {
            iPassedProgressErr = ETrue;
            }

        iOperation->NextStep(iStatus, iPassedProgress);

        if (iCancelTest)
            {
            User::After(1);
            }

        SetActive();
        }
    else
        {
        if (iStatus == KErrNone && iPassedProgress != 1.0)
            {
            iPassedProgressErr = ETrue;
            }

        iLastProgress = 0;
        CActiveScheduler::Stop();
        }


    }

// ---------------------------------------------------------
// CActiveLmOperation::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmOperation::DoCancel()
    {

    }

// ---------------------------------------------------------
// CActiveLmOperation::CheckOperationL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmOperation::CheckOperationL()
    {
    _LIT(KAsyncStatusErr, "Status incorrect in RunL = %d");
    _LIT(KAsyncProgressErr, "Progress not updated in RunL");
    _LIT(KPassedProgressErr, "Passed Progress is not updated correctly in RunL");

    if (iAsyncProgressErr)
        {
        iLog->Log(KAsyncProgressErr);
        User::Leave(KErrGeneral);
        }

    if (iPassedProgressErr)
        {
        iLog->Log(KPassedProgressErr);
        User::Leave(KErrGeneral);
        }

    if (iStatus != KErrNone)
        {
        if (!iUseLogFromThreadIsDisabled)
            {
            TBuf<100> info;
            info.Format(KAsyncStatusErr, iStatus.Int());
            iLog->Log(info);
            }
        User::Leave(iStatus.Int());
        }

    iAsyncProgressErr = iPassedProgressErr = EFalse;

    }

// ---------------------------------------------------------
// CActiveLmOperation::DeleteOperationD
//
// (other items were commented in a header).
// ---------------------------------------------------------
void CActiveLmOperation::DeleteOperationD()
    {
    delete iOperation;
    iOperation = NULL;
    }

// ---------------------------------------------------------
// CActiveLmOperation::Status
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CActiveLmOperation::Status() const
    {
    return iStatus.Int();
    }

// ---------------------------------------------------------
// CActiveLmOperation::Progress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal CActiveLmOperation::Progress() const
    {
    return iLastProgress;
    }

// ---------------------------------------------------------
// CActiveLmOperation::Operation
//
// (other items were commented in a header).
// ---------------------------------------------------------
CPosLmOperation* CActiveLmOperation::Operation()
    {
    return iOperation;
    }

//---------------------------------------------------------
// CActiveLmOperation::SetCancelTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmOperation::SetCancelTest()
    {
    iCancelTest = ETrue;
    }

//---------------------------------------------------------
// CActiveLmOperation::Threshold
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CActiveLmOperation::Threshold()
    {
    return iThreshold;
    }

//---------------------------------------------------------
// CActiveLmOperation::DeltaProgress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal32 CActiveLmOperation::DeltaProgress()
    {
    return iDeltaProgress;
    }

// <---------------- inner class COnTheFlyTester (utility class for testing multiDbSearch) ---------------->

// Constructor
CLandmarkTestProcedureBase::COnTheFlyTester::COnTheFlyTester() : CActive(CActive::EPriorityStandard)
    {
    CActiveScheduler::Add(this);
    }

// Desctructor
CLandmarkTestProcedureBase::COnTheFlyTester::~COnTheFlyTester()
    {
    delete iDisplayData;
    delete iOperation;
    Cancel();
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::COnTheFlyTester::NewLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CLandmarkTestProcedureBase::COnTheFlyTester* CLandmarkTestProcedureBase::COnTheFlyTester::NewLC()
    {
    CLandmarkTestProcedureBase::COnTheFlyTester* self = new (ELeave) CLandmarkTestProcedureBase::COnTheFlyTester;
    CleanupStack::PushL(self);
    return self;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::COnTheFlyTester::StartSearchCampaignL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::COnTheFlyTester::StartSearchCampaignL(
    CPosLmMultiDbSearch* aSearcher,
    const RIdArray& aExpectedMatches,
    CPosLmSearchCriteria* aSearchCriteria)
    {
    iSearcher = aSearcher;
    iExpectedMatches = &aExpectedMatches;

    // Set DisplayData
    delete iDisplayData;
    iDisplayData = NULL;
    iDisplayData = CPosLmDisplayData::NewL();
    CPosLmPartialReadParameters* partialReadParams = CPosLmPartialReadParameters::NewLC();
    partialReadParams->SetRequestedAttributes(CPosLandmark::ELandmarkName);
    iDisplayData->SetPartialReadParametersL(*partialReadParams);
    iSearcher->SetDisplayData(*iDisplayData);
    CleanupStack::PopAndDestroy(partialReadParams);

    // Start search
    TPosLmSortPref sortPref(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    delete iOperation;
    iOperation = NULL;
    iOperation = iSearcher->StartLandmarkSearchL(*aSearchCriteria, sortPref);
    iOperation->NextStep(iStatus, iProgress);
    SetActive();

    CActiveScheduler::Start();
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::COnTheFlyTester::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::COnTheFlyTester::RunL()
    {
    iErr = iStatus.Int();
    switch (iErr)
        {
        case KPosLmOperationNotComplete:
            VerifyOnTheFlyDataL();
            iOperation->NextStep(iStatus, iProgress);
            SetActive();
            break;

        case KErrNone:
            VerifyOnTheFlyDataL(ETrue);
            CActiveScheduler::Stop();
            break;

        default:
            User::Leave(iErr);
        }
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::COnTheFlyTester::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::COnTheFlyTester::DoCancel()
    {
    delete iOperation;
    iOperation = NULL;
    CActiveScheduler::Stop();
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::COnTheFlyTester::RunError
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::COnTheFlyTester::RunError(TInt aError)
    {
    iErr = aError;
    CActiveScheduler::Stop();
    return KErrNone;
    }

// ---------------------------------------------------------
// CLandmarkTestProcedureBase::COnTheFlyTester::VerifyOnTheFlyDataL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::COnTheFlyTester::VerifyOnTheFlyDataL(TBool aIsFinished)
    {
    // No errors should have occurred
    if (iSearcher->NumOfSearchErrors() != 0)
        {
        iBuf = _L("iSearcher->NumOfSearchErrors() != 0");
        User::Leave(KErrGeneral);
        }

    // Verify that number of found matches does not decrease
    TInt nrOfItems = iDisplayData->Count();
    if (nrOfItems < iNrOfOldItems)
        {
        iBuf = _L("nrOfItems < iNrOfOldItems");
        User::Leave(KErrGeneral);
        }
    iNrOfOldItems = nrOfItems;

    // Verify sort order
    for (TInt i = 0; i < nrOfItems; i++)
        {
        CPosLmDisplayItem& displayItem = iDisplayData->DisplayItem(i);
        if (displayItem.DisplayItemType() != CPosLmDisplayItem::ELandmarkItem)
            {
            iBuf = _L("Wrong displayItemType");
            User::Leave(KErrGeneral);
            }
        if (i < nrOfItems - 1)
            {
            TPtrC name1, name2;
            displayItem.Landmark().GetLandmarkName(name1);
            iDisplayData->DisplayItem(i + 1).Landmark().GetLandmarkName(name2);
            if (name1.CompareC(name2) > 0)
                {
                iBuf.Format(_L("%S is greater than %S"), &name1, &name2);
                User::Leave(KErrGeneral);
                }
            }
        }

    // When completed, the expected number of matches should have been found
    if (aIsFinished)
        {
        TInt totNrOfMatches = iSearcher->TotalNumOfMatches();
        TInt expectedNrOfHits = iExpectedMatches->Count();
        if (totNrOfMatches != expectedNrOfHits)
            {
            iBuf.Format(_L("Wrong number of landmarks returned when searching for landmarks, found: %d expected %d"), totNrOfMatches, expectedNrOfHits);
            User::Leave(KErrGeneral);
            }
        for (TInt i = 0; i < expectedNrOfHits; i++)
            {
            TInt found = iExpectedMatches->Find(iDisplayData->DisplayItem(i).Landmark().LandmarkId());
            if (found == KErrNotFound)
                {
                iBuf.Format(_L("Non-qualified landmark was found"));
                User::Leave(KErrNotFound);
                }
            TPtrC name;
            iDisplayData->DisplayItem(i).Landmark().GetLandmarkName(name);
            }
        }
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::ReadGlobalCategoriesFromResourceL(
	RArray<TUint>& aCatIds,
	RPointerArray<HBufC>& aCatNames,
	TInt aMaxNum )
    {
	_LIT(KGlobalCategoryResourceFile, "z:\\Resource\\eposlmglobalcategories.rsc");
	const TInt KPosLmGlCatResourceOffset = 1;

	TFileName filename(KGlobalCategoryResourceFile);
	BaflUtils::NearestLanguageFile( iFileSession, filename );

	CResourceFile* resFile = CResourceFile::NewLC( iFileSession, filename, 0, 0 );

	RResourceReader reader;
	reader.OpenLC( resFile, KPosLmGlCatResourceOffset );

	// Read the size of the icon path array.
	TInt size = reader.ReadInt8L();

	// read icon paths
	for (TInt i = 0; i < size; i++)
	    {
	    HBufC* path = reader.ReadHBufCL();
	    delete path;
	    }

	// read category data
	size = reader.ReadInt16L();
	if ( aMaxNum > 0 )
		{
		size = Min( size, aMaxNum );
		}

	for (TInt i = 0; i < size; i++)
	    {
	    TInt id = reader.ReadUint32L();
	    aCatIds.Append( id );
	    HBufC* name = reader.ReadHBufCL();
	    aCatNames.Append( name );

	    //iLog->Log(_L("Localized category: id: %d, name: '%S'"), id, name);

	    reader.ReadInt32L();
	    reader.ReadInt32L();
	    reader.ReadInt8L();
	    }
	CleanupStack::PopAndDestroy( &reader );
	CleanupStack::PopAndDestroy( resFile );
	}


// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::ResetAndDestroyHBufCArray( TAny* aPointer )
	{
	RPointerArray<HBufC>* array = ( RPointerArray<HBufC>* ) aPointer;
	array->ResetAndDestroy();
	}

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CLandmarkTestProcedureBase::CreateCorrectXmlFileL( const TDesC& aSampleFile, TFileName& aFileName )
	{
	_LIT( KTargetFolder, "c:\\documents\\" );
    _LIT( KNewFileSuffix, ".localized" );

    iLog->Log(_L("Making localized XML file out of '%S'"), &aSampleFile );

    RFileReadStream source;
    TInt err = source.Open( iFileSession, aSampleFile, EFileRead );
    if ( err  )
        {
        iLog->Log(_L("Failed to open source file '%S', err %d"), &aSampleFile, err );
        User::Leave( err );
        }
    CleanupClosePushL( source );

	TParse parse;
	parse.Set( aSampleFile, 0, 0 );
	aFileName = KTargetFolder;
	aFileName.Append( parse.Name() );
    aFileName.Append( KNewFileSuffix );
    aFileName.Append( parse.Ext() );

    RFile target;
	err = target.Replace( iFileSession, aFileName, EFileRead );
	if ( err  )
		{
		iLog->Log(_L("Failed to create target file '%S', err %d"), &aFileName, err );
		User::Leave( err );
		}
	CleanupClosePushL( target );

	// Replace Global Category macros with correct strings

	RArray<TUint> globalIds;
	RPointerArray<HBufC> names;
	CleanupStack::PushL( TCleanupItem( CLandmarkTestProcedureBase::ResetAndDestroyHBufCArray, &names ) );
	ReadGlobalCategoriesFromResourceL( globalIds, names );

	const TChar KIdStart = '{';
	const TChar KIdEnd = '}';
	//category name length should be <= 124
	_LIT( KGlobalCategoryMacro, "__GLOBAL_CAT_{" );

	TBuf8<1024> line8;
	while ( !err )
        {
        TRAP( err, source.ReadL( line8, TChar('\n') ) );
        if ( !err )
        	{
	        HBufC* line16 = CnvUtfConverter::ConvertToUnicodeFromUtf8L( line8 );
	        CleanupStack::PushL( line16 );
	        TPtr line( line16->Des() );

	        TInt macroStart = line.Find( KGlobalCategoryMacro );
	        if ( macroStart != KErrNotFound )
	            {
	            // find global categoryid in the macro
	            TInt idStart = line.Locate( KIdStart );
	            TInt idEnd = line.Locate( KIdEnd );

	            TLex idStr( line.Mid( idStart + 1, idEnd - idStart - 1 ) );

	            TUint globalId = 0;
	            idStr.Val( globalId );

	            // find localized string for the category
	            TInt idIndex = globalIds.Find( globalId );

	    	    // put it to the file instead of macro
	            if ( idIndex >= 0 )
	            	{
	            		HBufC* des = EscapeStringToXmlLC(*(names[idIndex]));
    					
    					
		            	if(des->Length() > (idEnd - macroStart + 1))
		            	{
		            		TInt diffrence = des->Length() - (idEnd - macroStart + 1);
		            		HBufC* diffStr = HBufC::NewLC(diffrence * 2);
		            		TPtr ptr(diffStr->Des());
		            		TInt i=0;
		            		for(i=0; i<diffrence; i++)
		            		{
		            			ptr.Append(' ');
		            		}
		            		line16->ReAllocL(line.MaxLength() + i);
		            		TPtr newPtr(line16->Des());
		            		newPtr.Insert(idEnd, ptr);
		            		newPtr.Replace( macroStart, (idEnd + i) - macroStart + 1, des->Des() );
		            		HBufC8* newLine8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( newPtr );
			        		target.Write( *newLine8 );
			        		delete newLine8;
		            		CleanupStack::PopAndDestroy( diffStr );
		            	}
		            	else
		            	{
		            		line.Replace( macroStart, idEnd - macroStart + 1, des->Des() );
		            		HBufC8* newLine8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( line );
			        		target.Write( *newLine8 );
			        		delete newLine8;
		            	}
	            		CleanupStack::PopAndDestroy(des);
	            	}


	            }
	        else
	        	{
	        	target.Write( line8 );
	        	}

	        CleanupStack::PopAndDestroy( line16 );
        	}
        else
            {
            iLog->Log(_L("Stopped reading from '%S' with code %d"), &aSampleFile, err );
            }
        }

    target.Flush();
	CleanupStack::PopAndDestroy(); // names
	CleanupStack::PopAndDestroy( &target );
	CleanupStack::PopAndDestroy( &source );
	}

// ---------------------------------------------------------
// ---------------------------------------------------------
//
HBufC* CLandmarkTestProcedureBase::GetGlobalCategoryFromResourceLC( TPosLmGlobalCategory aGlobalId )
    {
    HBufC* name = NULL;
    RArray<TUint> catIds;
    RPointerArray<HBufC> catNames;

    CleanupClosePushL( catIds );
    CleanupStack::PushL( TCleanupItem( ResetAndDestroyHBufCArray, &catNames ));

    ReadGlobalCategoriesFromResourceL( catIds, catNames );
    TInt idIndex = catIds.Find( aGlobalId );
    if ( idIndex >= 0 )
        {
        name = catNames[idIndex];
        catNames.Remove( idIndex );
        }
    else
        {
        iLog->Log(_L("Global category %d not found"), aGlobalId );
        User::Leave( KErrNotFound );
        }
    CleanupStack::PopAndDestroy(); //catNames
    CleanupStack::PopAndDestroy( &catIds );

    CleanupStack::PushL( name );
    return name;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
HBufC* CLandmarkTestProcedureBase::ReadDefaultDatabaseNameFromResourceLC()
    {
    _LIT(KDatabaseNameResourceFile, "z:\\resource\\eposlmdefaultdbname.rsc");
    const TInt KNameResourceOffset = 2;

    TFileName filename( KDatabaseNameResourceFile );
    BaflUtils::NearestLanguageFile( iFileSession, filename );

    CResourceFile* resFile = CResourceFile::NewLC( iFileSession, filename, 0, 0 );

    RResourceReader reader;
    reader.OpenLC( resFile, KNameResourceOffset );

    HBufC* name = reader.ReadHBufCL();

    CleanupStack::PopAndDestroy( &reader );
    CleanupStack::PopAndDestroy( resFile );

    CleanupStack::PushL( name );
    return name;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::EscapeStringToXmlLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* CLandmarkTestProcedureBase::EscapeStringToXmlLC(
    const TDesC& aStrToEscape)
    {
    HBufC* string = HBufC::NewLC(CalculateEscapedStringSize(aStrToEscape));
    string->Des().Copy(aStrToEscape);

    // Must start with &, because when escaping other characters & will be
    // added to the string.
    TPtr ptr = string->Des();
    EscapeString(ptr, KAmp, KAmpXmlStr);
    EscapeString(ptr, KLt, KLtXmlStr);
    EscapeString(ptr, KGt, KGtXmlStr);
    EscapeString(ptr, KApos, KAposXmlStr);
    EscapeString(ptr, KQuot, KQuotXmlStr);

    return string;
    }
    
   // -----------------------------------------------------------------------------
// PosLmConverterUtils::EscapeString
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLandmarkTestProcedureBase::EscapeString(
    TDes& aStrToEscape,
    const TDesC& aFromStr,
    const TDesC& aToStr)
    {
    TPtrC ptr(aStrToEscape);
    TInt lastPos = 0;

    // Find first occurence of the escape string.
    TInt pos = ptr.Find(aFromStr);

    while (pos != KErrNotFound)
        {
        aStrToEscape.Replace(pos+lastPos, aFromStr.Length(), aToStr);

        // Move ptr forward past the inserted aToStr in aStrToEscape
        pos += aToStr.Length();
        lastPos += pos;
        ptr.Set(aStrToEscape.Right(aStrToEscape.Length() - lastPos));

        // Find next occurence of the escape string.
        pos = ptr.Find(aFromStr);
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::CalculateEscapedStringSize
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CalculateEscapedStringSize(
    const TDesC& aStrToEscape)
    {
    return aStrToEscape.Length() +
        CalculateIncrease(aStrToEscape, KAmp, KAmpXmlStr) +
        CalculateIncrease(aStrToEscape, KLt, KLtXmlStr) +
        CalculateIncrease(aStrToEscape, KGt, KGtXmlStr) +
        CalculateIncrease(aStrToEscape, KApos, KAposXmlStr) +
        CalculateIncrease(aStrToEscape, KQuot, KQuotXmlStr);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::CalculateIncrease
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CLandmarkTestProcedureBase::CalculateIncrease(
    const TDesC& aStrToEscape,
    const TDesC& aFromStr,
    const TDesC& aToStr)
    {
    TPtrC ptr(aStrToEscape);
    TInt occurrences = 0;

    // Find first occurence of the escape string.
    TInt pos = ptr.Find(aFromStr);

    while (pos != KErrNotFound)
        {
        ++occurrences;

        // Move ptr forward past the found occurence.
        ptr.Set(ptr.Right(ptr.Length() - (pos + aFromStr.Length())));

        // Find next occurence of the escaped string.
        pos = ptr.Find(aFromStr);
        }

    // Return the increased (or decreased) size impact on the string to escape.
    return occurrences*(aToStr.Length() - aFromStr.Length());
    }
