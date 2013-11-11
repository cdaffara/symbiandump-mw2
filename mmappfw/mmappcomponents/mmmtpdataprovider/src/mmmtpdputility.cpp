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
* Description:  Implementation for MTP Common Function
*
*/


#include <bautils.h>
#include <e32math.h>
#include <f32file.h>
#include <mtp/mtpprotocolconstants.h>
#include <mtp/mtpdatatypeconstants.h>
#include <3gplibrary/mp4lib.h>
#include <caf/content.h>
#include <mtp/cmtpobjectmetadata.h>
#include <mtp/mmtpdataproviderframework.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/tmtptypeuint32.h>
#include <e32property.h>    // for RProperty

// for asf mimetype parsing
#ifdef __WINDOWS_MEDIA
#include <hxmetadatakeys.h>
#include <hxmetadatautil.h>
#endif

#include "mmmtpdputility.h"
#include "mmmtpdpfiledefs.h"
#include "tobjectdescription.h"
#include "mmmtpdplogger.h"
#include "mmmtpdp_variant.hrh"
#include "cmmmtpdpaccesssingleton.h"

using namespace ContentAccess;

// local to this file, non standard mimetype used for ASF parsing with helix
_LIT( KHxMimeTypeWma, "audio/x-hx-wma" );
_LIT( KHxMimeTypeWmv, "video/x-hx-wmv" );

const TInt KMaxRenameTimes = 10;
const TInt KRenameThreshold = 100 * 1000; // (100 millisec)

// -----------------------------------------------------------------------------
// MmMtpDpUtility::FormatFromFilename
// Utility function to get the type of an object from the filename
// The recommended way from the MS implementation
// These should be kept in sync with the object formats supported
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPFormatCode MmMtpDpUtility::FormatFromFilename( const TDesC& aFullFileName )
    {
    TMTPFormatCode format = EMTPFormatCodeUndefined;

    if ( aFullFileName.Right( 1 ).CompareF( KTxtBackSlash ) == 0 ) // We have a directory name
        {
        format = EMTPFormatCodeAssociation;
        }
    else 
        {
        TParsePtrC file( aFullFileName );

        if ( file.ExtPresent() )
            {
            // need to do it in popularity of format, to optmize performance
            if ( file.Ext().CompareF( KTxtExtensionMP3 ) == 0 )
                format = EMTPFormatCodeMP3;
#ifdef __WINDOWS_MEDIA
            else if ( file.Ext().CompareF( KTxtExtensionWMA ) == 0 )
                format = EMTPFormatCodeWMA;
#endif // __WINDOWS_MEDIA
            else if (( file.Ext().CompareF( KTxtExtensionPLA ) == 0 ) 
                || ( file.Ext().CompareF( KTxtExtensionVIR ) == 0 ) )
                format = EMTPFormatCodeAbstractAudioVideoPlaylist;
            else if ( ( file.Ext().CompareF( KTxtExtensionMP4 ) == 0 )
                || ( file.Ext().CompareF( KTxtExtensionM4A ) == 0 ) )
                format = EMTPFormatCodeMP4Container;
            else if ( ( file.Ext().CompareF( KTxtExtension3GP ) == 0 )
                || ( file.Ext().CompareF( KTxtExtension3G2 ) == 0 )
                || ( file.Ext().CompareF( KTxtExtensionO4A ) == 0 )
                || ( file.Ext().CompareF( KTxtExtensionO4V ) == 0 ) )
                format = EMTPFormatCode3GPContainer;
            else if ( file.Ext().CompareF( KTxtExtensionAAC ) == 0 )
                format = EMTPFormatCodeAAC;
            else if ( file.Ext().CompareF( KTxtExtensionWAV ) == 0 )
                format = EMTPFormatCodeWAV;
#ifdef __WINDOWS_MEDIA
            else if ( file.Ext().CompareF( KTxtExtensionWMV ) == 0 )
                format = EMTPFormatCodeWMV;
            else if ( file.Ext().CompareF( KTxtExtensionASF ) == 0 )
                format = EMTPFormatCodeASF;
#endif // __WINDOWS_MEDIA
            else if ( file.Ext().CompareF( KTxtExtensionODF ) == 0 )
                {
                HBufC8* mime(NULL);
                TRAP_IGNORE( mime = MmMtpDpUtility::OdfMimeTypeL( file.FullName() ) );
                if ( mime != NULL )
                    {
                    // 3GP
                    if ( mime->CompareF( KMimeTypeAudio3gpp ) == 0
                        || mime->CompareF( KMimeTypeVideo3gpp ) == 0 )
                        {
                        format = EMTPFormatCode3GPContainer;
                        }
                    delete mime;
                    mime = NULL;
                    }
                }
#ifdef MMMTPDP_ABSTRACTAUDIOALBUM_SUPPORT
            else if ( file.Ext().CompareF( KTxtExtensionALB ) == 0 )
                format = EMTPFormatCodeAbstractAudioAlbum;
#endif // MMMTPDP_ABSTRACTAUDIOALBUM_SUPPORT
            else if ( file.Ext().CompareF( KTxtExtensionM3U ) == 0 )
                format = EMTPFormatCodeM3UPlaylist;
            }
        }

    return format;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::HasMetadata
// Utility function to determine whether a format has metadata support
// -----------------------------------------------------------------------------
//
TBool MmMtpDpUtility::HasMetadata( TUint16 aObjFormatCode )
    {
    if ( aObjFormatCode == EMTPFormatCodeMP3
        || aObjFormatCode == EMTPFormatCodeWMA
        || aObjFormatCode == EMTPFormatCodeAAC
        || aObjFormatCode == EMTPFormatCodeWAV
        || aObjFormatCode == EMTPFormatCodeMP4Container
        || aObjFormatCode == EMTPFormatCode3GPContainer
        || aObjFormatCode == EMTPFormatCodeWMV
        || aObjFormatCode == EMTPFormatCodeASF )
        {
        return ETrue;
        }

    return EFalse;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::HasReference
// Utility function to decide if there is any reference
// -----------------------------------------------------------------------------
//
TBool MmMtpDpUtility::HasReference( TUint16 aObjFormatCode )
    {
    if ( aObjFormatCode == EMTPFormatCodeAbstractAudioAlbum
        || aObjFormatCode == EMTPFormatCodeAbstractAudioVideoPlaylist
        || aObjFormatCode == EMTPFormatCodeM3UPlaylist )
        return ETrue;
    else
        return EFalse;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::ValidateFilename
// Check the filename to see if it exceeds Symbian 256 limit.
// -----------------------------------------------------------------------------
//
TBool MmMtpDpUtility::ValidateFilename( const TDesC& aPathName,
    const TDesC& aFileName )
    {
    TBool result = ETrue;

    if ( ( aPathName.Length() + aFileName.Length() ) > KMaxFileName )
        {
        result = EFalse;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::GetObjectSizeL
// Get the filesize.
// -----------------------------------------------------------------------------
//
TUint64 MmMtpDpUtility::GetObjectSizeL( RFs& aFs, const TDesC& aFileName )
    {
    TEntry fileInfo;
    // Shouldn't leave
    User::LeaveIfError( aFs.Entry( aFileName, fileInfo ) );
    return fileInfo.FileSize();
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::GetDateModifiedL
// Get the file date modified.
// -----------------------------------------------------------------------------
//
void MmMtpDpUtility::GetObjectDateModifiedL( RFs& aFs, const TDesC& aFullFileName, TDes& aDateModified )
    {
    TTime date;
    TEntry fileInfo;
    // Shouldn't leave
    User::LeaveIfError( aFs.Entry( aFullFileName, fileInfo ) );
    date = fileInfo.iModified;
    date.FormatL( aDateModified, KMtpDateTimeFormat );
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::DesToTTimeL
// Converts a MTP format (YYYYMMDDTHHMMSS) date time string to a TTime.
// -----------------------------------------------------------------------------
//
TInt MmMtpDpUtility::DesToTTime( const TDesC& aDateTime, TTime& aTime )
    {
    PRINT1( _L ( "MM MTP => MmMtpDpUtility::DesToTTime aDateTime=%S" ), &aDateTime );

    TInt err = KErrNone;
    if ( aDateTime.Length() < KMtpMaxDateTimeStringLength )
        {
        err =  KErrGeneral;
        }
    else
        {
        TLex dateBuf( aDateTime.Left( 4 ) );
        TInt year;
        dateBuf.Val( year );

        dateBuf = aDateTime.Mid( 4, 2 );
        TInt month;
        dateBuf.Val( month );

        dateBuf = aDateTime.Mid( 6, 2 );
        TInt day;
        dateBuf.Val( day );

        dateBuf = aDateTime.Mid( 9, 2 );
        TInt hour;
        dateBuf.Val( hour );

        dateBuf = aDateTime.Mid( 11, 2 );
        TInt minute;
        dateBuf.Val( minute );

        dateBuf = aDateTime.Mid( 13, 2 );
        TInt second;
        dateBuf.Val( second );

        PRINT3( _L ( "MM MTP <> MmMtpDpUtility::DesToTTime Year = %d, Month = %d, Day = %d" ), year, month, day );
        PRINT3( _L ( "MM MTP <> MmMtpDpUtility::DesToTTime Hour = %d, Minute = %d, Second = %d" ), hour, minute, second );

        if ( ( month > 0 && month < 13 )
            && ( day > 0 && day < 32 )
            && ( hour >= 0 && hour < 60 )
            && ( minute >= 0 && minute < 60 )
            && ( second >= 0 && second < 60 ) )
            {
            // microsecond is ignored because MPX doesn't support it, following s60
            TDateTime dateTime( year, TMonth( --month ), --day, hour, minute, second, 0 );
            aTime = dateTime;
            }
        else
            {
            // date string syntax is wrong
            err =  KErrGeneral;
            }
        }
    PRINT1( _L ( "MM MTP <= MmMtpDpUtility::DesToTTime err = %d" ), err );
    return err;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::GetProtectionStatusL
// Get the file protection status.
// -----------------------------------------------------------------------------
//
TUint16 MmMtpDpUtility::GetProtectionStatusL( RFs& aFs, const TDesC& aFullFileName )
    {
    TUint16 protectionStatus = EMTPProtectionNoProtection;

    TEntry fileInfo;
    // Shouldn't leave
    User::LeaveIfError( aFs.Entry( aFullFileName, fileInfo ) );
    if ( fileInfo.IsReadOnly() )
        {
        protectionStatus = EMTPProtectionReadOnly;
        }

    PRINT2( _L( "MM MTP <> MmMtpDpUtility::GetProtectionStatusL aFullFileName = %S, protectionStatus = 0x%x" ),
        &aFullFileName,
        protectionStatus );

    return protectionStatus;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::CheckPropType
// Check if property type match datatype.
// -----------------------------------------------------------------------------
//
TMTPResponseCode MmMtpDpUtility::CheckPropType( TUint16 aPropertyCode, TUint16 aDataType )
    {
    PRINT2( _L( "MM MTP => MmMtpDpUtility::CheckPropCode aPropertyCode = 0x%x, aDataType = 0x%x" ),
        aPropertyCode,
        aDataType );

    TMTPResponseCode responseCode = EMTPRespCodeOK;
    switch ( aPropertyCode )
        {
        //Access denied Properties
        case EMTPObjectPropCodeStorageID:
        case EMTPObjectPropCodeObjectFormat:
        case EMTPObjectPropCodeProtectionStatus:
        case EMTPObjectPropCodeObjectSize:
        case EMTPObjectPropCodeParentObject:
        case EMTPObjectPropCodePersistentUniqueObjectIdentifier:
        case EMTPObjectPropCodeDateCreated:
        case EMTPObjectPropCodeDateModified:
        case EMTPObjectPropCodeDateAdded:
        case EMTPObjectPropCodeVideoBitRate:
        case EMTPObjectPropCodeRepresentativeSampleFormat:
        case EMTPObjectPropCodeRepresentativeSampleSize:
        case EMTPObjectPropCodeRepresentativeSampleHeight:
        case EMTPObjectPropCodeRepresentativeSampleWidth:
            {
            responseCode = EMTPRespCodeAccessDenied;
            }
            break;

        //String properties
        case EMTPObjectPropCodeObjectFileName: // 0xDC07
        case EMTPObjectPropCodeName: // 0xDC44
        case EMTPObjectPropCodeArtist: // 0xDC46
        case EMTPObjectPropCodeGenre: // 0xDC8C
        case EMTPObjectPropCodeParentalRating: // 0xDC94
        case EMTPObjectPropCodeComposer: // 0xDC96
        case EMTPObjectPropCodeOriginalReleaseDate: // 0xDC99
        case EMTPObjectPropCodeAlbumName: // 0xDC9A
        case EMTPObjectPropCodeEncodingProfile: // 0xDEA1
        case EMTPObjectPropCodeAlbumArtist: // 0xDC9B
            {
            if ( aDataType != EMTPTypeString )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            }
            break;

        case EMTPObjectPropCodeDescription:
            {
            if ( aDataType != EMTPTypeAUINT16 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            }
            break;

        //Uint32 properties
        case EMTPObjectPropCodeWidth: // 0xDC87
        case EMTPObjectPropCodeHeight: // 0xDC88
        case EMTPObjectPropCodeDuration: // 0xDC89
        case EMTPObjectPropCodeSampleRate: // 0xDE93
        case EMTPObjectPropCodeAudioWAVECodec: // 0xDE99
        case EMTPObjectPropCodeAudioBitRate: // 0xDE9A
        case EMTPObjectPropCodeVideoFourCCCodec: // 0xDE9B
        case EMTPObjectPropCodeFramesPerThousandSeconds: // 0xDE9D
        case EMTPObjectPropCodeKeyFrameDistance: // 0xDE9E
            {
            if ( aDataType != EMTPTypeUINT32 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            }
            break;

        //Uint16 properties
        case EMTPObjectPropCodeTrack: // 0xDC8B
        case EMTPObjectPropCodeDRMStatus: // 0xDC9D
        case EMTPObjectPropCodeNumberOfChannels: // 0xDE94
        case EMTPObjectPropCodeScanType: // 0xDE97
            {
            if ( aDataType != EMTPTypeUINT16 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            }
            break;

        // Uint8 properties
        case EMTPObjectPropCodeNonConsumable:
        case EMTPExtObjectPropCodeOmaDrmStatus: // 0xDB01
            {
            if ( aDataType != EMTPTypeUINT8 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            }
            break;

        case EMTPObjectPropCodeRepresentativeSampleData:
            {
            if ( aDataType != EMTPTypeAUINT8 )
                {
                responseCode = EMTPRespCodeInvalidObjectPropFormat;
                }
            }
            break;

        default:
            {
            responseCode = EMTPRespCodeInvalidObjectPropCode;
            }
            break;
        }
    PRINT1( _L( "MM MTP <= MmMtpDpUtility::CheckPropCode responseCode = 0x%x" ),
        responseCode );
    return responseCode;
    }

// -----------------------------------------------------------------------------
// MmMtpDpUtility::UpdateObjectFileName
// Update object file name.
// -----------------------------------------------------------------------------
//
TInt MmMtpDpUtility::UpdateObjectFileName( RFs& aFs,
    const TDesC& aFullFileName,
    TDes& aNewName )
    {
    TInt bufSize = aNewName.MaxLength() < KMaxFileName ? aNewName.MaxLength() : KMaxFileName;

    TParsePtrC parser( aFullFileName );
    TPtrC ptr( parser.DriveAndPath() );

    if ( aNewName.Length() + ptr.Length() > bufSize )
        {
        return KErrOverflow;
        }
    else
        {
        aNewName.Insert( 0, ptr );
        }

    TInt ret = KErrNone;
    // Some other component might be holding on to the file (MDS background harvesting),
    // try again after 100 millisec, up to 10 times, before give up
    TInt count = KMaxRenameTimes;
    while ( count > 0 )
        {
        ret = aFs.Rename( aFullFileName, aNewName );
        if ( ( ret == KErrInUse ) && ( count > 1 ) )
            {
            User::After( KRenameThreshold );
            count--;
            }
        else
            {
            break;
            }
        }

    PRINT1( _L( "MM MTP <> MmMtpDpUtility::UpdateObjectFileName, ret = %d" ), ret );

    return ret;
    }

// -----------------------------------------------------------------------------
// MetadataAccessWrapper::Mp4MimeTypeL
// Get mime type from mp4 file
// -----------------------------------------------------------------------------
//
HBufC8* MmMtpDpUtility::Mp4MimeTypeL( const TDesC& aFullPath )
    {
    PRINT( _L( "MM MTP => MmMtpDpUtility::Mp4MimeTypeL" ) );
    HBufC8* mimebuf = NULL;
    TParsePtrC file( aFullPath );

    if ( file.Ext().CompareF( KTxtExtensionMP4 ) == 0
        || file.Ext().CompareF( KTxtExtension3GP ) == 0
        || file.Ext().CompareF( KTxtExtension3G2 ) == 0 )
        {
        // get mime from file
        MP4Handle mp4Handle = NULL;

        RBuf buf;
        TInt err = buf.Create( aFullPath.Length() + 1 );
        User::LeaveIfError( err );
        buf = aFullPath;

        // open mp4 file
        MP4Err mp4err = MP4ParseOpen( &mp4Handle, (MP4FileName) buf.PtrZ() );

        if ( mp4err == MP4_OK )
            {

            mp4_u32 videoLength, videoType, videoWidth, videoHeight, timeScale;
            mp4_double frameRate;

            // get video description
            mp4err = MP4ParseRequestVideoDescription( mp4Handle, &videoLength,
                &frameRate, &videoType, &videoWidth, &videoHeight, &timeScale );

            // not the video file
            if ( mp4err == MP4_NO_VIDEO )
                {
                mp4_u32 audioLength, audioType, timeScale, averateBitRate;
                mp4_u8 framesPerSample;

                // get audio description
                mp4err = MP4ParseRequestAudioDescription( mp4Handle,
                    &audioLength, &audioType, &framesPerSample, &timeScale,
                    &averateBitRate );

                if ( mp4err == MP4_OK )
                    {
                    if ( file.Ext().CompareF( KTxtExtension3GP ) == 0
                        || file.Ext().CompareF( KTxtExtension3G2 ) == 0 )
                        {
                        mimebuf = KMimeTypeAudio3gpp().Alloc();
                        }
                    else
                        {
                        mimebuf = KMimeTypeAudioMp4().Alloc();
                        }
                    }
                }
            // is video file
            else if ( mp4err == MP4_OK )
                {
                if ( file.Ext().CompareF( KTxtExtension3GP ) == 0
                    || file.Ext().CompareF( KTxtExtension3G2 ) == 0 )
                    {
                    mimebuf = KMimeTypeVideo3gpp().Alloc();
                    }
                else
                    {
                    mimebuf = KMimeTypeVideoMp4().Alloc();
                    }
                }

            // Close mp4 parser
            if ( mp4Handle != NULL )
                {
                MP4ParseClose( mp4Handle );
                }
            }

        buf.Close();
        }
    else
        {
        User::Leave( KErrNotSupported );
        }

    if ( mimebuf == NULL )
        {
        PRINT( _L( "MM MTP <> MmMtpDpUtility::Mp4MimeTypeL, mimebuf == NULL, default as video" ) );

        if ( file.Ext().CompareF( KTxtExtension3GP ) == 0
            || file.Ext().CompareF( KTxtExtension3G2 ) == 0 )
            {
            mimebuf = KMimeTypeVideo3gpp().Alloc();
            }
        else
            {
            mimebuf = KMimeTypeVideoMp4().Alloc();
            }
        }
    PRINT( _L( "MM MTP <= MmMtpDpUtility::Mp4MimeTypeL" ) );
    return mimebuf;
    }

// -----------------------------------------------------------------------------
// MetadataAccessWrapper::OdfMimeTypeL
// Get mime type from odf file
// -----------------------------------------------------------------------------
//
HBufC8* MmMtpDpUtility::OdfMimeTypeL( const TDesC& aFullPath )
    {
    PRINT( _L( "MM MTP => MmMtpDpUtility::OdfMimeTypeL" ) );

    CContent* content = CContent::NewLC( aFullPath ); // + content
    HBufC* buffer = HBufC::NewLC( KMimeTypeMaxLength ); // + buffer

    TPtr data = buffer->Des();
    User::LeaveIfError( content->GetStringAttribute( EMimeType, data ) );

    HBufC8* mimebuf = HBufC8::NewL( buffer->Length() );
    mimebuf->Des().Copy( *buffer );

    CleanupStack::PopAndDestroy( buffer ); // - buffer
    CleanupStack::PopAndDestroy( content ); // - content

    PRINT( _L( "MM MTP <= MmMtpDpUtility::OdfMimeTypeL" ) );
    return mimebuf;
    }

// -----------------------------------------------------------------------------
// MetadataAccessWrapper::AsfMimeTypeL
// Get mime type from asf file
// -----------------------------------------------------------------------------
//
HBufC8* MmMtpDpUtility::AsfMimeTypeL( const TDesC& aFullPath )
    {
    PRINT1( _L( "MM MTP => MmMtpDpUtility::AsfMimeTypeL, aFullPath = %S" ), &aFullPath );

    HBufC8* mimebuf = NULL;

#ifdef __WINDOWS_MEDIA
    CHXMetaDataUtility *hxUtility = CHXMetaDataUtility::NewL();
    CleanupStack::PushL( hxUtility );

    hxUtility->OpenFileL( aFullPath );
    PRINT( _L( "MM MTP <> MmMtpDpUtility::AsfMimeTypeL OpenFileL" ) );

    HXMetaDataKeys::EHXMetaDataId id;
    TUint count = 0;
    TBool isAudio = EFalse;
    hxUtility->GetMetaDataCount( count );
    for ( TUint i = 0; i < count; i++ )
        {
        HBufC* buf = NULL;
        hxUtility->GetMetaDataAt( i, id, buf );

        if ( id == HXMetaDataKeys::EHXMimeType )
            {
            TPtr des = buf->Des();

            if ( des.Find( KHxMimeTypeWma() ) != KErrNotFound )
                {
                isAudio = ETrue;
                }
            else if ( des.Find( KHxMimeTypeWmv() ) != KErrNotFound )
                {
                PRINT( _L( "MM MTP <> MmMtpDpUtility::AsfMimeTypeL, video" ) );
                mimebuf = KMimeTypeVideoWm().Alloc();
                break;
                }
            }
        else if ( i == count - 1 )
            {
            if ( isAudio )
                {
                PRINT( _L( "MM MTP <> MmMtpDpUtility::AsfMimeTypeL, audio" ) );
                mimebuf = KMimeTypeAudioWm().Alloc();
                }
            else
                {
                User::Leave( KErrNotFound );
                }
            }
        }

    hxUtility->ResetL();
    CleanupStack::PopAndDestroy( hxUtility );
#else
    User::Leave( KErrNotSupported );
#endif

    PRINT( _L( "MM MTP <= MmMtpDpUtility::AsfMimeTypeL" ) );
    return mimebuf;
    }

// -----------------------------------------------------------------------------
// MetadataAccessWrapper::SubFormatCodeFromMime
// Get subformat code from mime string
// -----------------------------------------------------------------------------
//
TMmMtpSubFormatCode MmMtpDpUtility::SubFormatCodeFromMime( const TDesC8& aMimeType )
    {
    PRINT( _L( "MM MTP => MmMtpDpUtility::SubFormatCodeFromMime" ) );

    TMmMtpSubFormatCode subFormatCode = EMTPSubFormatCodeUnknown;

    if ( aMimeType.CompareF( KMimeTypeVideoMp4 ) == 0
        || aMimeType.CompareF( KMimeTypeVideo3gpp ) == 0
#ifdef __WINDOWS_MEDIA
        || aMimeType.CompareF( KMimeTypeVideoWm ) == 0
#endif
        )
        {
        subFormatCode = EMTPSubFormatCodeVideo;
        }
    else if ( aMimeType.CompareF( KMimeTypeAudioMp4 ) == 0
        || aMimeType.CompareF( KMimeTypeAudio3gpp ) == 0
#ifdef __WINDOWS_MEDIA
        || aMimeType.CompareF( KMimeTypeAudioWm ) == 0
#endif
        )
        {
        subFormatCode = EMTPSubFormatCodeAudio;
        }
    else
        {
        PRINT( _L( "MM MTP <= MmMtpDpUtility::SubFormatCodeFromMime format not supported" ) );
        subFormatCode = EMTPSubFormatCodeUndefined;
        }

    PRINT1( _L( "MM MTP <= MmMtpDpUtility::SubFormatCodeFromMime SubFormatCode = %d" ), subFormatCode );

    return subFormatCode;
    }

// -----------------------------------------------------------------------------
// MetadataAccessWrapper::GetDrmStatus
// Get drm status code from file
// -----------------------------------------------------------------------------
//
EXPORT_C TInt MmMtpDpUtility::GetDrmStatus( const TDesC& aFullFileName )
    {
    PRINT1( _L( "MM MTP => MmMtpDpUtility::GetDrmStatus, full file name: %S" ), &aFullFileName );
    TParsePtrC file( aFullFileName );
    TInt drmStatus = EMTPDrmStatusUnknown;

    if ( ( file.Ext().CompareF( KTxtExtensionODF ) == 0 ) ||
         ( file.Ext().CompareF( KTxtExtensionO4A ) == 0 ) ||
         ( file.Ext().CompareF( KTxtExtensionO4V ) == 0 ) )
        {
        CContent* content = NULL;

        TRAPD( err, content = CContent::NewL( aFullFileName ) );

        if ( err == KErrNone )
            {
            TInt value = 0;

            content->GetAttribute( EIsProtected, value );

            if ( value != 0 )
                {
                drmStatus = EMTPDrmStatusProtected;
                }
            else
                {
                drmStatus = EMTPDrmStatusNotProtected;
                }
            }

        if ( content )
            {
            delete content;
            content = NULL;
            }
        }

    PRINT1( _L( "MM MTP <= MmMtpDpUtility::GetDrmStatus, drmStatus: %d" ), drmStatus );

    return drmStatus;
    }

EXPORT_C void MmMtpDpUtility::SetPSStatus( TMtpPSStatus aStatus )
    {
    TBool changeScheduled = EFalse;
    CMmMtpDpAccessSingleton::CancelActiveToIdleStatusChange();  // cancel any outstanding delay status change
    
    if ( aStatus == EMtpPSStatusReadyToSync )
        {
        TInt mtpStatus;
        RProperty::Get( KMtpPSUid, KMtpPSStatus, mtpStatus );
        
        if ( mtpStatus == EMtpPSStatusActive )
            {
            CMmMtpDpAccessSingleton::ActiveToIdleStatusChange();
            changeScheduled = ETrue;
            }
        }

    if ( !changeScheduled )
        DoSetPSStatus( aStatus );
    }

void MmMtpDpUtility::DoSetPSStatus( TMtpPSStatus aStatus )
    {
    TInt mtpStatus;
    RProperty::Get( KMtpPSUid, KMtpPSStatus, mtpStatus );

    if ( mtpStatus != aStatus )
        {
        TInt err = RProperty::Set( KMtpPSUid, KMtpPSStatus, aStatus );
        PRINT3( _L("MM MTP <> CRequestProcessor::DoSetPSStatus err = %d, previous = %d, current = %d" ), err , mtpStatus, aStatus);
        }
    }
// end of file
