/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CMPXImageUtil.
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <s32mem.h>
#include <bitmaptransforms.h>
#include <mda/common/video.h>
#include <eikenv.h>
#include <MetaDataUtility.h>
#include <MetaDataFieldContainer.h>
#include <mpxuser.h>
#include <mpxlog.h>

#include "mpximageutil.h"

// CONSTANTS
const TInt KMPXBitmapBufferGranularity = 1024;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXImageUtil* CMPXImageUtil::NewL()
    {
    MPX_FUNC( "CMPXImageUtil::NewL" );
    CMPXImageUtil* self = new ( ELeave ) CMPXImageUtil();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self ); 
    return self; 
    }

// -----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// -----------------------------------------------------------------------------
//
CMPXImageUtil::CMPXImageUtil() :
    iOperation( EIdle )
    {
    MPX_FUNC( "CMPXImageUtil::CMPXImageUtil" );
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::ConstructL()
    {
    MPX_FUNC( "CMPXImageUtil::ConstructL" );
    User::LeaveIfError( iFs.Connect() );
    
    // this is needed in order to create CFbsBitmap instances and it has to be 
    // done before instantiating the bitmap scaler
    User::LeaveIfError( RFbsSession::Connect( iFs ) );

    iScaler = CBitmapScaler::NewL();
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXImageUtil::~CMPXImageUtil()
    {
    MPX_FUNC( "CMPXImageUtil::~CMPXImageUtil" );
    delete iDecoder; 
    delete iEncoder; 
    delete iScaler;
    
    if (RFbsSession::GetSession())
        {
        RFbsSession::Disconnect();
        }    
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// CMPXImageUtil::ExtractL
// Synchronous method.
// -----------------------------------------------------------------------------
//
HBufC8* CMPXImageUtil::ExtractL( const TDesC& aUri )
    {
    MPX_FUNC( "CMPXImageUtil::ExtractL" );
    
    HBufC8* ret( NULL );
    CMetaDataUtility* metaDataUtil = CMetaDataUtility::NewL();
    CleanupStack::PushL( metaDataUtil );

    RArray<TMetaDataFieldId> wantedFields;
    CleanupClosePushL( wantedFields );
    wantedFields.AppendL( EMetaDataJpeg );
    
    metaDataUtil->OpenFileL( aUri, wantedFields );
    const CMetaDataFieldContainer& metaCont = metaDataUtil->MetaDataFieldsL();
    TPtrC data = metaCont.Field( EMetaDataJpeg );
    
    if ( data.Length() )
        {
        // if no album art return NULL
        ret = MPXUser::Alloc8L( data );       
        }
    
    CleanupStack::PopAndDestroy( &wantedFields );
    CleanupStack::PopAndDestroy( metaDataUtil );
    
    return ret;    
    }

// -----------------------------------------------------------------------------
// CMPXImageUtil::Decode 
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::Decode(
    TRequestStatus& aStatus, 
    const TDesC& aSourceJPGFile, 
    CFbsBitmap& aDestBMP, 
    TSize aSize, 
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtil::Decode" );
    TRAPD( err, DoDecodeL( aStatus, aSourceJPGFile, aDestBMP, aSize, aDisplayMode ) );
    if ( err )
        {
        TRequestStatus* status = &aStatus;      
        User::RequestComplete( status, err );
        }
    }

// -----------------------------------------------------------------------------
// Starts to decode an image from a buffer. 
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::Decode(
    TRequestStatus& aStatus,
    const TDesC8& aJPGData, 
    CFbsBitmap& aDestBMP,     
    TSize aSize, 
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtil::Decode" );
    TRAPD( err, DoDecodeL( aStatus, aJPGData, aDestBMP, aSize, aDisplayMode ) );
    if ( err )
        {
        TRequestStatus* status = &aStatus;      
        User::RequestComplete( status, err );
        }
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::Encode
// Converts a BMP to a JPG.
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::Encode(
    TRequestStatus& aStatus,
    const CFbsBitmap& aSourceBMP,
    HBufC8*& aDestJPG )
    {
    MPX_FUNC( "CMPXImageUtil::Encode" );
    TRAPD( err, DoEncodeL( aStatus, aSourceBMP, aDestJPG ) );
    if ( err )
        {
        TRequestStatus* status = &aStatus;      
        User::RequestComplete( status, err );
        }
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::Scale
// Scales a BMP to a specified size.
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::Scale(
    TRequestStatus& aStatus,
    CFbsBitmap& aSrcBMP, 
    CFbsBitmap& aDestBMP,
    TSize aSize,     
    TDisplayMode aDisplayMode /* = EColor64K */)
    {    
    MPX_FUNC( "CMPXImageUtil::Scale" );
    TRAPD( err, DoScaleL( aStatus, aSrcBMP, aDestBMP, aSize, aDisplayMode ) );
    if ( err )
        {
        TRequestStatus* status = &aStatus;      
        User::RequestComplete( status, err );
        }
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::BitmapL
// -----------------------------------------------------------------------------
//
CFbsBitmap* CMPXImageUtil::BitmapL(
    const TDesC8& aBMPData)
    {
    MPX_FUNC( "CMPXImageUtil::BitmapL" );

    CFbsBitmap* bitmap = new ( ELeave ) CFbsBitmap;
    CleanupStack::PushL( bitmap );

    RDesReadStream stream( aBMPData );
    CleanupClosePushL( stream );
    stream >> *bitmap;
    CleanupStack::PopAndDestroy( &stream );
    
    CleanupStack::Pop( bitmap );

    return bitmap;
    }

// -----------------------------------------------------------------------------
// CMPXImageUtil::BitmapDataL
// -----------------------------------------------------------------------------
//
HBufC8* CMPXImageUtil::BitmapDataL(
    const CFbsBitmap& aBMP)
    {
    MPX_FUNC("CMPXImageUtil::BitmapDataL");
    
    CBufBase* buffer = CBufSeg::NewL( KMPXBitmapBufferGranularity );
    CleanupStack::PushL( buffer );

    RBufWriteStream stream( *buffer );
    CleanupClosePushL( stream );
    stream << aBMP;
    stream.CommitL();
    CleanupStack::PopAndDestroy( &stream );

    TInt length( buffer->Size() );
    HBufC8* data = HBufC8::NewL( length );
    TPtr8 ptr( data->Des() );
    buffer->Read( 0, ptr, length );

    CleanupStack::PopAndDestroy( buffer );

    return data;
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::CancelRequest
// Cancel Asynch requests
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::CancelRequest()
    {
    MPX_FUNC( "CMPXImageUtil::CancelRequest" );
    switch ( iOperation )
        {
        case EDecoding:
            {
            if( iDecoder )
                {
                iDecoder->Cancel();
                delete iDecoder;
                iDecoder = NULL;
                }
            break;
            }
        case EEncoding:
            {
            if( iEncoder )
                {
                iEncoder->Cancel();
                delete iEncoder;
                iEncoder = NULL;
                }
            break;
            }
        case EScaling:
            {
            if( iScaler )
                {
                iScaler->Cancel();
                }
            break;
            }
        default: 
            {
            // No Asynchronous events are taking place, do nothing.
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CMPXImageUtil::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::DoDecodeL(
    TRequestStatus& aStatus, 
    const TDesC& aSourceJPGFile, 
    CFbsBitmap& aDestBMP, 
    TSize aSize,    
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtil::DoDecodeL" );
    
    delete iDecoder; 
    iDecoder = NULL;
    // create the decoder
    iDecoder = CImageDecoder::FileNewL( iFs, aSourceJPGFile );
    
    // Get image size
    const TFrameInfo& frameInfo = iDecoder->FrameInfo();
    TSize bitmapSize = frameInfo.iOverallSizeInPixels;
    bitmapSize = OptimalLoadingSize( bitmapSize, aSize );

    User::LeaveIfError( aDestBMP.Create( bitmapSize, aDisplayMode ) ); 

    // start conversion to bitmap
    iOperation = EDecoding;
    iDecoder->Convert( &aStatus, aDestBMP );
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::DoDecodeL
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::DoDecodeL(
    TRequestStatus& aStatus, 
    const TDesC8& aSourceJPG, 
    CFbsBitmap& aDestBMP,     
    TSize aSize,    
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtil::DoDecodeL" );

    delete iDecoder; 
    iDecoder = NULL;

    // create the decoder
    iDecoder = CImageDecoder::DataNewL( iFs, aSourceJPG );

    // Get image size
    const TFrameInfo& frameInfo = iDecoder->FrameInfo();
    TSize bitmapSize = frameInfo.iOverallSizeInPixels;
    bitmapSize = OptimalLoadingSize( bitmapSize, aSize );

    // create the destination bitmap
    User::LeaveIfError( aDestBMP.Create( bitmapSize, aDisplayMode ) ); 

    // start conversion to bitmap
    iOperation = EDecoding;
    iDecoder->Convert( &aStatus, aDestBMP );
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::DoEncodeL
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::DoEncodeL(
    TRequestStatus& aStatus, 
    const CFbsBitmap& aSourceBMP, 
    HBufC8*& aDestJPG)
    {
    MPX_FUNC( "CMPXImageUtil::DoEncodeL" );
    
    // Encode as JPEG
    delete iEncoder;
    iEncoder = NULL;
    iEncoder = CImageEncoder::DataNewL(
        aDestJPG, CImageEncoder::EOptionNone, KImageTypeJPGUid );
    iOperation = EEncoding;
    iEncoder->Convert( &aStatus, aSourceBMP );        
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtil::DoScaleL
// -----------------------------------------------------------------------------
//
void CMPXImageUtil::DoScaleL(
    TRequestStatus& aStatus, 
    CFbsBitmap& aSourceBMP, 
    CFbsBitmap& aDestBMP,
    TSize aSize, 
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtil::DoScaleL" );

    // create the destination bitmap
    User::LeaveIfError( aDestBMP.Create( aSize, aDisplayMode ) );
    
    // if TBool aMaintainAspectRatio = ETrue, then the image may not have be
    // 95x95.  If TBool aMaintainAspectRatio = EFalse, the image may appear
    // to be stretched, as the aspect ratio is not maintained.
    iOperation = EScaling;
    iScaler->Scale( &aStatus, aSourceBMP, aDestBMP, ETrue );
    }

// -----------------------------------------------------------------------------
// Determines optimum loading size for the bitmap.
// -----------------------------------------------------------------------------
TSize CMPXImageUtil::OptimalLoadingSize(
    const TSize& aOriginalSize,
    const TSize& aNeededSize )
    {
    TSize size = AdjustSize( aOriginalSize, aNeededSize );
    
    // Find max scaling factor which won't make image smaller than target size
    TInt shift = 3;
    while (shift && ( ( aOriginalSize.iWidth >> shift ) < size.iWidth || 
        ( aOriginalSize.iHeight >> shift ) < size.iHeight ) )
        {
        shift--;
        }

    TInt round = ( 1 << shift ) - 1;  // Used to "round up" the scaled values
    TSize loadSize( ( aOriginalSize.iWidth + round ) >> shift,
        ( aOriginalSize.iHeight + round ) >> shift );
        
    return loadSize;
    }

// -----------------------------------------------------------------------------
// CMPXImageUtil::AdjustSize
// -----------------------------------------------------------------------------
//
TSize CMPXImageUtil::AdjustSize(
    const TSize& aSourceSize,
    const TSize& aTargetSize )
    {
    TSize size( aTargetSize );
    
    if ( aTargetSize.iHeight * aSourceSize.iWidth < 
        aTargetSize.iWidth * aSourceSize.iHeight )
        {
        // Source has taller aspect than target so reduce target width
        size.iWidth = ( aTargetSize.iHeight * aSourceSize.iWidth ) / 
            aSourceSize.iHeight;
        }
    else
        {
        // Source has wider aspect than target so reduce target height
        size.iHeight = ( aTargetSize.iWidth * aSourceSize.iHeight ) / 
            aSourceSize.iWidth;
        }
        
    return size;        
    }

//  End of File  
