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
* Description:  Harvests metadata for wmv video file
*
*/

#ifndef C_HARVESTERWMVPLUGIN_H
#define C_HARVESTERWMVPLUGIN_H

#include <e32base.h>
#include <apmstd.h>

// forward declaration
class CMdEPropertyDef;
class CMdEObjectDef;

const TInt KMaxTitleLength( 255 ); //to fit in db
const TInt KMaxMimeTypeLength( KMaxDataTypeLength );

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester OMA DRM plug-in.
 */
class CHarvesterWmvPluginPropertyDefs : public CBase
	{
	public:
		// Common property definitions
		CMdEPropertyDef* iCreationDatePropertyDef;
		CMdEPropertyDef* iLastModifiedDatePropertyDef;
		CMdEPropertyDef* iSizePropertyDef;
		CMdEPropertyDef* iItemTypePropertyDef;
        CMdEPropertyDef* iTitlePropertyDef;
		
        // Media property definitions
        CMdEPropertyDef* iDrmPropertyDef;
        CMdEPropertyDef* iDescriptionPropertyDef;
        CMdEPropertyDef* iAuthorPropertyDef;
        CMdEPropertyDef* iGenrePropertyDef;
        CMdEPropertyDef* iDefaultFolderPropertyDef;
		
	private:
		CHarvesterWmvPluginPropertyDefs();
	
		void ConstructL(CMdEObjectDef& aObjectDef);

	public:	
	    static CHarvesterWmvPluginPropertyDefs* NewL();
	    void SetByObjectDefL( CMdEObjectDef& aObjectDef );
	};

/**
 *  Data class for wmv harvester
 *
 *  Includes clip metadata to save to db.
 *
 */
class CHarvesterWmvClipDetails : public CBase
    {
public:

    /** NewL */
    static CHarvesterWmvClipDetails* NewL()
        {
        CHarvesterWmvClipDetails* self = new (ELeave) CHarvesterWmvClipDetails;
        return self;
        }
    
    /** Destructor */
    virtual ~CHarvesterWmvClipDetails()
        {
        }
    
public:

    /**
    * Mime type
    */
    TBuf<KMaxMimeTypeLength> iMimeType;

    /**
    * Modification date
    */
    TTime iModifiedDate;

    /**
    * File size
    */
    TUint32 iFileSize;

    TBuf<KMaxDataTypeLength> iDescription;
    TBuf<KMaxDataTypeLength> iTitle;
    TBuf<KMaxDataTypeLength> iAuthor;
    TBuf<KMaxDataTypeLength> iGenre;
    TBool iDrmProtected;  
    
private:

    /**
     * Default constructor
     */
    CHarvesterWmvClipDetails() : iFileSize( 0 )
    {
    }
        
    };

#include <harvesterplugin.h>
#include "harvesterdata.h"

/**
 *  This class implements WMV harvester plugin.
 *
 *  @lib harvesterwmvplugin.dll
 *  @since S60 S60 v3.2
 */
class CHarvesterWMVPlugin : public CHarvesterPlugin
    {
public:

    /**
     * Constructs a new CHarvesterWMVPlugin implementation.
     * 
     * @return A pointer to the new CHarvesterWMVPlugin implementation
     */
    static CHarvesterWMVPlugin* NewL();
 
    /**
    * Destructor.
    */
    virtual ~CHarvesterWMVPlugin();

// from base class CHarvesterPlugin
    
    /**
    * Harvest file. 
    * 
    * @param aHarvesterData  
    */
    void HarvestL( CHarvesterData* aHarvesterData );

protected: // from CHarvesterPlugin
    
    void GetMimeType( const TDesC& aUri, TDes& aMimeType );
    
private:

    /**
    * C++ constructor
    */
    CHarvesterWMVPlugin();

    /**
    * 2nd phase constructor
    */
    void ConstructL();
    
    /**
     * Gathers data from file to meta data object. 
     * Leaves if mime type is not supported
     *
     * @param aMetadataObject  Reference to metadata object to gather the data. 
     */
    void GatherDataL( CMdEObject& aMetadataObject, CHarvesterWmvClipDetails& aClipDetails );
     
     /**
      * Handle object properties
      */
    void HandleObjectPropertiesL( 
         CHarvesterData& aHD,
         CHarvesterWmvClipDetails& aClipDetails,
         TBool aIsAdd);

    void InitPropDefsL( CMdEObjectDef& aObjectDef );

private: // data
	CHarvesterWmvPluginPropertyDefs* iPropDefs;
	
	TInt iMaxTextLength;
	
    HBufC* iPhoneVideosPath;
    HBufC* iMmcVideosPath;
    };

#endif // C_HARVESTERWMVPLUGIN_H
