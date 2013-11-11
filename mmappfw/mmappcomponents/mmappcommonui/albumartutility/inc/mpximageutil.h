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
* Description:  Class declaration of CMPXImageUtil.
*
*/


#ifndef MPXIMAGEUTIL_H
#define MPXIMAGEUTIL_H

//  INCLUDES
#include <e32base.h>
#include <imageconversion.h>

// FORWARD DECLARATIONS
class CFbsBitmap;
class CBitmapScaler;
class CImageDecoder;
class CImageEncoder;

// CLASS DECLARATION

/**
* Image utilities class.
* @lib MPXSQLiteDbCommon.lib
**/
NONSHARABLE_CLASS( CMPXImageUtil ) : public CBase
    {
public:  // Constructors and destructor        
    /**
    * Two-phased constructor.
    * @return A pointer to a newly created object
    */
    static CMPXImageUtil* NewL();

    /**
    * Destructor.
    */
    ~CMPXImageUtil();

public: // New functions
      
    /**
    * Extracts embedded album art field from the media file.
    * @param aUri file URI
    * @return embebded JPG album art, ownership transferred. 
    */
    HBufC8* ExtractL(const TDesC& aUri);
        
    /*
    * Asynchronously decodes an image from a JPG file. Generates a BMP in memory.
    * @param aStatus request status
    * @param aSourceJPGFile Full path and filename of the image to be decoded.
    * @param aDestBMP Destination bitmap, allocated inside.
    * @param aSize destination size to be used for calculating the optimal loading size    
    * @param aDisplayMode the display mode of the destination BMP
    */  
    void Decode(TRequestStatus& aStatus, const TDesC& aSourceJPGFile, 
        CFbsBitmap& aDestBMP, TSize aSize, TDisplayMode aDisplayMode = EColor64K);
  
    /*
    * Asynchronously decodes an image from JPG buffer. Generates a BMP in memory.
    * @param aStatus request status
    * @param aSourceJPG JPG data
    * @param aDestBMP destination bitmap, created inside.
    * @param aSize destination size to be used for calculating the optimal loading size    
    * @param aDisplayMode the display mode of the destination BMP
    */
    void Decode(TRequestStatus& aStatus, const TDesC8& aSourceJPG, CFbsBitmap& aDestBMP,  
        TSize aSize, TDisplayMode aDisplayMode = EColor64K);
        
    /*
    * Asynchronously encodes a BMP to a JPG.
    * @param aStatus request status for ther asynchronous operation
    * @param aSourceBMP source BMP
    * @param aDestJPG destination JPG
    */
    void Encode(TRequestStatus& aStatus, const CFbsBitmap& aSourceBMP, 
        HBufC8*& aDestJPG);
            
    /*
    * Asynchronously scales a BMP to a specified size. Generates another 
    * BMP in memory.
    * @param aStatus request status for ther asynchronous operation
    * @param aSourceBMP source BMP
    * @param aDestBMP destination BMP
    * @param aSize size to scale to
    * @param aDisplayMode display mode for the destination image
    */
    void Scale(TRequestStatus& aStatus, CFbsBitmap& aSourceBMP, CFbsBitmap& aDestBMP,
        TSize aSize, TDisplayMode aDisplayMode = EColor64K);
    
    /*
    * Returns a new bitmap object from bitmnap data
    * @param aBMPData source BMP data
    * @return new bitmap object. Ownership is transferred.
    */
    CFbsBitmap* BitmapL(const TDesC8& aBMPData);

    /*
    * Returns bitmap data from a bitmap object
    * @param aBMP source BMP
    * @return bitmap data. Ownership is transferred.
    */
    HBufC8* BitmapDataL(const CFbsBitmap& aBMP);
                
    /*
    * Cancels any outstanding asynchronous request.
    */
    void CancelRequest();
    
private: // New functions
    /*
    * Asynchronously decodes an image from a JPG file - leaving version.
    * @param aStatus request status
    * @param aSourceJPGFile Full path and filename of the image to be decoded.
    * @param aDestBMP Destination bitmap, allocated inside.
    * @param aSize size that should be considered when loading the image. 
    *           Used to determine the optimal loading size.
    * @param aDisplayMode the display mode of the destination BMP
    */  
    void DoDecodeL(TRequestStatus& aStatus, const TDesC& aSourceJPGFile, CFbsBitmap& aDestBMP, 
        TSize aSize, TDisplayMode aDisplayMode = EColor64K);
  
    /*
    * Asynchronously decodes an image from JPG buffer - leaving version.
    * @param aStatus request status
    * @param aSourceJPG JPG data
    * @param aDestBMP destination bitmap, allocated inside.
    * @param aSize size that should be considered when loading the image. 
    *           Used to determine the optimal loading size.
    * @param aDisplayMode the display mode of the destination BMP
    */
    void DoDecodeL(TRequestStatus& aStatus, const TDesC8& aSourceJPG, CFbsBitmap& aDestBMP,     
        TSize aSize, TDisplayMode aDisplayMode = EColor64K);
        
    /*
    * Asynchronously encodes a BMP to JPG - leaving version.
    * @param aStatus request status for ther asynchronous operation
    * @param aSourceBMP source BMP
    * @param aDestJPG destination JPG
    */
    void DoEncodeL(TRequestStatus& aStatus, const CFbsBitmap& aSourceBMP, 
        HBufC8*& aDestJPG);
            
    /*
    * Asynchronously scales a BMP to a specified size - leaving version.
    * @param aStatus request status for ther asynchronous operation
    * @param aSourceBMP source BMP
    * @param aDestBMP destination BMP
    * @param aSize size to scale to
    * @param aDisplayMode display mode for the destination image
    */
    void DoScaleL(TRequestStatus& aStatus, CFbsBitmap& aSourceBMP, 
        CFbsBitmap& aDestBMP, TSize aSize, TDisplayMode aDisplayMode = EColor64K);

    /**
    * Gets an optimum loading size for the bitmap. You have a bitmap that has a
    * size of 640x480. You need to produce a bitmap with size 44x48 calling 
    * this method returns the maximum downscaled size: 80, 60. Using this size
    * (80x60) you get the fastest loading speed and less usage of memory when
    * you load the bitmap in memory. This method also handles the needed
    * rounding if the original size can not be divided evenly!
    * @param aOriginalSize the size of the unscaled bitmap  
    * @param aNeededSize the needed size  
    * @return the optimal loading size  
    */
    TSize OptimalLoadingSize(const TSize& aOriginalSize, const TSize& aNeededSize);

    /**
    * Reduce target size to same aspect ratio as source image.
    * @param aSourceSize source size
    * @param aTargetSize target size
    * @return adjusted size 
    */
    TSize AdjustSize(const TSize& aSourceSize, const TSize& aTargetSize);
    
private:

    /**
    * C++ default constructor.
    */
    CMPXImageUtil(); 

    /**
    * Second phase constructor.
    */
    void ConstructL();  
      
private:   
    // Internal operation codes
    enum TOperation
        {
        EIdle = 0,
        EDecoding,
        EEncoding,
        EScaling
        };
      
private:    // Data
    CImageDecoder* iDecoder;        // decoder
    CBitmapScaler* iScaler;         // bitmap scaler
    CImageEncoder* iEncoder;        // encoder    
    RFs iFs;                        // for opening/saving images from/to files
    TOperation iOperation;          // current operation
    };

#endif // MPXIMAGEUTIL_H   
            
// End of File
