/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Extracts metadata from a file
*  Version     : %version: da1mmcf#38.1.4.2.6.1.5.3.9 % << Don't touch! Updated by Synergy at check-out.
*
*/

#include <e32base.h>
#include <badesca.h>
#include <apgcli.h>
#include <MetaDataUtility.h>
#include <MetaDataFieldContainer.h>

#ifdef RD_MPX_TNM_INTEGRATION
#include <hash.h>
#include <f32file.h>
#include <sysutil.h>
#include <thumbnailmanager.h>
#endif //RD_MPX_TNM_INTEGRATION


#include <mpxlog.h>
#include <mpxmedia.h>
#include <mpxcollectionpath.h>
#include <mpxcollectiontype.h>
#include <mpxdrmmediautility.h>

#include <mpxmediamusicdefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediamtpdefs.h>
#include <mpxmetadataextractorobserver.h>

#include "mpxmetadataextractor.h"
#include "mpxfileinfoutility.h"

_LIT( KWmaMimeType, "audio/x-ms-wma" );
_LIT( KWmaCafMimeType, "x-caf-audio/x-ms-wma" );

#ifdef RD_MPX_TNM_INTEGRATION
_LIT( KImageFileType, "image/jpeg" );
const TInt KMPXTimeoutTimer = 3000000; // 3 seconds
const TInt KMPXMaxThumbnailRequest = 2;
#endif //RD_MPX_TNM_INTEGRATION

#ifdef ABSTRACTAUDIOALBUM_INCLUDED
_LIT( KNonEmbeddedArtExt, ".alb" );
#endif
//Helper functions
static void FindAndReplaceForbiddenChars(TDes& aString, TInt aLen)
    {
    MPX_ASSERT(aLen == aString.Length());

    for (TInt i = 0; i < aLen; ++i)
        {
        if (aString[i] == TText('\t'))
            {
            aString[i] = TText(' ');
            }
        }
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXMetadataExtractor::CMPXMetadataExtractor(
                                  RFs& aFs,
                                  RApaLsSession& aAppArc,
                                  RPointerArray<CMPXCollectionType>& aTypes  )
                                  : iFs( aFs ),
                                    iAppArc( aAppArc ),
                                    iSupportedTypes( aTypes ),
                                    iCancelled( EFalse ),
                                    iObs( NULL ),
                                    iMedia( NULL ),
                                    iMetadataOnly( EFalse ),
                                    iFileOpenError( KErrNone )
    {

    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXMetadataExtractor::ConstructL()
    {
    iMetadataUtility = CMetaDataUtility::NewL();
    iDrmMediaUtility = CMPXDrmMediaUtility::NewL();
    iFileInfoUtil    = CMPXFileInfoUtility::NewL();
    iTaskTimer = CPeriodic::NewL( CActive::EPriorityIdle );

#ifdef RD_MPX_TNM_INTEGRATION
    // Create Thumbnail Manager instance. This object is the observer.
    iTNManager = CThumbnailManager::NewL( *this );
    iTNManager->SetFlagsL( CThumbnailManager::EDefaultFlags );
    iTNManager->SetQualityPreferenceL( CThumbnailManager::EOptimizeForQuality);
    // create wait loop
    iTNSyncWait = new (ELeave) CActiveSchedulerWait;
    iTimer = CPeriodic::NewL( CActive::EPriorityIdle );
    iArrayTNRequestId.Reset();
    iArrayTasks.Reset();
#endif //RD_MPX_TNM_INTEGRATION
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXMetadataExtractor* CMPXMetadataExtractor::NewL(
                                  RFs& aFs,
                                  RApaLsSession& aAppArc,
                                  RPointerArray<CMPXCollectionType>& aTypes  )
    {
    CMPXMetadataExtractor* self = new( ELeave ) CMPXMetadataExtractor( aFs,
                                                                       aAppArc,
                                                                       aTypes );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXMetadataExtractor::~CMPXMetadataExtractor()
    {
    delete iMetadataUtility;
    delete iFileInfoUtil;
    delete iDrmMediaUtility;
    delete iTaskTimer;
    iFile.Close();
    if ( iMedia != NULL )
        {
        delete iMedia;
        }
#ifdef RD_MPX_TNM_INTEGRATION
    delete iTNManager;
    if (iTNSyncWait && iTNSyncWait->IsStarted() )
        {
        iTNSyncWait->AsyncStop();
        }
    delete iTNSyncWait;
    delete iTimer;
    iArrayTNRequestId.Close();
    iArrayTasks.Close();
#endif //RD_MPX_TNM_INTEGRATION
    }

// ---------------------------------------------------------------------------
// Constructs a media properties object : synchronous function
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXMetadataExtractor::CreateMediaL( const TDesC& aFile,
                                                   CMPXMedia*& aNewProperty,
                                                   TBool aMetadataOnly )
    {
    MPX_FUNC("CMPXMetadataExtractor::CreateMediaL()");
    // check if we are still processing a request.
    if ( iArrayTasks.Count() )
        {
        MPX_DEBUG1("CMPXMetadataExtractor::CreateMediaL Request ongoing. Abort!" );
        User::Leave( KErrAbort );
        }

    iCancelled = EFalse;
    iFileOpenError = KErrNone;
    iObs = NULL;
    aNewProperty = NULL;
    iFileName = aFile;
    iMetadataOnly = aMetadataOnly;

    // populate the task array
    AddTasksL();

    // execute all tasks in the array
    while ( iArrayTasks.Count() )
        {
        // execute task at index 0
        MPX_TRAPD( error, ExecuteTaskL() );
        if ( error || iCancelled )
            {
            // cleanup
            if ( iMedia != NULL )
                {
                delete iMedia;
                iMedia = NULL;
                }
            iArrayTasks.Reset();
            if ( error )
                {
                CleanUp();
                User::LeaveIfError( error );
                }
            break;
            }

        iArrayTasks.Remove( 0 );
        }

    aNewProperty = iMedia;
    iMedia = NULL;  // ownership transferred.
    CleanUp();
    }

// ---------------------------------------------------------------------------
// Sets all of the default media properties
// ---------------------------------------------------------------------------
//
void CMPXMetadataExtractor::SetDefaultL( CMPXMedia& aMediaProp )
    {
    MPX_FUNC("CMPXMetadataExtractor::SetDefaultL()");
    // Comment
    aMediaProp.SetTextValueL( KMPXMediaGeneralComment,
                              KNullDesC );
    // Artist
    aMediaProp.SetTextValueL( KMPXMediaMusicArtist,
                              KNullDesC );
    // Album
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbum,
                              KNullDesC );
    // Track
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbumTrack,
                              KNullDesC );
    // Genre
    aMediaProp.SetTextValueL( KMPXMediaMusicGenre,
                              KNullDesC );
    // Composer
    aMediaProp.SetTextValueL( KMPXMediaMusicComposer,
                              KNullDesC );
    // Album artFilename
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbumArtFileName,
                              KNullDesC );
    // URL
    aMediaProp.SetTextValueL( KMPXMediaMusicURL,
                              KNullDesC );
    // AlbumArtist
    aMediaProp.SetTextValueL( KMPXMediaMusicAlbumArtist,
                              KNullDesC );
    
    }

// ---------------------------------------------------------------------------
// Sets media object attributes from metadata utilities
// ---------------------------------------------------------------------------
//
void CMPXMetadataExtractor::SetMediaPropertiesL()
    {
    MPX_FUNC("CMPXMetadataExtractor::SetMediaPropertiesL()");

    const TDesC& mimeType = iMedia->ValueText( KMPXMediaGeneralMimeType );
    HBufC8* mimeType8 = HBufC8::NewLC( mimeType.Length() );
    mimeType8->Des().Append( mimeType );

    // Continue to extract metadata even if fail.
    MPX_TRAPD( metadataerror, iMetadataUtility->OpenFileL( iFile, *mimeType8 ) );
    CleanupStack::PopAndDestroy( mimeType8 );

    // Get MetadataUtility Container
    const CMetaDataFieldContainer& metaCont = iMetadataUtility->MetaDataFieldsL();

    // Get DRM data
    // Need the protected flag from metadataUtility Container
    MPX_TRAPD( drmerror, SetDrmMediaPropertiesL( metaCont ) );
    MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL, result=%d getting drm data",
            drmerror );

    // Get metadata fields
    TInt count( metaCont.Count() );
    for( TInt i=0; i<count; ++i )
        {
        TMetaDataFieldId fieldType;

        HBufC* value = NULL;
        metaCont.FieldIdAt( i, fieldType );  // get the field type

        // get the value, except for album art
        if ( fieldType != EMetaDataJpeg )
           {
            MPX_TRAPD( err, value = metaCont.At( i, fieldType ).AllocL() );
           if ( KErrNone != err )
               {
               MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL - error = %i", err);
               continue;
               }
           CleanupStack::PushL( value );
           }

        switch( fieldType )
            {
            case EMetaDataSongTitle:
                {
                TPtr valptr = value->Des();
                valptr.Trim();
                TInt vallen = value->Length();
                if (vallen>0)
                    {
                    FindAndReplaceForbiddenChars(valptr, vallen);
                    iMedia->SetTextValueL(KMPXMediaGeneralTitle, *value);
                    }
                break;
                }
            case EMetaDataArtist:
                {
                TPtr valptr = value->Des();
                valptr.Trim();
                TInt vallen = value->Length();
                if (vallen>0)
                    {
                    FindAndReplaceForbiddenChars(valptr, vallen);
                    iMedia->SetTextValueL(KMPXMediaMusicArtist, *value);
                    }
                break;
                }
            case EMetaDataAlbum:
                {
                TPtr valptr = value->Des();
                valptr.Trim();
                TInt vallen = value->Length();
                if (vallen>0)
                    {
                    FindAndReplaceForbiddenChars(valptr, vallen);
                    iMedia->SetTextValueL(KMPXMediaMusicAlbum, *value );
                    }
                break;
                }
            case EMetaDataYear:
                {
                // Perform the date time conversion
                TLex lexer( *value );
                TInt year( 0 );
                lexer.Val( year );

                // Convert from TInt -> TDateTime -> TTime -> TInt64
                TDateTime dt;
                dt.SetYear( year );
                TTime time( dt );
                iMedia->SetTObjectValueL<TInt64>( KMPXMediaMusicYear,
                                                 time.Int64() );
                break;
                }
            case EMetaDataComment:
                {
                iMedia->SetTextValueL( KMPXMediaGeneralComment,
                                      *value );
                break;
                }
            case EMetaDataAlbumTrack:
                {
                iMedia->SetTextValueL( KMPXMediaMusicAlbumTrack,
                                      *value );
                break;
                }
            case EMetaDataGenre:
                {
                TPtr valptr = value->Des();
                valptr.Trim();
                TInt vallen = value->Length();
                if (vallen>0)
                    {
                    FindAndReplaceForbiddenChars(valptr, vallen);
                    iMedia->SetTextValueL(KMPXMediaMusicGenre, *value);
                    }
                break;
                }
            case EMetaDataComposer:
                {
                TPtr valptr = value->Des();
                valptr.Trim();
                TInt vallen = value->Length();
                if (vallen>0)
                    {
                    FindAndReplaceForbiddenChars(valptr, vallen);
                    iMedia->SetTextValueL(KMPXMediaMusicComposer, *value);
                    }
                break;
                }
            case EMetaDataUrl:
            case EMetaDataUserUrl:  // fall through
                {
                iMedia->SetTextValueL( KMPXMediaMusicURL,
                                      *value );
                break;
                }
            case EMetaDataJpeg:
                {
                // Album art handled in AddMediaAlbumArtL()
                break;
                }
            case EMetaDataCopyright:
                {
                iMedia->SetTextValueL( KMPXMediaGeneralCopyright,
                                      *value );
                break;
                }
            case EMetaDataDuration:
                {
                const TDesC& mimeType = iMedia->ValueText( KMPXMediaGeneralMimeType );
                MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL, mimeType = %S", &mimeType);

                // Verify if WMA, get the duration
                if( mimeType.Compare(KWmaMimeType) == 0 || mimeType.Compare(KWmaCafMimeType) == 0 )
                    {
                    MPX_DEBUG1("CMPXMetadataExtractor::SetMediaPropertiesL- WMA");

                    // Perform the duration conversion
                    TLex lexer( *value );
                    TInt32 duration ( 0 );
                    lexer.Val( duration );   // [second]
                    duration *= 1000;        // [msec]

                    iMedia->SetTObjectValueL<TInt32>( KMPXMediaGeneralDuration,
                                                duration );

                    MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL- duration = %i", duration);
                    }
                break;
                }
            case EMetaDataSampleRate:
                {
                const TDesC& mimeType = iMedia->ValueText( KMPXMediaGeneralMimeType );
                MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL, mimeType = %S", &mimeType);

                // Verify if WMA, get the sample rate
                if( mimeType.Compare(KWmaMimeType) == 0 || mimeType.Compare(KWmaCafMimeType) == 0 )
                    {
                    MPX_DEBUG1("CMPXMetadataExtractor::SetMediaPropertiesL- WMA");

                    // Perform the sample rate conversion
                    TLex lexer( *value );
                    TInt32 sampleRate ( 0 );
                    lexer.Val( sampleRate );

                    iMedia->SetTObjectValueL<TUint>( KMPXMediaAudioSamplerate,
                                                      sampleRate );

                    MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL- sample rate = %i", sampleRate);
                    }
                break;
                }
            case EMetaDataBitRate:
                {
                const TDesC& mimeType = iMedia->ValueText( KMPXMediaGeneralMimeType );
                MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL, mimeType = %S", &mimeType);

                // Verify if WMA, get the bit rate
                if( mimeType.Compare(KWmaMimeType) == 0 || mimeType.Compare(KWmaCafMimeType) == 0 )
                    {
                    MPX_DEBUG1("CMPXMetadataExtractor::SetMediaPropertiesL- WMA");

                    // Perform the bit rate conversion
                    TLex lexer( *value );
                    TInt32 bitRate ( 0 );
                    lexer.Val( bitRate );

                    iMedia->SetTObjectValueL<TUint>( KMPXMediaAudioBitrate,
                                                     bitRate );

                    MPX_DEBUG2("CMPXMetadataExtractor::SetMediaPropertiesL- bit rate = %i", bitRate);
                    }
                break;
                }
            case EMetaDataAlbumArtist:
                {
                TPtr valptr = value->Des();
                valptr.Trim();
                TInt vallen = value->Length();
                if (vallen>0)
                    {
                    FindAndReplaceForbiddenChars(valptr, vallen);
                    iMedia->SetTextValueL(KMPXMediaMusicAlbumArtist, *value);
                    }
                break;
                }
            case EMetaDataOriginalArtist:  // fall through
            case EMetaDataVendor:          // fall through
            case EMetaDataRating:          // fall through
            case EMetaDataUniqueFileIdentifier:  // fall through
            case EMetaDataDate:            // fall through
                {
                // not used
                break;
                }
            default:
                {
                //ASSERT(0);
                break;
                }
            }
        if (fieldType != EMetaDataJpeg)
            {
            CleanupStack::PopAndDestroy( value );
            }
        }
    }

// ---------------------------------------------------------------------------
// Sets extra media properties not returned by metadata utilities
// ---------------------------------------------------------------------------
//
void CMPXMetadataExtractor::SetExtMediaPropertiesL()
    {
    MPX_FUNC("CMPXMetadataExtractor::SetExtMediaPropertiesL()");

    //
    // File Size --- The following needs MMF support
    //
    TInt size( 0 );
    if( iFileOpenError == KErrNone )
        {
        const TDesC& mimeType = iMedia->ValueText( KMPXMediaGeneralMimeType );
        MPX_DEBUG2("CMPXMetadataExtractor::SetExtMediaPropertiesL, mimeType = %S", &mimeType);
        iFile.Size( size );
        iMedia->SetTObjectValueL<TInt>( KMPXMediaGeneralSize, size );

        // Verify if WMA, skip getting info from MMF
        if( mimeType.Compare(KWmaMimeType) == 0 || mimeType.Compare(KWmaCafMimeType) == 0 )
            {
            // No need to get MMF support
            MPX_DEBUG1("CMPXMetadataExtractor::SetExtMediaPropertiesL, skip MMF ");
            }
        else
            {
            MPX_DEBUG1("CMPXMetadataExtractor::SetExtMediaPropertiesL, get MMF controller");
            // Duration, bitrate, samplerate, etc
            //
            if( !iMetadataOnly )
            {
                MPX_TRAPD(err2, iFileInfoUtil->OpenFileL(
                          iFile,
                          iMedia->ValueText(KMPXMediaGeneralMimeType)));
                MPX_DEBUG2("CMPXMetadataExtractor::SetExtMediaPropertiesL, file info util error %i", err2);
                if( KErrNone == err2 )
                    {
                    // Get duration prior getting sample and bit rates
                    // MMF blocks for accumulating audio data to calculate duration
					// but MMF does not block for calculating sample rate or bit rate 
                    TInt64 duration = (TInt64) iFileInfoUtil->Duration().Int64() / 1000; // ms
                    iMedia->SetTObjectValueL<TInt32>( KMPXMediaGeneralDuration,
                                                    duration );
                                                    
                    iMedia->SetTObjectValueL<TUint>( KMPXMediaAudioBitrate,
                                                   iFileInfoUtil->BitRate() );
                    iMedia->SetTObjectValueL<TUint>( KMPXMediaAudioSamplerate,
                                                   iFileInfoUtil->SampleRate() );

                    MPX_DEBUG2("CMPXMetadataExtractor::SetExtMediaPropertiesL -- duration %i", duration);
                    }

                iFileInfoUtil->Reset();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Check to see if this file is a supported container
// ---------------------------------------------------------------------------
//
TInt CMPXMetadataExtractor::SupportedContainerTypeL( const TDesC& aFile,
                                                     TInt aIndex )
    {
    MPX_FUNC("CMPXMetadataExtractor::SupportedContainerTypeL()");
    TInt index(KErrNotFound);

    TDataType dataType;
    TUid dummyUid(KNullUid);
    iAppArc.AppForDocument(aFile, dummyUid, dataType);

    TInt index2(KErrNotFound);
    const CDesCArray& mimeTypes = iSupportedTypes[aIndex]->Mimetypes();
    if (!mimeTypes.FindIsq(dataType.Des(), index2))
        { // found
        index = index2;
        }

    return ( index == KErrNotFound ? KErrNotSupported : index );
    }


// ---------------------------------------------------------------------------
// CMPXMetadataExtractor::ThumbnailPreviewReady
// Callback but not used here
// ---------------------------------------------------------------------------
void CMPXMetadataExtractor::ThumbnailPreviewReady(
        MThumbnailData& /*aThumbnail*/, TThumbnailRequestId /*aId*/ )
    {
    MPX_FUNC("CMPXMetadataExtractor::ThumbnailPreviewReady()");
    }


// ---------------------------------------------------------------------------
// CMPXMetadataExtractor::ThumbnailReady
// Callback but not used here
// ---------------------------------------------------------------------------
void CMPXMetadataExtractor::ThumbnailReady( TInt /*aError*/,
        MThumbnailData& /*aThumbnail*/, TThumbnailRequestId aId )
    {
    MPX_FUNC("CMPXMetadataExtractor::ThumbnailReady()");

    // Remove thumbnail id from array.
    TInt index = iArrayTNRequestId.Find( aId );
    if ( index >= 0 )
        {
        iArrayTNRequestId.Remove( index );
        }
    MPX_DEBUG2("CMPXMetadataExtractor::ThumbnailReady(): Outstanding Thumbnail Request = %d",
            iArrayTNRequestId.Count());

    if ( iArrayTNRequestId.Count() < KMPXMaxThumbnailRequest )
        {
        StopWaitLoop();
        }
    }

// ----------------------------------------------------------------------------
// Callback for timer.
// ----------------------------------------------------------------------------
TInt CMPXMetadataExtractor::TimeoutTimerCallback(TAny* aPtr)
    {
    MPX_FUNC("CMPXMetadataExtractor::TimeoutTimerCallback()");

    CMPXMetadataExtractor* ptr =
        static_cast<CMPXMetadataExtractor*>(aPtr);

    // Timer expired before thumbnail callback occurred. Stop wait loop to unblock.
    ptr->StopWaitLoop();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Stop the wait loop.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::StopWaitLoop()
    {
    MPX_FUNC("CMPXMetadataExtractor::StopWaitLoop()");
    // Cancel timer
    CancelTimeoutTimer();

    // Stop wait loop to unblock.
    if ( iTNSyncWait->IsStarted() )
        {
        MPX_DEBUG1("CMPXMetadataExtractor::StopWaitLoop(): Stopping the wait loop.");
        iTNSyncWait->AsyncStop();
        }
    }

// ----------------------------------------------------------------------------
// Cancel timer.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::CancelTimeoutTimer()
    {
    MPX_FUNC("CMPXMetadataExtractor::CancelTimeoutTimer()");

    // Cancel timer.
    if ( iTimer && iTimer->IsActive() )
        {
        MPX_DEBUG1("CMPXMetadataExtractor::CancelTimeoutTimer(): Timer active, cancelling");
        iTimer->Cancel();
        }
    }

// ----------------------------------------------------------------------------
// Extract album art from a file and add to thumbnail manager.
// ----------------------------------------------------------------------------
EXPORT_C TInt CMPXMetadataExtractor::ExtractAlbumArtL( CMPXMedia* aMedia )
    {
    MPX_FUNC("CMPXMetadataExtractor::ExtractAlbumArtL()");
    TInt err = KErrNone;
    iCancelled = EFalse;

    if ( !aMedia->IsSupported(KMPXMediaGeneralUri) )
        {
        return KErrArgument;
        }

    // Get full file name.
    const TDesC& path = aMedia->ValueText(KMPXMediaGeneralUri);
    MPX_DEBUG2("CMPXMetadataExtractor::ExtractAlbumArtL Filename:%S ", &path );
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
    TParsePtrC parse( path );
    TPtrC ext( parse.Ext() );
    if (ext.CompareF(KNonEmbeddedArtExt)== 0)
        {
#ifdef RD_MPX_TNM_INTEGRATION

        //check if can send TN request, If thumbnail creation is ongoing, wait til it is done
        CheckBeforeSendRequest();

        CThumbnailObjectSource* source( NULL );
        if (aMedia->IsSupported(KMPXMediaMTPSampleData))
            {
            TBuf<20> mimeType(KImageFileType);
            TInt sampleData = aMedia->ValueTObjectL<TInt>(KMPXMediaMTPSampleData);
            HBufC8* value8 = ((HBufC8*)sampleData)->Des().AllocLC();   // make a local copy of sampleData
            source = CThumbnailObjectSource::NewLC(
                value8, mimeType, path );    // give up ownership of value8

            MPX_DEBUG1("CMPXMetadataExtractor::ExtractAlbumArtL source created from buffer");
            TThumbnailRequestId tnId = iTNManager->CreateThumbnails( *source );
            iArrayTNRequestId.Append( tnId );
            CleanupStack::PopAndDestroy( source );
            CleanupStack::Pop( value8 );
            }
        else
            {
            source = CThumbnailObjectSource::NewLC(
                path, KImageFileType  );

            MPX_DEBUG1("CMPXMetadataExtractor::ExtractAlbumArtL source created from path");
            TThumbnailRequestId tnId = iTNManager->CreateThumbnails( *source );
            iArrayTNRequestId.Append( tnId );
            CleanupStack::PopAndDestroy( source );
            }
#endif // RD_MPX_TNM_INTEGRATION
        }
    else
        {
#endif // ABSTRACTAUDIOALBUM_INCLUDED
    // create wanted fields array
    RArray<TMetaDataFieldId> wantedFields;
    CleanupClosePushL( wantedFields );
    wantedFields.Append(EMetaDataJpeg);

    // Open file
    if ( aMedia->IsSupported(KMPXMediaGeneralMimeType) )
        {
        const TDesC& mimeType = aMedia->ValueText( KMPXMediaGeneralMimeType );
        MPX_DEBUG2("CMPXMetadataExtractor::ExtractAlbumArtL MimeType:%S ", &mimeType );
        HBufC8* mimeType8 = HBufC8::NewLC( mimeType.Length() );
        mimeType8->Des().Append( mimeType );
        MPX_TRAP( err, iMetadataUtility->OpenFileL( path, wantedFields, *mimeType8 ) );
        CleanupStack::PopAndDestroy( mimeType8 );
        }
    else
        {
        MPX_TRAP( err, iMetadataUtility->OpenFileL( path, wantedFields ) );
        }
    CleanupStack::PopAndDestroy( &wantedFields );

    if ( !err )
        {
        //check if can send TN request, If thumbnail creation is ongoing, wait til it is done
        CheckBeforeSendRequest();
        MPX_TRAP( err, AddMediaAlbumArtL( *aMedia, path ));
        }

    // Reset the utility
    iMetadataUtility->ResetL();
#ifdef ABSTRACTAUDIOALBUM_INCLUDED
      }
#endif // ABSTRACTAUDIOALBUM_INCLUDED
    return err;
    }

// ----------------------------------------------------------------------------
// Add album art to media object.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::AddMediaAlbumArtL( CMPXMedia& aMedia,
                                               const TDesC& aFile )
    {
    MPX_FUNC("CMPXMetadataExtractor::AddMediaAlbumArtL()");

    // get metadata container.
    const CMetaDataFieldContainer& metaCont = iMetadataUtility->MetaDataFieldsL();
    TPtrC8 data8 = metaCont.Field8( EMetaDataJpeg );

    if ( data8.Length() )
        {
        MPX_DEBUG1("CMPXMetadataExtractor::GetMediaAlbumArtL(): Album art exist.");
#ifdef RD_MPX_TNM_INTEGRATION
        HBufC8* value8 = NULL;
        MPX_TRAPD( err, value8 = data8.AllocL() );
        if ( KErrNone != err )
            {
            MPX_DEBUG2("CMPXMetadataExtractor::GetMediaAlbumArtL - error jpeg = %i", err);
            return;
            }
        CleanupStack::PushL( value8 );

        TBuf<256> mimeType;
        mimeType.Copy( KImageFileType );
        CThumbnailObjectSource* source = CThumbnailObjectSource::NewL(
                value8, mimeType, aFile );
        TThumbnailRequestId tnId = iTNManager->CreateThumbnails( *source );
        iArrayTNRequestId.Append( tnId ); // add thumbnail id to array
        CleanupStack::Pop( value8 );
        delete source;

        aMedia.SetTextValueL( KMPXMediaMusicOriginalAlbumArtFileName, aFile );
#endif // RD_MPX_TNM_INTEGRATION
        aMedia.SetTextValueL( KMPXMediaMusicAlbumArtFileName, aFile );
        }
    }

// ----------------------------------------------------------------------------
// Check if can send request to TNM or not.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::CheckBeforeSendRequest()
     {
     MPX_FUNC("CMPXMetadataExtractor::CheckBeforeSendRequest()");
#ifdef RD_MPX_TNM_INTEGRATION
    MPX_DEBUG2("CMPXMetadataExtractor::CheckBeforeSendRequest(): Outstanding Thumbnail Request = %d",
            iArrayTNRequestId.Count());
	// If thumbnail creation is ongoing, wait til it is done
    if ( iArrayTNRequestId.Count() >= KMPXMaxThumbnailRequest )
        {
        MPX_DEBUG1("CMPXMetadataExtractor::CheckBeforeSendRequest(): Thumbnail creation ongoing!");
        // Cancel timer.
        CancelTimeoutTimer();
        // Start timer in case there is no callback from ThumbNail Manager.
        iTimer->Start(
            KMPXTimeoutTimer,
            KMPXTimeoutTimer,
            TCallBack(TimeoutTimerCallback, this ));

        // Start wait loop until we get a callback from ThumbNail Manager.
        if ( !iTNSyncWait->IsStarted() )
            {
            iTNSyncWait->Start();
            }
        }
#endif // RD_MPX_TNM_INTEGRATION
     }

// ----------------------------------------------------------------------------
// Cancel request. This will empty the task array and stop the wait loop. This
//                 will cause the CreateMediaL() to finish more quickly.
// ----------------------------------------------------------------------------
EXPORT_C void CMPXMetadataExtractor::CancelRequest()
    {
    MPX_FUNC("CMPXMetadataExtractor::CancelRequest()");
    iCancelled = ETrue;
    // Cancel all tasks
    iArrayTasks.Reset();
    // Cancel all thumbnail request
    CancelAllThumbnailRequests();
    StopWaitLoop();
    }

// ----------------------------------------------------------------------------
// Cancel all outstanding thumbnail requests.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::CancelAllThumbnailRequests()
    {
    MPX_FUNC("CMPXMetadataExtractor::CancelAllThumbnailRequests()");
#ifdef RD_MPX_TNM_INTEGRATION
    // TODO: remove comments when TNM make CancelRequest asynchronous.
    /*TInt count = iArrayTNRequestId.Count();
    for ( TInt i=0; i<count; i++ )
        {
        iTNManager->CancelRequest( iArrayTNRequestId[i] );
        }
    */
    iArrayTNRequestId.Reset();
#endif // RD_MPX_TNM_INTEGRATION
    }

// ----------------------------------------------------------------------------
// Create media and set default data and mimetype.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::DoCreateMediaL()
    {
    MPX_FUNC("CMPXMetadataExtractor::DoCreateMediaL()");
    MPX_ASSERT( iMedia == NULL );
    RArray<TInt> contentIDs;
    CleanupClosePushL( contentIDs );
    contentIDs.AppendL( KMPXMediaIdGeneral );
    contentIDs.AppendL( KMPXMediaIdAudio );
    contentIDs.AppendL( KMPXMediaIdMusic );
    contentIDs.AppendL( KMPXMediaIdDrm );
    contentIDs.AppendL( KMPXMediaIdMTP );
    iMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PopAndDestroy( &contentIDs );

    // CMPXMedia default types
    iMedia->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType,
                                              EMPXItem );
    iMedia->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
                                                  EMPXSong );

    TParsePtrC parse( iFileName );
    // Title, default is file name
    iMedia->SetTextValueL( KMPXMediaGeneralTitle,
                          parse.Name() );
    // Default album track
    iMedia->SetTextValueL( KMPXMediaMusicAlbumTrack,
                          KNullDesC );

    // Set the Mime Type and collection UID
    //
    if( !iMetadataOnly )
        {
        TInt index(KErrNotFound);
        TInt count( iSupportedTypes.Count() );
        for (TInt i=0; i <count; ++i)
            {
            TInt index2(KErrNotFound);
            const CDesCArray& exts = iSupportedTypes[i]->Extensions();
            const TDesC& ext = parse.Ext();
            if (!exts.FindIsq(ext, index2))
                { // found
                index = i;
                break;
                }
            }
        if( KErrNotFound != index )
            {
            MPX_DEBUG1("CMPXMetadataExtractor::DoCreateMediaL apparc <---" );
            TInt mimeIndex = SupportedContainerTypeL( iFileName, index );
            User::LeaveIfError( mimeIndex );
            MPX_DEBUG1("CMPXMetadataExtractor::DoCreateMediaL apparc --->" );

            iMedia->SetTextValueL( KMPXMediaGeneralMimeType,
                                  iSupportedTypes[index]->Mimetypes()[mimeIndex] );

            iMedia->SetTObjectValueL( KMPXMediaGeneralCollectionId,
                                     iSupportedTypes[index]->Uid() );
            }
        else
            {
            User::Leave(KErrNotSupported);
            }
        }
    else // other case use apparc to fetch and set mimetype
        {
        TDataType dataType;
        TUid dummyUid(KNullUid);
        // File handle based mime type recogniton required , because AppArc does 
        // not have All Files capa required for files in private folders 
        TDataRecognitionResult result;
        RFs fs;
		RFile rFile;
        User::LeaveIfError(fs.Connect());
		CleanupClosePushL(fs);
        User::LeaveIfError(fs.ShareProtected());
        User::LeaveIfError(rFile.Open(fs, iFileName, EFileShareReadersOrWriters));
		CleanupClosePushL(rFile);
        User::LeaveIfError(iAppArc.RecognizeData(rFile, result)); 
        CleanupStack::PopAndDestroy(&rFile);
        CleanupStack::PopAndDestroy(&fs);
        dataType = result.iDataType;
        iMedia->SetTextValueL( KMPXMediaGeneralMimeType,dataType.Des() );
        }

    // Initially set default tags.
    SetDefaultL( *iMedia );
    }

// ----------------------------------------------------------------------------
// Execute task at index 0.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::ExecuteTaskL()
    {
    MPX_FUNC("CMPXMetadataExtractor::ExecuteTasksL()");

    if ( iArrayTasks.Count() )
        {
        switch ( iArrayTasks[0] )
            {
            case ETaskCreateMedia:
                DoCreateMediaL();
                break;
            case ETaskAddMetadata:
                SetMediaPropertiesL();
                break;
            case ETaskAddExtMetadata:
                SetExtMediaPropertiesL();
                break;
            case ETaskAddAlbumArt:
                AddMediaAlbumArtL( *iMedia, iFileName );
                break;
            case ETaskCheckBeforeSend:
                CheckBeforeSendRequest();
                break;
            default:
                MPX_ASSERT(0); // Should never get here
            }
        }
    }

// ---------------------------------------------------------------------------
// Constructs a media properties object : asynchronous funcion
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXMetadataExtractor::CreateMediaAsyncL( const TDesC& aFile,
                                                        MMPXMetadataExtractorObserver* aObs,
                                                        TBool aMetadataOnly )
    {
    MPX_FUNC("CMPXMetadataExtractor::CreateMediaAsyncL()");
    // check if we are still processing a request.
    if ( iArrayTasks.Count() )
        {
        MPX_DEBUG1("CMPXMetadataExtractor::CreateMediaAsyncL Request ongoing. Abort!" );
        User::Leave( KErrAbort );
        }

    iCancelled = EFalse;
    iFileOpenError = KErrNone;
    iFileName = aFile;
    iObs = aObs;
    iMetadataOnly = aMetadataOnly;

    // populate the task array
    AddTasksL();

    // Start task timer to execute task
    if ( iArrayTasks.Count() )
        {
        if ( iTaskTimer->IsActive() )
            {
            iTaskTimer->Cancel();
            }
        iTaskTimer->Start( 0, 0, TCallBack(TaskTimerCallback, this ));
        }
    }

// ---------------------------------------------------------------------------
// Opens the file
// ---------------------------------------------------------------------------
//
TInt CMPXMetadataExtractor::OpenFile()
    {
    MPX_FUNC("CMPXMetadataExtractor::OpenFile()");

    // Open the file
    iFile.Close();
    TInt error = iFile.Open( iFs, iFileName, EFileRead | EFileShareReadersOrWriters );
    MPX_DEBUG2("CMPXMetadataExtractor::OpenFile open error = %d", error );
    return error;
    }

// ---------------------------------------------------------------------------
// Populat task array
// ---------------------------------------------------------------------------
//
void CMPXMetadataExtractor::AddTasksL()
    {
    MPX_FUNC("CMPXMetadataExtractor::AddTasks()");
    iFileOpenError = OpenFile();

    // Do not change the order of the task below.
    iArrayTasks.Reset();
    if ( iFileOpenError == KErrNone )
        {
        // to avoid resources contention (i.e. accessing sql, file system, etc)
        // Ordering the tasks - so no multiple tasks run at the same time
        // put the longest running task at the last (i.e. thumbnailing)
        iArrayTasks.AppendL(ETaskCreateMedia);
        iArrayTasks.AppendL(ETaskAddExtMetadata);   // Get the MMF first
        iArrayTasks.AppendL(ETaskAddMetadata);      // Get metadata & maybe drm data
        iArrayTasks.AppendL(ETaskCheckBeforeSend);
        iArrayTasks.AppendL(ETaskAddAlbumArt);      // Get album art / thumbnail last
        }
    else
        {
        iArrayTasks.AppendL(ETaskCreateMedia);
        iArrayTasks.AppendL(ETaskAddExtMetadata);
        }
    }

// ----------------------------------------------------------------------------
// Callback for timer.
// ----------------------------------------------------------------------------
TInt CMPXMetadataExtractor::TaskTimerCallback(TAny* aPtr)
    {
    MPX_FUNC("CMPXMetadataExtractor::TaskTimerCallback()");

    CMPXMetadataExtractor* ptr =
        static_cast<CMPXMetadataExtractor*>(aPtr);

    ptr->HandleTaskTimerExpired();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Handle task timer expired
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::HandleTaskTimerExpired()
    {
    MPX_FUNC("CMPXMetadataExtractor::HandleTaskTimerExpired()");

    if ( iTaskTimer && iTaskTimer->IsActive() )
        {
        iTaskTimer->Cancel();
        }
    // execute task at index 0
    MPX_TRAPD( error, ExecuteTaskL() );
    if ( error || iCancelled )
        {
        // cleanup
        if ( iMedia != NULL )
            {
            delete iMedia;
            iMedia = NULL;
            }
        iArrayTasks.Reset();
        }

    // Remove task at index 0.
    if ( iArrayTasks.Count() )
        {
        iArrayTasks.Remove( 0 );
        }

    // check if we have any more task to run
    if ( iArrayTasks.Count() )
        {
        // start task timer
        iTaskTimer->Start( 0, 0, TCallBack(TaskTimerCallback, this ));
        }
    else
        {
        // done
        if ( iObs && !iCancelled )
            {
            iObs->HandleCreateMediaComplete( iMedia, error );
            iMedia = NULL;  // ownership transferred.
            }

        CleanUp();
        }
    }

// ----------------------------------------------------------------------------
// Callback for timer.
// ----------------------------------------------------------------------------
void CMPXMetadataExtractor::CleanUp()
    {
    MPX_FUNC("CMPXMetadataExtractor::CleanUp()");
    // Reset the utility
    TRAP_IGNORE( iMetadataUtility->ResetL() );
    iFile.Close();
    }


// ---------------------------------------------------------------------------
// Sets DRRM media properties not returned by metadata utilities
// ---------------------------------------------------------------------------
//
void CMPXMetadataExtractor::SetDrmMediaPropertiesL(const CMetaDataFieldContainer& aMetaCont)
    {
    MPX_FUNC("CMPXMetadataExtractor::SetDrmMediaPropertiesL()");

    TBool prot(EFalse);
    // DB Flags to set
    TUint dbFlags(KMPXMediaGeneralFlagsSetOrUnsetBit);
    // Validate the objects to be used
    MPX_ASSERT(iMedia);
    MPX_ASSERT(iDrmMediaUtility);

    // File Path
    TParsePtrC parse( iFileName );
    iMedia->SetTextValueL( KMPXMediaGeneralUri, iFileName );
    iMedia->SetTextValueL( KMPXMediaGeneralDrive, parse.Drive() );
    MPX_DEBUG2("CMPXMetadataExtractor::SetDrmMediaPropertiesL - Filename=%S", &iFileName);

    // get the protected flag from MetadataUtility Container
    TPtrC ptr = aMetaCont.Field( EMetaDataProtected );
    MPX_DEBUG2("CMPXMetadataExtractor::SetDrmMediaPropertiesL -- status=%S", &ptr);
    _LIT( KNonProtected, "0" );
    if ( ptr.Compare( KNonProtected ) == 0 )
        {
        // this is non-protected file
        // prot = EFalse
        MPX_DEBUG1("CMPXMetadataExtractor::SetDrmMediaPropertiesL - non protected");
        iMedia->SetTObjectValueL<TBool>( KMPXMediaDrmProtected, prot );
        iMedia->SetTObjectValueL<TUint16>( KMPXMediaMTPDrmStatus, (TUint16)prot );
        }
    else
        {  // DRM file

        // DRM Rights
        //
        CMPXMedia* drm = NULL;
        MPX_TRAPD( drmError, iDrmMediaUtility->InitL( iFileName );
        drm = CMPXMedia::NewL( *iDrmMediaUtility->GetMediaL( KMPXMediaDrmProtected.iAttributeId |
                                                                KMPXMediaDrmRightsStatus.iAttributeId ) );
             );

        TBool prot(EFalse);
        if( drm )
            {
            CleanupStack::PushL( drm );
            MPX_DEBUG1("CMPXMetadataExtractor::SetDrmMediaPropertiesL -- is a drm file");
            if( drm->IsSupported( KMPXMediaDrmProtected ) )
                {
                prot = drm->ValueTObjectL<TBool>( KMPXMediaDrmProtected );
                MPX_DEBUG2("CMPXMetadataExtractor::SetDrmMediaPropertiesL protected %i", prot);
                }

            TMPXMediaDrmRightsStatus status(EMPXDrmRightsFull);
            if( drm->IsSupported( KMPXMediaDrmRightsStatus ) )
                {
                status = drm->ValueTObjectL<TMPXMediaDrmRightsStatus>(KMPXMediaDrmRightsStatus);
                iMedia->SetTObjectValueL<TInt>(KMPXMediaDrmRightsStatus, status );
                MPX_DEBUG2("CMPXMetadataExtractor::SetDrmMediaPropertiesL -- status %i", status);
                }

            // Set DB flags
            dbFlags |= KMPXMediaGeneralFlagsIsDrmProtected;
            if( status != EMPXDrmRightsFull && status != EMPXDrmRightsRestricted && status != EMPXDrmRightsPreview )
                {
                dbFlags |= KMPXMediaGeneralFlagsIsDrmLicenceInvalid;
                }
            CleanupStack::PopAndDestroy( drm );
            }

        // Trapped PV DRM error. If -46, we know the file has no rights
        //
        if( drmError == KErrPermissionDenied )
            {
            dbFlags |= KMPXMediaGeneralFlagsIsDrmLicenceInvalid;
            }
        else
            {
            User::LeaveIfError( drmError );
            }

        iMedia->SetTObjectValueL<TBool>( KMPXMediaDrmProtected, prot );
        iMedia->SetTObjectValueL<TUint16>( KMPXMediaMTPDrmStatus, (TUint16)prot );

        iDrmMediaUtility->Close();

        }  // DRM file


    // verify the file open status
    if( iFileOpenError == KErrNotFound || iFileOpenError == KErrPathNotFound )
        {
        dbFlags |= KMPXMediaGeneralFlagsIsInvalid;
        }

    // Finally set the db flag
    //
    iMedia->SetTObjectValueL( KMPXMediaGeneralFlags, dbFlags );

    }
