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
* Description:  Definitions for message type
*
*/



#ifndef CMPXCOLLECTIONMESSAGEDEFS_H
#define CMPXCOLLECTIONMESSAGEDEFS_H


#include <e32base.h>
#include <mpxattribute.h>
/**
 * Item changed message
 */

const TInt KMPXMessageIdItemChanged = 0x10282945;

/**
 *  Attributes supported for KMPXMessageIdItemChanged
 */    
/**
 *  TUid. Collection id of the message originator
 */
const TMPXAttributeData KMPXMessageCollectionId={KMPXMessageIdItemChanged, 0x01};
/**
 * TMPXChangeEventType, change event type
 */    
const TMPXAttributeData KMPXMessageChangeEventType={KMPXMessageIdItemChanged, 0x02};
/**
 *  TMPXGeneralCategory, category of the media changed
 */
const TMPXAttributeData KMPXMessageMediaGeneralCategory={KMPXMessageIdItemChanged, 0x04};
/**
 *  TMPXItemId (as def'd in CollectionPath)
 *  ID of media changed
 */
const TMPXAttributeData KMPXMessageMediaGeneralId={KMPXMessageIdItemChanged, 0x08};
/**
 *  TMPXItemId (as def'd in CollectionPath)
 *  (Optional) Old ID of media if media is moved/renamed
 */
const TMPXAttributeData KMPXMessageMediaDeprecatedId={KMPXMessageIdItemChanged, 0x10};

/** 
 *  Attributes supported for KMPXMessageIdItemChanged
 *
 *  @deprecated please use constants above
 */    
enum 
    {
    /**
     *  TUid. Collection id of the message originator
     */
    EMPXMessageCollectionId=0x01, 
            
    /**
     * TMPXChangeEventType, change event type
     */    
    EMPXMessageChangeEventType = 0x02,
        
    /**
     *  TMPXGeneralCategory, category of the media changed
     */
    EMPXMessageMediaGeneralCategory=0x04, 
     
    /**
     *  TMPXItemId (as def'd in CollectionPath)
     *  ID of media changed
     */
    EMPXMessageMediaGeneralId=0x08, 
  
    /**
     *  TMPXItemId (as def'd in CollectionPath)
     *  (Optional) Old ID of media if media is moved/renamed
     */
    EMPXMessageMediaDeprecatedId=0x10
  
    };


//
// a) When a track is removed, the following messages will be sent:
//    1) EMPXItemDeleted/EMPXSong for the track removed
//    2) EMPXItemModified/EMPXArtist, EMPXItemModified/EMPXAlbum, ...etc.
//       for the affected artist/album/genre/composer where applicable
//    3) EMPXItemModified/EMPXPlaylist for the affected playlists where
//       applicable.
//
// b) When a track is added, EMPXItemInserted/EMPXSong will be sent
//    for the track added
//
// c) When a track is modified (including modifying its
//    artist/album/genre/composer), the following messages will be sent:
//    1) EMPXItemModified/EMPXSong for the track modified. If MTP renames
//       the track (i.e. changing the URI of the song), deprecated Id
//       of the track will be included in this message.
//    2) EMPXItemModified/EMPXArtist, EMPXItemModified/EMPXAlbum, ...etc.
//       for the affected artist/album/genre/composer where applicable.
//       If artist/album/genre/composer has been renamed, deprecated Id
//       will be included in this message.
//    3) EMPXItemModified/EMPXPlaylist for the affected playlists where
//       applicable.
//
// d) When a playlist removed, EMPXItemDeleted/EMPXPlaylist will be
//    sent for the playlist removed
//
// e) When a track is removed from a saved playlist,
//    EMPXItemModified/EMPXPlaylist will be sent
//
// f) When a playlist is added, EMPXItemInserted/EMPXPlaylist will be
//    sent for the playlist added
//
// g) When a track is appended to a playlist, EMPXItemModified/EMPXPlaylist
//    will be sent for the playlist modified
//
// h) When a playlist is modified, EMPXItemModified/EMPXPlaylist will be
//    sent. If the playlist is renamed, deprecated Id of the playlist
//    will be included in this message.
//
enum TMPXChangeEventType
    {
    EMPXItemDeleted,      // item/playlist deleted
    EMPXItemInserted,     // item/playlist added
    EMPXItemModified      // item/playlist/artist/album/genre/composer modified
    };


/**
 * A CMPXMessage to give additinal information about failures or events 
 * generated from collection plugins. This message can be customized
 * by a plugin to tell the UI about events
 */
const TInt KMPXCustomMessageId = 0x10282957;

/**
 *  Attributes supported for KMPXCustomMessageId
 */    
/**
 * The collection id that originated the message (TInt)
 */
const TMPXAttributeData KMPXCustomMessageCollectionId={KMPXCustomMessageId, 0x01};
/**
 * TInt, a generic event type such as TMPXCollectionServerOp
 * The interpretation of this attribute depends on the collection plugin.
 * 
 * For example, the Music DB plugin will return EMcsOpen, KErrOutOfDisk
 * if the plugin cannot merge the database because of low disk space
 */
const TMPXAttributeData KMPXCustomMessageEventType={KMPXCustomMessageId, 0x02};
/**
 * TInt, standard symbian error codes
 */
const TMPXAttributeData KMPXCustomMessageErrorCode={KMPXCustomMessageId, 0x04};

/**
 *
 *  @deprecated please use constants above
 *
 */
 
enum
    {
    /**
     * The collection id that originated the message (TInt)
     */
    EMPXCustomMessageCollectionId=0x01,
    
    /**
     * TInt, a generic event type such as TMPXCollectionServerOp
     * The interpretation of this attribute depends on the collection plugin.
     * 
     * For example, the Music DB plugin will return EMcsOpen, KErrOutOfDisk
     * if the plugin cannot merge the database because of low disk space
     */
    EMPXCustomMessageEventType=0x02,
    
    /**
     * TInt, standard symbian error codes
     */
    EMPXCustomMessageErrorCode=0x04
    
    };
#endif // CMPXCOLLECTIONMESSAGEDEFS_H

// End of file
