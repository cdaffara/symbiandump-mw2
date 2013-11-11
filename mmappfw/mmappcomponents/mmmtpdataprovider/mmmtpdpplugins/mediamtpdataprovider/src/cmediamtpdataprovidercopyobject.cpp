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
* Description:  Implement the operation: Copy Object
*
*/


#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>

#include "cmediamtpdataprovidercopyobject.h"
#include "mediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderCopyObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CMediaMtpDataProviderCopyObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CMediaMtpDataProviderCopyObject* self =
        new ( ELeave ) CMediaMtpDataProviderCopyObject( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderCopyObject::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderCopyObject::ConstructL()
    {
    CCopyObject::ConstructL();
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderCopyObject::~CMediaMtpDataProviderCopyObject
// Destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderCopyObject::~CMediaMtpDataProviderCopyObject()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderCopyObject::CMediaMtpDataProviderCopyObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderCopyObject::CMediaMtpDataProviderCopyObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CCopyObject( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL
//
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderCopyObject::ServiceGetSpecificObjectPropertyL( TUint16 aPropCode,
    TUint32 aHandle,
    const CMTPObjectMetaData& aObject )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL" ) );

    CMTPTypeString* textData = NULL;
    CMTPTypeArray* desData = NULL;
    TInt err = KErrNone;

    if ( iPropertyElement )
        {
        delete iPropertyElement;
        iPropertyElement = NULL;
        }

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeArtist:
        case EMTPObjectPropCodeGenre:
        case EMTPObjectPropCodeComposer:
        case EMTPObjectPropCodeOriginalReleaseDate:
        case EMTPObjectPropCodeAlbumName:
        case EMTPObjectPropCodeParentalRating:
        case EMTPObjectPropCodeEncodingProfile:
            {
            textData = CMTPTypeString::NewLC();   // + textData

            TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
                *textData,
                aObject ) );

            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL err = %d" ), err );

            if ( err == KErrNone )
                {
                iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
                iPropertyElement->SetStringL( CMTPTypeObjectPropListElement::EValue, textData->StringChars());
                }

            CleanupStack::PopAndDestroy( textData );  // - textData
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            desData = CMTPTypeArray::NewLC( EMTPTypeAUINT16 );   // + desData

            TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
                *desData,
                aObject ) );

            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL err = %d" ), err );

            if ( err == KErrNone )
                {
                iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
                iPropertyElement->SetArrayL( CMTPTypeObjectPropListElement::EValue, *desData);
                }

            CleanupStack::PopAndDestroy( desData );  // - desData
            }
            break;

        case EMTPObjectPropCodeTrack:
        case EMTPObjectPropCodeNumberOfChannels:
        case EMTPObjectPropCodeScanType:
        case EMTPObjectPropCodeDRMStatus:
            {
            TMTPTypeUint16 uint16( 0 );
            TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
                uint16,
                aObject ) );
            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL err = %d" ), err );

            if ( err == KErrNone )
                {
                iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
                iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EValue, uint16.Value());
                }
            }
            break;

        case EMTPObjectPropCodeWidth:
        case EMTPObjectPropCodeHeight:
        case EMTPObjectPropCodeDuration:
        case EMTPObjectPropCodeSampleRate:
        case EMTPObjectPropCodeAudioWAVECodec:
        case EMTPObjectPropCodeAudioBitRate:
        case EMTPObjectPropCodeVideoFourCCCodec:
        case EMTPObjectPropCodeVideoBitRate:
        case EMTPObjectPropCodeFramesPerThousandSeconds:
        case EMTPObjectPropCodeKeyFrameDistance:
            {
            TMTPTypeUint32 uint32 = 0;
            TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
                uint32,
                aObject ) );
            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL err = %d" ), err );

            if ( err == KErrNone )
                {
                iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
                iPropertyElement->SetUint32L( CMTPTypeObjectPropListElement::EValue, uint32.Value());
                }
            }
            break;

        case EMTPExtObjectPropCodeOmaDrmStatus:
            {
            TInt drmStatus = MmMtpDpUtility::GetDrmStatus( aObject.DesC( CMTPObjectMetaData::ESuid ) );
            TMTPTypeUint8 result;
            result.Set( 0 );

            if ( drmStatus == EMTPDrmStatusProtected )
                result.Set( 1 );

            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EDatatype, EMTPTypeUINT8 );
            iPropertyElement->SetUint8L( CMTPTypeObjectPropListElement::EValue, result.Value());
            }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    // centralize logic to handle error, doing so, that's on need to trap the
    // Ignore all errors came from mpx db.
    // It's enough that only file copied successfully no matter metadata got whatever failure.
    if ( err != KErrNone )
        {
        iPropertyElement = NULL;
        }
    PRINT( _L( "MM MTP <= CMediaMtpDataProviderCopyObject::ServiceSpecificObjectPropertyL" ) );
    }

// end of file
