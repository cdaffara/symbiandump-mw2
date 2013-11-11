/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Parser implementation for lmx format.
*
*
*/


#include <utf.h>
#include <xml/xmlparsererrors.h>
#include <lbs.h>
#include <EPos_PosLmImplExtension.h>
#include <EPos_LandmarksErrors.h>
#include "EPos_CPosLmXmlParser.h"
#include "EPos_CPosLmXmlParserOperation.h"
#include "EPos_CPosLmCollectionData.h"
#include "EPos_CPosLmParsedLmBuffer.h"
#include "EPos_PosLmConverterUtils.h"
#include "EPos_LmConverter.h"
#include "EPos_CPosLmLmxParser.h"

const TReal KPosLmParserMinLatitude = -90.0;
const TReal KPosLmParserMaxLatitude = 90.0;
const TReal KPosLmParserMinLongitude = -180.0;
const TReal KPosLmParserMaxLongitude = 180.0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLmxParser::CPosLmLmxParser()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLmxParser* CPosLmLmxParser::NewL()
    {
    CPosLmLmxParser* self = new (ELeave) CPosLmLmxParser;
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLmxParser::~CPosLmLmxParser()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxParser::CheckMandatoryTagsL( TPosXmlTagType aTreeType )
    {
    switch ( aTreeType )
        {
        case EPosXmlRoot:
            PosLmConverterUtils::AssertL(
                ChildTagFound( EPosXmlLandmark ) || 
                ChildTagFound( EPosXmlLmCollection ) );
            break;
        case EPosXmlLmCollection:
            PosLmConverterUtils::AssertL( ChildTagFound( EPosXmlLandmark ) );
            break;
        case EPosXmlLmCoordinates:
            // Latitude and longitude are mandatory in coordinates tag.
            PosLmConverterUtils::AssertL( ChildTagFound( EPosXmlCoordLatitude ) );
            PosLmConverterUtils::AssertL( ChildTagFound( EPosXmlCoordLongitude ) );
            break;
        case EPosXmlLmCategory:
            PosLmConverterUtils::AssertL( ChildTagFound( EPosXmlCategoryName ) );
            break;
        case EPosXmlLmMediaLink:
            PosLmConverterUtils::AssertL( ChildTagFound( EPosXmlMediaLinkUrl ) );
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxParser::SetContentL(
    TPosXmlTagType aTag,
    const TDesC& aContent)
    {
    switch (aTag)
        {
        // landmarkCollection Tags
        case EPosXmlLmCollName:
            AddCollectionDataL( EPosLmCollDataCollectionName, aContent );
            break;
        case EPosXmlLmCollDescription:
            AddCollectionDataL( EPosLmCollDataCollectionDescription, aContent );
            break;
        // landmark Tags
        case EPosXmlLandmark:
            {
            PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
            if (!Math::IsNaN(iParsedLatitude) && !Math::IsNaN(iParsedLongitude))
                {
                TLocality loc(TCoordinate(iParsedLatitude, iParsedLongitude,
                    iParsedAltitude), iParsedHorAcc, iParsedVertAcc);
                iWorkingLandmark->SetPositionL(loc);
                }

            TBuf<5> num;
            num.Num(iNoOfMediaLinks);
            iWorkingLandmark->SetPositionFieldL(EPositionFieldMediaLinks, num);

            HandleLandmarkParsedL();
            break;
            }
        case EPosXmlLmName:
            PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
            iWorkingLandmark->SetLandmarkNameL( aContent.Left( KPosLmMaxTextFieldLength ) );
            break;
        case EPosXmlLmDescription:
            PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
            iWorkingLandmark->SetLandmarkDescriptionL( aContent.Left( KPosLmMaxDescriptionLength ) );
            break;
        // coordinates Tag
        case EPosXmlCoordLatitude:
            PosLmConverterUtils::DesToRealL(aContent, iParsedLatitude);

            // Ensure latitude is in valid range
            PosLmConverterUtils::AssertL(
                (iParsedLatitude <= KPosLmParserMaxLatitude &&
                iParsedLatitude >= KPosLmParserMinLatitude) ||
                Math::IsNaN(iParsedLatitude));
            break;
        case EPosXmlCoordLongitude:
            PosLmConverterUtils::DesToRealL(aContent, iParsedLongitude);

            // Ensure latitude is in valid range
            PosLmConverterUtils::AssertL(
                (iParsedLongitude <= KPosLmParserMaxLongitude &&
                iParsedLongitude >= KPosLmParserMinLongitude) ||
                Math::IsNaN(iParsedLongitude));
            break;
        case EPosXmlCoordAltitude:
            PosLmConverterUtils::DesToRealL( aContent, iParsedAltitude );
            break;
        case EPosXmlCoordHorizontalAcc:
            PosLmConverterUtils::DesToRealL( aContent, iParsedHorAcc );

            // Ensure horizontal acc is >= 0
            PosLmConverterUtils::AssertL(
                iParsedHorAcc >= 0 || Math::IsNaN( iParsedHorAcc ) );
            break;
        case EPosXmlCoordVerticalAcc:
            PosLmConverterUtils::DesToRealL(aContent, iParsedVertAcc);

            // Ensure vertical acc is >= 0
            PosLmConverterUtils::AssertL(
                iParsedVertAcc >= 0 || Math::IsNaN( iParsedVertAcc ) );
            break;
        // coverageRadius Tag
        case EPosXmlLmCoverageRadius:
            TReal32 coverageRadius;
            PosLmConverterUtils::DesToRealL( aContent, coverageRadius );

            // Ensure coverageRadius is >= 0
            PosLmConverterUtils::AssertL(
                coverageRadius >= 0 || Math::IsNaN( coverageRadius ) );

            PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
            iWorkingLandmark->SetCoverageRadius(coverageRadius);
            break;
        // mediaLink Tag
        case EPosXmlMediaLinkMime:
            PosLmConverterUtils::AssertL( !iReadMimeType );
            iReadMimeType = aContent.AllocL();
            break;
        case EPosXmlMediaLinkUrl:
            PosLmConverterUtils::AssertL(
                PosLmConverterUtils::IsAnyUri(aContent));

            PosLmConverterUtils::AssertL( !iReadUrl );
            iReadUrl = aContent.AllocL();
            break;
        case EPosXmlLmMediaLink:
            {
            PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
            PosLmConverterUtils::AssertL( iReadUrl != NULL );
            
            HBufC* mediaLink =
                PosLmConverterUtils::ConcatenateMediaLinkLC(
                *iReadMimeType, *iReadUrl);
            iWorkingLandmark->SetPositionFieldL(static_cast<TPositionFieldId>
                (EPositionFieldMediaLinksStart + iNoOfMediaLinks),
                mediaLink->Left(KPosLmMaxTextFieldLength));
            iNoOfMediaLinks++;
            CleanupStack::PopAndDestroy(mediaLink);

            delete iReadMimeType;
            iReadMimeType = NULL;
            delete iReadUrl;
            iReadUrl = NULL;
            break;
            }

        // category Tags
        case EPosXmlLmCategory:
            PosLmConverterUtils::AssertL( iWorkingCategory && iWorkingLandmark );
            PosLmImplExtension::SetCategoryIdL( *iWorkingCategory, iWorkingItemId );
            iWorkingLandmark->AddCategoryL( iWorkingItemId++ );
            User::LeaveIfError( iCategories.Append( iWorkingCategory ) );
            iWorkingCategory = NULL;
            break;
        case EPosXmlCategoryId:
            // Set global category
            TPosLmGlobalCategory categoryid;
            PosLmConverterUtils::DesToTUintL(aContent, categoryid);
            PosLmConverterUtils::AssertL( iWorkingCategory != NULL );
            PosLmImplExtension::SetGlobalCategory( *iWorkingCategory, categoryid );
            break;
        case EPosXmlCategoryName:
            PosLmConverterUtils::AssertL(aContent.Length() > 0);
            PosLmConverterUtils::AssertL( iWorkingCategory != NULL );
            iWorkingCategory->SetCategoryNameL( aContent.Left( KPosLmMaxCategoryNameLength ) );
            break;
            //coverity[unterminated_case :FALSE]
        case EPosXmlAddrPlaceId: //addition for Place id
            PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
            iWorkingLandmark->SetPlaceIdL( aContent.Left( KPosLmMaxTextFieldLength ) );
            //coverity[fallthrough : FALSE]
        default:
            // We need to check if it is a addressInfo tag.
            // AddressInfo Tag
            TPositionFieldId fieldId = PositionFieldId( aTag );
            if (fieldId != EPositionFieldNone)
                {
                if (fieldId == EPositionFieldCountryCode)
                    {
                    // Ensure contrycode is of length 2
                    PosLmConverterUtils::AssertL(
                        PosLmConverterUtils::IsTokenL(aContent,
                        KPosCountryCodeLength));
                    }
                PosLmConverterUtils::AssertL( iWorkingLandmark != NULL );
                iWorkingLandmark->SetPositionFieldL( fieldId, aContent.Left(KPosLmMaxTextFieldLength ) );
                }
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLmxParser::IsLandmarkStarted( TPosXmlTagType aTag )
    {
    return ( aTag == EPosXmlLandmark );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLmxParser::IsCategoryStarted( TPosXmlTagType aTag )
    {
    return ( aTag == EPosXmlLmCategory );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmLmxParser::IsTreeTag( TPosXmlTagType aTag )
    {
    __ASSERT_DEBUG( aTag != EPosXmlNothing, Panic( KPosLmConverterPanic, KErrGeneral ) );
    __ASSERT_DEBUG( aTag != EPosXmlUnknown, Panic( KPosLmConverterPanic, KErrGeneral ) );
    
    return ( aTag >= EPosXmlLmLastTree ) ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosXmlTagType CPosLmLmxParser::TagTypeL(  const Xml::RTagInfo& aElement  )
    {
    TPosXmlTagType tagType = EPosXmlUnknown;
    TPosXmlTagType treeType = CurrentTree();
    
    TPtrC8 tagName = aElement.LocalName().DesC();
    
    if ( treeType == EPosXmlNothing )
        {
        if ( EqualsTagL( tagName, KPosXmlLmxRoot ) )
            {
            tagType = EPosXmlRoot;
            }
        }
    else if ( MatchesRootNamespace( aElement ) )
        {
        switch ( treeType )
            {
            case EPosXmlRoot:
                {
                if ( EqualsTagL( tagName, KPosXmlLmCollection ) )
                    {
                    tagType = EPosXmlLmCollection;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLm ) )
                    {
                    tagType = EPosXmlLandmark;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmxRoot ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }
                
            case EPosXmlLmCollection:
                {
                if ( EqualsTagL( tagName, KPosXmlLmCollName ) )
                    {
                    tagType = EPosXmlLmCollName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCollDescription ) )
                    {
                    tagType = EPosXmlLmCollDescription;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLm ) )
                    {
                    tagType = EPosXmlLandmark;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCollection ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }

            case EPosXmlLandmark:
                {
                if ( EqualsTagL( tagName, KPosXmlLmName ) )
                    {
                    tagType = EPosXmlLmName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmDescription ) )
                    {
                    tagType = EPosXmlLmDescription;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCoordinates ) )
                    {
                    tagType = EPosXmlLmCoordinates;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCoverageRadius ) )
                    {
                    tagType = EPosXmlLmCoverageRadius;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmAddressInfo ) )
                    {
                    tagType = EPosXmlLmAddressInfo;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmMediaLink ) )
                    {
                    tagType = EPosXmlLmMediaLink;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCategory ) )
                    {
                    tagType = EPosXmlLmCategory;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLm ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }

            case EPosXmlLmCoordinates:
                {
                if ( EqualsTagL( tagName, KPosXmlCoordLatitude ) )
                    {
                    tagType = EPosXmlCoordLatitude;
                    }
                else if ( EqualsTagL( tagName, KPosXmlCoordLongitude ) )
                    {
                    tagType = EPosXmlCoordLongitude;
                    }
                else if ( EqualsTagL( tagName, KPosXmlCoordAltitude ) )
                    {
                    tagType = EPosXmlCoordAltitude;
                    }
                else if ( EqualsTagL( tagName, KPosXmlCoordHorizontalAcc ) )
                    {
                    tagType = EPosXmlCoordHorizontalAcc;
                    }
                else if ( EqualsTagL( tagName, KPosXmlCoordVerticalAcc ) )
                    {
                    tagType = EPosXmlCoordVerticalAcc;
                    }
                else if ( EqualsTagL( tagName, KPosXmlCoordTimeStamp ) )
                    {
                    tagType = EPosXmlCoordTimestamp;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCoordinates ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }

            case EPosXmlLmAddressInfo:
                {
                if ( EqualsTagL( tagName, KPosXmlAddrCountry ) )
                    {
                    tagType = EPosXmlAddrCountry;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrCountryCode ) )
                    {
                    tagType = EPosXmlAddrCountryCode;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrState ) )
                    {
                    tagType = EPosXmlAddrState;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrCounty ) )
                    {
                    tagType = EPosXmlAddrCounty;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrCity ) )
                    {
                    tagType = EPosXmlAddrCity;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrDistrict ) )
                    {
                    tagType = EPosXmlAddrDistrict;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrPostalCode ) )
                    {
                    tagType = EPosXmlAddrPostalCode;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrCrossing1 ) )
                    {
                    tagType = EPosXmlAddrCrossing1;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrCrossing2 ) )
                    {
                    tagType = EPosXmlAddrCrossing2;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrStreet ) )
                    {
                    tagType = EPosXmlAddrStreet;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrBuildingName ) )
                    {
                    tagType = EPosXmlAddrBuildingName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrBuildingZone ) )
                    {
                    tagType = EPosXmlAddrBuildingZone;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrBuildingFloor ) )
                    {
                    tagType = EPosXmlAddrBuildingFloor;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrBuildingRoom ) )
                    {
                    tagType = EPosXmlAddrBuildingRoom;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrExtension ) )
                    {
                    tagType = EPosXmlAddrExtension;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrPhoneNumber ) )
                    {
                    tagType = EPosXmlAddrPhoneNumber;
                    }
                else if ( EqualsTagL( tagName, KPosXmlAddrPlaceID ) )
                    {
                    tagType = EPosXmlAddrPlaceId;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmAddressInfo ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }

            case EPosXmlLmMediaLink:
                {
                if ( EqualsTagL( tagName, KPosXmlMediaLinkName ) )
                    {
                    tagType = EPosXmlMediaLinkName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlMediaLinkMime ) )
                    {
                    tagType = EPosXmlMediaLinkMime;
                    }
                else if ( EqualsTagL( tagName, KPosXmlMediaLinkUrl ) )
                    {
                    tagType = EPosXmlMediaLinkUrl;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmMediaLink ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }

            case EPosXmlLmCategory:
                {
                if ( EqualsTagL( tagName, KPosXmlCategoryId ) )
                    {
                    tagType = EPosXmlCategoryId;
                    }
                else if ( EqualsTagL( tagName, KPosXmlCategoryName ) )
                    {
                    tagType = EPosXmlCategoryName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlLmCategory ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                break;
                }
            }
        }
    else
        {
        }
    return tagType;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxParser::OnAttributesL( 
    TPosXmlTagType /*aTag*/,
    const Xml::RAttributeArray& /*aAttributes*/ )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxParser::GetNextAllowedTagsL( RArray<TPosXmlTagType>& aChildTags )
    {
    switch ( CurrentTree() )
        {
        case EPosXmlNothing:
            {
            aChildTags.AppendL( EPosXmlRoot );
            break;
            }
        case EPosXmlRoot:
            {
            if ( LastKnownChildTag() == EPosXmlNothing )
                {
                aChildTags.AppendL( EPosXmlLmCollection );
                aChildTags.AppendL( EPosXmlLandmark );
                }
            break;
            }
        case EPosXmlLmCollection:
            {
            const TPosXmlTagType collectionChildren[] = { 
                EPosXmlLmCollName, EPosXmlLmCollDescription,
                EPosXmlLandmark, EPosXmlLandmark // landmark tag can be repeated
                };
            const TInt collectionChildrenSize = sizeof( collectionChildren ) / sizeof( TPosXmlTagType );
            GenValidSequenceL( collectionChildren, collectionChildrenSize, aChildTags );
            break;
            }
        case EPosXmlLandmark:
            {
            const TPosXmlTagType landmarkChildren[] = { 
                EPosXmlLmName, EPosXmlLmDescription,
                EPosXmlLmCoordinates, EPosXmlLmCoverageRadius,
                EPosXmlLmAddressInfo,
                EPosXmlLmMediaLink, EPosXmlLmMediaLink, // medialink can be repeated
                EPosXmlLmCategory, EPosXmlLmCategory, // category can be repeated
                EPosXmlLmCategory,EPosXmlLmMediaLink  //category can be there after media link
                };
            const TInt landmarkChildrenSize = sizeof( landmarkChildren ) / sizeof( TPosXmlTagType );
            GenValidSequenceL( landmarkChildren, landmarkChildrenSize, aChildTags );
            break;
            }
        case EPosXmlLmMediaLink:
            {
            const TPosXmlTagType mediaLinkChildren[] = { 
                EPosXmlMediaLinkName, EPosXmlMediaLinkMime, EPosXmlMediaLinkUrl
                };
            const TInt mediaLinkChildrenSize = sizeof( mediaLinkChildren ) / sizeof( TPosXmlTagType );
            GenValidSequenceL( mediaLinkChildren, mediaLinkChildrenSize, aChildTags );
            break;
            }
        case EPosXmlLmCategory:
            {
            const TPosXmlTagType categoryChildren[] = { 
                EPosXmlCategoryId, EPosXmlCategoryName
                };
            const TInt categoryChildrenSize = sizeof( categoryChildren ) / sizeof( TPosXmlTagType );
            GenValidSequenceL( categoryChildren, categoryChildrenSize, aChildTags );
            break;
            }
        case EPosXmlLmCoordinates:
            {
            const TPosXmlTagType coordinateChildren[] = { 
                EPosXmlCoordLatitude, EPosXmlCoordLongitude, EPosXmlCoordAltitude,
                EPosXmlCoordHorizontalAcc, EPosXmlCoordVerticalAcc, EPosXmlCoordTimestamp
                };
            const TInt coordinateChildrenSize = sizeof( coordinateChildren ) / sizeof( TPosXmlTagType );
            GenValidSequenceL( coordinateChildren, coordinateChildrenSize, aChildTags );
            break;
            }
        case EPosXmlLmAddressInfo:
            {
            const TPosXmlTagType addressChildren[] = { 
                EPosXmlAddrCountry, EPosXmlAddrCountryCode, EPosXmlAddrState,
                EPosXmlAddrCounty, EPosXmlAddrCity, EPosXmlAddrDistrict,
                EPosXmlAddrPostalCode, EPosXmlAddrCrossing1, EPosXmlAddrCrossing2,
                EPosXmlAddrStreet, EPosXmlAddrBuildingName, EPosXmlAddrBuildingZone,
                EPosXmlAddrBuildingFloor, EPosXmlAddrBuildingRoom, EPosXmlAddrExtension,
                EPosXmlAddrPhoneNumber, EPosXmlAddrPlaceId
                };
            const TInt addressChildrenSize = sizeof( addressChildren ) / sizeof( TPosXmlTagType );
            GenValidSetL( addressChildren, addressChildrenSize, aChildTags  );
            break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPositionFieldId CPosLmLmxParser::PositionFieldId( TPosXmlTagType aXmlTagType )
    {
    TPositionFieldId fieldId = EPositionFieldNone;

    switch (aXmlTagType)
        {
        case EPosXmlAddrBuildingFloor:
            fieldId = EPositionFieldBuildingFloor;
            break;
        case EPosXmlAddrBuildingName:
            fieldId = EPositionFieldBuildingName;
            break;
        case EPosXmlAddrBuildingRoom:
            fieldId = EPositionFieldBuildingRoom;
            break;
        case EPosXmlAddrBuildingZone:
            fieldId = EPositionFieldBuildingZone;
            break;
        case EPosXmlAddrCity:
            fieldId = EPositionFieldCity;
            break;
        case EPosXmlAddrCountry:
            fieldId = EPositionFieldCountry;
            break;
        case EPosXmlAddrCountryCode:
            fieldId = EPositionFieldCountryCode;
            break;
        case EPosXmlAddrCounty:
            fieldId = EPositionFieldCounty;
            break;
        case EPosXmlAddrCrossing1:
            fieldId = EPositionFieldCrossing1;
            break;
        case EPosXmlAddrCrossing2:
            fieldId = EPositionFieldCrossing2;
            break;
        case EPosXmlAddrDistrict:
            fieldId = EPositionFieldDistrict;
            break;
        case EPosXmlAddrExtension:
            fieldId = EPositionFieldStreetExtension;
            break;
        case EPosXmlAddrPhoneNumber:
            fieldId = EPositionFieldBuildingTelephone;
            break;
        case EPosXmlAddrPostalCode:
            fieldId = EPositionFieldPostalCode;
            break;
        case EPosXmlAddrState:
            fieldId = EPositionFieldState;
            break;
        case EPosXmlAddrStreet:
            fieldId = EPositionFieldStreet;
            break;
        default:
            break;
        }

    return fieldId;
    }
