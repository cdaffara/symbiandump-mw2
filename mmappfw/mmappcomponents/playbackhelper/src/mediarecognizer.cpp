/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CMediaRecognizer
*
*/

// Version : %version: 7 %



// INCLUDE FILES
#include <utf.h>
#include <eikenv.h>
#include <bautils.h>
#include <mmf/server/mmfdatasourcesink.hrh>
#include <mediarecognizer.h>

#include "playbackhelper_log.h"

// CONSTANTS
_LIT8(KDataTypeRAM,"audio/x-pn-realaudio-plugin");
_LIT8(KDataTypeSDP,"application/sdp");
_LIT8(KDataTypePlaylist,"audio/mpegurl");
_LIT8(KDataTypeRNG,"application/vnd.nokia.ringing-tone");

_LIT8(KDataTypeMp3,"audio/mp3");
_LIT8(KDataTypeXmp3,"audio/x-mp3");
_LIT8(KDataTypeAudio3gp,"audio/3gpp");
_LIT8(KDataTypeFlashVideo,"video/x-flv");
_LIT8(KDataTypeText,"text");

_LIT8(KDataTypeAVI,"video/avi");
_LIT8(KDataTypeMsAVI,"video/msvideo"); 
_LIT8(KDataTypeMsAVIVideo,"video/x-msvideo");

_LIT8(KDataTypeMatroskaVideo ,"video/x-matroska");

#ifdef __WINDOWS_MEDIA
_LIT8(KDataTypeWMV_APP,"application/vnd.rn-wmv");
_LIT8(KDataTypeWMV,"video/x-ms-wmv");
_LIT8(KDataTypeASF,"application/vnd.ms-asf");
_LIT8(KDataTypeASX, "video/x-ms-asf"); 
_LIT(KRtspTcpPrefix,"rtspt://");
_LIT(KRtspUdpPrefix,"rtspu://");
_LIT(KHttpPrefix,"http://");
_LIT(KHttpTcpPrefix,"httpt://");
_LIT(KMmsPrefix,"mms://");
_LIT(KMmstPrefix,"mmst://");
#endif // __WINDOWS_MEDIA

_LIT8(KDmFileExtension,".dm");
_LIT8(KDcfFileExtension,".dcf");
_LIT8(KOdfFileExtension,".odf");
_LIT(KRtspPrefix,"rtsp://");
_LIT(KFilePrefix,"file://");

const TInt KDefaultBufferSize = 1000;
const TInt KMinPrefixLength = 6;


// ============================ MEMBER FUNCTIONS ===================================================

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::CMediaRecognizer
// C++ default constructor can NOT contain any code, that might leave.
// -------------------------------------------------------------------------------------------------
//
CMediaRecognizer::CMediaRecognizer()
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::CMediaRecognizer()"));
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::ConstructL
// Symbian 2nd phase constructor can leave.
// -------------------------------------------------------------------------------------------------
//
void CMediaRecognizer::ConstructL()
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::ConstructL()"));

    User::LeaveIfError( iFs.Connect() );
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::NewL
// Two-phased constructor.
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CMediaRecognizer* CMediaRecognizer::NewL()
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::NewL()"));

    CMediaRecognizer* self = new( ELeave ) CMediaRecognizer;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::~CMediaRecognizer
// Destructor
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CMediaRecognizer::~CMediaRecognizer()
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::~CMediaRecognizer()"));

    if ( iFileHandle.SubSessionHandle() )
    {
        iFileHandle.Close();
    }

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    if ( iFileHandle64.SubSessionHandle() )
    {
        iFileHandle64.Close();
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

    iFs.Close();    
}

// -------------------------------------------------------------------------------------------------
//  CMediaRecognizer::IdentifyMediaTypeL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CMediaRecognizer::TMediaType
CMediaRecognizer::IdentifyMediaTypeL( const TDesC& aMediaName, TBool aIncludeUrls )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IdentifyMediaTypeL( %S )"), &aMediaName);

    TMediaType ret = EUnidentified;

    if ( BaflUtils::FileExists( iFs, aMediaName ) )
    {
        ret = MediaTypeL( aMediaName );
    }
    else if ( aIncludeUrls && IsValidStreamingPrefix( aMediaName ) )
    {
        ret = EUrl;
    }

    return ret;
}

// -------------------------------------------------------------------------------------------------
//  CMediaRecognizer::IdentifyMediaTypeL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CMediaRecognizer::TMediaType
CMediaRecognizer::IdentifyMediaTypeL( const TDesC& aMediaName, RFile& aFile, TBool aIncludeUrls )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IdentifyMediaTypeL( %S, RFile )"), &aMediaName);

    if ( aFile.SubSessionHandle() )
    {
        iFileHandle.Close();
        User::LeaveIfError( iFileHandle.Duplicate( aFile ) );
    }

    TMediaType ret = EUnidentified;

    if ( FileHandleExists() )
    {
        ret = MediaTypeL( aMediaName );
    }
    else if ( aIncludeUrls && IsValidStreamingPrefix( aMediaName ) )
    {
        ret = EUrl;
    }

    return ret;
}

// -------------------------------------------------------------------------------------------------
//  CMediaRecognizer::MediaTypeL
// -------------------------------------------------------------------------------------------------
//
CMediaRecognizer::TMediaType CMediaRecognizer::MediaTypeL( const TDesC& aMediaName )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::MediaTypeL( %S )"), &aMediaName);

    TMediaType ret = EUnidentified;

    TDataRecognitionResult fileRecognitionResult;
    RecognizeFileL( aMediaName, fileRecognitionResult );

    if ( ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeSDP ) )
    {
        ret = ELocalSdpFile;
    }    
    else if ( ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeRAM ) )
    {
        ret = ELocalRamFile;
    }
    else
    {
        //
        //  Create a list of the controller plugins that support video
        //
        RMMFControllerImplInfoArray videoControllers;
        CleanupResetAndDestroyPushL( videoControllers );
        
        CreateVideoFormatsArrayL( videoControllers );

        if ( FileHasVideoSupportL( aMediaName, fileRecognitionResult, videoControllers ) )
        {
            ret = ELocalVideoFile;
        }
#ifdef __WINDOWS_MEDIA
        else if ( ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeWMV ) ||
                  ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeWMV_APP ) ||
                  ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeASF ) )
        {
            ret = ELocalVideoFile;
        }
        else if ( ! fileRecognitionResult.iDataType.Des8().CompareF(KDataTypeASX) )
        {
            ret = ELocalAsxFile;
        }
#endif // __WINDOWS_MEDIA
        else if ( ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeAVI ) ||
                  ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeMsAVI) ||
                  ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeMsAVIVideo) )
        {
            ret = ELocalVideoFile;
        }
        else if ( ! fileRecognitionResult.iDataType.Des8().CompareF(KDataTypeFlashVideo) )
        {
            ret = EFlashVideoFile;
        }
        else if ( ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypePlaylist ) )
        {
            ret = ELocalAudioPlaylist;
        }
        else if ( ! fileRecognitionResult.iDataType.Des8().CompareF( KDataTypeMatroskaVideo ) )
        {
            ret = ELocalVideoFile;
        }
        else
        {
            //
            //  Create a list of the controller plugins that support audio
            //
            RMMFControllerImplInfoArray audioControllers;
            CleanupResetAndDestroyPushL( audioControllers );
            
            CreateAudioFormatsArrayL( audioControllers );

            if ( FileHasAudioSupport( aMediaName,
                                      fileRecognitionResult,
                                      audioControllers,
                                      videoControllers ) )
            {
                ret = ELocalAudioFile;
            }

            CleanupStack::PopAndDestroy();   // audioControllers
        }

        CleanupStack::PopAndDestroy();   // videoControllers
    }

    return ret;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::MimeTypeL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TBuf<KMaxDataTypeLength> CMediaRecognizer::MimeTypeL( const TDesC& aLocalFile )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::MimeTypeL()"));

    TDataRecognitionResult fileRecognitionResult;
    RecognizeFileL(aLocalFile,fileRecognitionResult);
    TPtrC mimeType( KNullDesC );

    if (fileRecognitionResult.iConfidence >= CApaDataRecognizerType::EPossible)
    {
        mimeType.Set( fileRecognitionResult.iDataType.Des() );
    }

    return mimeType;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::FreeFilehandle
// -------------------------------------------------------------------------------------------------
//
EXPORT_C void CMediaRecognizer::FreeFilehandle()
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::FreeFilehandle()"));

    if ( iFileHandle.SubSessionHandle() )
    {
        iFileHandle.Close();
    }

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    if ( iFileHandle64.SubSessionHandle() )
    {
        iFileHandle64.Close();
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::FileHasVideoSupport
// -------------------------------------------------------------------------------------------------
//
TBool CMediaRecognizer::FileHasVideoSupportL( const TDesC& aFileName,
                                              TDataRecognitionResult& aFileRecognitionResult,
                                              RMMFControllerImplInfoArray& aVideoControllers )
{
    TBool videoSupport = EFalse;

    if ( aFileRecognitionResult.iConfidence >= CApaDataRecognizerType::EPossible )
    {
        for ( TInt i = 0 ; i < aVideoControllers.Count() ; i++ )
        {
            RMMFFormatImplInfoArray formats = aVideoControllers[i]->PlayFormats();

            for ( TInt j = 0; j < formats.Count() ; j++ )
            {
                if ( formats[j]->SupportsMimeType( aFileRecognitionResult.iDataType.Des8() ) )
                {
                    TPtrC8 iDataTypePtr = aFileRecognitionResult.iDataType.Des8();

                    if ( iDataTypePtr.Find( KDataTypeText ) == KErrNotFound )
                    {
                        videoSupport = ETrue;
                    }
                }
                else if ( formats[j]->SupportsHeaderDataL( aFileRecognitionResult.iDataType.Des8()))
                {
                    TParsePtrC parser( aFileName );
                    TBuf8<KMaxFileName> fileExtension;
                    fileExtension.Copy( parser.Ext() );

                    TBuf8<KMaxFileName> dmExtension( KDmFileExtension );
                    TBuf8<KMaxFileName> dcfExtension( KDcfFileExtension );
                    TBuf8<KMaxFileName> odfExtension( KOdfFileExtension );

                    if ( ( fileExtension.Compare( dmExtension ) == 0 ) ||
                         ( fileExtension.Compare( dcfExtension ) == 0 ) ||
                         ( fileExtension.Compare( odfExtension ) == 0 ) )
                    {
                        videoSupport = ETrue;
                    }
                }
            }
        }
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::FileHasVideoSupport() support video [%d]"), videoSupport);

    return videoSupport;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::FileHasAudioSupport
// -------------------------------------------------------------------------------------------------
//
TBool CMediaRecognizer::FileHasAudioSupport( const TDesC& aFileName,
                                             TDataRecognitionResult& aFileRecognitionResult,
                                             RMMFControllerImplInfoArray& aAudioControllers,
                                             RMMFControllerImplInfoArray& aVideoControllers )
{

    TBool audioSupport = EFalse;

    if ( aFileRecognitionResult.iConfidence >= CApaDataRecognizerType::EPossible )
    {
        for ( TInt i = 0 ; i < aAudioControllers.Count() ; i++ )
        {
            RMMFFormatImplInfoArray formats = aAudioControllers[i]->PlayFormats();

            for ( TInt j = 0; j < formats.Count() ; j++ )
            {
                if ( formats[j]->SupportsMimeType( aFileRecognitionResult.iDataType.Des8() ) )
                {
                    TPtrC8 iDataTypePtr = aFileRecognitionResult.iDataType.Des8();

                    if ( iDataTypePtr.Find( KDataTypeText ) == KErrNotFound )
                    {
                        audioSupport = ETrue;
                    }
                }
            }
        }
    }
    else
    {
        //
        // If we can't quess, try with file extension
        //
        TParsePtrC parser( aFileName );
        TBuf8<KMaxFileName> fileExtension;
        fileExtension.Copy( parser.Ext() );
        for ( TInt i = 0 ; i < aVideoControllers.Count() ; i++ )
        {
            RMMFFormatImplInfoArray formats = aAudioControllers[i]->PlayFormats();

            for ( TInt j = 0; j < formats.Count() ; j++)
            {
                if ( formats[j]->SupportsFileExtension( fileExtension ) )
                {
                    audioSupport = ETrue;
                }
            }
        }
    }

    if ( audioSupport &&
         aFileRecognitionResult.iDataType.Des8().CompareF( KDataTypeRNG ) &&
         aFileRecognitionResult.iDataType.Des8().CompareF( KDataTypeMp3 ) &&
         aFileRecognitionResult.iDataType.Des8().CompareF( KDataTypeXmp3 ) &&
         aFileRecognitionResult.iDataType.Des8().CompareF( KDataTypeAudio3gp ) )
    {
        audioSupport = EFalse;
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::FileHasAudioSupport() audio support [%d]"), audioSupport);

    return audioSupport;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::MimeTypeL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TBuf<KMaxDataTypeLength> CMediaRecognizer::MimeTypeL( RFile& aFile )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::MimeTypeL( RFile )"));

    if ( aFile.SubSessionHandle() )
    {
        iFileHandle.Close();
        // Preferred
        User::LeaveIfError( iFileHandle.Duplicate( aFile ) );
    }

    // If new handle is not valid, old might still be
    if ( !FileHandleExists() )
    {
        User::Leave( KErrBadHandle );
    }

    TDataRecognitionResult fileRecognitionResult;
    RecognizeFileL( KNullDesC(), fileRecognitionResult );
    TPtrC mimeType( KNullDesC );

    if (fileRecognitionResult.iConfidence >= CApaDataRecognizerType::EPossible)
    {
        mimeType.Set( fileRecognitionResult.iDataType.Des() );
    }
    
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::MimeTypeL( RFile ) ret [%S]"),&mimeType);
        
    return mimeType;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::CreateVideoFormatsArrayL
// -------------------------------------------------------------------------------------------------
//
void CMediaRecognizer::CreateVideoFormatsArrayL( RMMFControllerImplInfoArray& aVideoControllers )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::CreateVideoFormatsArrayL()"));

    CMMFControllerPluginSelectionParameters* cSelect =
            CMMFControllerPluginSelectionParameters::NewLC();

    CMMFFormatSelectionParameters* fSelect = CMMFFormatSelectionParameters::NewLC();

    // Set the play and record format selection parameters to be blank.
    // - format support is only retrieved if requested.
    cSelect->SetRequiredPlayFormatSupportL( *fSelect );

    // Set the media ids
    RArray<TUid> mediaIds;
    CleanupClosePushL( mediaIds );
    User::LeaveIfError( mediaIds.Append( KUidMediaTypeVideo ) );

    //get plugins that support at least video
    cSelect->SetMediaIdsL( mediaIds, CMMFPluginSelectionParameters::EAllowOtherMediaIds );

    // aVideoControllers contains now all plugins that support at least video
    cSelect->ListImplementationsL( aVideoControllers );

    // Clean up
    CleanupStack::PopAndDestroy( 3 ); //fSelect, cSelect, mediaIds
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::CreateAudioFormatsArrayL
// -------------------------------------------------------------------------------------------------
//
void CMediaRecognizer::CreateAudioFormatsArrayL( RMMFControllerImplInfoArray& aAudioControllers )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::CreateAudioFormatsArrayL()"));

    CMMFControllerPluginSelectionParameters* cSelect =
        CMMFControllerPluginSelectionParameters::NewLC();

    CMMFFormatSelectionParameters* fSelect = CMMFFormatSelectionParameters::NewLC();

    // Set the play and record format selection parameters to be blank.
    // - format support is only retrieved if requested.
    cSelect->SetRequiredPlayFormatSupportL( *fSelect );

    // Set the media ids
    RArray<TUid> mediaIds;
    CleanupClosePushL( mediaIds );
    User::LeaveIfError( mediaIds.Append( KUidMediaTypeAudio ) );

    // Get plugins that supports audio only
    cSelect->SetMediaIdsL( mediaIds, CMMFPluginSelectionParameters::EAllowOnlySuppliedMediaIds );

    // aAudioControllers contains now all audio plugins that support at least audio.
    cSelect->ListImplementationsL( aAudioControllers );

    // Clean up
    CleanupStack::PopAndDestroy( 3 ); //fSelect, cSelect, mediaIds
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::RecognizeFileL
// -------------------------------------------------------------------------------------------------
//
void CMediaRecognizer::RecognizeFileL( const TDesC& aFileName, TDataRecognitionResult& aResult )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::RecognizeFileL()"));

    aResult.Reset();

    TInt bufSize( 0 );
    RApaLsSession  recognizer;
    User::LeaveIfError( recognizer.Connect() );

    CleanupClosePushL( recognizer );

    if ( recognizer.GetMaxDataBufSize( bufSize ) || bufSize <= 0 )
    {
        bufSize = KDefaultBufferSize;
    }

    HBufC8* fBuf = HBufC8::NewLC( bufSize );
    TPtr8 fileBuf = fBuf->Des();

    // recognize file
    if ( FileHandleExists() )
    {
        User::LeaveIfError( recognizer.RecognizeData( iFileHandle, aResult ) );
    }
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    else if ( FileHandle64Exists() )
    {
        User::LeaveIfError( recognizer.RecognizeData( iFileHandle64, aResult ) );
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    else
    {
        // read file
        User::LeaveIfError( ReadFile( aFileName, fileBuf, bufSize ) );
        User::LeaveIfError( recognizer.RecognizeData( aFileName, fileBuf, aResult ) );
    }

    CleanupStack::PopAndDestroy( 2 );  // fBuf & recognizer
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::ReadFile
// -------------------------------------------------------------------------------------------------
//
TInt CMediaRecognizer::ReadFile( const TDesC& aFileName, TDes8& aBuf, TInt aBufSize )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::ReadFile( %S )"), &aFileName);

    TInt err = KErrNone;     
    
    //
    // open using 32-bit file handle
    //
    err = ReadFileViaNewFileHandle( aFileName, aBuf, aBufSize );
        
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    if ( err == KErrTooBig )
    {
        //
        // if 32-bit file handle fails, open using 64-bit file handle
        //
        err = ReadFileViaNewFileHandle64( aFileName, aBuf, aBufSize );
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        
    return err;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::FileHandleExists
// -------------------------------------------------------------------------------------------------
//
TBool CMediaRecognizer::FileHandleExists()
{
    TInt size = 0;
    TInt err = KErrNone;
    TBool exist = EFalse;

    if ( iFileHandle.SubSessionHandle() )
    {
        err = iFileHandle.Size( size );
    }

    if ( err == KErrNone && size > 0 )
    {
        exist = ETrue;
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::FileHandleExists() exist [%d]"), exist);

    return exist;
}

//--------------------------------------------------------------------------------------------------
//  CMediaRecognizer::IsValidStreamingPrefix
//  Check for a valid streaming prefix given a URL
//--------------------------------------------------------------------------------------------------
//
EXPORT_C TBool CMediaRecognizer::IsValidStreamingPrefix( const TDesC& aUrl )
{
    TBool retVal = EFalse;

    if ( aUrl.Length() >= KMinPrefixLength )
    {
        PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IsValidStreamingPrefix(%S)"), &aUrl);

#ifdef __WINDOWS_MEDIA
        if ( ( ! aUrl.Left(KRtspPrefix().Length()).CompareF(KRtspPrefix) ) ||
             ( ! aUrl.Left(KHttpPrefix().Length()).CompareF(KHttpPrefix) ) ||
             ( ! aUrl.Left(KMmsPrefix().Length()).CompareF(KMmsPrefix) )   ||
             ( ! aUrl.Left(KMmstPrefix().Length()).CompareF(KMmstPrefix) ) ||
             ( ! aUrl.Left(KHttpTcpPrefix().Length()).CompareF(KHttpTcpPrefix) ) ||
             ( ! aUrl.Left(KRtspTcpPrefix().Length()).CompareF(KRtspTcpPrefix) ) ||
             ( ! aUrl.Left(KRtspUdpPrefix().Length()).CompareF(KRtspUdpPrefix) ) )
#else
        if ( ! aUrl.Left( KRtspPrefix().Length() ).CompareF( KRtspPrefix ) )
#endif
        {
            retVal = ETrue;
        }
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IsValidStreamingPrefix() ret %d"), retVal);

    return retVal;
}

//--------------------------------------------------------------------------------------------------
//  CMediaRecognizer::IsValidUrlPrefix
//  Check for a valid prefix given a URL
//--------------------------------------------------------------------------------------------------
//
EXPORT_C TBool CMediaRecognizer::IsValidUrlPrefix( const TDesC& aUrl )
{
    TBool retVal = EFalse;

    if ( aUrl.Length() >= KMinPrefixLength )
    {
        PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IsValidUrlPrefix(%S)"), &aUrl);


        if ( ( ! aUrl.Left(KFilePrefix().Length()).CompareF(KFilePrefix) ) ||
#ifdef __WINDOWS_MEDIA
             ( ! aUrl.Left(KHttpPrefix().Length()).CompareF(KHttpPrefix) ) ||
             ( ! aUrl.Left(KMmsPrefix().Length()).CompareF(KMmsPrefix) )   ||
             ( ! aUrl.Left(KMmstPrefix().Length()).CompareF(KMmstPrefix) ) ||
             ( ! aUrl.Left(KHttpTcpPrefix().Length()).CompareF(KHttpTcpPrefix) ) ||
             ( ! aUrl.Left(KRtspTcpPrefix().Length()).CompareF(KRtspTcpPrefix) ) ||
             ( ! aUrl.Left(KRtspUdpPrefix().Length()).CompareF(KRtspUdpPrefix) ) ||
#endif
             ( ! aUrl.Left(KRtspPrefix().Length()).CompareF(KRtspPrefix) ) )
        {
            retVal = ETrue;
        }
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IsValidUrlPrefix() ret %d"), retVal);

    return retVal;
}

//--------------------------------------------------------------------------------------------------
//  CMediaRecognizer::IsValidUrlPrefix
//  Check for a valid prefix given a URL
//--------------------------------------------------------------------------------------------------
//
EXPORT_C TBool CMediaRecognizer::IsValidUrlPrefix( const TDesC8& aUrl )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IsValidUrlPrefix(TDesC8)"));

    TBool retVal = EFalse;

    if ( aUrl.Length() >= KMinPrefixLength )
    {
        HBufC16* buf16 = NULL;

        TRAPD( err, buf16 = CnvUtfConverter::ConvertToUnicodeFromUtf8L(aUrl); );

        if ( ! err )
        {
            retVal = IsValidUrlPrefix( buf16->Des() );
            delete buf16;
        }
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IsValidUrlPrefix(TDesC8) ret %d"), retVal);

    return retVal;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::ReadFileViaNewFileHandle
// -------------------------------------------------------------------------------------------------
//
TInt CMediaRecognizer::ReadFileViaNewFileHandle( const TDesC& aFileName, TDes8& aBuf, TInt aBufSize  )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::ReadFileViaNewFileHandle()"));
    
    TInt err = KErrNone;
    
    RFile file;
    err = file.Open( iFs, aFileName, EFileRead | EFileShareReadersOnly );

    if ( err )
    {
        err = file.Open( iFs, aFileName, EFileRead | EFileShareAny );
    }

    if ( err == KErrNone )
    {
        // read the beginning of the file
        err = file.Read( 0, aBuf, aBufSize );
        file.Close();
    }
    
    return err;
}

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

// -------------------------------------------------------------------------------------------------
//  CMediaRecognizer::IdentifyMediaTypeL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C CMediaRecognizer::TMediaType
CMediaRecognizer::IdentifyMediaTypeL( const TDesC& aMediaName, RFile64& aFile, TBool aIncludeUrls )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::IdentifyMediaTypeL( %S, RFile64 )"), &aMediaName);

    if ( aFile.SubSessionHandle() )
    {
        iFileHandle64.Close();
        User::LeaveIfError( iFileHandle64.Duplicate( aFile ) );
    }

    TMediaType ret = EUnidentified;

    if ( FileHandle64Exists() )
    {
        ret = MediaTypeL( aMediaName );
    }
    else if ( aIncludeUrls && IsValidStreamingPrefix( aMediaName ) )
    {
        ret = EUrl;
    }

    return ret;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::MimeTypeL
// -------------------------------------------------------------------------------------------------
//
EXPORT_C TBuf<KMaxDataTypeLength> CMediaRecognizer::MimeTypeL( RFile64& aFile )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::MimeTypeL( RFile64 )"));
    
    if ( aFile.SubSessionHandle() )
    {
        iFileHandle64.Close();
        // Preferred
        User::LeaveIfError( iFileHandle64.Duplicate( aFile ) );
    }

    // If new handle is not valid, old might still be
    if ( !FileHandle64Exists() )
    {
        User::Leave( KErrBadHandle );
    }    
    
    TDataRecognitionResult fileRecognitionResult;
    RecognizeFileL( KNullDesC(), fileRecognitionResult );
    TPtrC mimeType( KNullDesC );

    if (fileRecognitionResult.iConfidence >= CApaDataRecognizerType::EPossible)
    {
        mimeType.Set( fileRecognitionResult.iDataType.Des() );
    }
     
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::DetermineMimeTypeL( RFile64 ) ret [%S]"), &mimeType);
    
    return mimeType;
}


// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::FileHandle64Exists
// -------------------------------------------------------------------------------------------------
//
TBool CMediaRecognizer::FileHandle64Exists()
{
    TInt64 size = 0;
    TInt err = KErrNone;
    TBool exist = EFalse;

    if ( iFileHandle64.SubSessionHandle() )
    {
        err = iFileHandle64.Size( size );
    }

    if ( err == KErrNone && size > 0 )
    {
        exist = ETrue;
    }

    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::FileHandle64Exists() exist [%d]"), exist);

    return exist;
}

// -------------------------------------------------------------------------------------------------
// CMediaRecognizer::ReadFileViaCurrentFileHandle64
// -------------------------------------------------------------------------------------------------
//
TInt CMediaRecognizer::ReadFileViaNewFileHandle64( const TDesC& aFileName, TDes8& aBuf, TInt aBufSize  )
{
    PLAYBACKHELPER_DEBUG(_L("CMediaRecognizer::ReadFileViaNewFileHandle64()"));
    
    TInt err = KErrNone;
    
    RFile64 file;
    err = file.Open( iFs, aFileName, EFileRead | EFileShareReadersOnly );

    if ( err )
    {
        err = file.Open( iFs, aFileName, EFileRead | EFileShareAny );
    }

    if ( err == KErrNone )
    {
        // read the beginning of the file
        err = file.Read( 0, aBuf, aBufSize );
        file.Close();
    }
    
    return err;
}

#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API



//  End of File
