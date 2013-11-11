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
* Description:  Collection Helper with Caching
*
*/


#ifndef CMPXCOLLECTIONCACHEDHELPER_H
#define CMPXCOLLECTIONCACHEDHELPER_H

#include <f32file.h>
#include <apgcli.h>
#include "mpxcollectionhelperimp.h"
#include "mpxitemid.h"
#include <mpxcollectiontype.h>

// FORWARD DECLARATION
class CMPXMediaArray;
class CMPXMetadataExtractor;

/**
 *  Extended collection helper with caching capabilities
 *
 *  @lib mpxcollectionhelper
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CMPXCollectionCachedHelper ) : public CMPXCollectionHelperImp
    {

public:

    static CMPXCollectionCachedHelper* NewL();

    static CMPXCollectionCachedHelper* NewLC();

    virtual ~CMPXCollectionCachedHelper();

protected:

    /**
    * Add a song or a playlist to the cache. This method will leave if
    * failed to add it to the cache. If this song or playlist fails to
    * be added to the collection when the cache is committed, client is
    * NOT notified. Failure to provide the required attributes will
    * result in such a failure.    
    *
    * @param aMedia, media to be added.
    *
    * To add a song to the collection, the media provided MUST
    * contain the following attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXSong
    *
    *    KMPXMediaGeneralUri:
    *       full path of the song. Only supporting local path, i.e. path
    *       must starts with a drive letter.
    * 
    *    Client must provide the metadata by filling other attributes. This
    *    song will be registered with Harvester so that it will not be
    *    unnecessarily scanned during the next scan/refresh.
    *
    * To add a playlist to the collection, the media provided MUST
    * contain the following attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXPlaylist
    *
    *    KMPXMediaGeneralUri:
    *       Provide full path of the playlist if there is an associated
    *       physical playlist file.; otherwise, provide the path and
    *       directory where the playlist should be located (e.g. e:\playlists\).
    *       In the later case, playlist will be created in the collection
    *       as a virtual playlist and URI will be generated. If client is
    *       interested in this generated URI, client can perform a search
    *       based on the returned Id. Only supporting local path, i.e. path
    *       must starts with a drive letter.
    *
    *    KMPXMediaGeneralTitle:
    *       Title of the playlist is NOT enforced be unique within the
    *       music collection.
    *
    *    KMPXMediaArrayCount:
    *    KMPXMediaArrayContents:
    *       This playlist can be an empty playlist. If this is the case,
    *       KMPXMediaArrayContents will be a CMPXMediaArray that doesn't
    *       contain any media and KMPXMediaArrayCount will be 0.
    *
    *       Each CMPXMedia contained in the media array represents
    *       a song. Each song must contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXSong
    *
    *       For songs that already exist in the collection, client
    *       must provide either KMPXMediaGeneralId or KMPXMediaGeneralUri.
    *
    *       For songs that do not exist in the collection, client
    *       must provide KMPXMediaGeneralUri and its metadata.
    *       These songs will be added to the music collection when
    *       the playlist is added. These songs added to the collection
    *       are NOT registered with Harvester; hence, during next
    *       scan/refresh, these songs will be scanned and updated in
    *       the collection unnecessarily and cause scan/refresh
    *       performance degradation. If any metadata of these songs is
    *       changed in the collection before next scan/refresh, it will
    *       be lost after scan/refresh.
    *
    *       WARNING:
    *       Client must add the songs to the collection prior to adding
    *       the playlist.
    *
    *    WARNING:
    *    This playlist is NOT registered with Harvester. If client
    *    creates a physical playlist file, Harvester will re-parse
    *    and update the playlist in the collection during the next
    *    scan/refresh unnecessarily and cause scan/refresh performance
    *    degradation. If this playlist is edited, the physical
    *    playlist file is NOT updated to reflect the changes.
    *
    * This API does not support adding songs to a saved playlist.
    *
    */
    void AddL( CMPXMedia* aMedia );
    
    /**
    * Remove a song or a playlist from the collection. The correlating
    * file is not removed from the file system but this file is
    * de-registered with Harvester; hence, if the file isn't deleted
    * from the file system before the next scan/refresh, it will be
    * added back to the collection when scan/refresh is performed.
    *
    * This API implementation is dedicated to MTP. Records in the 
    * collection will be deleted instead of being marked as deleted.
    *
    * @param aFile, file to remove.
    * @param aItemCat, category of the media to be removed. Either
    *        EMPXSong or EMPXPlaylist. Other categories, e.g. EMPXArtist,
    *        EMPXAlbum, EMPXGenre, and EMPXComposer cannot be removed
    *        from the collection with this method, they need to be removed
    *        from the collection via collection path.
    */
    void RemoveL( const TDesC& aFile, TMPXGeneralCategory aItemCat = EMPXSong  ); //lint !e1735

    /**
    * Delete all records that have been marked as deleted.
    *
    * This API implementation is dedicated to MTP.
    */
    void CleanupDeletedMediasL();

    /**
    * Add an updated song or playlist to the cache. This method will leave if
    * failed to add it to the cache. If this song or playlist fails to
    * be updated in the collection when the cache is committed, client is
    * NOT notified. Failure to provide the required attributes will
    * result in such a failure.    
    *
    * For performance consideration, it is recommended that client only
    * sets the attribute that needs to be updated.
    *
    * @param aMedia, media to be updated.
    *
    *        To update a song in the collection, the media provided MUST
    *        contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXSong
    *
    *           KMPXMediaGeneralUri:
    *               full path of the song. Only supporting local path,
    *               i.e. path must starts with a drive letter.
    *
    *        Additionally, this media should provide one or more of the
    *        following attributes for update:
    *               KMPXMediaGeneralTitle
    *               KMPXMediaGeneralComment
    *               KMPXMediaGeneralSynchronized
    *               KMPXMediaGeneralDeleted
    *               KMPXMediaGeneralModified
    *               KMPXMediaGeneralCopyright
    *               KMPXMediaGeneralDuration
    *               KMPXMediaGeneralFlags
    *               KMPXMediaGeneralPlayCount
    *               KMPXMediaGeneralLastPlaybackTime
    *           KMPXMediaIdMusic:
    *               KMPXMediaMusicArtist
    *               KMPXMediaMusicAlbum
    *               KMPXMediaMusicAlbumTrack
    *               KMPXMediaMusicGenre
    *               KMPXMediaMusicComposer
    *               KMPXMediaMusicYear
    *               KMPXMediaMusicRating
    *               KMPXMediaMusicAlbumArtFileName
    *               KMPXMediaMusicURL
    *           KMPXMediaIdAudio:
    *               KMPXMediaAudioSamplerate
    *               KMPXMediaAudioBitrate
    *               KMPXMediaAudioNumberOfChannels
    *           KMPXMediaIdDrm:
    *               KMPXMediaDrmType
    *           KMPXMediaIdMTP:
    *               KMPXMediaMTPDrmStatus
    *
    *        To update a playlist in the collection, the media provided MUST
    *        contain the following attributes:
    *           KMPXMediaGeneralType:
    *               must be EMPXItem
    *
    *           KMPXMediaGeneralCategory:
    *               must be EMPXPlaylist
    *
    *           KMPXMediaGeneralUri:
    *               full path of the playlist. Only supporting local path,
    *               i.e. path must starts with a drive letter.
    *
    *        If the media contains KMPXMediaArrayContents attribute, the
    *        content of the playlist will be overwritten with the new media
    *        array. If this attribute contains a CMPXMediaArray of 0 element,
    *        all songs in this playlist will be removed from the playlist
    *        (songs will remain in the collection and file system).
    *        e.g.
    *        1) "Playlist" originally contains "Song1", "Song2", and "Song3".
    *           If KMPXMediaArrayContents contains a CMPXMediaArray that
    *           contains "Song3" and "Song4", "Playlist" will be updated to
    *           contain "Song3" and "Song4".
    *        2) "Playlist" originally contains "Song1", "Song2", and "Song3".
    *           If KMPXMediaArrayContents contains a CMPXMediaArray that
    *           contains 0 element, "Playlist" will be updated so that it
    *           doesn't contain any songs. "Song1", "Song2", and "Song3"
    *           will remain in the collection.
    *
    *       If the media contains any of the following attributes, the
    *       playlist will be updated accordingly:
    *               KMPXMediaGeneralTitle
    *               KMPXMediaGeneralSynchronized
    *               KMPXMediaGeneralDeleted (DEPRECATED)
    *               KMPXMediaGeneralModified (DEPRECATED)
    *
    *       NOTE: If a client wishes to change the playlist title and modify
    *             its contents (i.e. the songs it includes), client needs to
    *             perform two separate SetL operations; one to update the
    *             playlist title providing KMPXMediaGeneralTitle attribute
    *             and the other to update its contents by providing
    *             KMPXMediaArrayContents.
    *
    */
    void SetL( CMPXMedia*& aMedia );

    /**
    * Changes the URI of a song or a playlist.
    *
    * @param aOldUri exiting URI of the song or playlist
    * @param aNewUri of the song or playlist
    *
    *   If a client requests the URI of a song to be changed, the corresponding
    *   song file will NOT be moved to the new location. Harvester's file registration
    *   will be updated, as well as the collection. Client MUST ensure that there
    *   is not already a file at the specified new location in the file system
    *   (Same restrictions as RFs::Rename) and the collection.
    *
    *   WARNING: For virtual playlists (i.e. playlists created from the device.
    *            They are referred to as virtual because there is no physical
    *            playlist file created), client MUST ensure the new URI doesn't
    *            clash with any existing playlist in the collection, as well as
    *            in the file system. Changing their filenames is NOT recommended.
    *            If a client wishes to change the extension of the virtual playlist,
    *            it's recommended the extension is changed to an extension that's
    *            not supported by any playlist plugin exists in the system.
    *
    *   Upon successful completion of this operation, the Id of the song/playlist
    *   will be changed in the collection. If client is interested in the Id of
    *   this media hereafter, GetL or FindAllL can be performed (search by new URI).
    *
    * @param aItemCat category of the file, either EMPXSong or EMPXPlaylist;
    *        otherwise, this method leaves with KErrArgument if the media
    *        isn't currently in the cache. If the media is currently in the
    *        cache, the media will fail to be added/updated in the collection
    *        and client is NOT notified.
    */
    void RenameL( const TDesC& aOldUri,
                  const TDesC& aNewUri,
                  TMPXGeneralCategory aItemCat );
        
    /**
    * Gets the media for the song or playlist via its URI
    *
    * WARNING: Subsequent calls to this method will void the
    *          returned object reference. For example, client
    *          should avoid the following:
    *          CMPXMedia& song1 = GetL(***);
    *          CMPXMedia& song2 = GetL(***);
    *          song1.IsSupported(***);
    *          song2.IsSupported(***);
    *
    * @param aFile, path of the song/playlist
    *
    * @param aItemCat category of the file, either EMPXSong or
    *        EMPXPlaylist; otherwise, this method leaves with
    *        KErrArgument.
    *
    * @return media object representing the given file with
    *         the following attributes:
    *           KMPXMediaGeneralTitle
    *           KMPXMediaGeneralDate
    *           KMPXMediaGeneralDuration
    *           KMPXMediaGeneralComment    
    *           KMPXMediaMusicArtist
    *           KMPXMediaMusicAlbum
    *           KMPXMediaMusicAlbumTrack
    *           KMPXMediaMusicComposer
    *           KMPXMediaMusicYear
    *           KMPXMediaMusicGenre
    *           KMPXMediaMTPDrmStatus
    *           KMPXMediaAudioNumberOfChannels
    *           KMPXMediaAudioBitrate
    *           KMPXMediaAudioSamplerate
    *           KMPXMediaAudioAudioCodec
    *
    *         Note that the playlist media object does not
    *         contain songs included in the playlist. If client
    *         is interested in finding the songs in the playlist,
    *         FindAllL can be used (please see #2 in FindAllL).
    */
    const CMPXMedia& GetL( const TDesC& aFile, 
                           TMPXGeneralCategory aItemCat );

    /**
    * Find media(s) in music collection.
    *
    * @param aCriteria, criteria to search upon
    *
    * 1) To find all songs in the collection, here are the mandatory attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXGroup
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXSong
    *
    *    If the following optional attributes are provided, the search results
    *    are narrowed down further: (these attributes can be combined)
    *       KMPXMediaGeneralTitle
    *           ==> songs with the specified title
    *       KMPXMediaGeneralDrive
    *           ==> songs in the specified drive
    *       KMPXMediaGeneralSynchronized
    *           ==> songs that are/are not synchronized through MTP
    *       KMPXMediaGeneralDeleted
    *           ==> songs that are/are not marked as deleted since the last MTP
    *               session
    *       KMPXMediaGeneralModified
    *           ==> songs that are/are not modified since the last MTP session
    *                   
    * 2) To find all songs for a particular artist/album/genre/composer/playlist
    *    in the collection, here are the mandatory attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXGroup
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXSong
    *
    *    KMPXMediaIdGeneralId:
    *       the Id of the artist/album/genre/composer/playlist
    *
    *    NOTE: Songs in an album are sorted by album track, then title, except
    *          for songs in the unknown album. Songs in the unknown album are
    *          sorted by title.
    *  
    * 3) To find all songs for a particular album of an artist in the collection,
    *    here are the mandatory attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXGroup
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXSong
    *
    *    KMPXMediaIdGeneralId:
    *       the Id of the album
    *
    *    KMPXMediaGeneralContainerId:
    *       the Id of the artist
    *  
    *    NOTE: These songs are sorted by album track, then title, except
    *          for songs in the unknown album. Songs in the unknown album are
    *          sorted by title.
    *
    * 4)To find a particular song, here are the mandatory attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXSong
    *
    *    And one or all of the following attributes:
    *       KMPXMediaGeneralUri
    *       KPXMediaIdGeneralId
    *
    * 5) To find all artists/albums/genres/composers/playlists in the
    *    collection, here are the mandatory attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXGroup
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXArtist for artists
    *               EMPXAlbum for albums
    *               EMPXGenre for genres
    *               EMPXComposer for composers
    *               EMPXPlaylist for playlists
    *
    *    If the following optional attributes are provided for playlists,
    *    the search results are narrowed down further: (these attributes
    *    can be combined)
    *       KMPXMediaGeneralDrive
    *           ==> playlists in the specified drive
    *       KMPXMediaGeneralSynchronized
    *           ==> playlists that are/are not synchronized through MTP
    *       KMPXMediaGeneralDeleted (DEPRECATED)
    *           ==> playlists that are/are not marked as deleted since the last MTP
    *               session
    *       KMPXMediaGeneralModified (DEPRECATED)
    *           ==> playlists that are/are not modified since the last MTP session
    *    
    *    NOTE: results returned do not contain songs included in
    *          the category. To obtain songs included in the category,
    *          please refer to 2 and/or 3.
    *
    * 6) To find a particular playlist in the collection, here are the mandatory
    *    attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXPlaylist
    *
    *    And one or all of the following attributes:
    *       KMPXMediaGeneralUri
    *       KMPXMediaGeneralTitle
    *       KMPXMediaIdGeneralId
    *
    *    NOTE: results returned do not contain songs included in the playlist.
    *          To obtain songs included in the playlist, please refer to 2.
    *
    * 7) To find a particular artist/album/genre/composer in the collection, here
    *    are the mandatory attributes:
    *    KMPXMediaGeneralType:
    *       must be EMPXItem
    *
    *    KMPXMediaGeneralCategory:
    *       must be EMPXArtist for artist
    *               EMPXAlbum for album
    *               EMPXGenre for genre
    *               EMPXComposer for composer
    *
    *    And one or all of the following attributes:
    *       KMPXMediaGeneralTitle
    *       KMPXMediaIdGeneralId
    *    
    *    NOTE:
    *    a) results returned do not contain songs included in the category.
    *       To obtain songs included in the category, please refer to 2.
    *    b) unknown artist/album/genre/composer will be the last item in
    *       the result array
    *
    * @param aAttrs, attributes to return
    *        Supported attributes for songs are:
    *           KMPXMediaIdGeneral:
    *               KMPXMediaGeneralId
    *               KMPXMediaGeneralTitle
    *               KMPXMediaGeneralUri
    *               KMPXMediaGeneralDrive
    *               KMPXMediaGeneralDuration
    *               KMPXMediaGeneralDate
    *               KMPXMediaGeneralComment
    *               KMPXMediaGeneralMimeType
    *               KMPXMediaGeneralSynchronized
    *               KMPXMediaGeneralDeleted
    *               KMPXMediaGeneralModified
    *               KMPXMediaGeneralCopyright
    *               KMPXMediaGeneralFlags
    *               KMPXMediaGeneralPlayCount
    *               KMPXMediaGeneralLastPlaybackTime
    *           KMPXMediaIdMusic:
    *               KMPXMediaMusicArtist
    *               KMPXMediaMusicAlbum
    *               KMPXMediaMusicAlbumTrack
    *               KMPXMediaMusicGenre
    *               KMPXMediaMusicComposer
    *               KMPXMediaMusicYear
    *               KMPXMediaMusicRating
    *               KMPXMediaMusicAlbumArtFileName
    *               KMPXMediaMusicURL
    *           KMPXMediaIdAudio:
    *               KMPXMediaAudioSamplerate
    *               KMPXMediaAudioBitrate
    *               KMPXMediaAudioNumberOfChannels
    *               KMPXMediaAudioCodec
    *           KMPXMediaIdMTP:
    *               KMPXMediaMTPDrmStatus
    *
    *        Supported attributes for playlists are:
    *           KMPXMediaIdGeneral:
    *               KMPXMediaGeneralId
    *               KMPXMediaGeneralCollectionId
    *               KMPXMediaGeneralTitle
    *               KMPXMediaGeneralUri
    *               KMPXMediaGeneralDrive
    *               KMPXMediaGeneralSynchronized
    *               KMPXMediaGeneralDeleted (DEPRECATED)
    *               KMPXMediaGeneralModified (DEPRECATED)
    *               KMPXMediaGeneralFlags
    *               KMPXMediaGeneralCount
    *               KMPXMediaGeneralDate
    *
    *        Supported attributes for artists/albums/genres/composers are:
    *           KMPXMediaIdGeneral:
    *               KMPXMediaGeneralId
    *               KMPXMediaGeneralTitle    
    *
    * @return search results. The returned media contains the following
    *         attributes:
    *           KMPXMediaGeneralType:
    *               EMPXGroup
    *
    *           KMPXMediaGeneralCategory:
    *               the category of the found media(s), e.g. EMPXSong for
    *               scenarios 1-4.
    *
    *           KMPXMediaArrayCount:
    *               the number of matching media(s). 0 if no match found.
    *
    *           KMPXMediaArrayContents:
    *               this array contains the found media(s). If client
    *               was looking for songs, each media in the array
    *               represents a song, similarly for artists, albums, genres,
    *               composers, and playlists. Each media contains
    *                   KMPXMediaGeneralType:
    *                       EMPXItem
    *                   KMPXMediaGeneralCategory:
    *                       category of the found media
    *                   
    *                   The rest of the attributes returned are as specified
    *                   by the aAttrs parameter.
    */
    CMPXMedia* FindAllL( CMPXMedia& aCriteria, 
                         const TArray<TMPXAttribute>& aAttrs );
                                 
    /**
    * Frees this object
    */
    void Close();

protected: 
    
    /**
    * Commits the transactions still left in the caching array
    * for abstractAlbum, it would be left in cache if aCacheAbstractAlbum is set to ETrue
    * @param aCacheAbstractAlbum whether abstractAlbum is to be cached
    */
    void Commit(TBool aCacheAbstractAlbum = EFalse);

private: 
    
    /**
    * Appends General Media Type information
    * @param aSrc source media to copy attributes from
    * @param aDestination destination media to copt attributes to
    */
    void DoAppendGeneralL( CMPXMedia& aSrc, CMPXMedia& aDestination );
            
    /**
    * Appends Music Type information
    * @param aSrc source media to copy attributes from
    * @param aDestination destination media to copt attributes to
    */
    void DoAppendMusicL( CMPXMedia& aSrc, CMPXMedia& aDestination );
    
    /**
    * Appends Audio Type information
    * @param aSrc source media to copy attributes from
    * @param aDestination destination media to copt attributes to
    */
    void DoAppendAudioL( CMPXMedia& aSrc, CMPXMedia& aDestination );
    
    /**
    * Appends DRM Type information
    * @param aSrc source media to copy attributes from
    * @param aDestination destination media to copt attributes to
    */
    void DoAppendDRML( CMPXMedia& aSrc, CMPXMedia& aDestination );

    /**
    * Appends array information
    * @param aSrc source media to copy attributes from
    * @param aDestination destination media to copt attributes to
    */
    void DoAppendContainerL( CMPXMedia& aSrc, CMPXMedia& aDestination );

    /**
    * Appends MTP information
    * @param aSrc source media to copy attributes from
    * @param aDestination destination media to copt attributes to
    */
    void DoAppendMTPL( CMPXMedia& aSrc, CMPXMedia& aDestination );
    
    /**
    * Attempt to set missing Metadata info in specific case
    * @param aMedia media object to be checked and adding the info if needed
    */
    void SetMissingMetadataL(CMPXMedia* aMedia);

#ifdef RD_MPX_COLLECTION_CACHE

    /*
    * Gets the media for the song or playlist via its URI
    * Note. This is used in conjunction with exported GetL
    * This method does not perform cache capability but only returns
    * the media object requested.
    *
    * @param aFile, path of the song/playlist
    *
    * @param aItemCat category of the file, either EMPXSong or
    *        EMPXPlaylist; otherwise, this method leaves with
    *        KErrArgument.
    *
    * @return void 
    *         the result of iFoundMedia media object representing the given file with
    *         the following attributes:
    *           KMPXMediaGeneralTitle
    *           KMPXMediaGeneralDate
    *           KMPXMediaGeneralDuration
    *           KMPXMediaGeneralComment    
    *           KMPXMediaMusicArtist
    *           KMPXMediaMusicAlbum
    *           KMPXMediaMusicAlbumTrack
    *           KMPXMediaMusicComposer
    *           KMPXMediaMusicYear
    *           KMPXMediaMusicGenre
    *           KMPXMediaMTPDrmStatus
    *           KMPXMediaAudioNumberOfChannels
    *           KMPXMediaAudioBitrate
    *           KMPXMediaAudioSamplerate
    */
    void GetSongL( const TDesC& aFile, TMPXGeneralCategory aItemCat, TUid aCollectionUId );
    
    /*
    * Gets the artist id of the media object inthe currect context (result of GetL())
    *
    * @param aCollectionUid TUid
    *
    * @return void 
    *         the result of iFoundMedia media object representing the given file with
    */
    TMPXItemId GetArtistIdL( const TDesC& aArtist, TUid aCollectionUId );
    
    
    /*
    * Gets all the songs that are belongs to the same artist id supplied
    *
    * @param id  TMPXItemId indicates the id for requesting artist
    * @param aCollectionUid TUid
    * @param aUnknownArtist TBool indicates if this is for unknown artist or not
    *
    * @return void 
    *         the result of iCachedArtistArray media object
    */
    void GetSongsL( TMPXItemId id, TUid aCollectionUId, TBool aUnknownArtist );

#endif //RD_MPX_COLLECTION_CACHE

    
private:
    
    enum TCachedOp
        {
        EAdd,
        ESet
        };
                
private:

    CMPXCollectionCachedHelper();

    void ConstructL();

private: // data

    CMPXMedia*        iFoundMedia;
    CMPXMediaArray*   iCache;
    RArray<TCachedOp> iOp;
#ifdef RD_MPX_COLLECTION_CACHE
    CMPXMediaArray*   iCachedArtistArray; 
    CMPXMediaArray*   iCachedUnknownArtistArray; 
    TBool             iNotInCache;
    TInt iHitFoundMedia;
    TInt iNotHitInCache;
    TInt iLookingInCache;
    TInt iLookingInUnknowCache;
#endif //RD_MPX_COLLECTION_CACHE    
    CMPXMetadataExtractor* iMetadataExtractor;
    RFs                    iFs;
    RApaLsSession          iAppArc;
    RPointerArray<CMPXCollectionType> iSupportedTypes;
    };

#endif // C_MPXCOLLECTIONCACHEDHELPER_H
