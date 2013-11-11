/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Media dp specific property description construction utility.
*              Used by all request processors which related to property description
*
*/

#include <mtp/mtpprotocolconstants.h>
#include <mtp/cmtptypeobjectpropdesc.h>
#include <mtp/cmtptypestring.h>

#include "cabstractmediamtpdataproviderdescriptionutility.h"
#include "abstractmediamtpdataproviderconst.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewL
//
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderDescriptionUtility* CAbstractMediaMtpDataProviderDescriptionUtility::NewL()
    {
    CAbstractMediaMtpDataProviderDescriptionUtility* self =
        new( ELeave ) CAbstractMediaMtpDataProviderDescriptionUtility();

    return self;
    }

CAbstractMediaMtpDataProviderDescriptionUtility::CAbstractMediaMtpDataProviderDescriptionUtility()
    {
    // Do nothing
    }

CAbstractMediaMtpDataProviderDescriptionUtility::~CAbstractMediaMtpDataProviderDescriptionUtility()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewInterdepentPropDescL
// Construct media dp specific interdependent property description.
// -----------------------------------------------------------------------------
//
CMTPTypeInterdependentPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewInterdepentPropDescL( MMmMtpDpConfig& /*aDpConfig*/,
    TUint /*aFormatCode*/ )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderDescriptionUtility::NewInterdepentPropDescL" ) );

    CMTPTypeInterdependentPropDesc* interdependentPropDesc = CMTPTypeInterdependentPropDesc::NewL();

    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderDescriptionUtility::NewInterdepentPropDescL" ) );
    return interdependentPropDesc;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewSpecificPropDescL
// Construct media dp specific property description.
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewSpecificPropDescL( TUint aFormatCode,
    TUint16 aPropCode )
    {
    CMTPTypeObjectPropDesc* propertyDesc = NULL;
    if( aFormatCode == EMTPFormatCodeAbstractAudioAlbum )
        {
        switch ( aPropCode )
            {
            case EMTPObjectPropCodeRepresentativeSampleFormat:
                propertyDesc = NewRepresentativeSampleFormatL();                   
                break;
    
            case EMTPObjectPropCodeRepresentativeSampleSize:
                propertyDesc = NewRepresentativeSampleSizeL();
                break;
    
            case EMTPObjectPropCodeRepresentativeSampleHeight:
                propertyDesc = NewRepresentativeSampleHeightL();
                break;
    
            case EMTPObjectPropCodeRepresentativeSampleWidth:
                propertyDesc = NewRepresentativeSampleWidthL();
                break;
    
            case EMTPObjectPropCodeRepresentativeSampleData:
                propertyDesc = NewRepresentativeSampleDataL();
                break;
            default:
                // do nothing
                break;
            }
        }
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleFormatL
// 
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleFormatL()
    {
    CMTPTypeObjectPropDescEnumerationForm* form =
        CMTPTypeObjectPropDescEnumerationForm::NewL( EMTPTypeUINT16 );
    CleanupStack::PushL( form ); // + form

    TInt numValues = sizeof ( TMmMtpSupportedSampleFormat ) / sizeof ( TMmMtpSupportedSampleFormat[0] );
    for ( TInt i = 0; i < numValues; i++ )
        {
        TMTPTypeUint16 data( TMmMtpSupportedSampleFormat[i] );
        form->AppendSupportedValueL( data );
        }

    CMTPTypeObjectPropDesc::TPropertyInfo propertyInfo;
    propertyInfo.iDataType = EMTPTypeUINT16;
    propertyInfo.iFormFlag = CMTPTypeObjectPropDesc::EEnumerationForm;
    propertyInfo.iGetSet = CMTPTypeObjectPropDesc::EReadOnly;
    CMTPTypeObjectPropDesc* propertyDesc = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeRepresentativeSampleFormat,
        propertyInfo,
        form );
    CleanupStack::PopAndDestroy( form ); // - form
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleSizeL
// 
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleSizeL()
    {
    CMTPTypeObjectPropDescRangeForm* form =
        CMTPTypeObjectPropDescRangeForm::NewLC( EMTPTypeUINT32 ); // + form

    // Set expected values
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMinimumValue, EMTPMinSampleSize );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMaximumValue, EMTPMaxSampleSize );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EStepSize, EMTPStepSampleSize );

    CMTPTypeObjectPropDesc::TPropertyInfo propertyInfo;
    propertyInfo.iDataType = EMTPTypeUINT32;
    propertyInfo.iFormFlag = CMTPTypeObjectPropDesc::ERangeForm;
    propertyInfo.iGetSet = CMTPTypeObjectPropDesc::EReadOnly;
    CMTPTypeObjectPropDesc* propertyDesc = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeRepresentativeSampleSize,
        propertyInfo,
        form );
    CleanupStack::PopAndDestroy( form ); // - form
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleHeightL
// 
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleHeightL()
    {
    CMTPTypeObjectPropDescRangeForm* form =
        CMTPTypeObjectPropDescRangeForm::NewLC( EMTPTypeUINT32 ); // + form

    // Set expected values
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMinimumValue, EMTPSampleMinHeight );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMaximumValue, EMTPSampleMaxHeight );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EStepSize, EMTPSampleStepHeight );

    CMTPTypeObjectPropDesc::TPropertyInfo propertyInfo;
    propertyInfo.iDataType = EMTPTypeUINT32;
    propertyInfo.iFormFlag = CMTPTypeObjectPropDesc::ERangeForm;
    propertyInfo.iGetSet = CMTPTypeObjectPropDesc::EReadOnly;
    CMTPTypeObjectPropDesc* propertyDesc = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeRepresentativeSampleHeight,
        propertyInfo,
        form );
    CleanupStack::PopAndDestroy( form ); // - form
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleWidthL
// 
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleWidthL()
    {
    CMTPTypeObjectPropDescRangeForm* form =
        CMTPTypeObjectPropDescRangeForm::NewLC( EMTPTypeUINT32 ); // + form

    // Set expected values
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMinimumValue, EMTPSampleMinWidth );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EMaximumValue, EMTPSampleMaxWidth );
    form->SetUint32L( CMTPTypeObjectPropDescRangeForm::EStepSize, EMTPSampleStepWidth );

    CMTPTypeObjectPropDesc::TPropertyInfo propertyInfo;
    propertyInfo.iDataType = EMTPTypeUINT32;
    propertyInfo.iFormFlag = CMTPTypeObjectPropDesc::ERangeForm;
    propertyInfo.iGetSet = CMTPTypeObjectPropDesc::EReadOnly;
    CMTPTypeObjectPropDesc* propertyDesc = CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeRepresentativeSampleWidth,
        propertyInfo,
        form );
    CleanupStack::PopAndDestroy( form ); // - form
    return propertyDesc;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleDataL
// 
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CAbstractMediaMtpDataProviderDescriptionUtility::NewRepresentativeSampleDataL() 
    {
    return CMTPTypeObjectPropDesc::NewL( EMTPObjectPropCodeRepresentativeSampleData );
    }

// end of file
