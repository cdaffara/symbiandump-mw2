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





// ---------------------------------------------------------------------------
// Constructor for CLocationData.
// ---------------------------------------------------------------------------
//
CLocationData::CLocationData( TReal64 aGpsLatitude, TReal64 aGpsLongitude,
    TReal64 aGpsAltitude, TBool aGpsLaLExists, TBool aGpsAExists )
    : iGpsLatitude(aGpsLatitude), iGpsLongitude(aGpsLongitude), iGpsAltitude(aGpsAltitude)
    , iGpsLatAndLongExists(aGpsLaLExists), iGpsAltitudeExists(aGpsAExists)
    {
    // initializers are enough
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CLocationData* CLocationData::NewL( TReal64 aGpsLatitude, TReal64 aGpsLongitude,
    TReal64 aGpsAltitude, TBool aGpsLaLExists, TBool aGpsAExists )
    {
    CLocationData* self = new (ELeave) CLocationData( aGpsLatitude,
        aGpsLongitude, aGpsAltitude, aGpsLaLExists, aGpsAExists );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CObjectDataArray::CObjectDataArray()
    {
    // no implementation required
    }

// ---------------------------------------------------------------------------
// Destructor for CObjectDataArray.
// ---------------------------------------------------------------------------
//
CObjectDataArray::~CObjectDataArray()
    {
    ResetAndDestroy();
    Close();
    }

// CObjectDataArray

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CObjectDataArray* CObjectDataArray::NewL()
    {
    CObjectDataArray* self = new (ELeave) CObjectDataArray();
    return self;
    }

// ---------------------------------------------------------------------------
// Appends a new entry to a CObjectDataArray.
// ---------------------------------------------------------------------------
//
void CObjectDataArray::Append( CMdEObject* aMdeObject, CLocationData* aLocationData, CMdEQuery* aQuery )
    {
    TInt mdeObjectIndex(KErrNone);
    TInt locationDataIndex(KErrNone);
    
    mdeObjectIndex = iMdeObjectArray.Append( aMdeObject );
    
    if( mdeObjectIndex != KErrNone )
        {
        delete aMdeObject;
        aMdeObject = NULL;
        
        delete aLocationData;
        aLocationData = NULL;
        
        delete aQuery;
        aQuery = NULL;
        }
    else
        {
        locationDataIndex = iLocationArray.Append( aLocationData );
        
        if( locationDataIndex != KErrNone )
            {
            iMdeObjectArray.Remove( mdeObjectIndex );
            delete aMdeObject;
            aMdeObject = NULL;
            
            delete aLocationData;
            aLocationData = NULL;
            
            delete aQuery;
            aQuery = NULL;
            }
        else
            {
            if( iQueryArray.Append( aQuery ) != KErrNone )
                {
                iMdeObjectArray.Remove( mdeObjectIndex );
                delete aMdeObject;
                aMdeObject = NULL;
                
                iLocationArray.Remove( locationDataIndex );
                delete aLocationData;
                aLocationData = NULL;
                
                delete aQuery;
                aQuery = NULL;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Removes an entry from the array.
// ---------------------------------------------------------------------------
//
void CObjectDataArray::Remove( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iMdeObjectArray.Count() )
        {
        return;
        }
    delete iMdeObjectArray[aIndex];
    iMdeObjectArray[aIndex] = NULL;
    
    delete iLocationArray[aIndex];
    iLocationArray[aIndex] = NULL;
    
    delete iQueryArray[aIndex];
    iQueryArray[aIndex] = NULL;
    
    iMdeObjectArray.Remove( aIndex );
    iLocationArray.Remove( aIndex );
    iQueryArray.Remove( aIndex );
    
    if( iMdeObjectArray.Count() == 0 )
        {
        iMdeObjectArray.Compress();
        }

    if( iLocationArray.Count() == 0 )
        {
        iLocationArray.Compress();
        }

    if( iQueryArray.Count() == 0 )
        {
        iQueryArray.Compress();
        }    
    }

// ---------------------------------------------------------------------------
// Returns the count of objects in the array.
// ---------------------------------------------------------------------------
//
TInt CObjectDataArray::Count()
    {
    return iMdeObjectArray.Count();
    }

// ---------------------------------------------------------------------------
// ResetAndDestroy()
// ---------------------------------------------------------------------------
//
void CObjectDataArray::ResetAndDestroy()
    {
    iMdeObjectArray.ResetAndDestroy();
    iLocationArray.ResetAndDestroy();
    iQueryArray.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// Close()
// ---------------------------------------------------------------------------
//
void CObjectDataArray::Close()
    {
    iMdeObjectArray.Close();
    iLocationArray.Close();
    iQueryArray.Close();
    }

// ---------------------------------------------------------------------------
// ObjectL()
// ---------------------------------------------------------------------------
//
CMdEObject* CObjectDataArray::ObjectL( const TInt aIndex ) 
    {
    if ( aIndex < 0 || aIndex >= iMdeObjectArray.Count() )
        {
        User::Leave( KErrArgument );
        }
    return iMdeObjectArray[aIndex];
    }

// ---------------------------------------------------------------------------
// LocationDataL
// ---------------------------------------------------------------------------
//
CLocationData* CObjectDataArray::LocationDataL( const TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iLocationArray.Count() )
        {
        User::Leave( KErrArgument );
        }
    return iLocationArray[aIndex];
    }

// ---------------------------------------------------------------------------
// QueryL
// ---------------------------------------------------------------------------
//
CMdEQuery* CObjectDataArray::QueryL( const TInt aIndex )
    {
    if ( aIndex < 0 || aIndex >= iQueryArray.Count() )
        {
        User::Leave( KErrArgument );
        }
    return iQueryArray[aIndex];
    }

// ---------------------------------------------------------------------------
// SetQuery
// ---------------------------------------------------------------------------
//
void CObjectDataArray::SetQuery( const TInt aIndex, CMdEQuery* aQuery )
    {
    if ( aIndex < 0 || aIndex >= iQueryArray.Count() )
        {
        return;
        }
    iQueryArray[aIndex] = aQuery;
    }

// ---------------------------------------------------------------------------
// MdeObjectArray
// ---------------------------------------------------------------------------
//
RPointerArray<CMdEObject>& CObjectDataArray::MdeObjectArray()
    {
    return iMdeObjectArray;
    }


// CFileData

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CFileData* CFileData::NewL()
    {
    CFileData* self = new (ELeave) CFileData;
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CFileData::CFileData()
    {
    // no implementation needed
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CFileData::~CFileData()
    {
    if ( iImageData )
        {
        delete iImageData;
        iImageData = NULL;
        }
    iImageDef = NULL;
    iUri = NULL;
    }

    
// CHarvestData

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvestData* CHarvestData::NewL()
    {
    CHarvestData* self = new (ELeave) CHarvestData;
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CHarvestData::CHarvestData()
    {
    // no implementation needed
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CHarvestData::~CHarvestData()
    {
    if ( iDescription16 )
        {
        delete iDescription16;
        iDescription16 = NULL;
        }
        
    if ( iComment16 )
        {
        delete iComment16;
        iComment16 = NULL;
        }
        
    if ( iCopyright16 )
        {
        delete iCopyright16;
        iCopyright16 = NULL;
        }
        
    if ( iDateModified8 )
        {
        delete iDateModified8;
        iDateModified8 = NULL;
        }
        
    if ( iDateOriginal8 )
        {
        delete iDateOriginal8;
        iDateOriginal8 = NULL;
        }
        
    if ( iDateDigitized8 )
        {
        delete iDateDigitized8;
        iDateDigitized8 = NULL;
        }
        
    if ( iMake )
        {
        delete iMake;
        iMake = NULL;
        }
        
    if ( iModel )
        {
        delete iModel;
        iModel = NULL;
        }
        
    if ( iArtist )
        {
        delete iArtist;
        iArtist = NULL;
        }
        
    if ( iRelatedSoundFile )
        {
        delete iRelatedSoundFile;
        iRelatedSoundFile = NULL;
        }
    }

