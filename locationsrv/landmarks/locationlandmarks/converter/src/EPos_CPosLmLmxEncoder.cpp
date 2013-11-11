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
* Description: Implementation of CPosLmXmlEncoder for LMX format.
*
*
*/


#include    <e32base.h>
#include    <e32math.h>
#include    <lbsposition.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmLmxEncoder.h"
#include    "EPos_PosLmConverterUtils.h"

const TInt KPosFieldsGranularity = 8;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLmxEncoder::CPosLmLmxEncoder()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLmxEncoder* CPosLmLmxEncoder::NewL()
    {
    return new (ELeave) CPosLmLmxEncoder;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLmxEncoder::~CPosLmLmxEncoder()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteStartTagsL()
    {
    // Root
    HBufC* rootAttributes = HBufC::NewLC( KPosLongStringLength );
    rootAttributes->Des().Append( KPosXmlLmNSAttName );
    rootAttributes->Des().Append( KPosXmlEq );
    rootAttributes->Des().Append( KPosXmlQuotationMark );
    rootAttributes->Des().Append( KPosXmlSchemaLocationAttValue );
    rootAttributes->Des().Append( KPosXmlVersionAttValue );
    rootAttributes->Des().Append( KPosXmlQuotationMark );

    rootAttributes->Des().Append( KPosXmlSpace );
    rootAttributes->Des().Append( KPosXmlXsiNSAttName );
    rootAttributes->Des().Append( KPosXmlEq );
    rootAttributes->Des().Append( KPosXmlQuotationMark );
    rootAttributes->Des().Append( KPosXmlXsiNSAttValue );
    rootAttributes->Des().Append( KPosXmlQuotationMark );

    rootAttributes->Des().Append( KPosXmlSpace );
    rootAttributes->Des().Append( KPosXmlSchemaLocationAttName );
    rootAttributes->Des().Append( KPosXmlEq );
    rootAttributes->Des().Append( KPosXmlQuotationMark );
    rootAttributes->Des().Append( KPosXmlSchemaLocationAttValue );
    rootAttributes->Des().Append( KPosXmlVersionAttValue );
    rootAttributes->Des().Append( KPosXmlSlash );
    rootAttributes->Des().Append( KPosXmlSpace );
    rootAttributes->Des().Append( KPosXmlSchemaAttValue );
    rootAttributes->Des().Append( KPosXmlQuotationMark );

    WriteSTagL( EPosXmlRoot, *rootAttributes );
    CleanupStack::PopAndDestroy( rootAttributes );

    // Start with collection right away
    WriteSTagL( EPosXmlLmCollection, KNullDesC );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteFinalTagsL()
    {
    WriteETagL( EPosXmlLmCollection );
    WriteETagL( EPosXmlRoot );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteCollectionDataL( 
    const HBufC* aCollectionName,
    const HBufC* aCollectionDescription )
    {
    if ( aCollectionName )
        {
        WriteStringWithTagsL( *aCollectionName, EPosXmlLmCollName );
        }

    if ( aCollectionDescription )
        {
        WriteStringWithTagsL( *aCollectionDescription, EPosXmlLmCollDescription );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteLandmarkL( 
    const CPosLandmark& aLandmark,
    const RPointerArray<const CPosLandmarkCategory>& aCategories )
    {
    TPtrC textField;

    WriteSTagL( EPosXmlLandmark, KNullDesC );
 
    // Write lm name
    if (aLandmark.GetLandmarkName(textField) == KErrNone)
        {
        WriteStringWithTagsL(textField, EPosXmlLmName);
        }

    // Write lm description
    if (aLandmark.GetLandmarkDescription(textField) == KErrNone)
        {
        WriteStringWithTagsL(textField, EPosXmlLmDescription);
        }

    // Write lm coordinates
    TLocality position;
    TInt status = aLandmark.GetPosition(position);
    if (status == KErrNone)
        {
        WriteLmCoordinatesL(position);
        }

    // Write lm coverage radius
    TReal32 coverageRadius;
    if (aLandmark.GetCoverageRadius(coverageRadius) == KErrNone)
        {
        WriteTRealWithTagsL(coverageRadius, EPosXmlLmCoverageRadius);
        }

    // Write address info and media link
    WriteLmPositionFieldsL(aLandmark);

    for ( TInt i = 0; i < aCategories.Count(); ++i )
        {
        WriteCategoryL( *(aCategories[i]) );
        }
    
    WriteETagL( EPosXmlLandmark );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteLmCoordinatesL( const TLocality& aLocality )
    {
    WriteSTagL( EPosXmlLmCoordinates, KNullDesC );

    // Write latitude, mandatory tag
    WriteTRealWithTagsL(aLocality.Latitude(), EPosXmlCoordLatitude);

    // Write longitude, mandatory tag
    WriteTRealWithTagsL(aLocality.Longitude(), EPosXmlCoordLongitude);

    // Write altitude
    if (!Math::IsNaN(aLocality.Altitude()))
        {
        WriteTRealWithTagsL(aLocality.Altitude(), EPosXmlCoordAltitude);
        }

    // Write horizontalAccuracy
    if (!Math::IsNaN(aLocality.HorizontalAccuracy()))
        {
        WriteTRealWithTagsL(aLocality.HorizontalAccuracy(),
                            EPosXmlCoordHorizontalAcc);
        }

    // Write verticalAccuracy
    if (!Math::IsNaN(aLocality.VerticalAccuracy()))
        {
        WriteTRealWithTagsL(aLocality.VerticalAccuracy(),
                            EPosXmlCoordVerticalAcc);
        }

    WriteETagL(EPosXmlLmCoordinates);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteLmPositionFieldsL( const CPosLandmark& aLandmark )
    {
    RArray<TPosXmlTagType> addressTags;
    CleanupClosePushL(addressTags);

    CDesCArray* addressStrings =
        new (ELeave) CDesCArrayFlat(KPosFieldsGranularity);
    CleanupStack::PushL(addressStrings);

    CDesCArray* mediaLinkStrings =
        new (ELeave) CDesCArrayFlat(KPosFieldsGranularity);
    CleanupStack::PushL(mediaLinkStrings);

    // Sort out address info and media link from position fields.
    TPositionFieldId id = aLandmark.FirstPositionFieldId();
    while (id != EPositionFieldNone)
        {
        TPtrC textField;
        if (aLandmark.GetPositionField(id, textField) == KErrNone)
            {
            if ((id > EPositionFieldAddressCapabilitiesBegin &&
                id < EPositionFieldNMEACapabilitiesBegin)|| id == EPositionFieldProprietaryFieldsBegin)
                {
                TPosXmlTagType tag = XmlTagForPositionField( id );
                TBool validToExport = (tag != EPosXmlNothing);

                if (id == EPositionFieldCountryCode)
                    {
                    // Check that this is a valid token
                    validToExport = PosLmConverterUtils::IsTokenL(
                        textField, KPosCountryCodeLength);
                    }

                if (validToExport)
                    {
                    User::LeaveIfError(addressTags.Append(tag));
                    addressStrings->AppendL(textField);
                    }
                }
            else if (id > EPositionFieldMediaLinks &&
                id < EPositionFieldProprietaryFieldsBegin)
                {
                mediaLinkStrings->AppendL(textField);
                }
            }

        id = aLandmark.NextPositionFieldId(id);
        }

    // Write lm address info
    if (addressTags.Count() != 0)
        {
        WriteSTagL( EPosXmlLmAddressInfo, KNullDesC );

        for (TInt i = 0; i < addressTags.Count(); i++)
            {
            WriteStringWithTagsL((*addressStrings)[i], addressTags[i]);
            }

        WriteETagL(EPosXmlLmAddressInfo);
        }

    // Write media links
    for (TInt i = 0; i < mediaLinkStrings->Count(); i++)
        {
        TInt maxLength = (*mediaLinkStrings)[i].Length();
        HBufC* mime = HBufC::NewLC(maxLength);
        HBufC* url = HBufC::NewLC(maxLength);

        TInt status = PosLmConverterUtils::SeparateMediaLink(
            (*mediaLinkStrings)[i], mime, url);

        if (status == KErrNone)
            {
            if (PosLmConverterUtils::IsAnyUri(*url))
                {
                WriteSTagL( EPosXmlLmMediaLink, KNullDesC );

                // Write mime
                if (mime->Length() != 0)
                    {
                    WriteStringWithTagsL(*mime, EPosXmlMediaLinkMime);
                    }

                // Write url, mandatory tag
                WriteStringWithTagsL(*url, EPosXmlMediaLinkUrl);

                WriteETagL(EPosXmlLmMediaLink);
                }
            }

        CleanupStack::PopAndDestroy(2, mime); // url
        }

    CleanupStack::PopAndDestroy(3, &addressTags); // addressStrings
                                                  // mediaLinkStrings
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmLmxEncoder::WriteCategoryL( const CPosLandmarkCategory& aCategory )
    {
    WriteSTagL( EPosXmlLmCategory, KNullDesC );

    // Write global id
    TPosLmGlobalCategory id = aCategory.GlobalCategory();
    if ( id != KPosLmNullGlobalCategory )
        {
        const TInt KMaxDecimalStringLen = 10;
        TBuf<KMaxDecimalStringLen> idStr;
        idStr.AppendNum( id );
        WriteStringWithTagsL( idStr, EPosXmlCategoryId );
        }

    // Write category name
    WriteSTagL( EPosXmlCategoryName, KNullDesC ); // mandatory tag
    TPtrC categoryName;
    if (aCategory.GetCategoryName(categoryName) == KErrNone)
        {
        WriteEscapedStringL(categoryName);
        }
    WriteETagL( EPosXmlCategoryName ); // mandatory tag

    WriteETagL( EPosXmlLmCategory );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosXmlTagType CPosLmLmxEncoder::XmlTagForPositionField( TPositionFieldId aPosFieldId )
    {
    TPosXmlTagType xmlType = EPosXmlNothing;

    switch (aPosFieldId)
        {
        case EPositionFieldBuildingFloor:
            xmlType = EPosXmlAddrBuildingFloor;
            break;
        case EPositionFieldBuildingName:
            xmlType = EPosXmlAddrBuildingName;
            break;
        case EPositionFieldBuildingRoom:
            xmlType = EPosXmlAddrBuildingRoom;
            break;
        case EPositionFieldBuildingZone:
            xmlType = EPosXmlAddrBuildingZone;
            break;
        case EPositionFieldCity:
            xmlType = EPosXmlAddrCity;
            break;
        case EPositionFieldCountry:
            xmlType = EPosXmlAddrCountry;
            break;
        case EPositionFieldCountryCode:
            xmlType = EPosXmlAddrCountryCode;
            break;
        case EPositionFieldCounty:
            xmlType = EPosXmlAddrCounty;
            break;
        case EPositionFieldCrossing1:
            xmlType = EPosXmlAddrCrossing1;
            break;
        case EPositionFieldCrossing2:
            xmlType = EPosXmlAddrCrossing2;
            break;
        case EPositionFieldDistrict:
            xmlType = EPosXmlAddrDistrict;
            break;
        case EPositionFieldStreetExtension:
            xmlType = EPosXmlAddrExtension;
            break;
        case EPositionFieldBuildingTelephone:
            xmlType = EPosXmlAddrPhoneNumber;
            break;
        case EPositionFieldPostalCode:
            xmlType = EPosXmlAddrPostalCode;
            break;
        case EPositionFieldState:
            xmlType = EPosXmlAddrState;
            break;
        case EPositionFieldStreet:
            xmlType = EPosXmlAddrStreet;
            break;
        case EPositionFieldProprietaryFieldsBegin:
            xmlType = EPosXmlAddrPlaceId;
            break;
        default:
            break;
        }

    return xmlType;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPtrC CPosLmLmxEncoder::TagNameL( TPosXmlTagType aTagType )
    {
    switch (aTagType)
        {
        case EPosXmlRoot:
            return KPosXmlLmxRoot();
        case EPosXmlLmCollection:
            return KPosXmlLmCollection();
        case EPosXmlLandmark:
            return KPosXmlLm();
        case EPosXmlLmCoordinates:
            return KPosXmlLmCoordinates();
        case EPosXmlLmAddressInfo:
            return KPosXmlLmAddressInfo();
        case EPosXmlLmMediaLink:
            return KPosXmlLmMediaLink();
        case EPosXmlLmCategory:
            return KPosXmlLmCategory();
        case EPosXmlLmCollName:
            return KPosXmlLmCollName();
        case EPosXmlLmCollDescription:
            return KPosXmlLmCollDescription();
        case EPosXmlLmName:
            return KPosXmlLmName();
        case EPosXmlLmDescription:
            return KPosXmlLmDescription();
        case EPosXmlLmCoverageRadius:
            return KPosXmlLmCoverageRadius();
        case EPosXmlCoordLatitude:
            return KPosXmlCoordLatitude();
        case EPosXmlCoordLongitude:
            return KPosXmlCoordLongitude();
        case EPosXmlCoordAltitude:
            return KPosXmlCoordAltitude();
        case EPosXmlCoordHorizontalAcc:
            return KPosXmlCoordHorizontalAcc();
        case EPosXmlCoordVerticalAcc:
            return KPosXmlCoordVerticalAcc();
        case EPosXmlAddrCountry:
            return KPosXmlAddrCountry();
        case EPosXmlAddrCountryCode:
            return KPosXmlAddrCountryCode();
        case EPosXmlAddrState:
            return KPosXmlAddrState();
        case EPosXmlAddrCounty:
            return KPosXmlAddrCounty();
        case EPosXmlAddrCity:
            return KPosXmlAddrCity();
        case EPosXmlAddrDistrict:
            return KPosXmlAddrDistrict();
        case EPosXmlAddrPostalCode:
            return KPosXmlAddrPostalCode();
        case EPosXmlAddrCrossing1:
            return KPosXmlAddrCrossing1();
        case EPosXmlAddrCrossing2:
            return KPosXmlAddrCrossing2();
        case EPosXmlAddrStreet:
            return KPosXmlAddrStreet();
        case EPosXmlAddrBuildingName:
            return KPosXmlAddrBuildingName();
        case EPosXmlAddrBuildingZone:
            return KPosXmlAddrBuildingZone();
        case EPosXmlAddrBuildingFloor:
            return KPosXmlAddrBuildingFloor();
        case EPosXmlAddrBuildingRoom:
            return KPosXmlAddrBuildingRoom();
        case EPosXmlAddrExtension:
            return KPosXmlAddrExtension();
        case EPosXmlAddrPhoneNumber:
            return KPosXmlAddrPhoneNumber();
        case EPosXmlMediaLinkMime:
            return KPosXmlMediaLinkMime();
        case EPosXmlMediaLinkUrl:
            return KPosXmlMediaLinkUrl();
        case EPosXmlCategoryId:
            return KPosXmlCategoryId();
        case EPosXmlCategoryName:
            return KPosXmlCategoryName();
        case EPosXmlAddrPlaceId:
            return KPosXmlAddrPlaceID();
        default:
            User::Leave(KErrNotSupported);
            return KNullDesC();
        }
    }
