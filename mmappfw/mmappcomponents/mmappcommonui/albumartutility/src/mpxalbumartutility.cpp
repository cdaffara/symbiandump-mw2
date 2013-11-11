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
* Description:  MPX album utility API 
*
*/

#include <e32std.h>
#include <mpxuser.h>
#include <mpxmedia.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxlog.h>

#include "mpximageutil.h"
#include "mpximageutilsync.h"
#include "mpxalbumartutility.h"

// ============================== MEMBER FUNCTIONS ============================

// ----------------------------------------------------------------------------
// Create the playback utility object
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXAlbumArtUtility* CMPXAlbumArtUtility::NewL()
    {
    CMPXAlbumArtUtility* self = new( ELeave )CMPXAlbumArtUtility();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXAlbumArtUtility::~CMPXAlbumArtUtility()
    {
    delete iImageUtil;
    delete iImageUtilSync;
    delete iBitmap;
    delete iAlbumArt;
    }

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
CMPXAlbumArtUtility::CMPXAlbumArtUtility() :
    CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }
    
// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
//
void CMPXAlbumArtUtility::ConstructL()
    {
    iImageUtil = CMPXImageUtil::NewL(); 
    iImageUtilSync = CMPXImageUtilSync::NewL(); 
    iBitmap = new ( ELeave ) CFbsBitmap();  
    }

// ----------------------------------------------------------------------------
// Create the playback utility object
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXAlbumArtUtility::ExtractAlbumArtL(const CMPXMedia& aMedia,
                                            MMPXAlbumArtUtilityObserver& aObs,
                                            const TSize& aSize, 
                                            TDisplayMode aDisplayMode /*= EColor64K*/)
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::ExtractAlbumArtL" );

    if ( iCurrentOp != EIdle )
        {
        User::Leave( KErrNotReady );
        }

    if ( aMedia.IsSupported( KMPXMediaMusicAlbumArtFileName ) )
        {
        if ( aMedia.ValueText( KMPXMediaMusicAlbumArtFileName ).Length() == 0)
            {
            User::Leave( KErrNotFound );
            }
        }

    if ( aMedia.IsSupported( KMPXMediaGeneralUri ) &&
         aMedia.IsSupported( KMPXMediaMusicAlbumArtFileName ))
        {
        if ( aMedia.ValueText( KMPXMediaGeneralUri ).CompareF( 
             aMedia.ValueText( KMPXMediaMusicAlbumArtFileName ) ) == 0 )
            { // embedded album art            
            iObs = &aObs;
            delete iAlbumArt;
            iAlbumArt = NULL;
            iAlbumArt = iImageUtil->ExtractL( aMedia.ValueText( KMPXMediaMusicAlbumArtFileName ) );
            if ( iAlbumArt )
                {
                iCurrentOp = EExtractAlbumArtL;
                iObs->ExtractAlbumArtStarted();
                SetActive();
                iImageUtil->Decode( iStatus, *iAlbumArt, *iBitmap, aSize, aDisplayMode );                
                }
            else
                {
                User::Leave( KErrUnderflow );
                }            
            }
        else
            {
            iCurrentOp = EExtractAlbumArtL;
            // TO-DO: if we save externalize bitmap, we only need to internalize bitmap here.
            iObs->ExtractAlbumArtStarted();
            SetActive();
            iImageUtil->Decode( iStatus, aMedia.ValueText( 
                KMPXMediaMusicAlbumArtFileName ), *iBitmap, aSize, aDisplayMode );
            }    
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    }

// -----------------------------------------------------------------------------
// Asynchronously decodes an image from a JPG file. Generates a BMP in memory.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXAlbumArtUtility::Decode(
    TRequestStatus& aStatus, const TDesC& aSourceJPGFile, 
    CFbsBitmap& aDestBMP, TSize aSize, TDisplayMode aDisplayMode )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::Decode Async File" );
    iImageUtil->Decode( aStatus, aSourceJPGFile, aDestBMP, aSize, aDisplayMode );
    }

// -----------------------------------------------------------------------------
// Asynchronously decodes an image from JPG buffer. Generates a BMP in memory.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXAlbumArtUtility::Decode(
    TRequestStatus& aStatus, const TDesC8& aSourceJPG, 
    CFbsBitmap& aDestBMP, TSize aSize, TDisplayMode aDisplayMode )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::Decode Async Buffer" );
    iImageUtil->Decode( aStatus, aSourceJPG, aDestBMP, aSize, aDisplayMode );
    }
    
// -----------------------------------------------------------------------------
// Asynchronously encodes a BMP to a JPG.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXAlbumArtUtility::Encode(
    TRequestStatus& aStatus, const CFbsBitmap& aSourceBMP, 
    HBufC8*& aDestJPG )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::Encode Async" );
    iImageUtil->Encode( aStatus, aSourceBMP, aDestJPG );
    }
    
// -----------------------------------------------------------------------------
// Asynchronously scales a BMP to a specified size. Generates another
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXAlbumArtUtility::Scale(
    TRequestStatus& aStatus, CFbsBitmap& aSourceBMP, CFbsBitmap& aDestBMP,
    TSize aSize, TDisplayMode aDisplayMode )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::Scale Async" );
    iImageUtil->Scale( aStatus, aSourceBMP, aDestBMP, aSize, aDisplayMode );
    }
    
// -----------------------------------------------------------------------------
// Synchronously decodes an image from a JPG file. 
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CMPXAlbumArtUtility::DecodeL(
    const TDesC& aSourceJPGFile, TSize aSize, TDisplayMode aDisplayMode )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::DecodeL Sync File" );
    return iImageUtilSync->DecodeL( aSourceJPGFile, aSize, aDisplayMode );
    }

// -----------------------------------------------------------------------------
// Synchronously converts a JPG buffer to an CFbsBitmap.
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CMPXAlbumArtUtility::DecodeL(
    const TDesC8& aSourceJPG, TSize aSize, TDisplayMode aDisplayMode )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::DecodeL Sync Buffer" );
    return iImageUtilSync->DecodeL( aSourceJPG, aSize, aDisplayMode );
    }
    
// -----------------------------------------------------------------------------
// Synchronously encodes a BMP to JPG.
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* CMPXAlbumArtUtility::EncodeL( const CFbsBitmap& aSourceBMP )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::EncodeL Sync" );
    return iImageUtilSync->EncodeL( aSourceBMP );
    }

// -----------------------------------------------------------------------------
// Synchronously scales a BMP to a specified size
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CMPXAlbumArtUtility::ScaleL( 
    CFbsBitmap& aSourceBMP, TSize aSize, TDisplayMode aDisplayMode )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::ScaleL Sync" );
    return iImageUtilSync->ScaleL( aSourceBMP, aSize, aDisplayMode );
    }
    
// -----------------------------------------------------------------------------
// Extracts embedded album art field from the media file.
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* CMPXAlbumArtUtility::ExtractL( const TDesC& aUri )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::ExtractL Sync" );
    return iImageUtil->ExtractL( aUri );
    }
    
// -----------------------------------------------------------------------------
// Returns a new bitmap object from bitmnap data
// -----------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* CMPXAlbumArtUtility::BitmapL( const TDesC8& aBMPData )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::BitmapL Sync" );
    return iImageUtil->BitmapL( aBMPData );
    }
    
// -----------------------------------------------------------------------------
// Returns bitmap data from a bitmap object
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* CMPXAlbumArtUtility::BitmapDataL( const CFbsBitmap& aBMP )
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::BitmapDataL Sync" );
    return iImageUtil->BitmapDataL( aBMP );
    }

// -----------------------------------------------------------------------------
// Cancel Asynch requests
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXAlbumArtUtility::CancelRequest()
    {
    MPX_FUNC_EX( "CMPXAlbumArtUtility::CancelRequest" );
    if ( iImageUtil )
        {
        iImageUtil->CancelRequest();
        }
    }

// -----------------------------------------------------------------------------
// CMPXImageUtilSync::DoCancel
// -----------------------------------------------------------------------------
//
void CMPXAlbumArtUtility::DoCancel()
    {
    MPX_FUNC( "CMPXAlbumArtUtility::DoCancel" );
    }

// -----------------------------------------------------------------------------
// CMPXImageUtilSync::RunL
// -----------------------------------------------------------------------------
//
void CMPXAlbumArtUtility::RunL()
    {
    MPX_FUNC( "CMPXAlbumArtUtility::RunL" );
    switch ( iCurrentOp )
        {
        case EExtractAlbumArtL:
            {
            iCurrentOp = EIdle;
            iObs->ExtractAlbumArtCompleted( iBitmap, KErrNone );
            delete iAlbumArt;
            iAlbumArt = NULL;
            iBitmap = NULL; // ownership transfered
            iBitmap = new ( ELeave ) CFbsBitmap();
            break;
            }
        default:
            {
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMPXImageUtilSync::RunError
// -----------------------------------------------------------------------------
//
TInt CMPXAlbumArtUtility::RunError( TInt /*aError*/ )
    {
    MPX_FUNC( "CMPXAlbumArtUtility::RunError" );
    return KErrNone;
    }


// End of file
