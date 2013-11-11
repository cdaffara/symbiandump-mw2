/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for music media type
*
*/



#ifndef CMPXMEDIAMUSICDEFS_H
#define CMPXMEDIAMUSICDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying music category of content provided
*  in the media object and associated attributes.
*/

const TInt KMPXMediaIdMusic=0x101FFC42;
/**
*  Attributes supported for KMPXMediaIdMusic
*/
const TMPXAttributeData KMPXMediaMusicAlbumArtFileName={KMPXMediaIdMusic,0x01}; // Text
const TMPXAttributeData KMPXMediaMusicArtist={KMPXMediaIdMusic,0x02}; // Text
const TMPXAttributeData KMPXMediaMusicAlbum={KMPXMediaIdMusic,0x04}; // Text
const TMPXAttributeData KMPXMediaMusicYear={KMPXMediaIdMusic,0x08}; // TInt64
const TMPXAttributeData KMPXMediaMusicAlbumTrack={KMPXMediaIdMusic,0x10}; // TInt
const TMPXAttributeData KMPXMediaMusicGenre={KMPXMediaIdMusic,0x20}; // Text
const TMPXAttributeData KMPXMediaMusicComposer={KMPXMediaIdMusic,0x40}; // Text
const TMPXAttributeData KMPXMediaMusicRating={KMPXMediaIdMusic,0x80}; // TInt
const TMPXAttributeData KMPXMediaMusicURL={KMPXMediaIdMusic,0x100}; // Text
const TMPXAttributeData KMPXMediaMusicOriginalAlbumArtFileName={KMPXMediaIdMusic,0x200}; // Text
const TMPXAttributeData KMPXMediaMusicAlbumArtThumbnail={KMPXMediaIdMusic,0x400}; // CFbsImage
const TMPXAttributeData KMPXMediaMusicAlbumArtThumbnailSize={KMPXMediaIdMusic,0x800}; // TSint
const TMPXAttributeData KMPXMediaMusicAlbumArtChanged={KMPXMediaIdMusic,0x1000}; // TInt
const TMPXAttributeData KMPXMediaMusicAlbumArtist={KMPXMediaIdMusic,0x2000}; // Text
const TMPXAttributeData KMPXMediaMusicAlbumId={KMPXMediaIdMusic,0x4000}; // TMPXItemId
const TMPXAttributeData KMPXMediaMusicAll={KMPXMediaIdMusic,0xffffffff}; // Text

/** 
*  Attributes supported for KMPXMediaIdMusic.
*
*  @deprecated Use the above consts
*/
enum
    {
    EMPXMediaMusicAlbumArtFileName=0x01, // Text
    EMPXMediaMusicArtist=0x02, // Text
    EMPXMediaMusicAlbum=0x04, // Text
    EMPXMediaMusicYear=0x08, // TInt64
    EMPXMediaMusicAlbumTrack=0x10, // Text
    EMPXMediaMusicGenre=0x20, // Text
    EMPXMediaMusicComposer=0x40, // Text
    EMPXMediaMusicRating=0x80, // TInt
    EMPXMediaMusicURL=0x100,         // Text
    EMPXMediaMusicOriginalAlbumArtFileName=0x200,    // Text
    EMPXMediaMusicAlbumArtThumbnail=0x400,
    EMPXMediaMusicAlbumArtThumbnailSize=0x800,
    EMPXMediaMusicAlbumArtist=0x2000,
    EMPXMediaMusicAlbumId=0x4000,
    EMPXMediaMusicAll=0xffffffff
    };


#endif // CMPXMEDIAMUSICDEFS_H
