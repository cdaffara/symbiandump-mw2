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
* Description:
*
*/


#ifndef __CHARVESTERIMAGEPLUGIN_H__
#define __CHARVESTERIMAGEPLUGIN_H__

#include <mdeobject.h>
#include <fbs.h>
#include <imageconversion.h>
#include <harvesterplugin.h>
#include <monitorplugin.h>

#include "mimetypemapping.h"

// FORWARD DECLARATION
class CFileData;
class CHarvestData;
class CHarvesterExifUtil;

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester image plug-in.
 */
class CHarvesterImagePluginPropertyDefs : public CBase
	{
	public:
		// Common property definitions
		CMdEPropertyDef* iCreationDatePropertyDef;
		CMdEPropertyDef* iLastModifiedDatePropertyDef;
		CMdEPropertyDef* iSizePropertyDef;
		CMdEPropertyDef* iTimeOffsetPropertyDef;
		CMdEPropertyDef* iItemTypePropertyDef;
	
		// Media property definitions
		CMdEPropertyDef* iWidthPropertyDef;
		CMdEPropertyDef* iHeightPropertyDef;
		CMdEPropertyDef* iDescriptionPropertyDef;
		CMdEPropertyDef* iCommentPropertyDef;
		CMdEPropertyDef* iReleaseDatePropertyDef;
		CMdEPropertyDef* iCopyrightPropertyDef;
		CMdEPropertyDef* iCaptureDatePropertyDef;
		CMdEPropertyDef* iResolutionUnitPropertyDef;
		CMdEPropertyDef* iArtistPropertyDef;
		CMdEPropertyDef* iDefaultFolderPropertyDef;
		
		// Image property definitions
		CMdEPropertyDef* iPixelYDimensionPropertyDef;
		CMdEPropertyDef* iPixelXDimensionPropertyDef;
		CMdEPropertyDef* iBitsPerSamplePropertyDef;
		CMdEPropertyDef* iFrameCountPropertyDef;
		CMdEPropertyDef* iDateTimeOriginalPropertyDef;
		CMdEPropertyDef* iDateTimeDigitizedPropertyDef;
		CMdEPropertyDef* iDateTimePropertyDef;
		CMdEPropertyDef* iWhiteBalancePropertyDef;
		CMdEPropertyDef* iFlashPropertyDef;
		CMdEPropertyDef* iExposureProgramPropertyDef;
		CMdEPropertyDef* iMakePropertyDef;
		CMdEPropertyDef* iModelPropertyDef;
		CMdEPropertyDef* iOrientationPropertyDef;
		CMdEPropertyDef* iXResolutionPropertyDef;
		CMdEPropertyDef* iYResolutionPropertyDef;
		CMdEPropertyDef* iYCbCrPositioningPropertyDef;
		CMdEPropertyDef* iExposureTimePropertyDef;
		CMdEPropertyDef* iFNumberPropertyDef;
		CMdEPropertyDef* iExifVersionPropertyDef;
		CMdEPropertyDef* iShutterSpeedValuePropertyDef;
		CMdEPropertyDef* iApertureValuePropertyDef;
		CMdEPropertyDef* iFocalLengthPropertyDef;
		CMdEPropertyDef* iFlashPixVersionPropertyDef;
		CMdEPropertyDef* iColourSpacePropertyDef;
		CMdEPropertyDef* iISOSpeedRatingsPropertyDef;
		CMdEPropertyDef* iComponentsConfigurationPropertyDef;
		CMdEPropertyDef* iExposureBiasValuePropertyDef;
		CMdEPropertyDef* iSamplesPerPixelPropertyDef;
		CMdEPropertyDef* iThumbCompressionPropertyDef;
		CMdEPropertyDef* iThumbXResolutionPropertyDef;
		CMdEPropertyDef* iThumbYResolutionPropertyDef;
		CMdEPropertyDef* iThumbResolutionUnitPropertyDef;
		CMdEPropertyDef* iFocalLengthIn35mmFilmPropertyDef;
		CMdEPropertyDef* iMeteringModePropertyDef;
		CMdEPropertyDef* iRelatedSoundFilePropertyDef;
		CMdEPropertyDef* iFocalPlaneResolutionUnitPropertyDef;
		CMdEPropertyDef* iFocalPlaneXResolutionPropertyDef;
		CMdEPropertyDef* iFocalPlaneYResolutionPropertyDef;
        CMdEPropertyDef* iDraftPropertyDef;

	private:
		CHarvesterImagePluginPropertyDefs();
	
		void ConstructL( CMdEObjectDef& aObjectDef );

	public:	
		static CHarvesterImagePluginPropertyDefs* NewL();
		void SetByObjectDefL( CMdEObjectDef& aObjectDef) ;
	};

class CHarvesterImagePlugin : public CHarvesterPlugin
	{
	public:

		/**
		* Construction
		* @return Harvester image plugin
		*/
		static CHarvesterImagePlugin* NewL();

		/**
		* Destruction
		*/
		virtual ~CHarvesterImagePlugin();

		/**
		* Harvesting multiple files
		* @param aHarvesterData  CHarvesterData datatype containing needed harvest data
 		* @param aClientData  TAny* to client specific data
		*/
		void HarvestL( CHarvesterData* aHD );

	protected:
		
	      void GetMimeType( const TDesC& aUri, TDes& aMimeType );
		
	private:
	    /**
	     * The method that actually extracts the metadata.
	     * @param aMetadataObject  Metadata object which is filled with metadata
	     * @param aFileData  File data to be filled
	     * @param aHarvestData  Harvested data to be filled
	     * @return Error code which indicates status of the operation
	     */                
		TInt GatherDataL( CMdEObject& aMetadataObject, CFileData& aFileData,
				CHarvestData& aHarvestData );

		/**
		 * Checks if the MIME type of current file is supported.
		 * @param aMimeBuf mime string
		 * @return KErrNone if MIME type is supported, KErrNotSupported if not
		 */
		TInt CheckIfMimeSupported( const TDesC& aMimeBuf );
		 
		/**
		 * Reads file data from image file.
		 */
		void DataFromImageFileL( CFileData& aFileData );
		
		/**
		 * Handles creation of new mde objects.
		 */
		void HandleObjectPropertiesL( CHarvestData& aHd, CFileData& aFileData,
		 		CHarvesterData& aHarvesterData, TBool aIsAdd );

		// Default constructor
		CHarvesterImagePlugin();
		
		// 2nd phase constructor
		void ConstructL();
		
		void InitPropDefsL( CMdEObjectDef& aObjectDef );

	private:
	
		enum TImageMetadataHandling
			{
			EJpegHandling,
			EGifHandling,
			EOtherHandling
			};
		
		/** */
        CHarvesterExifUtil* iExifUtil;
        
        /**
         *  Buffered image decoder (so that we don't have to create decoders all the time)
         */        
        CBufferedImageDecoder *iDecoder;
         
        /**
         *  Session to Font and Bitmap server. For some reason WMF-harvesting needs this
         */
        RFbsSession iFbs;
        
        
        /**
         *  Mime type mapper for extension/mimetype pairs
         */
        RArray< TMimeTypeMapping<TImageMetadataHandling> > iMimeTypeMappings;
        
        CHarvesterImagePluginPropertyDefs* iPropDefs;
        
        TInt iMaxTextLength;
        
        HBufC* iPhoneImagesPath;
        HBufC* iMmcImagesPath;
	};

#endif

