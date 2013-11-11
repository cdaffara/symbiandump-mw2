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
* Description:  Implement the operation: getobjectpropvalue
*
*/


#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>

#include "cmediamtpdataprovidergetobjectpropvalue.h"
#include "mediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropValue::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CMediaMtpDataProviderGetObjectPropValue::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CMediaMtpDataProviderGetObjectPropValue* self =
        new ( ELeave ) CMediaMtpDataProviderGetObjectPropValue( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropValue::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderGetObjectPropValue::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropValue::~CMediaMtpDataProviderGetObjectPropValue
// Destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderGetObjectPropValue::~CMediaMtpDataProviderGetObjectPropValue()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropValue::CMediaMtpDataProviderGetObjectPropValue
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderGetObjectPropValue::CMediaMtpDataProviderGetObjectPropValue( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CGetObjectPropValue( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

void CMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectPropertyL( TUint16 aPropCode )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectPropertyL" ) );
    switch ( aPropCode )
        {
        case EMTPObjectPropCodeArtist: // 0xDC46
        case EMTPObjectPropCodeGenre: // 0xDC8C
        case EMTPObjectPropCodeParentalRating: // 0xDC94
        case EMTPObjectPropCodeComposer: // 0xDC96
        case EMTPObjectPropCodeOriginalReleaseDate: // 0xDC99
        case EMTPObjectPropCodeAlbumName: // 0xDC9A
        case EMTPObjectPropCodeEncodingProfile: // 0xDEA1
            {
            if ( iMTPTypeString != NULL )
                {
                delete iMTPTypeString;
                iMTPTypeString = NULL;
                }

            iMTPTypeString = CMTPTypeString::NewL();
            ServiceMetaDataFromWrapperL( aPropCode, *iMTPTypeString, *iObjectInfo );
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            if ( iMTPTypeArray != NULL )
                {
                delete iMTPTypeArray;
                iMTPTypeArray = NULL;
                }

            iMTPTypeArray = CMTPTypeArray::NewL( EMTPTypeAUINT16 );
            ServiceMetaDataFromWrapperL( aPropCode, *iMTPTypeArray, *iObjectInfo );
            }
            break;

        case EMTPObjectPropCodeWidth: // 0xDC87
        case EMTPObjectPropCodeHeight: // 0xDC88
        case EMTPObjectPropCodeDuration: // 0xDC89
        case EMTPObjectPropCodeSampleRate: // 0xDE93
        case EMTPObjectPropCodeAudioWAVECodec: // 0xDE99
        case EMTPObjectPropCodeAudioBitRate: // 0xDE9A
        case EMTPObjectPropCodeVideoFourCCCodec: // 0xDE9B
        case EMTPObjectPropCodeVideoBitRate: // 0xDE9C
        case EMTPObjectPropCodeFramesPerThousandSeconds: // 0xDE9D
        case EMTPObjectPropCodeKeyFrameDistance: // 0xDE9E
            {
            iMTPTypeUint32.Set( 0 );
            ServiceMetaDataFromWrapperL( aPropCode, iMTPTypeUint32, *iObjectInfo );
            }
            break;

        case EMTPObjectPropCodeTrack: // 0xDC8B
        case EMTPObjectPropCodeDRMStatus: // 0xDC9D
        case EMTPObjectPropCodeNumberOfChannels: // 0xDE94
        case EMTPObjectPropCodeScanType: // 0xDE97
            {
            iMTPTypeUint16.Set( 0 );
            ServiceMetaDataFromWrapperL( aPropCode, iMTPTypeUint16, *iObjectInfo );

            PRINT1( _L( "MM MTP <> CGetObjectPropValue::ServiceL value = %d" ), iMTPTypeUint16.Value() );
            }
            break;

        case EMTPExtObjectPropCodeOmaDrmStatus:
            {
            TInt drmStatus = MmMtpDpUtility::GetDrmStatus( iObjectInfo->DesC( CMTPObjectMetaData::ESuid ) );
            iMTPTypeUint8.Set( 0 );

            if ( drmStatus == EMTPDrmStatusProtected )
                iMTPTypeUint8.Set( 1 );

            SendDataL( iMTPTypeUint8 );
            }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectPropertyL" ) );
    }

// end of file
