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
* Description:  Implementation of playlist recognizer
*
*/

// Version : %version: bh1mmcf#4 %




// INCLUDE FILES
#include <implementationproxy.h>
#include <apmrec.h>
#include <apmstd.h>
#include <f32file.h>
#include <mediarecognizer.h>

#include "playlistrecognizer.h"

#ifdef __WINDOWS_MEDIA
#include <asxparser.h>
#endif

// CONSTANTS
const TUid KUidPlaylistRecognizer = {0x101F8576};
const TInt KBufferSize = 50;
const TInt KM3UMarkerLength = 7;

const TInt KSupportedMimetypes = 11;


// Which are:
const TInt KTypeRAM = 0;
_LIT8(KDataTypeRAM,"audio/x-pn-realaudio-plugin");
const TInt KTypeRAM2 = 1;
_LIT8(KDataTypeRAM2,"audio/x-pn-realaudio");
const TInt KTypeM3U = 2;
_LIT8(KDataTypeM3U,"audio/mpegurl");
const TInt KTypeM3Ux = 3;
_LIT8(KDataTypeM3Ux,"audio/x-mpegurl");
const TInt KTypeWvxVideo = 4;
_LIT8(KDataTypeWvxVideo,"video/x-ms-wvx");
const TInt KTypeAsxFile = 5;
_LIT8(KDataTypeAsxFile,"video/x-ms-asf");

const TInt KTypeAVI = 6;
_LIT8(KDataTypeAVI,"video/avi");
const TInt KTypeMsAVI = 7;
_LIT8(KDataTypeMsAVI,"video/msvideo");
const TInt KTypeMsAVIVideo = 8;
_LIT8(KDataTypeMsAVIVideo,"video/x-msvideo");

//
// Matroska Types
//
const TInt KTypeMKAudio = 9;
_LIT8(KDataTypeMKAudio,"audio/x-matroska");

const TInt KTypeMKVideo = 10;
_LIT8(KDataTypeMKVideo,"video/x-matroska");


_LIT(KRamExt,".ram");
_LIT(KRmExt,".rm");
_LIT(KRaExt,".ra");
_LIT(KRvExt,".rv");
_LIT(KRpmExt,".rpm");
_LIT(KAsxExt,".asx");
_LIT(KAsfExt,".asf");
_LIT(KWvxExt,".wvx");
_LIT(KAviExt,".avi");
_LIT(KDivxExt,".divx");

_LIT(KM3UFileExtension,".m3u");
_LIT8(KM3UBeginMarker,"#EXTM3U");

//
// Matroska Extensions
//
_LIT(KMKAExt, ".mka");
_LIT(KMKVExt, ".mkv");

_LIT8(KMKV_HEADER_GUID, "\x6D\x61\x74\x72\x6F\x73\x6B\x61");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPlaylistRecognizer::CPlaylistRecognizer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPlaylistRecognizer::CPlaylistRecognizer()
    : CApaDataRecognizerType( KUidPlaylistRecognizer, CApaDataRecognizerType::ENormal )
{
    iCountDataTypes = KSupportedMimetypes;
}

// -----------------------------------------------------------------------------
// CPlaylistRecognizer::~CPlaylistRecognizer
// Destructor
// -----------------------------------------------------------------------------
//
CPlaylistRecognizer::~CPlaylistRecognizer()
{
}

// -----------------------------------------------------------------------------
// CPlaylistRecognizer::PreferredBufSize
// -----------------------------------------------------------------------------
//
TUint CPlaylistRecognizer::PreferredBufSize()
{
    return KBufferSize;
}

// -----------------------------------------------------------------------------
// CPlaylistRecognizer::SupportedDataTypeL
// -----------------------------------------------------------------------------
//
TDataType CPlaylistRecognizer::SupportedDataTypeL(TInt aIndex) const
{
     switch ( aIndex )
     {
        case KTypeRAM:
            return TDataType(KDataTypeRAM);
        case KTypeRAM2:
            return TDataType(KDataTypeRAM2);
        case KTypeM3U:
            return TDataType(KDataTypeM3U);
        case KTypeM3Ux:
            return TDataType(KDataTypeM3Ux);
        case KTypeWvxVideo:
            return TDataType(KDataTypeWvxVideo);
        case KTypeAsxFile:
            return TDataType(KDataTypeAsxFile);
        case KTypeAVI:
            return TDataType(KDataTypeAVI);
        case KTypeMsAVI:
            return TDataType(KDataTypeMsAVI);
        case KTypeMsAVIVideo:
            return TDataType(KDataTypeMsAVIVideo);
		case KTypeMKVideo:
		    return TDataType(KDataTypeMKVideo);
        case KTypeMKAudio:
            return TDataType(KDataTypeMKAudio);
        default:
            return TDataType();
     }
}


// -----------------------------------------------------------------------------
// CPlaylistRecognizer::DoRecognizeL
// -----------------------------------------------------------------------------
//
void CPlaylistRecognizer::DoRecognizeL( const TDesC& aName, const TDesC8& aBuffer )
{
    iConfidence = ENotRecognized;

    if ( ( aName.Right(KRamExt.iTypeLength).CompareF(KRamExt) == 0 ) ||
         ( aName.Right(KRmExt.iTypeLength).CompareF(KRmExt) == 0 ) ||
         ( aName.Right(KRaExt.iTypeLength).CompareF(KRaExt) == 0 ) ||
         ( aName.Right(KRvExt.iTypeLength).CompareF(KRvExt) == 0 ) ||
         ( aName.Right(KRpmExt.iTypeLength).CompareF(KRpmExt) == 0 ) )
    {
        //
        //  Real Media extension found, check for supported url prefix
        //
        CMediaRecognizer* helper = CMediaRecognizer::NewL();

        if ( helper->IsValidUrlPrefix( aBuffer ) )
        {
            iDataType=TDataType( KDataTypeRAM );
            iConfidence = ECertain;
        }
        else if ( aName.Right(KRamExt.iTypeLength).CompareF(KRamExt) == 0 )
        {
            //
            //  For all files with an extension of RAM,
            //  set the type to RAM file and confidence to Possible
            //  even without a supported streaming prefix
            //
            iDataType=TDataType( KDataTypeRAM );
            iConfidence = EPossible;
        }

        delete helper;
    }
    else if ( aName.Right( KM3UFileExtension.iTypeLength ).CompareF(KM3UFileExtension ) == 0 )
    {
        iDataType = TDataType( KDataTypeM3U );

        if ( aBuffer.Length() > KM3UMarkerLength &&
             aBuffer.Left( KM3UMarkerLength ).FindF( KM3UBeginMarker ) != KErrNotFound )
        {
            iConfidence = ECertain;
        }
        else
        {
            iConfidence = EProbable;
        }
    }
    else if ( ( aName.Right(KAviExt.iTypeLength).CompareF(KAviExt) == 0 ) ||
              ( aName.Right(KDivxExt.iTypeLength).CompareF(KDivxExt) == 0 ) )
    {
        iDataType=TDataType( KDataTypeAVI );
        iConfidence = ECertain;
    }
#ifdef __WINDOWS_MEDIA
    //
    //  According to Microsoft MimeTypes documention:
    //  File extension (asx or asf) has associated mimetype (video/x-ms-asf)
    //  File extension (wvx) has associated mimetype (video/x-ms-wvx)
    //
    else if ( ( aName.Right(KAsxExt.iTypeLength).CompareF(KAsxExt) == 0 ) ||
              ( aName.Right(KAsfExt.iTypeLength).CompareF(KAsfExt) == 0 ) )
    {
        iDataType = TDataType ( KDataTypeAsxFile );
        iConfidence = ECertain;
    }
    else if ( aName.Right(KWvxExt.iTypeLength).CompareF(KWvxExt) == 0 )
    {
        iDataType = TDataType ( KDataTypeWvxVideo );
        iConfidence = ECertain;
    }
#endif

    //
	// Check for Matroska
	//
    if (iConfidence == CApaDataRecognizerType::ENotRecognized)
    {
    	DoRecognizeMatroskaL(aName, aBuffer);
    }

}
// -----------------------------------------------------------------------------
// CPlaylistRecognizer::DoRecognizeMatroskaL
// -----------------------------------------------------------------------------
//
void CPlaylistRecognizer::DoRecognizeMatroskaL(
	const TDesC& aName,
	const TDesC8& aBuffer )
{
	  // assume match will fail
    iDataType = TDataType(KNullUid);
 	  iConfidence = CApaDataRecognizerType::ENotRecognized;

    if ( aName.Right( KMKVExt.iTypeLength ).CompareF(KMKVExt) == 0 )
        {
        iDataType = TDataType(KDataTypeMKVideo);
        }
    else if ( aName.Right( KMKAExt.iTypeLength ).CompareF(KMKAExt) == 0 )
        {
        iDataType = TDataType(KDataTypeMKAudio);
        }

    if ((aBuffer.FindF(KMKV_HEADER_GUID) != KErrNotFound))
        {
        if (iDataType != TDataType(KNullUid))
            {
            // Header and extension match
            iConfidence = ECertain;
            }
        else
            {
            // Header only match (assign mkv mime type)
            iConfidence = EPossible;
            iDataType = TDataType(KDataTypeMKVideo);
            }
        }
    else
        {
        if (iDataType != TDataType(KNullUid))
            {
            // Only extension match
            iConfidence = EProbable;
            }
        }
}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// CPlaylistRecognizer::CreateRecognizerL
// -----------------------------------------------------------------------------
//
CApaDataRecognizerType* CPlaylistRecognizer::CreateRecognizerL()
{
    return new (ELeave) CPlaylistRecognizer();
}

// -----------------------------------------------------------------------------
// ImplementationTable
// -----------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
{
    IMPLEMENTATION_PROXY_ENTRY( KPlaylistRecognizerImplementationUid,
                                CPlaylistRecognizer::CreateRecognizerL )
};

// -----------------------------------------------------------------------------
// ImplementationGroupProxy
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
{
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
}

//  End of File
