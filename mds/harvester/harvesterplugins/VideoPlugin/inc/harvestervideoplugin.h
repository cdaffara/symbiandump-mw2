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
* Description:  Harvests meta data from video file.*
*/


#ifndef __CHARVESTERVIDEOPLUGIN_H__
#define __CHARVESTERVIDEOPLUGIN_H__

#include <e32base.h>
#include "harvesterplugin.h"

#include "mimetypemapping.h"
#include "harvestermediaidutil.h"

// FORWARD DECLARATION
class CMdEObjectDef;
class CMdEPropertyDef;
class CMdEObject;
class CHarvesterData;

class TVideoMetadataHandling
	{
	public:
		enum TVideoMetadataHandlingLibrary
			{
			EMp4LibHandling,
			EHexilMetadataHandling
			};

		TVideoMetadataHandlingLibrary iLibrary;
		TPtrC iObjectDef;
		TPtrC iVideoMime;
		TPtrC iAudioMime;

		TVideoMetadataHandling()
			{		
			}
		
		TVideoMetadataHandling(TVideoMetadataHandlingLibrary aLibrary, 
				TPtrC aObjectDef, TPtrC aVideoMime, TPtrC aAudioMime) :
			iLibrary( aLibrary ), iObjectDef( aObjectDef ),
			iVideoMime( aVideoMime ), iAudioMime( aAudioMime ) 
			{		
			}
		
		TVideoMetadataHandling(const TVideoMetadataHandling& aHandling) :
			iLibrary( aHandling.iLibrary ), iObjectDef( aHandling.iObjectDef ),
			iVideoMime( aHandling.iVideoMime ), iAudioMime( aHandling.iAudioMime )
			{
			}
	};

typedef TMimeTypeMapping<TVideoMetadataHandling> THarvestingHandling;
	
/**
* A data transfer class for harvested video metadata.
*/
class CVideoHarvestData : public CBase
    {
    public:
    	TBool iVideoObject; // Is object video or audio

    	TTime iModified;
    	TInt64 iFileSize; // in bytes
        TReal32 iDuration; // in seconds
        TReal32 iFrameRate; // in frames per second
        TReal32 iSamplingFrequency; // in kHz 
        TInt iVideoBitrate; // in kbps
        TInt iAudioBitrate; // in kbps
        TInt iClipBitrate; // in kbps
        TInt16 iFrameWidth;
        TInt16 iFrameHeight;
        HBufC* iCopyright;
        HBufC* iAuthor;
        HBufC* iGenre;
        HBufC* iPerformer; // Artist
        HBufC* iDescription;
        HBufC* iTitle;

        HBufC* iMimeBuf;
        TUint32 iCodec;
        
        TBool iDrmProtected;

        CVideoHarvestData() : CBase()
        	{
        	}

        ~CVideoHarvestData()
        	{
        	delete iCopyright;
        	iCopyright = NULL;
            delete iAuthor;
            iAuthor = NULL;
            delete iGenre;
            iGenre = NULL;
            delete iPerformer;
            iPerformer = NULL;
            delete iDescription;
            iDescription = NULL;
            delete iTitle;
            iTitle = NULL;

        	delete iMimeBuf;
        	iMimeBuf = NULL;
        	}
    };

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester video plug-in.
 */
class CHarvesterVideoPluginPropertyDefs : public CBase
	{
	public:
		// Common property definitions
		CMdEPropertyDef* iCreationDatePropertyDef;
		CMdEPropertyDef* iLastModifiedDatePropertyDef;
		CMdEPropertyDef* iSizePropertyDef;
		CMdEPropertyDef* iTimeOffsetPropertyDef;
		CMdEPropertyDef* iItemTypePropertyDef;
		CMdEPropertyDef* iTitlePropertyDef;
	
		// Media property definitions
		CMdEPropertyDef* iReleaseDatePropertyDef;
		CMdEPropertyDef* iCaptureDatePropertyDef;
		CMdEPropertyDef* iDurationPropertyDef;
		CMdEPropertyDef* iWidthPropertyDef;
		CMdEPropertyDef* iHeightPropertyDef;
		CMdEPropertyDef* iBitratePropertyDef;
		CMdEPropertyDef* iCopyrightPropertyDef;
		CMdEPropertyDef* iAuthorPropertyDef;
		CMdEPropertyDef* iGenrePropertyDef;
		CMdEPropertyDef* iArtistPropertyDef;
		CMdEPropertyDef* iDescriptionPropertyDef;
		CMdEPropertyDef* iDrmPropertyDef;
		CMdEPropertyDef* iDefaultFolderPropertyDef;
		
		CMdEPropertyDef* iAudioFourCCDef;
	
		// Video property definitions
		CMdEPropertyDef* iFrameratePropertyDef;
	
		// Audio property definitions
		CMdEPropertyDef* iSamplingFrequencyPropertyDef;
	
	private:
		CHarvesterVideoPluginPropertyDefs();
	
		void ConstructL(CMdEObjectDef& aObjectDef);

	public:	
		static CHarvesterVideoPluginPropertyDefs* NewL();
		void SetByObjectDefL( CMdEObjectDef& aObjectDef );
	};

class CHarvesterVideoPlugin : public CHarvesterPlugin
  	{
	public:
		/**
		* Constructs a new CHarvesterVideoPlugin implementation.
		*
		* @return A pointer to the new CHarvesterVideoPlugin implementation
		*/
		static CHarvesterVideoPlugin* NewL();
		
		/**
		* Destructor
		*/
		virtual ~CHarvesterVideoPlugin();
		
		void GetObjectType( const TDesC& aUri, TDes& aObjectType );

		/**
		* Harvests several files. Inherited from CHarvestPlugin.
		*
		* @param aHarvesterData  CHarvesterData datatype containing needed harvest data
 		* @param aClientData  TAny* to client specific data
		*/
		void HarvestL( CHarvesterData* aHD );
				
    protected: // from CHarvesterPlugin
        
        void GetMimeType( const TDesC& aUri, TDes& aMimeType );
		
	private:
		/**
		* C++ constructor - not exported;
		* implicitly called from NewL()
		*
		* @return an instance of CHarvesterVideoPlugin.
		*/
		CHarvesterVideoPlugin();
		
		/**
		* 2nd phase construction, called by NewLC()
		*/
		void ConstructL();
		
		/**
        * Gathers data from file to meta data object.
        *
        * @param aMetadataObject  A reference to meta data object to gather the data.
        * @param aHarvestData  An object to store harvested video file data.
        */
        void GatherDataL( CMdEObject& aMetadataObject, CVideoHarvestData& aHarvestData );
		
        /**
         * Handle addition of new mde video objects.
         *
         * @param aMetadataObject  A reference to meta data object to gather the data.
         * @param aHarvestData  An object containing harvested video file data.
         */
        void HandleObjectPropertiesL( CHarvesterData& aHD, CVideoHarvestData& aVHD, TBool aIsAdd );
          
	private:
		void GetMp4Type( RFile64& aFile, TDes& aType );
		
		void GetRmTypeL( RFile64& aFile, TDes& aType );

        TInt AddFileToBlackList( const TFileName& aFullName, const TUint32& aMediaId );

        TInt RemoveFileFromBlackList( const TFileName& aFullName, const TUint32& aMediaId );

        TInt GetFileFullNameAndMediaId( const RFile64& aFile, TFileName& aFullName, TUint32& mediaId);
	
		const THarvestingHandling* FindHandler( const TDesC& aUri );
		
		void CheckForCodecSupport( HBufC* aMimeBuffer, CVideoHarvestData& aVHD );
		
        void InitPropDefsL( CMdEObjectDef& aObjectDef );

	private:
		RArray<THarvestingHandling> iMimeTypeMappings;
		
		CHarvesterVideoPluginPropertyDefs* iPropDefs;
		
		TInt iMaxTextLength;
		
		// not own
		CHarvesterMediaIdUtil* iMediaIdUtil;
		
	    HBufC* iPhoneVideosPath;
	    HBufC* iMmcVideosPath;
	};

#endif // __CHARVESTERVIDEOPLUGIN_H__
