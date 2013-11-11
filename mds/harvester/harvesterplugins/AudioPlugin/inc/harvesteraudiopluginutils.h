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


#ifndef __CHARVESTERAUDIOPLUGINUTILS_H__
#define __CHARVESTERAUDIOPLUGINUTILS_H__

#include <MetaDataUtility.h>
#include <MetaDataFieldContainer.h>

#include "mimetypemapping.h"

// Wma extension and mime type are shared with actual audio plugin
_LIT( KExtensionWma,    "wma" );
_LIT( KMimeTypeWma,    "audio/x-ms-wma");

enum TAudioMetadataHandling
	{
	EMetaDataUtilityHandling,
	EFileSystemHandling
	};

//
// CAudioMDParser
//
class CAudioMDParser : public CBase
	{
    public:
	    enum TAudioMetadataFields
	        {
	        EAudioMDFieldSong,
	        EAudioMDFieldArtist,
	        EAudioMDFieldAlbum,
	        EAudioMDFieldGenre,
	        EAudioMDFieldComposer,
	        EAudioMDFieldRating,
	        EAudioMDFieldTrack,
	        EAudioMDFieldOriginalArtist,
	        EAudioMDFieldDuration,
	        EAudioMDFieldCopyright,
	        EAudioMDFieldDate,
	        EAudioMDFieldJpeg,
	        EAudioMDFieldProtected,
	        EAudioMDFieldSampleRate,
	        EAudioMDFieldBitRate,
	        EAudioMDFieldAlbumArtist
	        };
	
	public:
		static CAudioMDParser* NewL( const TBool aAlbumArt );
		virtual ~CAudioMDParser();

        TBool ParseL( const TDesC& aFileName );
        TPtrC MetaDataFieldL( TInt aFieldId );
        TPtrC8 MetaDataField8L( TInt aFieldId );
        void ResetL();
        
        const TMimeTypeMapping<TAudioMetadataHandling>* ParseMimeType( const TDesC& aFileName );
		
	private:
        CAudioMDParser();
        void ConstructL( const TBool aAlbumArt );
		
        TMetaDataFieldId MapFieldId( TInt aFieldId );

    private:
        CMetaDataUtility* iMetaDataUtility;
        
        RArray<TMetaDataFieldId> iWantedMetadataFields;
        
        RArray< TMimeTypeMapping<TAudioMetadataHandling> > iMimeTypeMappings;
        
        TInt iLastParsedFileMimeType;
	};



#endif  // __CHARVESTERAUDIOPLUGINUTILS_H__

