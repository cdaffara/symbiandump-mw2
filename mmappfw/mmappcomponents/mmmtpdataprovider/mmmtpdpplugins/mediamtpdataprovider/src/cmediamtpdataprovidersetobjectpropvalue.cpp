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
* Description:  Implement opeartion setobjectpropvalue
*
*/


#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/mmtpobjectmgr.h>

#include "cmediamtpdataprovidersetobjectpropvalue.h"
#include "mediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdputility.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdpdefs.h"

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderSetObjectPropValue::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CMediaMtpDataProviderSetObjectPropValue::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CMediaMtpDataProviderSetObjectPropValue* self =
        new ( ELeave ) CMediaMtpDataProviderSetObjectPropValue( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderSetObjectPropValue::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderSetObjectPropValue::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderSetObjectPropValue::CMediaMtpDataProviderSetObjectPropValue
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderSetObjectPropValue::CMediaMtpDataProviderSetObjectPropValue( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CSetObjectPropValue( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderSetObjectPropValue::~CMediaMtpDataProviderSetObjectPropValue
// Destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderSetObjectPropValue::~CMediaMtpDataProviderSetObjectPropValue()
    {
    // Do nothing
    }

TBool CMediaMtpDataProviderSetObjectPropValue::IsSpecificPropCodeReadOnly( TUint16 aPropCode ) const
    {
    TBool returnCode = EFalse;

    if ( aPropCode == EMTPObjectPropCodeVideoBitRate )
        returnCode = ETrue;

    return returnCode;
    }

void CMediaMtpDataProviderSetObjectPropValue::ReceiveDataSpecificObjectPropertyL( TUint16 aPropCode )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderSetObjectPropValue::RecieveDataSpecificObjectPropertyL" ) );

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
            delete iMTPTypeString;
            iMTPTypeString = NULL;
            iMTPTypeString = CMTPTypeString::NewL();
            ReceiveDataL( *iMTPTypeString );
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            delete iMTPTypeArray;
            iMTPTypeArray = NULL;
            iMTPTypeArray = CMTPTypeArray::NewL( EMTPTypeAUINT16 );
            ReceiveDataL( *iMTPTypeArray );
            }
            break;

        //Get Data for Int16 objects
        case EMTPObjectPropCodeTrack: // 0xDC8B
        case EMTPObjectPropCodeDRMStatus: // 0xDC9D
        case EMTPObjectPropCodeNumberOfChannels: // 0xDE94
        case EMTPObjectPropCodeScanType: // 0xDE97
            ReceiveDataL( iMTPTypeUint16 );
            break;

        //Get Data for Int32 objects
        case EMTPObjectPropCodeWidth: // 0xDC87
        case EMTPObjectPropCodeHeight: // 0xDC88
        case EMTPObjectPropCodeDuration: // 0xDC89
        case EMTPObjectPropCodeSampleRate: // 0xDE93
        case EMTPObjectPropCodeAudioWAVECodec: // 0xDE99
        case EMTPObjectPropCodeAudioBitRate: // 0xDE9A
        case EMTPObjectPropCodeVideoFourCCCodec: // 0xDE9B
        case EMTPObjectPropCodeFramesPerThousandSeconds: // 0xDE9D
        case EMTPObjectPropCodeKeyFrameDistance: // 0xDE9E
            ReceiveDataL( iMTPTypeUint32 );
            break;

        case EMTPExtObjectPropCodeOmaDrmStatus:
            ReceiveDataL( iMTPTypeUint8 );
            break;

        default:
            User::Leave( KErrNotSupported );
            break;
        }

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderSetObjectPropValue::RecieveDataSpecificObjectPropertyL" ) );
    }

TMTPResponseCode CMediaMtpDataProviderSetObjectPropValue::ServiceSpecificObjectPropertyL( TUint16 aPropCode )
    {
    TMTPResponseCode responseCode = EMTPRespCodeOK;

    switch ( aPropCode )
        {
        case EMTPObjectPropCodeArtist: // 0xDC46
        case EMTPObjectPropCodeGenre: //0xDC8C
        case EMTPObjectPropCodeParentalRating: //0xDC94
        case EMTPObjectPropCodeComposer: //0xDC96
        case EMTPObjectPropCodeOriginalReleaseDate: //0xDC99
        case EMTPObjectPropCodeAlbumName: //0xDC9A
        case EMTPObjectPropCodeEncodingProfile: //0xDEA1
            {
            responseCode = ServiceMetaDataToWrapperL( iPropCode,
                *iMTPTypeString,
                *iObjectInfo );
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            responseCode = ServiceMetaDataToWrapperL( EMTPObjectPropCodeDescription,
                *iMTPTypeArray,
                *iObjectInfo );
            }
            break;

        case EMTPObjectPropCodeWidth: //0xDC87
        case EMTPObjectPropCodeHeight: //0xDC88
        case EMTPObjectPropCodeDuration: //0xDC89
        case EMTPObjectPropCodeSampleRate: //0xDE93
        case EMTPObjectPropCodeAudioWAVECodec: //0xDE99
        case EMTPObjectPropCodeAudioBitRate: //0xDE9A
        case EMTPObjectPropCodeVideoFourCCCodec: //0xDE9B
        case EMTPObjectPropCodeFramesPerThousandSeconds: //0xDE9D
        case EMTPObjectPropCodeKeyFrameDistance: //0xDE9E
            {
            responseCode = ServiceMetaDataToWrapperL( iPropCode,
                iMTPTypeUint32,
                *iObjectInfo );
            }
            break;

        case EMTPObjectPropCodeTrack: // 0xDC8B
        case EMTPObjectPropCodeDRMStatus: // 0xDC9D
        case EMTPObjectPropCodeNumberOfChannels: // 0xDE94
        case EMTPObjectPropCodeScanType: // 0xDE97
            {
            responseCode = ServiceMetaDataToWrapperL( iPropCode,
                iMTPTypeUint16,
                *iObjectInfo );
            }
            break;

        case EMTPExtObjectPropCodeOmaDrmStatus:
            {
            CMTPObjectMetaData& aObject( *iObjectInfo );
            TInt drmStatus = MmMtpDpUtility::GetDrmStatus( aObject.DesC( CMTPObjectMetaData::ESuid ) );

            if (drmStatus == EMTPDrmStatusUnknown)
                {
                responseCode = EMTPRespCodeAccessDenied;
                }
            else
                {
                // there's no DB field to remember the value, so return an error
                // if there's a mismatch to CAF protection status
                if ( ( ( drmStatus == EMTPDrmStatusProtected) && ( iMTPTypeUint8.Value() == 0 ) ) ||
                    ( ( drmStatus == EMTPDrmStatusNotProtected ) && ( iMTPTypeUint8.Value() == 1 ) ) )
                    {
                    responseCode = EMTPRespCodeAccessDenied;
                    }
                }
            }
            break;

        default:
            {
            User::Leave( KErrNotSupported );
            }
            break;
        }

    return responseCode;
    }

// end of file
