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


#ifndef HARVESTERAUDIOPLUGIN_H
#define HARVESTERAUDIOPLUGIN_H

#include <harvesterplugin.h>
#include <thumbnailmanager.h>
#include <thumbnailmanagerobserver.h>

#include "harvesteraudiopluginutils.h"
#include "mdeobjectwrapper.h"

// FORWARD DECLARATION
class CMdEObjectDef;
class CHarvestData;
class MThumbnailData;

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester audio plug-in.
 */
class CHarvesterAudioPluginPropertyDefs : public CBase
	{
	public:
		// Common property definitions
		CMdEPropertyDef* iCreationDatePropertyDef;
		CMdEPropertyDef* iLastModifiedDatePropertyDef;
		CMdEPropertyDef* iSizePropertyDef;
		CMdEPropertyDef* iItemTypePropertyDef;
		CMdEPropertyDef* iTitlePropertyDef;
		CMdEPropertyDef* iTimeOffsetPropertyDef;

		// Media property definitions	
		CMdEPropertyDef* iRatingPropertyDef;
		CMdEPropertyDef* iGenrePropertyDef;
		CMdEPropertyDef* iArtistPropertyDef;
		CMdEPropertyDef* iDurationPropertyDef;
		CMdEPropertyDef* iCopyrightPropertyDef;
		CMdEPropertyDef* iThumbnailPropertyDef;
		CMdEPropertyDef* iDatePropertyDef;
		CMdEPropertyDef* iDrmPropertyDef;
		CMdEPropertyDef* iDefaultFolderPropertyDef;
		CMdEPropertyDef* iTrackPropertyDef;
		CMdEPropertyDef* iBitratePropertyDef;

		// Audio property definitions
		CMdEPropertyDef* iAlbumPropertyDef;
		CMdEPropertyDef* iComposerPropertyDef;		
		CMdEPropertyDef* iOriginalArtistPropertyDef;
		CMdEPropertyDef* iSampleRatePropertyDef;
		CMdEPropertyDef* iAlbumArtistPropertyDef;
		

	private:
		CHarvesterAudioPluginPropertyDefs();
	
		void ConstructL( CMdEObjectDef& aObjectDef );

	public:	
		static CHarvesterAudioPluginPropertyDefs* NewL();
		void SetByObjectDefL( CMdEObjectDef& aObjectDef );
	};

class CHarvesterAudioPlugin : public CHarvesterPlugin,
                                            public MThumbnailManagerObserver
	{
	public:
		/**
		* Construction
		* @return Harvester audio plugin
		*/
		static CHarvesterAudioPlugin* NewL();

		/**
		* Destruction
		*/
		virtual ~CHarvesterAudioPlugin();

    private:
		// Default constructor
		CHarvesterAudioPlugin();
		
		// 2nd phase constructor
		void ConstructL();
    
    
    
    public: // from CHarvesterPlugin
    
		/**
		* Harvesting multiple files
		* @param aHarvesterData  CHarvesterData datatype containing needed harvest data
 		* @return None
		*/
		void HarvestL( CHarvesterData* aHD );

    public: // from Thumbnail Observer
		
	    void ThumbnailPreviewReady( MThumbnailData& aThumbnail,
	        TThumbnailRequestId aId );
	    
	    void ThumbnailReady( TInt aError, MThumbnailData& aThumbnail,
	        TThumbnailRequestId aId );

    protected: // from CHarvesterPlugin
	    
	    void GetMimeType( const TDesC& aUri, TDes& aMimeType );
	    
	private:
	     
		/**
		* Harvesting multiple files
		* @param aHarvesterData  CHarvesterData datatype containing needed harvest data
 		* 
		*/
		void DoHarvestL( CHarvesterData* aHD );

		/**
		 * Handles creation of new mde objects.
		 */
		void GetPropertiesL( CHarvesterData* aHarvesterData, TBool aIsAdd );
		 		
		/**
		 * Get placeholder properties (creation time, modify time and file size).
		 */
        void GetPlaceHolderPropertiesL( CHarvesterData* aHD, TBool aIsAdd );
		
		/**
		 * Get media file mime type (eg. "audio/mp3").
		 */
        const TMimeTypeMapping<TAudioMetadataHandling>* GetMimeTypePropertyL( 
        		CHarvesterData* aHD, TBool aIsAdd );

		/**
		 * Get song name, artist, album, genre and composer from normal music
         * file (eg. mp3).
		 */
        void GetMusicPropertiesL( CHarvesterData* aHD, TBool aIsAdd, TPtrC aMimeType );

        void InitPropDefsL( CMdEObjectDef& aObjectDef );

	private:
		CAudioMDParser* iAudioParser;
		
		CHarvesterAudioPluginPropertyDefs* iPropDefs;
		
		CThumbnailManager* iTNM;
		TBool iHarvestAlbumArt;
		
		TInt iMaxTextLength;
		
		HBufC* iPhoneSoundsPath;
		HBufC* iMmcSoundsPath;
	};


#endif  // HARVESTERAUDIOPLUGIN_H

