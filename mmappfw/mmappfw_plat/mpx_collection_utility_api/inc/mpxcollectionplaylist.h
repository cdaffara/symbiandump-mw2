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
* Description:  Encapsulates a 'playlist' from a collection
*
*/

 

#ifndef CMPXCOLLECTIONPLAYLIST_H
#define CMPXCOLLECTIONPLAYLIST_H

#include <s32strm.h>
#include <mpxattribute.h>
#include <mpxcollectionobserver.h>

class CMPXCollectionPath;
class MMPXCollectionUtility;
class RReadStream;
class MMPXCollectionPlaylistObserver;
class CMPXCollectionOpenUtility;
/**
 *  Encapsulates a playlist
 *
 *  @lib mpxcollectionutility.lib
 */
class CMPXCollectionPlaylist : public CBase, public MMPXCollectionObserver
    {
public: // Data type    
    enum TRepeatMode
        {
        ERepeatOff, // No repeat
        ERepeatOne, // Play one track over and over again
        ERepeatAll  // Repeat the whole play list
        };
public:

   /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C static CMPXCollectionPlaylist* NewL();

   /**
    *  Two-phase constructor.
    *
    *  @since S60 3.2.3
    *  @param aPlaylist reference to an existing playlist
    *  @param aPlObs playlist observer
    *  @return object created
    */
    IMPORT_C static CMPXCollectionPlaylist* NewL(
                                const CMPXCollectionPlaylist& aPlaylist,
                                MMPXCollectionPlaylistObserver* aPlObs = NULL);

    /**
     *  Two-phase constructor.
     *
     *  @since S60 3.2.3
     *  @param aPath reference to a collection path
     *  @param aPlObs playlist observer
     *  @return object created
     */
    IMPORT_C static CMPXCollectionPlaylist* NewL(
                                const CMPXCollectionPath& aPath,
                                MMPXCollectionPlaylistObserver* aPlObs = NULL);

    /**
     *  Two-phase constructor.
     *
     *  @since S60 3.2.3
     *  @param aStream reference to a collection path
     *  @param aPlObs playlist observer
     *  @return object created
     */
    IMPORT_C static CMPXCollectionPlaylist* NewL(
                                  RReadStream& aStream,
                                  MMPXCollectionPlaylistObserver* aPlObs = NULL);

    /**
     *  Destructor.
     *
     *  @since S60 3.2.3
     */
    IMPORT_C virtual ~CMPXCollectionPlaylist();
    
public: //Iterate through the playlist

    /**
     *  Copy content from other playlist.
     *
     *  @since S60 3.2.3
     *  @param aPlaylist reference to an existing playlist
     */
    IMPORT_C void CopyContentL(const CMPXCollectionPlaylist& aPlaylist);
    
    /**
     *  Next item.
     *
     *  @since S60 3.2.3
     *  @param aIgnoreRepeat ETrue to ignore repeat setting
     *  @return ETrue if exists
     */
    IMPORT_C TBool Next( TBool aIgnoreRepeat=EFalse );

    /**
     *  Gets the next index to play.  This method should not change
     *  any of the internal index values (const method).
     *
     *  @since S60 3.2.3
     *  @param aIgnoreRepeat ETrue to ignore repeat setting
     *  @param aPlaylistIndex Out Parameter - The next index. KErrNotFound if no valid index.
     *                                - This is the index in the playlist order.
     *  @return ETrue if exists
     */
    IMPORT_C TBool NextIndex( 
        TBool aIgnoreRepeat, 
        TInt& aPlaylistIndex ) const;

    /**
     *  Gets the path index of the item given it's position in the playlist.
     *
     *  @since S60 3.2.3
     *  @param aPlaylistIndex Index of item in the playlist
     *  @return The index of the item in the path
     */
    IMPORT_C TInt PathIndex( TInt aPlaylistIndex ) const;

    /**
     *  Previous item.
     *
     *  @since S60 3.2.3
     *  @param aIgnoreRepeat ETrue to ignore repeat setting
     *  @return ETrue if exists    
     */
    IMPORT_C TBool Previous( TBool aIgnoreRepeat=EFalse );
    
    /**
     *  Go to the first item.
     *
     *  @since S60 3.2.3
     */
    IMPORT_C void SetToFirst();

    /**
     *  Go to the last item.
     *
     *  @since S60 3.2.3
     */
    IMPORT_C void SetToLast();
    
    /**
     * Go to a particular item.
     *
     * @since S60 3.2.3
     * @param aIndex index to goto
     *               The index can be -1 for no selection and must be
     *               Smaller than the number of items in the playlist
     */
    IMPORT_C void CMPXCollectionPlaylist::SetToIndex( TInt aIndex );
    
public: // State information   
    /**
     * Number of items.
     *
     * @since S60 3.2.3
     * @return the number of items
     */
    IMPORT_C TInt Count() const;
    
    /**
     * Depth of the playlist.
     *
     * @since S60 3.2.3
     * @return the depth
     */
    IMPORT_C TInt Levels() const;
    
    /**
     * Index to current item in the playlist.
     *
     * @since S60 3.2.3
     * @return the index
     */
    IMPORT_C TInt Index() const;

    /**
     * Remote playlist.
     *
     * @since S60 3.2.3
     * @return ETrue remote, EFalse local
     */
    IMPORT_C TBool Remote() const;

    /**
     * Repeat mode.
     *
     * @since S60 3.2.3
     * @return the repeat mode
     */
    IMPORT_C TRepeatMode RepeatMode() const;

    /**
     * Shuffle mode.
     *
     * @since S60 3.2.3
     * @return ETrue shuffle on, EFalse shuffle off
     */
    IMPORT_C TBool Shuffle() const; 

    /**
     * Get collection path.
     *
     * @since S60 3.2.3
     * @return collection path
     */
    IMPORT_C const CMPXCollectionPath& Path() const;
    
    /**
     * Get extended media of current item.
     *
     * @since S60 3.2.3
     * @param aAttrs attributes requested
     * @param aMediaObs properties observer
     */    
    IMPORT_C void MediaL(const TArray<TMPXAttribute>& aAttrs,
                         MMPXCollectionMediaObserver& aMediaObs);
    
    /**
     * Set repeat mode.
     *
     * @since S60 3.2.3
     * @param aMode the repeat mode
     */
    IMPORT_C void SetRepeatMode(TRepeatMode aMode); 

    /**
     * Set repeat enabled.
     *
     * @since S60 3.2.3
     * @param aEnable To enable repeat mode or not
     */
    IMPORT_C void SetRepeatEnabled( TBool aEnable ); 

    /**
     * Turn shuffle mode on/off.
     *
     * @since S60 3.2.3
     * @param aShuffle new shuffle mode
     * @param aCurrentToTop move current item to the top
     */
    IMPORT_C void SetShuffleL(TBool aShuffle, TBool aCurrentToTop);
    
    /**
     * Set shuffle enabled.
     *
     * @since S60 3.2.3
     * @param aEnable To enable shuffle mode or not
     */
    IMPORT_C void SetShuffleEnabledL( TBool aEnable );
    
    /**
     * Externalize an object of this class to stream.
     *
     * @since S60 3.2.3
     * @param aStream write stream
     */
    IMPORT_C void ExternalizeL(RWriteStream& aStream) const;
    
    /**
     * Internalize an object of this class from stream.
     *
     * @since S60 3.2.3
     * @param aStream read stream
     */
    IMPORT_C void InternalizeL(RReadStream& aStream);
    
    /**
     * Set a new observer for current request.
     * Used when handling pre-initializing.
     *
     * @since S60 3.2.3
     * @param aMediaObs properties observer
     * @param aPlObs playlist observer
     * @param aCollectionObs collection observer
     */    
    IMPORT_C void SetObserver(MMPXCollectionMediaObserver& aMediaObs,
                              MMPXCollectionPlaylistObserver* aPlObs = NULL,
                              MMPXCollectionObserver* aCollectionObs = NULL);
    
    /**
     * Is this playlist launched from embedded mode.
     *
     * @since S60 3.2.3
     * @return ETrue if embedded mode
     */
    IMPORT_C TBool EmbeddedPlaylist() const;
    
    /**
     * Set or clear the embedded playlist flag.
     *
     * @since S60 3.2.3
     * @param aEmbedded is this playlist embedded?
     */
    IMPORT_C void SetEmbeddedPlaylist( TBool aEmbedded );
    
    /**
     *  Sets/updates the media for the item provided.
     *
     *  @since S60 3.2.3
     *  @param aMedia new value
     *  Callback via EMediaChanged message if currently opened
     */
    IMPORT_C void SetL( const CMPXMedia& aMedia );
    
    /**
     *  Stops any async operations that are currently under way.
     *
     *  @since S60 3.2.3
     */
    IMPORT_C void CancelRequest(); 
    
    /**
     *  Invalidate the playlist.
     *  
     */
    IMPORT_C void Invalidate();
    
    /**
     * AutoPlay
     *  
     * @since S60 TB9.2
     * @return the auto play value
     */
    IMPORT_C TBool AutoPlay() const;
    
    /**
     * Set auto play.
     * 
     * @since S60 TB9.2
     * @param aAutoPlay new value
     */
    IMPORT_C void SetAutoPlay(TBool aAutoPlay);
    
    
     /**
     * Single item playlist.
     *
     * @since S60 TB9.2
     * @return ETrue for single item, EFalse if not
     */
     IMPORT_C TBool IsSingleItemPlaylist();
    
    
    /**
     * Set single item playlist.
     *
     * @since S60 TB9.2
     */
     IMPORT_C void SetSingleItemPlaylist();
     
     /**
     * Pre-initialize next plugin.
     *
     * @since S60 TB9.2
     * @return ETrue for pre-init, EFalse if not
     */
     IMPORT_C TBool PreInitPlugin() const;

     /**
      * Set pre-initialize next plugin.
      *
      * @since S60 TB9.2
      * @param aPreInit new value
      */
      IMPORT_C void SetPreInitPlugin(TBool aPreInitPlugin);
      
private:  // From base class
    /**
     *  Handle media.
     *  From MMPXCollectionObserver / MMPXCollectionMediaObserver.
     *
     *  @since S60 3.2.3
     *  @param aMedia media properties
     *  @param aError error code
     */
    void HandleCollectionMediaL(const CMPXMedia& aMedia, TInt aError);
                                   
    /**
     *  Handle collection message.
     *  From MMPXCollectionObserver
     *
     *  @since S60 3.2.3
     *  @param aMessage collection message
     *  @param aErr system error code.
     */
    void HandleCollectionMessage(CMPXMessage* aMessage, TInt aErr);

    /**
     *  Handles the collection entries being opened. Typically called
     *  when client has Open()'d a folder.
     *  From MMPXCollectionObserver    
     *
     *  @since S60 3.2.3
     *  @param aEntries collection entries opened
     *  @param aIndex focused entry
     *  @param aComplete ETrue no more entries. EFalse more entries
     *                   expected
     *  @param aError error code   
     */
    void HandleOpenL(const CMPXMedia& aEntries,
                     TInt aIndex,TBool aComplete,TInt aError);
    
    /**
     *  Handles the item being opened. 
     *  Typically called when client has Open()'d an item. Client typically 
     *  responds by 'playing' the item via the playlist
     *  From MMPXCollectionObserver    
     *
     *  @since S60 3.2.3
     *  @param aPlaylist collection playlist
     *  @param aError error code   
     */                         
    void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError);

private:

    /**
     *  Restore ordinal after changes.
     *  
     *  @since S60 3.2.3
     *  @param aEntries collection entries opened
     *  @param aIndex index of current item 
     *  @leave IfNull 
     */
    void RestoreOrdinalL(const CMPXMedia& aEntries, TInt aIndex);
    
    /**
     *  Handle collection message.
     *
     *  @since S60 3.2.3
     *  @param aMessage collection message
     *  @param aErr system error code.
     *  @leave IfNull
     */
    void DoHandleCollectionMessageL(CMPXMessage* aMessage, TInt aErr);

    /**
     * Handle a CMPXCollection message with a change event.
     *
     * @since S60 3.2.3
     * @param aMessage single collection message to handle
     * @return ETrue if a re-open is needed, EFalse otherwise
     */
    TBool DoHandleCollectionChangeMessageL(CMPXMessage& aMessage);
    
    /**
     * Handle starting of fetch for incremental algorithm.
     *
     * @since S60 3.2.3
     */
    void DoIncrementalOpenL();
    
    /**
     * Do Restore Ordinal for incremental opens.
     *
     * @since S60 3.2.3
     * @param aMedia media returned from collection
     * @param aComplete OpenL Completed or not
     */
    void DoHandleIncompleteOpenL( const CMPXMedia& aMedia, TBool aComplete);
        
private:
    
    /**
     *  C++ default constructor.
     *  content not constructed yet, it may be internalized from buffer
     *
     *  @since S60 3.2.3
     *  @param aPlObs playlist observer
     */
    CMPXCollectionPlaylist(MMPXCollectionPlaylistObserver* aPlObs);
    
    /**
     * 2nd phase constructor.
     *
     * @since S60 3.2.3
     * @param aPath collection path
     */
    void ConstructL(const CMPXCollectionPath& aPath);
                                   
    /**
     * 2nd phase constructor.
     *
     * @since S60 3.2.3
     * @param aPlaylist playlist
     */
    void ConstructL(const CMPXCollectionPlaylist& aPlaylist);
    
    /**
     * 2nd phase contructor.
     *
     * @since S60 3.2.3
     * @param aStream stream
     */
    void ConstructL(RReadStream& aStream);

    /**
     * 2nd phase contructor.
     *
     *  @since S60 3.2.3
     */
    void ConstructL();
    
private:
    TRepeatMode iRepeatMode;
    TBool iShuffle;
    TBool iRemote;
    CMPXCollectionPath* iPath;
    RArray<TInt> iItemIndexes; // Indexes which define playlist items
    TInt iItemIndex; // current index to iItemIndexes array
    MMPXCollectionUtility* iColUtil;
    MMPXCollectionMediaObserver* iMediaObs;
    MMPXCollectionObserver* iCollectionObs;
    TBool iEmbedded;
    MMPXCollectionPlaylistObserver* iPlObs; 
    TBool iRepeatEnabled;
    TBool iShuffleEnabled;
    CMPXCollectionOpenUtility* iIncOpenUtil;
    TBool iReopenForChange;
    TBool iAutoPlaylist;
    TBool iAutoPlay; // Automatically play next item in playlist if in playing state. Default to TRUE for Music Player.
    TBool iSingleItemPlaylist;
    TBool iPreInitPlugin;
    };

#endif // CMPXCOLLECTIONPLAYLIST_H
