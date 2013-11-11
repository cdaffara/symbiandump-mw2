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
* Description:  Synchronous wrapper around CMPXImageUtil.
*
*/


#ifndef MPXIMAGEUTILSYNC_H
#define MPXIMAGEUTILSYNC_H

//  INCLUDES
#include <e32base.h>
#include <imageconversion.h>

// FORWARD DECLARATIONS
class CMPXImageUtil;

// CLASS DECLARATION

/**
* Declares the CMPXImageUtilSync class.
* @lib MPXSQLiteDbCommon.lib
**/
NONSHARABLE_CLASS( CMPXImageUtilSync ) : public CActive
    {
public:  // Constructors and destructor
    /**
    * Two-phased constructor.
    * @return pointer to a newly created object
    */
    static CMPXImageUtilSync* NewL();

    /**
    * Destructor.
    */
    ~CMPXImageUtilSync();

public: // New functions
    /*
    * Synchronously converts a JPG buffer to an CFbsBitmap.
    * @param aSourceJPG JPG data
    * @param aSize destination size to be used for calculating the optimal loading size
    * @param aDisplayMode the display mode of the destination BMP
    * @return converted bitmap. Ownership is transferred.
    */
    CFbsBitmap* DecodeL(const TDesC8& aSourceJPG, TSize aSize, TDisplayMode aDisplayMode = EColor64K);

    /*
    * Synchronously decodes an image from a JPG file. 
    * @param aSourceJPGFile Full path and filename of the image to be decoded.
    * @param aSize destination size to be used for calculating the optimal loading size
    * @param aDisplayMode the display mode of the destination BMP
    * @return converted bitmap. Ownership is transferred.
    */  
    CFbsBitmap* DecodeL(const TDesC& aSourceJPGFile, TSize aSize, TDisplayMode aDisplayMode = EColor64K);

    /*
    * Synchronously encodes a BMP to JPG.
    * @param aSourceBMP source BMP
    * @return destination JPG, ownership is transferred
    */
    HBufC8* EncodeL(const CFbsBitmap& aSourceBMP);
            
    /*
    * Synchronously scales a BMP to a specified size
    * @param aSourceBMP source BMP
    * @param aSize size to scale to
    * @param aDisplayMode display mode for the destination image
    * @return scaled BMP, ownership is transferred
    */
    CFbsBitmap* ScaleL(CFbsBitmap& aSourceBMP, TSize aSize, TDisplayMode aDisplayMode = EColor64K);

private:    // From CActive
    /**
    * @see CActive
    */
    void DoCancel();

    /**
    * @see CActive
    */
    void RunL();

    /**
    * @see CActive
    */
    TInt RunError(TInt aError);

private:
    /**
    * C++ default constructor.
    */
    CMPXImageUtilSync();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private:    // Data
    CActiveSchedulerWait* iActiveSchedulerWait;
    CMPXImageUtil* iImageUtil;
    CFbsBitmap* iBitmap;
    HBufC8* iData;
    TInt iError;
    };

#endif // MPXIMAGEUTILSYNC_H

// End of File
