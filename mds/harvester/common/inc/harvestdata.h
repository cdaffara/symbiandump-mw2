/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester image plugin data transfer objects
*
*/



#ifndef HARVESTDATA_H
#define HARVESTDATA_H

#include "mdeobject.h"
#include "mdequery.h"
#include "f32file.h"


/**
* Class used to store gps location data to an array.
* A data transfer object.
*/
class CLocationData : public CBase
    {
    public:
        /** Standard NewL */
        inline static CLocationData* NewL( TReal64 aGpsLatitude, TReal64 aGpsLongitude,
            TReal64 aGpsAltitude, TBool aGpsLaLExists, TBool aGpsAExists );
        
    private:
        /** Constructor. */
        inline CLocationData( TReal64 aGpsLatitude, TReal64 aGpsLongitude,
            TReal64 aGpsAltitude, TBool aGpsLaLExists, TBool aGpsAExists );

    public:

        /** GPS latitude */
        TReal64 iGpsLatitude;

        /** GPS longitude */
        TReal64 iGpsLongitude;

        /** GPS altitude */
        TReal64 iGpsAltitude;

        /** GPS latitude and longitude exists */
        TBool   iGpsLatAndLongExists;

        /** GPS altitude exists */
        TBool   iGpsAltitudeExists;
    };

/**
* This class is used to match corresponding mde object and location data (if any) with two arrays.
* Arrays are (and must be) always be of same size.
*/
class CObjectDataArray : public CBase
    {
    public:

        /** Standard NewL */
        inline static CObjectDataArray* NewL();
        
        /**
        * Virtual destructor.
        */
        inline virtual ~CObjectDataArray();
        
        /**
        * Appends a new entry to the array.
        * @param aMdeObject  A mde object pointer
        * @param aLocationData  Related location data
        * @param aQuery  Location query pointer for prepared queries
        */
        inline void Append( CMdEObject* aMdeObject,
        		CLocationData* aLocationData, CMdEQuery* aQuery = NULL );

        /**
        * Removes an entry from the array.
        */
        inline void Remove( TInt aIndex );

        /**
        * Returns how many entries there are in the array.
        * @return Count on entries.
        */
        inline TInt Count();
        
        /**
        * Destroys the array. Frees memory.
        */
        inline void ResetAndDestroy();
        
        /**
        * Closes the array.
        */
        inline void Close();
        
        /**
        * Returns a mde object by index.
        * Leaves if index is out of bounds.
        * @return A pointer to mde object in the array.
        */
        inline CMdEObject* ObjectL( const TInt aIndex );

        /**
        * Returns a location data object by index.
        * Leaves if index is out of bounds.
        * @return A location data object in the array.
        */
        inline CLocationData* LocationDataL( const TInt aIndex );
        
        /**
        * Returns a query pointer by index.
        * Might be NULL.
        * Leaves if index is out of bounds.
        * @return A mde query pointer in the array.
        */
        inline CMdEQuery* QueryL( const TInt aIndex );

        /**
        * Sets a query pointer by index in the array to the given value.
        * @param aIndex  Array index. Leaves if out of bounds.
        * @param aQuery  Mde query pointer to set.
        */
        inline void SetQuery( const TInt aIndex, CMdEQuery* aQuery );
        
        /**
        * Return a reference to the mde object array used by this array.
        * Useful as a parameter to Mde API.
        * @return RPointerArray reference to an array used by this array.
        */
        inline RPointerArray<CMdEObject>& MdeObjectArray();

    private:
    
        /** Private constructor */
        inline CObjectDataArray();
    
    private:

        /** Array of mde objects. */
        RPointerArray<CMdEObject> iMdeObjectArray;

        /** Array of location data objects. */
        RPointerArray<CLocationData> iLocationArray;

        /** Array of (prepared) mde query objects. */
        RPointerArray<CMdEQuery> iQueryArray;
    };


/**
* Class used to store harvested file information.
*/
class CFileData : public CBase
    {
    public:
        TBuf16<KMaxDataTypeLength> iMime16;
        TBuf8<KMaxDataTypeLength> iMime8;
        TInt64 iImageDataSize;
        TBool iJpeg;
        TTime iModified;
        TInt64 iFileSize;
        TInt iImageWidth;
        TInt iImageHeight;
        TInt iBitsPerPixel;
        TInt iFrameCount;
        TBool iExifSupported;
        TBool iDrmProtected;
        
        // no ownership to these
        CMdEObjectDef* iImageDef;
        TDesC* iUri;
        
        // ownership data
        HBufC8* iImageData;
    
    public:
        inline virtual ~CFileData();
        
        /** Standard NewL */
        inline static CFileData* NewL();

    private:
    
        /** Private constructor */
        inline CFileData();
    };

/**
* Class used to store harvested image data.
*/    
class CHarvestData : public CBase
    {
    public:
    
        TUint16 iWhiteBalance;
        TUint32 iImageWidthExif; 
        TUint32 iImageHeightExif; 
        TUint16 iFlash;
        TUint16 iExposureProgram;
        TUint16 iOrientation;
        TUint16 iYCbCrPositioning;
        TUint16 iResolutionUnit;
        TUint16 iIsoSpeedRating;
        TReal32 iExposureTime;
        TUint16 iColourSpace;
        TReal32 iAperture;
        TReal32 iExposureBias;
        TUint16 iMeteringMode;
        TUint32 iExifVersion;
        TUint32 iFlashPixVersion;
        TUint32 iThumbXResolution;
        TUint32 iThumbYResolution;
        TUint16 iThumbResolutionUnit;
        TUint16 iThumbCompression;
        TReal32 iShutterSpeed;
        TUint32 iComponentsConfiguration;
        
        TReal32 iXResolution;
        TReal32 iYResolution;
        
        TReal32 iFocalPlaneXResolution;
        TReal32 iFocalPlaneYResolution;
        TUint16 iFocalPlaneResolutionUnit;
        
        TReal32 iFNumber;
        TReal32 iFocalLength;
        TUint16 iFocalLengthIn35mm;
        
        // We must not try to store these unless they can be found in exif
        TBool iStoreWhiteBalance;
        TBool iStoreExposureProgram;
        TBool iStoreExposureBias;
        TBool iStoreOrientation;
        TBool iStoreYCbCrPositioning;
        TBool iStoreExifVersion;
        TBool iStoreFlashPixVersion;
        TBool iStoreShutterSpeed;
        TBool iStoreAperture;
        TBool iStoreColourSpace;
        TBool iStoreXResolution;
        TBool iStoreYResolution;
        TBool iStoreExposureTime;
        TBool iStoreThumbCompression;
        TBool iStoreThumbResolutionUnit;
        TBool iStoreFNumber;
        TBool iStoreFocalLength;
        TBool iStoreComponentsConfig;
        TBool iStoreSamplesPerPixel;
        TBool iStoreThumbXResolution;
        TBool iStoreThumbYResolution;
        TBool iStoreFocalLengthIn35;
        TBool iStoreIsoSpeedRating;
        TBool iStoreMeteringMode;
        TBool iStoreFlash;
        TBool iStoreFocalPlaneResolutionUnit;
        TBool iStoreFocalPlaneXResolution;
        TBool iStoreFocalPlaneYResolution;
        
        // bits per sample and samples per pixel not recorded in JPEG exif
        TUint16 iSamplesPerPixel;
        
        HBufC16* iDescription16;
        HBufC16* iComment16;
        HBufC16* iCopyright16;
        HBufC8* iDateModified8;
        HBufC8* iDateOriginal8;
        HBufC8* iDateDigitized8;
        HBufC16* iMake;
        HBufC16* iModel;
        HBufC16* iArtist;
        HBufC16* iRelatedSoundFile;
        
        // location
        TReal64 iGpsLatitude;
        TReal64 iGpsLongitude;
        TReal64 iGpsAltitude;

        TBool iStoreGpsLatitudeAndLongitude;
        TBool iStoreGpsAltitude;
    
    public:
        /** Virtual destructor. */
        inline virtual ~CHarvestData();

        /** Standard NewL */
        inline static CHarvestData* NewL();
        
    private:
    
        /** Private constructor */
        inline CHarvestData();
    };

#include "harvestdata.inl"

#endif
