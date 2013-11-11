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
* Description: Parser implementation for GPX format.
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
#include "EPos_CPosLmGpxParser.h"

const TReal KPosLmParserMinLatitude = -90.0;
const TReal KPosLmParserMaxLatitude = 90.0;
const TReal KPosLmParserMinLongitude = -180.0;
const TReal KPosLmParserMaxLongitude = 180.0;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmGpxParser::CPosLmGpxParser()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmGpxParser* CPosLmGpxParser::NewL()
    {
    CPosLmGpxParser* self = new (ELeave) CPosLmGpxParser;
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmGpxParser::~CPosLmGpxParser()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmGpxParser::OnStartDocumentL(
    const Xml::RDocumentParameters& aDocParam, 
    TInt aErrorCode)
    {
    CPosLmXmlParser::OnStartDocumentL( aDocParam, aErrorCode );
    iGpxVersion = EGpxVersionUnknown;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmGpxParser::GetNextAllowedTagsL( RArray<TPosXmlTagType>& aChildTags )
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
            // v1.0
            const TPosXmlTagType rootChildren10[] = { 
                EPosXmlLmCollName, EPosXmlLmCollDescription, 
                EPosXmlLandmark, EPosXmlLandmark // landmark can be repeated
                };
            const TInt rootChildren10Size = sizeof( rootChildren10 ) / sizeof( TPosXmlTagType );
    
            // v1.1
            const TPosXmlTagType rootChildren11[] = { 
                EPosXmlMetadata,
                EPosXmlLandmark, EPosXmlLandmark // landmark can be repeated
                };
            const TInt rootChildren11Size = sizeof( rootChildren11 ) / sizeof( TPosXmlTagType );

            if ( iGpxVersion < EGpxVersion11 )
                {
                GenValidSequenceL( rootChildren10, rootChildren10Size, aChildTags );
                }
            else
                {
                GenValidSequenceL( rootChildren11, rootChildren11Size, aChildTags );
                }
            break;
            }
        case EPosXmlLandmark:
            {
            // v1.0
            const TPosXmlTagType landmarkChildren10[] = { 
                EPosXmlCoordAltitude, 
                EPosXmlLmName, EPosXmlLmDescription2, EPosXmlLmDescription,
                EPosXmlMediaLinkUrl, EPosXmlMediaLinkName, EPosXmlCategoryName, 
                EPosXmlCoordHorizontalAcc, EPosXmlCoordVerticalAcc 
                };
            const TInt landmarkChildren10Size = sizeof( landmarkChildren10 ) / sizeof( TPosXmlTagType );

            // v1.1
            const TPosXmlTagType landmarkChildren11[] = { 
                EPosXmlCoordAltitude, 
                EPosXmlLmName, EPosXmlLmDescription2, EPosXmlLmDescription,
                EPosXmlLmMediaLink, EPosXmlLmMediaLink, // link can repeat
                EPosXmlCategoryName, EPosXmlCoordHorizontalAcc, EPosXmlCoordVerticalAcc 
                };
            const TInt landmarkChildren11Size = sizeof( landmarkChildren11 ) / sizeof( TPosXmlTagType );
            
            if ( iGpxVersion < EGpxVersion11 )
                {
                GenValidSequenceL( landmarkChildren10, landmarkChildren10Size, aChildTags );
                }
            else
                {
                GenValidSequenceL( landmarkChildren11, landmarkChildren11Size, aChildTags );
                }
            break;
            }
        case EPosXmlMetadata: // v1.1
            {
            const TPosXmlTagType metaChildren[] = { 
                EPosXmlLmCollName, EPosXmlLmCollDescription
                };
            const TInt metaChildrenSize = sizeof( metaChildren ) / sizeof( TPosXmlTagType );
            
            GenValidSequenceL( metaChildren, metaChildrenSize, aChildTags );
            break;
            }
        case EPosXmlLmMediaLink: // v1.1
            {
            const TPosXmlTagType linkChildren[] = { 
                EPosXmlMediaLinkName, EPosXmlMediaLinkMime
                };
            const TInt linkChildrenSize = sizeof( linkChildren ) / sizeof( TPosXmlTagType );
            
            GenValidSequenceL( linkChildren, linkChildrenSize, aChildTags );
            break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmGpxParser::CheckMandatoryTagsL( TPosXmlTagType /*aTreeType */ )
    {
    // GPX has only mandatory attributes, which is checked in OnAttributesL
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmGpxParser::SetContentL( TPosXmlTagType aTag, const TDesC& aContent )
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
            if ( !Math::IsNaN( iParsedLatitude ) && !Math::IsNaN( iParsedLongitude ) )
                {
                TLocality loc(
                    TCoordinate(
                        iParsedLatitude, 
                        iParsedLongitude,
                        iParsedAltitude), 
                    iParsedHorAcc, 
                    iParsedVertAcc);
                
                iWorkingLandmark->SetPositionL(loc);
                }

            TBuf<5> num;
            num.Num(iNoOfMediaLinks);
            iWorkingLandmark->SetPositionFieldL(EPositionFieldMediaLinks, num);

            // v1.0 media links
            if ( iReadUrl && iGpxVersion == EGpxVersion10 )
                {
                SetContentL( EPosXmlLmMediaLink, KNullDesC );
                }
            
            HandleLandmarkParsedL();
            break;
            }
        case EPosXmlLmName:
            iWorkingLandmark->SetLandmarkNameL( aContent.Left( KPosLmMaxTextFieldLength ) );
            break;
        case EPosXmlLmDescription:
            iWorkingLandmark->SetLandmarkDescriptionL( aContent.Left( KPosLmMaxDescriptionLength ) );
            break;
        case EPosXmlLmDescription2:
            iWorkingLandmark->SetPositionFieldL( EPositionFieldComment, aContent.Left( KPosLmMaxTextFieldLength ) );
            break;
        
        case EPosXmlCoordAltitude:
            PosLmConverterUtils::DesToRealL(aContent, iParsedAltitude);
            break;
        case EPosXmlCoordHorizontalAcc:
            PosLmConverterUtils::DesToRealL(aContent, iParsedHorAcc);

            // Ensure horizontal acc is >= 0
            PosLmConverterUtils::AssertL(
                iParsedHorAcc >= 0 || Math::IsNaN( iParsedHorAcc ) );
            
            iParsedHorAcc *= KPosLmDopToAccuracyFactor;
            break;
        case EPosXmlCoordVerticalAcc:
            PosLmConverterUtils::DesToRealL( aContent, iParsedVertAcc );

            // Ensure vertical acc is >= 0
            PosLmConverterUtils::AssertL(
                iParsedVertAcc >= 0 || Math::IsNaN( iParsedVertAcc ) );

            iParsedVertAcc *= KPosLmDopToAccuracyFactor;
            break;

        // mediaLink Tag
        case EPosXmlMediaLinkUrl:
            PosLmConverterUtils::AssertL( PosLmConverterUtils::IsAnyUri( aContent ) );
            iReadUrl = aContent.AllocL();
            break;
        case EPosXmlMediaLinkMime:
            iReadMimeType = aContent.AllocL();
            break;
        case EPosXmlLmMediaLink:
            {
            HBufC* mediaLink = PosLmConverterUtils::ConcatenateMediaLinkLC(
                *iReadMimeType, *iReadUrl );
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
        case EPosXmlCategoryName:
            PosLmImplExtension::SetCategoryIdL( *iWorkingCategory, iWorkingItemId );

            PosLmConverterUtils::AssertL( aContent.Length() > 0 );
            iWorkingCategory->SetCategoryNameL( aContent.Left( KPosLmMaxCategoryNameLength ) );

            iWorkingLandmark->AddCategoryL( iWorkingItemId++ );
            User::LeaveIfError( iCategories.Append( iWorkingCategory ) );
            iWorkingCategory = NULL;

            break;

        // Attributes    
        case EPosXmlCoordLatitude: // WayPoint attribute
            PosLmConverterUtils::DesToRealL(aContent, iParsedLatitude);

            // Ensure latitude is in valid range
            PosLmConverterUtils::AssertL(
                (iParsedLatitude <= KPosLmParserMaxLatitude &&
                iParsedLatitude >= KPosLmParserMinLatitude) ||
                Math::IsNaN(iParsedLatitude));
            break;

        case EPosXmlCoordLongitude: // WayPoint attribute
            PosLmConverterUtils::DesToRealL(aContent, iParsedLongitude);

            // Ensure latitude is in valid range
            PosLmConverterUtils::AssertL(
                (iParsedLongitude <= KPosLmParserMaxLongitude &&
                iParsedLongitude >= KPosLmParserMinLongitude) ||
                Math::IsNaN(iParsedLongitude));
            break;

        case EPosXmlVersion: 
            if ( aContent.Compare( KPosXmlVersionNum10 ) == 0 )
                {
                iGpxVersion = EGpxVersion10;
                }
            else // anything else is used as 1.1
                {
                iGpxVersion = EGpxVersion11;
                }
            break;

        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmGpxParser::IsLandmarkStarted( TPosXmlTagType aTag )
    {
    return ( aTag == EPosXmlLandmark );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmGpxParser::IsCategoryStarted( TPosXmlTagType aTag )
    {
    return ( aTag == EPosXmlCategoryName );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CPosLmGpxParser::IsTreeTag( TPosXmlTagType aTag )
    {
    __ASSERT_DEBUG( aTag != EPosXmlNothing, Panic( KPosLmConverterPanic, KErrGeneral ) );
    __ASSERT_DEBUG( aTag != EPosXmlUnknown, Panic( KPosLmConverterPanic, KErrGeneral ) );
    
    return ( aTag >= EPosXmlLmLastTree ) ? ETrue : EFalse;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosXmlTagType CPosLmGpxParser::AttributeTypeL( TPosXmlTagType aParent, const Xml::RTagInfo& aElement )
    {
    TPosXmlTagType tagType = EPosXmlUnknown;
    
    TPtrC8 tagName = aElement.LocalName().DesC();
    
    switch ( aParent )
        {
        case EPosXmlRoot:
            {
            if ( EqualsTagL( tagName, KPosXmlVersion ) )
                {
                tagType = EPosXmlVersion;
                }
            break;
            }
        case EPosXmlLandmark:
            {
            if ( EqualsTagL( tagName, KPosXmlGpxLmLatitude ) )
                {
                tagType = EPosXmlCoordLatitude;
                }
            else if ( EqualsTagL( tagName, KPosXmlGpxLmLongitude ) )
                {
                tagType = EPosXmlCoordLongitude;
                }
            break;
            }
        case EPosXmlLmMediaLink: // v1.1
            {
            if ( iGpxVersion >= EGpxVersion11 )
                {
                if ( EqualsTagL( tagName, KPosXmlGpxLinkHref ) )
                    {
                    tagType = EPosXmlMediaLinkUrl;
                    }
                }
            break;
            }
        default:
            break;
        }
    return tagType;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TPosXmlTagType CPosLmGpxParser::TagTypeL( const Xml::RTagInfo& aElement )
    {
    TPosXmlTagType tagType = EPosXmlUnknown;
    TPosXmlTagType treeType = CurrentTree();
    
    TPtrC8 tagName = aElement.LocalName().DesC();
    
    if ( treeType == EPosXmlNothing )
        {
        if ( EqualsTagL( tagName, KPosXmlGpxRoot ) )
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
                if ( EqualsTagL( tagName, KPosXmlGpxMetadata ) )
                    {
                    tagType = EPosXmlMetadata;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxWaypoint ) )
                    {
                    tagType = EPosXmlLandmark;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxCollName ) && 
                          iGpxVersion == EGpxVersion10 ) // v1.0
                    {
                    tagType = EPosXmlLmCollName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxCollDesc ) && 
                          iGpxVersion == EGpxVersion10 ) // v1.0
                    {
                    tagType = EPosXmlLmCollDescription;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxRoot ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                }
                break;
                
            case EPosXmlMetadata:
                {
                if ( EqualsTagL( tagName, KPosXmlGpxCollName ) && 
                     iGpxVersion >= EGpxVersion11 ) // v1.1
                    {
                    tagType = EPosXmlLmCollName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxCollDesc ) && 
                          iGpxVersion == EGpxVersion11 ) // v1.1
                    {
                    tagType = EPosXmlLmCollDescription;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxMetadata ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                }
                break;
                
            case EPosXmlLandmark:
                {
                if ( EqualsTagL( tagName, KPosXmlGpxLmName ) ) 
                    {
                    tagType = EPosXmlLmName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxLmDescription ) )
                    {
                    tagType = EPosXmlLmDescription;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxLmComment ) ) 
                    {
                    tagType = EPosXmlLmDescription2;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxLmAltitude ) ) 
                    {
                    tagType = EPosXmlCoordAltitude;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxHdop ) ) 
                    {
                    tagType = EPosXmlCoordHorizontalAcc;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxVdop ) ) 
                    {
                    tagType = EPosXmlCoordVerticalAcc;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxCategory ) ) 
                    {
                    tagType = EPosXmlCategoryName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxLink ) ) 
                    {
                    tagType = EPosXmlLmMediaLink;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxUrl ) && 
                          iGpxVersion == EGpxVersion10 )  // v1.0
                    {
                    tagType = EPosXmlMediaLinkUrl;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxUrlName ) && 
                          iGpxVersion == EGpxVersion10 ) // v1.0
                    {
                    tagType = EPosXmlMediaLinkName;
                    }
                else if ( EqualsTagL( tagName, KPosXmlGpxWaypoint ) )
                    {
                    User::Leave( EXmlRecursiveEntity );
                    }
                }
                break;
                
            case EPosXmlLmMediaLink: // v1.1
                {
                if ( iGpxVersion >= EGpxVersion11 )
                    {
                    if ( EqualsTagL( tagName, KPosXmlGpxLinkType ) )
                        {
                        tagType = EPosXmlMediaLinkMime;
                        }
                    else if ( EqualsTagL( tagName, KPosXmlGpxLinkText ) )
                        {
                        tagType = EPosXmlMediaLinkName;
                        }
                    else if ( EqualsTagL( tagName, KPosXmlGpxLink ) )
                        {
                        User::Leave( EXmlRecursiveEntity );
                        }
                    }
                }
                break;
                
            default:
                break; // EPosXmlUnknown
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
void CPosLmGpxParser::OnAttributesL( 
    TPosXmlTagType aParent,
    const Xml::RAttributeArray& aAttributes )
    {
    switch ( aParent )
        {
        case EPosXmlRoot:
            {
            for ( TInt i = 0; i < aAttributes.Count(); ++i )
                {
                TPosXmlTagType attribute = AttributeTypeL( aParent, aAttributes[i].Attribute() );
                if ( attribute == EPosXmlVersion )
                    {
                    HBufC* value = PosLmConverterUtils::Utf8ToUnicodeLC( aAttributes[i].Value().DesC() );
                    SetContentL( EPosXmlVersion, *value );
                    CleanupStack::PopAndDestroy( value );
                    }
                }
            PosLmConverterUtils::AssertL( iGpxVersion > EGpxVersionUnknown );
            }
            break;
            
        case EPosXmlLandmark:
            {
            for ( TInt i = 0; i < aAttributes.Count(); ++i )
                {
                TPosXmlTagType attribute = AttributeTypeL( aParent, aAttributes[i].Attribute() );
                
                if ( attribute == EPosXmlCoordLatitude ||
                     attribute == EPosXmlCoordLongitude )
                    {
                    HBufC* value = PosLmConverterUtils::Utf8ToUnicodeLC( aAttributes[i].Value().DesC() );
                    SetContentL( attribute, *value );
                    CleanupStack::PopAndDestroy( value );
                    }
                }

            // both lat and lon must be present
            PosLmConverterUtils::AssertL( 
                !Math::IsNaN( iParsedLatitude ) && 
                !Math::IsNaN( iParsedLongitude ) );
            }
            break;
            
        case EPosXmlLmMediaLink:
            {
            TBool isUrl( EFalse );
            for ( TInt i = 0; i < aAttributes.Count(); ++i )
                {
                TPosXmlTagType attribute = AttributeTypeL( aParent, aAttributes[i].Attribute() );
                if ( attribute == EPosXmlMediaLinkUrl )
                    {
                    HBufC* value = PosLmConverterUtils::Utf8ToUnicodeLC( aAttributes[i].Value().DesC() );
                    SetContentL( EPosXmlMediaLinkUrl, *value );
                    CleanupStack::PopAndDestroy( value );
                    isUrl = ETrue;
                    }
                }
            PosLmConverterUtils::AssertL( isUrl );
            }
            break;
            
        default:
            break;
        }
    }
