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
* Description:  MPX album art utility
*
*/

 
#ifndef CMPXALBUMARTUTILITY_H
#define CMPXALBUMARTUTILITY_H

#include <e32def.h>
#include <gdi.h>
#include <MetaDataField.hrh>

class CMPXMedia;
class CMPXImageUtil;
class CMPXImageUtilSync;
class CFbsBitmap;

// CLASS DECLARATION

/**
 *  Observer class for Async util operation
 *
 * @lib mpxalbumartutility.lib
 */
NONSHARABLE_CLASS( MMPXAlbumArtUtilityObserver )
    {
public:
    /**
     *  Notify that extraction of album art started.
     *
     *  @since S60 3.2.3
     */
    virtual void ExtractAlbumArtStarted() = 0;

    /**
     *  Notify that the extraction of album art has completed.
     *
     *  @since S60 3.2.3
     *  @param aBitmap a converted image.
     *  @param aErr error code
     */
    virtual void ExtractAlbumArtCompleted( CFbsBitmap* aBitmap, TInt aErr ) = 0;

    };

/**
 *  Music Player common utility class
 *
 *  @lib mpxalbumartutility.lib
 */
class CMPXAlbumArtUtility: public CActive
    {
public: 
    /**
     *  Two phased contructor
     *
     *  @since S60 3.2.3
     */ 
    IMPORT_C static CMPXAlbumArtUtility* NewL(); 
    
    /**
     *  Destuctor
     *
     *  @since S60 3.2.3
     */ 
    IMPORT_C ~CMPXAlbumArtUtility();
   
public: // async API
    /**
     *  Extract album art data. 
     *
     *  @since S60 3.2.3
     *  @param aProp media property the media must contain the following
     *               attributes:
     *               TMPXAttribute( KMPXMediaIdMusic, EMPXMediaMusicAlbumArtFileName )
     *               TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralUri )
     *  @param aObs callback after the operation completed.
     *  @param aSize The size of the required destination BMP.
     *  @param aDisplayMode the display mode of the destination BMP
     *  @leave KErrNotReady, KErrNotFound, or KErrUnderflow
     *                (Album Art, album art file name is not found 
     *                 or object is not ready)
     */
    IMPORT_C void ExtractAlbumArtL(const CMPXMedia& aProp, 
                                   MMPXAlbumArtUtilityObserver& aObs,
                                   const TSize& aSize, 
                                   TDisplayMode aDisplayMode = EColor64K);

    /**
     * Asynchronously decodes an image from a JPG file. Generates a BMP in memory.
     *
     * @since S60 3.2.3
     * @param aStatus request status
     * @param aSourceJPGFile Full path and filename of the image to be decoded.
     * @param aDestBMP Destination bitmap, allocated inside.
     * @param aSize destination size to be used for calculating the optimal loading size    
     * @param aDisplayMode the display mode of the destination BMP
     */  
    IMPORT_C void Decode(TRequestStatus& aStatus, const TDesC& aSourceJPGFile, 
        CFbsBitmap& aDestBMP, TSize aSize, TDisplayMode aDisplayMode = EColor64K);
  
    /**
     * Asynchronously decodes an image from JPG buffer. Generates a BMP in memory.
     *
     * @since S60 3.2.3
     * @param aStatus request status
     * @param aSourceJPG JPG data
     * @param aDestBMP destination bitmap, created inside.
     * @param aSize destination size to be used for calculating the optimal loading size    
     * @param aDisplayMode the display mode of the destination BMP
     */
    IMPORT_C void Decode(TRequestStatus& aStatus, const TDesC8& aSourceJPG, CFbsBitmap& aDestBMP,  
        TSize aSize, TDisplayMode aDisplayMode = EColor64K);
        
    /**
     * Asynchronously encodes a BMP to a JPG.
     *
     * @since S60 3.2.3
     * @param aStatus request status for ther asynchronous operation
     * @param aSourceBMP source BMP
     * @param aDestJPG destination JPG
     */
    IMPORT_C void Encode(TRequestStatus& aStatus, const CFbsBitmap& aSourceBMP, 
        HBufC8*& aDestJPG);
            
    /**
     * Asynchronously scales a BMP to a specified size. Generates another 
     * BMP in memory.
     *
     * @since S60 3.2.3
     * @param aStatus request status for ther asynchronous operation
     * @param aSourceBMP source BMP
     * @param aDestBMP destination BMP
     * @param aSize size to scale to
     * @param aDisplayMode display mode for the destination image
     */
    IMPORT_C void Scale(TRequestStatus& aStatus, CFbsBitmap& aSourceBMP, CFbsBitmap& aDestBMP,
        TSize aSize, TDisplayMode aDisplayMode = EColor64K);
    
public: // sync API    

    /**
     * Synchronously decodes an image from a JPG file. 
     *
     * @since S60 3.2.3
     * @param aSourceJPGFile Full path and filename of the image to be decoded.
     * @param aSize destination size to be used for calculating the optimal loading size
     * @param aDisplayMode the display mode of the destination BMP
     * @return converted bitmap. Ownership is transferred.
     */  
    IMPORT_C CFbsBitmap* DecodeL(const TDesC& aSourceJPGFile, TSize aSize, TDisplayMode aDisplayMode = EColor64K);
    
    /**
     * Synchronously converts a JPG buffer to an CFbsBitmap.
     *
     * @since S60 3.2.3
     * @param aSourceJPG JPG data
     * @param aSize destination size to be used for calculating the optimal loading size
     * @param aDisplayMode the display mode of the destination BMP
     * @return converted bitmap. Ownership is transferred.
     */
    IMPORT_C CFbsBitmap* DecodeL(const TDesC8& aSourceJPG, TSize aSize, TDisplayMode aDisplayMode = EColor64K);

    /**
     * Synchronously encodes a BMP to JPG.
     *
     * @since S60 3.2.3
     * @param aSourceBMP source BMP
     * @return destination JPG, ownership is transferred
     */
    IMPORT_C HBufC8* EncodeL(const CFbsBitmap& aSourceBMP);
            
    /**
     * Synchronously scales a BMP to a specified size
     *
     * @since S60 3.2.3
     * @param aSourceBMP source BMP
     * @param aSize size to scale to
     * @param aDisplayMode display mode for the destination image
     * @return scaled BMP, ownership is transferred
     */
    IMPORT_C CFbsBitmap* ScaleL(CFbsBitmap& aSourceBMP, TSize aSize, TDisplayMode aDisplayMode = EColor64K);

   /**
    * Extracts embedded album art field from the media file.
    *
    * @since S60 3.2.3
    * @param aUri file URI
    * @return embebded JPG album art. Ownership is transferred. 
    */
    IMPORT_C HBufC8* ExtractL(const TDesC& aUri);
    
    /**
     * Returns a new bitmap object from bitmnap data.
     *
     * @since S60 3.2.3
     * @param aBMPData source BMP data
     * @return new bitmap object. Ownership is transferred.
     */
    IMPORT_C CFbsBitmap* BitmapL(const TDesC8& aBMPData);

    /**
     * Returns bitmap data from a bitmap object.
     *
     * @since S60 3.2.3
     * @param aBMP source BMP
     * @return bitmap data. Ownership is transferred.
     */
    IMPORT_C HBufC8* BitmapDataL(const CFbsBitmap& aBMP);
    
   /**
    * Cancel outstanding asynch requests.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void CancelRequest(); 
   
private:

    /**
     * C++ constructor
     *
     * @since S60 3.2.3
     */
    CMPXAlbumArtUtility();

    /**
     * Second phase constructor
     *
     * @since S60 3.2.3
     *
     */
    void ConstructL();        
    
private:    // From CActive
    /**
     * @since S60 3.2.3
     * @see CActive
     */
    void DoCancel();

    /**
     * @since S60 3.2.3
     * @see CActive
     */
    void RunL();

    /**
    *  @since S60 3.2.3
     * @see CActive
     */
    TInt RunError(TInt aError);

private:   
    // Internal operation codes
    enum TOperation
        {
        EIdle = 0,
        EExtractAlbumArtL
        };
    
private:
    CMPXImageUtil* iImageUtil;
    CMPXImageUtilSync* iImageUtilSync;
    MMPXAlbumArtUtilityObserver* iObs;    
    TOperation iCurrentOp;
    HBufC8* iAlbumArt;
    CFbsBitmap* iBitmap;    
    };

#endif      // CMPXALBUMARTUTILITY_H

// End of File
