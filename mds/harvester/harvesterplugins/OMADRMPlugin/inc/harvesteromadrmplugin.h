/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvests meta data from oma drm file.*
*/


#ifndef __CHARVESTEROMADRMPLUGIN_H__
#define __CHARVESTEROMADRMPLUGIN_H__

#include <mdeobject.h>
#include <e32base.h>
#include <apmstd.h>
#include <imageconversion.h>
#include <harvesterplugin.h>
#include <harvesterdata.h>


// FORWARD DECLARATION
class CFileData;
class CHarvestData;

/**
* A data transfer class for harvested drm metadata.
*/
class CDRMHarvestData : public CBase
    {
    public:

        /** NewL */
        static CDRMHarvestData* NewL()
            {
            CDRMHarvestData* self = new (ELeave) CDRMHarvestData;
            return self;
            }
        
        /** Destructor */
        virtual ~CDRMHarvestData()
            {
            }

    private:
        /** Constructor */
    		CDRMHarvestData() :  iFileSize( 0 ), iDrmProtected( EFalse )
            {
            // no implementation required
            }

    public:

    	TBuf<KMaxDataTypeLength> iMimetype;
        TBuf<KMaxDataTypeLength> iDescription;
        TBuf<KMaxDataTypeLength> iTitle;
        TBuf<KMaxDataTypeLength> iAuthor;
        TBuf<KMaxDataTypeLength> iGenre;
        TInt64 iFileSize;
        TTime iModified;
        TBool iDrmProtected;
    };

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester OMA DRM plug-in.
 */
class CHarvesterOmaDrmPluginPropertyDefs : public CBase
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
		CMdEPropertyDef* iWidthPropertyDef;
		CMdEPropertyDef* iHeightPropertyDef;
	
		// Image property definitions
		CMdEPropertyDef* iFrameCountPropertyDef;
		CMdEPropertyDef* iBitsPerSamplePropertyDef;
	private:
		CHarvesterOmaDrmPluginPropertyDefs();
	
		void ConstructL(CMdEObjectDef& aObjectDef);

	public:	
        static CHarvesterOmaDrmPluginPropertyDefs* NewL();
        void SetByObjectDefL( CMdEObjectDef& aObjectDef) ;
	};

class CHarvesterOMADRMPlugin : public CHarvesterPlugin
  	{
	public:
		/**
		* Constructs a new CHarvesterOMADRMPlugin implementation.
		*
		* @return A pointer to the new CHarvesterOMADRMPlugin implementation
		*/
		static CHarvesterOMADRMPlugin* NewL();
		
		/**
		* Destructor
		*/
		virtual ~CHarvesterOMADRMPlugin();
		
		/**
		* Harvests several files. Inherited from CHarvestPlugin.
		*
		* @param aHarvesterData  CHarvesterData datatype containing needed harvest data
 		* @param aClientData  TAny* to client specific data
		*/
		void HarvestL( CHarvesterData* aHD );
		
		/** */
		void GetObjectType( const TDesC& aUri, TDes& aObjectType );
		
    protected: // from CHarvesterPlugin
        
        void GetMimeType( const TDesC& aUri, TDes& aMimeType );
	
	private:
		/**
        * Gathers data from file to meta data object.
        *
        * @param aMetadataObject  A reference to meta data object to gather the data.
        * @param aDRMharvestData
        * @param aFileData
        * @param aHarvestData
        */
        TInt GatherDataL( CMdEObject& aMetadataObject, CDRMHarvestData& aDRMharvestData, 
			CFileData& aFileData, CHarvestData& aHarvestData );
	
		
		
			/**
			
		* C++ constructor - not exported;
		* implicitly called from NewL()
		*
		* @return an instance of CHarvesterOMADRMPlugin.
		*/

		/**
         * Handle addition of new mde video objects.
         *
         * @param aMetadataObject  A reference to meta data object to gather the data.
         * @param aDRMharvestData
         * @param aFileData
         * @param aHarvestData
         */
        void HandleObjectPropertiesL( CHarvestData& aHarvestData, CDRMHarvestData& aDRMharvestData, CFileData& aFileData, 
			CHarvesterData& aHarvesterData, TBool aIsAdd );
		
				// Default constructor
		CHarvesterOMADRMPlugin();
	/**
	* 2nd phase construction, called by NewLC()
		*/
		void ConstructL();
		
	    void InitPropDefsL( CMdEObjectDef& aObjectDef );
	      
  protected:
        /** 
         * Handle to File server session.
         */
        RFs iFs;
        
	private:
	     /**
         *  image decoder
         */        
        

		
      


		CHarvesterOmaDrmPluginPropertyDefs* iPropDefs;
		
		TInt iMaxTextLength;
		
        HBufC* iPhoneImagesPath;
        HBufC* iMmcImagesPath;
        
        HBufC* iPhoneVideosPath;
        HBufC* iMmcVideosPath;
        
        HBufC* iPhoneSoundsPath;
        HBufC* iMmcSoundsPath;

	};

#endif // __CHarvesterOMADRMPlugin_H__
