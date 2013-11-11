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
 * Description:
 *
 */

#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>
#include <mtp/tmtptypeuint8.h>
#include <mtp/tmtptypeuint16.h>
#include <mtp/tmtptypeuint32.h>
#include <mtp/mtpdatatypeconstants.h>
#include <mtp/cmtpobjectmetadata.h>

#include "tobjectdescription.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdputility.h"

#include "cmediamtpdataproviderpropertysettingutility.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderPropertySettingUtility::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderPropertySettingUtility* CMediaMtpDataProviderPropertySettingUtility::NewL()
    {
    CMediaMtpDataProviderPropertySettingUtility* self =
        new ( ELeave ) CMediaMtpDataProviderPropertySettingUtility();

    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderPropertySettingUtility::~CMediaMtpDataProviderPropertySettingUtility
// Destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderPropertySettingUtility::~CMediaMtpDataProviderPropertySettingUtility()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderSetObjectPropList::CMediaMtpDataProviderSetObjectPropList
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderPropertySettingUtility::CMediaMtpDataProviderPropertySettingUtility()
    {
    // Do nothing
    }

TMTPResponseCode CMediaMtpDataProviderPropertySettingUtility::SetSpecificObjectPropertyL( MMmMtpDpConfig& aDpConfig,
    TUint16 aPropCode,
    const CMTPObjectMetaData& aObject,
    const CMTPTypeObjectPropListElement& aElement )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderPropertySettingUtility::SetSpecificObjectPropertyL" ) );
    TMTPResponseCode responseCode( EMTPRespCodeOK );

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
            CMTPTypeString* stringData =
                CMTPTypeString::NewLC( aElement.StringL( CMTPTypeObjectPropListElement::EValue ) );// + stringData

            responseCode = SetMetaDataToWrapper( aDpConfig,
                aPropCode,
                *stringData,
                aObject );

            CleanupStack::PopAndDestroy( stringData );// - stringData
            }
            break;

        case EMTPObjectPropCodeVideoBitRate:
            /*
             spec:
             * Object properties that are get-only (0x00 GET)
             * should accept values during object creation by
             * way of the SendObjectPropList command.
             */
            break;

        case EMTPObjectPropCodeDescription:
            {
            CMTPTypeArray* desData = CMTPTypeArray::NewLC( EMTPTypeAUINT16 ); // + desData
            desData->SetByDesL( aElement.ArrayL( CMTPTypeObjectPropListElement::EValue ) );
            responseCode = SetMetaDataToWrapper( aDpConfig,
                aPropCode,
                *desData,
                aObject );
            CleanupStack::PopAndDestroy( desData ); // - desData
            }
            break;

        case EMTPObjectPropCodeWidth:
        case EMTPObjectPropCodeHeight:
        case EMTPObjectPropCodeDuration:
        case EMTPObjectPropCodeSampleRate:
        case EMTPObjectPropCodeAudioWAVECodec:
        case EMTPObjectPropCodeAudioBitRate:
        case EMTPObjectPropCodeVideoFourCCCodec:
        case EMTPObjectPropCodeFramesPerThousandSeconds:
        case EMTPObjectPropCodeKeyFrameDistance:
            {
            TMTPTypeUint32 uint32( aElement.Uint32L( CMTPTypeObjectPropListElement::EValue ) );
            responseCode = SetMetaDataToWrapper( aDpConfig,
                aPropCode,
                uint32,
                aObject );
            }
            break;

        case EMTPObjectPropCodeTrack:
        case EMTPObjectPropCodeNumberOfChannels:
        case EMTPObjectPropCodeScanType:
        case EMTPObjectPropCodeDRMStatus:
            {
            TMTPTypeUint16 uint16( aElement.Uint16L( CMTPTypeObjectPropListElement::EValue ) );
            responseCode = SetMetaDataToWrapper( aDpConfig,
                aPropCode,
                uint16,
                aObject );
            }
            break;

        case EMTPExtObjectPropCodeOmaDrmStatus:
            {
            TInt drmStatus = MmMtpDpUtility::GetDrmStatus( aObject.DesC( CMTPObjectMetaData::ESuid ) );

            if ( drmStatus == EMTPDrmStatusUnknown )
                {
                responseCode = EMTPRespCodeAccessDenied;
                }
            else
                {
                TMTPTypeUint8 newValue( aElement.Uint8L( CMTPTypeObjectPropListElement::EValue ) );

                // there's no DB field to remember the value, so return an error
                // if there's a mismatch to CAF protection status
                if ( ( ( drmStatus == EMTPDrmStatusProtected )
                    && ( newValue.Value() == 0 ) )
                    || ( ( drmStatus == EMTPDrmStatusNotProtected )
                        && ( newValue.Value() == 1 ) ) )
                    {
                    responseCode = EMTPRespCodeAccessDenied;
                    }
                }
            }
            break;

        default:
            {
            // Should not happen, property code should have been checked before set to store.
            User::Leave( KErrNotSupported );
            }
            break;
        }

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderPropertySettingUtility::ServiceSpecificObjectPropertyL" ) );

    return responseCode;
    }

// end of file
