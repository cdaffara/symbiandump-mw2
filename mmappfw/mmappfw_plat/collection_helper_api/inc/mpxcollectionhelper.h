/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Collection helper abstract interface
*
*/



#ifndef MMPXCOLLECTIONHELPER_H
#define MMPXCOLLECTIONHELPER_H

#include <mpxmediageneraldefs.h>
#include <mpxcollectionhelperobserver.h>

// FORWARD DECLARATIONS
class CMPXMedia;
class TMPXAttribute;

/**
 *  Abstract class for collection helper. Use this class to add or remove songs and playlists from
 *  Music Player and Podcast app.
 *
 *  @lib mpxcollectionhelper.lib
 */
class MMPXCollectionHelper
    {

public:
    /**
    * Add an item based on file path and use the harvester to parse the file.
    * On completion MMPXCollectionHelperObserver::HandleAddFileCompleteL() 
    * is called back.
    *
 	  * @since S60 3.2.3
    * @param aFile full path and name of the file.
    * @param aObserver observer for the add operation
    */
    virtual void AddL( const TDesC& aFile, 
                       MMPXCollectionHelperObserver* aObserver ) = 0;

    /**
    * Add an item (song or playlist) with some media properties registers with playlists.
 	  * @since S60 3.2.3
    * @param aMedia media property to add.
    * @leave KErrArgument leave if the given media doesn't contain the correct attributes
    */
    virtual void AddL( CMPXMedia* aMedia ) = 0;

    /**
    * Remove a song, playlist, m3u file from the collection and from the harvester database.
 	  * @since S60 3.2.3
    * @param aFile file to remove.
    * @param aItemCat type of the item (optional).
    * @leave KErrArgument leave if the file to be removed does not exist
    */
    virtual void RemoveL( const TDesC& aFile, TMPXGeneralCategory aItemCat = EMPXSong) = 0; //lint !e1735

    /**
    * Remove all entries from the music, podcast and harvester databases.
 	  * @since S60 3.2.3
    */
    virtual void RemoveAllL() = 0;
    
    /**
    * Delete all records that have been marked as deleted.
 	  * @since S60 3.2.3
    * @leave KErrNotSupported Virtual Void function that will leave if called and not implemented
    */
    virtual void CleanupDeletedMediasL() = 0;
       
    /**
    * Updates the metadata for a file based on the path.
 	  * @since S60 3.2.3
    * @param aMedia media item to update.
    * @leave KErrArgument leave if the given media doesn't contain the correct attributes
    * 		or leave if not updating a song or a playlist
    */
    virtual void SetL( CMPXMedia*& aMedia ) = 0;

    /**
    * Renames a file.
 	  * @since S60 3.2.3
    * @param aOldUri existing URI of the file to be renamed.
    * @param aNewUri new URI of the file.
    * @param aItemCat category of the file either EMPXSong
    *        or EMPXPlaylist.
    * @leave KErrArgument leave if not updating a song or a playlist
    */
    virtual void RenameL( const TDesC& aOldUri,
                          const TDesC& aNewUri,
                          TMPXGeneralCategory aItemCat = EMPXSong ) = 0; //lint !e1735
    
    /**
    * Gets the metadata for a file.
 	  * @since S60 3.2.3
    * @param aFile path to the file.
    * @param aAttrs attributes to return.
    * @param aItemCat category of the file either EMPXSong
    *        or EMPXPlaylist.
    * @return Media object representing the given file with
    *         the requested attributes.    
    * @leave KErrArgument leave if not updating a song or a playlist.
    * @leave KErrNoMemory leave if search array is empty.
    * @leave KErrNotFound leave if array indexcount is 0.
    * @leave KErrCorrupt default case if the above conditions are not satisfied 
    * 		and object is not found
    */
    virtual CMPXMedia* GetL( const TDesC& aFile, 
                             const TArray<TMPXAttribute>& aAttrs,
                             TMPXGeneralCategory aItemCat = EMPXSong ) = 0; //lint !e1735

    /**
    * Gets the metadata for a file.
 	  * @since S60 3.2.3
    * @param aFile path to the file.
    * @param aItemCat category of the file either EMPXSong
    *        or EMPXPlaylist.
    * @return Media object representing the given file.
    * @leave KErrNotSupported Virtual const function that will leave if called and not implemented
    */
    virtual const CMPXMedia& GetL( const TDesC& aFile, 
                                   TMPXGeneralCategory aItemCat = EMPXSong ) = 0; //lint !e1735
        
    /**
    * Generic find function.
 	  * @since S60 3.2.3
    * @param aCriteria criterias to search upon.
    * @param aAttrs attributes to return.
    * @return Search results. found media(s) are accessible
    *         through KMPXMediaIdContainer/EMPXMediaArrayContents
    *         attribute    
    */
    virtual CMPXMedia* FindAllL( CMPXMedia& aCriteria, 
                                 const TArray<TMPXAttribute>& aAttrs) = 0;
    
    /**
    * Close the helper. 
    * @since S60 3.2.3
    */                
    virtual void Close() = 0;
    };
       
#endif // MMPXCOLLECTIONHELPER_H
