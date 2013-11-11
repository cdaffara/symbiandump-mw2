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
* Description:  Implement the operation: getobjectproplist
*
*/


#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypestring.h>
#include <mtp/cmtptypearray.h>

#include "cmediamtpdataprovidergetobjectproplist.h"
#include "mediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdputility.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropList::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CMediaMtpDataProviderGetObjectPropList::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CMediaMtpDataProviderGetObjectPropList* self =
        new ( ELeave ) CMediaMtpDataProviderGetObjectPropList( aFramework, aConnection, aDpConfig );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropList::~CMediaMtpDataProviderGetObjectPropList
// Destructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderGetObjectPropList::~CMediaMtpDataProviderGetObjectPropList()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropList::CMediaMtpDataProviderGetObjectPropList
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CMediaMtpDataProviderGetObjectPropList::CMediaMtpDataProviderGetObjectPropList( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CGetObjectPropList( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CMediaMtpDataProviderGetObjectPropList::ConstructL
// 2nd Phase Constructor
// -----------------------------------------------------------------------------
//
void CMediaMtpDataProviderGetObjectPropList::ConstructL()
    {
    CGetObjectPropList::ConstructL();
    }

TInt CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL( TUint16 aPropCode,
    TUint32 aHandle )
    {
    PRINT( _L( "MM MTP => CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL" ) );

    CMTPTypeString* textData = NULL;
    CMTPTypeArray* desData = NULL;
    TInt err = KErrNone;

    if ( iPropertyElement != NULL )
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
                *iObject ) );

            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL err = %d" ), err );

            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetStringL( CMTPTypeObjectPropListElement::EValue, textData->StringChars());

            CleanupStack::PopAndDestroy( textData );  // - textData
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            desData = CMTPTypeArray::NewLC( EMTPTypeAUINT16 );   // + desData

            TRAP( err, iDpConfig.GetWrapperL().GetObjectMetadataValueL( aPropCode,
                *desData,
                *iObject ) );

            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL err = %d" ), err );

            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetArrayL( CMTPTypeObjectPropListElement::EValue, *desData);
 
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
                *iObject ) );
            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL err = %d" ), err );

            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EValue, uint16.Value());
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
                *iObject ) );
            PRINT1( _L( "MM MTP <> CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL err = %d" ), err );

            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetUint32L( CMTPTypeObjectPropListElement::EValue, uint32.Value());
            }
            break;

        case EMTPExtObjectPropCodeOmaDrmStatus:
            {
            TInt drmStatus = MmMtpDpUtility::GetDrmStatus( iObject->DesC( CMTPObjectMetaData::ESuid ) );
            TMTPTypeUint8 result;
            result.Set( EMTPDrmStatusNotProtected );

            if ( drmStatus == EMTPDrmStatusProtected )
                result.Set( EMTPDrmStatusProtected );

            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EDatatype, EMTPTypeUINT8);
            iPropertyElement->SetUint8L( CMTPTypeObjectPropListElement::EValue, result.Value());
            }
            break;

        default:
            {
            iPropertyElement = NULL;
            err = KErrNotSupported;
            }
            break;
        }

    PRINT( _L( "MM MTP <= CMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL" ) );

    return err;
    }

// end of file
