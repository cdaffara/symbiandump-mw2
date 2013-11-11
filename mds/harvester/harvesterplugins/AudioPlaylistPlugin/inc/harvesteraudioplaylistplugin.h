/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __CHARVESTERAUDIOPLAYLISTPLUGIN_H__
#define __CHARVESTERAUDIOPLAYLISTPLUGIN_H__

#include <mdeobject.h>
#include <fbs.h>
#include <imageconversion.h>
#include <harvesterplugin.h>
#include <monitorplugin.h>
#include <mdesession.h>
#include "harvesteraudioplaylistparser.h"

class CHarvestData;

/**
 * Helper class to hold all property definitions 
 * (pointers are not owned) used in harvester audio plug-in.
 */
class CHarvesterAudioPlaylistPluginPropertyDefs : public CBase
    {
    public:
        // Common property definitions
        CMdEPropertyDef* iCreationDatePropertyDef;
        CMdEPropertyDef* iLastModifiedDatePropertyDef;
        CMdEPropertyDef* iSizePropertyDef;
        CMdEPropertyDef* iItemTypePropertyDef;
        CMdEPropertyDef* iTitlePropertyDef;
        
        CMdEPropertyDef* iAlbumTypeDef;
        
        CMdEPropertyDef* iAlbumIDDef;
        CMdEPropertyDef* iAudioObjectIDDef;
        CMdEPropertyDef* iPositionDef;

    private:
        CHarvesterAudioPlaylistPluginPropertyDefs();
    
        void ConstructL(CMdEObjectDef& aObjectDef);

    public: 
        static CHarvesterAudioPlaylistPluginPropertyDefs* NewL(CMdEObjectDef& aObjectDef);
    };


class CHarvesterAudioPlaylistPlugin : public CHarvesterPlugin
	{
	public:
		/**
		* Construction
		* @return Harvester audio plugin
		*/
		static CHarvesterAudioPlaylistPlugin* NewL();

		/**
		* Destruction
		*/
		virtual ~CHarvesterAudioPlaylistPlugin();

    private:
		// Default constructor
		CHarvesterAudioPlaylistPlugin();
		
		// 2nd phase constructor
		void ConstructL();
    
    
    public: // from CHarvesterPlugin
    
		/**
		* Harvesting multiple files
		* @param aHarvesterData  CHarvesterData datatype containing needed harvest data
 		* @return None
		*/
		void HarvestL( CHarvesterData* aHD );

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
        TBool GetMimeTypePropertyL( CHarvesterData* aHD, TBool aIsAdd );

		/**
		 * Get song name, artist, album, genre and composer from normal music
         * file (eg. mp3).
		 */
        void GetMusicPropertiesL( CHarvesterData* aHD, TBool aIsAdd );

	private:
		CHarvesterAudioPlaylistParser* iPlaylistParser;
		HBufC* iMime;
		
	    CHarvesterAudioPlaylistPluginPropertyDefs* iPropDefs;

        TInt iError;
	};


#endif  // __CHARVESTERAUDIOPLAYLISTPLUGIN_H__

